/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include <node.h>
#include <uv.h>
#include <time.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#else
#include <unistd.h>
#endif

using namespace v8;

bool w_processIsRunning(long pid)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return (ret == WAIT_TIMEOUT);
#else
    return (kill(pid, 0) == 0);
#endif
}

void w_sleep(int seconds)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}

long w_parentpid = 0; // id of the parent process
uv_thread_t w_monitor_thread_id; // id of the monitor thread

void w_monitor(void *arg)
{
    while (true)
    {
        if (!w_processIsRunning(w_parentpid))
        {
            w_sleep(5);
            exit(87);
        }
        w_sleep(1);
    }
}

void _Start(const FunctionCallbackInfo<Value> &args)
{
    w_parentpid = (long)Local<Number>::Cast(args[0])->Value();
    uv_thread_create(&w_monitor_thread_id, w_monitor, NULL);
}

void _Exit(const FunctionCallbackInfo<Value> &args)
{
    exit(args[0]->Int32Value());
}

void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "start", _Start);
    NODE_SET_METHOD(exports, "exit", _Exit);
}

NODE_MODULE(addon, init)
