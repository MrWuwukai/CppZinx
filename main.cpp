#include "StdInChannel.h"
#include "Roles.h"
#include <zinx.h>
#include <ZinxTCP.h>

int main()
{
	/*1-��ʼ�����*/
	ZinxKernel::ZinxKernelInit();

	/*4-��ͨ��������ӵ����*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(11111, new myFact())));

	ZinxKernel::Zinx_Add_Role(*(new Echo()));	
	ZinxKernel::Zinx_Add_Role(*(new DateRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));

	/*5-���п��*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}