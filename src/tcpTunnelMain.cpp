#include <iostream>
#include <stdio.h>
#include <thread>
#include "TunnelFactoryManager.h"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#endif

/*  √¸¡Ó––≤Œ ˝
 *    tcpTunnel -t tcpconnect://127.0.0.1:8080  -t tcplisten://127.0.0.1:8081/2
 *
 */

#include "Tunnels/TcpConnectFactory.h"
#include "Tunnels/TcpListenFactory.h"

int main(int argc, char *argv[])
{
#ifdef WIN32
	WSADATA wsaData;

	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != NO_ERROR)
		return -1;

#endif

	TunnelFactoryManager factorys;

	factorys.Reg("tcpconnect", new TcpConnectFactory());
	factorys.Reg("tcplisten", new TcpListenFactory());

	std::vector<ITunnel*> tunnels;

	int argIndex = 1;
	std::string optaddtunnel = "-t";

	while (argIndex < argc)
	{
		if (argv[argIndex] == optaddtunnel)
		{
			++argIndex;

			if (argIndex >= argc)
			{
				fprintf(stderr, "need tunnel desc follow option -t\n");
				return -1;
			}

			factorys.AddTunnels(tunnels, argv[argIndex]);
			++argIndex;

			continue;
		}
		
		++argIndex;
	}

	if (tunnels.empty())
	{
		fprintf(stderr, "no tunnels found.\n");
		return -1;
	}

	if (tunnels.size() == 1)
	{
		fprintf(stderr, "need 2 tunnels at least.\n");
		return -1;
	}

	std::vector<std::thread*> forwardthreads;
	for (ITunnel* tunnel : tunnels)
	{
		forwardthreads.push_back(new std::thread(
			[tunnel, &tunnels]() {

			unsigned char buff[1024];
			while (tunnel->isready())
			{
				int buflen = tunnel->read(buff, 1024);
				if (buflen > 0)
				{
					int writeCount = 0;
					for (ITunnel* t : tunnels)
					{
						if (t != tunnel && t->isready())
						{
							t->write(buff, buflen);
							++writeCount;
						}
					}

					if (writeCount <= 0)
					{
						return;
					}
				}
			}
		}
		));
	}

	for (std::thread* t : forwardthreads)
	{
		t->join();
		delete t;
	}

	for (ITunnel* tunnel : tunnels)
	{
		delete tunnel;
	}
	
	return 0;
}