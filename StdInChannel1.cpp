#include "StdInChannel1.h"
#include <iostream>

std::string stdin_channel::ReadFd(std::string& _input)
{
    std::string ret;
    cin >> ret;
    return ret;
}

void stdin_channel::WriteFd(std::string _output) {
}

int stdin_channel::GetFd() {
    // stdout ���ļ�������ͨ���� 1
    return 0;
}

//void stdin_channel::data_process(std::string _input) {
//    //�����ݻ��Ե���׼���--->���ñ�׼���ͨ�������sendout����
//    //m_out->data_sendout(_input);
//    m_proc->DataProcess(_input);
//}

bool stdin_channel::init() {
    return true;
}

void stdin_channel::fini() {

}

ZinxHandler* stdin_channel::GetInputNextStage(ByteMsg* _byte)
{
    return m_proc;
}

