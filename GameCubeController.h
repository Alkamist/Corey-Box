#ifndef GAMECUBECONTROLLER_H
#define GAMECUBECONTROLLER_H

#include "UnipolarControl.h"
#include "BipolarControl.h"

struct GameCubeController
{
    // Digital values
    const Activator* a;
    const Activator* b;
    const Activator* x;
    const Activator* y;
    const Activator* z;
    const Activator* l;
    const Activator* r;
    const Activator* start;
    const Activator* dLeft;
    const Activator* dRight;
    const Activator* dDown;
    const Activator* dUp;

    // Analog values
    const UnipolarControl* lAnalog;
    const UnipolarControl* rAnalog;
    const BipolarControl* lsX;
    const BipolarControl* lsY;
    const BipolarControl* cX;
    const BipolarControl* cY;
};

#endif // GAMECUBECONTROLLER_H