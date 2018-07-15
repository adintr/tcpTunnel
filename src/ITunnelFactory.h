#pragma once
#include "ITunnel.h"
#include <vector>
#include <string>

class ITunnelFactory
{
public:
	virtual ~ITunnelFactory() {}
	virtual std::vector<ITunnel*> MakeTunnels(const std::string& desc) = 0;
};

