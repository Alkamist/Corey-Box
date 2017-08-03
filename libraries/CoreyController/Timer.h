#ifndef TIMER_H
#define TIMER_H

// This class is a timer based in microseconds. You can set a target time and
// reset the timer, and it will tell you when it has reached the target time.
class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    void reset()                                       { _timer = 0; }

    void setTargetTime(const uint32_t targetTime)      { _targetTime = targetTime; }
    void subtractTargetTime()                          { _timer -= _targetTime; }

    //=================== OPERATORS ===================

    operator elapsedMicros() const                     { return _timer; }
    operator uint32_t() const                          { return _timer; }
    operator bool() const                              { return _timer >= _targetTime; }

    Timer& operator=(const Timer& value)               { _timer = value._timer; return *this; }
    Timer& operator=(const elapsedMicros& value)       { _timer = value; return *this; }
    Timer& operator=(const uint32_t value)             { _timer = value; return *this; }

    Timer& operator +=(const uint32_t value)           { _timer += value; return *this; }
    Timer& operator -=(const uint32_t value)           { _timer -= value; return *this; }
    Timer operator +(const uint32_t value) const       { Timer output(*this); output._timer += value; return output; }
    Timer operator -(const uint32_t value) const       { Timer output(*this); output._timer -= value; return output; }

    const bool operator !() const                      { return !(_timer >= _targetTime); }
    const bool operator &&(const bool value) const     { return (_timer >= _targetTime) && value; }
    const bool operator ||(const bool value) const     { return (_timer >= _targetTime) || value; }
    const bool operator ==(const bool value) const     { return (_timer >= _targetTime) == value; }
    const bool operator !=(const bool value) const     { return (_timer >= _targetTime) != value; }

    const bool operator >(const uint32_t value) const  { return _timer > value; }
    const bool operator <(const uint32_t value) const  { return _timer < value; }
    const bool operator >=(const uint32_t value) const { return _timer >= value; }
    const bool operator <=(const uint32_t value) const { return _timer <= value; }

private:
    elapsedMicros _timer;
    uint32_t _targetTime;
};

#endif // TIMER_H