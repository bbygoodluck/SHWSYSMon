#include "../../../ginc.h"
#include "CPUSVSegmentView.h"

int CCPUSVSegmentView::m_iSegmentNums[] = {
	0x7E, //0
	0x30, //1
	0x6D, //2
	0x79, //3
	0x33, //4
	0x5B, //5
	0x5F, //6
	0x70, //7
	0x7F, //8
	0x7B  //9
};

wxBEGIN_EVENT_TABLE(CCPUSVSegmentView, CViewCommon)
	EVT_PAINT(CCPUSVSegmentView::OnPaint)
	EVT_SIZE(CCPUSVSegmentView::OnSize)
	EVT_ERASE_BACKGROUND(CCPUSVSegmentView::OnErase)
	EVT_MY_CUSTOM_COMMAND(wxEVT_EVENT_NOTIFY_GRAPHVIEW, wxID_ANY, CCPUSVSegmentView::OnNofityEvent)
wxEND_EVENT_TABLE()

CCPUSVSegmentView::CCPUSVSegmentView(wxWindow* _parent, const wxSize& _size)
	: CViewCommon(_parent, wxID_ANY, wxDefaultPosition, _size, VIEW_STYLE)
	, m_pen(wxColour(255, 0 , 0))
	, m_brush(wxColour(255, 0 , 0))
{

}

CCPUSVSegmentView::~CCPUSVSegmentView()
{

}

void CCPUSVSegmentView::OnPaint(wxPaintEvent& event)
{
	if(!m_pDoubleBuffer)
		return;

	if (!m_pDoubleBuffer->IsOk())
		return;

	m_viewRect = GetClientRect();

	wxPaintDC dc(this);
	PrepareDC(dc);

	wxMemoryDC* pMemDC = CreateMemoryDC(&dc, m_viewRect);
	Render(pMemDC);

	dc.Blit(0, 0, m_pDoubleBuffer->GetWidth(), m_pDoubleBuffer->GetHeight(), pMemDC, 0, 0);

	ClearMemoryDC(pMemDC);
}

void CCPUSVSegmentView::OnSize(wxSizeEvent& event)
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

void CCPUSVSegmentView::OnErase(wxEraseEvent& event)
{

}

wxColour CCPUSVSegmentView::GetColor(int iVal, int ishift)
{
	int r = 255 * ((iVal >> ishift) & 1);
	int g = 0;
	int b = 0;
//	int a = (iVal >> ishift) & 1;

	return wxColour(r, g, b);
}

void CCPUSVSegmentView::Render(wxDC* pDC)
{
	rcArea = wxRect(wxPoint(m_viewRect.GetLeft() + m_iPadding + 3, m_viewRect.GetTop() + m_iPadding + 2), wxPoint(m_viewRect.GetRight() - m_iPadding, m_viewRect.GetBottom() - (m_iPadding + 2)));
	int iWidth = (rcArea.GetWidth() - (m_iPadding * 8)) / 3;//- (m_iPadding * 8);
	int iHeight = (rcArea.GetHeight() - (m_iPadding * 3)) / 2;

	rcA100 = wxRect(wxPoint(rcArea.GetLeft() + m_iPadding + 2, rcArea.GetTop()), wxPoint(rcArea.GetLeft() + iWidth, rcArea.GetTop() + m_iPadding));
	rcB100 = wxRect(wxPoint(rcA100.GetRight(), rcA100.GetBottom() + m_iPadding), wxPoint(rcA100.GetRight() + m_iPadding, rcA100.GetBottom() + iHeight));
	rcC100 = wxRect(wxPoint(rcA100.GetRight(), rcB100.GetBottom() + m_iPadding * 2), wxPoint(rcA100.GetRight() + m_iPadding, rcArea.GetBottom() - m_iPadding));
	rcD100 = wxRect(wxPoint(rcA100.GetLeft(), rcC100.GetBottom()), wxPoint(rcA100.GetRight() - m_iPadding, rcArea.GetBottom()));
	rcE100 = wxRect(wxPoint(rcArea.GetLeft(), rcC100.GetTop()), wxPoint(rcArea.GetLeft() + m_iPadding, rcC100.GetBottom()));
	rcF100 = wxRect(wxPoint(rcArea.GetLeft(), rcB100.GetTop()), wxPoint(rcArea.GetLeft() + m_iPadding, rcB100.GetBottom()));
	rcG100 = wxRect(wxPoint(rcA100.GetLeft(), rcF100.GetBottom() + 1), wxPoint(rcA100.GetRight() - m_iPadding, rcC100.GetTop() - 1));

	rcA10 = wxRect(wxPoint(rcA100.GetRight() + (m_iPadding * 4), rcA100.GetTop()), wxPoint(rcA100.GetRight() + (m_iPadding * 3) + iWidth, rcArea.GetTop() + m_iPadding));
	rcB10 = wxRect(wxPoint(rcA10.GetRight(), rcA10.GetBottom() + m_iPadding), wxPoint(rcA10.GetRight() + m_iPadding, rcA10.GetBottom() + iHeight));
	rcC10 = wxRect(wxPoint(rcA10.GetRight(), rcB10.GetBottom() + m_iPadding * 2), wxPoint(rcA10.GetRight() + m_iPadding, rcArea.GetBottom() - m_iPadding));
	rcD10 = wxRect(wxPoint(rcA10.GetLeft(), rcC10.GetBottom()), wxPoint(rcA10.GetRight() - m_iPadding, rcArea.GetBottom()));
	rcE10 = wxRect(wxPoint(rcA10.GetLeft() - m_iPadding, rcC10.GetTop()), wxPoint(rcA10.GetLeft(), rcC10.GetBottom()));
	rcF10 = wxRect(wxPoint(rcA10.GetLeft() - m_iPadding, rcB10.GetTop()), wxPoint(rcA10.GetLeft(), rcB10.GetBottom()));
	rcG10 = wxRect(wxPoint(rcA10.GetLeft() + m_iPadding, rcF10.GetBottom() + 1), wxPoint(rcA10.GetRight() - m_iPadding, rcC10.GetTop() - 1));

	rcA1 = wxRect(wxPoint(rcA10.GetRight() + (m_iPadding * 4), rcA10.GetTop()), wxPoint(rcA10.GetRight() + (m_iPadding * 3) + iWidth, rcArea.GetTop() + m_iPadding));
	rcB1 = wxRect(wxPoint(rcA1.GetRight(), rcA1.GetBottom() + m_iPadding), wxPoint(rcA1.GetRight() + m_iPadding, rcA1.GetBottom() + iHeight));
	rcC1 = wxRect(wxPoint(rcA1.GetRight(), rcB1.GetBottom() + m_iPadding * 2), wxPoint(rcA1.GetRight() + m_iPadding, rcArea.GetBottom() - m_iPadding));
	rcD1 = wxRect(wxPoint(rcA1.GetLeft(), rcC1.GetBottom()), wxPoint(rcA1.GetRight() - m_iPadding, rcArea.GetBottom()));
	rcE1 = wxRect(wxPoint(rcA1.GetLeft() - m_iPadding, rcC1.GetTop()), wxPoint(rcA1.GetLeft(), rcC1.GetBottom()));
	rcF1 = wxRect(wxPoint(rcA1.GetLeft() - m_iPadding, rcB1.GetTop()), wxPoint(rcA1.GetLeft(), rcB1.GetBottom()));
	rcG1 = wxRect(wxPoint(rcA1.GetLeft() + m_iPadding, rcF1.GetBottom() + 1), wxPoint(rcA1.GetRight() - m_iPadding, rcC1.GetTop() - 1));

	int iVal100 = 0;
	int iVal10  = 0;
	int iVal1   = 0;

	if(m_iPercent == 100)
		iVal100 = 1;
	else
	{
		iVal10 = m_iPercent / 10;
		iVal1  = m_iPercent % 10;
	}

	//백단위
	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 6));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 6));
	pDC->DrawRectangle(rcA100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 5));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 5));
	pDC->DrawRectangle(rcB100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 4));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 4));
	pDC->DrawRectangle(rcC100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 3));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 3));
	pDC->DrawRectangle(rcD100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 2));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 2));
	pDC->DrawRectangle(rcE100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 1));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 1));
	pDC->DrawRectangle(rcF100);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal100], 0));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal100], 0));
	pDC->DrawRectangle(rcG100);

	//십단위
	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 6));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 6));
	pDC->DrawRectangle(rcA10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 5));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 5));
	pDC->DrawRectangle(rcB10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 4));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 4));
	pDC->DrawRectangle(rcC10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 3));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 3));
	pDC->DrawRectangle(rcD10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 2));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 2));
	pDC->DrawRectangle(rcE10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 1));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 1));
	pDC->DrawRectangle(rcF10);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal10], 0));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal10], 0));
	pDC->DrawRectangle(rcG10);

	//일단위
	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 6));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 6));
	pDC->DrawRectangle(rcA1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 5));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 5));
	pDC->DrawRectangle(rcB1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 4));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 4));
	pDC->DrawRectangle(rcC1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 3));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 3));
	pDC->DrawRectangle(rcD1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 2));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 2));
	pDC->DrawRectangle(rcE1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 1));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 1));
	pDC->DrawRectangle(rcF1);

	pDC->SetPen(GetColor(m_iSegmentNums[iVal1], 0));
	pDC->SetBrush(GetColor(m_iSegmentNums[iVal1], 0));
	pDC->DrawRectangle(rcG1);
}

void CCPUSVSegmentView::OnNofityEvent(wxCommandEvent& event)
{
	m_iPercent = event.GetInt();
	UpdateView(this, m_viewRect);
}

void CCPUSVSegmentView::update()
{
	UpdateView(this, m_viewRect);
}

void CCPUSVSegmentView::SetUsageData(unsigned long _data)
{
	m_iPercent = _data;
}
