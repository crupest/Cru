#include "MainWindow.hpp"
#include "cru/ui/Base.hpp"
#include "cru/ui/controls/StackLayout.hpp"
#include "cru/ui/controls/TextBlock.hpp"

namespace cru::theme_builder {
using namespace cru::ui;
using namespace cru::ui::controls;
using namespace cru::platform::gui;

MainWindow::MainWindow() {
  window_ = Window::Create();
  window_->GetNativeWindow()->SetTitle(u"CruUI Theme Builder");

  main_layout_ = FlexLayout::Create();
  main_layout_->SetFlexDirection(FlexDirection::Horizontal);
  window_->AddChild(main_layout_, 0);

  preview_layout_ = StackLayout::Create();
  main_layout_->AddChild(preview_layout_, 0);

  preview_button_ = Button::Create();
  preview_button_->SetChild(TextBlock::Create(u"Preview"));
  preview_layout_->AddChild(preview_button_, 0);
  preview_layout_->SetChildLayoutData(
      0, StackChildLayoutData{Alignment::Center, Alignment::Center});
}

MainWindow::~MainWindow() { delete window_; }

void MainWindow::Show() {
  window_->GetNativeWindow()->SetVisibility(WindowVisibilityType::Show);
  window_->GetNativeWindow()->SetToForeground();
}
}  // namespace cru::theme_builder