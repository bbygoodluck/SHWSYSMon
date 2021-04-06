#include "ginc.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(CSHWSYSMon);

wxBEGIN_EVENT_TABLE(CSHWSYSMon, wxApp)
wxEND_EVENT_TABLE()

CSHWSYSMon::CSHWSYSMon()
#ifdef NDEBUG
	: m_pSingleInstance(nullptr)
#endif
{

}

CSHWSYSMon::~CSHWSYSMon()
{
#ifdef __WXMSW__
	SafeRelease(&_gServices);
    SafeRelease(&_gLocator);

    CoUninitialize();

	DeleteCriticalSection(&_gCriProcess);
	DeleteCriticalSection(&_gCriEventSink);
#endif

}

bool CSHWSYSMon::OnInit()
{
	if (!wxApp::OnInit())
	{
		wxMessageBox(wxT("윈도우 초기화 실패 : wxApp::OnInit()"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	theUtility->LoadImageList();
#ifdef NDEBUG
	m_pSingleInstance = new wxSingleInstanceChecker;
	if (m_pSingleInstance->IsAnotherRunning())
	{
		delete m_pSingleInstance;
		m_pSingleInstance = NULL;

		theUtility->ShowMessage(wxT("이미 실행중입니다."), wxOK | wxICON_INFORMATION);
		return false;
	}
#endif

	//환경파일 Load
	if(!OnLoadSettings())
		return false;

	//메뉴읽기
	wxXmlResource::Get()->InitAllHandlers();
	wxString strMenuFile = theUtility->GetWorkingDirectory() + SLASH + wxT("xrc") + SLASH + wxT("menu.xrc");
	bool bXRCLoaded = wxXmlResource::Get()->Load(strMenuFile);

	if (!bXRCLoaded)
	{
		wxMessageBox(wxT("메뉴 파일 읽기가 실패하였습니다."), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	wxString strPGTitle = PROGRAM_FULL_NAME + ENV_OS_VER;

#ifndef NDEBUG
	strPGTitle += wxT(" - Debug Version");
#endif

#ifdef __WXMSW__
	CreateCOMInstance();
	InitializeCriticalSection(&_gCriProcess);
	InitializeCriticalSection(&_gCriEventSink);
#endif // __WXMSW__

	CMainFrame* pMainFrame = new CMainFrame(strPGTitle);
	pMainFrame->Show();

	this->SetTopWindow(pMainFrame);
	return true;
}

int CSHWSYSMon::OnExit()
{
	return 0;
}

bool CSHWSYSMon::OnLoadSettings()
{
	if(!theSettings->Load())
		return false;

	return true;
}

#ifdef __WXMSW__
void CSHWSYSMon::CreateCOMInstance()
{
	//WMI이용을 위한 COM객체 생성
	HRESULT hResult = CoInitialize(NULL);//Ex(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hResult))
    {
		theUtility->ShowMessage(wxString::Format(wxT("Failed to initialize COM library. Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);
		return;
    }

    hResult =  CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
        );

	if (FAILED(hResult))
		theUtility->ShowMessage(wxString::Format(wxT("Failed to initialize security. Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);

	hResult = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *) &_gLocator);

    if (FAILED(hResult))
    {
    	theUtility->ShowMessage(wxString::Format(wxT("Failed to create IWbemLocator object.Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);
        CoUninitialize();
        return;
    }

    hResult = _gLocator->ConnectServer(
         _T("ROOT\\CIMV2"), // Object path of WMI namespace
         NULL,              // User name. NULL = current user
         NULL,              // User password. NULL = current
         NULL,              // Locale. NULL indicates current
         0,                 // Security flags.
         NULL,              // Authority (for example, Kerberos)
         NULL,              // Context object
         &_gServices        // pointer to IWbemServices proxy
         );

    if (FAILED(hResult))
    {
    	theUtility->ShowMessage(wxString::Format(wxT("Could not connect. Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);
    	SafeRelease(&_gLocator);
        CoUninitialize();
        return;                // Program has failed.
    }

    hResult = CoSetProxyBlanket(
       _gServices,                  // Indicates the proxy to set
       RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
       NULL,                        // Server principal name
       RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
       RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
       NULL,                        // client identity
       EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(hResult))
    {
    	theUtility->ShowMessage(wxString::Format(wxT("Could not set proxy blanket. Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);
    	SafeRelease(&_gLocator);
    	SafeRelease(&_gServices);
        CoUninitialize();
        return;
    }
}
#endif // __WXMSW__
