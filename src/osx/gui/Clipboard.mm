#include "cru/osx/gui/Clipboard.hpp"
#include "ClipboardPrivate.h"

#include "cru/common/Logger.hpp"
#include "cru/osx/Convert.hpp"

#include <memory>

namespace cru::platform::gui::osx {
using cru::platform::osx::Convert;

OsxClipboard::OsxClipboard(cru::platform::gui::IUiApplication* ui_application,
                           std::unique_ptr<details::OsxClipboardPrivate> p)
    : OsxGuiResource(ui_application), p_(std::move(p)) {}

OsxClipboard::~OsxClipboard() {}

String OsxClipboard::GetText() { return p_->GetText(); }

void OsxClipboard::SetText(String text) { p_->SetText(text); }

namespace details {
OsxClipboardPrivate::OsxClipboardPrivate(NSPasteboard* pasteboard) : pasteboard_(pasteboard) {}

OsxClipboardPrivate::~OsxClipboardPrivate() {}

String OsxClipboardPrivate::GetText() {
  auto result = [pasteboard_ readObjectsForClasses:@[ NSString.class ] options:nil];
  if (result == nil) {
    log::TagWarn(log_tag, u"Failed to get text from clipboard");
    return u"";
  } else {
    if (result.count == 0) {
      return u"";
    } else {
      return Convert((CFStringRef)result[0]);
    }
  }
}

void OsxClipboardPrivate::SetText(String text) {
  [pasteboard_ clearContents];
  [pasteboard_ writeObjects:@[ (NSString*)Convert(text) ]];
}
}
}  // namespace cru::platform::gui::osx
