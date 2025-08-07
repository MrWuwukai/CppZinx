#pragma once
#include <string>
#include "ZinxHandler.h"

class sysIODicMsg : public ZinxMsg {
public:
    enum IO_DIC {
        IO_IN, IO_OUT
    } m_DIC;
    sysIODicMsg(IO_DIC dic) :m_DIC(dic){}
};

class ByteMsg : public sysIODicMsg {
public:
    std::string content;
    ByteMsg(std::string _content, sysIODicMsg& iodic) :content(_content), sysIODicMsg(iodic.m_DIC) {}
};

class Ichannel : public ZinxHandler {
public:
    Ichannel();
    virtual ~Ichannel();
    virtual bool init() = 0;
    virtual void fini() = 0;
    virtual std::string ReadFd(std::string& _input) = 0;
    virtual void WriteFd(std::string _output) = 0;
    virtual int GetFd() = 0;
    //virtual void data_process(std::string _input) = 0;
    virtual void data_sendout(std::string _output);
    void flush_out();

    virtual ZinxHandler* GetInputNextStage(ByteMsg *_byte) = 0;
    virtual ZinxMsg* internel_handle(ZinxMsg* _inputMsg) override;
    virtual ZinxHandler* GetNext(ZinxMsg* _next_input) override;
private:
    std::string buffer;
};

