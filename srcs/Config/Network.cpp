/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:14:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 13:38:48 by lelhlami         ###   ########.fr       */
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

void Network::initServersSockets(Servers &servers)
{

	for (Servers::iterator server = servers.begin();
		 server != servers.end(); ++server)
	{
		createListenSocket(server->socketFd, servers);
		makeServerListen(server, servers);
		addPollFd(server);
	}

	while (1)
	{
		addServerFDListenCollection(servers);
		pollMultiplexing(servers);
		handleNewConnections();
		handleActions();
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
		// request	req;
		// Client	client;
		// int valread;
		// char buffer[MAX_BUFFER];
		
		if (pollFds[i].revents & POLLIN)
		{
			Socket newSock = getNewConnectionSock(pollFds[i].fd);
			initPollFdClient(newSock, serverCollection[i]);
			// requestCollection.push_back(std::make_pair(newSock, req));
			// valread = read(newSock, buffer, 300000);
			// std::cout << buffer << std::endl;
		}
	}
}

Socket Network::getNewConnectionSock(Socket listenSock)
{
	Socket newSock = accept(listenSock, NULL, NULL);
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
	char hello[] = "HTTP/1.0 200 OK\r\n"
				"Server: webserver-c\r\n"
				"Content-type: text/html\r\n\r\n"
				"<h1>hello from server</h1>\r\n";

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
				clientCollection.erase(clientCollection.begin() + j - serverCollection.size());
				throw std::runtime_error("Issue in read() function!");
			}
			else if (bytesRead == 0)
			{
				// Client disconnected
				// std::cout << "Client disconnected" << std::endl;
				pollFds[j].events = POLLOUT;
				clientCollection[j - serverCollection.size()].myStage = RESPONSE;
			}
			else
			{
				// Process the received data
				std::string receivedData(buffer, bytesRead);
				clientCollection[j - serverCollection.size()].myBuffer += receivedData;
				clientCollection[j - serverCollection.size()].processRequest();
				// std::cout << "Received: " << receivedData << 	std::endl;
				pollFds[j].events = POLLOUT;
			}
		}
		if (pollFds[j].revents & POLLOUT)
		{
			ssize_t bytesWritten = write(pollFds[j].fd, hello, strlen(hello));
			if (bytesWritten == -1)
				throw std::runtime_error("Error writing to client");
			else if ((size_t)bytesWritten == strlen(hello))
			{
				// printf("------------------Hello message sent-------------------\n");
				close(pollFds[j].fd);
				pollFds.erase(pollFds.begin() + j);
				delete clientCollection[j - serverCollection.size()].myRequest;
				clientCollection.erase(clientCollection.begin() + j - serverCollection.size()); // 2 erpresent size of server arrays Don't forget to edit it
				--j;
			}
		}
	}
}