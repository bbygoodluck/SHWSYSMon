#ifndef PANELCPU_H_INCLUDED
#define PANELCPU_H_INCLUDED

class CCPUMonitoring;
class CCPUUsageGraphView;
class CCPUSVSegmentView;
class CViewCommon;

class CPanelCPU : public wxPanel
{
public:
	explicit CPanelCPU(wxWindow* _parent);
	~CPanelCPU();

private:
	void InitPanel();

private:
	wxVector<CViewCommon *> m_vecViewCPUCore;
	CCPUUsageGraphView* m_viewCPUTotal;
	CCPUSVSegmentView* m_viewCPUSVSegment;

	std::unique_ptr<CCPUMonitoring> m_pCPUMonitor;
};

#endif // PANELCPU_H_INCLUDED
