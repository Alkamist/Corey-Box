#ifndef GAMECUBECONTROLLER_H
#define GAMECUBECONTROLLER_H

#include "Activator.h"
#include "Control.h"

struct GameCubeController
{
    GameCubeController()
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
};

#endif // GAMECUBECONTROLLER_H