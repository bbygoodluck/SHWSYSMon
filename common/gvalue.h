#ifndef __GLOBAL_VALUES_H__
#define __GLOBAL_VALUES_H__

#include "common.h"
#include "gvalue.h"

#ifdef __WXMSW__
	typedef HIMAGELIST SYSTEM_IMAGE_LIST;
	extern CRITICAL_SECTION _gCriProcess;
	extern CRITICAL_SECTION _gCriEventSink;
	extern IWbemLocator* _gLocator;
	extern IWbemServices* _gServices;
#else
	typedef wxImageList* SYSTEM_IMAGE_LIST;
#endif

// 이미지리스트
extern SYSTEM_IMAGE_LIST _gImageList;

#endif
