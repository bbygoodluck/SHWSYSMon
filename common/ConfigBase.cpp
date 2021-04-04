#include "common.h"
#include "ConfigBase.h"

bool CConfigBase::HasMember(const wxString& _strMember, const wxString& _strKey)
{
	if(_strKey.IsEmpty())
		return _jsonDoc.HasMember(_strMember.char_str());

	rapidjson::Value _Key(_strKey.c_str(), _jsonDoc.GetAllocator());
	return _jsonDoc[_Key].HasMember(_strMember.char_str());
}

bool CConfigBase::DoLoad(const wxString& strConfigFile)
{
	if(strConfigFile.IsEmpty())
	{
		theUtility->ShowMessage(wxT("환경파일을 설정하지 않았습니다."), wxICON_ERROR | wxOK);
		return false;
	}

	if (!wxFileExists(strConfigFile))
	{
		theUtility->ShowMessage(strConfigFile + wxT(" 파일이 존재하지 않습니다."), wxICON_ERROR | wxOK);
		return false;
	}

	m_strJsonConfigPath = strConfigFile;

	std::ifstream ifs;

	ifs.open(m_strJsonConfigPath.char_str());
	rapidjson::IStreamWrapper isw(ifs);

	bool bReturn = !_jsonDoc.ParseStream(isw).HasParseError();
	return bReturn;
}

void CConfigBase::UnLoad()
{
	//자동저장인경우
	if(m_bAutoSave) SaveConfig();
	//데이터 clear
	if(_jsonDoc.IsObject())
		_jsonDoc.RemoveAllMembers();
}

void CConfigBase::SaveConfig()
{
	std::ofstream ofs;

	ofs.open(m_strJsonConfigPath.char_str());

	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	_jsonDoc.Accept(writer);
}
