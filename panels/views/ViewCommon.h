#ifndef VIEWCOMMON_H_INCLUDED
#define VIEWCOMMON_H_INCLUDED

/*
	This class is common class for cpu usage
*/
class CViewCommon : public wxWindow
{
public:
	explicit CViewCommon(wxWindow* _pParent, wxWindowID id, const wxPoint &pos, const wxSize& _size, long _style);
	virtual ~CViewCommon();

public:
	virtual void update() = 0;
	virtual void SetUsageData(unsigned long _data) = 0;

	void SetDispText(const wxString& strDisp) { m_strDispText = strDisp; }

	void SetCPUCoreNumber(int iCPUNumber) { m_iCPUCoreNum = iCPUNumber;	}
	int GetCPUCoreNumber() { return m_iCPUCoreNum; }

protected:
	wxMemoryDC* CreateMemoryDC(wxDC* pDC, const wxRect& rc, const wxColour& colPen = wxColour(0, 0, 0), const wxColour colBrush = wxColour(0, 0, 0));
	void ClearMemoryDC(wxMemoryDC* pMemDC);

	virtual void OnPaint(wxPaintEvent& event)         = 0;
	virtual void OnSize(wxSizeEvent& event)           = 0;
	virtual void OnErase(wxEraseEvent& event)         = 0;
	virtual void Render(wxDC* pDC)                    = 0;
	virtual void OnNofityEvent(wxCommandEvent& event) = 0;
	void UpdateView(wxWindow* pWindow, const wxRect& rect, bool bUpdate = false);

protected:
	wxRect m_viewRect;
	wxRect m_rcUsage;
	wxBitmap* m_pDoubleBuffer;
	wxSize m_szChagned;
	wxString m_strDispText = wxT("");
	int m_iCPUCoreNum = -1;
	unsigned long m_lCPUData = 0;
	int m_iPercent = 0;

private:
	std::unique_ptr<wxMemoryDC> m_pMemDC = nullptr;
};

#endif // VIEWCOMMON_H_INCLUDED
