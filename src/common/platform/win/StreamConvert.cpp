#include "cru/common/platform/win/StreamConvert.h"
#include "BrigdeComStream.h"
#include "Win32FileStreamPrivate.h"
#include "cru/common/Exception.h"
#include "cru/common/io/MemoryStream.h"
#include "cru/common/io/OpenFileFlag.h"
#include "cru/common/platform/win/ComAutoInit.h"
#include "cru/common/platform/win/Exception.h"
#include "cru/common/platform/win/Win32FileStream.h"

#include <shlwapi.h>
#include <winnt.h>

namespace cru::platform::win {
IStream* ConvertStreamToComStream(io::Stream* stream) {
  static ComAutoInit com_auto_init;

  if (auto memory_stream = dynamic_cast<io::MemoryStream*>(stream)) {
    return SHCreateMemStream(
        reinterpret_cast<const BYTE*>(memory_stream->GetBuffer()),
        memory_stream->GetSize());
  } else if (auto file_stream = dynamic_cast<Win32FileStream*>(stream)) {
    return file_stream->GetPrivate_()->stream_;
  } else {
    return new BridgeComStream(stream);
  }
}
}  // namespace cru::platform::win
