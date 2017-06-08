#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    void reset()             { _timer = 0; }

    elapsedMillis getValue() { return _timer; }

private:
    elapsedMillis _timer;
};

#endif // TIMER_H