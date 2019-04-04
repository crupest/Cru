#pragma once
#include "cru/common/base.hpp"

#include "cru/common/event.hpp"
#include "cru/common/ui_base.hpp"
#include "cru/platform/basic_types.hpp"

#include <optional>

namespace cru::platform {
struct Painter;
}

namespace cru::ui {
class Control;
}

namespace cru::ui::event {
class UiEventArgs : public Object {
 public:
  UiEventArgs(Object* sender, Object* original_sender)
      : sender_(sender),
        original_sender_(original_sender),
        handled_(false) {}

  UiEventArgs(const UiEventArgs& other) = default;
  UiEventArgs(UiEventArgs&& other) = default;
  UiEventArgs& operator=(const UiEventArgs& other) = default;
  UiEventArgs& operator=(UiEventArgs&& other) = default;
  ~UiEventArgs() override = default;

  Object* GetOriginalSender() const { return original_sender_; }

  bool IsHandled() const { return handled_; }
  void SetHandled(const bool handled = true) { handled_ = handled; }

 private:
  Object* sender_;
  Object* original_sender_;
  bool handled_;
};

template <typename TEventArgs>
class RoutedEvent {
 public:
  static_assert(std::is_base_of_v<UiEventArgs, TEventArgs>,
                "TEventArgs must be subclass of UiEventArgs.");

  using EventArgs = TEventArgs;

  RoutedEvent() = default;
  RoutedEvent(const RoutedEvent& other) = delete;
  RoutedEvent(RoutedEvent&& other) = delete;
  RoutedEvent& operator=(const RoutedEvent& other) = delete;
  RoutedEvent& operator=(RoutedEvent&& other) = delete;
  ~RoutedEvent() = default;

  Event<TEventArgs> direct;
  Event<TEventArgs> bubble;
  Event<TEventArgs> tunnel;
};

class MouseEventArgs : public UiEventArgs {
 public:
  MouseEventArgs(Object* sender, Object* original_sender,
                 const std::optional<Point>& point = std::nullopt)
      : UiEventArgs(sender, original_sender), point_(point) {}
  MouseEventArgs(const MouseEventArgs& other) = default;
  MouseEventArgs(MouseEventArgs&& other) = default;
  MouseEventArgs& operator=(const MouseEventArgs& other) = default;
  MouseEventArgs& operator=(MouseEventArgs&& other) = default;
  ~MouseEventArgs() override = default;

  Point GetPoint() const { return point_.value_or(Point{}); }

 private:
  std::optional<Point> point_;
};

class MouseButtonEventArgs : public MouseEventArgs {
 public:
  MouseButtonEventArgs(Object* sender, Object* original_sender,
                       const Point& point, const platform::MouseButton button)
      : MouseEventArgs(sender, original_sender, point), button_(button) {}
  MouseButtonEventArgs(const MouseButtonEventArgs& other) = default;
  MouseButtonEventArgs(MouseButtonEventArgs&& other) = default;
  MouseButtonEventArgs& operator=(const MouseButtonEventArgs& other) = default;
  MouseButtonEventArgs& operator=(MouseButtonEventArgs&& other) = default;
  ~MouseButtonEventArgs() override = default;

  platform::MouseButton GetMouseButton() const { return button_; }

 private:
  platform::MouseButton button_;
};

class MouseWheelEventArgs : public MouseEventArgs {
 public:
  MouseWheelEventArgs(Object* sender, Object* original_sender,
                      const Point& point, const float delta)
      : MouseEventArgs(sender, original_sender, point), delta_(delta) {}
  MouseWheelEventArgs(const MouseWheelEventArgs& other) = default;
  MouseWheelEventArgs(MouseWheelEventArgs&& other) = default;
  MouseWheelEventArgs& operator=(const MouseWheelEventArgs& other) = default;
  MouseWheelEventArgs& operator=(MouseWheelEventArgs&& other) = default;
  ~MouseWheelEventArgs() override = default;

  float GetDelta() const { return delta_; }

 private:
  float delta_;
};

class PaintEventArgs : public UiEventArgs {
 public:
  PaintEventArgs(Object* sender, Object* original_sender,
                platform::Painter* painter)
      : UiEventArgs(sender, original_sender), painter_(painter) {}
  PaintEventArgs(const PaintEventArgs& other) = default;
  PaintEventArgs(PaintEventArgs&& other) = default;
  PaintEventArgs& operator=(const PaintEventArgs& other) = default;
  PaintEventArgs& operator=(PaintEventArgs&& other) = default;
  ~PaintEventArgs() = default;

  platform::Painter* GetPainter() const { return painter_; }

 private:
  platform::Painter* painter_;
};

class FocusChangeEventArgs : public UiEventArgs {
 public:
  FocusChangeEventArgs(Object* sender, Object* original_sender,
                       const bool is_window = false)
      : UiEventArgs(sender, original_sender), is_window_(is_window) {}
  FocusChangeEventArgs(const FocusChangeEventArgs& other) = default;
  FocusChangeEventArgs(FocusChangeEventArgs&& other) = default;
  FocusChangeEventArgs& operator=(const FocusChangeEventArgs& other) = default;
  FocusChangeEventArgs& operator=(FocusChangeEventArgs&& other) = default;
  ~FocusChangeEventArgs() override = default;

  // Return whether the focus change is caused by the window-wide focus change.
  bool IsWindow() const { return is_window_; }

 private:
  bool is_window_;
};

/*
class ToggleEventArgs : public UiEventArgs {
 public:
  ToggleEventArgs(Object* sender, Object* original_sender, bool new_state)
      : UiEventArgs(sender, original_sender), new_state_(new_state) {}
  ToggleEventArgs(const ToggleEventArgs& other) = default;
  ToggleEventArgs(ToggleEventArgs&& other) = default;
  ToggleEventArgs& operator=(const ToggleEventArgs& other) = default;
  ToggleEventArgs& operator=(ToggleEventArgs&& other) = default;
  ~ToggleEventArgs() override = default;

  bool GetNewState() const { return new_state_; }

 private:
  bool new_state_;
};
*/

class KeyEventArgs : public UiEventArgs {
 public:
  KeyEventArgs(Object* sender, Object* original_sender, int virtual_code)
      : UiEventArgs(sender, original_sender), virtual_code_(virtual_code) {}
  KeyEventArgs(const KeyEventArgs& other) = default;
  KeyEventArgs(KeyEventArgs&& other) = default;
  KeyEventArgs& operator=(const KeyEventArgs& other) = default;
  KeyEventArgs& operator=(KeyEventArgs&& other) = default;
  ~KeyEventArgs() override = default;

  int GetVirtualCode() const { return virtual_code_; }

 private:
  int virtual_code_;
};

/*
class CharEventArgs : public UiEventArgs {
 public:
  CharEventArgs(Object* sender, Object* original_sender, wchar_t c)
      : UiEventArgs(sender, original_sender), c_(c) {}
  CharEventArgs(const CharEventArgs& other) = default;
  CharEventArgs(CharEventArgs&& other) = default;
  CharEventArgs& operator=(const CharEventArgs& other) = default;
  CharEventArgs& operator=(CharEventArgs&& other) = default;
  ~CharEventArgs() override = default;

  wchar_t GetChar() const { return c_; }

 private:
  wchar_t c_;
};
*/
}  // namespace cru::ui::events