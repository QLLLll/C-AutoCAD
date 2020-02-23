//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"DwgDataBaseUtil.h"
#include<vector>
using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")


struct StLine
{
	AcGePoint2d ptPre;

	AcGePoint2d ptNext;

};

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCheckPolylineApp : public AcRxArxApp {

public:
	CCheckPolylineApp() : AcRxArxApp() {}

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


	static void ECDMyGroupEcdPlZj() {

		ads_name sName;
		ads_point sPt;
		ErrorStatus es;
	if (acedEntSel(L"选择多段线：\n", sName, sPt) == RTNORM){

		AcDbObjectId oId;

		es=	acdbGetObjectId(oId, sName);

		if (es != ErrorStatus::eOk) {

			acutPrintf(L"打开objectid失败es=%d\n", es);

			return;

		}

		AcDbPolyline * pl = NULL;
		
		
		es = acdbOpenObject(pl, oId, AcDb::kForRead);


		if (es != ErrorStatus::eOk) {
			acutPrintf(L"打开实体失败es=%d\n", es);

			return;
		}

		AcGePoint2dArray pt2dArr;

		AcGePoint2d temPt;

		vector<StLine> vecLines;

		for (int i=0;i<(int)pl->numVerts();i++)
		{
			pl->getPointAt(i, temPt);

			pt2dArr.append(temPt);

		}
		pl->close();
		for (int i=0;i<pt2dArr.length()-1;i++)
		{
			StLine stLine;

			stLine.ptPre = pt2dArr[i];

			stLine.ptNext = pt2dArr[i + 1];

			vecLines.push_back(stLine);
		}

		AcGePoint3d tempPt3d;

		AcGePoint3dArray p3dArr;
		for (int i=0;i<(int)vecLines.size();i++)
		{

			for (int j=i+1;j<(int)vecLines.size();j++)
			{

				if (JiaoDian(vecLines[i], vecLines[j], tempPt3d)) {


					p3dArr.append(tempPt3d);
				}

			}

		}


		for (int i=0;i<p3dArr.length();i++)
		{
			AcDbCircle *circle = new AcDbCircle(p3dArr[i], AcGeVector3d::kZAxis, 0.5);

			circle->setColorIndex(2);

			CDwgDataBaseUtil::PostToModelSpace(circle);

			circle->close();


		}

}

	}

	static bool JiaoDian(StLine &l1, StLine &l2, AcGePoint3d& ptCenter) {

		double x0, y0, x1, y1, x2, y2, x3, y3;

		x0 = l1.ptPre.x;
		y0 = l1.ptPre.y;

		x1 = l1.ptNext.x;
		y1 = l1.ptNext.y;

		x2= l2.ptPre.x;
		y2 = l2.ptPre.y;

		x3 = l2.ptNext.x;
		y3 = l2.ptNext.y;


		if (x0 - x1 == 0) {
			if (x2 - x3 == 0)
			{
				if (IsEqual(l1.ptPre, l2.ptPre, 1e-4) ||
					IsEqual(l1.ptPre, l2.ptNext, 1e-4) ||
					IsEqual(l1.ptNext, l2.ptPre, 1e-4) ||
					IsEqual(l1.ptNext, l2.ptNext, 1e-4)) {

					acutPrintf(L"存在重叠的部分\n");
					return false;
				}
				else {
					acutPrintf(L"存在平行的部分\n");
					return false;

				}
			}
			else {

				double k2 = (y3 - y2) / (x3 - x2);

				double b2 = (y2 - k2*x2);

				AcGePoint2d jd;

				jd.x = x0;
				jd.y = k2*x0 + b2;

				double distance = l1.ptPre.distanceTo(l1.ptNext);
				double dis1 = jd.distanceTo(l1.ptPre);
				double dis2 = jd.distanceTo(l1.ptNext);

				if (dis1 < distance&&dis2 < distance) {

					if (IsEqual(l1.ptPre, jd, 1e-4) ||
						IsEqual(jd, l2.ptNext, 1e-4) ||
						IsEqual(l1.ptNext, jd, 1e-4) ||
						IsEqual(jd, l2.ptPre, 1e-4)) {

						return false;
					}

					ptCenter.x = jd.x;
					ptCenter.y = jd.y;
					ptCenter.z = 0;

					return true;

				}
				else {
					return false;
				}
			}
		}
		else if (x3 - x2 == 0) {
		if (x0 - x1 != 0)
		{
			double k2 = (y1 - y0) / (x1 - x0);

			double b2 = (y1 - k2*x1);

			AcGePoint2d jd;

			jd.x = x3;
			jd.y = k2*x3 + b2;

			double distance = l1.ptPre.distanceTo(l1.ptNext);
			double dis1 = jd.distanceTo(l1.ptPre);
			double dis2 = jd.distanceTo(l1.ptNext);

			if (dis1 < distance&&dis2 < distance) {

				if (IsEqual(l1.ptPre, jd, 1e-4) ||
					IsEqual(jd, l2.ptNext, 1e-4) ||
					IsEqual(l1.ptNext, jd, 1e-4) ||
					IsEqual(jd, l2.ptPre, 1e-4)) {

					return false;
				}

				ptCenter.x = jd.x;
				ptCenter.y = jd.y;
				ptCenter.z = 0;

				return true;

			}
			//交点在延长线上
			else {
				return false;
			}


		}
		else {
			if (IsEqual(l1.ptPre, l2.ptPre, 1e-4) ||
				IsEqual(l1.ptPre, l2.ptNext, 1e-4) ||
				IsEqual(l1.ptNext, l2.ptPre, 1e-4) ||
				IsEqual(l1.ptNext, l2.ptNext, 1e-4)) {

				acutPrintf(L"存在重叠的部分\n");
				return false;
			}
			else {
				acutPrintf(L"存在平行的部分\n");
				return false;

			}

		}

		}

		
		else {

			double k1 = (y1 - y0) / (x1 - x0);

			double b1 = (y1 - k1*x1);

			double k2 = (y3 - y2) / (x3 - x2);

			double b2 = (y2 - k2*x2);

			AcGePoint2d jd;

			if (k1 == k2) {


				return false;


			}

			double x0 = (b2 - b1) / (k1 - k2);
			jd.x = x0;

			jd.y = k1*x0 + b1;

			double distance = l1.ptPre.distanceTo(l1.ptNext);
			double dis1 = jd.distanceTo(l1.ptPre);
			double dis2 = jd.distanceTo(l1.ptNext);

			if (dis1 < distance&&dis2 < distance) {

				if (IsEqual(l1.ptPre, jd, 1e-4) ||
					IsEqual(jd, l2.ptNext, 1e-4) ||
					IsEqual(l1.ptNext, jd, 1e-4) ||
					IsEqual(jd, l2.ptPre, 1e-4)) {

					return false;
				}

				ptCenter.x = jd.x;
				ptCenter.y = jd.y;
				ptCenter.z = 0;

				return true;


			}
			else {
				return false;
			}
		}

  }
	static	bool IsEqual(const AcGePoint2d & firstPoint, const AcGePoint2d & secondPoint, double tol)
	{
		return (fabs(firstPoint.x - secondPoint.x) < tol&&
			fabs(firstPoint.y - secondPoint.y) < tol);
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCheckPolylineApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CCheckPolylineApp, ECDMyGroup, EcdPlZj, EcdPlZj, ACRX_CMD_MODAL, NULL)


