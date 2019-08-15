/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "common.h"
#include <time.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__WIN32)
#ifndef WIN32
#define WIN32
#endif
#endif

#if !defined(WIN32)
#include <unistd.h>
#include <signal.h>
#else
#include <windows.h>
#endif

namespace
{

using WorkerInfo = struct
{
    int64_t parent_pid = 0;
    napi_async_work request = nullptr;
};

WorkerInfo worker_info;

bool w_processIsRunning(long pid)
{
#if defined(WIN32)
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
#if defined(WIN32)
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}

void Execute(napi_env env, void *data)
{
    auto *info = static_cast<WorkerInfo *>(data);

    if (info != &worker_info)
    {
        return;
    }

    while (true)
    {
        if (!w_processIsRunning(info->parent_pid))
        {
            NAPI_CALL_RETURN_VOID(env, napi_delete_async_work(env, info->request));
            w_sleep(5);
            exit(87);
            return;
        }
        w_sleep(1);
    }
}

} // namespace

napi_value Start(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1];
    NAPI_CALL(env,
              napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr));

    NAPI_ASSERT(env, argc == 1, "Wrong number of arguments, expected 1.");

    napi_valuetype t;
    NAPI_CALL(env, napi_typeof(env, argv[0], &t));
    NAPI_ASSERT(env, t == napi_number,
                "Wrong argument, number expected.");

    NAPI_CALL(env, napi_get_value_int64(env, argv[0], &worker_info.parent_pid));

    napi_value resource_name;
    NAPI_CALL(env, napi_create_string_utf8(
                       env, "StartWorkerProcess", NAPI_AUTO_LENGTH, &resource_name));
    NAPI_CALL(env, napi_create_async_work(env, nullptr, resource_name,
                                          Execute, nullptr, &worker_info, &worker_info.request));
    NAPI_CALL(env, napi_queue_async_work(env, worker_info.request));

    return nullptr;
}

napi_value Exit(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1];
    NAPI_CALL(env,
              napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr));

    NAPI_ASSERT(env, argc == 1, "Wrong number of arguments, expected 1.");

    napi_valuetype t;
    NAPI_CALL(env, napi_typeof(env, argv[0], &t));
    NAPI_ASSERT(env, t == napi_number,
                "Wrong argument, number expected.");

    int32_t code;
    NAPI_CALL(env, napi_get_value_int32(env, argv[0], &code));

    exit(code);

    return nullptr;
}

napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_PROPERTY("start", Start),
        DECLARE_NAPI_PROPERTY("exit", Exit)};

    NAPI_CALL(env, napi_define_properties(
                       env, exports, sizeof(properties) / sizeof(*properties), properties));

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);
