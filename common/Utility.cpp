#include "common.h"
#include "Utility.h"

std::unique_ptr<CUtility> CUtility::m_pInstance = nullptr;

CUtility* CUtility::Get()
{
	if (m_pInstance.get() == nullptr)
	{
		m_pInstance.reset(new CUtility());
		m_pInstance->SetWorkingDirectory();
	}

	return m_pInstance.get();
}

void CUtility::SetWorkingDirectory()
{
	if(m_strWorkingDirectory.IsEmpty())
		m_strWorkingDirectory = wxGetCwd();

#ifndef NDEBUG
	m_strWorkingDirectory += SLASH;
	m_strWorkingDirectory += wxT("src");
#endif // __WXDEBUG__
}

const wxString& CUtility::GetWorkingDirectory() const
{
	return m_strWorkingDirectory;
}

void CUtility::ShowMessage(const wxString& strMessage, long style)
{
	wxMessageBox(strMessage, PROGRAM_FULL_NAME, style);
}

void CUtility::ShowMessage(const wxString& strMessage, long style, int& iRet)
{
	iRet = wxMessageBox(strMessage, PROGRAM_FULL_NAME, style);
}

#ifndef NDEBUG
void CUtility::LogDisplay(const wxString& strMsg)
{
#if defined(_UNICODE) || defined(UNICODE)
	std::wcout << strMsg << std::endl;
#else
	std::cout << strMsg << std::endl;
#endif
}
#endif

void CUtility::LoadImageList()
{
#ifdef __WXMSW__
	// IID_IImageList {46EB5926-582E-4017-9FDF-E8998DAA0950}
	static const GUID IID_IImageList = { 0x46EB5926, 0x582E, 0x4017, { 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x9, 0x50 } };
	// IID_IImageList2 {192B9D83-50FC-457B-90A0-2B82A8B5DAE1}
	static const GUID IID_IImageList2 = { 0x192B9D83, 0x50FC, 0x457B, { 0x90, 0xA0, 0x2B, 0x82, 0xA8, 0xB5, 0xDA, 0xE1 } };

	_gImageList = nullptr;
	HRESULT res = SHGetImageList(SHIL_SMALL, IID_IImageList2, (void**)& _gImageList);
	if (FAILED(res))
		res = SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)& _gImageList);
#else
	_gImageList = wxTheFileIconsTable->GetSmallImageList();
#endif
}

void CUtility::GetIconIndexFromImageList(const wxString& strFullPath, int &iIconIndex, int& iOverlayIndex)
{
#ifdef __WXMSW__
	iIconIndex = 0;
	iOverlayIndex = 0;

	SHFILEINFO sfi;
	wxZeroMemory(sfi);

	DWORD dwNum = GetFileAttributes(strFullPath);
	if(dwNum != INVALID_FILE_ATTRIBUTES)
	{
		SHGetFileInfo(strFullPath, dwNum, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_OVERLAYINDEX);

		iIconIndex = (sfi.iIcon & 0x00FFFFFF);
		iOverlayIndex = (sfi.iIcon >> 24) - 1;

		DestroyIcon(sfi.hIcon);
	}
#else
#endif
}

void CUtility::SetComma(const wxString& strSource, wxString& strRet)
{
	wxString strTmp(strSource);
	wxString strReturn = wxT("");
	int nLen = strTmp.Len();

	if (nLen > 3)
	{
		int nRestItemCount = nLen % 3;
		int nCommaCount = (nLen / 3);

		if (nRestItemCount == 0)
			nCommaCount -= 1;

		if (nCommaCount == 0)
			strReturn = strTmp;
		else
		{
			if (nRestItemCount != 0)
				strReturn = strTmp.Left(nRestItemCount) + wxT(",");

			wxString strNext = strTmp.Mid(nRestItemCount, nLen);
			nLen = strNext.Len();

			for (int nCnt = 0; nCnt < nLen; nCnt = nCnt + 3)
			{
				strReturn += strNext.Mid(nCnt, 3);
				if ((nCnt + 3) < (nLen - 1))
					strReturn += wxT(",");
			}
		}
	}
	else
		strReturn = strTmp;

	strRet = strReturn;
}
