#ifndef JSONBASE_H_INCLUDED
#define JSONBASE_H_INCLUDED

#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/ostreamwrapper.h"
#include "../rapidjson/prettywriter.h"

//using namespace rapidjson;

class CConfigBase
{
public:
	CConfigBase() {}
	~CConfigBase() {}

	bool HasMember(const wxString& _strMember, const wxString& _strKey = wxT(""));

	const wxString& GetConfigFile() const { return m_strJsonConfigPath; }

	virtual bool Load() = 0;
	void UnLoad();
	void SaveConfig();

protected:
	bool DoLoad(const wxString& strConfigFile);

protected:
	rapidjson::Document _jsonDoc;
	wxString m_strJsonConfigPath;

	bool m_bAutoSave = false;

};
#endif // JSONBASE_H_INCLUDED
