#include "StdAfx.h"
#include "resource.h"
#include "acdocman.h"
#include"acedads.h"
#include "acedCmdNF.h"
#include<vector>
#include"MoveJig.h"
//#include <shlwapi.h>
#include"StringUtil.h"
#include "algorithm" //sort函数、交并补函数
#include "iterator" //求交并补使用到的迭代器
#include"dbmleaderstyle.h"

//#pragma comment(lib,"Shlwapi.lib")

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

	/*static void ECDMyGroupEcdRR()
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

		long len = 0;

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

	}*/

	static void  ECDMyGroupMR() {

		CString path = acDocManager->curDocument()->fileName();

		int lastInt = path.ReverseFind('\\');
		ErrorStatus es2=ErrorStatus::eOk;

		CString fileName = path.Mid(lastInt + 1);
		int lastDot = fileName.ReverseFind('.');

		CString newFileName;
		newFileName.Format(L"%s_M", fileName.Mid(0, lastDot));

		CString copyFileName;
		copyFileName.Format(L"%s_M_C", fileName.Mid(0, lastDot));

		CString  newPath;
		newPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), newFileName);

		CString aixPath;
		aixPath.Format(L"%s\\%s.txt", path.Mid(0, lastInt), newFileName);

		CString  cPath;
		cPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), copyFileName);


		vector<AcDbEntity*>vecEnt;

		AcDbObjectIdArray oIds;
		//GetSel(oIds);
		AcDbBlockTable *pBlkTbl;
		es2=acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);
		
		//获取块表记录
		AcDbBlockTableRecord *pBlkRcd;
		es2=pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkRcd,AcDb::kForWrite);
		pBlkTbl->close();

		

		//建立迭代器准备进行循环
		AcDbBlockTableRecordIterator *pBlkTblRcdltr;
		es2=pBlkRcd->newIterator(pBlkTblRcdltr);
		
	
		//定义实体指针
		AcDbEntity *pEnt=NULL;
		//在读入的块表记录中进行迭代
		

		for (pBlkTblRcdltr->start();!pBlkTblRcdltr->done();pBlkTblRcdltr->step())
		{
			es2=pBlkTblRcdltr->getEntity(pEnt,AcDb::kForWrite);
			
			AcDbObjectId tempId;
			pBlkTblRcdltr->getEntityId(tempId);

			if (es2 != ErrorStatus::eOk) {
				continue;
				es2=pEnt->upgradeOpen();
				if(es2==ErrorStatus::eOk){
				pEnt->erase();
				pEnt->close();
				}else{
					CString text;
					text.Format(L"es2=%d",es2);

					AfxMessageBox(text);
				}
				continue;
			}
			//读取实体
			AcDbEntity * cL=AcDbEntity::cast(pEnt->clone());

			vecEnt.push_back(cL);

			oIds.append(pEnt->objectId());

			pEnt->close();

		}
		//关闭块表
		pBlkRcd->close();
		

		//删除块表记录迭代器
		delete pBlkTblRcdltr;

		/*CString test;

		test.Format(L"Oids=%d,vecEnts=%d",oIds.length(),vecEnt.size());

		AfxMessageBox(test);*/




		AcGePoint3d formPt, toPt;
		bool findPt = false;
		

		if (!findPt) {
			
			CString allStr;
			CFile file;
			CFileException pError;
			if(file.Open(aixPath,CFile::modeReadWrite,&pError)!=0){

				allStr=ReadUnicode(file);
			}else
			{
				acutPrintf(L"不存在对称轴数据文件\n");
				file.Close();
				return;
			}
			file.Close();
			
			if(allStr.IsEmpty()){
				acutPrintf(L"对称轴数据文件沒有记录\n");
				return;

			}

			vector<CString>vecF;
			vector<CString>vecPt;
			//AfxMessageBox(allStr);
			CStringUtil::Split(allStr,L"|",vecF,false);

			CString s1=vecF[0];
			CString s2=vecF[1];

			CStringUtil::Split(s1,L",",vecPt,false);

			formPt.x=_wtof(vecPt[0]);
			formPt.y=_wtof(vecPt[1]);
			formPt.z=_wtof(vecPt[2]);
		
			vecPt.clear();


			CStringUtil::Split(s2,L",",vecPt,false);

			toPt.x=_wtof(vecPt[0]);
			toPt.y=_wtof(vecPt[1]);
			toPt.z=_wtof(vecPt[2]);

		}

		CMoveJig jig(formPt, toPt);

		jig.UpdateDoIt(oIds, true);


		//CopyFile(newPath,cPath,FALSE);

		

//删除原实体
		for (int i=0;i<oIds.length();i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, oIds[i], AcDb::kForWrite) == Acad::eOk) {

				l->erase();

				l->close();
				l=NULL;

			}

		}

		if(DeepClone(jig.m_idsC,newPath)){
			ErrorStatus es =ErrorStatus::eOk;
			AcDbLine * aixLine=new AcDbLine(formPt,jig.m_ToPoint);

			DeleteFile(cPath);
			acutPrintf(L"更新成功");
			AcDbExtents ext;
			AcGePoint3d ptO;
			for (int i=0;i<(int)vecEnt.size();i++){

				AcDbEntity * ent1=vecEnt[i];

				es=ent1->getGeomExtents(ext);
				if(es==ErrorStatus::eOk){
					ptO=ext.maxPoint();
					break;
				}
			}

			AcGePoint3d ptOnL;

			aixLine->getClosestPointTo(ptO,ptOnL,Adesk::kTrue);

			AcGeVector3d vec=(ptO-ptOnL).normal();


			DeleteOnDesDb(vec,newPath,aixLine);

		}
		else{

			//DeleteFile(newPath);
			//CopyFile(cPath,newPath,FALSE);
			acutPrintf(L"更新文件不成功！");

		}

		for (int i=0;i<jig.m_idsC.length();i++)
		{
			AcDbEntity * l = NULL;

			if (acdbOpenObject(l, jig.m_idsC[i], AcDb::kForWrite) == Acad::eOk) {

				l->erase();

				l->close();
				l=NULL;

			}

		}

		for (int i=0;i<(int)vecEnt.size();i++)
		{

			PostToModelSpace(vecEnt[i]);
			vecEnt[i]->close();
		}
	}

	static void ECDMyGroupMM() {
		ErrorStatus es =ErrorStatus::eOk;
		CString path = acDocManager->curDocument()->fileName();

		int lastInt = path.ReverseFind('\\');


		CString fileName = path.Mid(lastInt + 1);
		int lastDot = fileName.ReverseFind('.');

		CString newFileName;
		newFileName.Format(L"%s_M", fileName.Mid(0, lastDot));

		CString  newPath;
		newPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), newFileName);

		

		CString copyFileName;
		copyFileName.Format(L"%s_M_C", fileName.Mid(0, lastDot));

		CString aixPath;
		aixPath.Format(L"%s\\%s.txt", path.Mid(0, lastInt), newFileName);

		CString  cPath;
		cPath.Format(L"%s\\%s.dwg", path.Mid(0, lastInt), copyFileName);

		AcDbObjectIdArray oIds;

		GetSel(oIds);
		ads_point pt1;

		AcGePoint3d fPt;

		if (acedGetPoint(NULL, L"轴线第一个点：\n", pt1) != RTNORM) {
			return;
		}


		fPt = asPnt3d(pt1);

		vector<AcDbEntity*>vecEnt;

		CMoveJig jig(fPt);

		jig.doIt(oIds, true);

		if (jig.m_idsC.length() == 0) {
			return;
		}

	//CString strHdl;

//写xData
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
		
			/*AcDbHandle h=oIds[i].handle();

			Adesk::UInt32 high=h.high();
			Adesk::UInt32 low=h.low();

			CString strNum;
			strNum.Format(L"%d%d",high,low);

			int num=_ttoi(strNum);

			strNum="";
			strNum.Format(L"%X",num);

			strHdl+=strNum+"&";*/


			
		}
//克隆原实体		
	for (int i = 0; i < oIds.length(); i++)
	{
		AcDbEntity * l = NULL;

		if (acdbOpenObject(l, oIds[i], AcDb::kForWrite) == Acad::eOk) {

			AcDbEntity * cL=AcDbEntity::cast(l->clone());

			vecEnt.push_back(cL);
			l->close();
		}
	}
//原实体删除
	for (int i=0;i<oIds.length();i++)
	{
		AcDbEntity * l = NULL;

		if (acdbOpenObject(l, oIds[i], AcDb::kForWrite) == Acad::eOk) {

			l->erase();

			l->close();
			l=NULL;

		}

	}

	CString strFpt;
	strFpt.Format(L"%f,%f,%f",fPt.x,fPt.y,fPt.z);

	CString strEpt;
	strEpt.Format(L"%f,%f,%f",jig.m_ToPoint.x,jig.m_ToPoint.y,jig.m_ToPoint.z);

	CString wStr;

	wStr.Format(L"%s|%s",strFpt,strEpt);

	CFile f;

	if(true){
		if(f.Open(aixPath, CFile::modeCreate | CFile::modeWrite)!=0){

			f.SeekToBegin();

			char ww[2] ={0xFF,0xFE};

			f.Write(ww,2);

			f.Seek(2,CFile::begin);

			f.Write(wStr,wStr.GetLength()*sizeof(wchar_t));
			f.Flush();
			f.Close();
		}
	}
	else{

		CString strE;
		if (f.Open(aixPath, CFile::modeReadWrite, NULL)) {
			
			strE=ReadUnicode(f);
			f.Close();
				
		}
		else {
			AfxMessageBox(_T("打开记录轴的文件失败！"));
			f.Close();
			return;
			
		}
	
		
	wStr+=L"\r\n"+strE;
	//写入新内容
		if(f.Open(aixPath,CFile::modeNoTruncate|CFile::modeWrite)!=0){

			f.SeekToBegin();

			char ww[2] ={0xFF,0xFE};

			f.Write(ww,2);

			f.Seek(2,CFile::begin);

			f.Write(wStr,wStr.GetLength()*sizeof(wchar_t));
			f.Flush();
			f.Close();

		}
	}

	DeepClone(jig.m_idsC,newPath);
	
	AcDbLine * aixLine=new AcDbLine(fPt,jig.m_ToPoint);
	
	AcDbExtents ext;
	AcGePoint3d ptO;
	for (int i=0;i<(int)vecEnt.size();i++){

		AcDbEntity * ent1=vecEnt[i];
		
		es=ent1->getGeomExtents(ext);
		if(es==ErrorStatus::eOk){
			ptO=ext.maxPoint();
			break;
		}
	}
	
	AcGePoint3d ptOnL;

	aixLine->getClosestPointTo(ptO,ptOnL,Adesk::kTrue);

	AcGeVector3d vec=(ptO-ptOnL).normal();


	DeleteOnDesDb(vec,newPath,aixLine);

//镜像实体删除
	for (int i=0;i<jig.m_idsC.length();i++)
	{
		AcDbEntity * l = NULL;

		if (acdbOpenObject(l, jig.m_idsC[i], AcDb::kForWrite) == Acad::eOk) {

			l->erase();

			l->close();
			l=NULL;

		}

	}

	for (int i=0;i<(int)vecEnt.size();i++)
	{

		PostToModelSpace(vecEnt[i]);

		vecEnt[i]->close();

	}

}

	static void DeleteOnDesDb(const AcGeVector3d&vec,CString path,AcDbLine*aixLine){
		ErrorStatus es=ErrorStatus::eOk;
		AcDbDatabase *pDb=new AcDbDatabase(Adesk::kFalse);
		pDb->readDwgFile(path,AcDbDatabase::OpenMode::kForReadAndWriteNoShare);
		//获取块表
		AcDbBlockTable *pBlkTbl;
		pDb->getSymbolTable(pBlkTbl,AcDb::kForRead);
		//获取块表记录
		AcDbBlockTableRecord *pBlkRcd;
		pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkRcd,AcDb::kForWrite);
		pBlkTbl->close();
		//建立迭代器准备进行循环
		AcDbBlockTableRecordIterator *pBlkTblRcdltr;
		pBlkRcd->newIterator(pBlkTblRcdltr);
		//定义实体指针
		AcDbEntity *pEnt=NULL;
		//在读入的块表记录中进行迭代
		AcDbExtents ext;

		for (pBlkTblRcdltr->start();!pBlkTblRcdltr->done();pBlkTblRcdltr->step())
		{
			

			//读取实体
			pBlkTblRcdltr->getEntity(pEnt,AcDb::kForWrite);
			AcGePoint3d ptO;
			es=pEnt->getGeomExtents(ext);
			if(es!=ErrorStatus::eOk){
				 pEnt->close();
				continue;
			}
			ptO=ext.maxPoint();

			AcGePoint3d ptOnL;

			aixLine->getClosestPointTo(ptO,ptOnL,Adesk::kTrue);

			AcGeVector3d vec2=(ptO-ptOnL).normal();

			if(vec2.x*vec.x>=0&&vec2.y*vec.y>=0){

				pEnt->erase();

				pEnt->close();
				

			}
			else{
				pEnt->close();
				
			}
			
		}
		//关闭块表
		pBlkRcd->close();
		es=pDb->saveAs(path);

		
		//删除块表记录迭代器
		delete pBlkTblRcdltr;
		//删除图形
		delete pDb;

		delete aixLine;

	}

	static void   GetSel( AcDbObjectIdArray & ids) {
		ads_name aName;
		
		if (acedSSGet(NULL, NULL, NULL, NULL, aName) != RTNORM) {
			return ;
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
		
	}

	static  bool DeepClone(AcDbObjectIdArray & ids, CString path)
	{
		ErrorStatus es = ErrorStatus::eOk;
		

		AcDbObjectId  modelSpaceId;


		AcDbBlockTable *pBT1 = NULL;


		AcDbDatabase* pTempDb = new AcDbDatabase();

		pTempDb->setLtscale(acdbHostApplicationServices()->workingDatabase()->ltscale());
		pTempDb->setLineWeightDisplay(acdbHostApplicationServices()->workingDatabase()->lineWeightDisplay());
		pTempDb->setFillmode(acdbHostApplicationServices()->workingDatabase()->fillmode());
		
		AcDbIdMapping idMap;
		es = idMap.setDestDb(pTempDb);
		AcDbObjectIdArray txtIds;
		if (!CopyTextStyleIdInfo(acdbHostApplicationServices()->workingDatabase(), pTempDb, txtIds)) {

			return false;
		}
		if(!CopyDimStyle(acdbHostApplicationServices()->workingDatabase(), pTempDb)){
		AfxMessageBox(L"CopyDimStyleIdInfo false");
		return false;
		}

		//ids.append(txtIds);

		DeleteDim(pTempDb);
		AcDbObjectIdArray dimIds;
		GetDimId(dimIds);
		ids.append(dimIds);
		
		AcDbObjectIdArray tblIds;
		CopyTableStyle(acdbHostApplicationServices()->workingDatabase(), pTempDb, tblIds);
		
		CopyMleaderStyle(acdbHostApplicationServices()->workingDatabase(), pTempDb);
		
		pTempDb->getSymbolTable(pBT1, AcDb::kForRead);
		pBT1->getAt(ACDB_MODEL_SPACE, modelSpaceId);
		
		es=pBT1->close();

		es = pTempDb->wblockCloneObjects(ids, modelSpaceId, idMap, AcDb::kDrcIgnore);
		
		

		if (es != ErrorStatus::eOk) {
			delete pTempDb;
			pTempDb=NULL;
			return false;
		}
		es = pTempDb->saveAs(path);
		if (es != ErrorStatus::eOk) {
			delete pTempDb;
			pTempDb=NULL;
			return false;
		}
		delete pTempDb;
		pTempDb=NULL;
		return true;




	}

	static	bool CopyTextStyleIdInfo(AcDbDatabase *pFromDataSrc/*in*/, AcDbDatabase *pToDataDes/*in*/,AcDbObjectIdArray &txtIds)
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
		AcDbTextStyleTableRecord *txtRec = NULL;
		int jk = 0;
		for (pIterator->start(); !pIterator->done(); pIterator->step())
		{
			if ((es = pIterator->getRecord(txtRec, AcDb::kForRead)) != Acad::eOk) {
				if (txtRec != NULL) {
					txtRec->close();
				}
				continue;
			}

			AcDbObjectId styleId = AcDbObjectId::kNull;
			bool isFound = true;
			if ((es = pIterator->getRecordId(styleId)) == Acad::eOk)
			{
				AcDbTextStyleTableRecord* pNewRec = NULL;
				if (styleId == stdId) {

					es = pNewSt->getAt(L"Standard", pNewRec, AcDb::kForWrite);

				}
				else if (styleId == anoId) {
					es = pNewSt->getAt(L"Annotative", pNewRec, AcDb::kForWrite);

				}
				
				else{									
					txtRec->close();					
					continue;

				}

				//AcGiTextStyle *pTextStyle = new AcGiTextStyle(pToDataDes);
				if (pNewRec!=NULL)
				{
					ACHAR * pTypeface = NULL;
					Adesk::Boolean bold;
					Adesk::Boolean italic;
					Charset  charset;
					Autodesk::AutoCAD::PAL::FontUtils::FontPitch  pitchAndFamily;
					Autodesk::AutoCAD::PAL::FontUtils::FontFamily fontFamily;
					ACHAR *na, *na1, *bigFontN;
					txtRec->getName(na);

					txtRec->fileName(na1);
					txtRec->bigFontFileName(bigFontN);
					Adesk::UInt8 flagB = txtRec->flagBits();
					setSymbolName(pNewRec, na);
					es = pNewRec->setFileName(na1);
					es = pNewRec->setBigFontFileName(bigFontN);
					pNewRec->setFlagBits(flagB);
					pNewRec->setIsVertical(txtRec->isVertical());
					pNewRec->setIsShapeFile(txtRec->isShapeFile());


					/*待完善*/
					es = txtRec->font(pTypeface, bold, italic, charset, pitchAndFamily, fontFamily);
					//es = pTextStyle->font(pTypeface, bold, italic, charset, pitchAndFamily, fontFamily);
					if (es == Acad::eOk)
						pNewRec->setFont(pTypeface, bold, italic, charset, pitchAndFamily, fontFamily);

					// must explicitly set to ""
					es = pNewRec->setTextSize(txtRec->textSize());
					if (styleId != stdId&&styleId != anoId)
					{
						

						es = pNewRec->setXScale(txtRec->xScale());
					}
					es = pNewRec->setObliquingAngle(txtRec->obliquingAngle());

					if (isFound&&pNewRec!=NULL) {
						pNewRec->close();
						pNewRec = NULL;
					}
					else if(!isFound&&pNewRec != NULL) {
						
						bool flag = addToSymbolTableAndClose(pNewRec, pNewSt);
						
					}
				}
				if (txtRec != NULL) {
					es = txtRec->close();
					txtRec = NULL;
				}
				/*if (pTextStyle != NULL)
				{
					delete pTextStyle;
					pTextStyle = NULL;
				}*/
			}
		}
		if (pNewSt != NULL) {
			es = pNewSt->close();
			pNewSt = NULL;

		}
		if (pIterator != NULL)
		{
			delete pIterator;
			pIterator = NULL;
			pStyleTable->close();
			pStyleTable = NULL;
		}
		return true;
	}

	static bool  CopyDimStyle(AcDbDatabase *pFromDataSrc/*in*/, AcDbDatabase *pToDataDes/*in*/)
	{


		if (pFromDataSrc == NULL || pToDataDes == NULL)
			return false;
		ErrorStatus es2 = ErrorStatus::eOk;
		ACHAR * styleName = L"LL_tiDai00";

		AcDbDimStyleTable *pDimStyleTbl = NULL;
		pToDataDes->getDimStyleTable(pDimStyleTbl, AcDb::kForWrite);


		// 创建新的标注样式表记录
		AcDbDimStyleTableRecord *pDimStyleTblRcd = NULL;
		pDimStyleTblRcd = new AcDbDimStyleTableRecord();

		// 设置标注样式的特性
		es2=pDimStyleTblRcd->setName(styleName); // 样式名称
		pDimStyleTblRcd->setDimasz(3);  // 箭头长度
		pDimStyleTblRcd->setDimexe(3);  // 尺寸界线与标注点的偏移量
		pDimStyleTblRcd->setDimtad(1);  // 文字位于标注线的上方
		pDimStyleTblRcd->setDimtxt(3); // 标注文字的高度

									   // 将标注样式表记录添加到标注样式表中
		AcDbObjectId oId;
		es2=pDimStyleTbl->add(oId, pDimStyleTblRcd);
		pDimStyleTblRcd->close();
		pDimStyleTbl->close();

		es2=pToDataDes->setDimstyleData(oId);
		es2 = pToDataDes->setDimstyle(oId);
		return true;
		AcDbDimStyleTable *pStyleTable = NULL;
		AcDbDimStyleTable *pNewSt = NULL;
		Acad::ErrorStatus es = Acad::eOk;
		es = pFromDataSrc->getDimStyleTable(pStyleTable, AcDb::kForRead);
		if (es != Acad::eOk)
		{
			CString ttt;
			ttt.Format(L"pStyleTable open false=%d", es);
			AfxMessageBox(ttt);

			return false;
		}
		es = pToDataDes->getSymbolTable(pNewSt, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			CString ttt;
			ttt.Format(L"pNewSt open false=%d", es);
			AfxMessageBox(ttt);

			pStyleTable->close();
			pStyleTable = NULL;
			return false;

		}



		AcDbDimStyleTableIterator *pIterator = NULL;
		es = pStyleTable->newIterator(pIterator);
		if (es != Acad::eOk)
		{
			CString ttt;
			ttt.Format(L"pStyleTable->newIterator false=%d", es);
			AfxMessageBox(ttt);

			pStyleTable->close();
			pStyleTable = NULL;

			pNewSt->close();
			pNewSt = NULL;
			return false;
		}



		AcDbObjectId stdId, anoId, iso_25;
		pStyleTable->getAt(L"Standard", stdId);

		pStyleTable->getAt(L"Annotative", anoId);


		AcDbDimStyleTableRecord *txtRec = NULL;
		for (pIterator->start(); !pIterator->done(); pIterator->step())
		{
			AcDbObjectId styleId = AcDbObjectId::kNull;

			if ((es = pIterator->getRecord(txtRec, AcDb::kForRead)) != Acad::eOk) {
				CString ttt;
				ttt.Format(L"pIterator->getRecord(txtRec,AcDb::kForRead))=%d", es);
				AfxMessageBox(ttt);

				if (txtRec != NULL) {
					txtRec->close();
				}

			}

			if ((es = pIterator->getRecordId(styleId)) == Acad::eOk)
			{

				AcDbDimStyleTableRecord* pNewRec = NULL;
				bool isFind = true;
				if (styleId == stdId) {

					es = pNewSt->getAt(L"Standard", pNewRec, AcDb::kForWrite);

				}
				else if (styleId == anoId) {
					es = pNewSt->getAt(L"Annotative", pNewRec, AcDb::kForWrite);

				}
				/*else if (styleId == iso_25) {
				es=pNewSt->getAt(L"ISO-25", pNewRec, AcDb::kForWrite);

				}*/
				else {
					txtRec->close();
					txtRec = NULL;
					continue;
				}
				ACHAR* na;

				txtRec->getName(na);
				es = pNewRec->setName(na);
				es = pNewRec->setDimadec(txtRec->dimadec());
				es = pNewRec->setDimalt(txtRec->dimalt());
				es = pNewRec->setDimaltd(txtRec->dimaltd());
				es = pNewRec->setDimaltf(txtRec->dimaltf());
				es = pNewRec->setDimaltmzf(txtRec->dimaltmzf());
				es = pNewRec->setDimaltmzs(txtRec->dimaltmzs());
				es = pNewRec->setDimaltrnd(txtRec->dimaltrnd());
				es = pNewRec->setDimalttd(txtRec->dimalttd());
				es = pNewRec->setDimalttz(txtRec->dimalttz());
				es = pNewRec->setDimaltu(txtRec->dimaltu());
				es = pNewRec->setDimaltz(txtRec->dimaltz());
				es = pNewRec->setDimapost(txtRec->dimapost());
				es = pNewRec->setDimarcsym(txtRec->dimarcsym());
				es = pNewRec->setDimasz(txtRec->dimasz());
				es = pNewRec->setDimatfit(txtRec->dimatfit());

				es = pNewRec->setDimaunit(txtRec->dimaunit());
				es = pNewRec->setDimazin(txtRec->dimazin());
				es = pNewRec->setDimblk(txtRec->dimblk());
				es = pNewRec->setDimblk1(txtRec->dimblk1());
				es = pNewRec->setDimblk2(txtRec->dimblk2());
				es = pNewRec->setDimcen(txtRec->dimcen());
				es = pNewRec->setDimclrd(txtRec->dimclrd());
				es = pNewRec->setDimclre(txtRec->dimclre());
				es = pNewRec->setDimclrt(txtRec->dimclrt());
				es = pNewRec->setDimdec(txtRec->dimdec());
				es = pNewRec->setDimdle(txtRec->dimdle());

				es = pNewRec->setDimdli(txtRec->dimdli());
				es = pNewRec->setDimdsep(txtRec->dimdsep());

				es = pNewRec->setDimexe(txtRec->dimexe());
				es = pNewRec->setDimexo(txtRec->dimexo());

				es = pNewRec->setDimfit(txtRec->dimfit());
				es = pNewRec->setDimfrac(txtRec->dimfrac());
				es = pNewRec->setDimfxlen(txtRec->dimfxlen());
				es = pNewRec->setDimfxlenOn(txtRec->dimfxlenOn());

				es = pNewRec->setDimgap(txtRec->dimgap());
				es = pNewRec->setDimjogang(txtRec->dimjogang());
				es = pNewRec->setDimjust(txtRec->dimjust());

				es = pNewRec->setDimldrblk(txtRec->dimldrblk());
				es = pNewRec->setDimlfac(txtRec->dimlfac());
				es = pNewRec->setDimlim(txtRec->dimlim());
				es = pNewRec->setDimltype(txtRec->dimltype());
				es = pNewRec->setDimltex1(txtRec->dimltex1());
				es = pNewRec->setDimltex2(txtRec->dimltex2());

				es = pNewRec->setDimlunit(txtRec->dimlunit());
				es = pNewRec->setDimlwd(txtRec->dimlwd());
				es = pNewRec->setDimlwe(txtRec->dimlwe());

				es = pNewRec->setDimmzf(txtRec->dimmzf());
				es = pNewRec->setDimmzs(txtRec->dimmzs());
				es = pNewRec->setDimpost(txtRec->dimpost());
				es = pNewRec->setDimrnd(txtRec->dimrnd());

				es = pNewRec->setDimsah(txtRec->dimsah());
				es = pNewRec->setDimscale(txtRec->dimscale());
				es = pNewRec->setDimsd1(txtRec->dimsd1());
				es = pNewRec->setDimsd2(txtRec->dimsd2());
				es = pNewRec->setDimse1(txtRec->dimse1());
				es = pNewRec->setDimse2(txtRec->dimse2());
				es = pNewRec->setDimsoxd(txtRec->dimsoxd());
				es = pNewRec->setDimtad(txtRec->dimtad());
				es = pNewRec->setDimtdec(txtRec->dimtdec());
				es = pNewRec->setDimtfac(txtRec->dimtfac());
				es = pNewRec->setDimtfill(txtRec->dimtfill());
				es = pNewRec->setDimtfillclr(txtRec->dimtfillclr());
				es = pNewRec->setDimtih(txtRec->dimtih());
				es = pNewRec->setDimtix(txtRec->dimtix());
				es = pNewRec->setDimtm(txtRec->dimtm());
				es = pNewRec->setDimtmove(txtRec->dimtmove());
				es = pNewRec->setDimtofl(txtRec->dimtofl());
				es = pNewRec->setDimtoh(txtRec->dimtoh());
				es = pNewRec->setDimtolj(txtRec->dimtolj());
				es = pNewRec->setDimtp(txtRec->dimtp());
				es = pNewRec->setDimtsz(txtRec->dimtsz());
				es = pNewRec->setDimtvp(txtRec->dimtvp());
				es = pNewRec->setDimtxsty(txtRec->dimtxsty());
				es = pNewRec->setDimtxt(txtRec->dimtxt());
				es = pNewRec->setDimtxtdirection(txtRec->dimtxtdirection());
				es = pNewRec->setDimtzin(txtRec->dimtzin());
				es = pNewRec->setDimunit(txtRec->dimunit());
				es = pNewRec->setDimupt(txtRec->dimupt());
				es = pNewRec->setDimzin(txtRec->dimzin());
				if (isFind&&pNewRec!=NULL) {
					es = pNewRec->close();
					pNewRec = NULL;
				}
				else if (isFind == false && pNewRec != NULL) {
					es = pNewSt->close();
					pNewSt = NULL;
					bool flag = false;

					flag = addToDimTableAndClose(pNewRec, pToDataDes);

					pToDataDes->getSymbolTable(pNewSt, AcDb::kForWrite);

				}
			}
			if (txtRec != NULL) {
				es = txtRec->close();

				txtRec = NULL;


			}

		}


		if (pNewSt != NULL) {
			es = pNewSt->close();
			pNewSt = NULL;

		}
		if (pIterator != NULL)
		{
			delete pIterator;
			pIterator = NULL;
			es = pStyleTable->close();

			pStyleTable = NULL;

		}

		return true;
	}

	static void DeleteDim(AcDbDatabase *pToDataDes) {
		ErrorStatus es = ErrorStatus::eOk;
		AcDbDimStyleTable *pStyleTable = NULL;
		
		es=pToDataDes->getSymbolTable(pStyleTable, AcDb::kForWrite);

		AcDbDimStyleTableIterator *pIterator = NULL;
		es=pStyleTable->newIterator(pIterator);

		AcDbDimStyleTableRecord *txtRec = NULL;
		for (pIterator->start(); !pIterator->done(); pIterator->step())
		{


			if (pIterator->getRecord(txtRec, AcDb::kForWrite) == Acad::eOk) {

				es=txtRec->erase();

				es=txtRec->close();
				txtRec = NULL;

			}
		}

		if (pIterator != NULL)
		{
			delete pIterator;
			pIterator = NULL;
			es=pStyleTable->close();

			pStyleTable = NULL;

		}

	}

	static void GetDimId(AcDbObjectIdArray & dimIds) {
		AcDbDimStyleTable *pStyleTable = NULL;

		Acad::ErrorStatus es = Acad::eOk;

		acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pStyleTable, AcDb::kForRead);
		AcDbDimStyleTableIterator *pIterator = NULL;
		es = pStyleTable->newIterator(pIterator);

		AcDbObjectId styleId;

		for (pIterator->start(); !pIterator->done(); pIterator->step())
		{

			es = pIterator->getRecordId(styleId);
			if (es == ErrorStatus::eOk) {
				dimIds.append(styleId);
			}

		}

		if (pIterator != NULL)
		{
			delete pIterator;
			pIterator = NULL;
			es = pStyleTable->close();

			pStyleTable = NULL;

		}
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
	static BOOL addToSymbolTableAndClose(AcDbSymbolTableRecord* systemTextRec/*in*/, AcDbTextStyleTable *symTextTbl/*in*/)
	{
		if (symTextTbl == NULL || systemTextRec == NULL)
			return FALSE;

		ErrorStatus es = ErrorStatus::eOk;
		
		
		AcDbSymbolTable *pSysRec = AcDbTextStyleTable::cast(symTextTbl);
		//覆盖字体样式
		es = pSysRec->add(systemTextRec);
		if (es != Acad::eOk)
		{
			//symTextTbl->close();
			systemTextRec->close();
			return FALSE;
		}
		else
		{
			systemTextRec->close();
			//symTextTbl->close();
		}
		return TRUE;
	}

	static BOOL addToDimTableAndClose(AcDbSymbolTableRecord* systemTextRec/*in*/, AcDbDatabase *pDataBase/*in*/)
	{
		if (pDataBase == NULL || systemTextRec == NULL)
			return FALSE;

		AcDbDimStyleTable* symTextTbl = NULL;
		Acad::ErrorStatus es = Acad::eOk;
		es = pDataBase->getDimStyleTable(symTextTbl, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			if (systemTextRec != NULL)
			{
				delete systemTextRec;
				systemTextRec = NULL;
			}
			return FALSE;
		}
		AcDbSymbolTable *pSysRec = AcDbDimStyleTable::cast(symTextTbl);
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

	static bool CopyTableStyle(AcDbDatabase *pFromDataSrc, AcDbDatabase *pDataBase/*in*/,AcDbObjectIdArray &bIds) {
		ErrorStatus es = ErrorStatus::eOk;

		AcDbBlockTable *pBlkTbl,*newTbl;
		pFromDataSrc->getBlockTable(pBlkTbl, AcDb::kForRead);
		pDataBase->getBlockTable(newTbl, AcDb::kForWrite);
			
		AcDbDictionary *pDict = NULL,*newDict=NULL;
		AcDbObjectId idTblStyle, idTblStyle2;

		pFromDataSrc->getTableStyleDictionary(pDict, AcDb::kForRead);
		pDataBase->getTableStyleDictionary(newDict, AcDb::kForWrite);

		bool flag=newDict->setName(L"Standard", L"oldStandard");



		pDict->getAt(_T("Standard"), idTblStyle2);
		AcDbDictionaryIterator *pIterator = pDict->newIterator();

		AcDbObject* pObj=NULL;
		AcDbTableStyle * pTStyle = NULL;
		//while (!pIterator->done()) {

		//	AcDbTableStyle* newStl = NULL;

		//	idTblStyle = pIterator->objectId();

		//	if (idTblStyle != idTblStyle2) {
		//		continue;
		//	}
		//	
		//	pIterator->getObject(pObj, AcDb::kForWrite);
		//	//bIds.append(idTblStyle);
		//	pTStyle = AcDbTableStyle::cast(pObj);

		//	const ACHAR* n=NULL;
		//	n = pIterator->name();

		//	newStl = AcDbTableStyle::cast(pTStyle->clone());

		//	es=newDict->setAt(n, newStl,idTblStyle);

		//	pObj->close();
		//	pObj = NULL;
		//	newStl->close();
		//	newStl = NULL;
		//		pIterator->next();

		//}
		if (newDict != NULL) {
			newDict->close();
			newDict = NULL;
			newTbl->close();
			newTbl = NULL;
		}
		
		if (pIterator != NULL) {
			delete pIterator;
			pIterator = NULL;
			es = pBlkTbl->close();
			pBlkTbl = NULL;
		}

		return true;

		
	}

	static bool CopyMleaderStyle(AcDbDatabase *pFromDataSrc, AcDbDatabase *pDataBase) {
		ErrorStatus es = ErrorStatus::eOk;

		AcDbBlockTable *pBlkTbl, *newTbl;

		pFromDataSrc->getBlockTable(pBlkTbl, AcDb::kForRead);
		pDataBase->getBlockTable(newTbl, AcDb::kForWrite);

		AcDbDictionary *pDict = NULL, *newDict = NULL;
		AcDbObjectId idTblStyle, idTblStyle2;

		pFromDataSrc->getMLeaderStyleDictionary(pDict, AcDb::kForRead);
		pDataBase->getMLeaderStyleDictionary(newDict, AcDb::kForWrite);
		bool flag = newDict->setName(L"Standard", L"oldStandard");

		pDict->getAt(_T("Standard"), idTblStyle2);

		AcDbDictionaryIterator *pIterator = pDict->newIterator();

		AcDbObject* pObj = NULL;
		AcDbMLeaderStyle * pTStyle = NULL;
		//while (!pIterator->done()) {

		//	AcDbMLeaderStyle* newStl = NULL;

		//	idTblStyle = pIterator->objectId();

		//	if (idTblStyle != idTblStyle2)
		//	{
		//		continue;
		//	}
		//	pIterator->getObject(pObj, AcDb::kForWrite);
		//	//bIds.append(idTblStyle);
		//	pTStyle = AcDbMLeaderStyle::cast(pObj);

		//	const ACHAR* n = NULL;
		//	n = pIterator->name();

		//	newStl = AcDbMLeaderStyle::cast(pTStyle->clone());

		//	es = newDict->setAt(n, newStl, idTblStyle);

		//	pObj->close();
		//	pObj = NULL;
		//	newStl->close();
		//	newStl = NULL;
		//	pIterator->next();

		//}
		if (newDict != NULL) {
			newDict->close();
			newDict = NULL;
			newTbl->close();
			newTbl = NULL;
		}

		if (pIterator != NULL) {
			delete pIterator;
			pIterator = NULL;
			es = pBlkTbl->close();
			pBlkTbl = NULL;
		}

		return true;

	}

	static bool CopyMlStyle(AcDbDatabase *pFromDataSrc, AcDbDatabase *pDataBase) {
		ErrorStatus es = ErrorStatus::eOk;

		AcDbBlockTable *pBlkTbl, *newTbl;

		pFromDataSrc->getBlockTable(pBlkTbl, AcDb::kForRead);
		pDataBase->getBlockTable(newTbl, AcDb::kForWrite);

		AcDbDictionary *pDict = NULL, *newDict = NULL;
		AcDbObjectId idTblStyle, idTblStyle2;

		pFromDataSrc->getMLStyleDictionary(pDict, AcDb::kForRead);
		pDataBase->getMLStyleDictionary(newDict, AcDb::kForWrite);
		bool flag = newDict->setName(L"Standard", L"oldStandard");

		pDict->getAt(_T("Standard"), idTblStyle2);

		AcDbDictionaryIterator *pIterator = pDict->newIterator();

		if (newDict != NULL) {
			newDict->close();
			newDict = NULL;
			newTbl->close();
			newTbl = NULL;
		}

		if (pIterator != NULL) {
			delete pIterator;
			pIterator = NULL;
			es = pBlkTbl->close();
			pBlkTbl = NULL;
		}

		return true;

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

	static CString ReadUnicode(CFile& file)//Unicode文件编码获取
	{
		
		file.Seek(2, CFile::begin);//向后移动三个字节
		LONGLONG nLen = file.GetLength();//64位操作系统
		TCHAR* pText = new TCHAR[nLen / 2 + 1];//防止不够 + 1
		nLen =(LONGLONG)file.Read(pText, nLen);
		pText[nLen / 2] = _T('\0');
		
		CString str(pText);
		delete[] pText;
		
		return str;
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdMirrorApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, MR, MR, ACRX_CMD_MODAL, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, EcdRR, EcdRR, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, MM, MM, ACRX_CMD_MODAL, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMirrorApp, ECDMyGroup, EcdCC, EcdCC, ACRX_CMD_MODAL, NULL)
