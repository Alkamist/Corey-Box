#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "Joystick.h"
#include "SingleModAxis.h"
#include "TwoButtonControl.h"

class ButtonOnlyCStick : public Joystick
{
public:
    ButtonOnlyCStick(const Activator& cLeft, const Activator& cRight,
                     const Activator& cDown, const Activator& cUp,
                     const Activator& lsDown, const Activator& lsUp,
                     const Activator& tilt)
    : Joystick(_xAxis, _yAxis),
      _tilt(tilt),
      _lsDown(lsDown),
      _lsUp(lsUp),
      _cLeft(cLeft),
      _cRight(cRight),
      _cDown(cDown),
      _cUp(cUp),
      _xAxis(cLeft, cRight),
      _yAxis(_cDownOut, _cUpOut, _skewOut)
    {}

    virtual void process()
    {
        bool lsDownOrUp = _lsDown || _lsUp;
        bool cLeftOrRight = _cLeft || _cRight;
        bool cDownOrUp = _cDown || _cUp;

        _skewOut.setState(_tilt && lsDownOrUp && cLeftOrRight && !cDownOrUp);

        bool skewDown = _lsDown && _skewOut;
        bool skewUp = _lsUp && _skewOut;

        _cDownOut.setState(_cDown || skewDown);
        _cUpOut.setState(_cUp || skewUp);

        _xAxis.process();
        _yAxis.process();

        Joystick::process();
    }

    virtual void endCycle()
    {
        Joystick::endCycle();
        _xAxis.endCycle();
        _yAxis.endCycle();
        _skewOut.endCycle();
        _cDownOut.endCycle();
        _cUpOut.endCycle();
    }

private:
    const Activator& _tilt;
    const Activator& _lsDown;
    const Activator& _lsUp;
    const Activator& _cLeft;
    const Activator& _cRight;
    const Activator& _cDown;
    const Activator& _cUp;

    TwoButtonControl _xAxis;
    SingleModAxis _yAxis;

    Activator _skewOut;
    Activator _cDownOut;
    Activator _cUpOut;
};

#endif // BUTTONONLYCSTICK_H
