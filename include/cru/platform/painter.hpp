#pragma once
#include "cru/common/base.hpp"

#include "cru/common/ui_base.hpp"

namespace cru::platform {
struct Brush;
struct Geometry;

struct Painter : virtual Interface {
  virtual void StrokeGeometry(Geometry* geometry, Brush* brush,
                              float width) = 0;
  virtual void FillGeometry(Geometry* geometry, Brush* brush) = 0;
  virtual void EndDraw() = 0;
  virtual bool IsDisposed() = 0;
};
}  // namespace cru::platform
