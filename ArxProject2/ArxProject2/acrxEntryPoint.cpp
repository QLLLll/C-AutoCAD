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
