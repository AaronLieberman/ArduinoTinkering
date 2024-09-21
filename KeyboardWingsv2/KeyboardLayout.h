#pragma once

#include <HID-Project.h>

#include <iostream>

struct LayoutKey {
    KeyboardKeycode keyboardKeycode;
    ConsumerKeycode consumerKeycode;

    bool operator==(const LayoutKey &other) const {
        return keyboardKeycode == other.keyboardKeycode && consumerKeycode == other.consumerKeycode;
    }
};

namespace std {
template <>
struct hash<LayoutKey> {
    std::size_t operator()(const LayoutKey &key) const {
        std::size_t h1 = std::hash<uint8_t>()(static_cast<uint8_t>(key.keyboardKeycode));
        std::size_t h2 = std::hash<uint16_t>()(static_cast<uint16_t>(key.consumerKeycode));
        return h1 ^ (h2 << 1);
    }
};
}  // namespace std

class KeyboardLayout {
public:
    LayoutKey getKey(int row, int col);

private:
};
