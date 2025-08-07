#pragma once

#include "StdInChannel1.h"
#include "Ichannel.h"
#include <list>

class zinxkernel {  
public:     
    void run();
    void AddChannel(Ichannel* _pChannel);
    void DelChannel(Ichannel* _pChannel);
    void ModChannel_Add0ut(Ichannel* _pChannel);
    void ModChannel_DelOut(Ichannel* _pChannel);
    stdin_channel* GetChannel() {
        return m_channel;
	}
	static zinxkernel& GetInstance() {
        return kernel;
	}
    static void zinx_sendout(std::string _output, Ichannel* _pchannel);
private:
    // µ¥Àý
    zinxkernel();
    virtual ~zinxkernel();
    std::list<Ichannel*> m_channel = nullptr;
    int m_epollFd = -1;
    static zinxkernel kernel;
};