#include "../../ginc.h"
#include "ViewCommon.h"

CViewCommon::CViewCommon(wxWindow* _pParent, wxWindowID id, const wxPoint &pos, const wxSize& _size, long _style)
	: wxWindow(_pParent, id, pos, _size, _style)
	, m_viewRect(0, 0, 0, 0)
	, m_rcUsage(0, 0, 0, 0)
	, m_pDoubleBuffer(nullptr)
{

}

CViewCommon::~CViewCommon()
{
	if (m_pDoubleBuffer)
		delete m_pDoubleBuffer;

	m_pDoubleBuffer = nullptr;
}

wxMemoryDC* CViewCommon::CreateMemoryDC(wxDC* pDC, const wxRect& rc, const wxColour& colPen, const wxColour colBrush)
{
	m_pMemDC = std::make_unique<wxMemoryDC>(pDC);

	m_pMemDC->SelectObject(*m_pDoubleBuffer);
	m_pMemDC->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));

	wxPen pen(colPen);
	wxBrush brush(colBrush);

	m_pMemDC->SetPen(pen);
	m_pMemDC->SetBrush(brush);
	m_pMemDC->DrawRectangle(0, 0, rc.GetWidth(), rc.GetHeight());

	return m_pMemDC.get();
}

void CViewCommon::ClearMemoryDC(wxMemoryDC* pMemDC)
{
	pMemDC->SetFont(wxNullFont);
	pMemDC->SetPen(wxNullPen);
	pMemDC->SetBrush(wxNullBrush);
	pMemDC->SelectObject(wxNullBitmap);
}

void CViewCommon::UpdateView(wxWindow* pWindow, const wxRect& rect, bool bUpdate)
{
#ifdef __WXMSW__
	HWND hWnd = pWindow->GetHWND();
	if (rect.GetWidth() != 0)
	{
		RECT mswRect;
		const RECT* pRect;

		wxCopyRectToRECT(rect, mswRect);
		pRect = &mswRect;

		::InvalidateRect(hWnd, pRect, false);
	}
	else
		pWindow->Refresh(false);

	if (bUpdate) pWindow->Update();
#else
	pWindow->RefreshRect(rect, false);
	pWindow->Update();
#endif
}
