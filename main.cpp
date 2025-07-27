#include "StdInChannel.h"
#include "Roles.h"
#include <zinx.h>
#include <ZinxTCP.h>
#include "ZinxTimer.h"

void daemonlize() {
	//1 fork
	int ipid = fork();
	if (0 > ipid) {
		exit(-1);
	}

	if (0 < ipid) {
		//2 �������˳�
		exit(0);
	}

	//3 �ӽ��� ���ûػ�ID
	setsid();

	//4 �ӽ��� ����ָ��·��
	// �˴�����
	 
	//5 �ӽ��� �ض���3���ļ�������/dev/null
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd >= 0) {
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);
		close(nullfd);
	}

	//���̼��
	while (1) {
		int pid = fork();
		if (0 > pid) {
			exit(-1);
		}

		/*�����̵��ӽ����˳�*/
		if (0 < pid) {
			int iStatus = 0;
			wait(&iStatus);
			if (0 == iStatus) {
				exit(0);
			}
		}

		/*�ӽ�������ѭ��ִ����Ϸҵ��*/
		else {
			break;
		}
	}
}

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
	daemonlize();

	/*1-��ʼ�����*/
	ZinxKernel::ZinxKernelInit();
	TimerOutMng::GetInstance().AddTask(new timerhello());
	TimerOutMng::GetInstance().AddTask(new timerbye());

	/*4-��ͨ��������ӵ����*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(11111, new myFact())));

	ZinxKernel::Zinx_Add_Role(*(new Echo()));	
	ZinxKernel::Zinx_Add_Role(*(new DateRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));

	/*5-���п��*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}