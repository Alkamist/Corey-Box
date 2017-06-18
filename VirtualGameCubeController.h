#ifndef VIRTUALCONTROLLER_H
#define VIRTUALCONTROLLER_H

#include "Nintendo.h"
#include "GameCubeControls.h"

class VirtualGameCubeController : public CGamecubeConsole
{
public:
    explicit VirtualGameCubeController(const uint8_t pin);

    void update();

    void setControls(GameCubeControls controls) { _controls = controls; }

private:
    GameCubeControls _controls;

    Gamecube_Data_t _rawData;
};



void VirtualGameCubeController::update()
{
    _controls.update();

    _rawData.report.a = _controls.a;
    _rawData.report.b = _controls.b;
    _rawData.report.x = _controls.x;
    _rawData.report.y = _controls.y;
    _rawData.report.z = _controls.z;
    _rawData.report.l = _controls.l;
    _rawData.report.r = _controls.r;
    _rawData.report.start = _controls.start;
    _rawData.report.dleft = _controls.dLeft;
    _rawData.report.dright = _controls.dRight;
    _rawData.report.ddown = _controls.dDown;
    _rawData.report.dup = _controls.dUp;
    _rawData.report.left = _controls.lAnalog * 255;
    _rawData.report.right = _controls.rAnalog * 255;
    _rawData.report.xAxis = _controls.lsX * 255;
    _rawData.report.yAxis = _controls.lsY * 255;
    _rawData.report.cxAxis = _controls.cX * 255;
    _rawData.report.cyAxis = _controls.cY * 255;

    write(_rawData);
}

VirtualGameCubeController::VirtualGameCubeController(const uint8_t pin)
: CGamecubeConsole(pin),
  _rawData(defaultGamecubeData)
{}

#endif // VIRTUALCONTROLLER_H