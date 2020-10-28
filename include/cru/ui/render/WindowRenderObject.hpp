#pragma once
#include "RenderObject.hpp"

#include <string_view>

namespace cru::ui::render {
class WindowRenderObject : public RenderObject {
 public:
  WindowRenderObject(WindowHost* host);
  WindowRenderObject(const WindowRenderObject& other) = delete;
  WindowRenderObject(WindowRenderObject&& other) = delete;
  WindowRenderObject& operator=(const WindowRenderObject& other) = delete;
  WindowRenderObject& operator=(WindowRenderObject&& other) = delete;
  ~WindowRenderObject() override = default;

  RenderObject* HitTest(const Point& point) override;

 public:
  std::u16string_view GetName() const override;

 protected:
  Size OnMeasureContent(const MeasureRequirement& requirement,
                        const MeasureSize& preferred_size) override;
  void OnLayoutContent(const Rect& content_rect) override;

 private:
  RenderObject* GetChild() const {
    return GetChildren().empty() ? nullptr : GetChildren()[0];
  }

 private:
  EventRevokerGuard after_layout_event_guard_;
};
}  // namespace cru::ui::render
