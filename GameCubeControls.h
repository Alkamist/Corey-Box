#ifndef GAMECUBECONTROLS_H
#define GAMECUBECONTROLS_H

#include "ControlValue.h"
#include "AnalogAxis.h"

class GameCubeControls
{
public:
    void update();

    // Digital values
    ControlValue a;
    ControlValue b;
    ControlValue x;
    ControlValue y;
    ControlValue z;
    ControlValue l;
    ControlValue r;
    ControlValue start;
    ControlValue dLeft;
    ControlValue dRight;
    ControlValue dDown;
    ControlValue dUp;

    // Analog values
    ControlValue lAnalog;
    ControlValue rAnalog;
    AnalogAxis lsX;
    AnalogAxis lsY;
    AnalogAxis cX;
    AnalogAxis cY;
};



void GameCubeControls::update()
{
    a.update();
    b.update();
    x.update();
    y.update();
    z.update();
    l.update();
    r.update();
    start.update();
    dLeft.update();
    dRight.update();
    dDown.update();
    dUp.update();
    lAnalog.update();
    rAnalog.update();
    lsX.update();
    lsY.update();
    cX.update();
    cY.update();
}

#endif // GAMECUBECONTROLS_H