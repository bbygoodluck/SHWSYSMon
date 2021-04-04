#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

class CUtility
{
private:
	explicit CUtility() {};

public:
	~CUtility() {};

	static CUtility* Get();
	const wxString& GetWorkingDirectory() const;
	void ShowMessage(const wxString& strMessage, long style);
	void ShowMessage(const wxString& strMessage, long style, int& iRet);
	void LoadImageList();
	void GetIconIndexFromImageList(const wxString& strFullPath, int &iIconIndex, int& iOverlayIndex);
	void SetComma(const wxString& strSource, wxString& strRet);
#ifndef NDEBUG
	void LogDisplay(const wxString& strMsg);
#endif
private:
	void SetWorkingDirectory();

private:
	static std::unique_ptr<CUtility> m_pInstance;
	wxString m_strWorkingDirectory = wxT("");
};

#endif // UTILITY_H_INCLUDED
