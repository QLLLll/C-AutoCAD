#include "StdAfx.h"
#include "resource.h"
#include "acdocman.h"
#include"acedads.h"
#include "acedCmdNF.h"
#include<vector>
#include"MoveJig.h"

using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdMirrorApp : public AcRxArxApp {

public:
	CEcdMirrorApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void ECDMyGroupEcdRR()
	{


		ads_name ent;
		ads_point pt2, pt3;
		if (acedSSGet(NULL, NULL, NULL, NULL, ent) != RTNORM) {
			return;
		}

		if (acedGetPoint(NULL, L"Pt:", pt2) != RTNORM) {
			return;
		}
		if (acedGetPoint(pt2, L"Pt:", pt3) != RTNORM) {
			return;
		}
		ads_name n1;
		int len = 0;

		acedSSLength(ent, &len);



		for (int i = 0; i < len; i++)
		{
			ads_name a;
			acedSSName(ent, i, a);
			acedSSAdd(a, NULL, n1);
			acedCommandS(RTSTR, _T("MIRROR"),

				RTPICKS, n1,
				RTSTR, _T(""),
				RTPOINT, pt2,
				RTPOINT, pt3,
				RTSTR, _T("N"),
				RTSTR, _T(""),
				RTNONE);

			acedSSFree(n1);
		}

		acedSSFree(ent);

	}

	static void  ECDMyGroupEcdUpMR() {

		CString path = acDocManager->curDocument()->fileName();

		int lastInt = path.ReverseFind('\\');


		CString fileName = path.Mid(lastInt + 1);
		int lastDot = fileName.ReverseFind('.');

		CString newFileName;
		newFileName.Format(L"%s_M", fileName.Mid(0, lastDot));

		CString copyFile;
		copyFile.Format(L"%s_M_C", fileName.Mid(0, lastDot));

		CString  newPath;
		newPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), newFileName);

		CString  copyPath;
		copyPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), copyFile);

		AcDbObjectIdArray filterIds, otherIds;

		AcDbObjectIdArray oIds = GetSel();
		AcGePoint3d formPt, toPt;
		bool findPt = false;
		for (int i = 0; i < oIds.length(); i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, oIds[i], AcDb::kForRead) == Acad::eOk) {

				CString appName = "XDataDCZ";
				resbuf * rb = l->xData(appName);

				if (rb != NULL) {

					resbuf *tempRs = rb;

					tempRs = tempRs->rbnext;


					int flag = tempRs->resval.rint;
					tempRs = tempRs->rbnext;
					if (flag == 1) {
						formPt = asPnt3d(tempRs->resval.rpoint);
						tempRs = tempRs->rbnext;
						toPt = asPnt3d(tempRs->resval.rpoint);
						acutRelRb(rb);
						findPt = true;
						break;
					}
					acutRelRb(rb);

				}
				l->close();
			}
		}

		if (!findPt) {
			acutPrintf(L"没有找到对称轴数据");
			return;
		}


		for (int i = 0; i < oIds.length(); i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, oIds[i], AcDb::kForRead) == Acad::eOk) {

				if (l->isA() == AcDbBlockReference::desc()) {

					l->close();
					continue;
				}
				else {
					AcDbDimension *dim = NULL;
					AcDbText *txt = NULL;
					AcDbMText *mTxt = NULL;

					if (l->layerId().isNull() == false)
					{
						otherIds.append(l->layerId());

					}
					if (l->linetypeId().isNull() == false) {
						otherIds.append(l->linetypeId());
					}

					dim = AcDbDimension::cast(l);

					if (dim != NULL) {
						if (dim->dimensionStyle().isNull() == false) {
							otherIds.append(dim->dimensionStyle());
						}
					}
					else if (l->isA() == AcDbText::desc())
					{
						txt = AcDbText::cast(l);

						if (txt->textStyle().isNull() == false) {

							otherIds.append(txt->textStyle());
						}

					}
					else if (l->isA() == AcDbMText::desc())
					{
						mTxt = AcDbMText::cast(l);

						if (mTxt->textStyle().isNull() == false) {

							otherIds.append(mTxt->textStyle());
						}
					}

					filterIds.append(oIds[i]);
					l->close();
				}


			}
		}

		CMoveJig jig(formPt, toPt);

		jig.UpdateDoIt(filterIds, true);

		if (jig.m_idsC.length() == 0) {
			return;
		}
		acutPrintf(L"\notherids=%d\n", otherIds.length());
		otherIds.append(jig.m_idsC);

		CopyFile(newPath, copyPath, TRUE);

		DeleteFile(newPath);

		otherIds.append(jig.m_idsC);

		if (DeepClone(otherIds, newPath))
		{
			DeleteFile(copyPath);
		}
		else {
			CopyFile(copyPath, newPath, FALSE);

			DeleteFile(copyPath);
		}



		//DeepClone(otherIds, newPath);
		/*AcDbDatabase *pDb = new AcDbDatabase(false,true);

		ErrorStatus es = pDb->readDwgFile(newPath, AcDbDatabase::kForReadAndWriteNoShare);



		if (es != ErrorStatus::eOk) {

			delete pDb;
			pDb = NULL;

			acutPrintf(L"镜像文件被删除，请重新镜像之后，在更新");
			return;

		}

		for (int i=0;i<jig.m_idsC.length();i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, jig.m_idsC[i], AcDb::kForWrite) == Acad::eOk) {

				AcDbEntity * lC = NULL;

				lC = AcDbEntity::cast(l->clone());


				PostToModelSpace(lC, pDb);

				l->close();

			}


		}


		es=pDb->closeInput(Adesk::kTrue);

		es=pDb->saveAs(newPath);

		delete pDb;

		pDb = NULL;*/
		

		/*	AcDbBlockTable *pBlockTable;
			pDb->getSymbolTable(pBlockTable, AcDb::kForRead);

			AcDbBlockTableRecord *pBlkTblRcd;
			pBlockTable->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
			pBlockTable->close();*/





			//创建块表记录遍历器
			//AcDbBlockTableRecordIterator *pItr; //块表记录遍历器
			//pBlkTblRcd->newIterator(pItr);
			//pBlkTblRcd->close();
			//AcDbEntity *pEnt = NULL; //遍历的临时实体指针
			//for (pItr->start(); !pItr->done(); pItr->step()) {
			//	//利用遍历器获得每一个实体
			//	pItr->getEntity(pEnt, AcDb::kForWrite);

			//	pEnt->erase();
			//	pEnt->close();
			//}
	}


	static void ECDMyGroupEcdMR() {


		CString path = acDocManager->curDocument()->fileName();

		int lastInt = path.ReverseFind('\\');


		CString fileName = path.Mid(lastInt + 1);
		int lastDot = fileName.ReverseFind('.');

		CString newFileName;
		newFileName.Format(L"%s_M", fileName.Mid(0, lastDot));

		CString  newPath;
		newPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), newFileName);

		//acutPrintf(dir);

		AcDbObjectIdArray filterIds, otherIds;

		AcDbObjectIdArray oIds = GetSel();

		ads_point pt1;

		AcGePoint3d fPt;

		if (acedGetPoint(NULL, L"轴线第一个点：\n", pt1) != RTNORM) {
			return;
		}


		fPt = asPnt3d(pt1);

		//vector<AcDbEntity*>vecEnts;
		for (int i = 0; i < oIds.length(); i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, oIds[i], AcDb::kForRead) == Acad::eOk) {

				if (l->isA() == AcDbBlockReference::desc()) {

					l->close();
					continue;
				}
				else {
					AcDbDimension *dim = NULL;
					AcDbText *txt = NULL;
					AcDbMText *mTxt = NULL;

					if (l->layerId().isNull() == false)
					{
						otherIds.append(l->layerId());

					}
					if (l->linetypeId().isNull() == false) {
						otherIds.append(l->linetypeId());
					}

					dim = AcDbDimension::cast(l);

					if (dim != NULL) {
						if (dim->dimensionStyle().isNull() == false) {
							otherIds.append(dim->dimensionStyle());
						}
					}
					else if (l->isA() == AcDbText::desc())
					{
						txt = AcDbText::cast(l);

						if (txt->textStyle().isNull() == false) {

							otherIds.append(txt->textStyle());
						}

					}
					else if (l->isA() == AcDbMText::desc())
					{
						mTxt = AcDbMText::cast(l);

						if (mTxt->textStyle().isNull() == false) {

							otherIds.append(mTxt->textStyle());
						}
					}

					filterIds.append(oIds[i]);
					l->close();
				}


			}
		}


		CMoveJig jig(fPt);

		jig.doIt(filterIds, true);

		//acutPrintf(L"\n%d", jig.m_idsC.length());

		if (jig.m_idsC.length() == 0) {
			return;
		}

		acutPrintf(L"\notherids=%d\n", otherIds.length());
		otherIds.append(jig.m_idsC);

		DeepClone(otherIds, newPath);

		//jig.m_ToPoint

		for (int i = 0; i < oIds.length(); i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, oIds[i], AcDb::kForWrite) == Acad::eOk) {

				CString appName = "XDataDCZ";

				resbuf * rb = l->xData(appName);

				if (rb == NULL) {

					acdbRegApp(appName);

				}
				else {
					rb = NULL;
					l->setXData(rb);
				}

				rb = acutBuildList(AcDb::kDxfRegAppName, appName,
					AcDb::kDxfXdInteger16, 1,
					AcDb::kDxfXdWorldXCoord, pt1,
					AcDb::kDxfXdWorldXCoord, asDblArray(jig.m_ToPoint),
					RTNONE);

				l->setXData(rb);

				acutRelRb(rb);
				l->close();

			}
		}

	}

	static AcDbObjectIdArray GetSel() {
		ads_name aName;
		AcDbObjectIdArray ids;
		if (acedSSGet(NULL, NULL, NULL, NULL, aName) != RTNORM) {
			return ids;
		}

		int len = 0;
		acedSSLength(aName, &len);

		for (int i = 0; i < len; i++)
		{
			ads_name temp;

			acedSSName(aName, i, temp);

			AcDbObjectId tempId;
			acdbGetObjectId(tempId, temp);

			ids.append(tempId);

		}
		acedSSFree(aName);
		return ids;
	}

	static  bool DeepClone(const AcDbObjectIdArray & ids, CString path)
	{
		ErrorStatus es = ErrorStatus::eOk;
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);

		AcDbObjectId  modelSpaceId, paperSpaceId;


		AcDbBlockTable *pBT1 = NULL;


		AcDbDatabase* pTempDb = new AcDbDatabase();

		AcDbIdMapping idMap;
		es = idMap.setDestDb(pTempDb);

		if (!CopyTextStyleIdInfo(acdbHostApplicationServices()->workingDatabase(), pTempDb)) {

			return false;
		}

		pTempDb->getSymbolTable(pBT1, AcDb::kForRead);
		pBT1->getAt(ACDB_MODEL_SPACE, modelSpaceId);
		pBT1->close();

		es = pTempDb->wblockCloneObjects(ids, modelSpaceId, idMap, AcDb::kDrcIgnore);
		//e = pTempDb->deepCloneObjects(ids, modelSpaceId, idMap);
		if (es != ErrorStatus::eOk) {
			return false;
		}
		es = pTempDb->saveAs(path);
		if (es != ErrorStatus::eOk) {
			return false;
		}
		return true;




	}

	static	bool CopyTextStyleIdInfo(AcDbDatabase *pFromDataSrc/*in*/, AcDbDatabase *pToDataDes/*in*/)
	{

		if (pFromDataSrc == NULL || pToDataDes == NULL)
			return false;

		AcDbTextStyleTable *pStyleTable = NULL, *pNewSt = NULL;
		Acad::ErrorStatus es = Acad::eOk;
		es = pFromDataSrc->getSymbolTable(pStyleTable, AcDb::kForRead);
		if (es != Acad::eOk)
			return false;

		es = pToDataDes->getSymbolTable(pNewSt, AcDb::kForWrite);
		if (es != Acad::eOk)
			return false;




		AcDbTextStyleTableIterator *pIterator = NULL;
		es = pStyleTable->newIterator(pIterator);
		if (es != Acad::eOk)
		{
			pStyleTable->close();
			pStyleTable = NULL;
			return false;
		}

		AcDbObjectId stdId, anoId;
		pStyleTable->getAt(L"Standard", stdId);
		pStyleTable->getAt(L"Annotative", anoId);

		for (pIterator->start(); !pIterator->done(); pIterator->step())
		{
			AcDbObjectId styleId = AcDbObjectId::kNull;
			if ((es = pIterator->getRecordId(styleId)) == Acad::eOk)
			{
				AcDbTextStyleTableRecord* pNewRec = NULL;
				if (styleId == stdId) {

					pNewSt->getAt(L"Standard", pNewRec, AcDb::kForWrite);

				}
				else if (styleId == anoId) {
					pNewSt->getAt(L"Annotative", pNewRec, AcDb::kForWrite);
				}
				else {
					pNewRec = new AcDbTextStyleTableRecord;
				}

				AcGiTextStyle *pTextStyle = new AcGiTextStyle(pToDataDes);
				if ((es = fromAcDbTextStyle(*pTextStyle, styleId)) == Acad::eOk)
				{
					ACHAR * pTypeface = NULL;
					Adesk::Boolean bold;
					Adesk::Boolean italic;
					Charset charset;
					Autodesk::AutoCAD::PAL::FontUtils::FontPitch pitchAndFamily;
					Autodesk::AutoCAD::PAL::FontUtils::FontFamily fontFamily;



					if (styleId != stdId&&styleId != anoId) {
						setSymbolName(pNewRec, pTextStyle->styleName());
						pNewRec->setFileName(pTextStyle->fileName());
						pNewRec->setBigFontFileName(_T(""));
					}


					/*待完善*/

					es = pTextStyle->font(pTypeface, bold, italic, charset, pitchAndFamily, fontFamily);
					if (es == Acad::eOk)
						pNewRec->setFont(pTypeface, bold, italic, charset, pitchAndFamily, fontFamily);

					// must explicitly set to ""
					if (styleId != stdId&&styleId != anoId)
					{
						pNewRec->setTextSize(pTextStyle->textSize());

						pNewRec->setXScale(pTextStyle->xScale());
					}
					pNewRec->setObliquingAngle(pTextStyle->obliquingAngle());

					if (styleId == stdId || styleId == anoId) {
						pNewRec->close();

					}
					else {
						addToSymbolTableAndClose(pNewRec, pToDataDes);
					}
				}
				if (pTextStyle != NULL)
				{
					delete pTextStyle;
					pTextStyle = NULL;
				}
			}
		}
		pNewSt->close();
		if (pIterator != NULL)
		{
			delete pIterator;
			pIterator = NULL;
			pStyleTable->close();
			pStyleTable = NULL;
		}
		return true;
	}

	static BOOL setSymbolName(AcDbSymbolTableRecord* newRec, LPCTSTR newName)
	{
		Acad::ErrorStatus es;
		es = newRec->setName(newName);
		if (es != Acad::eOk) {
			newRec->close();
		}
		return(es);
	}
	//添加文字样式块表记录
	static BOOL addToSymbolTableAndClose(AcDbSymbolTableRecord* systemTextRec/*in*/, AcDbDatabase *pDataBase/*in*/)
	{
		if (pDataBase == NULL || systemTextRec == NULL)
			return FALSE;

		AcDbTextStyleTable* symTextTbl = NULL;
		Acad::ErrorStatus es = Acad::eOk;
		es = pDataBase->getTextStyleTable(symTextTbl, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			if (systemTextRec != NULL)
			{
				delete systemTextRec;
				systemTextRec = NULL;
			}
			return FALSE;
		}
		AcDbSymbolTable *pSysRec = AcDbTextStyleTable::cast(symTextTbl);
		//覆盖字体样式
		es = pSysRec->add(systemTextRec);
		if (es != Acad::eOk)
		{
			symTextTbl->close();
			systemTextRec->close();
			return FALSE;
		}
		else
		{
			systemTextRec->close();
			symTextTbl->close();
		}
		return TRUE;
	}

	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase()) {

		if (pEnt == NULL) {
			return AcDbObjectId::kNull;
		}
		AcDbBlockTable *pTable;

		AcDbBlockTableRecord *blkTblRec;

		AcDbObjectId objId;

		pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
		pTable->getAt(ACDB_MODEL_SPACE, blkTblRec, AcDb::OpenMode::kForWrite);

		pTable->close();

		Acad::ErrorStatus status = blkTblRec->appendAcDbEntity(objId, pEnt);

		if (status != Acad::eOk) {

			blkTblRec->close();

			delete pEnt;
			pEnt = NULL;

			return AcDbObjectId::kNull;

		}

		blkTblRec->close();
		pEnt->close();

		return objId;
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdMirrorApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, EcdMR, EcdMR, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, EcdRR, EcdRR, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, EcdUpMR, EcdUpMR, ACRX_CMD_MODAL, NULL)

