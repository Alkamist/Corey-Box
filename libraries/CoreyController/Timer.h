#ifndef TIMER_H
#define TIMER_H

const uint8_t microsToFrames(const uint32_t us)
{
    return us / 16666;
}

class FramesElapsed
{
private:
    uint8_t _frames;
public:
    FramesElapsed()                                       { _frames = microsToFrames(micros()); }
    FramesElapsed(uint8_t value)                          { _frames = microsToFrames(micros()) - value; }
    FramesElapsed(const FramesElapsed& input)             { _frames = input._frames; }

    operator uint8_t() const                              { return microsToFrames(micros()) - _frames; }
    FramesElapsed& operator =(const FramesElapsed& input) { _frames = input._frames; return *this; }
    FramesElapsed& operator =(uint8_t value)              { _frames = microsToFrames(micros()) - value; return *this; }

    FramesElapsed& operator +=(uint8_t value)             { _frames -= value; return *this; }
    FramesElapsed& operator -=(uint8_t value)             { _frames += value; return *this; }
    FramesElapsed operator +(uint8_t value) const         { FramesElapsed output(*this); output._frames -= value; return output; }
    FramesElapsed operator -(uint8_t value) const         { FramesElapsed output(*this); output._frames += value; return output; }
};

// This class is a timer based in frames. You can set a target time and
// reset the timer, and it will tell you when it has reached the target time.
class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    void reset()                                       { _timer = 0; }

    void setTargetTime(const uint8_t targetTime)      { _targetTime = targetTime; }
    void subtractTargetTime()                          { _timer -= _targetTime; }

    //=================== OPERATORS ===================

    operator FramesElapsed() const                    { return _timer; }
    operator uint8_t() const                          { return _timer; }
    operator bool() const                             { return _timer >= _targetTime; }

    Timer& operator=(const Timer& value)              { _timer = value._timer; return *this; }
    Timer& operator=(const FramesElapsed& value)      { _timer = value; return *this; }
    Timer& operator=(const uint8_t value)             { _timer = value; return *this; }

    Timer& operator +=(const uint8_t value)           { _timer += value; return *this; }
    Timer& operator -=(const uint8_t value)           { _timer -= value; return *this; }
    Timer operator +(const uint8_t value) const       { Timer output(*this); output._timer += value; return output; }
    Timer operator -(const uint8_t value) const       { Timer output(*this); output._timer -= value; return output; }

    const bool operator !() const                     { return !(_timer >= _targetTime); }
    const bool operator &&(const bool value) const    { return (_timer >= _targetTime) && value; }
    const bool operator ||(const bool value) const    { return (_timer >= _targetTime) || value; }
    const bool operator ==(const bool value) const    { return (_timer >= _targetTime) == value; }
    const bool operator !=(const bool value) const    { return (_timer >= _targetTime) != value; }

    const bool operator >(const uint8_t value) const  { return _timer > value; }
    const bool operator <(const uint8_t value) const  { return _timer < value; }
    const bool operator >=(const uint8_t value) const { return _timer >= value; }
    const bool operator <=(const uint8_t value) const { return _timer <= value; }

private:
    FramesElapsed _timer;
    uint8_t _targetTime;
};

#endif // TIMER_H