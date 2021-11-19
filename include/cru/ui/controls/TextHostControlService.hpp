#pragma once
#include "Base.hpp"

#include "cru/platform/gui/InputMethod.hpp"
#include "cru/platform/gui/TimerHelper.hpp"
#include "cru/platform/gui/UiApplication.hpp"
#include "cru/ui/controls/Control.hpp"
#include "cru/ui/helper/ShortcutHub.hpp"

#include <functional>
#include <vector>

namespace cru::ui::render {
class TextRenderObject;
class ScrollRenderObject;
}  // namespace cru::ui::render

namespace cru::ui::controls {
constexpr int k_default_caret_blink_duration = 500;

struct ITextHostControl : virtual Interface {
  virtual gsl::not_null<render::TextRenderObject*> GetTextRenderObject() = 0;
  // May return nullptr.
  virtual render::ScrollRenderObject* GetScrollRenderObject() = 0;
};

class TextHostControlService;

class TextControlMovePattern : public Object {
 public:
  static TextControlMovePattern kLeft;
  static TextControlMovePattern kRight;
  static TextControlMovePattern kCtrlLeft;
  static TextControlMovePattern kCtrlRight;
  static TextControlMovePattern kUp;
  static TextControlMovePattern kDown;
  static TextControlMovePattern kHome;
  static TextControlMovePattern kEnd;
  static TextControlMovePattern kCtrlHome;
  static TextControlMovePattern kCtrlEnd;
  static TextControlMovePattern kPageUp;
  static TextControlMovePattern kPageDown;

  static std::vector<TextControlMovePattern> kDefaultPatterns;

  using MoveFunction =
      std::function<gsl::index(TextHostControlService* service, StringView text,
                               gsl::index current_position)>;

  TextControlMovePattern(helper::ShortcutKeyBind key_bind,
                         MoveFunction move_function)
      : key_bind_(key_bind), move_function_(move_function) {}

  CRU_DEFAULT_COPY(TextControlMovePattern)
  CRU_DEFAULT_MOVE(TextControlMovePattern)

  ~TextControlMovePattern() override = default;

 public:
  helper::ShortcutKeyBind GetKeyBind() const { return key_bind_; }
  gsl::index Move(TextHostControlService* service, StringView text,
                  gsl::index current_position) const {
    return move_function_(service, text, current_position);
  }

 private:
  helper::ShortcutKeyBind key_bind_;
  MoveFunction move_function_;
};

class TextHostControlService : public Object {
  CRU_DEFINE_CLASS_LOG_TAG(u"cru::ui::controls::TextControlService")

 public:
  TextHostControlService(gsl::not_null<Control*> control);

  CRU_DELETE_COPY(TextHostControlService)
  CRU_DELETE_MOVE(TextHostControlService)

  ~TextHostControlService() = default;

 public:
  bool IsEnabled() { return enable_; }
  void SetEnabled(bool enable);

  bool IsEditable() { return this->editable_; }
  void SetEditable(bool editable);

  bool IsMultiLine() { return this->multi_line_; }
  // If text contains line feed characters, it will be converted to space.
  void SetMultiLine(bool multi_line);

  String GetText() { return this->text_; }
  StringView GetTextView() { return this->text_; }
  void SetText(String text, bool stop_composition = false);

  void InsertText(gsl::index position, StringView text,
                  bool stop_composition = false);
  void DeleteChar(gsl::index position, bool stop_composition = false);

  // Return the position of deleted character.
  gsl::index DeleteCharPrevious(gsl::index position,
                                bool stop_composition = false);
  void DeleteText(TextRange range, bool stop_composition = false);

  void CancelComposition();

  std::optional<platform::gui::CompositionText> GetCompositionInfo();

  bool IsCaretVisible() { return caret_visible_; }
  void SetCaretVisible(bool visible);

  int GetCaretBlinkDuration() { return caret_blink_duration_; }
  void SetCaretBlinkDuration(int milliseconds);

  gsl::index GetCaretPosition() { return selection_.GetEnd(); }
  TextRange GetSelection() { return selection_; }

  StringView GetSelectedText();

  void SetSelection(gsl::index caret_position);
  void SetSelection(TextRange selection, bool scroll_to_caret = true);

  void SelectAll();

  void ChangeSelectionEnd(gsl::index new_end);
  void AbortSelection();

  void DeleteSelectedText();
  // If some text is selected, then they are deleted first. Then insert text
  // into caret position.
  void ReplaceSelectedText(StringView text);

  void ScrollToCaret();

  void Cut();
  void Copy();
  void Paste();

  gsl::not_null<render::TextRenderObject*> GetTextRenderObject();
  render::ScrollRenderObject* GetScrollRenderObject();

 private:
  // May return nullptr.
  platform::gui::IInputMethodContext* GetInputMethodContext();

  void CoerceSelection();

  void SetupCaret();
  void TearDownCaret();

  void SyncTextRenderObject();

  void UpdateInputMethodPosition();

  template <typename TArgs>
  void SetupOneHandler(event::RoutedEvent<TArgs>* (Control::*event)(),
                       void (TextHostControlService::*handler)(
                           typename event::RoutedEvent<TArgs>::EventArgs)) {
    this->event_guard_ += (this->control_->*event)()->Bubble()->AddHandler(
        std::bind(handler, this, std::placeholders::_1));
  }

  void MouseMoveHandler(event::MouseEventArgs& args);
  void MouseDownHandler(event::MouseButtonEventArgs& args);
  void MouseUpHandler(event::MouseButtonEventArgs& args);
  void GainFocusHandler(event::FocusChangeEventArgs& args);
  void LoseFocusHandler(event::FocusChangeEventArgs& args);

  void SetUpShortcuts();

 private:
  gsl::not_null<Control*> control_;
  gsl::not_null<ITextHostControl*> text_host_control_;

  EventRevokerListGuard event_guard_;
  EventRevokerListGuard input_method_context_event_guard_;

  String text_;
  TextRange selection_;

  bool enable_ = false;
  bool editable_ = false;
  bool multi_line_ = false;

  bool caret_visible_ = false;
  platform::gui::TimerAutoCanceler caret_timer_canceler_;
  int caret_blink_duration_ = k_default_caret_blink_duration;

  helper::ShortcutHub shortcut_hub_;

  // true if left mouse is down and selecting
  bool mouse_move_selecting_ = false;
};
}  // namespace cru::ui::controls
