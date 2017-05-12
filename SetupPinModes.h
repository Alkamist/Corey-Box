#ifndef SETUPPINMODES_H
#define SETUPPINMODES_H


void setupPinModes()
{
    pinMode (lsLeft, INPUT_PULLUP);
    pinMode (lsRight, INPUT_PULLUP);
    pinMode (lsDown, INPUT_PULLUP);
    pinMode (lsUp, INPUT_PULLUP);
    
    pinMode (cLeft, INPUT_PULLUP);
    pinMode (cRight, INPUT_PULLUP);
    pinMode (cDown, INPUT_PULLUP);
    pinMode (cUp, INPUT_PULLUP);
    
    pinMode (xMod1, INPUT_PULLUP);
    pinMode (xMod2, INPUT_PULLUP);
    pinMode (yMod1, INPUT_PULLUP);
    pinMode (yMod2, INPUT_PULLUP);   
    pinMode (tiltMod, INPUT_PULLUP);

    pinMode (lsXOutPin, OUTPUT);
    pinMode (lsYOutPin, OUTPUT);
    pinMode (cXOutPin, OUTPUT);
    pinMode (cYOutPin, OUTPUT);

    //pinMode (rButtonSignal, INPUT);
    //pinMode (rButtonPin, INPUT_PULLUP);
    //pinMode (lButtonSignal, INPUT);
    //pinMode (lButtonPin, INPUT_PULLUP);
}

#endif // SETUPPINMODES_H
