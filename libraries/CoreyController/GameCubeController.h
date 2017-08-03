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
      _lsYOffset(0)
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

    void resetLsXTrim()                      { _lsXOffset = 0; }
    void resetLsYTrim()                      { _lsYOffset = 0; }

    const int8_t getLsXOffset() const        { return _lsXOffset; }
    const int8_t getLsYOffset() const        { return _lsYOffset; }

    void setLsXOffset(const int8_t value)    { _lsXOffset = value; }
    void setLsYOffset(const int8_t value)    { _lsYOffset = value; }

private:
    int8_t _lsXOffset;
    int8_t _lsYOffset;
};

#endif // GAMECUBECONTROLLER_H