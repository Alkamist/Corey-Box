#ifndef GAMECUBEOUTPUT_H
#define GAMECUBEOUTPUT_H

#include "Nintendo.h"
#include "GameCubeController.h"
#include "GameCubeControllerSlot.h"

class GameCubeOutput : public CGamecubeConsole
{
public:
    GameCubeOutput(const uint8_t pin)
    : CGamecubeConsole(pin),
      _rawData(defaultGamecubeData)
    {}

    void update();

    void connectController(const GameCubeController& controller) { _controllerSlot.connect(controller); }

private:
    GameCubeControllerSlot _controllerSlot;

    Gamecube_Data_t _rawData;

    void writeData();
};



void GameCubeOutput::update()
{
    if (_controllerSlot.isConnected())
        writeData();
}

void GameCubeOutput::writeData()
{
    // Digital
    if (_controllerSlot.getA().hasChanged())       _rawData.report.a = _controllerSlot.getA().getValue();
    if (_controllerSlot.getB().hasChanged())       _rawData.report.b = _controllerSlot.getB().getValue();
    if (_controllerSlot.getX().hasChanged())       _rawData.report.x = _controllerSlot.getX().getValue();
    if (_controllerSlot.getY().hasChanged())       _rawData.report.y = _controllerSlot.getY().getValue();
    if (_controllerSlot.getZ().hasChanged())       _rawData.report.z = _controllerSlot.getZ().getValue();
    if (_controllerSlot.getL().hasChanged())       _rawData.report.l = _controllerSlot.getL().getValue();
    if (_controllerSlot.getR().hasChanged())       _rawData.report.r = _controllerSlot.getR().getValue();
    if (_controllerSlot.getStart().hasChanged())   _rawData.report.start = _controllerSlot.getStart().getValue();
    if (_controllerSlot.getDLeft().hasChanged())   _rawData.report.dleft = _controllerSlot.getDLeft().getValue();
    if (_controllerSlot.getDRight().hasChanged())  _rawData.report.dright = _controllerSlot.getDRight().getValue();
    if (_controllerSlot.getDDown().hasChanged())   _rawData.report.ddown = _controllerSlot.getDDown().getValue();
    if (_controllerSlot.getDUp().hasChanged())     _rawData.report.dup = _controllerSlot.getDUp().getValue();

    // Analog
    if (_controllerSlot.getLAnalog().hasChanged()) _rawData.report.left = _controllerSlot.getLAnalog().getValue() * 255;
    if (_controllerSlot.getRAnalog().hasChanged()) _rawData.report.right = _controllerSlot.getRAnalog().getValue() * 255;
    if (_controllerSlot.getLsX().hasChanged())     _rawData.report.xAxis = _controllerSlot.getLsX().getValue() * 255 + 1;
    if (_controllerSlot.getLsY().hasChanged())     _rawData.report.yAxis = _controllerSlot.getLsY().getValue() * 255 + 1;
    if (_controllerSlot.getCX().hasChanged())      _rawData.report.cxAxis = _controllerSlot.getCX().getValue() * 255 + 1;
    if (_controllerSlot.getCY().hasChanged())      _rawData.report.cyAxis = _controllerSlot.getCY().getValue() * 255 + 1;

    write(_rawData);
}


#endif // GAMECUBEOUTPUT_H