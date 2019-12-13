//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"RunCommand.h"
#include"EllipseUtil.h"
#include"SplieUtil.h"
#include"PolylineUtil.h"
#include"MathUtil.h"
#include"RegionUtil.h"
#include"TextUtil.h"
#include"HatchUtil.h"
#include"Dimension.h"
#include"GePointUtil.h"
#include"LineUtil.h"
#include"CircleUtil.h"
#include"TransUtil.h"
#include"BlockUtil.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject1App : public AcRxArxApp {

public:
	CArxProject1App () : AcRxArxApp () {}

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
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupMyCommand () {
		// Put your command code here
		AfxMessageBox(TEXT("Hello World!"));
	}
	//创建直线
	static void ECDMyGroupMyLine() {
		
		RunCommand run= RunCommand();

		AcDbObjectId lineId = run.CreateLine();

		run.ChangeColor(lineId, 1);

	}

	static void ECDMyGroupAddEllipse(){

		AcGeVector3d vecNormal(0, 0, 1);

		AcGeVector3d majorAxis(40, 0, 0);

		AcDbObjectId entId;

		entId = CEllipseUtil::Add(AcGePoint3d::kOrigin, vecNormal, majorAxis, 0.5);

		AcGePoint2d pt1(60, 80), pt2(140, 120);

		CEllipseUtil::Add(pt1, pt2);

	}
	
	static void ECDMyGroupAddSpline() {

		AcGePoint3d pt1(0, 0, 0), pt2(10, 30, 0), pt3(60, 80, 0), pt4(100, 100, 0);

		AcGePoint3dArray points;

		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);

		CSplieUtil::Add(points, 4, 0.0);

		pt2.set(30, 10, 0);
		pt3.set(80, 60, 0);
		points.removeSubArray(0, 3);

		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);

		AcGeVector3d startTangent(5, 1, 0);
		AcGeVector3d endTangent(5, 1, 0);

		CSplieUtil::Add(points, startTangent, endTangent, 4, 0.0);

	}
	
	static void ECDMyGroupAddPolyline() {
	
		AcGePoint2d &ptCenter = AcGePoint2d(200, 200);

		AcGePoint2d &ptCenter2 = AcGePoint2d(300, 300);

		double rotation = 45.0 / 180 * CMathUtil::PI();

		CPolylineUtil::AddPolygon(ptCenter, 6, 30, rotation, 1);

		CPolylineUtil::AddPolyCircle(ptCenter2, 40, 1);
	}
	
	static void ECDMyGroupAddRegion() {

		ads_name ss;

		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);

		AcDbObjectIdArray objIds;

		if (rt == RTNORM) {

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
		acedSSFree(ss);

		AcDbObjectIdArray regionIds;

		regionIds = CRegionUtil::Add(objIds);

		int number = regionIds.length();

		acutPrintf(TEXT("\n已经创建了%d个面域"), number);

	}
	
	static void ECDMyGroupAddText() {

		AcGePoint3d ptInsert(0, 4, 0);

		CTextUtil::AddText(ptInsert, TEXT("LL_ABC"));

		ptInsert.set(0, 0, 0);

		CTextUtil::AddMText(ptInsert, TEXT("多行文字\n123\n"));

	}

	static void ECDMyGroupAddHatch() {

		ads_name ss;

		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);

		AcDbObjectIdArray objIds;

		if (rt == RTNORM) {

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
		acedSSFree(ss);

		CHatchUtil::Add(objIds, TEXT("BOX"), 0.1);
	}

	static void ECDMyGroupAddDimension() {
		// 指定起始点位置
		AcGePoint3d pt1(200, 160, 0);
		AcGePoint3d pt2 = CGePointUtil::RelativePoint(pt1, -40, 0);
		AcGePoint3d pt3 = CGePointUtil::PolarPoint(pt2,
			7 * CMathUtil::PI() / 6, 20);
		AcGePoint3d pt4 = CGePointUtil::RelativePoint(pt3, 6, -10);
		AcGePoint3d pt5 = CGePointUtil::RelativePoint(pt1, 0, -20);

		// 绘制外轮廓线
		CLineUtil::Add(pt1, pt2);
		CLineUtil::Add(pt2, pt3);
		CLineUtil::Add(pt3, pt4);
		CLineUtil::Add(pt4, pt5);
		CLineUtil::Add(pt5, pt1);

		// 绘制圆形
		AcGePoint3d ptCenter1, ptCenter2;
		ptCenter1 = CGePointUtil::RelativePoint(pt3, 16, 0);
		ptCenter2 = CGePointUtil::RelativePoint(ptCenter1, 25, 0);
		CCircleUtil::Add(ptCenter1, 3);
		CCircleUtil::Add(ptCenter2, 4);

		AcGePoint3d ptTemp1, ptTemp2;
		// 水平标注
		ptTemp1 = CGePointUtil::RelativePoint(pt1, -20, 3);
		CDimension::AddDimRotated(0,pt1, pt2, ptTemp1,NULL);

		// 垂直标注
		ptTemp1 = CGePointUtil::RelativePoint(pt1, 4, 10);
		CDimension::AddDimRotated(CMathUtil::PI() / 2,pt1, pt5, ptTemp1,NULL);

		// 转角标注
		ptTemp1 = CGePointUtil::RelativePoint(pt3, -3, -6);
		CDimension::AddDimRotated(7 * CMathUtil::PI() / 4,pt3, pt4, ptTemp1,NULL);

		// 对齐标注
		ptTemp1 = CGePointUtil::RelativePoint(pt2, -3, 4);
		CDimension::AddDimAligned(pt2, pt3, ptTemp1,
			AcGeVector3d(4, 10, 0), TEXT("new position"));

		// 角度标注
		ptTemp1 = CGePointUtil::RelativePoint(pt5, -5, 5);
		CDimension::AddDim3PtAngular(pt5, pt1, pt4, ptTemp1);

		// 半径标注
		ptTemp1 = CGePointUtil::PolarPoint(ptCenter1,
			CMathUtil::PI() / 4, 3);
		CDimension::AddDimRadial(ptCenter1, ptTemp1, -3);

		// 直径标注
		ptTemp1 = CGePointUtil::PolarPoint(ptCenter2, CMathUtil::PI() / 4, 4);
		ptTemp2 = CGePointUtil::PolarPoint(ptCenter2, CMathUtil::PI() / 4, -4);
		CDimension::AddDimDiametric(ptTemp1, ptTemp2, 0);

		// 坐标标注
		CDimension::AddDimOrdinate(ptCenter2, AcGeVector3d(0, -10, 0),
			AcGeVector3d(10, 0, 0));

	}

	static void ECDMyGroupMyDrag() {

		ads_name  entName;
		ads_point pt;

		if (acedEntSel(TEXT("请选择圆弧"), entName, pt) != RTNORM) {
			return;
		}
		//获得对象的指针
		AcDbObjectId arcId = NULL;
		if (acdbGetObjectId(arcId, entName) != ErrorStatus::eOk)
			return;
		
		//通过id获得实体对象
		AcDbEntity *pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, arcId, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
			return;

		//判断实体对象是否是圆弧
		if (!pEnt->isKindOf(AcDbArc::desc())) {

			pEnt->close();
			return;

		}

		//将实体转换为圆弧
		AcDbArc *pArc = AcDbArc::cast(pEnt);
		
		AcGePoint3d ptCenter = pArc->center();
		AcGePoint3d ptStart,ptEnd,ptMiddle;
		pArc->getStartPoint(ptStart);
		pArc->getEndPoint(ptEnd);

		double length = 0.0;
		pArc->getDistAtPoint(ptEnd,length);
		pArc->getPointAtDist(length / 2, ptMiddle);

		pEnt->close();

		//创建三点角度标注
		CString strLength;
		strLength.Format(_T("%.2f"), length);

		AcDbObjectId dimId;

		dimId = CDimension::AddDim3PointAngular(ptCenter, ptStart, ptEnd, ptMiddle, strLength, NULL);

		//拖动鼠标改变标注文字的位置
		AcGePoint3d ptText;
		int track = 1, type;
		struct resbuf result;

		while (track > 0) {

			acedGrRead(track, &type, &result);

			ptText.x = result.resval.rpoint[X];
			ptText.y = result.resval.rpoint[Y];

			//重新设置dim的文字标注位置

			if (acdbOpenAcDbEntity(pEnt, dimId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

				AcDb3PointAngularDimension *dim3;

				if (pEnt->isKindOf(AcDb3PointAngularDimension::desc())) {

					dim3 = AcDb3PointAngularDimension::cast(pEnt);

					dim3->setTextPosition(ptText);
				}

				pEnt->close();
			}

			if (type == 3) {

				track = 0;

			}
		}
	}

	static void ECDMyGroupMyDrag2() {

		ads_name entName;
		ads_point ptBase, ptPick;

		if (acedEntSel(_T("请选择单行文字"), entName, ptPick) != RTNORM)
			return;
		
		AcDbObjectId textId;
		if (acdbGetObjectId(textId, entName) != Acad::eOk)
			return;

		AcDbEntity *pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, textId, AcDb::OpenMode::kForRead) != Acad::eOk)
			return;
		
		AcDbText *t;

		if (!pEnt->isKindOf(AcDbText::desc())) {
			pEnt->close();
			return;
		}
		t = AcDbText::cast(pEnt);
		
		AcGePoint3d ptInsertOld=t->position();
         
		pEnt->close();

		if (acedGetPoint(NULL, _T("\n选择基点"), ptBase) != RTNORM)
			return;


		acedPrompt(_T("\n请输入第二个点"));
		
		AcGePoint3d ptInsertNew(0, 0, 0);
		AcGePoint3d ptPick3d = asPnt3d(ptBase);

		int track = 1;
		int type;
		struct resbuf result;

		while (track > 0) {

			acedGrRead(track, &type, &result);

			if (acdbOpenAcDbEntity(pEnt, textId, AcDb::OpenMode::kForWrite) == Acad::eOk) {

				AcDbText *text = AcDbText::cast(pEnt);

				if (text != NULL) {

					ptInsertNew.x = result.resval.rpoint[X] - (ptPick3d.x - ptInsertOld.x);
					ptInsertNew.y= result.resval.rpoint[Y] - (ptPick3d.y - ptInsertOld.y);

					text->setPosition(ptInsertNew);

				}
				pEnt->close();

			}

			if (type == 3) {
				track = 0;
			}

		}




	}

	static void ECDMyGroupMyTrans() {

		AcDbObjectId oId=CTransUtil::GetId(TEXT("选择"));

		AcDbObjectId newId=CTransUtil::MyClone(oId);

		ads_point basePt;
		//ads_point movePt;

		acedGetPoint(NULL, _T("请输入基点"), basePt);
		//acedGetPoint(NULL, _T("请输入终点"), movePt);

		AcGePoint3d pt1, pt2;
		pt1 = asPnt3d(basePt);
		//pt2 = asPnt3d(movePt);

		//CTransUtil::Move(pt1, pt2, oId);

		//CTransUtil::Rotate(pt1, CMathUtil::PI() / 2, oId);
		CTransUtil::Scale(newId,3,pt1);

	}

	static void ECDMyGroupMyBlk() {

		AcDbObjectId recId=BlockUtil::CreateBlk();

		//BlockUtil::InsertBlk(recId);

		//wchar_t brName[40];

		//acedGetString(1, L"\n请输入块名：", brName);

		//BlockUtil::InsertBlk(brName);

    	BlockUtil::InsertBlkWidthAttr(recId, AcGePoint3d(0, 0, 0));

		AcDbBlockReference *br = NULL;

		AcDbObjectId brId = CTransUtil::GetId(L"选择块参照");
		AcDbEntity *pEnt = NULL;

		if (acdbOpenObject(pEnt, brId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			br = AcDbBlockReference::cast(pEnt);

			if (br != NULL) {


				BlockUtil::SetAttribute2Br(br, _T("周长"), _T("10"));
				BlockUtil::SetAttribute2Br(br, _T("面积"), _T("200"));

				br->close();
				

			}
			pEnt->close();

		}


	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject1App)
//創建命令的宏
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyLine, MyLine, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddEllipse, AddEllipse, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddSpline, AddSpline, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddPolyline, AddPolyline, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddRegion, AddRegion, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddText, AddText, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddHatch, AddHatch, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, AddDimension, AddDimension, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyDrag, MyDrag, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyDrag2, MyDrag2, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyTrans, MyTrans, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, ECDMyGroup, MyBlk, MyBlk, ACRX_CMD_MODAL, NULL)