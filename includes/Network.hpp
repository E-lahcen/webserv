/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:12:12 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/07 17:55:59 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <Server.hpp>
#include <request.hpp>

#include <fcntl.h>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <utils.hpp>
#include <vector>

class request;

typedef std::vector<Server> Servers;
typedef Servers::iterator ServerRef;

static std::unordered_map<Socket, Server> listenFDCollection;
static std::vector<std::pair<Socket, request> >	requestCollection; 

class Network
{

public:
	static void initServersSockets(Servers &servers);
	static void clearServersSockets(Servers &servers);
	static std::string getSocketServerName(const Socket sock);
	static std::string getSocketClientName(const Socket sock);


	Network(Servers &servers);
	~Network();

private:
	const static int sockType = SOCK_STREAM;
	const static int addrFamily = AF_INET;
	const static int flags = AI_PASSIVE;
	const static int protocol;

	static void createSocket(Socket &socketID, Servers &servers);

	static void makeSocketReuseAddr(const Socket &socketID, Servers &servers);

	static void makeSocketNonBlocking(const Socket &socketID, Servers &servers);

	static void makeServerListen(const ServerRef &server, Servers &servers);

	static addrinfo *getServerAddrInfo(const ServerRef &server, Servers &servers);

	static void freeServerAddrInfo(addrinfo *addr);

	static std::string sockAddrToName(const sockaddr *addr, socklen_t addrLen);

	static void throwAddrInfoError(int errorCode, const std::string &errorMsg);

	static void addServerFDListenCollection(Servers &servers);

	static Socket getNewConnectionSock(Socket listenSock);

	static void initRequestSocket(const Server &server, const Socket &socket);

	static void handleNewConnections();
};
