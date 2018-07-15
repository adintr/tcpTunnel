#pragma once

class ITunnel
{
public:
	virtual ~ITunnel()	{}
	virtual int read(unsigned char* buff, int bufsize) = 0;
	virtual void write(unsigned char* buff, int bufsize) = 0;
	virtual bool isready() = 0;
};

