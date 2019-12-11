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