#ifndef PROCESSMANAGER_H_INCLUDED
#define PROCESSMANAGER_H_INCLUDED

class CProcessMonitoring;
class CProcessManager : public wxThreadHelper
{
public:
	CProcessManager(CProcessMonitoring* pMonitoring);
	~CProcessManager();

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

	void StartMonitoring();

protected:
	virtual wxThread::ExitCode Entry();

private:
	CProcessMonitoring* m_pMonitoring;
	std::vector<wxWindow *> m_notifyItems;

	bool m_bThreadStop = false;
};

#endif // PROCESSMANAGER_H_INCLUDED
