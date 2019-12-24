#pragma once
class CAppDirectoryUtil
{
public:
	CAppDirectoryUtil();
	~CAppDirectoryUtil();
	static CString GetCurrentDirectory(HMODULE hInstance = _hdllInstance);
	static CString GetParentDirectory(HMODULE hInstance = _hdllInstance);

};

