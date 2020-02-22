#include "stdafx.h"
#include "SelectUtil.h"


CSelectUtil::CSelectUtil()
{
}


CSelectUtil::~CSelectUtil()
{
}

bool CSelectUtil::PromptSelectEnts(const TCHAR * prompt, resbuf * rb,
	AcDbObjectIdArray & entIds)
{
	ads_name selName;

	acutPrintf(prompt);

	int rc = acedSSGet(NULL, NULL, NULL, rb, selName);

	if (rc == RTNORM) {

		int length;

		acedSSLength(selName, &length);

		for (int i = 0; i < length; i++)
		{
			ads_name name;

			acedSSName(selName, i, name);

			AcDbObjectId oId;

			if (acdbGetObjectId(oId, name) == ErrorStatus::eOk) {

				entIds.append(oId);
			}
		}
		acedSSFree(selName);
		return true;
	}
	else {
		acedSSFree(selName);
		return false;
	}

}

bool CSelectUtil::PromptSelectEnts(const TCHAR * prompt, resbuf * rb, std::vector<AcRxClass*>& classDescs, AcDbObjectIdArray & entIds)
{
	ads_name selName;

	acutPrintf(prompt);

	int rc = acedSSGet(NULL, NULL, NULL, rb, selName);

	if (rc == RTNORM) {

		int length;

		acedSSLength(selName, &length);

		for (int i = 0; i < length; i++)
		{
			ads_name name;

			acedSSName(selName, i, name);

			AcDbObjectId oId;

			if (acdbGetObjectId(oId, name) == ErrorStatus::eOk) {

				AcDbEntity *ent = NULL;

				if (acdbOpenObject(ent, oId) != ErrorStatus::eOk) {

					continue;
				}

				for (int j = 0; j < (int)classDescs.size(); j++)
				{

					if (ent->isKindOf(classDescs[j])) {

						entIds.append(ent->objectId());
						break;

					}
				}

				ent->close();
			}
		}
	}
	acedSSFree(selName);
	return (entIds.length() > 0);
}

bool CSelectUtil::PromptSelectEnts(const TCHAR * prompt, resbuf * rb, AcRxClass *& classDescs, AcDbObjectIdArray & entIds)
{
	std::vector<AcRxClass*> vec;

	vec.push_back(classDescs);

	return CSelectUtil::PromptSelectEnts(prompt, rb, vec, entIds);
}

bool CSelectUtil::PromptSelectEntity(const TCHAR * prompt, std::vector<AcRxClass*>& classDescs, AcDbEntity *& pEnt, AcGePoint3d & pickPoint, bool bOpenForWrite)
{
	ads_name entName;

RETRY:if (acedEntSel(prompt, entName, asDblArray(pickPoint)) == RTNORM) {

	AcDbObjectId oId;

	if (acdbGetObjectId(oId, entName) == ErrorStatus::eOk) {

		ErrorStatus es;

		if (bOpenForWrite) {

			es = acdbOpenObject(pEnt, oId, AcDb::OpenMode::kForWrite);
		}
		else {
			es = acdbOpenObject(pEnt, oId, AcDb::OpenMode::kForRead);
		}

		if (es != ErrorStatus::eOk) {

			pEnt = NULL;
			return false;

		}
		else {
			bool ret = false;
			for (int i = 0; i < (int)classDescs.size(); i++)
			{

				if (pEnt->isKindOf(classDescs[i])) {

					ret = true;
					break;
				}
			}

			if (ret)
				return true;
			else {
				pEnt->close();
				acutPrintf(L"\n选择的实体类型不符合要求，请再次选择...");
				goto RETRY;
			}
		}

	}
	else {

		pEnt = NULL;
		return false;

	}
}
	  else {

		  pEnt = NULL;
		  return false;

	  }
}

bool CSelectUtil::PromptSelectEntity(const TCHAR * prompt, AcRxClass *& classDescs, AcDbEntity *& pEnt, AcGePoint3d & pickPoint, bool bOpenForWrite)
{
	std::vector<AcRxClass*>vec;
	vec.push_back(classDescs);

	return CSelectUtil::PromptSelectEntity(prompt, vec, pEnt, pickPoint, bOpenForWrite);
}

