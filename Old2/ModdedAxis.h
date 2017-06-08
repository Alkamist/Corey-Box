#ifndef MODDEDAXIS_H
#define MODDEDAXIS_H

#include "DigitalButton.h"
#include "TwoButtonAxis.h"
#include "ModList.h"
#include "AxisMod.h"

class ModdedAxis : public TwoButtonAxis
{
public:
    ModdedAxis(ModList modList,
               DigitalButton& mod1Button,
               DigitalButton& mod2Button,
               DigitalButton& mod3Button,
               DigitalButton& tiltButton,
               DigitalButton& tiltTempDisableButton,
               unsigned int tiltTempDisableTime)
     : _modList(modList),
       _mod1(mod1Button),
       _mod2(mod2Button),
       _mod3(mod3Button),
       _tilt(tiltButton),
       _tiltTempDisableButton(tiltTempDisableButton),
       _tiltTempDisableTime(tiltTempDisableTime)
    {}

    void update();

private:
    AxisMod _mod1;
    AxisMod _mod2;
    AxisMod _mod3;
    AxisMod _tilt;

    DigitalButton* _tiltTempDisableButton;

    ModList _modList;

    static elapsedMillis _tiltTimer;

    unsigned int _tiltTempDisableTime;
    elapsedMillis _tiltTempDisableTimer;
};



void ModdedAxis::update()
{
    TwoButtonAxis::update();

    double outputValue = getValue();

    _mod1.setValue(outputValue);
    _mod2.setValue(outputValue);
    _mod3.setValue(outputValue);
    _tilt.setValue(outputValue);

    _mod1.update();
    _mod2.update();
    _mod3.update();
    _tilt.update();

    if (hasChanged())
        resetTiltTimer();

    if (_tiltTempDisableButton->justPressed())
        tempDisableTilt();

    bool tiltTempDisabled = _tiltTempDisableTimer < _tiltTempDisableTime;

    bool tiltNotActivated = !_tiltButton.pressed()
                          || _tiltTimer > tiltTime
                          || tiltTempDisabled
                          || _tiltButton == null;



    setValue(outputValue);
}

#endif // MODDEDAXIS_H