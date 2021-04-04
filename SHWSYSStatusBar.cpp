#include "./ginc.h"
#include "SHWSYSStatusBar.h"

static const char *numlockIndicators[] = { "OFF", " NUM" };
static const char *capslockIndicators[] = { "", " CAPS" };
static const char *ctrlIndicators[] = { "", " CTRL" };
static const char *shiftIndicators[] = { "", "SHIFT" };
static const char *altIndicators[] = { "", " ALT" };

wxBEGIN_EVENT_TABLE(CSHWSYSStatusBar, wxStatusBar)
	EVT_SIZE(CSHWSYSStatusBar::OnSize)
	EVT_TIMER(wxID_ANY, CSHWSYSStatusBar::OnTimer)
	EVT_IDLE(CSHWSYSStatusBar::OnIdle)
wxEND_EVENT_TABLE()

CSHWSYSStatusBar::CSHWSYSStatusBar(wxWindow *parent, long style)
	: wxStatusBar(parent, wxID_ANY, style, wxT("SHWSYSStatusBar"))
	, m_timer(this)
{
	wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Fixedsys"));
	this->SetFont(font);//wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));

	Setting();
	m_timer.Start(1000);
	UpdateClock();
}

CSHWSYSStatusBar::~CSHWSYSStatusBar()
{
	if (m_timer.IsRunning())
		m_timer.Stop();
}

void CSHWSYSStatusBar::OnSize(wxSizeEvent& event)
{
	m_sz = event.GetSize();
	Setting();
}

void CSHWSYSStatusBar::OnIdle(wxIdleEvent& event)
{
	SetStatusText(ctrlIndicators[wxGetKeyState(WXK_CONTROL)], Field_Ctrl);
	SetStatusText(shiftIndicators[wxGetKeyState(WXK_SHIFT)], Field_Shift);
	SetStatusText(altIndicators[wxGetKeyState(WXK_ALT)], Field_Alt);
	SetStatusText(numlockIndicators[wxGetKeyState(WXK_NUMLOCK)],Field_NumLockIndicator);
	SetStatusText(capslockIndicators[wxGetKeyState(WXK_CAPITAL)],Field_CapsLockIndicator);

	event.Skip();
}

void CSHWSYSStatusBar::Setting()
{
	int numLockWidth = 40;
	int capsLockWidth = 40;
	int ctrlWidth = 40;
	int shiftWidth = 40;
	int altWidth = 40;
	int timeWidth = 250;

	int processorWidth = m_sz.GetWidth() - (numLockWidth + capsLockWidth + ctrlWidth + shiftWidth + altWidth + timeWidth);
	int widths[Field_Max];

	widths[Field_CpuInfo]           = processorWidth;
	widths[Field_Ctrl]              = ctrlWidth;
	widths[Field_Shift]             = shiftWidth;
	widths[Field_Alt]               = altWidth;
	widths[Field_NumLockIndicator]  = numLockWidth;
	widths[Field_CapsLockIndicator] = capsLockWidth;
	widths[Field_Clock]             = timeWidth;

	SetFieldsCount(Field_Max);
	SetStatusWidths(Field_Max, widths);

	wxString strProcessorInfo(theSystem->GetProcessorInfo() + wxString::Format(wxT(" ( %d Core )"), theSystem->GetCPUCoreCount()));
	SetStatusText(strProcessorInfo, Field_CpuInfo);
}

void CSHWSYSStatusBar::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	UpdateClock();
}

void CSHWSYSStatusBar::UpdateClock()
{
	wxDateTime tm = wxDateTime::Now();
	wxString strTimeNow = wxT(" ") + tm.FormatISODate() + wxT(" ") + tm.FormatISOTime();
	SetStatusText(strTimeNow, Field_Clock);
}
