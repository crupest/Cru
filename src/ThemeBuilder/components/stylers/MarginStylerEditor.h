#pragma once
#include "../properties/ThicknessPropertyEditor.h"
#include "StylerEditor.h"
#include "cru/base/ClonablePtr.h"
#include "cru/ui/style/Styler.h"

namespace cru::theme_builder::components::stylers {
class MarginStylerEditor : public StylerEditor {
 public:
  MarginStylerEditor();
  ~MarginStylerEditor() override;

  ClonablePtr<ui::style::MarginStyler> GetValue();
  void SetValue(ui::style::MarginStyler* styler, bool trigger_change = true);
  void SetValue(const ClonablePtr<ui::style::MarginStyler>& styler,
                bool trigger_change = true) {
    SetValue(styler.get(), trigger_change);
  }

  ClonablePtr<ui::style::Styler> GetStyler() override { return GetValue(); }

 private:
  properties::ThicknessPropertyEditor thickness_editor_;
};
}  // namespace cru::theme_builder::components::stylers
