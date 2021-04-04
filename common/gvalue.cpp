#include "common.h"
#include "gvalue.h"

// 이미지리스트
SYSTEM_IMAGE_LIST _gImageList;
#ifdef __WXMSW__
CRITICAL_SECTION _gCriProcess;
CRITICAL_SECTION _gCriEventSink;
IWbemLocator* _gLocator = nullptr;
IWbemServices* _gServices = nullptr;
#endif // __WXMSW__
