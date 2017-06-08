#ifndef DEADZONE_H
#define DEADZONE_H

class DeadZone
{
public:
    DeadZone();

    bool check(double value);

    double getCenter()           { return _center; }

    void setCenter(double value) { _center = value; }
    void setRange(double value)  { _range = value; }

private:
    double _center;
    double _range;
};



bool DeadZone::check(double value)
{
    bool inDeadZone = true;

    if (value > (_center + _range / 2.0)
     || value < (_center - _range / 2.0))
        inDeadZone = false;

    return inDeadZone;
}

DeadZone::DeadZone()
: _center(0.0),
  _range(0.0)
{}

#endif // DEADZONE_H