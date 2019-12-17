#pragma once
#include<vector>
class CTextStyleUtil
{
public:
	CTextStyleUtil();
	~CTextStyleUtil();

	static AcDbObjectId Add(const TCHAR* name, 
		const TCHAR* fontFileName = TEXT("txt.shx"),
		const TCHAR* bigFontFileName = TEXT("gbcbig.shx"));

	static AcDbObjectId GetAt(const TCHAR* name);

	static void GetAll(std::vector<CString>&textStyles);

};

