#pragma once
#include "cru/common/Base.hpp"
#include "cru/common/Bitmask.hpp"
#include "cru/platform/graphics/Base.hpp"

#include "../Resource.hpp"

#ifdef CRU_PLATFORM_WINDOWS
#ifdef CRU_PLATFORM_GUI_EXPORT_API
#define CRU_PLATFORM_GUI_API __declspec(dllexport)
#else
#define CRU_PLATFORM_GUI_API __declspec(dllimport)
#endif
#else
#define CRU_PLATFORM_GUI_API
#endif

namespace cru::platform::gui {
struct ICursor;
struct ICursorManager;
struct IUiApplication;
struct INativeWindow;
struct IInputMethodContext;

namespace details {
struct TagMouseButton {};
}  // namespace details

using MouseButton = Bitmask<details::TagMouseButton>;

namespace mouse_buttons {
constexpr MouseButton left{0b1};
constexpr MouseButton middle{0b10};
constexpr MouseButton right{0b100};
}  // namespace mouse_buttons
}  // namespace cru::platform::gui
