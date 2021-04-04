#ifndef SHWSYSMON_H_INCLUDED
#define SHWSYSMON_H_INCLUDED

//***************************************************************************************
// Program : RealTime System Monitoring
// Make by : Bae Byoung Yong
// Date    : 2021.03.10 ~
//***************************************************************************************

class CSHWSYSMon : public wxApp
{
public:
	explicit CSHWSYSMon();
	virtual ~CSHWSYSMon();

	virtual bool OnInit() override;
	virtual int OnExit() override;

private:
	bool OnLoadSettings();

#ifdef __WXMSW__
	void CreateCOMInstance();
#endif // __WXMSW__
private:
#ifdef NDEBUG
	wxSingleInstanceChecker* m_pSingleInstance;
#endif

private:
	wxDECLARE_EVENT_TABLE();
};
#endif // SHWSYSMON_H_INCLUDED
