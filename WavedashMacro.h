#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "TemporaryActivator.h"
#include "ControlState.h"
#include "Frames.h"

class WavedashMacro
{
public:
    WavedashMacro()
    : _jump(frames(2)),
      _lTemp(frames(2)),
      _rTemp(frames(2)),
      _airDodgeDelayFrames(3)
    {}

    void update()
    {
        _activator.update();
        _trimDown.update();
        _trimUp.update();
        _jump.update();
        _lTemp.update();
        _rTemp.update();
    }

    void setControls(const bool activator, const bool r,
                     const bool trimDown, const bool trimUp)
    {
        _activator = activator;
        _trimDown = trimDown;
        _trimUp = trimUp;

        if (_trimDown.justActivated())
        {
            --_airDodgeDelayFrames;
            _airDodgeDelayFrames = enforceRange(_airDodgeDelayFrames);
        }

        if (_trimUp.justActivated())
        {
            ++_airDodgeDelayFrames;
            _airDodgeDelayFrames = enforceRange(_airDodgeDelayFrames);
        }

        if (_activator.justActivated())
            _timer.reset();

        _jump.setControls(activator);

        _lTemp.setControls(checkTimer(frames(_airDodgeDelayFrames - 1)));
        _rTemp.setControls(checkTimer(frames(_airDodgeDelayFrames)));

        if ((_timer.getValue() < frames(_airDodgeDelayFrames)) && (_timer.getValue() > frames(1)))
            _rOut = false;
        else
            _rOut = _rTemp.isActive() || r;

    }

    const bool getJump() const { return _jump; }
    const bool getL() const    { return _lTemp; }
    const bool getR() const    { return _rOut; }

private:
    Timer _timer;

    ControlState _activator;
    ControlState _trimDown;
    ControlState _trimUp;

    TemporaryActivator _jump;
    TemporaryActivator _lTemp;
    TemporaryActivator _rTemp;

    bool _rOut;

    unsigned int _airDodgeDelayFrames;

    const bool checkTimer(const unsigned int time) const
    {
        unsigned int currentTime = _timer.getValue();
        unsigned int lowBound = time;
        unsigned int highBound = time + frames(1);

        return (currentTime < highBound) && (currentTime > lowBound);
    }

    const unsigned int enforceRange(unsigned int value)
    {
        if (value > 8)
            return value = 8;

        if (value < 3)
            return value = 3;

        return value;
    }
};

#endif // WAVEDASHMACRO_H
