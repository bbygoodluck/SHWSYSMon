#ifndef MEMORYSPECTRUMVIEW_H_INCLUDED
#define MEMORYSPECTRUMVIEW_H_INCLUDED

#include "../ViewCommon.h"
class CMemorySpectrumView : public CViewCommon
{
protected:
	enum {
		ENUM_MEM_SPECTRUM_TIMER_ID = MEMORY_SPECTRUM_TIMER_ID
	};

public:
	explicit CMemorySpectrumView(wxWindow* _parent, const wxSize& _size);
	virtual ~CMemorySpectrumView();

	virtual void update() override;
	virtual void SetUsageData(unsigned long _data) override;

protected:
	virtual void Render(wxDC* pDC) override;

private:
	wxTimer m_timer;
	int m_iXPosStart = 0;
	float m_fPhysicalTotalMem = 0.0f;
	float m_fPhysicalUsedMem = 0.0f;

	int   m_iPercent = 0;

	wxPen m_penSpectrum;
	wxBrush m_brushSpectrum;

	bool m_bFront = true;

	wxRect m_rcUsage;
private:
	virtual void OnPaint(wxPaintEvent& event) override;
	virtual void OnSize(wxSizeEvent& event) override;
	virtual void OnErase(wxEraseEvent& event) override;
	virtual void OnNofityEvent(wxCommandEvent& event) override;
	void OnTimer(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // MEMORYSPECTRUMVIEW_H_INCLUDED
