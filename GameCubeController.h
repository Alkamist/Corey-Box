#ifndef GAMECUBECONTROLLER_H
#define GAMECUBECONTROLLER_H

#include "GameCubeControls.h"
#include "Nintendo.h"

class GameCubeController : public GameCubeControls,
                           public CGamecubeConsole
{
public:
    GameCubeController(const uint8_t pin)
    : GameCubeControls(),
      CGamecubeConsole(pin),
      _rawData(defaultGamecubeData)
    {}

    virtual void update();

private:
    Gamecube_Data_t _rawData;
};



void GameCubeController::update()
{
    // Digital
    if (a.hasChanged())       _rawData.report.a = a.getValue();
    if (b.hasChanged())       _rawData.report.b = b.getValue();
    if (x.hasChanged())       _rawData.report.x = x.getValue();
    if (y.hasChanged())       _rawData.report.y = y.getValue();
    if (z.hasChanged())       _rawData.report.z = z.getValue();
    if (l.hasChanged())       _rawData.report.l = l.getValue();
    if (r.hasChanged())       _rawData.report.r = r.getValue();
    if (start.hasChanged())   _rawData.report.start = start.getValue();
    if (dLeft.hasChanged())   _rawData.report.dleft = dLeft.getValue();
    if (dRight.hasChanged())  _rawData.report.dright = dRight.getValue();
    if (dDown.hasChanged())   _rawData.report.ddown = dDown.getValue();
    if (dUp.hasChanged())     _rawData.report.dup = dUp.getValue();

    // Analog
    if (lAnalog.hasChanged()) _rawData.report.left = lAnalog.getValue() * 255;
    if (rAnalog.hasChanged()) _rawData.report.right = rAnalog.getValue() * 255;
    if (lsX.hasChanged())     _rawData.report.xAxis = lsX.getValue() * 255;
    if (lsY.hasChanged())     _rawData.report.yAxis = lsY.getValue() * 255;
    if (cX.hasChanged())      _rawData.report.cxAxis = cX.getValue() * 255;
    if (cY.hasChanged())      _rawData.report.cyAxis = cY.getValue() * 255;

    GameCubeControls::update();
    write(_rawData);
}


#endif // GAMECUBECONTROLLER_H