#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>

float sign(float value) {
  if (value < 0.0f) return -1.0f;
  else if (value > 0.0f) return 1.0f;
  return 0.0f;
}

float bipolarMax(float value, float magnitude) {
  if (value > 0.0f) return max(value, magnitude);
  else if (value < 0.0f) return min(value, -magnitude);
  return 0.0f;
}

class State {
public:
  explicit State(bool initialState)
    : m_currentState(initialState),
      m_previousState(initialState) {}

  void update() { m_previousState = m_currentState; }
  void setState(bool newState) { m_currentState = newState; }
  const bool isActive() const { return m_currentState; }
  const bool justActivated() const { return m_currentState && !m_previousState; }
  const bool justDeactivated() const { return !m_currentState && m_previousState; }

private:
  bool m_currentState{false};
  bool m_previousState{false};
};

class Button {
public:
  explicit Button(uint8_t pinNumber, bool useBounce)
    : m_pinNumber(pinNumber),
      m_useBounce(useBounce) {

    pinMode(m_pinNumber, INPUT_PULLUP);

    if (m_useBounce) {
      m_bounce.attach(m_pinNumber);
      m_bounce.interval(4);
    }
  }

  void update() {
    m_previousState = m_currentState;

    if (m_useBounce) {
      m_bounce.update();
      m_currentState = !m_bounce.read();
    }

    else {
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

class AnalogAxis {
public:
  explicit AnalogAxis(float deadZone)
    : m_deadZone(deadZone) {}

  float value() const { return m_value; }
  float direction() const { return sign(m_value); }
  float deadZone() const { return m_deadZone; }
  bool isActive() const { return abs(m_value) >= m_deadZone; }

  void setValue(float value) { m_value = value; }

  void setValueFromStates(bool lowState, bool highState) {
    bool lowAndHigh = lowState && highState;
    bool onlyHigh = highState && !lowState;
    bool onlyLow = lowState && !highState;

    if (onlyHigh) {
      m_highStateWasFirst = true;
    }
    else if (onlyLow) {
      m_highStateWasFirst = false;
    }

    if (onlyLow || (lowAndHigh && m_highStateWasFirst)) {
      m_value = -1.0f;
    }
    else if (onlyHigh || (lowAndHigh && !m_highStateWasFirst)) {
      m_value = 1.0f;
    }
    else {
      m_value = 0.0f;
    }
  }

private:
  float m_value;
  float m_deadZone;
  bool m_highStateWasFirst;
};

Button extraButton(16, false);
Button yModButton(8, false);
Button xModButton(7, false);
Button leftButton(3, false);
Button rightButton(6, false);
Button downButton(4, false);
Button upButton(5, false);
Button aButton(20, false);
Button bButton(22, false);
Button zButton(15, false);
Button startButton(9, false);
Button cLeftButton(10, false);
Button cRightButton(19, false);
Button cDownButton(23, false);
Button cUpButton(21, false);
Button shortHopButton(17, true);
Button fullHopButton(14, false);
Button shieldButton(2, false);
Button airDodgeButton(18, false);

AnalogAxis xAxis(0.2875f);
AnalogAxis yAxis(0.2875f);
AnalogAxis cXAxis(0.2875f);
AnalogAxis cYAxis(0.2875f);

bool xOut = false;
bool aOut = false;
bool bOut = false;
bool yOut = false;
bool lOut = false;
bool rOut = false;
bool zOut = false;
bool startOut = false;
bool dUpOut = false;
bool dDownOut = false;
bool dLeftOut = false;
bool dRightOut = false;
float xAxisOut = 0.0f;
float yAxisOut = 0.0f;
float cXAxisOut = 0.0f;
float cYAxisOut = 0.0f;
float lAnalogOut = 0.0f;

bool isLightShielding = false;
bool delayBackdash = false;
bool isDoingSafeDownB = false;
bool isShortHopping = false;
bool isFullHopping = false;
bool isAirDodging = false;
bool isUpTilting = false;
bool isDownTilting = false;
bool isLeftTilting = false;
bool isRightTilting = false;
bool isDoingNeutralA = false;

uint32_t backdashTime = 0;
uint32_t safeDownBTime = 0;
uint32_t shortHopTime = 0;
uint32_t fullHopTime = 0;
uint32_t airDodgeTime = 0;
uint32_t aAttackTime = 0;

void sendJoystickOutputs() {
  Joystick.button(1, aOut);
  Joystick.button(2, bOut);
  Joystick.button(3, xOut);
  Joystick.button(4, yOut);
  Joystick.button(5, zOut);
  Joystick.button(6, lOut);
  Joystick.button(7, rOut);
  Joystick.button(8, startOut);
  Joystick.button(9, dLeftOut);
  Joystick.button(10, dUpOut);
  Joystick.button(11, dRightOut);
  Joystick.button(12, dDownOut);

  if (xAxisOut >= 0.0) Joystick.X(xAxisOut * 322 + 512);
  else Joystick.X(xAxisOut * 320 + 512);

  if (yAxisOut >= 0.0) Joystick.Y(yAxisOut * 324 + 512);
  else Joystick.Y(yAxisOut * 320 + 512);

  if (cYAxisOut >= 0.0) Joystick.Z(cYAxisOut * 322 + 512);
  else Joystick.Z(cYAxisOut * 320 + 512);

  if (cXAxisOut >= 0.0) Joystick.Zrotate(-cXAxisOut * 298 + 680);
  else Joystick.Zrotate(-cXAxisOut * 220 + 680);

  if (lAnalogOut > 42) Joystick.sliderLeft(512 + lAnalogOut * 2.0);
  else Joystick.sliderLeft(0);

  Joystick.send_now();
}

void readButtons() {
  extraButton.update();
  yModButton.update();
  xModButton.update();
  leftButton.update();
  rightButton.update();
  downButton.update();
  upButton.update();
  aButton.update();
  bButton.update();
  zButton.update();
  startButton.update();
  cLeftButton.update();
  cRightButton.update();
  cDownButton.update();
  cUpButton.update();
  shortHopButton.update();
  fullHopButton.update();
  shieldButton.update();
  airDodgeButton.update();
}

void updateAxesFromDirections() {
  xAxis.setValueFromStates(leftButton.isPressed(), rightButton.isPressed());
  yAxis.setValueFromStates(downButton.isPressed(), upButton.isPressed());

  bool tilt = xModButton.isPressed() || yModButton.isPressed();
  bool enableCStick = shieldButton.isPressed() || !tilt;

  cXAxis.setValueFromStates(cLeftButton.isPressed() && enableCStick,
                            cRightButton.isPressed() && enableCStick);
  cYAxis.setValueFromStates(cDownButton.isPressed() && enableCStick,
                            cUpButton.isPressed() && enableCStick);
}

void handleBackdashOutOfCrouchFix() {
  if (downButton.isPressed() &&
     (leftButton.justPressed() || rightButton.justPressed())) {
    delayBackdash = true;
    backdashTime = millis();
  }

  if (downButton.justReleased()) {
    delayBackdash = false;
  }

  if (delayBackdash) {
    if (!(fullHopButton.isPressed() ||
          shortHopButton.isPressed() ||
          airDodgeButton.isPressed() ||
          shieldButton.isPressed() ||
          zButton.isPressed() ||
          aButton.isPressed() ||
          bButton.isPressed())) {
      xAxisOut = 0.0f;
    }

    if (millis() - backdashTime >= 50) {
      delayBackdash = false;
    }
  }
}

void handleModifierAngles() {
  bool diagonal = (leftButton.isPressed() || rightButton.isPressed()) &&
                  (downButton.isPressed() || upButton.isPressed());

  if (xModButton.isPressed()) {
    float x = diagonal ? 0.7375f : 0.6625f;
    float y = diagonal ? 0.3125f : 0.5375f;

    xAxisOut = xAxis.direction() * x;
    yAxisOut = yAxis.direction() * y;
  }
  else if (yModButton.isPressed()) {
    float x = diagonal ? 0.3125f : 0.3375f;
    float y = 0.7375f;

    xAxisOut = xAxis.direction() * x;
    yAxisOut = yAxis.direction() * y;
  }
}

void handleCStickTilting() {
  aOut = aButton.isPressed();

  if (!shieldButton.isPressed()) {
    bool tilt = xModButton.isPressed() || yModButton.isPressed();

    // Left Tilt:
    if (tilt && cLeftButton.justPressed()) {
      aAttackTime = millis();
      isLeftTilting = true;
    }
    if (isLeftTilting) {
      aOut = true;
      xAxisOut = -0.6f;
      yAxisOut = yAxis.direction() * 0.35f;
    }
    if (millis() - aAttackTime >= 34) {
      isLeftTilting = false;
    }

    // Right Tilt:
    if (tilt && cRightButton.justPressed()) {
      aAttackTime = millis();
      isRightTilting = true;
    }
    if (isRightTilting) {
      aOut = true;
      xAxisOut = 0.6f;
      yAxisOut = yAxis.direction() * 0.35f;
    }
    if (millis() - aAttackTime >= 34) {
      isRightTilting = false;
    }

    // Down Tilt:
    if (tilt && cDownButton.justPressed()) {
      aAttackTime = millis();
      isDownTilting = true;
    }
    if (isDownTilting) {
      aOut = true;
      xAxisOut = xAxis.direction() * 0.35f;
      yAxisOut = -0.6f;
    }
    if (millis() - aAttackTime >= 34) {
      isDownTilting = false;
    }

    // Up Tilt:
    if (tilt && cUpButton.justPressed()) {
      aAttackTime = millis();
      isUpTilting = true;
    }
    if (isUpTilting) {
      aOut = true;
      xAxisOut = xAxis.direction() * 0.35f;
      yAxisOut = 0.6f;
    }
    if (millis() - aAttackTime >= 34) {
      isUpTilting = false;
    }
  }

  // Neutral A:
  if (aButton.justPressed()) {
    aAttackTime = millis();
    isDoingNeutralA = true;
  }
  if (isDoingNeutralA) {
    aOut = true;
    xAxisOut = 0.0f;
    yAxisOut = 0.0f;
  }
  if (millis() - aAttackTime >= 34) {
    isDoingNeutralA = false;
  }
}

void handleSafeDownB() {
  if (bButton.justPressed() &&
     (downButton.isPressed() || upButton.isPressed())) {
    isDoingSafeDownB = true;
    safeDownBTime = millis();
  }

  if (isDoingSafeDownB) {
    if (millis() - safeDownBTime < 25) {
      xAxisOut = xAxis.direction() * 0.5875f;
      yAxisOut = yAxis.direction() * 0.6f;
    }

    else {
      isDoingSafeDownB = false;
    }
  }
}

void handleAirDodgeLogic() {
  bool isLeft = xAxis.isActive() && xAxis.value() < 0.0f;
  bool isRight = xAxis.isActive() && xAxis.value() > 0.0f;
  bool isDown = yAxis.isActive() && yAxis.value() < 0.0f;
  bool isUp = yAxis.isActive() && yAxis.value() > 0.0f;
  bool isSideways = (isLeft || isRight) && !isDown;

  if (airDodgeButton.justPressed()) {
    isAirDodging = true;
    airDodgeTime = millis();
  }

  if (isAirDodging && !isUp) {
    if (millis() - airDodgeTime < 51) {
      if (isSideways) {
        xAxisOut = xAxis.direction() * 0.7375f;
        yAxisOut = -0.3125f;
      }

      else if (!isDown) {
        yAxisOut = -0.3f;
      }
    }
    else {
      isAirDodging = false;
    }
  }
}

void handleAngledSmashes() {
  bool tilt = xModButton.isPressed() || yModButton.isPressed();
  bool cAngled = (cLeftButton.isPressed() || cRightButton.isPressed()) &&
                 (downButton.isPressed() || upButton.isPressed());

  if (cAngled && !tilt) {
    cYAxisOut = yAxis.direction() * 0.3375f;
  }
}

void handleJumpLogic() {
  // Short hop handling.
  bool startShortHop = shortHopButton.justPressed() ||
                      (isFullHopping && fullHopButton.justPressed());

  if (startShortHop) {
    yOut = true;
    isShortHopping = true;
    shortHopTime = millis();
  }

  if (isShortHopping && millis() - shortHopTime >= 25) {
    yOut = false;
    isShortHopping = false;
  }

  // Full hop handling.
  bool startFullHop = fullHopButton.justPressed();

  if (startFullHop) {
    xOut = true;
    isFullHopping = true;
    fullHopTime = millis();
  }

  if (isFullHopping && !fullHopButton.isPressed()) {
    if (millis() - fullHopTime >= 134) {
      xOut = false;
    }

    // Wait one extra frame so you can't miss a double jump by
    // pushing the full hop button on the same frame of release.
    if (millis() - fullHopTime >= 150) {
      isFullHopping = false;
    }
  }
}

void handleShieldTilt() {
  if (shieldButton.isPressed()) {
    float scaleValue = 0.6625f;

    float xAxisMagnitude = abs(xAxis.value());
    if (xAxisMagnitude > scaleValue) {
      float scaleFactor = scaleValue / xAxisMagnitude;
      xAxisOut = xAxis.direction() * scaleValue;
      yAxisOut = bipolarMax(yAxis.value() * scaleFactor, yAxis.deadZone());
    }

    float yAxisMagnitude = abs(yAxis.value());
    if (yAxisMagnitude > scaleValue) {
      float scaleFactor = scaleValue / yAxisMagnitude;
      yAxisOut = yAxis.direction() * scaleValue;
      xAxisOut = bipolarMax(xAxisOut * scaleFactor, xAxis.deadZone());
    }
  }
}

void setup() {
  Joystick.useManualSend(true);
}

void loop() {
  readButtons();
  updateAxesFromDirections();

  dDownOut = extraButton.isPressed();
  bOut = bButton.isPressed();
  lOut = airDodgeButton.isPressed();
  rOut = shieldButton.isPressed();
  zOut = zButton.isPressed();
  startOut = startButton.isPressed();
  xAxisOut = xAxis.value();
  yAxisOut = yAxis.value();
  cXAxisOut = cXAxis.value();
  cYAxisOut = cYAxis.value();

  handleBackdashOutOfCrouchFix();
  handleModifierAngles();
  handleShieldTilt();
  handleCStickTilting();
  handleSafeDownB();
  handleAirDodgeLogic();
  handleAngledSmashes();
  handleJumpLogic();

  sendJoystickOutputs();
}