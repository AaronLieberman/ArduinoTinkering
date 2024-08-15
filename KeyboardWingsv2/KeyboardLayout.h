#pragma once

#include <HID-Project.h>

struct LayoutKey {
    KeyboardKeycode keyboardKeycode;
    ConsumerKeycode consumerKeycode;
};

class KeyboardLayout {
public:
    LayoutKey getKey(int row, int col);

private:
};
