#pragma once
class Echo :
    public Irole
{
public:
    Echo();                   // 构造函数
    ~Echo();                  // 析构函数

    bool Init();              // 初始化方法
    UserData* ProcMsg(UserData& _poUserData);  // 处理消息方法
    void Fini();
};

class OutputCtrl :
    public Irole
{
public:
    OutputCtrl();                   // 构造函数
    ~OutputCtrl();                  // 析构函数

    bool Init();              // 初始化方法
    UserData* ProcMsg(UserData& _poUserData);  // 处理消息方法
    void Fini();
    Ichannel* pOut = nullptr;
};

class DateRole :
    public Irole
{
public:
    DateRole();                   // 构造函数
    ~DateRole();                  // 析构函数

    bool Init();              // 初始化方法
    UserData* ProcMsg(UserData& _poUserData);  // 处理消息方法
    void Fini();
    bool isNeedDate = false;
};
