#pragma once
#include "cru/common/pre_config.hpp"

#include <string_view>

namespace cru::platform::debug {
void DebugMessage(const std::wstring_view& message);
}