#ifndef MEMORYMONITORING_H_INCLUDED
#define MEMORYMONITORING_H_INCLUDED

#include "../ResourceCommon.h"

class CSystemCommon;
class CMemoryMonitoring : public CResourceCommon
{
	friend class CSystemCommon;

protected:
	enum {
		ENUM_MEM_TIMER_ID = MEMORY_TIMER_ID
	};

public:
	CMemoryMonitoring();
	~CMemoryMonitoring();

protected:
	void InitMemoryInfo();

private:
	virtual void notify() override;
	void update();

protected:
	MEMORYINFO m_memInfo;

private:
	void OnTimer(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};
#endif // MEMORYMONITORING_H_INCLUDED
