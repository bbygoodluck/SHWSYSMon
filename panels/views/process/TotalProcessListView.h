#ifndef PROCESSVIEW_H_INCLUDED
#define PROCESSVIEW_H_INCLUDED

#include "../ProcessListViewCommon.h"
class CTotalProcessListView : public CProcessListViewCommon
{
	struct COLUMN_INFO
	{
		e_ListColums column;
		wxString szCaption;
		wxListColumnFormat cfFormat;
		int nWidth;
	};

public:
	CTotalProcessListView(wxWindow* _parent);
	~CTotalProcessListView();

public:
	virtual void update() override;
	virtual void reload() override;
	virtual void AddNewProcess(unsigned long ulProcessID) override;

	void SetItemSelected(const wxString& strSelProcessID);

private:
	virtual void Initialize() override;

private:
	wxWindow* m_pParent;
};

#endif // PROCESSVIEW_H_INCLUDED
