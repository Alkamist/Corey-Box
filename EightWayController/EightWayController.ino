#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include "Nintendo.h"

CGamecubeConsole gamecubeOutput(45);
Gamecube_Data_t controllerData;

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

class ButtonAxis
{
public:
    void update(const Button& lowButton, const Button& highButton)
    {
        if (lowButton.justPressed() || (lowButton.isPressed() && !highButton.isPressed()))
        {
            m_value = -1.0;
        }
        else if (highButton.justPressed() || (highButton.isPressed() && !lowButton.isPressed()))
        {
            m_value = 1.0;
        }
        else if (!lowButton.isPressed() && !highButton.isPressed())
        {
            m_value = 0.0;
        }
    }
    const float getValue() const { return m_value; }
private:
    bool m_lowWasFirst{false};
    float m_value{0.0};
};

Button aButton(18, false);
Button bButton(19, false);
Button lsLeftButton(3, false);
Button lsRightButton(5, false);
Button lsDownButton(4, false);
Button lsUpButton(40, false);
Button xModButton(7, false);
Button yModButton(8, false);
Button cLeftButton(38, false);
Button cRightButton(20, false);
Button cDownButton(39, false);
Button cUpButton(21, false);
Button shortHopButton(23, true);
Button fullHopButton(25, true);
Button zButton(24, false);
Button airdodgeButton(22, true);
Button shieldButton(2, false);
Button startButton(27, false);
Button dPadButton(10, false);
Button settingsButton(9, false);
Button smashDIButton(11, false);

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
//uint8_t lAnalogOut = 0;
//uint8_t rAnalogOut = 0;
float lsXOut = 0.0;
float lsYOut = 0.0;
float cXOut = 0.0;
float cYOut = 0.0;

bool isShortHopping = false;
unsigned long shortHopTime = 0;
bool isFullHopping = false;
unsigned long fullHopTime = 0;
bool isAirDodging = false;
unsigned long airDodgeTime = 0;

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
    dPadButton.update();
    settingsButton.update();
    smashDIButton.update();
}

void handleShortAndFullHops()
{
    //if short hop button is pressed or the full hop button is pressed during the 134 ms window:
    //    press y for 33 ms
    //if full hop button is pressed:
    //    press x for as long as the full hop button is held, but no less than 134 ms
    if (shortHopButton.justPressed() || (isFullHopping && fullHopButton.justPressed()))
    {
        isShortHopping = true;
        shortHopTime = millis();
        yOut = true;
    }
    if (!isFullHopping && fullHopButton.justPressed())
    {
        isFullHopping = true;
        fullHopTime = millis();
        xOut = true;
    }

    if (isShortHopping)
    {
        if (millis() - shortHopTime >= 33)
        {
            isShortHopping = false;
            yOut = false;
        }
    }
    if (isFullHopping && !fullHopButton.isPressed())
    {
        if (millis() - fullHopTime >= 134)
        {
            isFullHopping = false;
            xOut = false;
        }
    }
}

void handleAssistedAirdodge()
{
    // If the l button is pressed, push l on that frame, and then r one frame later.
    if (airdodgeButton.justPressed())
    {
        isAirDodging = true;
        airDodgeTime = millis();
        lOut = true;
        rOut = false;
    }
    if (isAirDodging)
    {
        auto currentDuration = millis() - airDodgeTime;
        if (currentDuration >= 16 && currentDuration < 33)
        {
            lOut = false;
            rOut = true;
        }
        if (currentDuration >= 33)
        {
            isAirDodging = false;
        }
    }
    else
    {
        rOut = shieldButton.isPressed();
    }
}

void handleAngleModifiers()
{
    if (xModButton.isPressed())
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
    if (shieldButton.isPressed() && !bButton.isPressed() && !airdodgeButton.isPressed() && !xModButton.isPressed() && !yModButton.isPressed())
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
    if (bButton.isPressed() && !shieldButton.isPressed() && !airdodgeButton.isPressed() && lsDownButton.isPressed() && (lsLeftButton.isPressed() || lsRightButton.isPressed()))
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

    lsXRaw.update(lsLeftButton, lsRightButton);
    lsYRaw.update(lsDownButton, lsUpButton);
    cXRaw.update(cLeftButton, cRightButton);
    cYRaw.update(cDownButton, cUpButton);

    aOut = aButton.isPressed();
    bOut = bButton.isPressed();
    zOut = zButton.isPressed();
    startOut = startButton.isPressed();
    lsXOut = lsXRaw.getValue();
    lsYOut = lsYRaw.getValue();
    cXOut = cXRaw.getValue();
    cYOut = cYRaw.getValue();

    handleShortAndFullHops();
    handleAssistedAirdodge();
    handleAngleModifiers();
    handleShieldTilt();
    handleSafeDownB();

    packAndWriteGamecubeData();

    //Serial.print(uint8_t(lsXOut * 80 + 127));
    //Serial.println();
    //delay(200);
}
