#include "common.h"
#include "Settings.h"

#include <iostream>
#include <fstream>
#include <streambuf>

std::unique_ptr<CSettings> CSettings::m_pInstance(nullptr);

CSettings::CSettings()
	: m_rcDefault(100, 100, 1280, 900)
{

}

CSettings::~CSettings()
{
	//환경파일 정보 UnLoad
	UnLoad();
}

CSettings* CSettings::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CSettings());

	return m_pInstance.get();
}

bool CSettings::Load()
{
	wxString strPathFile(theUtility->GetWorkingDirectory() + SLASH + wxT("settings") + SLASH + wxT("config.json"));
	if(!DoLoad(strPathFile))
		return false;

	if(!DoSettings())
		return false;

	return true;
}

bool CSettings::DoSettings()
{
	if(!_jsonDoc.IsObject())
	{
		theUtility->ShowMessage(wxT("rapidjson::Document _jsonDoc은 Object가 아닙니다."), wxICON_ERROR | wxOK);
		return false;
	}

	if(!HasMember(wxT("globalsettings")))
	{
		theUtility->ShowMessage(m_strJsonConfigPath + wxT("파일에 globalsettings 멤버가 없습니다."), wxICON_ERROR | wxOK);
		return false;
	}

	//표시 모니터
	m_iCurrentMonitor   = _jsonDoc["globalsettings"]["DispMonitor"].GetInt();
	//최대/최소 여부
	m_iMaximized        = _jsonDoc["globalsettings"]["Maximized"].GetInt();
	//윈도우 Width
	m_iWidth            = _jsonDoc["globalsettings"]["Width"].GetInt();
	//윈도우 Height
	m_iHeight           = _jsonDoc["globalsettings"]["Height"].GetInt();
	//윈도우 xPos
	m_iXPos             = _jsonDoc["globalsettings"]["XPos"].GetInt();
	//윈도우 yPos
	m_iYPos             = _jsonDoc["globalsettings"]["YPos"].GetInt();
	//CPU정보 표시 윈도우 Height
	m_iCPUViewHeight    = _jsonDoc["globalsettings"]["CPUViewHeignt"].GetInt();
	//CPU/Memory 리스트 Width
	m_iCPUMemListWidth  = _jsonDoc["globalsettings"]["CPUMEMListWidth"].GetInt();
	//CPU코어 표시 카운터
	m_iCPUCoreDispInRow = _jsonDoc["globalsettings"]["CPUCoreDisplayInRow"].GetInt();
	//CPU사용량 표시
	m_bCPUUsageDisp     = _jsonDoc["globalsettings"]["CPUUsageDisp"].GetInt() == 1 ? true : false;
	//자동저장여부
	m_bAutoSave         = _jsonDoc["globalsettings"]["AutoSave"].GetInt() == 1 ? true : false;

	return true;
}

void CSettings::SetChnageOptionVal(const wxString& strMember, int iVal)
{
	wxString _strMember(strMember);

	rapidjson::Value _Key(_strMember.c_str(), _jsonDoc.GetAllocator());
	_jsonDoc["globalsettings"][_Key] = iVal;
}

void CSettings::SetChnageOptionVal(const wxString& strMember, const wxString& strVal)
{
	wxString _strMember(strMember);
	wxString _strVal(strVal);

	rapidjson::Value _Key(_strMember.c_str(), _jsonDoc.GetAllocator());
	rapidjson::Value _Val(_strVal.c_str(), _jsonDoc.GetAllocator());

	_jsonDoc["globalsettings"][_Key] = _Val;
}
