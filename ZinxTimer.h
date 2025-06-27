#pragma once
#include <vector>
#include <list>

class ZinxTimerChannel :
    public Ichannel
{
    int m_TimerFd = -1;
public:
    ZinxTimerChannel();
    virtual ~ZinxTimerChannel();

    virtual bool Init() override;
    virtual bool ReadFd(std::string& _input) override;
    virtual bool WriteFd(std::string& _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class TimerOutProc {
public:
    virtual void Proc() = 0;
    virtual int GetTimerSec() = 0;
    virtual ~TimerOutProc();
    int iCount = -1;
};

class TimerOutMng :public AZinxHandler {
    std::vector<std::list<TimerOutProc *> > m_timer_wheel;
    static TimerOutMng single;
    int cur_index = 0;
    TimerOutMng();
public:
    /*处理超时事件*/
    virtual IZinxMsg* InternalHandle(IZinxMsg& _oInput) override;

    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

    void AddTask(TimerOutProc * _ptask);
    void DelTask(TimerOutProc * _ptask);

    static TimerOutMng& GetInstance() {
        return single;
    }
};
