/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelhlami <lelhlami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:12:12 by lelhlami          #+#    #+#             */
/*   Updated: 2023/06/11 10:11:45 by lelhlami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <request.hpp>
#include <Server.hpp>
#include <Config.hpp>
#include <Client.hpp>

#include <fcntl.h>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <utils.hpp>
#include <poll.h>
#include <vector>

#define	MAX_BUFFER	1024


class	request;
class	Client;

typedef std::vector<Server> Servers;
typedef Servers::iterator ServerRef;

static std::vector<	Server> 					serverCollection;
static std::vector<Client>				 		clientCollection;
static std::vector<std::pair<Socket, request> >	requestCollection;
static std::vector<struct pollfd>				pollFds;

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
		static void createListenSocket(Socket &socketID, Servers &servers);
		static void makeSocketReuseAddr(const Socket &socketID, Servers &servers);
		static void makeSocketNonBlocking(const Socket &socketID, Servers &servers);
		static void makeServerListen(const ServerRef &server, Servers &servers);
		static addrinfo *getServerAddrInfo(const ServerRef &server, Servers &servers);
		static void freeServerAddrInfo(addrinfo *addr);
		static void throwAddrInfoError(int errorCode, const std::string &errorMsg);
		static void addServerFDListenCollection(Servers &servers);
		static Socket getNewConnectionSock(Socket listenSock);
		static void handleNewConnections();
		static void addPollFd( const ServerRef &server );
		static void pollMultiplexing( Servers& );
		static void	initPollFdClient( Socket& socket, Server& );
		static void	handleActions( void );
};
