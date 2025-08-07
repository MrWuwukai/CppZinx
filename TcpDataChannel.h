#pragma once
#include "Ichannel.h"
class TcpDataChannel : public Ichannel {
public:
	TcpDataChannel(int _socket);
	virtual ~TcpDataChannel();
	// Í¨¹ý Ichannel ¼Ì³Ð
	bool init() override;
	void fini() override;
	std::string ReadFd(std::string& _input) override;
	void WriteFd(std::string _output) override;
	int GetFd() override;
	virtual ZinxHandler* GetInputNextStage(ByteMsg* _byte) = 0;

	int m_socket = -1;
};

class TcpDataChannelFactory {
public:
	virtual TcpDataChannel* CreateChannel(int _fd) = 0;
};

class TcpListenChannel :public Ichannel {
public:
	TcpListenChannel(unsigned short port, TcpDataChannelFactory* _pfact);
	virtual ~TcpListenChannel();

	bool init() override;
	void fini() override;
	std::string ReadFd(std::string& _input) override;
	void WriteFd(std::string _output) override;
	int GetFd() override;
	ZinxHandler* GetInputNextStage(ByteMsg* _byte) override;

	TcpDataChannelFactory* m_factory = NULL;
	unsigned short m_port = 0;
	int iListenFd = -1;
};