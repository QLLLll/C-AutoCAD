#include "StdAfx.h"
#include "resource.h"
#include"GetInputUtil.h"
#include"SelectUtil.h"
#include"AreaFenGe.h"
#include"DwgDataBaseUtil.h"
#include"acedCmdNF.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAreaDengFenApp : public AcRxArxApp {

public:
	CAreaDengFenApp() : AcRxArxApp() {}

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


	static void ECDMyGroupMyCommand() {


		AcRxClass *cls = AcDbPolyline::desc();

		AcGePoint3d pickPt, basePoint, secondPoint;

		AcDbPolyline *pEnt = NULL;

		AcDbLine * line = NULL;

		/*ACHAR  fenGeStr[132];

		if (acedGetString(0, L"\n请输入分割比例(总和为1，逗号分割):\n", fenGeStr, 132) != RTNORM)
			return;*/

		ACHAR *fenGeStr = L"0.2,0.2,0.2,0.4";

		if (CSelectUtil::PromptSelectEntity(L"\n请选择要分割的封闭多段线：", cls, (AcDbEntity*&)pEnt, pickPt) != true)
		{
			return;
		}

		if (CGetInputUtil::GetPoint(L"\n请在封闭曲线一侧输入一个点：", basePoint)) {

			if (CGetInputUtil::GetPoint(basePoint, L"\n输入第二个点：", secondPoint)) {

				line = new AcDbLine(basePoint, secondPoint);
			}

		}

		if (line == NULL)
		{

			delete pEnt;
			pEnt = NULL;
			return;
		}

		CAreaFenGe *areaFg = new CAreaFenGe(fenGeStr, pEnt, line);
		areaFg->Command();
		delete areaFg;
		areaFg = NULL;

	}


	static void  ECDMyGroupMyBd() {

		AcGePoint3d basePoint;

		if (CGetInputUtil::GetPoint(L"\n请在封闭曲线一侧输入一个点：", basePoint)) {

			AcDbVoidPtrArray ptrArr;

			/*ErrorStatus es = acedTraceBoundary(basePoint, false, ptrArr);

			if (es != Acad::eOk) {
				acutPrintf(L"\n%d", es);
			}

			AcDbPolyline * pl = (AcDbPolyline*)ptrArr[0];

			pl->setColorIndex(1);

			CDwgDataBaseUtil::PostToModelSpace(pl);

			pl->close();*/

			ads_name pent;
			ads_point pt = {basePoint.x,basePoint.y,basePoint.z};
			acedCommandS(RTSTR, L"-boundary", RTPOINT, pt, RTSTR, L"", RTNONE);
			acdbEntLast(pent);
			//acedRedraw(pent, 3);
			AcDbObjectId eId;

			acdbGetObjectId(eId, pent);

			AcDbEntity * pEnt = NULL;

			ErrorStatus es = acdbOpenObject(pEnt, eId, AcDb::OpenMode::kForWrite);

			if (es != Acad::eOk) {
				acutPrintf(L"\npEnt=%d", es);
				AcDbObject *obj = NULL;
				 es = acdbOpenAcDbObject(obj, eId, AcDb::OpenMode::kForWrite);
				if (es != Acad::eOk) {
					acutPrintf(L"\nobj=%d", es);
					return;
				}
				if (obj->isKindOf(AcDbRegion::desc())){

					acutPrintf(L"\nregion");
				}
				
			}
			pEnt->setColorIndex(1);
			pEnt->close();
		}

	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAreaDengFenApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CAreaDengFenApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAreaDengFenApp, ECDMyGroup, MyBd, MyBd, ACRX_CMD_MODAL, NULL)

