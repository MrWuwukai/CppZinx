#include "StdInChannel.h"
#include "CmdCheck.h"

StdInChannel::StdInChannel()
{
}

StdInChannel::~StdInChannel()
{
}

bool StdInChannel::Init()
{
    return true;
}

bool StdInChannel::ReadFd(std::string& _input)
{
    cin >> _input;
    return true;
}

bool StdInChannel::WriteFd(std::string& _output)
{
    return false;
}

void StdInChannel::Fini()
{
}

int StdInChannel::GetFd()
{
    return 0;
}

std::string StdInChannel::GetChannelInfo()
{
    return "stdin";
}

AZinxHandler* StdInChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return CmdCheck::GetInstance();
}

StdOutChannel::StdOutChannel() {
}

StdOutChannel::~StdOutChannel() {
}

bool StdOutChannel::Init() {
    return true;
}

bool StdOutChannel::ReadFd(std::string& _input) {
    // stdout ͨ�������ڶ�ȡ�����Է��� false
    return false;
}

bool StdOutChannel::WriteFd(std::string& _output) {
    // д�� stdout
    std::cout << _output;
    return true;
}

void StdOutChannel::Fini() {
    // ������Դ������У�
}

int StdOutChannel::GetFd() {
    // stdout ���ļ�������ͨ���� 1
    return 1;
}

std::string StdOutChannel::GetChannelInfo() {
    return "stdout";
}

AZinxHandler* StdOutChannel::GetInputNextStage(BytesMsg& _oInput) {
    // ���� nullptr����ʾû����һ������׶Σ����ɿ�ܾ�����
    return nullptr;
}