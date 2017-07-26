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
      _controller(pin),
      _isConnected(false)
    {}

    void process()
    {
        if (_controller.read())
        {
            _isConnected = true;
            auto report = _controller.getReport();
            updateControls(report);
        }
        else
            _isConnected = false;
    }

    const bool isConnected() const { return _isConnected; }

private:
    CGamecubeController _controller;

    bool _isConnected;

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
        cY = screport.cyAxis;
    }
};

#endif // GAMECUBECONTROLLERREADER_H
