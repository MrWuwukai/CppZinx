#include "GameProtocol.h"
#include <iostream>
#include "GameMsg.h"
#include "GameChannel.h"
using namespace std;

GameProtocol::GameProtocol()
{
}

GameProtocol::~GameProtocol()
{
}

/*
输入参数是通道传来的原始报文
返回值是转换后的消息对象
转换方式:TCP粘包处理
*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
    MultiMsg* pRet = new MultiMsg();

    szTcpBuffer.append(_szInput);

    while(1) {
        if (szTcpBuffer.size() < 8) {
            break;
        }

        /*在前四个字节中读取消息内容长度(低字节在前)*/
        int iLength = 0;
        iLength |= szTcpBuffer[0] << 0;
        iLength |= szTcpBuffer[1] << 8;
        iLength |= szTcpBuffer[2] << 16;
        iLength |= szTcpBuffer[3] << 24;
        /*中四个字节读类型id(低字节在前)*/
        int id = 0;
        id |= szTcpBuffer[4] << 0;
        id |= szTcpBuffer[5] << 8;
        id |= szTcpBuffer[6] << 16;
        id |= szTcpBuffer[7] << 24;

        /*通过读到的长度判断后续报文是否合法*/
        if (szTcpBuffer.size() - 8 < iLength)
        {
            /*本条报文还没够, 啥都不干*/
            break;
        }
        /*构造一条用户请求*/
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szTcpBuffer.substr(8, iLength));
        pRet->m_Msgs.push_back(pMsg);
        /*剩下的不足一条报文的内容继续存里面*/
        szTcpBuffer.erase(0, 8 + iLength);
    }

    return pRet;
}

/*参数来自业务层，待发送的消息
返回值:转换后的字节流*/
std::string* GameProtocol::response2raw(UserData& _oUserData) {
    int iLength = 0;
    int id = 0;
    std::string MsgContent;

    GET_REF2DATA(GameMsg, oOutput, _oUserData);
    id = oOutput.enMsgType;
    MsgContent = oOutput.serialize();
    iLength = MsgContent.size();

    auto pret = new std::string();

    pret->push_back((iLength >> 0) & 0xff);
    pret->push_back((iLength >> 8) & 0xff);
    pret->push_back((iLength >> 16) & 0xff);
    pret->push_back((iLength >> 24) & 0xff);
    pret->push_back((id >> 0) & 0xff);
    pret->push_back((id >> 8) & 0xff);
    pret->push_back((id >> 16) & 0xff);
    pret->push_back((id >> 24) & 0xff);
    pret->append(MsgContent);

    return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg) { return nullptr; }

/*返回数据发送的通道*/
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes) {
    return m_channel;
}