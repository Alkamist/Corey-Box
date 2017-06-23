#ifndef GAMECUBECONTROLS_H
#define GAMECUBECONTROLS_H

#include "Control.h"

struct GameCubeControls
{
    void update();

    // Digital values
    Control a;
    Control b;
    Control x;
    Control y;
    Control z;
    Control l;
    Control r;
    Control start;
    Control dLeft;
    Control dRight;
    Control dDown;
    Control dUp;

    // Analog values
    Control lAnalog;
    Control rAnalog;
    Control lsX;
    Control lsY;
    Control cX;
    Control cY;
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