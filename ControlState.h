#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

#include "UpdatedValue.h"

class ControlState : public UpdatedValue<bool>
{
public:
    ControlState()
    : UpdatedValue<bool>(false)
    {}

    explicit ControlState(const bool value)
    : UpdatedValue<bool>(value)
    {}

    const bool justActivated() const   { return hasChanged() && (getValue() == true); }
    const bool justDeactivated() const { return hasChanged() && (getValue() == false); }

    //=================== OPERATORS ===================

    virtual const UpdatedValue<bool>& operator =(const bool value)
    {
        setValue(value);
        return *this;
    }

    virtual operator bool() const                          { return getValue(); }

    virtual const bool operator !() const                  { return !getValue(); }
    virtual const bool operator &&(const bool value) const { return getValue() && value; }
    virtual const bool operator ||(const bool value) const { return getValue() || value; }
    virtual const bool operator ==(const bool value) const { return getValue() == value; }
    virtual const bool operator !=(const bool value) const { return getValue() != value; }
    virtual const bool operator >(const bool value) const  { return getValue() > value; }
    virtual const bool operator <(const bool value) const  { return getValue() < value; }
    virtual const bool operator >=(const bool value) const { return getValue() >= value; }
    virtual const bool operator <=(const bool value) const { return getValue() <= value; }
};

#endif // CONTROLSTATE_H