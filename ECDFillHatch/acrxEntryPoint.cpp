#include "StdAfx.h"
#include "resource.h"
#include <vector>
#include <algorithm>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CECDFillHatchApp : public AcRxArxApp {

public:
	CECDFillHatchApp () : AcRxArxApp () {}

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
	
	
	static void ECDMyGroupECDAddHatch () {
		AddHatch();
	}
	static void ECDMyGroupECDSplitHatch() {

		ads_point pt;
		ads_name en,en2;
		ErrorStatus es;

		ads_point pt1, pt2;
		if (acedEntSel(TEXT("\n选择填充图案:"), en, pt) != RTNORM)  return;

		AcDbObjectId Id;
		acdbGetObjectId(Id, en);

		AcDbEntity *pEnt = NULL;
		acdbOpenObject(pEnt, Id, AcDb::kForWrite);

		AcDbHatch *pHatch = AcDbHatch::cast(pEnt);
		if (pHatch == NULL)
		{
			pEnt->close();
			return;
		}

		if (acedEntSel(TEXT("\n选择分割填充的曲线:"), en2, pt) != RTNORM) 
		{
			pEnt->close(); 
			return; }

		if (acedGetPoint(NULL, L"请在填充实体上指定一个点：\n", pt1) != RTNORM) 
		{
			pEnt->close();
			return; 
		}

		if (acedGetPoint(NULL, L"请在填充实体上指定另一个点：\n", pt2) != RTNORM)
		{
			pEnt->close();
			return;
		}


		AcDbObjectId Id2;
		acdbGetObjectId(Id2, en2);

		AcDbEntity *pEnt2 = NULL;
		es=acdbOpenObject(pEnt2, Id2, AcDb::kForWrite);

		vector<AcDbEntity*>vecBo;

		GetHatchBo(pHatch, vecBo);

		sort(vecBo.begin(), vecBo.end(), comp);

		AcGePoint3dArray ptArr;

		vecBo[0]->intersectWith(pEnt2, AcDb::kExtendArg, ptArr);

		if (ptArr.length() != 2) {
			acutPrintf(L"分割曲线应与最大边界有两个交点：%d\n", ptArr.length());
			pEnt->close();
			pEnt2->close();
			deleteVec(vecBo);
			return;
		}

		if (pEnt2->isA() == AcDbPolyline::desc()) {

			AcDbPolyline * plTemp = AcDbPolyline::cast(pEnt2);

			AcGeDoubleArray dbArr;
			AcGePoint2dArray pt2dArr;

			int indexS = 0, indexE = 0;

			GetCollOfPl(plTemp, dbArr, pt2dArr);
			//获得两个交点的索引用来构造新的多段线
			GetIndexOfPl(plTemp, pt2dArr, ptArr, indexS, indexE);

			AcDbPolyline *newPl = new AcDbPolyline();

			newPl->addVertexAt(newPl->numVerts(), AcGePoint2d(ptArr[0].x, ptArr[0].y), 0, 0, 0);

			for (int i=indexS;i<=indexE;i++)
			{
				newPl->addVertexAt(newPl->numVerts(), pt2dArr[i], dbArr[i], 0, 0);

			}
			newPl->addVertexAt(newPl->numVerts(), AcGePoint2d(ptArr[1].x, ptArr[1].y), 0, 0, 0);

			newPl->setColorIndex(4);

			PostToModelSpace(newPl);

			newPl->close();

		}

		AcDbVoidPtrArray ptrArr1;
		if (GetBoundary(asPnt3d(pt1), ptrArr1) != true) {

			pEnt->close();
			pEnt2->close();
			deleteVec(vecBo);
			return;

		}
		
		/*for (unsigned int i=0;i<vecBo.size();i++)
		{
			vecBo[i]->setColorIndex(i + 1);

			PostToModelSpace(vecBo[i]);

			vecBo[i]->close();

		}

		pEnt->erase();
		pEnt->close();
		pEnt2->erase();
		pEnt2->close();
		return;*/
		
		vector<AcDbEntity*>vecEntLeft, vecEntRight;
		
		AcDbObjectIdArray idsLeft,idsRight;

		if (ptrArr1.length() > 1) {

			PostBo(ptrArr1, idsLeft);

		}
		else if (ptrArr1.length() == 1)
		{
			AcDbEntity *pBo1 = (AcDbEntity*)ptrArr1[0];

			AcDbExtents ext;

			pBo1->getGeomExtents(ext);

			AcGePoint3d pBo1Pt1 = ext.maxPoint();
			AcGePoint3d pBo1Pt2 = ext.minPoint();

			AcGePoint3d zxPt = AcGePoint3d((pBo1Pt1.x + pBo1Pt2.x) / 2, (pBo1Pt1.y + pBo1Pt2.y) / 2, 0);


			AcGePoint3dArray tempArr1;
			AcDbExtents ext2;

			AcDbLine *l = new AcDbLine();

			l->setStartPoint(zxPt);

           for (unsigned int  i=1;i<vecBo.size();i++)
           {

			   pBo1->intersectWith(vecBo[i], AcDb::kOnBothOperands, tempArr1);

			   if (tempArr1.length() >= 1) {

				   tempArr1.removeAll();

				   continue;
			   }

			   vecBo[i]->getGeomExtents(ext2);

			   AcGePoint3d t1 = ext2.maxPoint();
			   AcGePoint3d t2= ext2.minPoint();

			   AcGePoint3d zxPt2 = AcGePoint3d((t1.x + t2.x) / 2, (t1.y + t2.y) / 2, 0);

			   l->setEndPoint(zxPt2);

			   l->intersectWith(pBo1, AcDb::kOnBothOperands, tempArr1);

			   if (tempArr1.length() >= 1) {

				   AcDbPolyline * p = NULL;
				   
				   
				   if (vecBo[i]->isA() == AcDbPolyline::desc()) {

					   p = AcDbPolyline::cast(vecBo[i]);

					   AcGePoint2dArray d2Arr;

					   AcGePoint2d ptTemp;

					   for (unsigned int m=0;m<p->numVerts();m++)
					   {
						   p->getPointAt(m, ptTemp);

						   d2Arr.append(ptTemp);

					   }

					   AcGePoint3dArray pt3dA;

					   points2dTo3d(d2Arr, pt3dA, 0);

					   if (JudgeXj(pt3dA, pBo1,asPnt3d(pt1))) {

						   vecEntRight.push_back(vecBo[i]);

					   }
					   else {
						   vecEntLeft.push_back(vecBo[i]);
					   }

				   }
				   tempArr1.removeAll();
			   }
			   else {
				   vecEntLeft.push_back(vecBo[i]);
			   }


           }

		   delete l;
		   l = NULL;

		   for (unsigned int j=0;j<vecEntLeft.size();j++)
		   {

			   idsLeft.append(PostToModelSpace(vecEntLeft[j]));
			   vecEntLeft[j]->close();


		   }
		   PostBo(ptrArr1, idsLeft);


		}
		CreateHatch(idsLeft, TEXT("ANGLE"), false, 1);



		AcDbVoidPtrArray ptrArr2;
		if (GetBoundary(asPnt3d(pt2), ptrArr2) != true) {

			pEnt->close();
			pEnt2->close();
			deleteVec(vecBo);
			return;

		}

	     
		if (ptrArr2.length() > 1) {
			PostBo(ptrArr2, idsRight);
		}
		else if (ptrArr2.length() == 1) {


			for (unsigned int j = 0; j < vecEntRight.size(); j++)
			{

				idsRight.append(PostToModelSpace(vecEntRight[j]));
				vecEntRight[j]->close();


			}
			PostBo(ptrArr2, idsRight);

		}
		CreateHatch(idsRight, TEXT("ANGLE"), false, 3);

		pEnt->erase();
		pEnt->close();
		pEnt2->erase();
		pEnt2->close();
	}


	static void AddHatch()
	{
		// 提示用户选择填充边界
		ads_name ss;
		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);
		AcDbObjectIdArray objIds;

		// 初始化填充边界的ID数组
		if (rt == RTNORM)
		{
			int length;
			acedSSLength(ss, &length);
			for (int i = 0; i < length; i++)
			{
				ads_name ent;
				acedSSName(ss, i, ent);
				AcDbObjectId objId;
				acdbGetObjectId(objId, ent);
				objIds.append(objId);
			}
		}
		// 释放选择集
		acedSSFree(ss);
		CreateHatch(objIds, TEXT("SOLID"), false,1);
	}

	static AcDbObjectId CreateHatch(AcDbObjectIdArray objIds, const ACHAR* patName, bool bAssociative,int colorIndex)
	{
		Acad::ErrorStatus es;
		AcDbHatch *pHatch = new AcDbHatch();

		// 设置填充平面
		AcGeVector3d normal(0, 0, 1);
		es=pHatch->setNormal(normal);
		es=pHatch->setElevation(0);

		// 设置关联性
		es=	pHatch->setAssociative(bAssociative);

		// 设置填充图案
		es=	pHatch->setPattern(AcDbHatch::kPreDefined, patName);
		pHatch->setColorIndex(colorIndex);

		AcDbObjectIdArray TempIds;
		// 添加填充边界
		for (int i=0;i<objIds.length();i++)
		{
			
			TempIds.append(objIds[i]);

			es = pHatch->appendLoop(AcDbHatch::kDefault, TempIds);
			TempIds.removeAll();

		}

		// 显示填充对象
		es = pHatch->evaluateHatch();

		// 添加到模型空间
		AcDbObjectId hatchId;
		hatchId = PostToModelSpace(pHatch);

		// 如果是关联性的填充，将填充对象与边界绑定，以便使其能获得边界对象修改的通知
		if (bAssociative)
		{
			AcDbEntity *pEnt;
			for (int i = 0; i < objIds.length(); i++)
			{
				es = acdbOpenAcDbEntity(pEnt, objIds[i], AcDb::kForWrite);
				if (es == Acad::eOk)
				{
					// 添加一个永久反应器
					pEnt->addPersistentReactor(hatchId);
					pEnt->close();
				}
			}
		}
		return hatchId;
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

	static void GetHatchBo(AcDbHatch *pHatch,vector<AcDbEntity*>&vecBo)
	{
		Acad::ErrorStatus es;

		Adesk::Int32 LoopType;

		AcGeVoidPointerArray edgeptrAry;
		AcGeIntArray edgeTypesAry;

		AcGePoint2dArray vertices;
		AcGeDoubleArray bulges;

		//获得填充边界的数目
		int LoopNum = pHatch->numLoops();

		for (int i = 0; i < LoopNum; i++)
		{
			//获取边界类型
			LoopType = pHatch->loopTypeAt(i);
			//如果边界是多义线
			if (LoopType & AcDbHatch::kPolyline)
			{
				//取得边界定义数据(polyline)的顶点数组和凸度数组，它们是一一对应的
				es = pHatch->getLoopAt(i, LoopType, vertices, bulges);
				acutPrintf(L"\n多段线");
				//是不是根据这些顶点数组和凸度数组构造一条AcDb多义线取决于你
				AcDbPolyline *pl = new AcDbPolyline();

				GetPolyline(vertices, bulges, pl);

				vecBo.push_back(pl);

				

			}
			else
			{
				//几乎可以取得除polyline外的所有边界定义数据
				//第三个参数返回值是无值指针数组
				//第四个参数返回值是组成边界的每一条边的类型
				//它们也是一一对应的关系
				es = pHatch->getLoopAt(i, LoopType, edgeptrAry, edgeTypesAry);

				//遍历，因为每一条边界又可能由多种AcGe曲线构成
				for (int j = 0; j < edgeTypesAry.length(); j++)
				{

					if (edgeTypesAry[j] == AcDbHatch::kLine)//直线
					{
						AcGeLineSeg2d *LnSeg = (AcGeLineSeg2d *)edgeptrAry[j];
						acutPrintf(L"\n直线");
						AcGePoint2d pt1 = LnSeg->startPoint();
						AcGePoint2d pt2 = LnSeg->endPoint();

						AcDbLine *line = new AcDbLine(AcGePoint3d(pt1.x,pt1.y,0), AcGePoint3d(pt2.x, pt2.y, 0));

						vecBo.push_back(line);


					}
					//圆弧
					else if (edgeTypesAry[j] == AcDbHatch::kCirArc)
					{
						AcGeCircArc2d *cirArc = (AcGeCircArc2d *)edgeptrAry[j];
						acutPrintf(L"\n圆弧");
						//可以根据数学圆弧构造相应的AcDb圆弧，取决于你（以下同）
						AcGePoint2d center = cirArc->center();
						double ra = cirArc->radius();
						double angle1 = cirArc->startAng();
						double angle2 = cirArc->endAng();
						AcDbCircle *cir = new AcDbCircle(AcGePoint3d(center.x, center.y, 0), AcGeVector3d::kZAxis, ra);

						vecBo.push_back(cir);

					}
					else if (edgeTypesAry[j] == AcDbHatch::kEllArc)//椭圆弧
					{
						AcGeEllipArc2d *ellArc = (AcGeEllipArc2d *)edgeptrAry[j];
						acutPrintf(L"\n椭圆弧");

						AcGePoint2d center = ellArc->center();
						AcGeVector2d majorVec = ellArc->majorAxis();
						double angle1 = ellArc->startAng();
						double angle2 = ellArc->endAng();
						double rad = ellArc->majorRadius();
						double rad2 = ellArc->minorRadius();
						

						AcDbEllipse *ell = new AcDbEllipse(AcGePoint3d(center.x, center.y, 0), 
							AcGeVector3d::kZAxis, AcGeVector3d(majorVec.x,majorVec.y,0), rad / rad2, angle1, angle2);
						vecBo.push_back(ell);
					}
					else if (edgeTypesAry[j] == AcDbHatch::kSpline)//NURBS曲线
					{
						AcGeNurbCurve2d *spline = (AcGeNurbCurve2d *)edgeptrAry[j];
						acutPrintf(L"\nNURBS曲线");

						AcDbSpline * spl = NULL;

						createSpline(*spline, spl, 0);

						vecBo.push_back(spl);

					}
				}

				
			}
		
			vertices.removeAll();
			bulges.removeAll();
			edgeptrAry.removeAll();
			edgeTypesAry.removeAll();

		
		}

		
	}

	static void GetPolyline(const AcGePoint2dArray & vertices, const AcGeDoubleArray & bulges, AcDbPolyline *pl) {

		for (int i=0;i<vertices.length();i++)
		{
			pl->addVertexAt(i, vertices[i], 0, 0, 0, 0);

		}
		for (int i = 0; i < bulges.length(); i++)
		{
			pl->setBulgeAt(i,bulges[i]);

		}


	}

	static Acad::ErrorStatus createSpline(const AcGeNurbCurve2d& crv, AcDbSpline *&spl, double z) {

		AcGe::EntityId type;

		if (crv.isDegenerate(type)) {

			return Acad::eDegenerateGeometry;
		}

		int degree;

		Adesk::Boolean rational, periodic, closed;
		AcGeKnotVector knots;
		AcGePoint2dArray controlPoints2d;
		AcGePoint3dArray controlPoints;
		AcGeDoubleArray weights;
		int i;

		crv.getDefinitionData(degree, rational, periodic, knots, controlPoints2d, weights);
		closed = crv.isClosed();
		points2dTo3d(controlPoints2d, controlPoints, z);

		AcGeDoubleArray splknots;
		for (i = 0; i < knots.length(); i++)
		{
			splknots.append(knots[i]);
		}
			spl = new AcDbSpline(degree, rational, closed, periodic, controlPoints, splknots, weights);

			if (spl->isNull()) {

				return Acad::eNotNewlyCreated;
			}
			else {
				return Acad::eOk;
			}

	}
	static void points2dTo3d(const AcGePoint2dArray & controlPoints2d,
		AcGePoint3dArray & controlPoints,double z) {

		for (int i=0;i<controlPoints2d.length();i++)
		{

			controlPoints.append(AcGePoint3d(controlPoints2d[i].x, controlPoints2d[i].y, z));

		}

	}

	static bool comp(AcDbEntity * ent1, AcDbEntity *ent2) {
		AcDbCurve * c1 = (AcDbCurve*)ent1;
		AcDbCurve * c2 = (AcDbCurve*)ent2;

		double c1A = 0.0, c2A = 0.0;

		c1->getArea(c1A);
		c2->getArea(c2A);

		return c1A > c2A;

	}
	
	static void deleteVec(vector<AcDbEntity*>vecBo) {

		for (size_t i=0;i<vecBo.size();i++)
		{
			delete vecBo[i];
			vecBo[i] = NULL;

		}
	}

	static void GetCollOfPl(AcDbPolyline *pl, AcGeDoubleArray &dbArr, AcGePoint2dArray& pt2dArr) {

		double bulge = 0;
		AcGePoint2d temp;
		for (unsigned int i=0;i<pl->numVerts();i++)
		{
			pl->getPointAt(i, temp);
			pl->getBulgeAt(i, bulge);

			dbArr.append(bulge);
			pt2dArr.append(temp);

		}

	}

	static void GetIndexOfPl(AcDbPolyline *pl, const AcGePoint2dArray& pt2dArr, AcGePoint3dArray& pt3dArr,
		int & indexS, int &indexE) {

		double dis1 = 0.0, dis2 = 0.0 ,temp;
		pl->getDistAtPoint(pt3dArr[0], dis1);
		pl->getDistAtPoint(pt3dArr[1], dis2);

		temp = dis1;
		if (dis1 > dis2) {

			dis1 = dis2;
			dis2 = temp;

			pt3dArr.reverse();

		}

		

		for (int i=0;i<pt2dArr.length();i++)
		{

			AcGePoint3d pt3dT = AcGePoint3d(pt2dArr[i].x, pt2dArr[i].y, 0);

			double td = 0.0;

			pl->getDistAtPoint(pt3dT, td);

			if (td >= dis1) {
				indexS = i;

				break;
			}
		}

		for (int i=indexS;i<pt2dArr.length();i++)
		{

			AcGePoint3d pt3dT = AcGePoint3d(pt2dArr[i].x, pt2dArr[i].y, 0);

			double td = 0.0;

			pl->getDistAtPoint(pt3dT, td);

			if (td >= dis2) {
				indexE = i-1;

				break;
			}
		}

		
	}
	
	static bool  GetBoundary(const AcGePoint3d & seedPoint, AcDbVoidPtrArray& ptrArr)
	{

		ErrorStatus es = acedTraceBoundary(seedPoint, false, ptrArr);

		if (es != Acad::eOk) {
			acutPrintf(L"\nboundary=%d", es);
			return false;
		}
		return true;
	}
	static void  PostBo(const AcDbVoidPtrArray &ptrArr, AcDbObjectIdArray &ids) {

		for (int i=0;i<ptrArr.length();i++)
		{
			AcDbEntity* pEnt = (AcDbEntity*)ptrArr[i];

			ids.append(PostToModelSpace(pEnt));
			pEnt->close();

		}


	}
	static bool JudgeXj(CONST AcGePoint3dArray & ptArr, AcDbEntity *pEnt,const AcGePoint3d &innerPt) {

		AcDbLine * l = new AcDbLine();

		l->setStartPoint(innerPt);

		AcGePoint3dArray ptTemp;

		for (int i = 0; i < ptArr.length(); i++)
		{
			l->setEndPoint(ptArr[i]);

			l->intersectWith(pEnt, AcDb::kOnBothOperands, ptTemp);

			if (ptTemp.length() >= 1) {
				
				delete l;
				l = NULL;

				return true;
			}

			
		}

		return false;
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CECDFillHatchApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CECDFillHatchApp, ECDMyGroup, ECDAddHatch, ECDAddHatch, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CECDFillHatchApp, ECDMyGroup, ECDSplitHatch, ECDSplitHatch, ACRX_CMD_MODAL, NULL)



