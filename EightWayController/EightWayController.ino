#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include "Nintendo.h"

CGamecubeConsole gamecubeOutput(45);
Gamecube_Data_t controllerData;

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

Button aButton(38, false);
Button bButton(18, false);
Button lsLeftButton(2, false);
Button lsRightButton(4, false);
Button lsDownButton(3, false);
Button lsUpButton(27, false);
Button xModButton(5, false);
Button yModButton(8, false);
Button cLeftButton(39, false);
Button cRightButton(19, false);
Button cDownButton(40, false);
Button cUpButton(20, false);
Button shortHopButton(23, true);
Button fullHopButton(25, true);
Button zButton(21, false);
Button airdodgeButton(22, true);
Button shieldButton(24, false);
Button startButton(9, false);
Button smashDIButton(7, false);

ButtonAxis lsXRaw;
ButtonAxis lsYRaw;
ButtonAxis cXRaw;
ButtonAxis cYRaw;

// Define the outputs that eventually get pushed out to the GameCube.
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
//uint8_t lAnalogOut = 0;
//uint8_t rAnalogOut = 0;
float lsXOut = 0.0;
float lsYOut = 0.0;
float cXOut = 0.0;
float cYOut = 0.0;

unsigned long framesToMillis(const float frames)
{
    return floor(1000.0 * frames / 60.0);
}

void packAndWriteGamecubeData()
{
    controllerData.report.a = aOut;
    controllerData.report.b = bOut;
    controllerData.report.x = xOut;
    controllerData.report.y = yOut;
    controllerData.report.z = zOut;
    controllerData.report.l = lOut;
    controllerData.report.r = rOut;
    controllerData.report.start = startOut;
    controllerData.report.dleft = dLeftOut;
    controllerData.report.dright = dRightOut;
    controllerData.report.ddown = dDownOut;
    controllerData.report.dup = dUpOut;
    //controllerData.report.left = lAnalogOut;
    //controllerData.report.right = rAnalogOut;
    controllerData.report.xAxis = uint8_t(lsXOut * 80 + 128);
    controllerData.report.yAxis = uint8_t(lsYOut * 80 + 128);
    controllerData.report.cxAxis = uint8_t(cXOut * 80 + 128);
    controllerData.report.cyAxis = uint8_t(cYOut * 80 + 128);
    gamecubeOutput.write(controllerData);
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
    if (needToAirdodge && (millis() - wavedashTime >= framesToMillis(float(jumpsquatFrames) - 0.5)))
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
    if (shieldButton.isPressed() && !bButton.isPressed() && !isAirDodging && !xModButton.isPressed() && !yModButton.isPressed())
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

void setup()
{}

void loop()
{
    readButtons();

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

    lOut = wavedashAirdodgeOut;
    if (isWavedashing)
    {
        rOut = wavedashShieldOut;
    }
    else
    {
        rOut = shieldButton.isPressed();
    }

    handleAngleModifiers();
    handleShieldTilt();
    handleSafeDownB();

    packAndWriteGamecubeData();
}
