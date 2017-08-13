#ifndef GAMECUBEOUTPUT_H
#define GAMECUBEOUTPUT_H

#include "Nintendo.h"
#include "GameCubeController.h"
#include "Timer.h"

// This class is how you output to a GameCube. Set the pin, connect a controller,
// and call process() to write the data.
class GameCubeOutput : public CGamecubeConsole
{
public:
    GameCubeOutput(const uint8_t pin)
    : CGamecubeConsole(pin),
      _rawData(defaultGamecubeData)
    {}

    void process();

    void connectController(const GameCubeController& controller) { _controller = &controller; }

private:
    const GameCubeController* _controller;

    Gamecube_Data_t _rawData;

    void writeData();
};



void GameCubeOutput::process()
{
    if (_controller)
        writeData();
}

void GameCubeOutput::writeData()
{
    // Digital
    if (_controller->a.justChanged())       _rawData.report.a = _controller->a;
    if (_controller->b.justChanged())       _rawData.report.b = _controller->b;
    if (_controller->x.justChanged())       _rawData.report.x = _controller->x;
    if (_controller->y.justChanged())       _rawData.report.y = _controller->y;
    if (_controller->z.justChanged())       _rawData.report.z = _controller->z;
    if (_controller->l.justChanged())       _rawData.report.l = _controller->l;
    if (_controller->r.justChanged())       _rawData.report.r = _controller->r;
    if (_controller->start.justChanged())   _rawData.report.start = _controller->start;
    if (_controller->dLeft.justChanged())   _rawData.report.dleft = _controller->dLeft;
    if (_controller->dRight.justChanged())  _rawData.report.dright = _controller->dRight;
    if (_controller->dDown.justChanged())   _rawData.report.ddown = _controller->dDown;
    if (_controller->dUp.justChanged())     _rawData.report.dup = _controller->dUp;

    // Analog Triggers
    if (_controller->lAnalog.justChanged()) _rawData.report.left = _controller->lAnalog;
    if (_controller->rAnalog.justChanged()) _rawData.report.right = _controller->rAnalog;

    // Analog Axes
    if (_controller->lsX.justChanged())     _rawData.report.xAxis = _controller->lsX;
    if (_controller->lsY.justChanged())     _rawData.report.yAxis = _controller->lsY;
    if (_controller->cX.justChanged())      _rawData.report.cxAxis = _controller->cX;
    if (_controller->cY.justChanged())      _rawData.report.cyAxis = _controller->cY;

    write(_rawData);
    HalfFramesElapsed::count();
}

#endif // GAMECUBEOUTPUT_H
