#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

/*
	global include file
*/
//included wxWidgets
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/snglinst.h>
#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/sizer.h>
#include <wx/display.h>
#include <wx/listctrl.h>
#include <wx/busyinfo.h>
#include <wx/xrc/xmlres.h>

//included std
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <streambuf>

#ifdef __WXMSW__
	#include <wx/msw/private.h>
	#include <windows.h>
	#include <Pdh.h>
	#include <PdhMsg.h>
	#include <Iphlpapi.h>
	#include <Wbemidl.h>
	#include <wbemcli.h>
	#include <CommCtrl.h>
	#include <Psapi.h>
//	#include <TlHelp32.h>
//	#include <winver.h>
//	#include <shellapi.h>
//	#include <shlobj.h>
//	#include <Propkey.h>
//	#include <wtsapi32.h>
//	#include <aclapi.h>
#endif

#ifdef __WXMSW__
template <typename T>
void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}
#endif // __WXMSW__
#include "defs.h"
#include "EventDefs.h"
#include "EnumDef.h"
#include "Version.h"
#include "gvalue.h"
#include "Utility.h"
#include "Settings.h"

#include "../system/SystemCommon.h"

#define theUtility	 (CUtility::Get())
#define theSettings  (CSettings::Get())
#define theSystem    (CSystemCommon::Get())

#endif // COMMON_H_INCLUDED
