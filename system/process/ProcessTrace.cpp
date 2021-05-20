#include "../../ginc.h"
#include "ProcessTrace.h"

//#include "ProcessAddEventSink.h"
//#include "ProcessDelEventSink.h"

CProcessTrace::CProcessTrace()
{
	if(Init())
	{
		CreateStartTraceInfo();
		CreateEndTraceInfo();
	}
}

CProcessTrace::~CProcessTrace()
{
	_gServices->CancelAsyncCall(m_pAddSinkStub);
	_gServices->CancelAsyncCall(m_pDelSinkStub);

	SafeRelease(&m_pUnsecApp);
	SafeRelease(&m_pAddStubUnk);
	SafeRelease(&m_pAddEventSink);
	SafeRelease(&m_pAddSinkStub);

	SafeRelease(&m_pDelStubUnk);
	SafeRelease(&m_pDelEventSink);
	SafeRelease(&m_pDelSinkStub);

	delete m_pAddEventSink;
	delete m_pDelEventSink;
}

bool CProcessTrace::Init()
{
	m_hResult = CoCreateInstance(CLSID_UnsecuredApartment, NULL, CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment, (void **)&m_pUnsecApp);
	if (FAILED(m_hResult))
	{
		theUtility->ShowMessage(wxString::Format(wxT("CoCreateInstance. CLSID_UnsecuredApartment Error code = %0x"), GetLastError()), wxOK | wxICON_ERROR);
        return false;
	}

	return true;
}

bool CProcessTrace::CreateStartTraceInfo()
{
	m_pAddEventSink = new CProcessAddEventSink();
	m_pAddEventSink->AddRef();

	m_pUnsecApp->CreateObjectStub(m_pAddEventSink, &m_pAddStubUnk);
	if(m_pAddStubUnk == nullptr)
	{
		SafeRelease(&m_pAddEventSink);
		return false;
	}

	m_pAddStubUnk->QueryInterface(IID_IWbemObjectSink, (void **)&m_pAddSinkStub);
	if(m_pAddSinkStub == nullptr)
	{
		theUtility->ShowMessage(wxString::Format(wxT("IID_IWbemObjectSink QueryInterface error %0x"), GetLastError()), wxOK | wxICON_ERROR);
		SafeRelease(&m_pAddEventSink);
		SafeRelease(&m_pAddStubUnk);
		return false;
	}

	wxString strQuery = wxT("SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");
	m_hResult = _gServices->ExecNotificationQueryAsync(_TEXT("WQL"), strQuery.wchar_str(), WBEM_FLAG_SEND_STATUS, nullptr, m_pAddSinkStub);
	if(FAILED(m_hResult))
	{
		theUtility->ShowMessage(wxString::Format(wxT("ExecNotificationQueryAsync error %08x"), GetLastError()), wxOK | wxICON_ERROR);
		SafeRelease(&m_pAddEventSink);
		SafeRelease(&m_pAddStubUnk);
		SafeRelease(&m_pAddSinkStub);
		return false;
	}

	return true;
}

bool CProcessTrace::CreateEndTraceInfo()
{
	m_pDelEventSink = new CProcessDelEventSink();
	m_pDelEventSink->AddRef();

	m_pUnsecApp->CreateObjectStub(m_pDelEventSink, &m_pDelStubUnk);
	if(m_pDelStubUnk == nullptr)
	{
		SafeRelease(&m_pDelEventSink);
		return false;
	}

	m_pDelStubUnk->QueryInterface(IID_IWbemObjectSink, (void **)&m_pDelSinkStub);
	if(m_pDelSinkStub == nullptr)
	{
		theUtility->ShowMessage(wxString::Format(wxT("IID_IWbemObjectSink QueryInterface error %0x"), GetLastError()), wxOK | wxICON_ERROR);
		SafeRelease(&m_pDelEventSink);
		SafeRelease(&m_pDelStubUnk);
		return false;
	}

	wxString strQuery = wxT("SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");
	m_hResult = _gServices->ExecNotificationQueryAsync(_TEXT("WQL"), strQuery.wchar_str(), WBEM_FLAG_SEND_STATUS, nullptr, m_pDelSinkStub);
	if(FAILED(m_hResult))
	{
		theUtility->ShowMessage(wxString::Format(wxT("ExecNotificationQueryAsync error %08x"), GetLastError()), wxOK | wxICON_ERROR);
		SafeRelease(&m_pDelEventSink);
		SafeRelease(&m_pDelStubUnk);
		SafeRelease(&m_pDelSinkStub);
		return false;
	}

	return true;
}
