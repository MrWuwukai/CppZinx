#include "ZinxTimer.h"

ZinxTimerChannel::ZinxTimerChannel() 
{
}

ZinxTimerChannel::~ZinxTimerChannel()
{
}

bool ZinxTimerChannel::Init()
{
    bool bRet = false;
    /*创建文件描述符*/
    int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (0 <= iFd)
    {
        /*设置定时周期*/
        struct itimerspec period = {
            {1,0}, {1,0}
        };
        if (0 == timerfd_settime(iFd, 0, &period, NULL))
        {
            bRet = true;
            m_TimerFd = iFd;
        }
    }
    return bRet;
    return false;
}

bool ZinxTimerChannel::ReadFd(std::string& _input)
{
	/*读取超时次数*/

	bool bRet = false;
	char buff[8] = { 0 };

	if (sizeof(buff) == read(m_TimerFd, buff, sizeof(buff)))
	{
		bRet = true;
        _input.assign(buff, sizeof(buff));
	}
	return bRet;
}

bool ZinxTimerChannel::WriteFd(std::string& _output)
{
    return false;
}

void ZinxTimerChannel::Fini()
{
    close(m_TimerFd);
    m_TimerFd = -1;
}

int ZinxTimerChannel::GetFd()
{
    return m_TimerFd;
}

std::string ZinxTimerChannel::GetChannelInfo()
{
    return "TimerFd";
}

class output_hello :public AZinxHandler {
    // 通过 AZinxHandler 继承
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
    {
        auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
        std::string output = "hello world";
        ZinxKernel::Zinx_SendOut(output, *pchannel);
        return nullptr;
    }
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
    {
        return nullptr;
    }
} * pout_hello = new output_hello();

AZinxHandler* ZinxTimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return &TimerOutMng::GetInstance();
}

TimerOutMng TimerOutMng::single;

TimerOutMng::TimerOutMng() {
    // 时间轮长度=10
    for (int i = 0; i < 10; i++)
    {
        std::list<TimerOutProc*> tmp;
        m_timer_wheel.push_back(tmp);
    }
}


IZinxMsg* TimerOutMng::InternalHandle(IZinxMsg& _OInput)
{
    unsigned long iTimeoutCount = 0;
    GET_REF2DATA(BytesMsg, obytes, _oInput);
    obytes.szData.copy((char*)&iTimeoutCount, sizeof(iTimeoutCount), 0);

	while (iTimeoutCount-- > 0) {
		/*移动刻度*/
		cur_index++;
		cur_index %= 10;

		std::list<TimerOutProc*> m_cache;
		/*遍历当前刻度所有节点，指向处理函数或圈数-1,*/
		for (auto itr = m_timer_wheel[cur_index].begin(); itr != m_timer_wheel[cur_index].end();) // 不要在此处++itr，防止在list里面删东西导致整个for循环迭代器失效
		{
			/*如果圈数小于等于0-->取出任务*/
			if ((*itr)->iCount <= 0)
			{
				//(*itr)->Proc();
				m_cache.push_back(*itr); // 用一个cache防止迭代器突然失效，例如Proc()里面突然删除自己
				auto tmp = *itr;
				itr = m_timer_wheel[cur_index].erase(itr);

				AddTask(tmp);
			}
			else
			{
				(*itr)->iCount--;
				++itr;
			}
			/*否则，圈数-1*/
		}

		/*统一待处理超时任务*/
		for (auto task : m_cache)
		{
			task->Proc();
		}
	}

	return nullptr;
}

void TimerOutMng::AddTask(TimerOutProc* _ptask)
{
    int next_pos = (_ptask->GetTimerSec() + cur_index) % 10;
    m_timer_wheel[next_pos].push_back(_ptask);
    _ptask->iCount = _ptask->GetTimerSec() / 10;
}

void TimerOutMng::DelTask(TimerOutProc* _ptask)
{
    for (auto& chi : m_timer_wheel) // 注意引用，处理原始数据
    {
        for (auto task : chi)
        {
            if (task == _ptask)
            {
                chi.remove(_ptask);
                break;
            }
        }
    }
}
