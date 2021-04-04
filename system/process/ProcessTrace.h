#ifndef PROCESSTRACE_H_INCLUDED
#define PROCESSTRACE_H_INCLUDED

#include "ProcessEventSink.h"
class CProcessAddEventSink : public CEventSink
{
public:
	CProcessAddEventSink() {};
	virtual ~CProcessAddEventSink() {};
	virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray)
	{
		m_bAddNewProcess = true;
		return CEventSink::Indicate(lObjectCount, apObjArray);
	}
};

class CProcessDelEventSink : public CEventSink
{
public:
	CProcessDelEventSink() {};
	virtual ~CProcessDelEventSink() {};

	virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray)
	{
		m_bAddNewProcess = false;
		return CEventSink::Indicate(lObjectCount, apObjArray);
	}
};

class CProcessTrace
{
public:
	CProcessTrace();
	~CProcessTrace();

private:
	bool Init();
	bool CreateStartTraceInfo();
	bool CreateEndTraceInfo();

private:
	CProcessAddEventSink* m_pAddEventSink;
	CProcessDelEventSink* m_pDelEventSink;

	HRESULT m_hResult = S_OK;
	IUnsecuredApartment* m_pUnsecApp = nullptr;
	///////////////////////////////////////////////////////////////////
	IUnknown* m_pAddStubUnk = nullptr;
	IWbemObjectSink* m_pAddSinkStub = nullptr;

	IUnknown* m_pDelStubUnk = nullptr;
	IWbemObjectSink* m_pDelSinkStub = nullptr;
};

#endif // PROCESSTRACE_H_INCLUDED
