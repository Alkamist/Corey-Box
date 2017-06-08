#ifndef VIRTUALCONTROLLER_H
#define VIRTUALCONTROLLER_H

#include "Nintendo.h"

class VirtualController : public Gamecube_Data_t
{
public:
    VirtualController();

    // Digital values
    void setA(bool value)          { report.a = value; }
    void setB(bool value)          { report.b = value; }
    void setX(bool value)          { report.x = value; }
    void setY(bool value)          { report.y = value; }
    void setZ(bool value)          { report.z = value; }
    void setL(bool value)          { report.l = value; }
    void setR(bool value)          { report.r = value; }
    void setStart(bool value)      { report.start = value; }
    void setDLeft(bool value)      { report.dleft = value; }
    void setDRight(bool value)     { report.dright = value; }
    void setDDown(bool value)      { report.ddown = value; }
    void setDUp(bool value)        { report.dup = value; }

    // Analog values
    void setLAnalog(uint8_t value) { report.left = value; }
    void setRAnalog(uint8_t value) { report.right = value; }
    void setLsX(uint8_t value)     { report.xAxis = value; }
    void setLsY(uint8_t value)     { report.yAxis = value; }
    void setCX(uint8_t value)      { report.cxAxis = value; }
    void setCY(uint8_t value)      { report.cyAxis = value; }
};



VirtualController::VirtualController()
: Gamecube_Data_t(defaultGamecubeData)
{}

#endif // VIRTUALCONTROLLER_H