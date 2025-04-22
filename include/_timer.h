#ifndef _TIMER_H
#define _TIMER_H

#include<_common.h>

class _timer
{
    public:
        _timer();
        virtual ~_timer();

        clock_t startTime;

        clock_t getTicks();
        void reset();

        void pauseTime();
        void resumeTime();

    protected:

    private:
};

#endif // _TIMER_H
