#ifndef RADJAVVM_SELFDESTROYABLECALLABLE_H
#define RADJAVVM_SELFDESTROYABLECALLABLE_H

#include "Jni.h"
#include <functional>

class UiThreadDispatcher;
class AppInterface;

typedef std::function<void (JNIEnv* env, void* data)> UiThreadCallbackFunctionType;

class UiThreadCallbackFunction
{
public:
    UiThreadCallbackFunction() = delete;
    UiThreadCallbackFunction(const UiThreadCallbackFunction&) = delete;

    UiThreadCallbackFunction(UiThreadDispatcher* dispatcher,
                             UiThreadCallbackFunctionType function, void* data);

    UiThreadDispatcher* getDispatcher() const;

    void operator ()(JNIEnv* env);

private:
    UiThreadDispatcher* _dispatcher;
    UiThreadCallbackFunctionType _function;
    void* _data;
};

#endif //RADJAVVM_SELFDESTROYABLECALLABLE_H
