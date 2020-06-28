#pragma once
#include "cru/common/Base.hpp"
#include "cru/common/Bitmask.hpp"
#include "cru/platform/graph/Base.hpp"
#include "Keyboard.hpp"

namespace cru::platform::native {
struct ICursor;
struct ICursorManager;
struct IUiApplication;
struct INativeWindow;
struct INativeWindowResolver;
struct IInputMethodManager;
struct IInputMethodContext;

struct Dpi {
  float x;
  float y;
};

namespace details {
struct TagMouseButton {};
}  // namespace details

using MouseButton = Bitmask<details::TagMouseButton>;

namespace mouse_buttons {
constexpr MouseButton left{0b1};
constexpr MouseButton middle{0b10};
constexpr MouseButton right{0b100};
}  // namespace mouse_buttons

enum class SystemCursorType {
  Arrow,
  Hand,
};

struct NativeMouseButtonEventArgs {
  MouseButton button;
  Point point;
  KeyModifier modifier;
};

struct NativeKeyEventArgs {
  KeyCode key;
  KeyModifier modifier;
};

enum class FocusChangeType { Gain, Lost };

enum class MouseEnterLeaveType { Enter, Leave };
}  // namespace cru::platform::native