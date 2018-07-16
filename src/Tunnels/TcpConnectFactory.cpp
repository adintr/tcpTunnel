#include "TcpConnectFactory.h"
#include "TcpTunnel.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


TcpConnectFactory::TcpConnectFactory()
{
}


TcpConnectFactory::~TcpConnectFactory()
{
}

std::vector<ITunnel*> TcpConnectFactory::MakeTunnels(const std::string& desc)
{
	std::vector<ITunnel*> tunnels;

	std::string::size_type portPos = desc.find(':');

	if (portPos == std::string::npos)
	{
		fprintf(stderr, "tcpconnect need ip:port format\n");
		return tunnels;
	}

	std::string ipaddrDesc = desc.substr(0, portPos);
	std::string portDesc = desc.substr(portPos + 1);
	int port = atoi(portDesc.c_str());
	
	sockaddr_in connAddr;
	memset(&connAddr, 0, sizeof(connAddr));
	connAddr.sin_family = AF_INET;
	connAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipaddrDesc.c_str(), &connAddr.sin_addr);

	int connSock = socket(AF_INET, SOCK_STREAM, 0);
	if (connSock < 0)
	{
		fprintf(stderr, "create connect socket error.\n");
		return tunnels;
	}

	if (connect(connSock, (sockaddr*)&connAddr, sizeof(connAddr)) < 0)
	{
		closesocket(connSock);
		fprintf(stderr, "connect error.\n");
		return tunnels;
	}
	
	tunnels.push_back(new TcpTunnel(connSock));
	return tunnels;
}
