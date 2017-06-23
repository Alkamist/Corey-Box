#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    explicit Timer(const unsigned int targetTime)
    : _targetTime(targetTime)
    {}

    void reset()                                      { _timer = 0; }
    void setTargetTime(const unsigned int targetTime) { _targetTime = targetTime; }

    const bool targetTimeReached() const              { return _timer >= _targetTime; }
    const elapsedMillis& getValue() const             { return _timer; }

private:
    elapsedMillis _timer;
    unsigned int _targetTime;
};

#endif // TIMER_H
