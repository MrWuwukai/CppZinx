#include "ProcessFunc.h"
#include <algorithm>
#include "kernel.h"

void ProcessFunc::DataProcess(std::string _input) {
    //参数中第一个字母是小写字母则转大写输出
    if (_input[0] <= 'z' && _input[0] >= 'a')
    {
        UpperOut(_input);
    }
    else
    {
        //否则原样输出
        OrigOut(_input);
    }
}

void ProcessFunc::UpperOut(std::string _input) {
    transform(_input.begin(), _input.end(), _input.begin(), ::toupper);
    m_out->data_sendout(_input);
}

void ProcessFunc::OrigOut(std::string _input) {
    m_out->data_sendout(_input);
}

ZinxMsg* ProcessFunc::internel_handle(ZinxMsg* _inputMsg) {
    auto byte = dynamic_cast<ByteMsg*>(_inputMsg);
    if (NULL != byte) {
        std::string s = byte->content;
        if (s[0] <= 'z' && s[0] >= 'a')
        {
            transform(s.begin(), s.end(), s.begin(), ::toupper);
        }
        //调用发送函数
        zinxkernel::zinx_sendout(s, m_out);
    }
    return nullptr;
}

ZinxHandler* ProcessFunc::GetNext(ZinxMsg* _next_input)
{
    return NULL;
}
