#pragma once
#include "Ichannel.h"
#include <string>

class FIFO_channel : public Ichannel {
public:
    FIFO_channel();
    FIFO_channel(std::string _file, bool _isRead);
    virtual ~FIFO_channel();

    virtual std::string ReadFd() override;
    virtual void WriteFd(std::string _output) override;
    virtual int GetFd() override;
    //virtual void data_process(std::string _input) override;
    virtual bool init() override;
    virtual void fini() override;

    FIFO_channel* m_out = NULL;
private:
    std::string fifo_name;
    int m_fd = -1;
    bool bIsRead = true;   

    // Í¨¹ý Ichannel ¼Ì³Ð
    ZinxHandler* GetInputNextStage(ByteMsg* _byte) override;
};