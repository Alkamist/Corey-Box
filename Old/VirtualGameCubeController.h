#ifndef VIRTUALCONTROLLER_H
#define VIRTUALCONTROLLER_H

#include "Nintendo.h"
#include "GameCubeControls.h"

class VirtualGameCubeController : public CGamecubeConsole
{
public:
    explicit VirtualGameCubeController(const uint8_t pin);

    void update();

    void setControls(const GameCubeControls& controls) { _controls = &controls; }

private:
    const GameCubeControls* _controls;

    Gamecube_Data_t _rawData;
};



void VirtualGameCubeController::update()
{
    if (_controls->a.hasChanged())
        _rawData.report.a = _controls->a;
    if (_controls->b.hasChanged())
        _rawData.report.b = _controls->b;
    if (_controls->x.hasChanged())
        _rawData.report.x = _controls->x;
    if (_controls->y.hasChanged())
        _rawData.report.y = _controls->y;
    if (_controls->z.hasChanged())
        _rawData.report.z = _controls->z;
    if (_controls->l.hasChanged())
        _rawData.report.l = _controls->l;
    if (_controls->r.hasChanged())
        _rawData.report.r = _controls->r;
    if (_controls->start.hasChanged())
        _rawData.report.start = _controls->start;
    if (_controls->dLeft.hasChanged())
        _rawData.report.dleft = _controls->dLeft;
    if (_controls->dRight.hasChanged())
        _rawData.report.dright = _controls->dRight;
    if (_controls->dDown.hasChanged())
        _rawData.report.ddown = _controls->dDown;
    if (_controls->dUp.hasChanged())
        _rawData.report.dup = _controls->dUp;
    if (_controls->lAnalog.hasChanged())
        _rawData.report.left = _controls->lAnalog * 255;
    if (_controls->rAnalog.hasChanged())
        _rawData.report.right = _controls->rAnalog * 255;
    if (_controls->lsX.hasChanged())
        _rawData.report.xAxis = _controls->lsX * 255;
    if (_controls->lsY.hasChanged())
        _rawData.report.yAxis = _controls->lsY * 255;
    if (_controls->cX.hasChanged())
        _rawData.report.cxAxis = _controls->cX * 255;
    if (_controls->cY.hasChanged())
        _rawData.report.cyAxis = _controls->cY * 255;

    write(_rawData);
}

VirtualGameCubeController::VirtualGameCubeController(const uint8_t pin)
: CGamecubeConsole(pin),
  _rawData(defaultGamecubeData)
{}

#endif // VIRTUALCONTROLLER_H