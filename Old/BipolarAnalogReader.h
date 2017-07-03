#ifndef BIPOLARANALOGREADER_H
#define BIPOLARANALOGREADER_H

#include <ResponsiveAnalogRead.h>

#include "BipolarControl.h"
#include "Timer.h"

class BipolarAnalogReader : public BipolarControl
{
public:
    explicit BipolarAnalogReader(const uint8_t pin)
    : BipolarControl(),
      _analogReader(pin, true),
      _readRange(0.6250),
      _readResolution(1024.0),
      _deadRange(0.05),
      _activeRange(0.15),
      _isCalibrating(true),
      _calibrationTimer(100000)
    {
        _resolutionRange.setLowerBound(0.0);
        _resolutionRange.setUpperBound(_readResolution);

        _newLow.setLowerBound(-1.0);
        _newLow.setUpperBound(-_activeRange);
        _newHigh.setLowerBound(_activeRange);
        _newHigh.setUpperBound(1.0);
    }

    void endCycle()
    {
        BipolarControl::endCycle();
        _analogReader.update();

        if (_isCalibrating)
        {
            if (_calibrationTimer.targetTimeReached())
            {
                double unscaledCenterValue = static_cast<double>(_analogReader.getValue());
                double scaledCenterValue = rescale<double>(unscaledCenterValue, _resolutionRange, getValueRange());

                _oldLow.setLowerBound(scaledCenterValue - _readRange);
                _oldLow.setUpperBound(scaledCenterValue - _deadRange);
                _oldHigh.setLowerBound(scaledCenterValue + _deadRange);
                _oldHigh.setUpperBound(scaledCenterValue + _readRange);

                _isCalibrating = false;
            }
        }

        if (!_isCalibrating)
        {
            double unscaledOldValue = static_cast<double>(_analogReader.getValue());
            double scaledOldValue = rescale<double>(unscaledOldValue, _resolutionRange, getValueRange());

            if (scaledOldValue < _oldLow.getUpperBound())
            {
                setValue(rescale<double>(scaledOldValue, _oldLow, _newLow));
                return;
            }

            if (scaledOldValue > _oldHigh.getLowerBound())
            {
                setValue(rescale<double>(scaledOldValue, _oldHigh, _newHigh));
                return;
            }

            setValue(0.0);
        }
    }

private:
    ResponsiveAnalogRead _analogReader;

    Range<double> _resolutionRange;

    Range<double> _oldLow;
    Range<double> _oldHigh;
    Range<double> _newLow;
    Range<double> _newHigh;

    double _readRange;
    double _readResolution;
    double _deadRange;
    double _activeRange;

    bool _isCalibrating;
    Timer _calibrationTimer;
};

#endif // BIPOLARANALOGREADER_H