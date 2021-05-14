#include "../../../ginc.h"
#include "CPUUsageGraphView.h"

wxBEGIN_EVENT_TABLE(CCPUUsageGraphView, CViewCommon)
	EVT_PAINT(CCPUUsageGraphView::OnPaint)
	EVT_SIZE(CCPUUsageGraphView::OnSize)
	EVT_ERASE_BACKGROUND(CCPUUsageGraphView::OnErase)
	EVT_MY_CUSTOM_COMMAND(wxEVT_EVENT_NOTIFY_GRAPHVIEW, wxID_ANY, CCPUUsageGraphView::OnNofityEvent)
wxEND_EVENT_TABLE()

CCPUUsageGraphView::CCPUUsageGraphView(wxWindow* _parent, const wxSize& _size)
	: CViewCommon(_parent, wxID_ANY, wxDefaultPosition, _size, VIEW_STYLE)
	, m_colRuler(140, 140, 140)
{

}

CCPUUsageGraphView::~CCPUUsageGraphView()
{
	ClearData();
}

void CCPUUsageGraphView::ClearData()
{
	GRAPH_DATA_NODE* pCurrNode = m_headNode;
	while (pCurrNode != nullptr)
	{
		m_headNode = pCurrNode->_next;
		delete pCurrNode;
		pCurrNode = m_headNode;
	}

	delete m_headNode;
//	GRAPH_DATA_NODE* pCurrNode = m_headNode;
//	while (pCurrNode != nullptr)
//	{
//		GRAPH_DATA_NODE* tmpTarget = pCurrNode;
//		pCurrNode = pCurrNode->_next;
//		delete tmpTarget;
//	}
}

void CCPUUsageGraphView::OnErase(wxEraseEvent& event)
{

}

void CCPUUsageGraphView::OnPaint(wxPaintEvent& event)
{
	if(!m_pDoubleBuffer)
		return;

	if (!m_pDoubleBuffer->IsOk())
		return;

	m_viewRect = GetClientRect();

	wxPaintDC dc(this);
	PrepareDC(dc);

	wxMemoryDC* pMemDC = CreateMemoryDC(&dc, m_viewRect);

	::SetViewportOrgEx(pMemDC->GetHDC(), m_viewRect.GetRight(), m_viewRect.GetBottom(), nullptr);
	m_iViewportMaxX = -1 * m_viewRect.GetRight();
	m_iViewportMaxY = -1 * m_viewRect.GetBottom();

	Render(pMemDC);

	dc.Blit(0, 0, m_pDoubleBuffer->GetWidth(), m_pDoubleBuffer->GetHeight(), pMemDC, m_iViewportMaxX, m_iViewportMaxY);

	ClearMemoryDC(pMemDC);
}

void CCPUUsageGraphView::Render(wxDC* pDC)
{
	int iXPos = 0;
	int iCurXPos = iXPos;
	int iCurYPos = CONST_ONE_SECOND_Y_HEIGHT;

	wxPen pen(m_colRuler, 1, wxPENSTYLE_SHORT_DASH );
	pDC->SetPen(pen);

	int iRowPos = m_iViewportMaxY / 4;
	for (int i = 0; i < 3; i++)
		pDC->DrawLine(0, (i + 1) * iRowPos, m_iViewportMaxX, (i + 1) * iRowPos);

	pen.SetColour(m_colRuler);
	pen.SetStyle(wxPENSTYLE_SOLID);
	pDC->SetPen(pen);

	int iTenSecCnt = 0;
	while (iCurXPos > m_iViewportMaxX)
	{
		if ((iTenSecCnt % 10) == 0)
		{
			iTenSecCnt = 0;
			iCurYPos = CONST_TEN_SECOND_Y_HEIGHT;
		}
		else
		{
			iCurYPos = CONST_ONE_SECOND_Y_HEIGHT;
		}

		pDC->DrawLine(iCurXPos + 1, 0, iCurXPos + 1, -1 * iCurYPos);
		pDC->DrawLine(iCurXPos + 1, m_iViewportMaxY, iCurXPos + 1, m_iViewportMaxY + iCurYPos);

		iCurXPos -= CONST_ONE_SECOND;
		iTenSecCnt++;
	}

	DrawGraph(pDC);
	if (!m_strDispText.IsEmpty())
	{
		wxString strDisp = m_strDispText;
		if(theSettings->GetCPUUsageDisplay())
			strDisp += wxString::Format(wxT(" : %02d"), m_iPercent) + wxT("%");

		pDC->SetTextForeground(wxColour(150, 150, 150));
		pDC->DrawText(strDisp, m_iViewportMaxX + 5, m_iViewportMaxY + 2);

		pDC->SetTextForeground(wxColour(240, 240, 240));
		pDC->DrawText(strDisp, m_iViewportMaxX + 4, m_iViewportMaxY + 1);
	}
}

void CCPUUsageGraphView::OnSize(wxSizeEvent& event)
{
	wxSize size = GetClientSize();
	if ((size.x == 0) || (size.y == 0))
		return;

	if (m_szChagned.x != size.x)
	{
		m_szChagned = size;
		if (m_pDoubleBuffer)
			delete m_pDoubleBuffer;

		m_pDoubleBuffer = new wxBitmap(m_szChagned.x, m_szChagned.y);
	}

	UpdateView(this, m_viewRect);
}

void CCPUUsageGraphView::DrawGraph(wxDC* pDC)
{
	int iYTop = m_iViewportMaxY;
	int iYBottom = 0;

//	if (m_headNode == nullptr || m_headNode->_next == nullptr)
//		return;
	if(m_headNode == m_tailNode)
		return;

	int iXPos = -1;
	int x1;
	int y1;
	int x2;
	int y2;

//	GRAPH_DATA_NODE* pCurrNode = m_headNode->_next;
	GRAPH_DATA_NODE* pCurrNode = m_headNode;

	while (pCurrNode != nullptr)
	{
		iXPos = pCurrNode->_iXPos;

		if (pCurrNode->_bLineDraw)
		{
			wxPen pen(pCurrNode->_col, 1, pCurrNode->_penStyle);
			pDC->SetPen(pen);
			pDC->DrawLine(iXPos, iYTop, iXPos, iYBottom);
		}

		x1 = pCurrNode->_iXPos;
		y1 = pCurrNode->_iYPos;
		if (pCurrNode->_next)
		{
			x2 = pCurrNode->_next->_iXPos;
			y2 = pCurrNode->_next->_iYPos;

			wxPen pen(wxColour(0, 255, 0), 1, wxPENSTYLE_SOLID);
			pDC->SetPen(pen);
			pDC->DrawLine(x1, y1, x2, y2);
		}

		pCurrNode = pCurrNode->_next;
	}
}

void CCPUUsageGraphView::SetCPUData(unsigned long _data)
{
	m_lCPUData = _data;
}

void CCPUUsageGraphView::update()
{
	if (m_viewRect.GetWidth() == 0)
		return;

	int iYPos = (int)(((m_lCPUData * m_viewRect.GetHeight()) / 100) * -1);
	if (iYPos < m_iViewportMaxY)
		iYPos = m_iViewportMaxY;

	m_iPercent = wx_static_cast(int, m_lCPUData);

	GRAPH_DATA_NODE* _dataNode = new GRAPH_DATA_NODE();
	_dataNode->_next = nullptr;
	_dataNode->_iXPos = 1;
	_dataNode->_ulData = m_lCPUData;
	_dataNode->_iYPos = iYPos;

	m_iAddCount++;
	if ((m_iAddCount % 10) != 0)
		_dataNode->_bLineDraw = false;

	if (m_headNode == nullptr)
	{
	//	m_headNode = new GRAPH_DATA_NODE;
	//	m_headNode->_next = _dataNode;
		m_headNode = _dataNode;
		m_tailNode = _dataNode;
	}
	else
	{
		m_tailNode->_next = _dataNode;
		m_tailNode = _dataNode;

		GRAPH_DATA_NODE* pCurrNode = m_headNode;
		while (pCurrNode != nullptr)
		{
			pCurrNode->_iXPos -= CONST_ONE_SECOND;
			pCurrNode = pCurrNode->_next;

			if(pCurrNode == m_tailNode)
				break;
		}

		GRAPH_DATA_NODE* firstNode = m_headNode;
		if (firstNode->_iXPos <= m_iViewportMaxX)
		{
			m_headNode = firstNode->_next;
			delete firstNode;
		}

	//	GRAPH_DATA_NODE* pLastNode = m_headNode;
	//	GRAPH_DATA_NODE* pTmpNode = m_headNode->_next;
	//	while (pTmpNode != nullptr)
	//	{
	//		if (pLastNode->_next != nullptr)
	//			pLastNode = pLastNode->_next;

	//		pTmpNode->_iXPos -= CONST_ONE_SECOND;
	//		pTmpNode = pTmpNode->_next;
	//	}

	//	pLastNode->_next = _dataNode;

	//	GRAPH_DATA_NODE* firstNode = m_headNode->_next;
	//	if (firstNode->_iXPos <= m_iViewportMaxX)
	//	{
	//		m_headNode->_next = firstNode->_next;
	//		delete firstNode;
	//	}
	}

	if (m_iAddCount == 10)
		m_iAddCount = 0;

	UpdateView(this, m_viewRect);
}

void CCPUUsageGraphView::OnNofityEvent(wxCommandEvent& event)
{
	int iUsage = event.GetInt();
	m_lCPUData = iUsage;

	update();
}
