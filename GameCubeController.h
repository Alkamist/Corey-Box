#ifndef GAMECUBECONTROLLER_H
#define GAMECUBECONTROLLER_H

#include "Control.h"

struct GameCubeController
{
    GameCubeController()
    : lsX(0.5, Range<double>(Bounds<double>(0.0, 1.0)), Range<double>(CenterAndMagnitude<double>(0.5, 0.1))),
      lsY(0.5, Range<double>(Bounds<double>(0.0, 1.0)), Range<double>(CenterAndMagnitude<double>(0.5, 0.1))),
      cX(0.5, Range<double>(Bounds<double>(0.0, 1.0)), Range<double>(CenterAndMagnitude<double>(0.5, 0.1))),
      cY(0.5, Range<double>(Bounds<double>(0.0, 1.0)), Range<double>(CenterAndMagnitude<double>(0.5, 0.1)))
    {}

    virtual void update();

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



void GameCubeController::update()
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

#endif // GAMECUBECONTROLLER_H