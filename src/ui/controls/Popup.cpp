#include "cru/ui/controls/Popup.hpp"

#include "cru/platform/gui/UiApplication.hpp"
#include "cru/ui/controls/RootControl.hpp"
#include "cru/ui/host/WindowHost.hpp"
#include "cru/ui/render/StackLayoutRenderObject.hpp"

#include <memory>

namespace cru::ui::controls {
Popup::Popup(Control* attached_control)
    : RootControl(
          attached_control,
          host::CreateWindowParams{
              nullptr, platform::gui::CreateWindowFlags::NoCaptionAndBorder}) {
  SetGainFocusOnCreateAndDestroyWhenLoseFocus(true);
}

Popup::~Popup() = default;
}  // namespace cru::ui::controls
