#include "StdInChannel.h"
#include "Roles.h"
#include <zinx.h>
#include <ZinxTCP.h>
#include "ZinxTimer.h"

class timerhello :public TimerOutProc {
	virtual void Proc() override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "hello world";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
	}
	virtual int GetTimerSec() override
	{
		return 3;
	}
};
class timerbye :public TimerOutProc {
	virtual void Proc() override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "bye";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
	}
	virtual int GetTimerSec() override
	{
		return 4;
	}

};
int main()
{
	/*1-初始化框架*/
	ZinxKernel::ZinxKernelInit();
	TimerOutMng::GetInstance().AddTask(new timerhello());
	TimerOutMng::GetInstance().AddTask(new timerbye());

	/*4-将通道对象添加到框架*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(11111, new myFact())));

	ZinxKernel::Zinx_Add_Role(*(new Echo()));	
	ZinxKernel::Zinx_Add_Role(*(new DateRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));

	/*5-运行框架*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}