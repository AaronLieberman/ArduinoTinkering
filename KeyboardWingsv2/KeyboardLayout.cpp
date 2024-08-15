#include "KeyboardLayout.h"

#include <array>

const uint8_t kRows = 6;
const uint8_t kCols = 16;

const KeyboardKeycode NONE = KEY_RESERVED;

const std::array<KeyboardKeycode, kRows * kCols> _layoutKeyboard = {
    KEY_ESC,        KEY_F1,         KEY_F2,         KEY_F3,         KEY_F4,         KEY_F5,         KEY_F6,         KEY_F7,         KEY_F8,         KEY_F9,         KEY_F10,        KEY_F11,        KEY_F12,        KEY_PRINT,      KEY_INSERT,     KEY_PAUSE,
    KEY_TILDE,      KEY_1,          KEY_2,          KEY_3,          KEY_4,          KEY_5,          KEY_6,          KEY_7,          KEY_8,          KEY_9,          KEY_0,          KEY_MINUS,      KEY_EQUAL,      KEY_BACKSPACE,  KEY_DELETE,     NONE,
    NONE,           KEY_TAB,        KEY_Q,          KEY_W,          KEY_E,          KEY_R,          KEY_T,          KEY_Y,          KEY_U,          KEY_I,          KEY_O,          KEY_P,          KEY_LEFT_BRACE, KEY_RIGHT_BRACE,KEY_BACKSLASH,  KEY_HOME,
    NONE,           KEY_CAPS_LOCK,  KEY_A,          KEY_S,          KEY_D,          KEY_F,          KEY_G,          KEY_H,          KEY_J,          KEY_K,          KEY_L,          KEY_SEMICOLON,  KEY_QUOTE,      KEY_ENTER,      NONE,           KEY_END,
    NONE,           KEY_LEFT_SHIFT, KEY_Z,          KEY_X,          KEY_C,          KEY_V,          KEY_B,          KEY_N,          KEY_M,          KEY_COMMA,      KEY_PERIOD,     KEY_SLASH,      KEY_RIGHT_SHIFT,KEY_UP,         NONE,           KEY_PAGE_UP,
    NONE,           NONE,           NONE,           KEY_LEFT_CTRL,  KEY_LEFT_GUI,   KEY_LEFT_ALT,   KEY_SPACE,      KEY_SPACE,      KEY_RIGHT_ALT,  KEY_RIGHT_GUI,  KEY_RIGHT_CTRL, KEY_LEFT,       KEY_DOWN,       KEY_RIGHT,      NONE,           KEY_PAGE_DOWN,
};

LayoutKey KeyboardLayout::getKey(int row, int col) {
    if (row < 0 || row > kRows || col < 0 || col > kCols )
        return { KEY_RESERVED, (ConsumerKeycode)0 };
    return { _layoutKeyboard[row * kCols + col], (ConsumerKeycode)0 };
}
