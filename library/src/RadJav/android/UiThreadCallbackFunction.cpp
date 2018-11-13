#include "android/UiThreadCallbackFunction.h"
#include "android/UiThreadDispatcher.h"

UiThreadCallbackFunction::UiThreadCallbackFunction(UiThreadDispatcher* dispatcher,
                                                   UiThreadCallbackFunctionType function, void* data)
: _dispatcher(dispatcher),
  _function(function),
  _data(data)
{
}

UiThreadDispatcher* UiThreadCallbackFunction::getDispatcher() const
{
    return _dispatcher;
}

void UiThreadCallbackFunction::operator ()(JNIEnv* env)
{
    _function(env, _data);
}
