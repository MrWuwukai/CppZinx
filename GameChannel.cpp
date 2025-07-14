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
    /*����tcpͨ������*/
    auto pChannel = new GameChannel(_fd);
    /*����Э�����*/
    auto pProtocol = new GameProtocol();
    /*����role����*/
    auto pRole = new GameRole();
    /*��Э�����*/
    pChannel->m_proto = pProtocol;
    pProtocol->m_channel = pChannel;
    pProtocol->m_role = pRole;
    pRole->m_pProto = pProtocol;
    /*��Э�������ӵ�kernel*/
    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    /*��role������ӵ�kernel*/
    ZinxKernel::Zinx_Add_Role(*pRole);
    return pChannel;
}