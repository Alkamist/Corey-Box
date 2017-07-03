#ifndef GAMECUBEOUTPUT_H
#define GAMECUBEOUTPUT_H

#include "Nintendo.h"
#include "GameCubeController.h"

// This class is how you output to a GameCube. Set the pin, connect a controller,
// and call process() to write the data.
class GameCubeOutput : public CGamecubeConsole
{
public:
    GameCubeOutput(const uint8_t pin)
    : CGamecubeConsole(pin),
      _rawData(defaultGamecubeData)
    {
        _axisRange.setCenter(0.5 * 255.0);
        _axisRange.setMagnitude(0.6250 * 0.5 * 255.0);
    }

    void process();

    void connectController(const GameCubeController& controller) { _controller = &controller; }

private:
    const GameCubeController* _controller;

    Gamecube_Data_t _rawData;

    Range<float> _axisRange;

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
    if (_controller->a.stateJustChanged())
        _rawData.report.a = _controller->a.isActive();

    if (_controller->b.stateJustChanged())
        _rawData.report.b = _controller->b.isActive();

    if (_controller->x.stateJustChanged())
        _rawData.report.x = _controller->x.isActive();

    if (_controller->y.stateJustChanged())
        _rawData.report.y = _controller->y.isActive();

    if (_controller->z.stateJustChanged())
        _rawData.report.z = _controller->z.isActive();

    if (_controller->l.stateJustChanged())
        _rawData.report.l = _controller->l.isActive();

    if (_controller->r.stateJustChanged())
        _rawData.report.r = _controller->r.isActive();

    if (_controller->start.stateJustChanged())
        _rawData.report.start = _controller->start.isActive();

    if (_controller->dLeft.stateJustChanged())
        _rawData.report.dleft = _controller->dLeft.isActive();

    if (_controller->dRight.stateJustChanged())
        _rawData.report.dright = _controller->dRight.isActive();

    if (_controller->dDown.stateJustChanged())
        _rawData.report.ddown = _controller->dDown.isActive();

    if (_controller->dUp.stateJustChanged())
        _rawData.report.dup = _controller->dUp.isActive();


    // Analog
    if (_controller->lAnalog.valueJustChanged())
        _rawData.report.left = _controller->lAnalog.getValue() * 255;

    if (_controller->rAnalog.valueJustChanged())
        _rawData.report.right = _controller->rAnalog.getValue() * 255;

    if (_controller->lsX.valueJustChanged())
        _rawData.report.xAxis = _axisRange.rescaleValue(_controller->lsX.getValue(), _controller->lsX.getValueRange());

    if (_controller->lsY.valueJustChanged())
        _rawData.report.yAxis = _axisRange.rescaleValue(_controller->lsY.getValue(), _controller->lsY.getValueRange());

    if (_controller->cX.valueJustChanged())
        _rawData.report.cxAxis = _axisRange.rescaleValue(_controller->cX.getValue(), _controller->cX.getValueRange());

    if (_controller->cY.valueJustChanged())
        _rawData.report.cyAxis = _axisRange.rescaleValue(_controller->cY.getValue(), _controller->cY.getValueRange());

    write(_rawData);
}

#endif // GAMECUBEOUTPUT_H
