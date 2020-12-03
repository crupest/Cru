#include "cru/ui/UiManager.hpp"

#include "Helper.hpp"
#include "cru/platform/graphics/Brush.hpp"
#include "cru/platform/graphics/Factory.hpp"
#include "cru/platform/graphics/Font.hpp"
#include "cru/platform/gui/UiApplication.hpp"
#include "cru/ui/style/ApplyBorderStyleInfo.hpp"
#include "cru/ui/style/Condition.hpp"
#include "cru/ui/style/Styler.hpp"

namespace cru::ui {
using namespace cru::platform::graphics;
using namespace cru::ui::style;
using namespace cru::ui::helper;

namespace {
std::unique_ptr<ISolidColorBrush> CreateSolidColorBrush(IGraphFactory* factory,
                                                        const Color& color) {
  auto brush = factory->CreateSolidColorBrush();
  brush->SetColor(color);
  return brush;
}
}  // namespace

UiManager* UiManager::GetInstance() {
  static UiManager* instance = new UiManager();
  GetUiApplication()->AddOnQuitHandler([] {
    delete instance;
    instance = nullptr;
  });
  return instance;
}

UiManager::UiManager() {
  const auto factory = GetGraphFactory();

  theme_resource_.default_font_family = u"等线";

  theme_resource_.default_font =
      factory->CreateFont(theme_resource_.default_font_family, 24.0f);

  const auto black_brush =
      std::shared_ptr<platform::graphics::ISolidColorBrush>(
          CreateSolidColorBrush(factory, colors::black));
  theme_resource_.text_brush = black_brush;
  theme_resource_.text_selection_brush =
      CreateSolidColorBrush(factory, colors::skyblue);
  theme_resource_.caret_brush = black_brush;

  theme_resource_.button_style.AddStyleRule(
      {ClickStateCondition::Create(ClickState::None),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0x00bfff)),
           Thickness(3), CornerRadius(5), nullptr, nullptr}),
       u"DefaultButtonNormal"});
  theme_resource_.button_style.AddStyleRule(
      {ClickStateCondition::Create(ClickState::Hover),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0x47d1ff)),
           Thickness(3), CornerRadius(5), nullptr, nullptr}),
       u"DefaultButtonHover"});
  theme_resource_.button_style.AddStyleRule(
      {ClickStateCondition::Create(ClickState::Press),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0x91e4ff)),
           Thickness(3), CornerRadius(5), nullptr, nullptr}),
       u"DefaultButtonPress"});
  theme_resource_.button_style.AddStyleRule(
      {ClickStateCondition::Create(ClickState::PressInactive),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0x91e4ff)),
           Thickness(3), CornerRadius(5), nullptr, nullptr}),
       u"DefaultButtonPressInactive"});

  theme_resource_.text_box_style.AddStyleRule(
      {HoverCondition::Create(false),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0xced4da)),
           Thickness(1), CornerRadius(5), nullptr, nullptr}),
       u"DefaultTextBoxNormal"});

  theme_resource_.text_box_style.AddStyleRule(
      {HoverCondition::Create(true),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0xced4da)),
           Thickness(1), CornerRadius(5), nullptr, nullptr}),
       u"DefaultTextBoxHover"});

  theme_resource_.text_box_style.AddStyleRule(
      {FocusCondition::Create(true),
       BorderStyler::Create(ApplyBorderStyleInfo{
           CreateSolidColorBrush(factory, Color::FromHex(0x495057)),
           Thickness(1), CornerRadius(5), nullptr, nullptr}),
       u"DefaultTextBoxHover"});
}

UiManager::~UiManager() = default;
}  // namespace cru::ui
