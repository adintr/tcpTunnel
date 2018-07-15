#pragma once
#include "ITunnelFactory.h"
#include <map>

class TunnelFactoryManager
{
public:
	TunnelFactoryManager();
	virtual ~TunnelFactoryManager();

	void Reg(const std::string& name, ITunnelFactory* fac);
	void AddTunnels(std::vector<ITunnel*>& tunnels, const std::string& desc);

private:
	std::map<std::string, ITunnelFactory*>  m_factorys;
};

