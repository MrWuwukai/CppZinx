#pragma once
class Echo :
    public Irole
{
public:
    Echo();                   // ���캯��
    ~Echo();                  // ��������

    bool Init();              // ��ʼ������
    UserData* ProcMsg(UserData& _poUserData);  // ������Ϣ����
    void Fini();
};

class OutputCtrl :
    public Irole
{
public:
    OutputCtrl();                   // ���캯��
    ~OutputCtrl();                  // ��������

    bool Init();              // ��ʼ������
    UserData* ProcMsg(UserData& _poUserData);  // ������Ϣ����
    void Fini();
    Ichannel* pOut = nullptr;
};

class DateRole :
    public Irole
{
public:
    DateRole();                   // ���캯��
    ~DateRole();                  // ��������

    bool Init();              // ��ʼ������
    UserData* ProcMsg(UserData& _poUserData);  // ������Ϣ����
    void Fini();
    bool isNeedDate = false;
};
