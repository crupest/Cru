#pragma once
#include "../Editor.h"
#include "../properties/ColorPropertyEditor.h"
#include "StylerEditor.h"
#include "cru/base/ClonablePtr.h"
#include "cru/ui/style/Styler.h"

namespace cru::theme_builder::components::stylers {
class ContentBrushStylerEditor : public StylerEditor {
 public:
  ContentBrushStylerEditor();
  ~ContentBrushStylerEditor();

 public:
  ClonablePtr<ui::style::ContentBrushStyler> GetValue() const;
  void SetValue(ui::style::ContentBrushStyler* value,
                bool trigger_change = true);

  ClonablePtr<ui::style::Styler> GetStyler() override { return GetValue(); }

 private:
  properties::ColorPropertyEditor color_editor_;
};
}  // namespace cru::theme_builder::components::stylers
