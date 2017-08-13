#ifndef TIMER_H
#define TIMER_H

class HalfFramesElapsed
{
public:
    HalfFramesElapsed()                                           { _halfFrames = _halfFramesCounted; }
    HalfFramesElapsed(uint8_t value)                              { _halfFrames = _halfFramesCounted - value; }
    HalfFramesElapsed(const HalfFramesElapsed& input)             { _halfFrames = input._halfFrames; }

    operator uint8_t() const                                      { return _halfFramesCounted - _halfFrames; }
    HalfFramesElapsed& operator =(const HalfFramesElapsed& input) { _halfFrames = input._halfFrames; return *this; }
    HalfFramesElapsed& operator =(uint8_t value)                  { _halfFrames = _halfFramesCounted - value; return *this; }

    HalfFramesElapsed& operator +=(uint8_t value)                 { _halfFrames -= value; return *this; }
    HalfFramesElapsed& operator -=(uint8_t value)                 { _halfFrames += value; return *this; }
    HalfFramesElapsed operator +(uint8_t value) const             { HalfFramesElapsed output(*this); output._halfFrames -= value; return output; }
    HalfFramesElapsed operator -(uint8_t value) const             { HalfFramesElapsed output(*this); output._halfFrames += value; return output; }

    static void count();

    static void setConsoleTiming()                                { _writesPerHalfFrame = 1; }
    static void setDolphinTiming()                                { _writesPerHalfFrame = 6; }

    static const uint32_t getHalfFrames();

private:
    uint8_t _halfFrames;

    static uint8_t _writesPerHalfFrame;
    static uint32_t _halfFramesCounted;
    static uint16_t _writeCounter;
};

// This class is a timer based in half frames. You can set a target time and
// reset the timer, and it will tell you when it has reached the target time.
class Timer
{
public:
    Timer()
    : _targetTime(0)
    {}

    void reset()                                      { _timer = 0; }

    void setTargetTime(const uint8_t targetTime)      { _targetTime = targetTime; }
    void subtractTargetTime()                         { _timer -= _targetTime; }

    //=================== OPERATORS ===================

    operator HalfFramesElapsed() const                { return _timer; }
    operator uint8_t() const                          { return _timer; }
    operator bool() const                             { return _timer >= _targetTime; }

    Timer& operator=(const Timer& value)              { _timer = value._timer; return *this; }
    Timer& operator=(const HalfFramesElapsed& value)      { _timer = value; return *this; }
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
    HalfFramesElapsed _timer;
    uint8_t _targetTime;
};

#endif // TIMER_H