#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "Joystick.h"
#include "SingleModAxis.h"
#include "TwoButtonControl.h"

class ButtonOnlyCStick : public Joystick
{
public:
    ButtonOnlyCStick()
    : Joystick(),
      _cLeftState(false),
      _cRightState(false),
      _cDownState(false),
      _cUpState(false),
      _lsDownState(false),
      _lsUpState(false),
      _tiltState(false),
      _skewOut(false),
      _cDownOut(false),
      _cUpOut(false)
    {}

    virtual void process()
    {
        bool lsDownOrUp = _lsDownState || _lsUpState;
        bool cLeftOrRight = _cLeftState || _cRightState;
        bool cDownOrUp = _cDownState || _cUpState;

        _skewOut = _tiltState && lsDownOrUp && cLeftOrRight && !cDownOrUp;

        bool skewDown = _lsDownState && _skewOut;
        bool skewUp = _lsUpState && _skewOut;

        _cDownOut = _cDownState || skewDown;
        _cUpOut = _cUpState || skewUp;

        _yAxis.setLowState(_cDownOut);
        _yAxis.setHighState(_cUpOut);
        _yAxis.setModState(_skewOut);

        _xAxis.process();
        _yAxis.process();

        if (_xAxis.valueJustChanged())
            Joystick::setXValue(_xAxis.getValue());

        if (_yAxis.valueJustChanged())
            Joystick::setYValue(_yAxis.getValue());

        Joystick::process();
    }

    virtual void endCycle()
    {
        Joystick::endCycle();
        _xAxis.endCycle();
        _yAxis.endCycle();
    }

    void setCLeftState(const bool state)  { _cLeftState = state; _xAxis.setLowState(state); }
    void setCRightState(const bool state) { _cRightState = state; _xAxis.setHighState(state); }
    void setCDownState(const bool state)  { _cDownState = state; }
    void setCUpState(const bool state)    { _cUpState = state; }
    void setLsDownState(const bool state) { _lsDownState = state; }
    void setLsUpState(const bool state)   { _lsUpState = state; }
    void setTiltState(const bool state)   { _tiltState = state; }

private:
    bool _cLeftState;
    bool _cRightState;
    bool _cDownState;
    bool _cUpState;
    bool _lsDownState;
    bool _lsUpState;
    bool _tiltState;

    bool _skewOut;
    bool _cDownOut;
    bool _cUpOut;

    TwoButtonControl _xAxis;
    SingleModAxis _yAxis;
};

#endif // BUTTONONLYCSTICK_H
