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
    /*�����ļ�������*/
    int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (0 <= iFd)
    {
        /*���ö�ʱ����*/
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
	/*��ȡ��ʱ����*/

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
    // ͨ�� AZinxHandler �̳�
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
    // ʱ���ֳ���=10
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
		/*�ƶ��̶�*/
		cur_index++;
		cur_index %= 10;

		std::list<TimerOutProc*> m_cache;
		/*������ǰ�̶����нڵ㣬ָ��������Ȧ��-1,*/
		for (auto itr = m_timer_wheel[cur_index].begin(); itr != m_timer_wheel[cur_index].end();) // ��Ҫ�ڴ˴�++itr����ֹ��list����ɾ������������forѭ��������ʧЧ
		{
			/*���Ȧ��С�ڵ���0-->ȡ������*/
			if ((*itr)->iCount <= 0)
			{
				//(*itr)->Proc();
				m_cache.push_back(*itr); // ��һ��cache��ֹ������ͻȻʧЧ������Proc()����ͻȻɾ���Լ�
				auto tmp = *itr;
				itr = m_timer_wheel[cur_index].erase(itr);

				AddTask(tmp);
			}
			else
			{
				(*itr)->iCount--;
				++itr;
			}
			/*����Ȧ��-1*/
		}

		/*ͳһ������ʱ����*/
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
    for (auto& chi : m_timer_wheel) // ע�����ã�����ԭʼ����
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
