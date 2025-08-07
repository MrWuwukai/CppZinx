#pragma once
#include <string>
#include "Ichannel.h"
#include "StdOutChannel1.h"
#include "ProcessFunc.h"

class stdin_channel : public Ichannel {
public:
    stdin_channel();
    virtual ~stdin_channel();
    
    virtual std::string ReadFd(std::string& _input) override;
    virtual void WriteFd(std::string _output) override;
    virtual int GetFd() override;
    virtual bool init() override;
    virtual void fini() override;

    stdout_channel* m_out = NULL;
    ProcessFunc* m_proc = NULL;

    // Í¨¹ý Ichannel ¼Ì³Ð
    ZinxHandler* GetInputNextStage(ByteMsg* _byte) override;
};

