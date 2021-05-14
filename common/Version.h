#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

/********************************************************************************
	class Name :
	설      명 : 버전정보 표시
*********************************************************************************/
const wxString PROGRAM_NAME			= wxT("SHW-SYSMon");
const wxString PROGRAM_MAJOR_VER	= wxT("0");
const wxString PROGRAM_MINOR_HI_VER = wxT("2");
const wxString PROGRAM_MINOR_LO_VER = wxT("2");
const wxString PROGRAM_VERSION		= wxT(" Ver ") + PROGRAM_MAJOR_VER + wxT(".") + PROGRAM_MINOR_HI_VER + wxT(".") + PROGRAM_MINOR_LO_VER;
const wxString PROGRAM_FULL_NAME	= PROGRAM_NAME + PROGRAM_VERSION;

#ifdef _X64
	const wxString ENV_OS_VER = wxT(" - x64");
#else
	const wxString ENV_OS_VER = wxT(" - x86");
#endif

#endif // VERSION_H_INCLUDED
