#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "TwoButtonControl.h"

// This class represents the c stick used in the button only controller.
class ButtonOnlyCStick
{
public:
    ButtonOnlyCStick()
    : _cLeftState(false),
      _cRightState(false),
      _cDownState(false),
      _cUpState(false),
      _lsDownState(false),
      _lsUpState(false),
      _tiltState(false)
    {}

    virtual void process()
    {
        bool lsDownOrUp = _lsDownState || _lsUpState;
        bool cLeftOrRight = _cLeftState || _cRightState;
        bool cDownOrUp = _cDownState || _cUpState;

        bool skewOut = _tiltState && lsDownOrUp && cLeftOrRight && !cDownOrUp;

        bool skewDown = _lsDownState && skewOut;
        bool skewUp = _lsUpState && skewOut;

        bool cDownOut = _cDownState || skewDown;
        bool cUpOut = _cUpState || skewUp;

        _yAxis.setLowState(cDownOut);
        _yAxis.setHighState(cUpOut);

        _xAxis.process();
        _yAxis.process();

        if (skewOut)
            _yAxis.setBipolarMagnitude(50);
    }

    virtual void endCycle()
    {
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

    void setRange(const uint8_t value)
    {
        _xAxis.setRange(value);
        _yAxis.setRange(value);
    }

    const uint8_t getXValue() const       { return _xAxis; }
    const uint8_t getYValue() const       { return _yAxis; }

private:
    bool _cLeftState;
    bool _cRightState;
    bool _cDownState;
    bool _cUpState;
    bool _lsDownState;
    bool _lsUpState;
    bool _tiltState;

    TwoButtonControl _xAxis;
    TwoButtonControl _yAxis;
};

#endif // BUTTONONLYCSTICK_H
