#pragma once
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
