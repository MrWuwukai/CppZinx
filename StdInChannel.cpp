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
    // stdout 通常不用于读取，所以返回 false
    return false;
}

bool StdOutChannel::WriteFd(std::string& _output) {
    // 写入 stdout
    std::cout << _output;
    return true;
}

void StdOutChannel::Fini() {
    // 清理资源（如果有）
}

int StdOutChannel::GetFd() {
    // stdout 的文件描述符通常是 1
    return 1;
}

std::string StdOutChannel::GetChannelInfo() {
    return "stdout";
}

AZinxHandler* StdOutChannel::GetInputNextStage(BytesMsg& _oInput) {
    // 返回 nullptr，表示没有下一个处理阶段（或由框架决定）
    return nullptr;
}