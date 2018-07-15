#pragma once
#include "../ITunnelFactory.h"

class TcpListenFactory : public ITunnelFactory
{
public:
	TcpListenFactory();
	virtual ~TcpListenFactory();

	virtual std::vector<ITunnel*> MakeTunnels(const std::string& desc);
};

