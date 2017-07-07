#ifndef GAMECUBECONTROLLERREADER_H
#define GAMECUBECONTROLLERREADER_H

#include "Nintendo.h"
#include "GameCubeController.h"
#include "ScaleBipolar.h"

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
        lsX = scaleBipolar(report.xAxis, 80);
        lsY = scaleBipolar(report.yAxis, 80);
        cX = scaleBipolar(report.cxAxis, 80);
        cY = scaleBipolar(report.cyAxis, 80);
    }
};

#endif // GAMECUBECONTROLLERREADER_H
