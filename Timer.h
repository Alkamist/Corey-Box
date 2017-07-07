#ifndef TIMER_H
#define TIMER_H

class MillisElapsed
{
private:
    uint16_t _ms;
public:
    MillisElapsed()                                       { _ms = millis(); }
    MillisElapsed(uint16_t value)                         { _ms = millis() - value; }
    MillisElapsed(const MillisElapsed& input)             { _ms = input._ms; }

    operator uint16_t() const                             { return millis() - _ms; }
    MillisElapsed& operator =(const MillisElapsed& input) { _ms = input._ms; return *this; }
    MillisElapsed& operator =(uint16_t value)             { _ms = millis() - value; return *this; }

    MillisElapsed& operator +=(uint16_t value)            { _ms -= value; return *this; }
    MillisElapsed& operator -=(uint16_t value)            { _ms += value; return *this; }
    MillisElapsed operator +(uint16_t value) const        { MillisElapsed output(*this); output._ms -= value; return output; }
    MillisElapsed operator -(uint16_t value) const        { MillisElapsed output(*this); output._ms += value; return output; }
};

// This is a simple timer class based in milliseconds.
class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    void reset()                                                    { _timer = 0; }

    void setTargetTime(const uint16_t targetTime)                   { _targetTime = targetTime; }
    void subtractTargetTime()                                       { _timer -= _targetTime; }

    //=================== OPERATORS ===================

    operator MillisElapsed() const                                  { return _timer; }
    operator uint16_t() const                                       { return _timer; }
    operator bool() const                                           { return _timer >= _targetTime; }

    Timer& operator=(const Timer& value)                            { _timer = value._timer; return *this; }
    Timer& operator=(const MillisElapsed& value)                    { _timer = value; return *this; }
    Timer& operator=(const uint16_t value)                          { _timer = value; return *this; }

    Timer& operator +=(const uint16_t value)                        { _timer += value; return *this; }
    Timer& operator -=(const uint16_t value)                        { _timer -= value; return *this; }
    Timer operator +(const uint16_t value) const                    { Timer output(*this); output._timer += value; return output; }
    Timer operator -(const uint16_t value) const                    { Timer output(*this); output._timer -= value; return output; }

    const bool operator !() const                                   { return !(_timer >= _targetTime); }
    const bool operator &&(const bool value) const                  { return (_timer >= _targetTime) && value; }
    const bool operator ||(const bool value) const                  { return (_timer >= _targetTime) || value; }
    const bool operator ==(const bool value) const                  { return (_timer >= _targetTime) == value; }
    const bool operator !=(const bool value) const                  { return (_timer >= _targetTime) != value; }

    const bool operator >(const uint16_t value) const               { return _timer > value; }
    const bool operator <(const uint16_t value) const               { return _timer < value; }
    const bool operator >=(const uint16_t value) const              { return _timer >= value; }
    const bool operator <=(const uint16_t value) const              { return _timer <= value; }

private:
    MillisElapsed _timer;
    uint16_t _targetTime;
};

#endif // TIMER_H