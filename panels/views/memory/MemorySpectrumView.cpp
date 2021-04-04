#include "../../../ginc.h"
#include "MemorySpectrumView.h"

wxBEGIN_EVENT_TABLE(CMemorySpectrumView, CViewCommon)
	EVT_PAINT(CMemorySpectrumView::OnPaint)
	EVT_SIZE(CMemorySpectrumView::OnSize)
	EVT_ERASE_BACKGROUND(CMemorySpectrumView::OnErase)
	EVT_TIMER(ENUM_MEM_SPECTRUM_TIMER_ID, CMemorySpectrumView::OnTimer)
wxEND_EVENT_TABLE()

CMemorySpectrumView::CMemorySpectrumView(wxWindow* _parent, const wxSize& _size)
	: CViewCommon(_parent, wxID_ANY, wxDefaultPosition, _size, VIEW_STYLE)
	, m_penSpectrum(wxColour(255, 255, 0), 1, wxPENSTYLE_SOLID)
	, m_brushSpectrum(wxColour(255, 255, 0))
	, m_rcUsage(0, 0, 0, 0)
{
	m_timer.SetOwner(this, ENUM_MEM_SPECTRUM_TIMER_ID);
	m_timer.Start(500);
}

CMemorySpectrumView::~CMemorySpectrumView()
{
	if(m_timer.IsRunning())
		m_timer.Stop();
}

void CMemorySpectrumView::OnPaint(wxPaintEvent& event)
{
	if(!m_pDoubleBuffer)
		return;

	if (!m_pDoubleBuffer->IsOk())
		return;

	m_viewRect = GetClientRect();
	m_rcUsage = GetClientRect();

	wxPaintDC dc(this);
	PrepareDC(dc);

	wxMemoryDC* pMemDC = CreateMemoryDC(&dc, m_viewRect);//, wxColour(200, 200, 200), wxColour(220, 220, 220));
	Render(pMemDC);

	dc.Blit(0, 0, m_pDoubleBuffer->GetWidth(), m_pDoubleBuffer->GetHeight(), pMemDC, 0, 0);
	ClearMemoryDC(pMemDC);
}

void CMemorySpectrumView::OnSize(wxSizeEvent& event)
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

void CMemorySpectrumView::OnErase(wxEraseEvent& event)
{

}

void CMemorySpectrumView::OnNofityEvent(wxCommandEvent& event)
{

}

void CMemorySpectrumView::Render(wxDC* pDC)
{
	int iRed = (2 * m_iPercent) + 55;
	int iGreen = 255 - (m_iPercent * 3);
	iGreen = iGreen < 0 ? 0 : iGreen;
	wxColour col(iRed, iGreen, 0);

	int iRight = (m_iPercent * m_viewRect.GetRight()) / 100;
	m_rcUsage.SetRight(iRight);

	pDC->GradientFillLinear(m_rcUsage, wxColour(0, 255, 0), col);

	int x1Pos = 0;
	int y1Pos = m_viewRect.GetTop();
	int x2Pos = 2;
	int y2Pos = m_viewRect.GetHeight();

	if(m_bFront)
		x1Pos = iRight + 3;
	else
		x1Pos = iRight + 1;

	m_bFront = !m_bFront;

	wxColour colPen(iRed, iGreen, 0);
	pDC->SetPen(colPen);
	pDC->SetBrush(col);

	pDC->DrawRectangle(x1Pos, y1Pos, x2Pos, y2Pos);

	wxString strMemPercent = wxString::Format(wxT("%d%"), m_iPercent);
	wxString strMemUsed = wxString::Format(wxT("%3.2fGB"), m_fPhysicalUsedMem);

	wxRect viewRectShadow(2, 2, iRight, m_viewRect.GetBottom());
	pDC->SetTextForeground(wxColour(192, 192, 192));
	pDC->DrawLabel(strMemUsed, viewRectShadow, wxALIGN_CENTER);

	wxRect viewRectUsed(1, 1, iRight, m_viewRect.GetBottom());
	pDC->SetTextForeground(wxColour(10, 10, 10));
	pDC->DrawLabel(strMemUsed, viewRectUsed, wxALIGN_CENTER);

	pDC->SetTextForeground(col);//wxColour(60, 60, 60));
	pDC->DrawText(strMemPercent, x1Pos + 3, y1Pos + 2);
}

void CMemorySpectrumView::update()
{
	MEMORYINFO* pInfo = theSystem->GetMemoryInfo();

	m_fPhysicalUsedMem = (float)pInfo->_ullPhysicalUsedRam / 1024.0f;
	m_fPhysicalTotalMem = (float)pInfo->_ullPhysicalTotalRam / 1000.0f;

	m_iPercent = wx_static_cast(int, ((pInfo->_ullPhysicalUsedRam  * 100) / pInfo->_ullPhysicalTotalRam));
}

void CMemorySpectrumView::OnTimer(wxTimerEvent& event)
{
	UpdateView(this, m_viewRect);
}
