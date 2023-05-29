/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:12:12 by lelhlami          #+#    #+#             */
/*   Updated: 2023/05/29 17:58:10 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Config.hpp>
#include <fcntl.h>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <Server.hpp>
#include <utils.hpp>
#include <list>

class Network
{

public:
	typedef std::list<Server> Servers;
	typedef Servers::iterator ServerRef;
	static void initServersSockets(Servers &servers);
	static void clearServersSockets(Servers &servers);
	static std::string getSocketServerName(const Socket sock);
	static std::string getSocketClientName(const Socket sock);

private:
	const static int mSockType = SOCK_STREAM;
	const static int mAddrFamily = AF_INET;
	const static int mFlags = AI_PASSIVE;
	const static int mProtocol;

	static void createSocket(Socket &socketID, Servers &servers);

	static void makeSocketReuseAddr(const Socket &socketID, Servers &servers);

	static void makeSocketNonBlocking(const Socket &socketID, Servers &servers);

	static void makeServerListen(const ServerRef &server, Servers &servers);

	static addrinfo *getServerAddrInfo(const ServerRef &server, Servers &servers);

	static void freeServerAddrInfo(addrinfo *addr);

	static std::string sockAddrToName(const sockaddr *addr, socklen_t addrLen);

	static void throwAddrInfoError(int errorCode, const std::string &errorMsg);
};
