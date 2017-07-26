#ifndef GAMECUBECONTROLLER_H
#define GAMECUBECONTROLLER_H

#include "Activator.h"
#include "Control.h"

// This is the basic data structure for a GameCube controller. It can trim
// its left stick axes up or down to provide a defense for bad calibration.
struct GameCubeController
{
    GameCubeController()
    : _lsXOffset(0),
      _lsYOffset(0),
      _lsXOffsetPrev(0),
      _lsYOffsetPrev(0)
    {}

    // Digital values
    Activator a;
    Activator b;
    Activator x;
    Activator y;
    Activator z;
    Activator l;
    Activator r;
    Activator start;
    Activator dLeft;
    Activator dRight;
    Activator dDown;
    Activator dUp;

    // Analog values
    Control lAnalog;
    Control rAnalog;
    Control lsX;
    Control lsY;
    Control cX;
    Control cY;

    void endCycle()
    {
        _lsXOffsetPrev = _lsXOffset;
        _lsYOffsetPrev = _lsYOffset;

        a.endCycle();
        b.endCycle();
        x.endCycle();
        y.endCycle();
        z.endCycle();
        l.endCycle();
        r.endCycle();
        start.endCycle();
        dLeft.endCycle();
        dRight.endCycle();
        dDown.endCycle();
        dUp.endCycle();
        lAnalog.endCycle();
        rAnalog.endCycle();
        lsX.endCycle();
        lsY.endCycle();
        cX.endCycle();
        cY.endCycle();
    }

    void trimLsXDown()
    {
        if (_lsXOffset > -15)
            --_lsXOffset;
    }

    void trimLsXUp()
    {
        if (_lsXOffset < 15)
            ++_lsXOffset;
    }

    void trimLsYDown()
    {
        if (_lsYOffset > -15)
            --_lsYOffset;
    }

    void trimLsYUp()
    {
        if (_lsYOffset < 15)
            ++_lsYOffset;
    }

    void resetLsXTrim() { _lsXOffset = 0; }
    void resetLsYTrim() { _lsYOffset = 0; }

    const int8_t getLsXOffset() const        { return _lsXOffset; }
    const int8_t getLsYOffset() const        { return _lsYOffset; }

    const bool lsXOffsetJustChanged() const  { return _lsXOffset != _lsXOffsetPrev; }
    const bool lsYOffsetJustChanged() const  { return _lsYOffset != _lsYOffsetPrev; }

private:
    int8_t _lsXOffset;
    int8_t _lsYOffset;

    int8_t _lsXOffsetPrev;
    int8_t _lsYOffsetPrev;
};

#endif // GAMECUBECONTROLLER_H