#ifndef RADJAVVM_UITHREADDISPATCHER_H
#define RADJAVVM_UITHREADDISPATCHER_H


class UiThreadDispatcher
{
public:
    virtual ~UiThreadDispatcher() {}

    virtual void uiThreadArrived() = 0;
};
#endif //RADJAVVM_UITHREADDISPATCHER_H
