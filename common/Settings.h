#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include "ConfigBase.h"
class CSettings : public CConfigBase
{
private:
	CSettings();

public:
	~CSettings();
	static CSettings* Get();

	virtual bool Load() override;

	const wxRect& GetDefaultRect() const     { return m_rcDefault; }

	const int GetMonitorPos()    const       { return m_iCurrentMonitor;    }
	const int GetMaximized()     const       { return m_iMaximized;         }
	const int GetXPos()          const       { return m_iXPos;              }
	const int GetYPos()          const       { return m_iYPos;              }
	const int GetWidth()         const       { return m_iWidth;             }
	const int GetHeight()        const       { return m_iHeight;            }
	const int GetCPUViewHeignt() const       { return m_iCPUViewHeight;     }
	const int GetCPUMemListWidth() const     { return m_iCPUMemListWidth;   }
	const int GetCPUCoreDisplayInRow() const { return m_iCPUCoreDispInRow;  }
	const bool GetCPUUsageDisplay() const    { return m_bCPUUsageDisp;      }
	const int IsMaximized() const            { return m_iMaximized;         }

	void SetChnageOptionVal(const wxString& strMember, int iVal);
	void SetChnageOptionVal(const wxString& strMember, const wxString& strVal);
private:
	bool DoSettings();

private:
	//singleton pattern 구현을 위한 인스턴스
	static std::unique_ptr<CSettings> m_pInstance;
	//표시 모니터
	int m_iCurrentMonitor   = 0;
	//최대/최소 여부
	int m_iMaximized        = 0;
	//윈도우 Width
	int m_iWidth            = 0;
	//윈도우 Height
	int m_iHeight           = 0;
	//윈도우 xPos
	int m_iXPos             = 0;
	//윈도우 yPos
	int m_iYPos             = 0;
	//CPU정보 표시 윈도우 Height
	int m_iCPUViewHeight    = 0;
	//CPU/Memory 리스트 Width
	int m_iCPUMemListWidth  = 0;
	//CPU코어 표시 카운터
	int m_iCPUCoreDispInRow = 0;
	//CPUUsage 표시
	bool m_bCPUUsageDisp    = false;
	wxRect m_rcDefault;
};

#endif // SETTINGS_H_INCLUDED
