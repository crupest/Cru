#pragma once
#include "render_object.hpp"

#include <memory>

namespace cru::platform {
struct Brush;
struct Geometry;
}  // namespace cru::platform

namespace cru::ui::render {
struct CornerRadius {
  constexpr CornerRadius()
      : left_top(), right_top(), left_bottom(), right_bottom() {}
  constexpr CornerRadius(const Point& value)
      : left_top(value),
        right_top(value),
        left_bottom(value),
        right_bottom(value) {}
  constexpr CornerRadius(Point left_top, Point right_top, Point left_bottom,
                         Point right_bottom)
      : left_top(left_top),
        right_top(right_top),
        left_bottom(left_bottom),
        right_bottom(right_bottom) {}

  Point left_top;
  Point right_top;
  Point left_bottom;
  Point right_bottom;
};

class BorderRenderObject : public RenderObject {
 public:
  explicit BorderRenderObject(std::shared_ptr<platform::Brush> brush);
  BorderRenderObject(const BorderRenderObject& other) = delete;
  BorderRenderObject(BorderRenderObject&& other) = delete;
  BorderRenderObject& operator=(const BorderRenderObject& other) = delete;
  BorderRenderObject& operator=(BorderRenderObject&& other) = delete;
  ~BorderRenderObject() override = default;

  bool IsEnabled() const { return is_enabled_; }
  void SetEnabled(bool enabled) { is_enabled_ = enabled; }

  std::shared_ptr<platform::Brush> GetBrush() const { return border_brush_; }
  void SetBrush(std::shared_ptr<platform::Brush> new_brush) {
    border_brush_ = std::move(new_brush);
  }

  Thickness GetBorderWidth() const { return border_thickness_; }
  void SetBorderWidth(const Thickness& thickness) {
    border_thickness_ = thickness;
  }

  CornerRadius GetCornerRadius() const { return corner_radius_; }
  void SetCornerRadius(const CornerRadius& new_corner_radius) {
    corner_radius_ = new_corner_radius;
  }

  void Refresh() { RecreateGeometry(); }

  void Draw(platform::Painter* painter) override;

  RenderObject* HitTest(const Point& point) override;

 protected:
  void OnAddChild(RenderObject* new_child, int position) override;

  void OnSizeChanged(const Size& old_size, const Size& new_size) override;

  void OnMeasureCore(const Size& available_size) override;
  void OnLayoutCore(const Rect& rect) override;
  Size OnMeasureContent(const Size& available_size) override;
  void OnLayoutContent(const Rect& content_rect) override;

 private:
  RenderObject* GetChild() const {
    return GetChildren().empty() ? nullptr : GetChildren()[0];
  }

  void RecreateGeometry();

 private:
  bool is_enabled_ = false;

  std::shared_ptr<platform::Brush> border_brush_ = nullptr;
  Thickness border_thickness_{};
  CornerRadius corner_radius_{};

  std::shared_ptr<platform::Geometry> geometry_ = nullptr;
  std::shared_ptr<platform::Geometry> border_outer_geometry_ = nullptr;
};
}  // namespace cru::ui::render