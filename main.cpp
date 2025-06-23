#include "StdInChannel.h"
#include "Roles.h"
#include <zinx.h>
#include <ZinxTCP.h>

int main()
{
	/*1-初始化框架*/
	ZinxKernel::ZinxKernelInit();

	/*4-将通道对象添加到框架*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(11111, new myFact())));

	ZinxKernel::Zinx_Add_Role(*(new Echo()));	
	ZinxKernel::Zinx_Add_Role(*(new DateRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));

	/*5-运行框架*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}