#include "GameChannel.h"

int main()
{
    ZinxKernel::ZinxKernelInit();
    /*��Ӽ���ͨ��*/
    ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
    ZinxKernel::Zinx_Run();
    ZinxKernel::ZinxKernelFini();
}