//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"AlgoHelper.h"
#include"SelectUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
#include"DwgDataBaseUtil.h"
#include"GePointUtil.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CMultiDaoJiaoApp : public AcRxArxApp {

public:
	CMultiDaoJiaoApp() : AcRxArxApp() {}

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

	static void ECDMyGroupEcdDaojiao() {
		// Put your command code here

		AcRxClass *cls = AcDbPolyline::desc();
		double tol = 1e-4;
		while (true) {

			AcDbEntity* pEnt = NULL;
			AcGePoint3d ptSelect;

			if (CSelectUtil::PromptSelectEntity(TEXT("请选择要倒角的多段线："), cls, pEnt, ptSelect, true)) {

				double radius = 0.1;




				if (CGetInputUtil::GetReal(TEXT("请输入半径："), 0.1, 2, radius)) {

					AcDbPolyline* pl = (AcDbPolyline*)pEnt;

					vector<AcGePoint2d>vecPt;
					vector<double>vecBu;

					if (pl->isClosed()) {

						AcGePoint2d ptFirst;
						pl->getPointAt(0, ptFirst);

						int lastIndex = pl->numVerts() - 1;
						
						//判断最后个点是否和第一个点重合
						AcGePoint2d ptEnd;
						pl->getPointAt(lastIndex, ptEnd);

						if (CGePointUtil::IsEqual(ptFirst, ptEnd, 1e-4)) {

							lastIndex -= 1;

						}

						for (int i = 0; i <= lastIndex; i++)
						{
							AcGePoint2d tempPt1, tempPt2, tempPt3;

							pl->getPointAt(((lastIndex + 1) + i - 1) % (lastIndex + 1), tempPt1);
							pl->getPointAt(i, tempPt2);
							pl->getPointAt((i + 1)%(lastIndex+1), tempPt3);


							AcGeVector2d vec1 = tempPt1 - tempPt2;
							AcGeVector2d vec2 = tempPt3 - tempPt2;
							AcGePoint2d pt1, pt2;

							AlgoHelper::Fillet(tempPt2,
								vec1, vec2, radius, pt1, pt2);

							if (vec2.angle() > vec1.angle()) {
								AlgoHelper::Fillet(tempPt2,
									vec1, vec2, radius, pt1, pt2);
								vecPt.push_back(pt1);
								vecPt.push_back(pt2);
							}
							else {
								AlgoHelper::Fillet(tempPt2,
									vec2, vec1, radius, pt1, pt2);
								vecPt.push_back(pt2);
								vecPt.push_back(pt1);
							}

							double bulge = getBulge(vec1, vec2, 1e-4);

							vecBu.push_back(bulge);


						}
						AcDbPolyline *plNew = new AcDbPolyline();

						for (int i = 0; i < (int)vecPt.size(); i++)
						{

							plNew->addVertexAt(i, vecPt[i], 0, 0, 0);

						}
						plNew->setClosed(true);



							for (int i = 0; i < (int)vecBu.size(); i++)
							{
								plNew->setBulgeAt(i * 2, vecBu[i]);

							}

						plNew->setColorIndex(1);
						CDwgDataBaseUtil::PostToModelSpace(plNew);
						plNew->close();

						pl->close();


					}
					//非封闭多段线的情况
					else {

						AcGePoint2d ptFirst;
						pl->getPointAt(0, ptFirst);
						vecPt.push_back(ptFirst);

						//多段线的点小于3个不倒角
						if (pl->numVerts() < 3) {
							pl->close();
							continue;
						}
						else {
							for (int i = 1; i < (int)pl->numVerts() - 1; i++)
							{
								AcGePoint2d tempPt1, tempPt2, tempPt3;

								pl->getPointAt(i - 1, tempPt1);
								pl->getPointAt(i, tempPt2);
								pl->getPointAt(i + 1, tempPt3);


								AcGeVector2d vec1 = tempPt1 - tempPt2;
								AcGeVector2d vec2 = tempPt3 - tempPt2;
								AcGePoint2d pt1, pt2;



								if (vec2.angle() > vec1.angle()) {
									AlgoHelper::Fillet(tempPt2,
										vec1, vec2, radius, pt1, pt2);
									vecPt.push_back(pt1);
									vecPt.push_back(pt2);
								}
								else {
									AlgoHelper::Fillet(tempPt2,
										vec2, vec1, radius, pt1, pt2);
									vecPt.push_back(pt2);
									vecPt.push_back(pt1);
								}

								double bulge = getBulge(vec1, vec2, 1e-4);

								vecBu.push_back(bulge);

							}

							AcGePoint2d ptEnd;
							pl->getPointAt(pl->numVerts() - 1, ptEnd);
							vecPt.push_back(ptEnd);

							AcDbPolyline *plNew = new AcDbPolyline();

							for (int i = 0; i < (int)vecPt.size(); i++)
							{

								plNew->addVertexAt(i, vecPt[i], 0, 0, 0);

							}

							for (int i = 0; i < (int)vecBu.size(); i++)
							{
								plNew->setBulgeAt(i * 2 + 1, vecBu[i]);

							}

							plNew->setColorIndex(1);
							CDwgDataBaseUtil::PostToModelSpace(plNew);
							plNew->close();

							pl->close();

						}
					}

				}
				else {
					pEnt->close();
					continue;
				}
			}
			else {
				break;
			}
		}

	}
	//得到凸度
	static double getBulge(AcGeVector2d vec1, AcGeVector2d vec2, double tol) {


		double bulge = 0.0;

		if (fabs(vec1.x) <= tol&&vec1.y <= 0) {

			if (vec2.x >= 0 && vec2.y <= 0) {

				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x <= 0 && (vec2.y <= 0 || fabs(vec2.y) <= tol)) {

				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x >= 0 && vec2.y >= 0) {

				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x <= 0 && vec2.y >= 0) {

				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}

		}
		else if (fabs(vec1.y) <= tol&&vec1.x >= 0) {

			if ((fabs(vec2.x) <= tol || vec2.x <= 0) && vec2.y >= 0) {

				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
			else if ((fabs(vec2.x) <= tol || vec2.x <= 0) && vec2.y < 0) {
				acutPrintf(L"KKK");
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x > 0 && vec2.y > 0) {
				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x >= 0 && vec2.y <= 0) {


				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}

		}
		else if (fabs(vec1.y) <= tol&&vec1.x <= 0) {

			if ((fabs(vec2.x) <= tol || vec2.x <= 0) && vec2.y >= 0) {

				bulge = tan(((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
			else if ((fabs(vec2.x) <= tol || vec2.x <= 0) && vec2.y < 0) {

				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}
			else if (vec2.x >= 0 && vec2.y >= 0) {

				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
			else if (vec2.x >= 0 && vec2.y < 0) {


				bulge = tan((-(CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
		}

		//2
		else if (vec1.x <= 0 && vec1.y >= 0) {
			//1
			if ((vec2.x > 0 && vec2.y > 0))
			{
				//acutPrintf(TEXT("2_1"));
				//+
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}//4
			 //4
			else if (vec2.x > 0 && vec2.y < 0) {

				AcGeVector2d vec1Fan = -1 * vec1;

				if (vec1Fan.angle() > vec2.angle()) {
					bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}
				else {
					bulge = tan(((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}
			}
			else if (fabs(vec2.x) <= tol && vec2.y >= 0) {
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}
			else if (fabs(vec2.y) <= tol&& vec2.x >= 0) {
				acutPrintf(L"2-1-0");
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}
			else {
				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}
		}
		//3
		else if (vec1.x <= 0 && vec1.y <= 0) {
			//2
			if (vec2.x <= 0 && vec2.y >= 0) {
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}//1-
			else if ((vec2.x > 0 && vec2.y > 0)) {
				AcGeVector2d vec1Fan = -1 * vec1;

				if (vec1Fan.angle() > vec2.angle()) {
					bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
				}
				else {
					bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
				}


			}
			else if (vec2.x > 0 && vec2.y < 0) {
				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}
			else if (vec2.x >= 0 && fabs(vec2.y) < tol) {
				acutPrintf(L"3-1-0");
				bulge = tan(-(CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}
		}
		//4
		else if (vec1.x >= 0 && vec1.y <= 0)
		{

			if (fabs(vec2.x) < tol&&vec2.y >= 0) {

				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
			else if (vec2.x < 0 && vec2.y < 0) {

				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}//2
			else if (vec2.x < 0 && (vec2.y > 0 || fabs(vec2.y) < tol)) {
				acutPrintf(L"MMMM");
				AcGeVector2d vec1Fan = -1 * vec1;

				if (vec1Fan.angle() > vec2.angle()) {
					bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}
				else {
					bulge = tan(((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}

			}
			else if ((vec2.x > 0 && vec2.y > 0)) {
				//acutPrintf(TEXT("4_4"));
				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
			}
			else if (vec2.x >= 0 && (fabs(vec2.y) <= tol || vec2.y <= 0)) {

				//acutPrintf(TEXT("4_1"));
				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
			}
		}
		//1
		else if (vec1.x >= 0 && vec1.y >= 0) {

			if (vec2.x >= 0 && vec2.y >= 0)
			{

				acutPrintf(TEXT("1_1"));
				if (vec2.angle() > vec1.angle()) {

					bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

				}
				else {
					bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
				}
			}//4
			else  if (vec2.x > 0 && vec2.y < 0)
			{
				AcGeVector2d vec1Fan = -1 * vec1;
				acutPrintf(TEXT("1_4"));
				/*if (vec1Fan.angle() > vec2.angle()) {
				bulge = tan(((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}
				else {
				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}*/

				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);
			}//3-1
			else if (vec2.x <= 0 && vec2.y <= 0) {
				AcGeVector2d vec1Fan = -1 * vec1;

				if (vec1Fan.angle() > vec2.angle()) {
					bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}
				else {
					bulge = tan(((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);
				}

			}//2
			else if (vec2.x < 0 && vec2.y>0) {
				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
		}//同一象限
		else if (vec1.x*vec2.x > 0 && vec1.y*vec2.y > 0) {

			if (vec2.angle() > vec1.angle()) {

				bulge = tan(-((CMathUtil::PI() - vec2.angleTo(vec1))) / 4);

			}
			else {
				bulge = tan((CMathUtil::PI() - vec2.angleTo(vec1)) / 4);

			}

		}

		return bulge;
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMultiDaoJiaoApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CMultiDaoJiaoApp, ECDMyGroup, EcdDaojiao, EcdDaojiao, ACRX_CMD_MODAL, NULL)

