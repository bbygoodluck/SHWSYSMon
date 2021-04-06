#ifndef CPUPROCESSLISTVIEW_H_INCLUDED
#define CPUPROCESSLISTVIEW_H_INCLUDED

#include "../ProcessListViewCommon.h"
class CCPUProcessListView : public CProcessListViewCommon
{
	struct COLUMN_INFO
	{
		e_CPUProcessInfo column;
		wxString szCaption;
		wxListColumnFormat cfFormat;
		int nWidth;
	};
public:
	CCPUProcessListView(wxWindow* _parent, const wxSize& sz);
	~CCPUProcessListView();

public:
	virtual void update() override;
	virtual void reload() override;
	virtual void AddNewProcess(unsigned long ulProcessID) override;

private:
	virtual void Initialize() override;

private:
	wxWindow* m_pParent;
};


#endif // CPUPROCESSLISTVIEW_H_INCLUDED
