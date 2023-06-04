/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:14:53 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/04 16:55:07 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Network.hpp>
#include <iostream>

const int Network::mProtocol = getprotobyname("tcp")->p_proto;

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
		createSocket(server->socketFd, servers);
		makeServerListen(server, servers);
	}
}

void Network::createSocket(Socket &socketFd, Servers &servers)
{
	socketFd = socket(mAddrFamily, mSockType, mProtocol);
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
	int new_socket;
	int valread;
	char hello[] = "HTTP/1.0 200 OK\r\n"
				   "Server: webserver-c\r\n"
				   "Content-type: text/html\r\n\r\n"
				   "<h1>hello from server</h1>\r\n";

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

	// Accepting clients requests
	while (1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server->socketFd, serverAddr->ai_addr, &serverAddr->ai_addrlen)) < 0)
		{
			std::string error = "failed to accept socket of ";
			error += server->hostname + ':' + server->port;
			clearServersSockets(servers);
			throwErrnoException(error);
		}

		char buffer[1024] = {0};
		valread = read(new_socket, buffer, 1024);
		printf("%s\n", buffer);
		write(new_socket, hello, strlen(hello));
		printf("------------------Hello message sent-------------------\n");
		close(new_socket);
	}

	// Log::socketBinding(server->socketFd);

	freeServerAddrInfo(serverAddr);
}

addrinfo *Network::getServerAddrInfo(const ServerRef &server, Servers &servers)
{

	addrinfo hints;
	// clears hints
	bzero(&hints, sizeof(hints));
	hints.ai_flags = mFlags;
	hints.ai_family = mAddrFamily;
	hints.ai_socktype = mSockType;
	hints.ai_protocol = mProtocol;

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
