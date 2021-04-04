#ifndef RESOURCECOMMON_H_INCLUDED
#define RESOURCECOMMON_H_INCLUDED

class CResourceCommon : public wxEvtHandler
{
public:
	CResourceCommon() {};
	~CResourceCommon()
	{
		if(m_timer.IsRunning())
			m_timer.Stop();
	};

public:
	virtual void notify() = 0;
	void StartMonitoring() {
		m_timer.Start(1000);
	}

	void AddItem(wxWindow* pItem)
	{
		m_notifyItems.emplace_back(pItem);
	}

	void RemoveItem(wxWindow* pItem)
	{
		std::vector<wxWindow *>::iterator findIter = std::find(m_notifyItems.begin(), m_notifyItems.end(), pItem);
		if(findIter != m_notifyItems.end())
		m_notifyItems.erase(findIter);
	}

protected:
	std::vector<wxWindow *> m_notifyItems;
	wxTimer m_timer;
};

#endif // RESOURCECOMMON_H_INCLUDED
