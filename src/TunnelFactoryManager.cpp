#include "TunnelFactoryManager.h"
#include <stdio.h>

TunnelFactoryManager::TunnelFactoryManager()
{
}

TunnelFactoryManager::~TunnelFactoryManager()
{
	for (auto& vp : m_factorys)
	{
		delete vp.second;
	}

	m_factorys.clear();
}

void TunnelFactoryManager::Reg(const std::string& name, ITunnelFactory* fac)
{
	m_factorys[name] = fac;
}

void TunnelFactoryManager::AddTunnels(std::vector<ITunnel*>& tunnels, const std::string& desc)
{
	std::string::size_type pos = desc.find("://");
	if (pos == std::string::npos)
	{
		fprintf(stderr, "tunnel desc %s is not correct.\n", desc.c_str());
		return;
	}

	std::string protocl = desc.substr(0, pos);
	std::string leftdesc = desc.substr(pos + 3);

	auto iter = m_factorys.find(protocl);
	if (iter == m_factorys.end())
	{
		fprintf(stderr, "unknown protocol %s.\n", protocl.c_str());
		return;
	}

	std::vector<ITunnel*> ts = iter->second->MakeTunnels(leftdesc);
	tunnels.insert(tunnels.end(), ts.begin(), ts.end());
}
