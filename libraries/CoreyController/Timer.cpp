#include <stdint.h>
#include "Arduino.h"
#include "Timer.h"

uint8_t FramesElapsed::_writesPerHalfFrame = 1;
uint32_t FramesElapsed::_halfFramesCounted = 0;
uint16_t FramesElapsed::_writeCounter = 0;

void FramesElapsed::count()
{
    ++_writeCounter;

    if (_writeCounter >= _writesPerHalfFrame)
    {
        _writeCounter = 0;
        ++_halfFramesCounted;
    }
}

const uint16_t FramesElapsed::getWrites()
{
    return _writeCounter;
}

void FramesElapsed::resetWrites()
{
    _writeCounter = 0;
}

const uint32_t FramesElapsed::getHalfFrames()
{
    return _halfFramesCounted;
}