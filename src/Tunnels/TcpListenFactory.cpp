#include "TcpListenFactory.h"
#include "TcpTunnel.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

TcpListenFactory::TcpListenFactory()
{
}

TcpListenFactory::~TcpListenFactory()
{
}

std::vector<ITunnel*> TcpListenFactory::MakeTunnels(const std::string& desc)
{
	std::vector<ITunnel*> result;

	std::string::size_type portPos = desc.find(':');
	std::string::size_type countPos = desc.find('/', portPos);

	if (portPos == std::string::npos)
	{
		fprintf(stderr, "tcplisten need ip:port/count format\n");
		return result;
	}

	std::string ipaddrDesc = desc.substr(0, portPos);
	std::string portDesc = desc.substr(portPos + 1);
	std::string countDesc = "1";

	if (countPos != std::string::npos)
	{
		countDesc = desc.substr(countPos + 1);
		portDesc = desc.substr(portPos + 1, countPos);
	}

	int port = atoi(portDesc.c_str());
	int count = atoi(countDesc.c_str());

	sockaddr_in listenAddr;
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_port = htons(port);
	//inet_pton(AF_INET, ipaddrDesc.c_str(), &listenAddr.sin_addr);
	
	int listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock < 0)
	{
		fprintf(stderr, "create listen socket error.\n");
		return result;
	}

	if (bind(listenSock, (sockaddr*)&listenAddr, sizeof(listenAddr)) < 0)
	{
		close(listenSock);
		fprintf(stderr, "bind listen address error.\n");
		return result;
	}

	if (listen(listenSock, count) < 0)
	{
		close(listenSock);
		fprintf(stderr, "listen error.\n");
		return result;
	}

    printf("listen on %d\n", port);
	for (int i = 0; i < count; ++i)
	{
		sockaddr_in remoteaddr;
		socklen_t addrlen = sizeof(remoteaddr);
        printf("wait accept for %d client\n", i);
		int sk = accept(listenSock, (sockaddr*)&remoteaddr, &addrlen);

		if (sk < 0)
		{
			fprintf(stderr, "accept error.\n");
			break;
		}

        printf("get %d client\n", i);
		result.push_back(new TcpTunnel(sk));
	}
	
	close(listenSock);
	return result;
}
