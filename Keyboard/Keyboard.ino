#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>

class State {
public:
    explicit State(const bool initialState)
    : m_currentState(initialState),
      m_previousState(initialState) {}

    void update() { m_previousState = m_currentState; }
    void setState(const bool newState) { m_currentState = newState; }
    const bool isActive() const { return m_currentState; }
    const bool justActivated() const { return m_currentState && !m_previousState; }
    const bool justDeactivated() const { return !m_currentState && m_previousState; }

private:
    bool m_currentState{false};
    bool m_previousState{false};
};

class Key {
public:
    explicit Key(const uint8_t pinNumber,
                 const int keyCode,
                 const bool useBounce)
    : m_pinNumber(pinNumber),
      m_keyCode(keyCode),
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

        if (justPressed()) {
            Keyboard.press(m_keyCode);
        }

        else if (justReleased()) {
            Keyboard.release(m_keyCode);
        }
    }

    const bool isPressed() const { return m_currentState; }
    const bool justPressed() const { return m_currentState && !m_previousState; }
    const bool justReleased() const { return !m_currentState && m_previousState; }

private:
    Bounce m_bounce;
    uint8_t m_pinNumber{3};
    int m_keyCode{KEY_A};
    bool m_useBounce{false};
    bool m_currentState{false};
    bool m_previousState{false};
};

Key tiltButton(2, KEY_A, true);
Key yModButton(7, KEY_F, true);
Key xModButton(8, KEY_G, true);

Key leftButton(3, KEY_B, true);
Key rightButton(6, KEY_E, true);
Key downButton(4, KEY_C, true);
Key upButton(5, KEY_D, true);

Key aButton(20, KEY_M, true);
Key bButton(22, KEY_K, true);
Key zButton(15, KEY_R, true);

Key startButton(9, KEY_H, true);

Key cLeftButton(10, KEY_I, true);
Key cRightButton(19, KEY_N, true);
Key cDownButton(23, KEY_J, true);
Key cUpButton(21, KEY_L, true);

Key shortHopButton(17, KEY_P, true);
Key fullHopButton(16, KEY_Q, true);

Key shieldButton(14, KEY_S, true);
Key airdodgeButton(18, KEY_O, true);

void setup() {}

void loop() {
    tiltButton.update();
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
    airdodgeButton.update();
}