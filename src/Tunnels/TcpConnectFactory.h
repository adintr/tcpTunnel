#pragma once
#include "../ITunnelFactory.h"

class TcpConnectFactory : public ITunnelFactory
{
public:
	TcpConnectFactory();
	virtual ~TcpConnectFactory();

	virtual std::vector<ITunnel*> MakeTunnels(const std::string& desc);
};

