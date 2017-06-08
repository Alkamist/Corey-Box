#ifndef MODLIST_H
#define MODLIST_H

// This is a class that holds modifier values
class ModList
{
public:
    ModList::ModList()
    : _mod1(0.3125),
      _mod2(0.4250),
      _combinedMod(0.7000),
      _tilt(0.3875),
      _mod1Tilt(0.1875),
      _mod2Tilt(0.2125),
      _combinedModTilt(0.2875),
      _tiltTime(104)
    {}

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
    double _mod1;
    double _mod2;
    double _combinedMod;
    double _tilt;
    double _mod1Tilt;
    double _mod2Tilt;
    double _combinedModTilt;

    unsigned int _tiltTime;
};

#endif // MODLIST_H