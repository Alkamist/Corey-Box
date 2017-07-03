#ifndef GAMECUBECONTROLLERREADER_H
#define GAMECUBECONTROLLERREADER_H

#include "Nintendo.h"
#include "GameCubeController.h"

class GameCubeControllerReader : public GameCubeController
{
public:
    GameCubeControllerReader(const uint8_t pin)
    : GameCubeController(),
      _controller(pin)
    {
        _axisRange.setCenter(0.5 * 255.0);
        _axisRange.setMagnitude(0.6250 * 0.5 * 255.0);
    }

    void process()
    {
        if (_controller.read())
        {
            auto report = _controller.getReport();
            updateControls(report);
        }
    }

private:
    CGamecubeController _controller;

    Range<float> _axisRange;

    void updateControls(const Gamecube_Report_t& report)
    {
        // Digital
        a = report.a;
        b = report.b;
        x = report.x;
        y = report.y;
        z = report.z;
        l = report.l;
        r = report.r;
        start = report.start;
        dLeft = report.dleft;
        dRight = report.dright;
        dDown = report.ddown;
        dUp = report.dup;

        // Analog
        lAnalog = report.left / 255.0;
        rAnalog = report.right / 255.0;
        lsX = lsX.getValueRange().rescaleValue(report.xAxis, _axisRange);
        lsY = lsY.getValueRange().rescaleValue(report.yAxis, _axisRange);
        cX = cX.getValueRange().rescaleValue(report.cxAxis, _axisRange);
        cY = cY.getValueRange().rescaleValue(report.cyAxis, _axisRange);
    }
};

#endif // GAMECUBECONTROLLERREADER_H
