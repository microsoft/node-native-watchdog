/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include <node.h>
#include <uv.h>
#include <unistd.h>
#include <time.h>
#include <v8-inspector.h>

using namespace v8;

Isolate *isolate;                       // captured v8 isolate
double timeout;                         // configured timeout (in millis)
uv_thread_t monitor_thread_id;          // id of the monitor thread
uv_rwlock_t _last_ping_time_lock;       // rw lock for the ping time
unsigned long long _last_ping_time = 0; // last ping time (when the JS event loop was alive)

unsigned long long epoch_millis()
{
    time_t seconds;
    time(&seconds);
    return (unsigned long long)seconds * 1000;
}

void init_last_ping_time()
{
    uv_rwlock_init(&_last_ping_time_lock);
    _last_ping_time = epoch_millis();
}
void destroy_last_ping_time()
{
    uv_rwlock_destroy(&_last_ping_time_lock);
}
unsigned long long read_last_ping_time()
{
    unsigned long long r;
    uv_rwlock_rdlock(&_last_ping_time_lock);
    r = _last_ping_time;
    uv_rwlock_rdunlock(&_last_ping_time_lock);
    return r;
}
void write_last_ping_time(unsigned long long value)
{
    uv_rwlock_wrlock(&_last_ping_time_lock);
    _last_ping_time = value;
    uv_rwlock_wrunlock(&_last_ping_time_lock);
}

void monitor_stop(Isolate *isolate, void *data)
{
    fprintf(stderr, "The module native-watchdog has detected that the event loop is unresponsive.\n");
    fprintf(stderr, "Here is the JavaScript stack trace:\n");
    fprintf(stderr, "======================================native-watchdog======================================\n");
    Message::PrintCurrentStackTrace(isolate, stderr);
    fprintf(stderr, "======================================native-watchdog======================================\n");
    fprintf(stderr, "The module native-watchdog will now terminate the process with exit code 87.\n");

    // Choosing a value different than any of the ones at
    // https://github.com/nodejs/node/blob/master/doc/api/process.md#exit-codes
    exit(87);
}

void monitor(void *arg)
{
    while (true)
    {
        unsigned long long last_ping_time = read_last_ping_time();
        unsigned long long delta = epoch_millis() - last_ping_time;

        if (delta > timeout)
        {
            // More time than allowed via `timeout` has passed since we've been pinged
            isolate->RequestInterrupt(monitor_stop, NULL);
        }

        sleep(1);
    }
}

void _Start(const FunctionCallbackInfo<Value> &args)
{
    isolate = args.GetIsolate();
    timeout = Local<Number>::Cast(args[0])->Value();

    init_last_ping_time();
    uv_thread_create(&monitor_thread_id, monitor, NULL);
    args.GetReturnValue().SetUndefined();
}

void _Ping(const FunctionCallbackInfo<Value> &args)
{
    write_last_ping_time(epoch_millis());
    args.GetReturnValue().SetUndefined();
}

void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "start", _Start);
    NODE_SET_METHOD(exports, "ping", _Ping);
}

NODE_MODULE(addon, init)
