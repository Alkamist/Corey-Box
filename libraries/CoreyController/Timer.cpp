#include <stdint.h>
#include "Arduino.h"
#include "Timer.h"

uint8_t HalfFramesElapsed::_writesPerHalfFrame = 1;
uint32_t HalfFramesElapsed::_halfFramesCounted = 0;
uint16_t HalfFramesElapsed::_writeCounter = 0;

void HalfFramesElapsed::count()
{
    ++_writeCounter;

    if (_writeCounter >= _writesPerHalfFrame)
    {
        _writeCounter = 0;
        ++_halfFramesCounted;
    }
}

const uint32_t HalfFramesElapsed::getHalfFrames()
{
    return _halfFramesCounted;
}