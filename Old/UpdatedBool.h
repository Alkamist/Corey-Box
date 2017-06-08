#ifndef UPDATEDBOOL_H
#define UPDATEDBOOL_H

class UpdatedBool
{
public:
    UpdatedBool();

    // Be sure to call update at the very end of the loop.
    void update();
    inline bool hasChanged()    { return _hasChanged; }

    inline void set(bool value) { _value = value; }

    inline bool isTrue()        { return _value; }
    inline bool wasTrue()       { return _previousValue; }

private:
    bool _value;
    bool _previousValue;
    bool _hasChanged;
};



UpdatedBool::UpdatedBool()
 : _value(false),
   _previousValue(false),
   _hasChanged(false)
{}

void UpdatedBool::update()
{
    if (_value != _previousValue)
        _hasChanged = true;
    else
        _hasChanged = false;

    _previousValue = _value;
}

#endif // UPDATEDBOOL_H