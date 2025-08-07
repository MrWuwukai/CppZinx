#pragma once
#include <string>
#include "Ichannel.h"
#include "ZinxHandler.h"

class ProcessFunc : public ZinxHandler{
public:
	ProcessFunc();
	virtual ~ProcessFunc();
	void DataProcess(std::string _input);
	void UpperOut(std::string _input);
	void OrigOut(std::string _input);

	Ichannel* m_out = NULL;

	// Í¨¹ý ZinxHandler ¼Ì³Ð
	ZinxMsg* internel_handle(ZinxMsg* _inputMsg) override;
	ZinxHandler* GetNext(ZinxMsg* _next_input) override;
};

