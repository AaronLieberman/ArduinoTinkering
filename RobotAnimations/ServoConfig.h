#pragma once

#include "ServoController.h"

// 150 (0) -> 400 (90) -> 650 (180)
const ServoSpec kServoA0090 = {150, 650};
const ServoSpec kServoKy66 = {150, 590};

// clang-format off

const std::vector<ServoConfig> kServoSpecs = {
	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoA0090, 170, true},

	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoKy66, 90, true},
	{kServoA0090, 125, false},
};

// clang-format on
  