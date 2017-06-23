#ifndef GAMECUBECONTROLLERSLOT_H
#define GAMECUBECONTROLLERSLOT_H

#include "GameCubeController.h"

class GameCubeControllerSlot
{
public:
    GameCubeControllerSlot()
    : _controller(nullptr),
      _isConnected(false)
    {}

    const bool isConnected() const    { return _isConnected; }

    void connect(const GameCubeController& controller)
    {
        _controller = &controller;
        _isConnected = true;
    }

    void disconnect()
    {
        _controller = nullptr;
        _isConnected = false;
    }

    const Control& getA() const       { return _controller->a; }
    const Control& getB() const       { return _controller->b; }
    const Control& getX() const       { return _controller->x; }
    const Control& getY() const       { return _controller->y; }
    const Control& getZ() const       { return _controller->z; }
    const Control& getL() const       { return _controller->l; }
    const Control& getR() const       { return _controller->r; }
    const Control& getStart() const   { return _controller->start; }
    const Control& getDLeft() const   { return _controller->dLeft; }
    const Control& getDRight() const  { return _controller->dRight; }
    const Control& getDDown() const   { return _controller->dDown; }
    const Control& getDUp() const     { return _controller->dUp; }
    const Control& getLAnalog() const { return _controller->lAnalog; }
    const Control& getRAnalog() const { return _controller->rAnalog; }
    const Control& getLsX() const     { return _controller->lsX; }
    const Control& getLsY() const     { return _controller->lsY; }
    const Control& getCX() const      { return _controller->cX; }
    const Control& getCY() const      { return _controller->cY; }

private:
    const GameCubeController* _controller;

    bool _isConnected;
};

#endif // GAMECUBECONTROLLERSLOT_H