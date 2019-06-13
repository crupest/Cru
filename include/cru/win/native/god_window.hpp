#pragma once
#include "../win_pre_config.hpp"

#include "cru/common/base.hpp"

#include <memory>

namespace cru::win::native {
class WinApplication;
class WindowClass;

class GodWindow : public Object {
 public:
  explicit GodWindow(WinApplication* application);
  GodWindow(const GodWindow& other) = delete;
  GodWindow(GodWindow&& other) = delete;
  GodWindow& operator=(const GodWindow& other) = delete;
  GodWindow& operator=(GodWindow&& other) = delete;
  ~GodWindow() override;

  HWND GetHandle() const { return hwnd_; }

  bool HandleGodWindowMessage(HWND hwnd, UINT msg, WPARAM w_param,
                              LPARAM l_param, LRESULT* result);

 private:
  WinApplication* application_;

  std::shared_ptr<WindowClass> god_window_class_;
  HWND hwnd_;
};
}  // namespace cru::win::native