#pragma once
#include "../win_pre_config.hpp"

#include "cru/platform/native/native_window.hpp"
#include "window_native_message_event_args.hpp"

#include <memory>

namespace cru::win::native {
class WinApplication;
class WindowClass;
class WindowManager;
class WindowRenderTarget;

class WinNativeWindow : public Object,
                        public virtual platform::native::INativeWindow {
 public:
  WinNativeWindow(WinApplication* application,
                  std::shared_ptr<WindowClass> window_class, DWORD window_style,
                  WinNativeWindow* parent);
  WinNativeWindow(const WinNativeWindow& other) = delete;
  WinNativeWindow(WinNativeWindow&& other) = delete;
  WinNativeWindow& operator=(const WinNativeWindow& other) = delete;
  WinNativeWindow& operator=(WinNativeWindow&& other) = delete;
  ~WinNativeWindow() override;

  bool IsValid() override;
  void SetDeleteThisOnDestroy(bool value) override;

  void Close() override;

  INativeWindow* GetParent() override { return parent_window_; }

  bool IsVisible() override;
  void SetVisible(bool is_visible) override;

  ui::Size GetClientSize() override;
  void SetClientSize(const ui::Size& size) override;

  // Get the rect of the window containing frame.
  // The lefttop of the rect is relative to screen lefttop.
  ui::Rect GetWindowRect() override;

  // Set the rect of the window containing frame.
  // The lefttop of the rect is relative to screen lefttop.
  void SetWindowRect(const ui::Rect& rect) override;

  platform::graph::IPainter* BeginPaint() override;

  IEvent<std::nullptr_t>* DestroyEvent() override { return &destroy_event_; }
  IEvent<std::nullptr_t>* PaintEvent() override { return &paint_event_; }
  IEvent<ui::Size>* ResizeEvent() override { return &resize_event_; }
  IEvent<bool>* FocusEvent() override { return &focus_event_; }
  IEvent<bool>* MouseEnterLeaveEvent() override {
    return &mouse_enter_leave_event_;
  }
  IEvent<ui::Point>* MouseMoveEvent() override { return &mouse_move_event_; }
  IEvent<platform::native::NativeMouseButtonEventArgs>* MouseDownEvent()
      override {
    return &mouse_down_event_;
  }
  IEvent<platform::native::NativeMouseButtonEventArgs>* MouseUpEvent()
      override {
    return &mouse_up_event_;
  }
  IEvent<int>* KeyDownEvent() override { return &key_down_event_; }
  IEvent<int>* KeyUpEvent() override { return &key_up_event_; }

  IEvent<WindowNativeMessageEventArgs&>* NativeMessageEvent() {
    return &native_message_event_;
  }

  // Get the handle of the window. Return null if window is invalid.
  HWND GetWindowHandle() const { return hwnd_; }

  bool HandleNativeWindowMessage(HWND hwnd, UINT msg, WPARAM w_param,
                                 LPARAM l_param, LRESULT* result);

  WindowRenderTarget* GetWindowRenderTarget() const {
    return window_render_target_.get();
  }

 private:
  // Get the client rect in pixel.
  RECT GetClientRectPixel();

  //*************** region: native messages ***************

  void OnDestroyInternal();
  void OnPaintInternal();
  void OnResizeInternal(int new_width, int new_height);

  void OnSetFocusInternal();
  void OnKillFocusInternal();

  void OnMouseMoveInternal(POINT point);
  void OnMouseLeaveInternal();
  void OnMouseDownInternal(platform::native::MouseButton button, POINT point);
  void OnMouseUpInternal(platform::native::MouseButton button, POINT point);

  void OnMouseWheelInternal(short delta, POINT point);
  void OnKeyDownInternal(int virtual_code);
  void OnKeyUpInternal(int virtual_code);
  void OnCharInternal(wchar_t c);

  void OnActivatedInternal();
  void OnDeactivatedInternal();

 private:
  WinApplication* application_;

  bool delete_this_on_destroy_ = true;

  HWND hwnd_;
  WinNativeWindow* parent_window_;

  bool has_focus_ = false;
  bool is_mouse_in_ = false;

  std::shared_ptr<WindowRenderTarget> window_render_target_;

  Event<std::nullptr_t> destroy_event_;
  Event<std::nullptr_t> paint_event_;
  Event<ui::Size> resize_event_;
  Event<bool> focus_event_;
  Event<bool> mouse_enter_leave_event_;
  Event<ui::Point> mouse_move_event_;
  Event<platform::native::NativeMouseButtonEventArgs> mouse_down_event_;
  Event<platform::native::NativeMouseButtonEventArgs> mouse_up_event_;
  Event<int> key_down_event_;
  Event<int> key_up_event_;

  Event<WindowNativeMessageEventArgs&> native_message_event_;
};
}  // namespace cru::win::native