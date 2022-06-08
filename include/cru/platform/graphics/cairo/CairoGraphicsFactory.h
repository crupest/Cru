#pragma once

#include "../Factory.h"
#include "CairoResource.h"

#include <cairo/cairo.h>
#include <pango/pango.h>

namespace cru::platform::graphics::cairo {
class CRU_PLATFORM_GRAPHICS_CAIRO_API CairoGraphicsFactory
    : public CairoResource,
      public virtual IGraphicsFactory {
 public:
  CairoGraphicsFactory();
  ~CairoGraphicsFactory() override;

 public:
  cairo_surface_t* GetDefaultCairoSurface() { return default_cairo_surface_; }
  cairo_t* GetDefaultCairo() { return default_cairo_; }
  PangoContext* GetDefaultPangoContext() { return default_pango_context_; }

 private:
  cairo_surface_t* default_cairo_surface_;
  cairo_t* default_cairo_;
  PangoContext* default_pango_context_;
};
}  // namespace cru::platform::graphics::cairo