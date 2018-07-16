#pragma once
#include "../ITunnel.h"

class TcpTunnel : public ITunnel
{
public:
	TcpTunnel(int sk);
	virtual ~TcpTunnel();

	virtual int read(unsigned char* buff, int bufsize) override;
	virtual void write(unsigned char* buff, int bufsize) override;
	virtual bool isready() override;

private:
	bool   m_isfin;
	int    m_sock;
};

