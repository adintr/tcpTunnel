#include "TcpTunnel.h"

#ifdef WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#define closesocket close
#endif

TcpTunnel::TcpTunnel(int sk)
	: m_sock(sk), m_isfin(false)
{
}

TcpTunnel::~TcpTunnel()
{
	closesocket(m_sock);
}

int TcpTunnel::read(unsigned char* buff, int bufsize)
{
	int readbytes = recv(m_sock, (char*)buff, bufsize, 0);
	if (readbytes <= 0)
	{
		m_isfin = true;
		readbytes = 0;
	}

	return readbytes;
}

void TcpTunnel::write(unsigned char* buff, int bufsize)
{
	int sendsize = 0;
	while (sendsize < bufsize)
	{
		int onesize = send(m_sock, (char*)&buff[sendsize], bufsize - sendsize, 0);

		if (onesize < 0)
		{
			m_isfin = true;
			return;
		}

		sendsize += onesize;
	}
}

bool TcpTunnel::isready()
{
	return !m_isfin;
}
