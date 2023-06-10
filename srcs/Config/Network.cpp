/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:14:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/10 15:38:26 by lelhlami         ###   ########.fr       */
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
	}

	while (1)
	{
		addServerFDListenCollection(servers);
		handleNewConnections();
	}
}

void Network::addServerFDListenCollection(Servers &servers)
{

	listenFDCollection.clear();

	Servers::const_iterator server;
	for (server = servers.begin();
		 server != servers.end(); ++server)
	{
		listenFDCollection[server->socketFd] = *server;
	}
}

void Network::handleNewConnections()
{
	std::unordered_map<Socket, Server>::const_iterator listenFD;
	for (listenFD = listenFDCollection.begin();
		 listenFD != listenFDCollection.end(); ++listenFD)
	{
		try
		{
			request	req;
			int valread;
			char buffer[30000];
			
			Socket newSock = getNewConnectionSock(listenFD->first);
			requestCollection.push_back(std::make_pair(newSock, req));
            valread = read(newSock, buffer, 300000);
			std::cout << buffer << std::endl;
			
		}
		catch (const std::exception &error)
		{
			std::cerr << error.what();
		}
	}
}

Socket Network::getNewConnectionSock(Socket listenSock)
{
	Socket newSock = accept(listenSock, NULL, NULL);
	std::cout << "here " << listenSock << std::endl;
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

std::string Network::sockAddrToName(const sockaddr *addr, socklen_t addrLen)
{

	// creates 2 buffers that are large enough
	// to hold any hostname and port
	// the hostname and port of the addr parameter
	// are stored by getnameinfo in
	// these 2 buffers
	char hostname[NI_MAXHOST];
	char port[NI_MAXSERV];

	int successCode = getnameinfo(addr, addrLen, hostname,
								  sizeof(hostname), port, sizeof(port),
								  // gets hostname and port in numeric format
								  NI_NUMERICHOST | NI_NUMERICSERV);

	if (successCode)
		throwAddrInfoError(successCode,
						   "failed to convert socket to name");

	return (std::string(hostname) + ':' + port);
}

std::string Network::getSocketServerName(const Socket sock)
{

	// server's socket address will be stored here
	// by getsockname
	sockaddr_storage serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);

	if (getsockname(sock,
					reinterpret_cast<sockaddr *>(&serverAddr),
					&serverAddrLen) == -1)
	{

		throwErrnoException("failed to get socket's server name");
	}

	std::string serverName = sockAddrToName(reinterpret_cast<sockaddr *>(&serverAddr), serverAddrLen);

	return serverName;
}

std::string Network::getSocketClientName(const Socket sock)
{

	// client's socket address will be stored here
	// by getpeername
	sockaddr_storage clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	if (getpeername(sock,
					reinterpret_cast<sockaddr *>(&clientAddr),
					&clientAddrLen) == -1)
	{

		throwErrnoException("failed to get socket's client name");
	}

	std::string clientName = sockAddrToName(reinterpret_cast<sockaddr *>(&clientAddr), clientAddrLen);

	return clientName;
}

void Network::throwAddrInfoError(int errorCode, const std::string &errorMsg)
{

	std::string exceptionStr = errorMsg + ": ";
	exceptionStr += gai_strerror(errorCode);

	throw std::runtime_error(exceptionStr);
}

