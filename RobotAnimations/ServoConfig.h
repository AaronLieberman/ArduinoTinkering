#pragma once

#include "ServoController.h"

// 150 (0) -> 400 (90) -> 650 (180)
const ServoSpec kServoA0090 = {150, 650};
const ServoSpec kServoKy66 = {150, 590};

// clang-format off

// const vector<short> kServoCenter = { 0, 320, 320, 345, 290, 330, 320, 345, 290 };

const std::vector<ServoConfig> kServoSpecs = {
	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoA0090, 0, false},

	{kServoKy66, 90, false},
	{kServoKy66, 90, false},
	{kServoKy66, 90, false},
	{kServoA0090, 45, true},
};

// clang-format on
