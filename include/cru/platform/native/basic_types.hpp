#pragma once
#include "cru/common/pre_config.hpp"

namespace cru::platform::native {
struct Dpi {
  float x;
  float y;
};

enum class MouseButton { Left, Right, Middle };
}  // namespace cru::platform