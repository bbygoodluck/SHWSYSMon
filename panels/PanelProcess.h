#ifndef PANELPROCESS_H_INCLUDED
#define PANELPROCESS_H_INCLUDED

class CProcessMonitoring;
class CMemoryMonitoring;

class CTotalProcessListView;
class CCPUProcessListView;
class CMemProcessListView;
class CMemorySpectrumView;

class CPanelProcess : public wxPanel
{
	friend class CTotalProcessListView;
	friend class CCPUProcessListView;
	friend class CMemProcessListView;

public:
	explicit CPanelProcess(wxWindow* _parent);
	~CPanelProcess();

	void SetSelectedItem(const wxString& strSelProcessID);

private:
	void InitPanel();

protected:
	std::unique_ptr<CProcessMonitoring> m_pProcessMonitor;
	std::unique_ptr<CMemoryMonitoring> m_pMemoryMonitor;

	std::unique_ptr<CTotalProcessListView> m_pTotalProcessList;
	std::unique_ptr<CCPUProcessListView> m_pCPUProcessList;
	std::unique_ptr<CMemProcessListView> m_pMemoryProcessList;
	std::unique_ptr<CMemorySpectrumView> m_pMemSpectrumView;

	wxStaticText* m_staticCPU;
	wxStaticText* m_staticMemory;
	wxStaticText* m_staticPhysical;
};

#endif // PANELPROCESS_H_INCLUDED
