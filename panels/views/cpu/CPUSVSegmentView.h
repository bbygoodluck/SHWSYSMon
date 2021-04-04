#ifndef CPUSVSEGMENTVIEW_H_INCLUDED
#define CPUSVSEGMENTVIEW_H_INCLUDED

#include "../ViewCommon.h"
class CCPUSVSegmentView : public CViewCommon
{
public:
	explicit CCPUSVSegmentView(wxWindow* _parent, const wxSize& _size);
	virtual ~CCPUSVSegmentView();
	void SetCPUData(unsigned long _data);
	virtual void update() override;

protected:
	virtual void Render(wxDC* pDC) override;

private:
	wxColour GetColor(int iVal, int ishift);

private:
	wxRect rcArea;
	wxRect rcA100;
	wxRect rcB100;
	wxRect rcC100;
	wxRect rcD100;
	wxRect rcE100;
	wxRect rcF100;
	wxRect rcG100;

	wxRect rcA10;
	wxRect rcB10;
	wxRect rcC10;
	wxRect rcD10;
	wxRect rcE10;
	wxRect rcF10;
	wxRect rcG10;

	wxRect rcA1;
	wxRect rcB1;
	wxRect rcC1;
	wxRect rcD1;
	wxRect rcE1;
	wxRect rcF1;
	wxRect rcG1;

	int m_iLeftPadding = 2;
	int m_iTopPadding = 2;
	int m_iRigthPadding = 2;
	int m_iBottomPadding = 2;
	int m_iPadding = 2;
	wxPen m_pen;
	wxBrush m_brush;

	int m_iPercent = 0;

	static int m_iSegmentNums[];
private:
	virtual void OnPaint(wxPaintEvent& event) override;
	virtual void OnSize(wxSizeEvent& event) override;
	virtual void OnErase(wxEraseEvent& event) override;
	virtual void OnNofityEvent(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // CPUSVSEGMENTVIEW_H_INCLUDED
