#ifndef TIMER_H
#define TIMER_H

// This is a simple timer class based in microseconds.
class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    explicit Timer(const uint64_t targetTime)
    : _targetTime(targetTime)
    {}

    void reset()                                      { _timer = 0; }

    void setValue(const uint64_t time)                { _timer = time; }
    void setTargetTime(const uint64_t targetTime)     { _targetTime = targetTime; }

    void subtractTargetTime()                         { _timer -= _targetTime; }

    const bool targetTimeReached() const              { return _timer >= _targetTime; }
    const uint64_t getValue() const                   { return _timer; }

    //=================== OPERATORS ===================

    const elapsedMicros operator =(const elapsedMicros value)        { return _timer = value; }
    const elapsedMicros operator +(const elapsedMicros value) const  { return _timer + value; }
    const elapsedMicros operator +=(const elapsedMicros value)       { return _timer += value; }
    const elapsedMicros operator -(const elapsedMicros value) const  { return _timer - value; }
    const elapsedMicros operator -=(const elapsedMicros value)       { return _timer -= value; }
    operator double() const                                          { return static_cast<double>(_timer); }
    operator bool() const                                            { return targetTimeReached(); }
    const bool operator !() const                                    { return !targetTimeReached(); }
    const bool operator &&(const bool value) const                   { return targetTimeReached() && value; }
    const bool operator ||(const bool value) const                   { return targetTimeReached() || value; }
    const bool operator ==(const bool value) const                   { return targetTimeReached() == value; }
    const bool operator !=(const bool value) const                   { return targetTimeReached() != value; }
    const bool operator >(const uint64_t value) const                { return _timer > value; }
    const bool operator <(const uint64_t value) const                { return _timer < value; }
    const bool operator >=(const uint64_t value) const               { return _timer >= value; }
    const bool operator <=(const uint64_t value) const               { return _timer <= value; }

private:
    elapsedMicros _timer;
    uint64_t _targetTime;
};

#endif // TIMER_H
