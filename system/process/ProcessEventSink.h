#ifndef PROCESSEVENTSINK_H_INCLUDED
#define PROCESSEVENTSINK_H_INCLUDED

class CEventSink : public IWbemObjectSink
{
	LONG m_lRef;
	bool m_bDone;

public:
	CEventSink();
	~CEventSink();

	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray);
	virtual HRESULT STDMETHODCALLTYPE SetStatus(
			/* [in] */ LONG lFlags,
			/* [in] */ HRESULT hResult,
			/* [in] */ BSTR strParam,
			/* [in] */ IWbemClassObject __RPC_FAR* apObjArray);

protected:
	bool m_bAddNewProcess = true;
};

#endif // PROCESSEVENTSINK_H_INCLUDED
