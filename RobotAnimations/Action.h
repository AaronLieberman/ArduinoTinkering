#pragma once

#include "Frame.h"

struct Action
{
  const char* name;
  std::vector<Frame> frames;
};
