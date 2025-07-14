#include "GameChannel.h"

GameChannel::GameChannel(int _fd) :ZinxTcpData(_fd)
{
}

GameChannel::~GameChannel()
{
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    /*创建tcp通道对象*/
    auto pChannel = new GameChannel(_fd);
    /*创建协议对象*/
    auto pProtocol = new GameProtocol();
    /*创建role对象*/
    auto pRole = new GameRole();
    /*绑定协议对象*/
    pChannel->m_proto = pProtocol;
    pProtocol->m_channel = pChannel;
    pProtocol->m_role = pRole;
    pRole->m_pProto = pProtocol;
    /*将协议对象添加到kernel*/
    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    /*将role对象添加到kernel*/
    ZinxKernel::Zinx_Add_Role(*pRole);
    return pChannel;
}