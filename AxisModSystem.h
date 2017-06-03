#ifndef AXISMODSYSTEM_H
#define AXISMODSYSTEM_H

#include "Globals.h"

// This is a class that holds modifier values
class ModList
{
public:
    ModList();
    ModList(double maximumValue,
            double mod1,
            double mod2,
            double combinedMod,
            double tilt,
            double mod1Tilt,
            double mod2Tilt,
            double combinedModTilt,
            double cYAxisSkew,
            unsigned int tiltTime);

    inline double getMaxiumumValue()   { return _maximumValue; }
    inline double getMod1()            { return _mod1; }
    inline double getMod2()            { return _mod2; }
    inline double getCombinedMod()     { return _combinedMod; }
    inline double getTilt()            { return _tilt; }
    inline double getMod1Tilt()        { return _mod1Tilt; }
    inline double getMod2Tilt()        { return _mod2Tilt; }
    inline double getCombinedModTilt() { return _combinedModTilt; }
    inline double getCYAxisSkew()      { return _cYAxisSkew; }
    inline unsigned int getTiltTime()  { return _tiltTime; }
private:
    double _maximumValue;
    double _mod1;
    double _mod2;
    double _combinedMod;
    double _tilt;
    double _mod1Tilt;
    double _mod2Tilt;
    double _combinedModTilt;

    // C-Stick Y axis skew for angled smashes:
    double _cYAxisSkew;

    unsigned int _tiltTime;
};

// Default Melee mod values for safety
ModList::ModList()
:  _maximumValue(0.9500),
   _mod1(0.3125),
   _mod2(0.4250),
   _combinedMod(0.7000),
   _tilt(0.3875),
   _mod1Tilt(0.1875),
   _mod2Tilt(0.2125),
   _combinedModTilt(0.2875),
   _cYAxisSkew(0.6500),
   _tiltTime(104)
{}

// Use this constructor
ModList::ModList(double maximumValue,
                 double mod1,
                 double mod2,
                 double combinedMod,
                 double tilt,
                 double mod1Tilt,
                 double mod2Tilt,
                 double combinedModTilt,
                 double cYAxisSkew,
                 unsigned int tiltTime)
 : _maximumValue (maximumValue),
   _mod1 (mod1),
   _mod2 (mod2),
   _combinedMod (combinedMod),
   _tilt (tilt),
   _mod1Tilt (mod1Tilt),
   _mod2Tilt (mod2Tilt),
   _combinedModTilt (combinedModTilt),
   _cYAxisSkew (cYAxisSkew),
   _tiltTime (tiltTime)
{}



class AxisModSystem
{
public:
    AxisModSystem();

    void processCurrentValues();

    inline void setModList(ModList inputModList) { _modList = inputModList; }
    inline void resetTiltTimer() { _tiltTimeCounter = 0; }

    inline void setTiltTempDisableTime(unsigned int inputTime) { _tiltTempDisableTime = inputTime; }
    inline void tempDisableTilt()
    {
        _tiltTempDisabled = true;
        _tiltTempDisableCounter = 0;
    }

    inline void setCurrentLsXValue(double inputValue) { _currentLsXValue = inputValue; }
    inline void setCurrentLsYValue(double inputValue) { _currentLsYValue = inputValue; }
    inline void setCurrentCXValue(double inputValue) { _currentCXValue = inputValue; }
    inline void setCurrentCYValue(double inputValue) { _currentCYValue = inputValue; }

    inline double getCurrentLsXValue() { return _currentLsXValue; }
    inline double getCurrentLsYValue() { return _currentLsYValue; }
    inline double getCurrentCXValue() { return _currentCXValue; }
    inline double getCurrentCYValue() { return _currentCYValue; }
private:
    ModList _modList;

    double _currentLsXValue;
    double _currentLsYValue;
    double _currentCXValue;
    double _currentCYValue;

    bool _tiltTempDisabled;
    unsigned int _tiltTempDisableTime;
    elapsedMillis _tiltTempDisableCounter;
    elapsedMillis _tiltTimeCounter;

    void applyModValue(double &axisValue, double mod);
    void applyCStickMods();
    void applyLeftStickModifiers();
};



AxisModSystem::AxisModSystem ()
 : _currentLsXValue(0.50),
   _currentLsYValue(0.50),
   _currentCXValue(0.50),
   _currentCYValue(0.50),
   _tiltTempDisableTime(50),
   _tiltTempDisabled(false)
{}

void AxisModSystem::processCurrentValues()
{
    if (_tiltTempDisableCounter >= _tiltTempDisableTime)
        _tiltTempDisabled = false;

    applyLeftStickModifiers();
    applyCStickMods();
}

void AxisModSystem::applyModValue(double &axisValue, double mod)
{
    if (axisValue < 0.50)
    {
        axisValue = 0.50 - 0.50 * mod;
        return;
    }
    if (axisValue > 0.50)
    {
        axisValue = 0.50 + 0.50 * mod;
        return;
    }
}

// This function applies the c-stick modifiers.
// The current purpose of this is to allow for tilted
// smashes based on input from the left stick and tilt
// button:
void AxisModSystem::applyCStickMods()
{
    bool tiltModIsPressed = !digitalRead(TILT_MOD_PIN);

    if (tiltModIsPressed
     && _currentCXValue != 0.50
     && _currentLsYValue != 0.50)
    {
        _currentCYValue = _currentLsYValue;
        applyModValue(_currentCYValue, _modList.getCYAxisSkew());
        return;
    }
}

// This function applies the left stick modifiers.
// There is an if statement and corresponding action
// for every single possibility of button presses that
// has to do with the left stick:
void AxisModSystem::applyLeftStickModifiers()
{
    bool xMod1IsPressed = !digitalRead(X_MOD1_PIN);
    bool xMod2IsPressed = !digitalRead(X_MOD2_PIN);
    bool yMod1IsPressed = !digitalRead(Y_MOD1_PIN);
    bool yMod2IsPressed = !digitalRead(Y_MOD2_PIN);
    bool tiltModIsPressed = !digitalRead(TILT_MOD_PIN) && !_tiltTempDisabled;

    // Tilt mod not pressed:
    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod1());
        applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod2());
        applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getCombinedMod());
        applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
        applyModValue(_currentLsYValue, _modList.getMod1());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod1());
        applyModValue(_currentLsYValue, _modList.getMod1());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod2());
        applyModValue(_currentLsYValue, _modList.getMod1());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getCombinedMod());
        applyModValue(_currentLsYValue, _modList.getMod1());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
        applyModValue(_currentLsYValue, _modList.getMod2());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod1());
        applyModValue(_currentLsYValue, _modList.getMod2());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod2());
        applyModValue(_currentLsYValue, _modList.getMod2());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getCombinedMod());
        applyModValue(_currentLsYValue, _modList.getMod2());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
        applyModValue(_currentLsYValue, _modList.getCombinedMod());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod1());
        applyModValue(_currentLsYValue, _modList.getCombinedMod());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getMod2());
        applyModValue(_currentLsYValue, _modList.getCombinedMod());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModValue(_currentLsXValue, _modList.getCombinedMod());
        applyModValue(_currentLsYValue, _modList.getCombinedMod());
        return;
    }

    // Tilt mod pressed:

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
            applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getTilt());
            applyModValue(_currentLsYValue, _modList.getTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod1());
            applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod1Tilt());
            applyModValue(_currentLsYValue, _modList.getTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod2());
            applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod2Tilt());
            applyModValue(_currentLsYValue, _modList.getTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getCombinedMod());
            applyModValue(_currentLsYValue, _modList.getMaxiumumValue());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getCombinedModTilt());
            applyModValue(_currentLsYValue, _modList.getTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
            applyModValue(_currentLsYValue, _modList.getMod1());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getTilt());
            applyModValue(_currentLsYValue, _modList.getMod1Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod1());
            applyModValue(_currentLsYValue, _modList.getMod1());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod1Tilt());
            applyModValue(_currentLsYValue, _modList.getMod1Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod2());
            applyModValue(_currentLsYValue, _modList.getMod1());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod2Tilt());
            applyModValue(_currentLsYValue, _modList.getMod1Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getCombinedMod());
            applyModValue(_currentLsYValue, _modList.getMod1());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getCombinedModTilt());
            applyModValue(_currentLsYValue, _modList.getMod1Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
            applyModValue(_currentLsYValue, _modList.getMod2());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getTilt());
            applyModValue(_currentLsYValue, _modList.getMod2Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod1());
            applyModValue(_currentLsYValue, _modList.getMod2());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod1Tilt());
            applyModValue(_currentLsYValue, _modList.getMod2Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod2());
            applyModValue(_currentLsYValue, _modList.getMod2());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod2Tilt());
            applyModValue(_currentLsYValue, _modList.getMod2Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getCombinedMod());
            applyModValue(_currentLsYValue, _modList.getMod2());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getCombinedModTilt());
            applyModValue(_currentLsYValue, _modList.getMod2Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMaxiumumValue());
            applyModValue(_currentLsYValue, _modList.getCombinedMod());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getTilt());
            applyModValue(_currentLsYValue, _modList.getCombinedModTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod1());
            applyModValue(_currentLsYValue, _modList.getCombinedMod());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod1Tilt());
            applyModValue(_currentLsYValue, _modList.getCombinedModTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getMod2());
            applyModValue(_currentLsYValue, _modList.getCombinedMod());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getMod2Tilt());
            applyModValue(_currentLsYValue, _modList.getCombinedModTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (_tiltTimeCounter >= _modList.getTiltTime())
        {
            applyModValue(_currentLsXValue, _modList.getCombinedMod());
            applyModValue(_currentLsYValue, _modList.getCombinedMod());
            return;
        }
        else
        {
            applyModValue(_currentLsXValue, _modList.getCombinedModTilt());
            applyModValue(_currentLsYValue, _modList.getCombinedModTilt());
            return;
        }
    }
}

#endif // AXISMODSYSTEM_H