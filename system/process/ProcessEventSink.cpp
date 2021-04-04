#include "../../ginc.h"

#ifdef __WXMSW__
#include "ProcessEventSink.h"
#include "ProcessMonitoring.h"

CEventSink::CEventSink()
{
	m_lRef = 0;
}

CEventSink::~CEventSink()
{
	m_bDone = true;
}

ULONG CEventSink::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

ULONG CEventSink::Release()
{
	ULONG lRef = InterlockedDecrement(&m_lRef);
	if(lRef == 0)
		return 0;

	return lRef;
}

HRESULT CEventSink::QueryInterface(REFIID riid, void** ppv)
{
	if(riid == IID_IUnknown || riid == IID_IWbemObjectSink)
	{
		*ppv = (IWbemObjectSink *)this;
		AddRef();

		return WBEM_S_NO_ERROR;
	}

	return E_NOINTERFACE;
}

HRESULT CEventSink::Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray)
{
	EnterCriticalSection(&_gCriEventSink);
	HRESULT hr = S_OK;
	for(int i = 0; i < lObjectCount; i++)
	{
		VARIANT vtProp;
		VARIANT cn;

		hr = apObjArray[i]->Get(L"TargetInstance", 0, &vtProp, 0, 0);
		if(FAILED(hr))
			continue;

		IUnknown* pUnk = vtProp.punkVal;

		HRESULT hr = pUnk->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(&apObjArray[i]));
		if(FAILED(hr))
			continue;

		hr = apObjArray[i]->Get(L"ProcessId", 0, &cn, NULL, NULL);
		if(FAILED(hr))
			continue;

		int iProcessID = wx_static_cast(int, cn.lVal);
		wxString strAddDel = m_bAddNewProcess ? wxT("ADD") : wxT("DEL");

		wxCommandEvent evt(wxEVT_EVENT_NOTIFY_ADD_DELPROCESS);

		evt.SetInt(iProcessID);
		evt.SetString(strAddDel);

		CProcessMonitoring* pMonitoring = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);
		wxQueueEvent(pMonitoring, evt.Clone());
		/*
		if(m_pAddNewProcess)
			theSystem->GetMonitoringObj()->AddNewProcess(ulProcessID);
		else
			theSystem->GetMonitoringObj()->DelProcess(ulProcessID);
		*/
	}
	LeaveCriticalSection(&_gCriEventSink);
	return WBEM_S_NO_ERROR;
}

HRESULT CEventSink::SetStatus(
		/* [in] */ LONG lFlags,
		/* [in] */ HRESULT hResult,
		/* [in] */ BSTR strParam,
		/* [in] */ IWbemClassObject __RPC_FAR* apObjArray)
{
	if(lFlags == WBEM_STATUS_COMPLETE)
	{

	}
	else if(lFlags == WBEM_STATUS_PROGRESS)
	{

	}

	return WBEM_S_NO_ERROR;
}
#endif // __WXMSW__
