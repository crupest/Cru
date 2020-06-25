#include "cru/ui/render/RenderObject.hpp"

#include "cru/common/Logger.hpp"
#include "cru/ui/UiHost.hpp"

#include <algorithm>

namespace cru::ui::render {
void RenderObject::AddChild(RenderObject* render_object, const Index position) {
  Expects(child_mode_ != ChildMode::None);
  Expects(!(child_mode_ == ChildMode::Single && children_.size() > 0));

  Expects(render_object->GetParent() ==
          nullptr);        // Render object already has a parent.
  Expects(position >= 0);  // Position index is less than 0.
  Expects(
      position <=
      static_cast<Index>(children_.size()));  // Position index is out of bound.

  children_.insert(children_.cbegin() + position, render_object);
  render_object->SetParent(this);
  render_object->SetRenderHostRecursive(GetUiHost());
  OnAddChild(render_object, position);
}

void RenderObject::RemoveChild(const Index position) {
  Expects(position >= 0);  // Position index is less than 0.
  Expects(position < static_cast<Index>(
                         children_.size()));  // Position index is out of bound.

  const auto i = children_.cbegin() + position;
  const auto removed_child = *i;
  children_.erase(i);
  removed_child->SetParent(nullptr);
  removed_child->SetRenderHostRecursive(nullptr);
  OnRemoveChild(removed_child, position);
}

Point RenderObject::GetTotalOffset() const {
  Point result{};
  const RenderObject* render_object = this;

  while (render_object != nullptr) {
    const auto o = render_object->GetOffset();
    result.x += o.x;
    result.y += o.y;
    render_object = render_object->GetParent();
  }

  return result;
}

Point RenderObject::FromRootToContent(const Point& point) const {
  const auto offset = GetTotalOffset();
  const auto rect = GetContentRect();
  return Point{point.x - (offset.x + rect.left),
               point.y - (offset.y + rect.top)};
}

void RenderObject::Measure(const MeasureRequirement& requirement,
                           const MeasureSize& preferred_size) {
  MeasureRequirement merged_requirement =
      MeasureRequirement::Merge(requirement, custom_measure_requirement_)
          .Normalize();
  MeasureSize merged_preferred_size =
      preferred_size.OverrideBy(preferred_size_);

  size_ = OnMeasureCore(merged_requirement, merged_preferred_size);
  Ensures(size_.width >= 0);
  Ensures(size_.height >= 0);
  Ensures(requirement.Satisfy(size_));
}

void RenderObject::Layout(const Point& offset) {
  offset_ = offset;
  OnLayoutCore();
}

RenderObject* RenderObject::GetSingleChild() const {
  Expects(child_mode_ == ChildMode::Single);
  const auto& children = GetChildren();
  if (children.empty())
    return nullptr;
  else
    return children.front();
}

void RenderObject::OnParentChanged(RenderObject* old_parent,
                                   RenderObject* new_parent) {
  CRU_UNUSED(old_parent)
  CRU_UNUSED(new_parent)
}

void RenderObject::OnAddChild(RenderObject* new_child, Index position) {
  CRU_UNUSED(new_child)
  CRU_UNUSED(position)

  InvalidateLayout();
  InvalidatePaint();
}

void RenderObject::OnRemoveChild(RenderObject* removed_child, Index position) {
  CRU_UNUSED(removed_child)
  CRU_UNUSED(position)

  InvalidateLayout();
  InvalidatePaint();
}

Size RenderObject::OnMeasureCore(const MeasureRequirement& requirement,
                                 const MeasureSize& preferred_size) {
  const Size space_size{
      margin_.GetHorizontalTotal() + padding_.GetHorizontalTotal(),
      margin_.GetVerticalTotal() + padding_.GetVerticalTotal()};

  auto coerced_space_size = space_size;

  MeasureRequirement content_requirement = requirement;

  if (!requirement.max.width.IsNotSpecified()) {
    const auto max_width = requirement.max.width.GetLengthOrMax();
    if (coerced_space_size.width > max_width) {
      log::Warn(
          "RenderObject: During measure, horizontal length of padding and "
          "margin is bigger than required max length.");
      coerced_space_size.width = max_width;
    }
    content_requirement.max.width = max_width - coerced_space_size.width;
  }

  if (!requirement.min.width.IsNotSpecified()) {
    const auto min_width = requirement.min.width.GetLengthOr0();
    content_requirement.min.width = std::max(0.f, min_width - space_size.width);
  }

  if (!requirement.max.height.IsNotSpecified()) {
    const auto max_height = requirement.max.height.GetLengthOrMax();
    if (coerced_space_size.height > max_height) {
      log::Warn(
          "RenderObject: During measure, vertical length of padding and "
          "margin is bigger than required max length.");
      coerced_space_size.height = max_height;
    }
    content_requirement.max.height = max_height - coerced_space_size.height;
  }

  if (!requirement.min.height.IsNotSpecified()) {
    const auto min_height = requirement.min.height.GetLengthOr0();
    content_requirement.min.height =
        std::max(0.f, min_height - space_size.height);
  }

  MeasureSize content_preferred_size =
      content_requirement.Coerce(preferred_size.Minus(space_size));

  const auto content_size =
      OnMeasureContent(content_requirement, content_preferred_size);

  return coerced_space_size + content_size;
}

void RenderObject::OnLayoutCore() {
  Size total_size = GetSize();
  Size space_size{margin_.GetHorizontalTotal() + padding_.GetHorizontalTotal(),
                  margin_.GetVerticalTotal() + padding_.GetVerticalTotal()};

  auto content_size = total_size - space_size;

  if (content_size.width < 0) {
    log::Warn(
        "RenderObject: During layout, horizontal length of padding and margin "
        "is bigger than available length.");
    content_size.width = 0;
  }
  if (content_size.height < 0) {
    log::Warn(
        "RenderObject: During layout, vertical length of padding and margin "
        "is bigger than available length.");
    content_size.height = 0;
  }

  Point lefttop{margin_.left + padding_.left, margin_.top + padding_.top};
  if (lefttop.x > total_size.width) {
    lefttop.x = total_size.width;
  }
  if (lefttop.y > total_size.height) {
    lefttop.y = total_size.height;
  }

  const Rect content_rect{lefttop, content_size};

  OnLayoutContent(content_rect);
}

void RenderObject::OnAfterLayout() {}

Rect RenderObject::GetPaddingRect() const {
  const auto size = GetSize();
  Rect rect{Point{}, size};
  rect = rect.Shrink(GetMargin());
  rect.left = std::min(rect.left, size.width);
  rect.top = std::min(rect.top, size.height);
  rect.width = std::max(rect.width, 0.0f);
  rect.height = std::max(rect.height, 0.0f);
  return rect;
}

Rect RenderObject::GetContentRect() const {
  const auto size = GetSize();
  Rect rect{Point{}, size};
  rect = rect.Shrink(GetMargin());
  rect = rect.Shrink(GetPadding());
  rect.left = std::min(rect.left, size.width);
  rect.top = std::min(rect.top, size.height);
  rect.width = std::max(rect.width, 0.0f);
  rect.height = std::max(rect.height, 0.0f);
  return rect;
}

void RenderObject::SetParent(RenderObject* new_parent) {
  const auto old_parent = parent_;
  parent_ = new_parent;
  OnParentChanged(old_parent, new_parent);
}

void RenderObject::InvalidateLayout() {
  if (ui_host_ != nullptr) ui_host_->InvalidateLayout();
}

void RenderObject::InvalidatePaint() {
  if (ui_host_ != nullptr) ui_host_->InvalidatePaint();
}

void RenderObject::NotifyAfterLayoutRecursive(RenderObject* render_object) {
  render_object->OnAfterLayout();
  for (const auto o : render_object->GetChildren()) {
    NotifyAfterLayoutRecursive(o);
  }
}

void RenderObject::SetRenderHostRecursive(UiHost* host) {
  ui_host_ = host;
  for (const auto child : GetChildren()) {
    child->SetRenderHostRecursive(host);
  }
}
}  // namespace cru::ui::render
