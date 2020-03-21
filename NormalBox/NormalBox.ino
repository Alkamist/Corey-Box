class State
{
public:
    explicit State(const bool initialState)
    : m_currentState(initialState),
      m_previousState(initialState)
    {}
    void update() { m_previousState = m_currentState; }
    void setState(const bool newState) { m_currentState = newState; }
    const bool isActive() const { return m_currentState; }
    const bool justActivated() const { return m_currentState && !m_previousState; }
    const bool justDeactivated() const { return !m_currentState && m_previousState; }
private:
    bool m_currentState{false};
    bool m_previousState{false};
};

// The class for reading a button. Specify the input pin number.
class Button
{
public:
    explicit Button(const uint8_t pinNumber)
    : m_pinNumber(pinNumber)
    {
        pinMode(m_pinNumber, INPUT_PULLUP);
    }
    void update()
    {
        m_previousState = m_currentState;
        m_currentState = !digitalRead(m_pinNumber);
    }
    const bool isPressed() const { return m_currentState; }
    const bool justPressed() const { return m_currentState && !m_previousState; }
    const bool justReleased() const { return !m_currentState && m_previousState; }
private:
    uint8_t m_pinNumber{3};
    bool m_useBounce{false};
    bool m_currentState{false};
    bool m_previousState{false};
};

// The class for an analog axis that is driven by a high and low state.
// The most recently activated direction will override the previous.
class ButtonAxis
{
public:
    void update(const bool lowState, const bool highState)
    {
        m_lowState.update();
        m_highState.update();
        m_lowState.setState(lowState);
        m_highState.setState(highState);
        if (m_lowState.justActivated() || (m_lowState.isActive() && !m_highState.isActive()))
        {
            m_value = -1.0;
        }
        else if (m_highState.justActivated() || (m_highState.isActive() && !m_lowState.isActive()))
        {
            m_value = 1.0;
        }
        else if (!m_lowState.isActive() && !m_highState.isActive())
        {
            m_value = 0.0;
        }
    }
    const float getValue() const { return m_value; }
private:
    State m_lowState{false};
    State m_highState{false};
    bool m_lowWasFirst{false};
    float m_value{0.0};
};

Button yButton(21);
Button xButton(19);
Button shieldButton(7);
Button airdodgeButton(20);
Button aButton(15);
Button bButton(16);
Button zButton(22);
Button startButton(9);
Button cUpButton(18);
Button cLeftButton(14);
Button cRightButton(17);
Button cDownButton(10);
Button lsUpButton(23);
Button lsLeftButton(3);
Button lsRightButton(5);
Button lsDownButton(4);
Button smashDIButton(6);
Button xModButton(2);
Button yModButton(8);

ButtonAxis lsXRaw;
ButtonAxis lsYRaw;
ButtonAxis cXRaw;
ButtonAxis cYRaw;

bool aOut = false;
bool bOut = false;
bool xOut = false;
bool yOut = false;
bool zOut = false;
bool lOut = false;
bool rOut = false;
bool startOut = false;
bool dLeftOut = false;
bool dRightOut = false;
bool dDownOut = false;
bool dUpOut = false;
// 43 is the lightest analog shield value.
// 140 is the hardest analog shield value.
int lAnalogOut = 0;
//uint8_t rAnalogOut = 0;
float lsXOut = 0.0;
float lsYOut = 0.0;
float cXOut = 0.0;
float cYOut = 0.0;

unsigned long framesToMillis(const float frames)
{
    return floor(1000.0 * frames / 60.0);
}

void sendJoystickOutputs()
{
    Joystick.button(1, xOut);
    Joystick.button(2, aOut);
    Joystick.button(3, bOut);
    Joystick.button(4, yOut);
    Joystick.button(5, lOut);
    Joystick.button(6, rOut);
    Joystick.button(8, zOut);
    Joystick.button(10, startOut);
    Joystick.button(13, dUpOut);
    Joystick.button(15, dDownOut);
    Joystick.button(16, dLeftOut);
    Joystick.button(14, dRightOut);

    if (lsXOut >= 0.0)
        Joystick.X(lsXOut * 322 + 512);
    else
        Joystick.X(lsXOut * 320 + 512);
    if (lsYOut >= 0.0)
        Joystick.Y(-lsYOut * 330 + 512);
    else
        Joystick.Y(-lsYOut * 318 + 512);
    if (cYOut >= 0.0)
        Joystick.Z(cYOut * 322 + 512);
    else
        Joystick.Z(cYOut * 320 + 512);
    if (cXOut >= 0.0)
        Joystick.Zrotate(-cXOut * 298 + 680);
    else
        Joystick.Zrotate(-cXOut * 220 + 680);

    //Joystick.sliderLeft(lAnalogOut * 12);
    Joystick.sliderLeft(512 - max(lAnalogOut - 42, 0) * 2.888);

    Joystick.send_now();
}

void readButtons()
{
    aButton.update();
    bButton.update();
    lsLeftButton.update();
    lsRightButton.update();
    lsDownButton.update();
    lsUpButton.update();
    xModButton.update();
    yModButton.update();
    cLeftButton.update();
    cRightButton.update();
    cDownButton.update();
    cUpButton.update();
    yButton.update();
    xButton.update();
    zButton.update();
    airdodgeButton.update();
    shieldButton.update();
    startButton.update();
    smashDIButton.update();
}

void handleAngleModifiers()
{
    bool isDiagonal = (lsLeftButton.isPressed() || lsRightButton.isPressed()) && (lsDownButton.isPressed() || lsUpButton.isPressed());
    bool isAirdodging = shieldButton.isPressed() || airdodgeButton.isPressed();
    if (xModButton.isPressed())
    {
        if (isDiagonal)
        {
            if (isAirdodging)
            {
                lsXOut = lsXRaw.getValue() * 0.6375;
            }
            else
            {
                lsXOut = lsXRaw.getValue() * 0.7375;
            }
        }
        else
        {
            lsXOut = lsXRaw.getValue() * 0.6625;
        }
        if (isAirdodging)
        {
            lsYOut = lsYRaw.getValue() * 0.3750;
        }
        else
        {
            lsYOut = lsYRaw.getValue() * 0.2875;
        }
    }
    if (yModButton.isPressed())
    {
        if (isDiagonal)
        {
            if (isAirdodging)
            {
                lsXOut = lsXRaw.getValue() * 0.5000;
            }
            else
            {
                lsXOut = lsXRaw.getValue() * 0.2875;
            }
        }
        else
        {
            lsXOut = lsXRaw.getValue() * 0.3375;
        }
        if (isAirdodging)
        {
            lsYOut = lsYRaw.getValue() * 0.8500;
        }
        else
        {
            lsYOut = lsYRaw.getValue() * 0.7375;
        }
    }
}

bool isWavelanding = false;
unsigned long wavelandTime = 0;
void handleWaveland()
{
    bool wavelandSideways = !lsUpButton.isPressed() && !lsDownButton.isPressed();
    if (airdodgeButton.justPressed())
    {
        isWavelanding = true;
        wavelandTime = millis();
    }
    if (isWavelanding && !lsDownButton.isPressed())
    {
        if (millis() - wavelandTime < 25)
        {
            if (wavelandSideways)
            {
                lsXOut = lsXRaw.getValue() * 0.6375;
                lsYOut = -0.3750;
            }
            else
            {
                lsYOut = -1.0000;
            }
        }
        else
        {
            isWavelanding = false;
        }
    }
}

bool isTiltingShield = false;
unsigned long shieldTiltTime = 0;
void handleShieldTilt()
{
    bool tiltTemporarily = shieldButton.isPressed() && (lsLeftButton.justPressed() || lsRightButton.justPressed());
    bool tiltTemporarilyOnRelease = shieldButton.isPressed() && ((lsLeftButton.justReleased() && lsRightButton.isPressed()) || (lsRightButton.justReleased() && lsLeftButton.isPressed()));
    bool tiltShieldDown = lsDownButton.isPressed() && shieldButton.isPressed();
    if (shieldButton.justPressed() || tiltTemporarily || tiltTemporarilyOnRelease || tiltShieldDown)
    {
        isTiltingShield = true;
        shieldTiltTime = millis();
    }
    if (isTiltingShield && !airdodgeButton.isPressed() && !xModButton.isPressed() && !yModButton.isPressed())
    {
        if (millis() - shieldTiltTime < 100)
        {
            lsXOut = lsXRaw.getValue() * 0.6625;
            lsYOut = lsYRaw.getValue() * 0.6625;
        }
        else
        {
            isTiltingShield = false;
        }
    }
}

bool isDoingSafeB = false;
unsigned long safeBTime = 0;
void handleSafeGroundB()
{
    if (bButton.justPressed() && (lsDownButton.isPressed() || lsUpButton.isPressed()))
    {
        isDoingSafeB = true;
        safeBTime = millis();
    }
    if (isDoingSafeB)
    {
        if (millis() - safeBTime < 25)
        {
            lsXOut = lsXRaw.getValue() * 0.5875;
            lsYOut = lsYRaw.getValue() * 0.6000;
        }
        else
        {
            isDoingSafeB = false;
        }
    }
    //if (bButton.isPressed() && !xModButton.isPressed() && !yModButton.isPressed() && !shieldButton.isPressed() && lsDownButton.isPressed() && (lsLeftButton.isPressed() || lsRightButton.isPressed()))
    //{
    //    lsXOut = lsXRaw.getValue() * 0.5875;
    //    lsYOut = lsYRaw.getValue() * 0.8000;
    //}
}

// If you hold both modifier buttons at the same time,
// the c-stick buttons will turn into DPad buttons.
void handleDPad()
{
    if (xModButton.isPressed() && yModButton.isPressed())
    {
        cXOut = 0.0;
        cYOut = 0.0;
        dLeftOut = cLeftButton.isPressed();
        dRightOut = cRightButton.isPressed();
        dDownOut = cDownButton.isPressed();
        dUpOut = cUpButton.isPressed();
    }
}

// If you hold either XMod or YMod while holding analog down or up,
// your smashes will be angled appropriately.
void handleAngledSmashes()
{
    if ((xModButton.isPressed() || yModButton.isPressed()) && (cLeftButton.isPressed() || cRightButton.isPressed()))
    {
        if (lsYRaw.getValue() > 0.0)
        {
            cYOut = 0.5;
        }
        else if (lsYRaw.getValue() < 0.0)
        {
            cYOut = -0.5;
        }
    }
}

bool delayBackdash = false;
unsigned long backdashTime = 0;
void handleBackdashOutOfCrouchFix()
{
    if (lsDownButton.isPressed() && (lsLeftButton.justPressed() || lsLeftButton.justPressed())
    && !aButton.isPressed() && !bButton.isPressed() && !airdodgeButton.isPressed() && !shieldButton.isPressed() && !zButton.isPressed()
    && !xButton.isPressed() && !yButton.isPressed())
    {
        delayBackdash = true;
        backdashTime = millis();
    }
    if (lsDownButton.justReleased())
    {
        delayBackdash = false;
    }
    if (delayBackdash)
    {
        lsXOut = 0.0;
        if (millis() - backdashTime >= 25)
        {
            delayBackdash = false;
        }
    }
}

bool spamAOut = false;
bool spamBOut = false;
bool isSpammingB = false;
unsigned long spamABTime = 0;
void handleABSpam()
{
    if (aButton.justPressed() && bButton.isPressed())
    {
        isSpammingB = true;
        spamBOut = true;
        spamABTime = millis();
    }
    if (aButton.justReleased() || bButton.justReleased())
    {
        isSpammingB = false;
        spamBOut = false;
    }
    if (isSpammingB)
    {
        if ((millis() - spamABTime >= 33))
        {
            spamBOut = true;
            spamABTime = millis();
            if (smashDIButton.isPressed())
            {
                spamAOut = false;
            }
        }
        else if (millis() - spamABTime >= 16)
        {
            spamBOut = false;
            if (smashDIButton.isPressed())
            {
                spamAOut = true;
            }
        }
    }
}

void setup()
{
    Joystick.useManualSend(true);
}

void loop()
{
    readButtons();

    handleABSpam();

    lsXRaw.update(lsLeftButton.isPressed(), lsRightButton.isPressed());
    lsYRaw.update(lsDownButton.isPressed(), lsUpButton.isPressed());
    lsXOut = lsXRaw.getValue();
    lsYOut = lsYRaw.getValue();

    cXRaw.update(cLeftButton.isPressed(), cRightButton.isPressed());
    cYRaw.update(cDownButton.isPressed(), cUpButton.isPressed());
    cXOut = cXRaw.getValue();
    cYOut = cYRaw.getValue();
    //aOut = aButton.isPressed();
    //bOut = bButton.isPressed();
    yOut = yButton.isPressed();
    xOut = xButton.isPressed();
    zOut = zButton.isPressed();
    lOut = shieldButton.isPressed();
    rOut = airdodgeButton.isPressed();
    startOut = startButton.isPressed();

    if (isSpammingB)
    {
        bOut = spamBOut;
        aOut = spamAOut;
    }
    else
    {
        bOut = bButton.isPressed();
        aOut = aButton.isPressed();
    }

    handleAngleModifiers();
    handleShieldTilt();
    handleBackdashOutOfCrouchFix();
    handleSafeGroundB();
    handleAngledSmashes();
    handleDPad();
    handleWaveland();

    sendJoystickOutputs();
}