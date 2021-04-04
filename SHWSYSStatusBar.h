#ifndef SHWSYSSTATUSBAR_H_INCLUDED
#define SHWSYSSTATUSBAR_H_INCLUDED

class CSHWSYSStatusBar : public wxStatusBar
{
	enum _DEFAULT
	{
		Field_CpuInfo = 0,
		Field_Ctrl,
		Field_Shift,
		Field_Alt,
		Field_NumLockIndicator,
		Field_CapsLockIndicator,
		Field_Clock,
		Field_Max
	};

public:
	CSHWSYSStatusBar(wxWindow *parent, long style = wxSTB_DEFAULT_STYLE);
	virtual ~CSHWSYSStatusBar();

private:
	void Setting();
	void UpdateClock();

private:
	wxSize m_sz;
	wxTimer m_timer;

private:
	void OnSize(wxSizeEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnTimer(wxTimerEvent& WXUNUSED(event));
	wxDECLARE_EVENT_TABLE();
};


#endif // SHWSYSSTATUSBAR_H_INCLUDED
