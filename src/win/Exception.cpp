#include "cru/win/Exception.hpp"

#include <optional>

namespace cru::platform::win {

inline String HResultMakeMessage(HRESULT h_result,
                                 std::optional<String> message) {
  if (message.has_value())
    return Format(L"HRESULT: {}. Message: {}", h_result, message->WinCStr());
  else
    return Format(L"HRESULT: {}.", h_result);
}

HResultError::HResultError(HRESULT h_result)
    : PlatformException(HResultMakeMessage(h_result, std::nullopt)),
      h_result_(h_result) {}

HResultError::HResultError(HRESULT h_result,
                           std::string_view additional_message)
    : PlatformException(HResultMakeMessage(
          h_result, String::FromUtf8(additional_message.data(),
                                     additional_message.size()))),
      h_result_(h_result) {}

inline String Win32MakeMessage(DWORD error_code, String message) {
  return Format(L"Last error code: {}.\nMessage: {}\n", error_code,
                message.WinCStr());
}

Win32Error::Win32Error(std::string_view message)
    : Win32Error(::GetLastError(), message) {}

Win32Error::Win32Error(DWORD error_code, std::string_view message)
    : PlatformException(Win32MakeMessage(
          error_code, String::FromUtf8(message.data(), message.size()))),
      error_code_(error_code) {}
}  // namespace cru::platform::win
