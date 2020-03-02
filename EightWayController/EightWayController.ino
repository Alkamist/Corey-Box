#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>

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

// The class for reading a button. Specify the input pin number and
// whether or not you want the button to be debounced.
class Button
{
public:
    explicit Button(const uint8_t pinNumber, const bool useBounce)
    : m_pinNumber(pinNumber),
      m_useBounce(useBounce)
    {
        pinMode(m_pinNumber, INPUT_PULLUP);
        if (m_useBounce)
        {
            m_bounce.attach(m_pinNumber);
            m_bounce.interval(16);
        }
    }
    void update()
    {
        m_previousState = m_currentState;
        if (m_useBounce)
        {
            m_bounce.update();
            m_currentState = !m_bounce.read();
        }
        else
        {
            m_currentState = !digitalRead(m_pinNumber);
        }
    }
    const bool isPressed() const { return m_currentState; }
    const bool justPressed() const { return m_currentState && !m_previousState; }
    const bool justReleased() const { return !m_currentState && m_previousState; }
private:
    Bounce m_bounce;
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

Button fullHopButton(23, true);
Button shieldButton(22, false);
Button shortHopButton(21, true);
Button airdodgeButton(20, true);
Button zButton(19, false);
Button cUpButton(18, false);
Button cRightButton(17, false);
Button bButton(16, false);
Button aButton(15, false);
Button cLeftButton(14, false);
Button cDownButton(10, false);

Button startButton(9, false);

Button lsUpButton(2, false);
Button lsLeftButton(3, false);
Button lsDownButton(4, false);
Button lsRightButton(5, false);
Button smashDIButton(6, false);
Button xModButton(7, false);
Button yModButton(8, false);

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
    shortHopButton.update();
    fullHopButton.update();
    zButton.update();
    airdodgeButton.update();
    shieldButton.update();
    startButton.update();
    smashDIButton.update();
}

bool isShortHopping = false;
bool shortHopOut = false;
unsigned long shortHopTime = 0;
bool isFullHopping = false;
bool fullHopOut = false;
unsigned long fullHopTime = 0;
void handleShortAndFullHops()
{
    // If the short hop button is pressed or the full hop button is pressed during the 134 ms window:
    //     press y for 25 ms.
    // If the full hop button is pressed:
    //     press x for as long as the full hop button is held, but no less than 134 ms.
    if (shortHopButton.justPressed() || (isFullHopping && fullHopButton.justPressed()))
    {
        isShortHopping = true;
        shortHopTime = millis();
        shortHopOut = true;
    }
    if (!isFullHopping && fullHopButton.justPressed())
    {
        isFullHopping = true;
        fullHopTime = millis();
        fullHopOut = true;
    }
    if (isShortHopping)
    {
        if (millis() - shortHopTime >= 25)
        {
            isShortHopping = false;
            shortHopOut = false;
        }
    }
    if (isFullHopping && !fullHopButton.isPressed())
    {
        if (millis() - fullHopTime >= 134)
        {
            fullHopOut = false;
        }
        // Wait one extra frame so you can't miss a double jump by
        // pushing the full hop button on the same frame of release.
        if (millis() - fullHopTime >= 150)
        {
            isFullHopping = false;
        }
    }
}

uint8_t jumpsquatFrames = 3;
unsigned long jumpsquatTime = framesToMillis(float(jumpsquatFrames) - 0.3);
void changeJumpsquatFrames(const int8_t byFrames)
{
    jumpsquatFrames += byFrames;
    if (jumpsquatFrames > 8) jumpsquatFrames = 8;
    if (jumpsquatFrames < 3) jumpsquatFrames = 3;
    jumpsquatTime = framesToMillis(float(jumpsquatFrames) - 0.3);
}

bool wavedashJumpOut = false;
bool wavedashShieldOut = false;
bool wavedashAirdodgeOut = false;
bool wavedashDownOut = false;
bool isAirDodging = false;
bool isJumpingBeforeAirdodge = false;
bool needToAirdodge = false;
bool isWavedashing = false;
unsigned long airDodgeTime = 0;
unsigned long wavedashTime = 0;
void handleWavedash()
{
    // Peform a short hop initially.
    if (airdodgeButton.justPressed())
    {
        isWavedashing = true;
        isJumpingBeforeAirdodge = true;
        needToAirdodge = true;
        wavedashTime = millis();
        wavedashJumpOut = true;
        if (shieldButton.isPressed())
        {
            wavedashShieldOut = true;
        }
    }
    if (isJumpingBeforeAirdodge)
    {
        if (millis() - wavedashTime >= 25)
        {
            isJumpingBeforeAirdodge = false;
            wavedashJumpOut = false;
            wavedashShieldOut = false;
        }
    }
    // Attempt to push l just after the jumpsquat ends.
    if (needToAirdodge && (millis() - wavedashTime >= jumpsquatTime))
    {
        isAirDodging = true;
        needToAirdodge = false;
        airDodgeTime = millis();
        wavedashAirdodgeOut = true;
        wavedashDownOut = true;
    }
    // Push r one frame later just in case l doesn't come out on time.
    if (isAirDodging)
    {
        auto currentDuration = millis() - airDodgeTime;
        if (currentDuration >= 16 && currentDuration < 33)
        {
            wavedashAirdodgeOut = false;
            wavedashShieldOut = true;
        }
        if (currentDuration >= 33)
        {
            isAirDodging = false;
            isWavedashing = false;
            wavedashDownOut = false;
            wavedashShieldOut = false;
        }
    }
}

void handleAngleModifiers()
{
    if (xModButton.isPressed() || (isWavedashing && !lsDownButton.isPressed()))
    {
        lsYOut = lsYRaw.getValue() * 0.2875;
        if (shieldButton.isPressed())
        {
            lsXOut = lsXRaw.getValue() * 0.6500;
        }
        else
        {
            lsXOut = lsXRaw.getValue() * 0.7375;
        }
    }
    if (yModButton.isPressed())
    {
        lsXOut = lsXRaw.getValue() * 0.2875;
        lsYOut = lsYRaw.getValue() * 0.6250;
    }
}

void handleShieldTilt()
{
    if (shieldButton.isPressed() && !smashDIButton.isPressed() && !isAirDodging && !xModButton.isPressed() && !yModButton.isPressed())
    {
        lsXOut = lsXRaw.getValue() * 0.6625;
        if (lsDownButton.isPressed())
        {
            lsYOut = lsYRaw.getValue() * 0.6625;
        }
        else
        {
            lsYOut = lsYRaw.getValue() * 0.5625;
        }
    }
}

void handleSafeDownB()
{
    if (bButton.isPressed() && !shieldButton.isPressed() && !isAirDodging && lsDownButton.isPressed() && (lsLeftButton.isPressed() || lsRightButton.isPressed()))
    {
        lsXOut = lsXRaw.getValue() * 0.5875;
        lsYOut = lsYRaw.getValue() * 0.8000;
    }
}

void handleJumpsquatTimingChanges()
{
    // If all digital directions are held, allow the cYAxis to increment jumpsquat timing.
    if (lsLeftButton.isPressed() && lsDownButton.isPressed() && lsUpButton.isPressed() && lsRightButton.isPressed())
    {
        if (cUpButton.justPressed())
            changeJumpsquatFrames(1);
        if (cDownButton.justPressed())
            changeJumpsquatFrames(-1);
    }
}

void disableWavedashForLRAStart()
{
    // To allow for LRA start, disable wavedashing while start is held.
    if (aButton.isPressed() && shieldButton.isPressed() && smashDIButton.isPressed())
    {
        lOut = airdodgeButton.isPressed();
    }
    else
    {
        lOut = wavedashAirdodgeOut;
    }
}

bool autoLCancelOut = false;
unsigned long autoLCancelTime = 0;
bool allowDashAttack = false;
unsigned long dashAttackTime = 0;
void handleAutoLCancelling()
{
    if (aButton.isPressed() || cLeftButton.isPressed() || cRightButton.isPressed() || cUpButton.isPressed() || cDownButton.isPressed())
    {
        if (aButton.justPressed() && (lsLeftButton.isPressed() || lsRightButton.isPressed()))
        {
            allowDashAttack = true;
            dashAttackTime = millis();
        }
        if (millis() - dashAttackTime >= 66)
        {
            allowDashAttack = false;
        }
        bool lCancelButtonsJustPressed = aButton.justPressed() || cLeftButton.justPressed() || cRightButton.justPressed() || cUpButton.justPressed() || cDownButton.justPressed();
        if (!allowDashAttack)
        {
            if (lCancelButtonsJustPressed || (millis() - autoLCancelTime >= 33))
            {
                autoLCancelOut = false;
                autoLCancelTime = millis();
            }
            else
            {
                if (millis() - autoLCancelTime >= 16)
                {
                    autoLCancelOut = true;
                }
            }
        }
    }
    else
    {
        autoLCancelOut = false;
    }
}

unsigned long smashDITime = 0;
void handleSmashDIMacro()
{
    if (smashDIButton.isPressed())
    {
        if (smashDIButton.justPressed() || (millis() - smashDITime >= 33))
        {
            smashDITime = millis();
        }
        else
        {
            if (millis() - smashDITime >= 16)
            {
                lsXOut = 0.0;
                lsYOut = 0.0;
            }
        }
    }
}

// If you hold the XMod, YMod, and Smash DI buttons at the same time,
// the c-stick buttons will turn into DPad buttons.
void handleDPad()
{
    if (smashDIButton.isPressed() && xModButton.isPressed() && yModButton.isPressed())
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

void setup()
{
    Joystick.useManualSend(true);
}

void loop()
{
    readButtons();

    handleJumpsquatTimingChanges();
    handleShortAndFullHops();
    handleWavedash();

    lsXRaw.update(lsLeftButton.isPressed(), lsRightButton.isPressed());
    lsYRaw.update(lsDownButton.isPressed() || wavedashDownOut, lsUpButton.isPressed() && !wavedashDownOut);
    lsXOut = lsXRaw.getValue();
    lsYOut = lsYRaw.getValue();

    cXRaw.update(cLeftButton.isPressed(), cRightButton.isPressed());
    cYRaw.update(cDownButton.isPressed(), cUpButton.isPressed());
    cXOut = cXRaw.getValue();
    cYOut = cYRaw.getValue();

    aOut = aButton.isPressed();
    bOut = bButton.isPressed();
    yOut = shortHopOut || wavedashJumpOut;
    xOut = fullHopOut;
    zOut = zButton.isPressed();
    startOut = startButton.isPressed();

    disableWavedashForLRAStart();

    if (autoLCancelOut)
    {
        lAnalogOut = 80;
    }
    else
    {
        lAnalogOut = 0;
    }

    if (isWavedashing)
    {
        rOut = wavedashShieldOut;
    }
    else
    {
        rOut = shieldButton.isPressed();
    }

    handleAutoLCancelling();
    handleAngleModifiers();
    handleShieldTilt();
    handleSafeDownB();
    handleSmashDIMacro();
    handleAngledSmashes();
    handleDPad();

    sendJoystickOutputs();
}
