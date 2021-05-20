#ifndef CPUUSAGEVIEW_H_INCLUDED
#define CPUUSAGEVIEW_H_INCLUDED

#include "../ViewCommon.h"
class CCPUUsageGraphView : public CViewCommon
{
	typedef struct _GRAPH_DATA_NODE {
		int _iXPos = 0;
		int _iYPos = 0;
		unsigned long _ulData = 0;
		wxColour _col = wxColour(0, 170, 0);
		wxPenStyle _penStyle = wxPENSTYLE_DOT;
		bool _bLineDraw = true;
		_GRAPH_DATA_NODE* _next;
	} GRAPH_DATA_NODE;

	enum _CONST_ENUM {
		CONST_ONE_SECOND = 4,
		CONST_TEN_SECOND = CONST_ONE_SECOND * 10,
		CONST_ONE_SECOND_Y_HEIGHT = 3,
		CONST_TEN_SECOND_Y_HEIGHT = 6
	};

public:
	explicit CCPUUsageGraphView(wxWindow* _parent, const wxSize& _size);
	virtual ~CCPUUsageGraphView();
	virtual void SetUsageData(unsigned long _data) override;
	virtual void update() override;

protected:
	virtual void Render(wxDC* pDC) override;
	void DrawGraph(wxDC* pDC);
	void ClearData();

private:
	wxColour m_colRuler;
	int m_iAddCount = -1;
	int m_iReveseCount = 0;
	int m_iViewportMaxX = -1;
	int m_iViewportMaxY = -1;
	int m_iPercent = 0;

	GRAPH_DATA_NODE* m_headNode = nullptr;
	GRAPH_DATA_NODE* m_tailNode = nullptr;

private:
	virtual void OnPaint(wxPaintEvent& event) override;
	virtual void OnSize(wxSizeEvent& event) override;
	virtual void OnErase(wxEraseEvent& event) override;
	virtual void OnNofityEvent(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // CPUUSAGEVIEW_H_INCLUDED
