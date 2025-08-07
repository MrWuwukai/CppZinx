#include "kernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include "ZinxHandler.h"

zinxkernel zinxkernel::kernel;

void zinxkernel::zinx_sendout(std::string _output, Ichannel* _pchannel) {
    sysIODicMsg iodic(sysIODicMsg::IO_OUT);
    ByteMsg byte(_output, iodic);
    _pchannel->handle(&byte);
}

zinxkernel::zinxkernel() {
    int fd = epoll_create(1);
    if (fd >= 0) {
        m_epollFd = fd;
    }
}

zinxkernel::~zinxkernel() {
    if (m_epollFd >= 0) {
        close(m_epollFd);
    }
}

void zinxkernel::run() {
    while (1) {
        //1等输入 (epoll)
        struct epoll_event astEvents[200];
        int ready_count = epoll_wait(m_epollFd, astEvents, 200, -1);
        if (0 == ready_count) {
            if (EINTR == errno) {
                continue;
            }
        }
        //2调用通道的读/写函数
        for (int i = 0; i < ready_count; i++) {
            if (0 != (astEvents[i].events & EPOLLIN)) {
                auto pchannel = static_cast<Ichannel*>(astEvents[i].data.ptr);
                //触发处理责任链
                sysIODicMsg dic(sysIODicMsg::IO_IN);
                pchannel->handle(&dic);
            }
            if (0 != (astEvents[i].events & EPOLLOUT)) {
                auto pchannel = static_cast<Ichannel*>(astEvents[i].data.ptr);
                /*sysIODicMsg dic(sysIODicMsg::IO_OUT);
                pchannel->handle(&dic);*/
                pchannel->flush_out();
                //删掉输出方向的epoll监听
                ModChannel_DelOut(pchannel);
            }
        }
    }
}

void zinxkernel::AddChannel(Ichannel* _pChannel) {
    if (_pChannel->init()) {
        //将参数和文件描述符0(std in)关联起来 (epoll_ctl)
        struct epoll_event stEvent;
        stEvent.events = EPOLLIN;
        stEvent.data.ptr = _pChannel;
        epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);
    }
}

void zinxkernel::DelChannel(Ichannel* _pChannel) {
    epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
    _pChannel->fini();
}

void zinxkernel::ModChannel_Add0ut(Ichannel* _pChannel) {
    struct epoll_event stEvent;
    stEvent.events = EPOLLIN | EPOLLOUT;
    stEvent.data.ptr = _pChannel;
    epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

void zinxkernel::ModChannel_DelOut(Ichannel* _pChannel) {
    struct epoll_event stEvent;
    stEvent.events = EPOLLIN;
    stEvent.data.ptr = _pChannel;
    epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}
