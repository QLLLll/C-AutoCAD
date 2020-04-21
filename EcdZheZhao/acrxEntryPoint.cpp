#include "StdAfx.h"
#include "resource.h"
#include"dbwipe.h"
#include<vector>
#define  PI 3.1415926
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdZheZhaoApp : public AcRxArxApp {

public:
	CEcdZheZhaoApp() : AcRxArxApp() {}

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
	
	static void setVisible() {

		ads_name aName;
		if (acedSSGet(NULL, NULL, NULL, NULL, aName) != RTNORM) {
			return;
		}

		int len = 0;
		acedSSLength(aName, &len);

		AcDbObjectIdArray ids;

		for (int i = 0; i < len; i++)
		{
			ads_name temp;

			acedSSName(aName, i, temp);

			AcDbObjectId tempId;
			acdbGetObjectId(tempId, temp);

			ids.append(tempId);

		}
		acedSSFree(aName);

		for (int i = 0; i < ids.length(); i++)
		{
			AcDbEntity * ent = NULL;

			if (acdbOpenObject(ent, ids[i], AcDb::kForWrite) == Acad::eOk) {

				ent->setVisibility(AcDb::Visibility::kInvisible, true);
				ent->close();
				
			}
		}

	}

	static void ECDMyGroupEcdCC() {
# if 0
		CString dicName = L"frame";
		struct resbuf rb;
		int i = acedGetVar(dicName, &rb);
		int m = 2;
		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)2;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}
#endif
		

		ads_point pt;
		int nums;
		double radius;

		if (acedGetPoint(NULL, L"\n圆心：\n", pt) != RTNORM) {
			return;
		}
		if (acedGetReal(L"\n半径：\n", &radius) != RTNORM) {
			return;
		}
		if (acedGetInt(L"\n多边形边数：\n", &nums) != RTNORM) {
			return;
		}

		AcGePoint3d ptc = asPnt3d(pt);

		AcDbObjectId polyId =CreatePolygon(AcGePoint2d(ptc.x, ptc.y), nums, radius, 0, 0);


		AcDbPolyline *pl = NULL;
		
#if 0
		AcDbPolyline *pl2 = NULL;
		if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			pl->setClosed(true);
			
			AcDbVoidPtrArray offsetCurves;
			

			double wide = radius*0.07;
			pl->getOffsetCurves(wide, offsetCurves);
			pl2 = (AcDbPolyline*)offsetCurves[0];
			pl->erase();
			pl->close();
			pl = NULL;
			pl2->setClosed(true);
			polyId = PostToModelSpace(pl2);
			pl2->close();
			pl2 = NULL;
		}
		AcGePoint2dArray ptArr;
		if (acdbOpenObject(pl2, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			AcGePoint2d ptStart;
			for (int i = 0; i < (int)pl2->numVerts(); i++)
			{

				AcGePoint2d pt;

				pl2->getPointAt(i, pt);

				ptArr.append(pt);

			}
			pl2->getPointAt(0, ptStart);
			ptArr.append(ptStart);
			pl2->erase();
			pl2->close();
			pl2 = NULL;
		}
#endif		
		AcGePoint2dArray ptArr;
		if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			AcGePoint2d ptStart;
			for (int i = 0; i < (int)pl->numVerts(); i++)
			{

				AcGePoint2d pt;

				pl->getPointAt(i, pt);

				ptArr.append(pt);

			}
			pl->getPointAt(0, ptStart);
			ptArr.append(ptStart);
			pl->erase();
			pl->close();
			pl = NULL;
		}
		AcDbWipeout *pWipeout = new AcDbWipeout();


		ErrorStatus es;// = wipeOut->fitPointsToImage(ptArr, minPt, scale);
		pWipeout->setDatabaseDefaults();
		AcGePoint3d originPnt(AcGePoint3d::kOrigin);
		AcGeVector3d Udirection(1, 0, 0);
		AcGeVector3d Vdirection(0, -1, 0);
		pWipeout->setOrientation(originPnt, Udirection, Vdirection);
		pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent, Adesk::kTrue);
		pWipeout->setDisplayOpt(AcDbRasterImage::kShow, Adesk::kTrue);
		es = pWipeout->setClipBoundaryToWholeImage();
		es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly, ptArr);

		if (es == ErrorStatus::eOk) {

			PostToModelSpace(pWipeout);
			pWipeout->close();
		}
#if 0
		i = acedGetVar(dicName, &rb);

		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)m;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}
#endif
	}

	static void ECDMyGroupEcdNN () {
#if 0
		CString dicName = L"frame";
		struct resbuf rb;
		int i = acedGetVar(dicName, &rb);
		int m = 2;
		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)2;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}
#endif
		

		AcDbObjectId polyId;
		 ECDMyPL(polyId);

		if (polyId.isNull()) {
			AfxMessageBox(L"创建多段线出错。");
			return;
		}

		AcDbPolyline *pl = NULL;
		AcGePoint2dArray ptArr;

		AcDbPolyline *pl2 = NULL;
		double dis = 0.0;

		if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			pl->setClosed(true);
			pl->close();
			pl = NULL;
			/*AcDbVoidPtrArray offsetCurves;
			double wide =dis*0.01;

			pl->getOffsetCurves(wide, offsetCurves);
			pl2 = (AcDbPolyline*)offsetCurves[0];
			pl->erase();
			pl->close();
			pl = NULL;
			pl2->setClosed(true);
			polyId = PostToModelSpace(pl2);
			pl2->close();
			pl2 = NULL;*/
		}
		
		if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			AcGePoint2d ptStart;
			for (int i = 0; i < (int)pl->numVerts(); i++)
			{

				AcGePoint2d pt;

				pl->getPointAt(i, pt);

				ptArr.append(pt);

			}
			pl2->getPointAt(0, ptStart);
			//ptArr.append(ptStart);
			pl->erase();
			pl->close();
			pl= NULL;
		}
		
		AcDbWipeout *pWipeout = new AcDbWipeout();

		
			ErrorStatus es;// = wipeOut->fitPointsToImage(ptArr, minPt, scale);
			pWipeout->setDatabaseDefaults();
			AcGePoint3d originPnt(AcGePoint3d::kOrigin);
			AcGeVector3d Udirection(1, 0, 0);
			AcGeVector3d Vdirection(0, -1, 0);
			pWipeout->setOrientation(originPnt, Udirection, Vdirection);
			pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent, Adesk::kTrue);
			pWipeout->setDisplayOpt(AcDbRasterImage::kShow, Adesk::kTrue);
			es = pWipeout->setClipBoundaryToWholeImage();
			es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly, ptArr);

		if (es == ErrorStatus::eOk) {

			PostToModelSpace(pWipeout);
			pWipeout->close();
			
		}
#if 0		
		 i= acedGetVar(dicName, &rb);
		
		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)m;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}
#endif
	}

	static  void ECDMyPL(AcDbObjectId & polyId ) {

		int index = 2;
		AcGePoint3d ptStart;
		ads_point pt1;
		if (acedGetPoint(NULL, L"\n请输入第一个点：", pt1) != RTNORM) {
			return ;
		}
		ptStart = asPnt3d(pt1);
		AcGePoint3d ptPrevious, ptCurrent;
		AcGePoint3d ptZj;
		ads_point ptCur;
		ptPrevious = ptStart;
		double *ptPre = asDblArray(ptPrevious);

		int rc;

		AcDbPolyline * pl = NULL;
		int plIndex = 0;

		rc = acedGetPoint(ptPre, L"\n请输入下一个点：", ptCur);

		while ( rc == RTNORM) {
			switch (rc) {
			case RTNORM:

				ptCurrent = asPnt3d(ptCur);

				if (index == 2) {
					pl = new AcDbPolyline();

					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptPrevious.x, ptPrevious.y));
					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptCurrent.x, ptCurrent.y));
					
					polyId = PostToModelSpace(pl);
					plIndex++;

					pl->close();
					pl = NULL;
				}
				else if (index > 2) {

					if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
						return;
					pl->addVertexAt(index - 1, AcGePoint2d(ptCurrent.x, ptCurrent.y), 0, 0, 0);
					plIndex++;
					pl->close();					
				}
				index++;
				ptPrevious = ptCurrent;

				ptPre = asDblArray(ptPrevious);
				break;
			}
			rc = acedGetPoint(ptPre, L"\n请输入下一个点:", ptCur);
		}
		
	}
	static 	int GetPointReturnCode(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
	{
		AcGePoint3d ucsBasePoint = basePoint;

		int nReturn = acedGetPoint(asDblArray(ucsBasePoint), prompt, asDblArray(point));

		if (nReturn == RTNORM) {

			//point = CConvertUtil::UcsToWcsPoint(point);
		}

		return nReturn;


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
	static AcDbObjectId CreatePolygon(AcGePoint2d ptCenter, int number,
		double radius, double rotation, double width)
	{
		AcGePoint2dArray points;
		double angle = 2 *PI / (double)number;
		for (int i = 0; i < number; i++)
		{
			AcGePoint2d pt;
			pt.x = ptCenter.x + radius * cos(i * angle);
			pt.y = ptCenter.y + radius * sin(i * angle);
			points.append(pt);
		}

		AcDbObjectId polyId = CreatePolyline(points, width);

		// 将其闭合
		AcDbEntity *pEnt;
		acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite);
		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
		if (pPoly != NULL)
		{
			pPoly->setClosed(Adesk::kTrue);
		}

		pEnt->close();
		
		return polyId;
	}

	static AcDbObjectId CreatePolyline(AcGePoint2dArray points, double width)
	{
		int numVertices = points.length();
		AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			pPoly->addVertexAt(i, points.at(i), 0, width, width);
		}
		AcDbObjectId polyId;
		polyId =PostToModelSpace(pPoly);
		return polyId;
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdZheZhaoApp)

//ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZheZhaoApp, ECDMyGroup, EcdNN, EcdNN, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZheZhaoApp, ECDMyGroup, EcdCC, EcdCC, ACRX_CMD_MODAL, NULL)

