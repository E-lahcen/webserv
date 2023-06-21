/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:14:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/21 04:40:52 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Network.hpp>
#include <iostream>

const int Network::protocol = getprotobyname("tcp")->p_proto;

Network::Network(Servers &servers)
{
	initServersSockets(servers);
}

Network::~Network() {}

void Network::initServersSockets(Servers& servers)
{	
	for (Servers::iterator server = servers.begin();
		 server != servers.end(); ++server)
	{
		createListenSocket(server->socketFd, servers);
		makeServerListen(server, servers);
		addPollFd(server);
	}

	addServerFDListenCollection(servers);
	while (1)
	{
		try
		{
			pollMultiplexing(servers);
			handleNewConnections();
			handleActions();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

void Network::addServerFDListenCollection(Servers &servers)
{

	serverCollection.clear();

	Servers::const_iterator server;
	for (server = servers.begin();
		 server != servers.end(); ++server)
	{
		serverCollection.push_back(*server);
	}
}

void Network::handleNewConnections()
{
	for (size_t i = 0; i < serverCollection.size(); i++)
	{
		if (pollFds[i].revents & POLLIN)
		{
			Socket newSock = getNewConnectionSock(pollFds[i].fd);
			initPollFdClient(newSock, serverCollection[i]);
		}
	}
}

Socket Network::getNewConnectionSock(Socket listenSock)
{
	int enable = 1;
	Socket newSock = accept(listenSock, NULL, NULL);
	setsockopt(newSock, SOL_SOCKET, SO_NOSIGPIPE, &enable, sizeof(enable));
	if (newSock == -1)
		throwErrnoException("getNewConnectionSock() failed to accept new connection");

	makeFDNonBlock(newSock);
	return newSock;
}

void Network::createListenSocket(Socket &socketFd, Servers &servers)
{
	socketFd = socket(addrFamily, sockType, protocol);
	if (socketFd == -1)
	{
		clearServersSockets(servers);
		throwErrnoException("failed to create socket");
	}

	makeSocketReuseAddr(socketFd, servers);

	makeSocketNonBlocking(socketFd, servers);
}

void Network::makeSocketReuseAddr(const Socket &socketFd, Servers &servers)
{

	const int enable = 1;
	if (setsockopt(socketFd, SOL_SOCKET,
				   SO_REUSEADDR, &enable, sizeof(enable)))
	{
		clearServersSockets(servers);
		throwErrnoException("failed to make socket reuse address");
	}
}

void Network::makeSocketNonBlocking(const Socket &socketFd, Servers &servers)
{

	try
	{
		makeFDNonBlock(socketFd);
	}
	catch (const std::exception &error)
	{

		clearServersSockets(servers);
		throw error;
	}
}

void Network::makeServerListen(const ServerRef &server, Servers &servers)
{

	addrinfo *serverAddr = getServerAddrInfo(server, servers);

	if (bind(server->socketFd, serverAddr->ai_addr,
			 serverAddr->ai_addrlen) < 0)
	{
		std::string error = "failed to bind socket to ";
		error += server->hostname + ':' + server->port;
		clearServersSockets(servers);
		throwErrnoException(error);
	}

	if (listen(server->socketFd, SOMAXCONN) == -1)
	{

		std::string error = "failed to listen on ";
		error += server->hostname + ':' + server->port;
		clearServersSockets(servers);
		throwErrnoException(error);
	}
	freeServerAddrInfo(serverAddr);
}

addrinfo *Network::getServerAddrInfo(const ServerRef &server, Servers &servers)
{

	addrinfo hints;
	// clears hints
	bzero(&hints, sizeof(hints));
	hints.ai_flags = flags;
	hints.ai_family = addrFamily;
	hints.ai_socktype = sockType;
	hints.ai_protocol = protocol;

	addrinfo *resultedAddr = NULL;
	int successCode = getaddrinfo(server->hostname.c_str(),
								  server->port.c_str(), &hints, &resultedAddr);

	if (successCode)
	{
		clearServersSockets(servers);
		std::string error = "getServerAddrInfo failed for ";
		error += server->hostname + ':' + server->port;
		throwAddrInfoError(successCode, error);
	}

	return resultedAddr;
}

void Network::freeServerAddrInfo(addrinfo *addr)
{
	freeaddrinfo(addr);
}

void Network::clearServersSockets(Servers &servers)
{

	for (Servers::iterator server = servers.begin();
		 server != servers.end(); ++server)
	{

		close(server->socketFd);
		server->socketFd = -1;
	}
}

void Network::throwAddrInfoError(int errorCode, const std::string &errorMsg)
{

	std::string exceptionStr = errorMsg + ": ";
	exceptionStr += gai_strerror(errorCode);

	throw std::runtime_error(exceptionStr);
}

void Network::addPollFd(const ServerRef &server)
{
	struct pollfd poll;

	poll.fd = server->socketFd;
	poll.events = POLLIN;
	pollFds.push_back(poll);
}

void Network::pollMultiplexing(Servers &servers)
{
	int status;

	if ((status = poll(&pollFds[0], pollFds.size(), -1)) < 1)
	{
		clearServersSockets(servers);
		throw std::runtime_error("Issue in multiplexing using poll!");
	}
	signal(SIGPIPE, SIG_IGN);
}

void Network::initPollFdClient(Socket &socket, Server& server)
{
	struct pollfd 	clientPollFd;
	Client			client;

	clientPollFd.fd = socket;
	clientPollFd.events = POLLIN;
	
	client.mySocket = socket;
	client.myStage = REQUEST;
	client.myServer = server;
	
	pollFds.push_back(clientPollFd);
	clientCollection.push_back(client);
}

void Network::handleActions(void)   
{
	std::string test;

	for (size_t j = serverCollection.size(); j < pollFds.size(); j++)
	{
		if (pollFds[j].revents & POLLIN)
		{
			char buffer[MAX_BUFFER];
			ssize_t bytesRead = read(pollFds[j].fd, buffer, sizeof(buffer));
			if (bytesRead == -1)
			{
				clientCollection[j - serverCollection.size()].myStage = FINISH;
				close(pollFds[j].fd);
				pollFds.erase(pollFds.begin() + j);
				delete clientCollection[j - serverCollection.size()].myRequest;
				delete clientCollection[j - serverCollection.size()].myResponse;
				clientCollection.erase(clientCollection.begin() + j - serverCollection.size());
				--j;
				throw std::runtime_error("Issue in read() function!");
			}
			else if (bytesRead == 0)
			{
				clientCollection[j - serverCollection.size()].myStage = FINISH;
				close(pollFds[j].fd);
				pollFds.erase(pollFds.begin() + j);
				delete clientCollection[j - serverCollection.size()].myRequest;
				delete clientCollection[j - serverCollection.size()].myResponse;
				clientCollection.erase(clientCollection.begin() + j - serverCollection.size());
				--j;
				throw std::runtime_error("Client Disconnected!");
			}
			else
			{
				// Process the received data
				std::string receivedData(buffer, bytesRead);
				clientCollection[j - serverCollection.size()].myBuffer += receivedData;
				clientCollection[j - serverCollection.size()].processRequest(serverCollection);
 				if (clientCollection[j - serverCollection.size()].myStage == RESPONSE)
					pollFds[j].events = POLLOUT;
			}
		}
		if (pollFds[j].revents & POLLOUT)
		{
			clientCollection[j - serverCollection.size()].processResponse();
			if (clientCollection[j - serverCollection.size()].CloseSocket == true)
			{
				// printf("------------------Hello message sent-------------------\n");
				close(pollFds[j].fd);
				pollFds.erase(pollFds.begin() + j);
				delete clientCollection[j - serverCollection.size()].myRequest;
				delete clientCollection[j - serverCollection.size()].myResponse;
				clientCollection.erase(clientCollection.begin() + j - serverCollection.size()); // 2 erpresent size of server arrays Don't forget to edit it
				--j;
			}
		}
	}
}