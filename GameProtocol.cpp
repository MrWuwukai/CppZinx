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
���������ͨ��������ԭʼ����
����ֵ��ת�������Ϣ����
ת����ʽ:TCPճ������
*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
    MultiMsg* pRet = new MultiMsg();

    szTcpBuffer.append(_szInput);

    while(1) {
        if (szTcpBuffer.size() < 8) {
            break;
        }

        /*��ǰ�ĸ��ֽ��ж�ȡ��Ϣ���ݳ���(���ֽ���ǰ)*/
        int iLength = 0;
        iLength |= szTcpBuffer[0] << 0;
        iLength |= szTcpBuffer[1] << 8;
        iLength |= szTcpBuffer[2] << 16;
        iLength |= szTcpBuffer[3] << 24;
        /*���ĸ��ֽڶ�����id(���ֽ���ǰ)*/
        int id = 0;
        id |= szTcpBuffer[4] << 0;
        id |= szTcpBuffer[5] << 8;
        id |= szTcpBuffer[6] << 16;
        id |= szTcpBuffer[7] << 24;

        /*ͨ�������ĳ����жϺ��������Ƿ�Ϸ�*/
        if (szTcpBuffer.size() - 8 < iLength)
        {
            /*�������Ļ�û��, ɶ������*/
            break;
        }
        /*����һ���û�����*/
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szTcpBuffer.substr(8, iLength));
        pRet->m_Msgs.push_back(pMsg);
        /*ʣ�µĲ���һ�����ĵ����ݼ���������*/
        szTcpBuffer.erase(0, 8 + iLength);
    }

    return pRet;
}

/*��������ҵ��㣬�����͵���Ϣ
����ֵ:ת������ֽ���*/
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

/*�������ݷ��͵�ͨ��*/
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes) {
    return m_channel;
}