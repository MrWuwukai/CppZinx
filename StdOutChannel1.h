#pragma once
#include "Ichannel.h"

class stdout_channel : public Ichannel {
public:
    stdout_channel();
    virtual ~stdout_channel();

    virtual std::string ReadFd(std::string& _input) override;
    virtual void WriteFd(std::string _output) override;
    virtual int GetFd() override;
    //virtual void data_process(std::string _input) override;
    virtual bool init() override;
    virtual void fini() override;

    // Í¨¹ý Ichannel ¼Ì³Ð
    ZinxHandler* GetInputNextStage(ByteMsg* _byte) override;
};

