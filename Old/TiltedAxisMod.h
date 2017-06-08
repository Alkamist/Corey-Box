#ifndef TILTEDAXISMOD_H
#define TILTEDAXISMOD_H

#include "DigitalButton.h"

class TiltedAxisMod
{
public:
    TiltedAxisMod::TiltedAxisMod(DigitalButton& modButton
                                 DigitalButton& tiltButton
                                 unsigned int tiltTime)
     : _regularMod(modButton),
       _tiltMod(tiltButton)
    {}

    void update();

    void setValue(double value);
    double getValue();

    inline void resetTiltTimer() { _tiltTimer = 0; }

private:
    AxisMod _regularMod;
    AxisMod _tiltMod;

    elapsedMillis _tiltTimer;
    unsigned int _tiltTime;
};

void TiltedAxisMod::update()
{
    _regularMod.update();
    _tiltMod.update();
}

void TiltedAxisMod::setValue(double value);
{
    _regularMod.setValue(value);
    _tiltMod.setValue(value);
}

double TiltedAxisMod::getValue()
{
    if (_tiltTimer > _tiltTime)
        _regularMod.getValue();
    else
        _tiltMod.getValue();
}

#endif // TILTEDAXISMOD_H