#ifndef DEADZONE_H
#define DEADZONE_H

class DeadZone
{
public:
    DeadZone();

    const bool check(const double value) const;

    const double getCenter() const     { return _center; }

    void setCenter(const double value) { _center = value; }
    void setRange(const double value)  { _range = value; }

private:
    double _center;
    double _range;
};



const bool DeadZone::check(const double value) const
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