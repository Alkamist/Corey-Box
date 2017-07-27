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
    {}

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
        lAnalog = report.left;
        rAnalog = report.right;
        lsX = report.xAxis;
        lsY = report.yAxis;
        cX = report.cxAxis;
        cY = report.cyAxis;
    }
};

#endif // GAMECUBECONTROLLERREADER_H
