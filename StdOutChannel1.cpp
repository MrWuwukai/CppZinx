#include "StdOutChannel1.h"
#include <iostream>

stdout_channel::stdout_channel() {
}

stdout_channel::~stdout_channel() {
}

std::string stdout_channel::ReadFd(std::string& _input) {
    return std::string();
}

void stdout_channel::WriteFd(std::string _output) {
    cout << _output << endl;
}

int stdout_channel::GetFd() {
    // stdout 的文件描述符通常是 1
    return 1;
}

//void stdout_channel::data_process(std::string _input) {
//    
//}

bool stdout_channel::init() {
    return true;
}

void stdout_channel::fini() {

}

ZinxHandler* stdout_channel::GetInputNextStage(ByteMsg* _byte)
{
    return nullptr;
}
