#pragma once
#include <zinx.h>
#include <ZinxTCP.h>

class StdInChannel :
    public Ichannel
{
public:
    StdInChannel();
    ~StdInChannel();
    bool Init();
    bool ReadFd(std::string& _input);
    bool WriteFd(std::string& _output);
    void Fini();
    int GetFd();
    std::string GetChannelInfo();
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput);
};

class StdOutChannel :
    public Ichannel
{
public:
    StdOutChannel();
    ~StdOutChannel();
    bool Init();
    bool ReadFd(std::string& _input);
    bool WriteFd(std::string& _output);
    void Fini();
    int GetFd();
    std::string GetChannelInfo();
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput);
};

class myTcpData :public ZinxTcpData {
public:
    myTcpData(int _fd) :ZinxTcpData(_fd) {};
    // 通过 ZinxTcpData 继承
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class myFact :public IZinxTcpConnFact {
public:
    // 通过 IZinxTcpConnFact 继承
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};