#include "base.h"

#include "system_headers.h"
#include "exception.h"

namespace cru
{
    MultiByteString ToUtf8String(const StringView& string)
    {
        if (string.empty())
            return MultiByteString();

        const auto length = ::WideCharToMultiByte(CP_UTF8, 0, string.data(), -1, nullptr, 0, nullptr, nullptr);
        MultiByteString result;
        result.reserve(length);
        if (::WideCharToMultiByte(CP_UTF8, 0, string.data(), -1, result.data(), static_cast<int>(result.capacity()), nullptr, nullptr) == 0)
            throw Win32Error(::GetLastError(), "Failed to convert wide string to UTF-8.");
        return result;
    }
}