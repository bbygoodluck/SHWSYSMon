#ifndef MEMPROCESSLISTVIEW_H_INCLUDED
#define MEMPROCESSLISTVIEW_H_INCLUDED

#include "../ProcessListViewCommon.h"
class CMemProcessListView : public CProcessListViewCommon
{
	struct COLUMN_INFO
	{
		e_MemProcessInfo column;
		wxString szCaption;
		wxListColumnFormat cfFormat;
		int nWidth;
	};
public:
	CMemProcessListView(wxWindow* _parent, const wxSize& sz);
	~CMemProcessListView();

public:
	virtual void update() override;
	virtual void AddNewProcess(unsigned long ulProcessID) override;

private:
	virtual void Initialize() override;
	void OnItemSelected(wxListEvent& event);

private:
	wxWindow* m_pParent;


};




#endif // MEMPROCESSLISTVIEW_H_INCLUDED
