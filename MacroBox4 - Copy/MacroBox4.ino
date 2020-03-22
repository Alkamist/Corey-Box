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
            m_bounce.interval(4);
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

Button shortHopButton(21, true);
Button fullHopButton(19, true);
Button shieldButton(7, false);
Button airdodgeButton(20, true);
Button aButton(15, false);
Button bButton(16, false);
Button zButton(22, false);
Button startButton(9, false);
Button cUpButton(18, false);
Button cLeftButton(14, false);
Button cRightButton(17, false);
Button cDownButton(10, false);
Button lsUpButton(23, false);
Button lsLeftButton(3, false);
Button lsRightButton(5, false);
Button lsDownButton(4, false);
Button smashDIButton(6, false);
Button xModButton(2, false);
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

void handleAngleModifiers()
{
    bool isDiagonal = (lsLeftButton.isPressed() || lsRightButton.isPressed()) && (lsDownButton.isPressed() || lsUpButton.isPressed());
    bool isAirdodging = shieldButton.isPressed() || airdodgeButton.isPressed();
    if (xModButton.isPressed() && !airdodgeButton.isPressed())
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
    if (smashDIButton.isPressed())
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
    && !fullHopButton.isPressed() && !shortHopButton.isPressed())
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

int sign(const float input)
{
    if (input < 0.0) return -1;
    else return 1;
}

unsigned long smashDITime = 0;
void handleSmashDIMacro()
{
    if (xModButton.isPressed() && yModButton.isPressed())
    {
        if (xModButton.justPressed() || yModButton.justPressed() || (millis() - smashDITime >= 33))
        {
            smashDITime = millis();
        }
        else
        {
            bool horizontal = lsLeftButton.isPressed() || lsRightButton.isPressed();
            bool vertical = lsDownButton.isPressed() || lsUpButton.isPressed();
            bool diagonal = horizontal && vertical;
            if (millis() - smashDITime >= 16)
            {
                if (diagonal)
                {
                    lsXOut = 0.0;
                    lsYOut = sign(lsYRaw.getValue());
                }
                else
                {
                    if (horizontal)
                    {
                        lsXOut = sign(lsXRaw.getValue());
                        lsYOut = 0.7;
                    }
                    else if (vertical)
                    {
                        lsXOut = 0.7;
                        lsYOut = sign(lsYRaw.getValue());
                    }
                }
            }
            else
            {
                if (diagonal)
                {
                    lsXOut = sign(lsXRaw.getValue());
                    lsYOut = 0.0;
                }
                else
                {
                    if (horizontal)
                    {
                        lsXOut = sign(lsXRaw.getValue());
                        lsYOut = -0.7;
                    }
                    else if (vertical)
                    {
                        lsXOut = -0.7;
                        lsYOut = sign(lsYRaw.getValue());
                    }
                }
            }
        }
    }
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
    if (airdodgeButton.justPressed() && !xModButton.isPressed() && !yModButton.isPressed())
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

bool isWavelanding = false;
unsigned long wavelandTime = 0;
void handleWaveland()
{
    bool wavelandSideways = !lsUpButton.isPressed() && !lsDownButton.isPressed();
    if (airdodgeButton.justPressed() || isAirDodging)
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

void setup()
{
    Joystick.useManualSend(true);
}

void loop()
{
    readButtons();

    handleJumpsquatTimingChanges();
    handleWavedash();
    handleShortAndFullHops();
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
    yOut = shortHopOut || wavedashJumpOut;
    xOut = fullHopOut;
    zOut = zButton.isPressed();
    //lOut = shieldButton.isPressed();
    //rOut = airdodgeButton.isPressed();
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

    if (xModButton.isPressed() || yModButton.isPressed())
    {
        rOut = airdodgeButton.isPressed();
    }
    else
    {
        rOut = wavedashAirdodgeOut;
    }

    if (isWavedashing)
    {
        lOut = wavedashShieldOut;
    }
    else
    {
        lOut = shieldButton.isPressed();
    }

    handleAngleModifiers();
    handleShieldTilt();
    handleBackdashOutOfCrouchFix();
    handleSafeGroundB();
    handleAngledSmashes();
    handleDPad();
    handleWaveland();
    handleSmashDIMacro();

    sendJoystickOutputs();
}
