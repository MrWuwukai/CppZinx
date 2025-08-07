#include <iostream>
#include "kernel.h"
#include "StdInChannel1.h"
#include "StdOutChannel1.h"
#include "FIFO_channel.h"
#include "ProcessFunc.h"

using namespace std;

int main()
{
    stdin_channel in_channel;
    stdout_channel out_channel;
    ProcessFunc pf;
    pf.m_out = &out_channel;
    in_channel.m_proc = &pf;

    zinxkernel& kernel = zinxkernel::GetInstance();
    kernel.AddChannel(&in_channel);
    kernel.AddChannel(&out_channel);

    //FIFO_channel fifo_input("input", true);
    //FIFO_channel fifo_output("output", false);
    //fifo_input.m_out = &fifo_output;
    //kernel.AddChannel(&fifo_input);
    //kernel.AddChannel(&fifo_output);

    kernel.run();
}