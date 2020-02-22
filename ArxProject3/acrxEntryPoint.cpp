//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"SelectUtil.h"
#include"BlockUtil.h"
#include"DwgDataBaseUtil.h"
#include"AppDirectoryUtil.h"
#include"PolylineUtil.h"
#include"MathUtil.h"
#include"MFCDialog.h"
#include"ArxDialog.h"
#include"ModelessDialog.h"
#include"resource.h"
#include"DimMirror.h"
#include"DatabaseJigEntity.h"
#include"MoveJig.h"
#include"DrawRecJig.h"
//#include"MyTuBao.h"
#include"ConvertUtil.h"
#include"RegionUtil.h"
#include"FindTextAndBlock.h"
CModelessDialog *pDialog = NULL;
BOOL CloseDialog() {

	if (pDialog == NULL)return TRUE;

	BOOL bRet = pDialog->DestroyWindow();

	if (bRet) {

		pDialog = NULL;
	}
	return bRet;
}


//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject3App : public AcRxArxApp {

public:
	
	 

	CArxProject3App () : AcRxArxApp () { }

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void ViewXData() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n请选择实体：\n", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";

			resbuf * rb = pEnt->xData(appName);

			if (rb != NULL) {

				resbuf *tempRs = rb;

				tempRs = tempRs->rbnext;

				acutPrintf(L"字符串:%s\n", tempRs->resval.rstring);
				tempRs = tempRs->rbnext;
				acutPrintf(L"整型：%d\n", tempRs->resval.rint);
				tempRs = tempRs->rbnext;
				acutPrintf(L"浮点型：%f\n", tempRs->resval.rreal);
				tempRs = tempRs->rbnext;
				acutPrintf(L"坐标数据：(%.2f,%.2f,%.2f)\n", tempRs->resval.rpoint[X],
					tempRs->resval.rpoint[Y],
					tempRs->resval.rpoint[Z]);

				acutRelRb(rb);
				
			}
			pEnt->close();
		}
	}

	static void DeleteXData() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n请选择要删除扩展数据的实体：\n", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";

			resbuf * rb = acutBuildList(AcDb::kDxfRegAppName, appName, RTNONE);

			pEnt->setXData(rb);

			acutRelRb(rb);

			pEnt->close();

		}
	}

	static void AddXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n请选择实体：\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";

			pEnt->createExtensionDictionary();

			AcDbObjectId dicId = pEnt->extensionDictionary();

			AcDbDictionary *myDic = NULL;

			if (acdbOpenAcDbObject((AcDbObject*&)myDic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				AcDbXrecord *record = new AcDbXrecord();

				AcDbObjectId xRecId;

				myDic->setAt(appName, record, xRecId);

				resbuf * rb= acutBuildList(
					AcDb::kDxfText, L"字符串测试数据",
					AcDb::kDxfInt16, 2,
					AcDb::kDxfReal, 3.14,
					AcDb::kDxfXCoord, asDblArray(pickPoint),
					RTNONE);
				
				record->setFromRbChain(*rb);

				acutRelRb(rb);
				record->close();
				myDic->close();
				pEnt->close();


				acutPrintf(L"\n添加扩展记录成功");
			}




		}


	}

	static void ViewXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n请选择实体：\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";
		
			AcDbObjectId dicId = pEnt->extensionDictionary();
			pEnt->close();
			if (dicId.isNull()==false) {

				AcDbDictionary *dic = NULL;

				if (acdbOpenObject(dic, dicId, AcDb::kForRead) == ErrorStatus::eOk) {


					AcDbXrecord * record = NULL;

					dic->getAt(appName, record);

						if (record!=NULL) {

							resbuf * rb = NULL;

							record->rbChain(&rb);

							record->close();
							dic->close();

							if (rb != NULL) {

								resbuf *tempRs = rb;

								acutPrintf(L"字符串:%s\n", tempRs->resval.rstring);
								tempRs = tempRs->rbnext;
								acutPrintf(L"整型：%d\n", tempRs->resval.rint);
								tempRs = tempRs->rbnext;
								acutPrintf(L"浮点型：%f\n", tempRs->resval.rreal);
								tempRs = tempRs->rbnext;
								acutPrintf(L"坐标数据：(%.2f,%.2f,%.2f)\n", tempRs->resval.rpoint[X],
									tempRs->resval.rpoint[Y],
									tempRs->resval.rpoint[Z]);

								acutRelRb(rb);

							}
							
						}
					}
					else {

						acutPrintf(L"\n该字典无这个名称的扩展记录。\n");

					}
				}			
			else {
				acutPrintf(L"\n该实体无扩展数据。\n");

			}

			pEnt->close();
		}


	}

	static void DeleteXDicRecord() {

		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n请选择实体：\n", cls, pEnt, pickPoint)) {

			CString appName = "xRecord";

			AcDbObjectId dicId = pEnt->extensionDictionary();
			pEnt->close();
			if (dicId.isNull() == false) {
				AcDbDictionary *dic = NULL;
				if (acdbOpenObject(dic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				ErrorStatus es=	dic->remove(appName);

				if (es == ErrorStatus::eOk) {

					acutPrintf(L"\ndelete success");

				}
				else if (es == ErrorStatus::eKeyNotFound) {
					acutPrintf(L"\nKeyNotFound");

				}
				else {
					acutPrintf(L"\ndelete fail");
				}
				dic->close();
				}
			
			}
		}

	}

	static void AddNamedDic() {
		AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		CString dicName = "MyDic";

		if (CSelectUtil::PromptSelectEntity(L"\n请选择实体：\n", cls, pEnt, pickPoint)) {
		
			pEnt->close();

			AcDbDictionary *pNameDic = NULL;

			acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

			AcDbDictionary *dic = NULL;

			AcDbObjectId dicId;
			
			if (pNameDic->getAt(dicName, dic,AcDb::kForWrite) == ErrorStatus::eKeyNotFound) {

				dic = new AcDbDictionary();

				pNameDic->setAt(dicName, dic, dicId);

				dic->close();
			}
			pNameDic->close();

			if (acdbOpenObject(dic, dicId, AcDb::kForWrite) == ErrorStatus::eOk) {

				AcDbLayerTableRecord *layerRec = new AcDbLayerTableRecord();

				AcCmColor color;
				color.setColorIndex(20);
				layerRec->setColor(color);

				layerRec->setIsOff(true);
				layerRec->setIsFrozen(true);
				
				AcDbObjectId layerId;
				Acad::ErrorStatus es = dic->setAt(dicName, layerRec, layerId);

				layerRec->close();
				dic->close();



			}

		
		
		}
	}

	static void ViewNamedDic() {

		CString dicName = "MyDic";

		AcDbDictionary *pNameDic = NULL;

		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

		AcDbDictionary *dic = NULL;

		AcDbObjectId dicId;


		ErrorStatus es = pNameDic->getAt(dicName, dic);
		if ( es== ErrorStatus::eKeyNotFound) {
			acutPrintf(L"\nfirst KeyNotFound");

		}
		else if (es == ErrorStatus::eOk) {

			AcDbLayerTableRecord *layRec = NULL;

			dic->getAt(dicName, layRec);

			dic->close();
			pNameDic->close();

			if (layRec == NULL) {
				acutPrintf(L"\nsecond KeyNotFound");
			}
			else {

				acutPrintf(L"\n颜色：%d", layRec->color().colorIndex());
				acutPrintf(L"\n是否关闭:%s", layRec->isOff() ? L"是" : L"否");
				acutPrintf(L"\n是否冻结：%s", layRec->isFrozen() ? L"是" : L"否");
				layRec->close();

			}
		}
	}

	static void DeleteNamedDic() {

		CString dicName = "MyDic";
		AcDbDictionary *pNameDic = NULL;

		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameDic, AcDb::kForWrite);

		AcDbDictionary *dic = NULL;

		AcDbObjectId dicId;


		ErrorStatus es = pNameDic->getAt(dicName, dic);
		if (es == ErrorStatus::eOk) {

			ErrorStatus es2 = pNameDic->remove(dicName);

			if (es2 == ErrorStatus::eOk) {

				acutPrintf(L"\ndelete success");

			}
			else if (es2 == ErrorStatus::eKeyNotFound) {
				acutPrintf(L"\nKeyNotFound");

			}
			else {
				acutPrintf(L"\ndelete fail");
			}
			dic->close();
		}
		pNameDic->close();
	}

	static void AddGroup() {

		AcDbObjectIdArray entIds;

		AcRxClass *clas = AcDbEntity::desc();

		if (CSelectUtil::PromptSelectEnts(L"\n請選擇實體：", NULL, clas, entIds)) {

			AcDbGroup *pGroup = new AcDbGroup();

			for (int i = 0; i < entIds.length (); i++)
			{
				pGroup->append(entIds[i]);
			}
			AcDbDictionary *pGroupDict = NULL;

			acdbHostApplicationServices()->workingDatabase()
				->getGroupDictionary(pGroupDict, AcDb::kForWrite);

			CString groupName = "*";
			AcDbObjectId groupId;
			pGroupDict->setAt(groupName, pGroup, groupId);

			pGroupDict->close();
			pGroup->close();
		}
	}

	static void DeleteGroup() {

		AcDbObjectIdArray entIds;
		AcRxClass *clas = AcDbEntity::desc();

		if (CSelectUtil::PromptSelectEnts(L"\n選擇要分解的組中的實體：", NULL, entIds)) {

			AcDbDictionary *pGroupDict = NULL;
			acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(
				pGroupDict, AcDb::kForWrite);
			
			AcDbDictionaryIterator *it = pGroupDict->newIterator();

			for (; !it->done(); it->next()) {

				AcDbGroup *pGroup = NULL;

				if (it->getObject((AcDbObject*&)pGroup, AcDb::kForWrite)== Acad::eOk) {

					AcDbObjectIdArray groupEntIds;
					pGroup->allEntityIds(groupEntIds);
					bool bFound = false;

					for (int i = 0; i < entIds.length(); i++) {

						if (groupEntIds.contains(entIds[i])) {

							bFound = true;
							break;
						}
					}
					if (bFound) {

						pGroup->erase();
						acutPrintf(L"delete success");
					}
					pGroup->close();
				}
			}
			pGroupDict->close();
		}
	}

	static void CreateDwgFile() {

		CString fileName = L"D:\\Users\\liu.qiang\\Desktop\\cad图纸\\New.Dwg";

		AcDbDatabase *pDb = new AcDbDatabase(true, false);

		BlockUtil::CreateBlk(pDb);

		pDb->saveAs(fileName);

		delete pDb;

	}
	
	static void ReadDwgFile() {

		CString fileName = L"D:\\Users\\liu.qiang\\Desktop\\cad图纸\\New.Dwg";

		AcDbDatabase *pDb = new AcDbDatabase(false);

		pDb->readDwgFile(fileName, AcDbDatabase::kForReadAndReadShare);

		AcDbObjectIdArray oIds = CDwgDataBaseUtil::GetAllEntityIds(NULL, pDb);


		for (AcDbObjectId oId : oIds) {

			AcDbEntity* pEnt = NULL;

			if (acdbOpenObject(pEnt, oId, AcDb::kForRead) == Acad::eOk) {

				acutPrintf(L"類名稱：%s\n", pEnt->isA()->name());
			}
			else {

				acutPrintf(L"open fail");

			}
			pEnt->close();
		}
	}

	/*static void NewDocDrawEnts() {


		acDocManager->lockDocument(acDocManager->curDocument());

		AcDbObjectIdArray allEntIds = CDwgDataBaseUtil::GetAllEntityIds(NULL,acdbHostApplicationServices()
			->workingDatabase());
		int lineCount = 0;

		for (int i = 0; i < allEntIds.length(); i++)
		{

			AcDbLine *pLine = NULL;

			if (acdbOpenObject(pLine, allEntIds[i], AcDb::kForRead) == Acad::eOk) {


				lineCount++;
				pLine->close();
			}
		}

		acDocManager->unlockDocument(acDocManager->curDocument());

		Acad::ErrorStatus es = acDocManager->appContextNewDocument(NULL);

		acDocManager->lockDocument(acDocManager->curDocument());

		for (int i = 0; i < lineCount; i++) {

			double y = i * 20;

			AcGePoint3d startPoint(0, y, 0);
			AcGePoint3d endPoint(100, y, 0);
			AcDbLine *line = new AcDbLine(startPoint, endPoint);

			CDwgDataBaseUtil::PostToModelSpace(line);

			line->close();

		}
	
		acDocManager->unlockDocument(acDocManager->curDocument());


	}

	static void SaveDwgOpenDoc() {
	
		acDocManager->lockDocument(acDocManager->curDocument());

		AcDbObjectIdArray allEntIds = CDwgDataBaseUtil::GetAllEntityIds(NULL, acdbHostApplicationServices()
			->workingDatabase());

		int lineCount = 0, circleCount = 0;


		for (int i = 0; i < allEntIds.length(); i++) {

			AcDbEntity *pEnt = NULL;

			if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk) {

				if (pEnt->isKindOf(AcDbLine::desc())) {

					lineCount++;
				}
				else if (pEnt->isKindOf(AcDbCircle::desc())) {
					
					circleCount++;

				}

				pEnt->close();
			}
		}

		AcDbDatabase *pDbl = new AcDbDatabase(true, false);


		for (int i = 0; i < lineCount; i++) {

			double y = i * 20;

			AcGePoint3d startPoint(0, y, 0);
			AcGePoint3d endPoint(100, y, 0);
			AcDbLine *line = new AcDbLine(startPoint, endPoint);

			CDwgDataBaseUtil::PostToModelSpace(line,pDbl);

			line->close();

		}

		CString fileName1 = CAppDirectoryUtil::GetCurrentDirectoryW() + TEXT("\\db1.dwg");

		pDbl->saveAs(fileName1);

		delete pDbl;
	
		AcGeVector3d normal(0, 0, 1);

		AcDbDatabase *pDb2 = new AcDbDatabase(true, false);

		for (int i = 0; i < circleCount; i++) {

			double y = i * 20;

			AcGePoint3d center(0, y, 0);

			AcDbCircle *c = new AcDbCircle(center, normal, 5);

			CDwgDataBaseUtil::PostToModelSpace(c, pDb2);
			c->close();
		}
		CString fileName2 = CAppDirectoryUtil::GetCurrentDirectoryW() + TEXT("\\db2.dwg");

		pDb2->saveAs(fileName2);

		delete pDb2;

		acDocManager->unlockDocument(acDocManager->curDocument());

		Acad::ErrorStatus es = acDocManager->appContextOpenDocument(fileName1);
		es = acDocManager->appContextOpenDocument(fileName2);

	
	}
	*/

	static void ECDMyGroupMyXData() {
		//创建Xdata数据
		/*AcDbEntity *pEnt = NULL;

		AcRxClass *cls = AcDbEntity::desc();

		AcGePoint3d pickPoint;

		if (CSelectUtil::PromptSelectEntity(L"\n选择实体：", cls, pEnt, pickPoint)) {

			CString appName = "xDataApp";
			acdbRegApp(appName);

			struct resbuf *rb = acutBuildList(AcDb::kDxfRegAppName, appName,
				AcDb::kDxfXdAsciiString, L"字符串测试数据",
				AcDb::kDxfXdInteger16, 2,
				AcDb::kDxfXdReal, 3.14,
				AcDb::kDxfXdWorldXCoord, asDblArray(pickPoint),
				RTNONE);

			pEnt->setXData(rb);

			acutRelRb(rb);

			acutPrintf(L"\n添加扩展数据成功");

			pEnt->close();

		}*/
		
		//ViewXData();

		//DeleteXData();

	}
	static void ECDMyGroupMyXRec() {

		//AddXDicRecord();
		//ViewXDicRecord();
		//DeleteXDicRecord();
		//AddNamedDic();
		//ViewNamedDic();
		DeleteNamedDic();
	}
	static void ECDMyGroupMyAddGroup() {
		AddGroup();
	}
	static void ECDMyGroupMyDelGroup() {
		DeleteGroup();
	}
	static void ECDMyGroupMyCreateDwgFile() {
	
		CreateDwgFile();
	}
	static void ECDMyGroupMyReadFile() {
		ReadDwgFile();
	
	}
	static void ECDMyGroupMyFileToFile() {

		//BlockUtil::CopyBlockDefFromOtherDwg(L"D:\\Users\\liu.qiang\\Desktop\\cad图纸\\New.Dwg", L"ABCD");
		BlockUtil::InsertDwgBlockDef(L"D:\\Users\\liu.qiang\\Desktop\\cad图纸\\New.Dwg", L"LL_ABCD", true);

	}
	static void ECDMyGroupMyNewDoc() {
		//NewDocDrawEnts();
		//SaveDwgOpenDoc();
	}
	static void ECDMyGroupMyTestDotInPolyline() {

		int count = 100000;

		AcDbEntity* pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *cls = AcDbEntity::desc();

		if (CSelectUtil::PromptSelectEntity(L"请选择需要进行测试的闭合多段线：", cls, pEnt, pickPoint)) {

			AcDbExtents ext;
			pEnt->getGeomExtents(ext);
			double margin = 10;
			double xmin = ext.minPoint().x - margin;
			double ymin = ext.minPoint().y - margin;
			double xSpan = ext.maxPoint().x - ext.minPoint().x + 2 * margin;
			double ySpan = ext.maxPoint().y - ext.minPoint().y + 2 * margin;

			AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
			srand((unsigned)time(NULL));

			for (int i = 0; i < count; i++) {

				int maxRand = 100000;
				int xRand = CMathUtil::GetRand(0, maxRand);
				int yRand = CMathUtil::GetRand(0, maxRand);

				double x = xmin + ((double)xRand / maxRand)*xSpan;
				double y = ymin + ((double)yRand / maxRand)*ySpan;

				int relation = CPolylineUtil::PtRelationToPoly(pPoly, AcGePoint2d(x, y), 1.0E-4);

				int colorIndex = 0;

				switch (relation) {

				case -1:
					colorIndex = 1;
					break;
				case 0:
					colorIndex = 5;
					break;
				case 1:
					colorIndex = 6;
					break;
				default:
					break;
				}
				AcDbPoint *pPoint = new AcDbPoint(AcGePoint3d(x, y, 0));
				pPoint->setColorIndex(colorIndex);
				CDwgDataBaseUtil::PostToModelSpace(pPoint);

				pPoint->close();

			}
			pEnt->close();
		}

	}
	
	/*static void ECDMyGroupMySetDwgScale() {

		CString DWG_SCALE_KEY = L"DwgScale";

		AcDbDictionary *pNameObjDict = NULL;
		acdbHostApplicationServices()->
			workingDatabase()->
			getNamedObjectsDictionary(pNameObjDict, 
				AcDb::kForWrite);

		AcDbObjectId dictObjId;

		ECDDwgScaleCO *pDwgScale = new ECDDwgScaleCO();
		pDwgScale->Set(1, 100);

		pNameObjDict->setAt(DWG_SCALE_KEY, pDwgScale, dictObjId);

		pDwgScale->close();
		pNameObjDict->close();
	}
	static void ECDMyGroupMyGetDwgScale() {

		CString DWG_SCALE_KEY = L"DwgScale";

		AcDbDictionary *pNameObjDict = NULL;
		Acad::ErrorStatus es;

		acdbHostApplicationServices()->
			workingDatabase()->
			getNamedObjectsDictionary(pNameObjDict,
				AcDb::kForRead);

		ECDDwgScaleCO *pDwgScale = NULL;

		es = pNameObjDict->getAt(DWG_SCALE_KEY, (AcDbObject*&)pDwgScale, AcDb::kForRead);

		pNameObjDict->close();

		if (es == Acad::eKeyNotFound) {

			acutPrintf(L"\n不存在字典項%s", DWG_SCALE_KEY);
			return;
		}
		acutPrintf(L"當前圖形條件圖比例:%d,出圖比例：%d", pDwgScale->GetInfoScale(), pDwgScale->GetLableScale());
		pDwgScale->close();



	}*/
	
	static void ECDMyGroupEModalD() {

		/*MFCDialog theDialog;

		if (theDialog.DoModal() == IDOK) {
			AfxMessageBox(L"点击确定关闭了对话框");
		}*/

		/*CAcModuleResourceOverride resOverride;

		CArxDialog theDialog;

		theDialog.DoModal();*/

		CAcModuleResourceOverride resOverride;

			if (pDialog==NULL) {

				pDialog = new CModelessDialog(acedGetAcadFrame());
				pDialog->Create(IDD_DIALOG_MODELESS);
				pDialog->ShowWindow(SW_SHOW);
			}
			else {

				pDialog->ShowWindow(SW_SHOW);

			}


	}

	static void ECDMyGroupEMirroDim() {
		CDimMirror dim =CDimMirror();
		dim.Command();
		dim.~CDimMirror();
	}
	static void ECDMyGroupEJigMove(void)
	{
		ads_name ename;
		ads_point pt;
		ads_name ss;
		int rt;

		if (rt = acedSSGet(NULL, NULL, NULL, NULL, ss) == RTCAN)
			return;
		int len;
		acedSSLength(ss, &len);
		if (0 == len) return;

		AcDbObjectId id;
		//acdbGetObjectId(id,ename);
		AcDbObjectIdArray ids;
		for (int i = 0; i < len; i++)
		{
			acedSSName(ss, i, ename);
			acdbGetObjectId(id, ename);
			ids.append(id);
		}
		acedSSFree(ss);
		acedGetPoint(NULL, L"/n起点：", pt);

		CMoveJig * pJig = new CMoveJig(asPnt3d(pt));
		pJig->doIt(ids);
	}
	static void ECDMyGroupEJigRec() {

		//DrawRecJig recJig = DrawRecJig();

		//ads_point  pt;

		//AcDbPolyline *pl1 = NULL, *pl2 = NULL,*pl3;


		//if (acedGetPoint(NULL, L"\n请选择插入点：", pt) == RTNORM) {

		//	AcDbObjectId  oId1, oId2;

		//	if (recJig.DoIt(asPnt3d(pt), oId1)) {

		//		pl1 = AcDbPolyline::cast(recJig.entity());
		//	}

		//	if (recJig.DoIt(asPnt3d(pt), oId2)) {

		//		pl2 = AcDbPolyline::cast(recJig.entity());
		//	}

		//	AcGePoint3dArray ptArr;

		//	for (int i = 0; i < pl1->numVerts(); i++) {

		//		AcGePoint2d pt;
		//		pl1->getPointAt(i, pt);

		//		ptArr.append(CConvertUtil::ToPoint3d(pt));

		//	}
		//	for (int i = 0; i < pl2->numVerts(); i++) {

		//		AcGePoint2d pt;
		//		pl2->getPointAt(i, pt);

		//		ptArr.append(CConvertUtil::ToPoint3d(pt));

		//	}
		//	pl1->close();
		//	pl2->close();



		//	AcDbEntity *pEnt1=NULL, *pEnt2=NULL;

		//	if (acdbOpenObject(pEnt1, oId1, AcDb::kForWrite) == Acad::eOk) {

		//		pEnt1->erase();
		//		pEnt1->close();

		//	}
		//	if (acdbOpenObject(pEnt2, oId2, AcDb::kForWrite) == Acad::eOk) {

		//		pEnt2->erase();
		//		pEnt2->close();
		//	}
		//	
		//	

		//	MyTuBao tubao = MyTuBao(ptArr);

		//	AcGePoint3dArray ptArrGet;
		//		tubao.GetTuBao(ptArrGet);

		//	pl3 = new AcDbPolyline();

		//	for (int i = 0; i <ptArrGet.length(); i++)
		//	{

		//		pl3->addVertexAt(pl3->numVerts(), CConvertUtil::ToPoint2d(ptArrGet.at(i)), 0, 0, 0);
		//	}

		//	pl3->setClosed(Adesk::kTrue);

		//AcGePoint2d ptStart, ptEnd;
	
		//for (int j = 0; j < pl3->numVerts(); j++)
		//{
		//	AcGeLineSeg2d l2d;

		//	pl3->getLineSegAt(j, l2d);

		//	AcGeVector2d vec2d= l2d.direction();

		//	double an = vec2d.angle();

		//	if (!CMathUtil::IsEqual(vec2d.angle(), CMathUtil::PI(), 1e-4) &&
		//		!CMathUtil::IsEqual(vec2d.angle(), CMathUtil::PI() / 2.0, 1e-4) &&
		//		!CMathUtil::IsEqual(vec2d.angle(), CMathUtil::PI() * 2, 1e-4) &&
		//		!CMathUtil::IsEqual(vec2d.angle(), CMathUtil::PI() * 3 / 2.0, 1e-4) &&
		//		!CMathUtil::IsEqual(vec2d.angle(), 0, 1e-4))
		//	{
		//		ptStart = l2d.startPoint();
		//		ptEnd = l2d.endPoint();
		//		break;
		//	}
		//}

		//AcDbObjectId pId = CDwgDataBaseUtil::PostToModelSpace(pl3);
		//pl3->close();

		//AcGePoint3d pt1 = CConvertUtil::ToPoint3d(ptStart), pt2 = CConvertUtil::ToPoint3d(ptEnd);
		//
		//double x1 = ptStart.x, x2 = ptEnd.x;
		//double y1 = ptStart.y, y2 = ptEnd.y;

		//// 计算矩形的角点
		//AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));
		//AcGePoint2d ptRightBottom(max(x1, x2), min(y1, y2));
		//AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));
		//AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));

		//AcGePoint2dArray pt2dArr;
		//pt2dArr.append(ptLeftBottom);
		//pt2dArr.append(ptRightBottom);
		//pt2dArr.append(ptRightTop);
		//pt2dArr.append(ptLeftTop);

		//if (pt2dArr.contains(ptStart)) {
		//	pt2dArr.remove(ptStart);
		//}
		//if (pt2dArr.contains(ptEnd)) {
		//	pt2dArr.remove(ptEnd);
		//}

		//int index = ptArrGet.find(pt1);

		//if (index != -1) {

		//	ptArrGet.insertAt(index+1, CConvertUtil::ToPoint3d(pt2dArr[0]));

		//}

		//AcDbPolyline *plTemp1 = new AcDbPolyline();


		//for (int i = 0; i <ptArrGet.length(); i++)
		//{

		//	plTemp1->addVertexAt(plTemp1->numVerts(), CConvertUtil::ToPoint2d(ptArrGet.at(i)), 0, 0, 0);
		//}

		//plTemp1->setClosed(Adesk::kTrue);

		//plTemp1->setColorIndex(1);

		//

		//double area1 = 0.0;

		//plTemp1->getArea(area1);

		////plTemp1->close();


		////ptArrGet.insertAt(index, CConvertUtil::ToPoint3d(pt2dArr[0]));
		//ptArrGet.remove(CConvertUtil::ToPoint3d(pt2dArr[0]));
		//ptArrGet.insertAt(index+1, CConvertUtil::ToPoint3d(pt2dArr[1]));


		//AcDbPolyline *plTemp2 = new AcDbPolyline();


		//for (int i = 0; i <ptArrGet.length(); i++)
		//{

		//	plTemp2->addVertexAt(plTemp2->numVerts(), CConvertUtil::ToPoint2d(ptArrGet.at(i)), 0, 0, 0);
		//}

		//plTemp2->setClosed(Adesk::kTrue);

		//plTemp2->setColorIndex(2);

		//

		//double area2 = 0.0;

		//plTemp2->getArea(area2);
		////plTemp2->close();



		//if ( area1<area2) {
		//	AcDbObjectId pIdTemp1 = CDwgDataBaseUtil::PostToModelSpace(plTemp1);
		//	plTemp1->close();

		//	plTemp2->erase();
		//	plTemp2->close();

		//}
		//else {
		//	
		//	AcDbObjectId pIdTemp2 = CDwgDataBaseUtil::PostToModelSpace(plTemp2);
		//	plTemp2->close();

		//	plTemp1->erase();
		//	plTemp1->close();


		//}
		//
		//
		///*AcDbLine * line = new AcDbLine(CConvertUtil::ToPoint3d(ptStart), CConvertUtil::ToPoint3d(ptEnd));

		//line->setColorIndex(3);

		//CDwgDataBaseUtil::PostToModelSpace(line);

		//line->close();*/



		


		//	}

	}

	static void ECDMyGroupEUnion() {

		DrawRecJig recJig = DrawRecJig();

		ads_point  pt;

		AcDbPolyline *pl1 = NULL, *pl2 = NULL;


		if (acedGetPoint(NULL, L"\n请选择插入点：", pt) == RTNORM) {

			AcDbObjectId  oId1, oId2;

			if (recJig.DoIt(asPnt3d(pt), oId1)) {

				pl1 = AcDbPolyline::cast(recJig.entity());
			}

			if (recJig.DoIt(asPnt3d(pt), oId2)) {

				pl2 = AcDbPolyline::cast(recJig.entity());
			}
			
			AcDbVoidPtrArray curveSegments;
			AcDbVoidPtrArray regions;

			curveSegments.append(pl1);
			curveSegments.append(pl2);

			if (AcDbRegion::createFromCurves(curveSegments, regions) == Acad::eOk) {

				AcDbRegion *region =(AcDbRegion *)regions[0];

				region->booleanOper(AcDb::BoolOperType::kBoolUnite, (AcDbRegion *)regions[1]);			

				AcGePoint3dArray ptArr;

				CRegionUtil::GetRegionPoints(region, ptArr);

				for (int i = 0; i < regions.length (); i++)
				{
					AcDbRegion *regionTemp = (AcDbRegion *)regions[i];

					regionTemp->erase();

					regionTemp->close();

				}

				region->close();

				AcDbPolyline *plTemp1 = new AcDbPolyline();

				for (int i = 0; i <ptArr.length(); i++)
				{

					plTemp1->addVertexAt(plTemp1->numVerts(), CConvertUtil::ToPoint2d(ptArr.at(i)), 0, 0, 0);
				}

				plTemp1->setClosed(Adesk::kTrue);

				plTemp1->setColorIndex(1);

				CDwgDataBaseUtil::PostToModelSpace(plTemp1);

				plTemp1->close();

			}
			pl1->close();
			pl2->close();
		}
	}

	static void ECDMyGroupEFindPos() {

		CFindTextAndBlock * f1 = new CFindTextAndBlock(L"abc", L"0");
		CFindTextAndBlock * f2 = new CFindTextAndBlock(L"LL", L"0",2);
		f1->Command();
		f2->Command();

		delete f1;
		delete f2;

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject3App)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyXData, MyXData, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyXRec, MyXRec, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyAddGroup, MyAddGroup, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyDelGroup, MyDelGroup, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyCreateDwgFile, MyCreateDwgFile, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyReadFile, MyReadFile, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyFileToFile, MyFileToFile, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyNewDoc, MyNewDoc, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyTestDotInPolyline, MyTestDotInPolyline, ACRX_CMD_MODAL, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MySetDwgScale, MySetDwgScale, ACRX_CMD_MODAL, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, MyGetDwgScale, MyGetDwgScale, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EModalD, EModalD, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EMirroDim, EMirroDim, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EJigMove, EJigMove, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EJigRec, EJigRec, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EUnion, EUnion, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject3App, ECDMyGroup, EFindPos, EFindPos, ACRX_CMD_MODAL, NULL)