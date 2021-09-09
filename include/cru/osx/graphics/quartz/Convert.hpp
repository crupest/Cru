#pragma once
#include "cru/platform/Matrix.hpp"
#include "cru/common/String.hpp"
#include "cru/common/Range.hpp"

#include <CoreGraphics/CoreGraphics.h>

namespace cru::platform::graphics::osx::quartz {
CFStringRef Convert(const String& string);
String Convert(CFStringRef string);

CGAffineTransform Convert(const Matrix& matrix);
Matrix Convert(const CGAffineTransform& matrix);

CGRect Convert(const Rect& rect);
Rect Convert(const CGRect& rect);

CFRange Convert(const Range& range);
Range Convert(const CFRange& range);
}  // namespace cru::platform::graphics::osx::quartz
