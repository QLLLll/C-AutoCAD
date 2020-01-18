#pragma once
#include<vector>
class CSelectUtil
{
public:
	CSelectUtil();
	~CSelectUtil();
	static bool PromptSelectEnts(const TCHAR *prompt, struct resbuf *rb,
		AcDbObjectIdArray &entIds);

	static bool PromptSelectEnts(const TCHAR *prompt, struct resbuf *rb,
	std::vector<AcRxClass*> &classDescs,	AcDbObjectIdArray &entIds);

	static bool PromptSelectEnts(const TCHAR *prompt, struct resbuf *rb,
		AcRxClass* &classDescs, AcDbObjectIdArray &entIds);

	static bool PromptSelectEntity(const TCHAR *prompt, 
		std::vector<AcRxClass*> &classDescs, AcDbEntity *&pEnt,AcGePoint3d &pickPoint,
		bool bOpenForWrite=true);

	static bool PromptSelectEntity(const TCHAR *prompt,
		AcRxClass *&classDescs, AcDbEntity *&pEnt, AcGePoint3d &pickPoint,
		bool bOpenForWrite = true);
};

