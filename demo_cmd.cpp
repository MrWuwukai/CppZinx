#include <zinx.h>
#include <iostream>

using namespace std;

/*2-���ܴ�����*/
class Echo :public AZinxHandler {
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override {
        /*���Ե���׼���*/
        GET_REF2DATA(BytesMsg, input, _oInput);
        //cout << input.szData << endl;
        ZinxKernel::Zinx_Send0ut(input.szData, *poOut);
        return nullptr;
    }
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
    {
        return nullptr;
    }
}*poEcho = new Echo();
class Exit :public AZinxHandler {
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override {
        GET_REF2DATA(BytesMsg, oBytes, _oInput);
        if (oBytes.szData == "exit")
        {
            ZinxKernel::Zinx_Exit();
            return nullptr;
        }
        return new BytesMsg(oBytes);
    }
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
    {
        return poEcho;
    }
}*poExit = new Exit();
class Cmd :public AZinxHandler {
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override {
        /*���Ե���׼���*/
        GET_REF2DATA(BytesMsg, oBytes, _oInput);
        
        if (oBytes.szData == "open")
        {
            ZinxKernel::Zinx_Add_Channel(*poOut);
            return nullptr;
        }
        else if (oBytes.szData == "close")
        {
            ZinxKernel::Zinx_Del_Channel(*poOut);
            return nullptr;
        }
        return new BytesMsg(oBytes);
    }
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
    {
        GET_REF2DATA(BytesMsg, oBytes, _oNextMsg);
        if (oBytes.szData == "exit")
        {
            return poExit;
        }
        else
        {
            return poEcho;
        }
    }
}*poCmd = new Cmd();

/*3-ͨ����*/
class TestStdin :public Ichannel {
    // ͨ�� Ichannel �̳�
    virtual bool Init() override
    {
        return true;
    }
    virtual bool ReadFd(std::string& _input) override
    {
        cin >> _input;
        return true;
    }
    virtual bool WriteFd(std::string& _output) override
    {
        return false;
    }
    virtual void Fini() override
    {
    }
    virtual int GetFd() override
    {
        return 0;
    }
    virtual std::string GetChannelInfo() override
    {
        return "stdin";
    }
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
    {
        return poCmd;
    }
};

class TestStdout :public Ichannel {
    // ͨ�� Ichannel �̳�
    virtual bool Init() override
    {
        return true;
    }
    virtual bool ReadFd(std::string& _input) override
    {
        return false;
    }
    virtual bool WriteFd(std::string& _output) override
    {
        cout << _output << endl;
        return true;
    }
    virtual void Fini() override
    {
    }
    virtual int GetFd() override
    {
        return 1;
    }
    virtual std::string GetChannelInfo() override
    {
        return "stdout";
    }
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
    {
        return nullptr;
    }
} *poOut = new TestStdout();

int main()
{
    /*1-��ʼ�����*/
    ZinxKernel::ZinxKernelInit();

    /*4-��ͨ��������ӵ����*/
    TestStdin* poStdin = new TestStdin();
    ZinxKernel::Zinx_Add_Channel(*poStdin);
    ZinxKernel::Zinx_Add_Channel(*poOut);

    /*5-���п��*/
    ZinxKernel::Zinx_Run();

    ZinxKernel::ZinxKernelFini();
    return 0;
}