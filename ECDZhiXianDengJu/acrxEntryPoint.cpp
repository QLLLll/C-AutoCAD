//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"DwgDataBaseUtil.h"
#include<algorithm>
#include<vector>
using namespace std;


//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CECDZhiXianDengJuApp : public AcRxArxApp {

public:
	CECDZhiXianDengJuApp() : AcRxArxApp() {}

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

	static void ECDMyGroupEcdZxDj() {

		//resbuf *rb;

		//rb = acutBuildList(, adsPt, RTNONE);
		AcDbObjectIdArray entIds;
		vector<AcDbLine*>vecLines;
		vector<AcDbPolyline*>vecPls;
		ErrorStatus es;

		while (SelectEnts(TEXT("选择平行线：\n"), NULL, entIds)) {

			ads_point adPt;

			if (acedGetPoint(NULL, L"请在要等距的线的两端选择一个点：\n", adPt)!=RTNORM) {

				return;

		}

			AcGePoint3d disPt = asPnt3d(adPt);


			for (int i = 0; i < entIds.length(); i++)
			{
				AcDbEntity * ent = NULL;

				es = acdbOpenAcDbEntity(ent, entIds[i], AcDb::kForWrite);

				if (es == ErrorStatus::eOk) {

					if (ent->isKindOf(AcDbLine::desc())) {

						AcDbLine* tempLine = AcDbLine::cast(ent);

						//ent->close();

						vecLines.push_back(tempLine);

					}
					else if (ent->isKindOf(AcDbPolyline::desc())) {

						AcDbPolyline* tempPl = AcDbPolyline::cast(ent);

						//ent->close();
						vecPls.push_back(tempPl);
					}
					else {
						acutPrintf(L"不是直线或者多段线\n");
					}

				}
				else {
					acutPrintf(L"err=%d\n", es);
					return;
				}

				acutPrintf(L"数量=%d\n", entIds.length());



			}

			int lSize = (int)vecLines.size();

			int plSize = (int)vecPls.size();
			//vector<AcDbLine*>vecLineSorted;
			//CArray<AcDbLine*>vecLineSorted;

			int vecIndex[100] = { 0 };

			if (lSize > 2) {
				sort(vecLines.begin(), vecLines.end(), compAcDbLine);

				//排序
				int flag = lSize;
				vector<int> ff;
				int j = 0;
				int q = 0;
				while (flag >0) {

					AcGePoint3d tPt2;

					for (int m=0;m<lSize;m++)
					{
						if (!find(ff, m)) {

							j = m;
							break;

						}
					}


					vecLines[j]->getClosestPointTo(disPt, tPt2, Adesk::kTrue);
					
					int index = j;
					
					double minDis = tPt2.distanceTo(disPt);					

					for (int i=0;i<lSize/*&&!find(ff,i)*/;i++)
					{
						AcGePoint3d tempPt;

						if (find(ff, i)) {
							continue;
						}

						vecLines[i]->getClosestPointTo(disPt, tempPt, Adesk::kTrue);

						double dis= tempPt.distanceTo(disPt);
						CString ss;
						ss.Format(L"mindis=%f,dis=%f",minDis, dis);
						//AfxMessageBox(ss);

						if (dis < minDis) {

							minDis = dis;

							index = i;
						}
					}
					CString str;

					str.Format(L"the minDis=%f", minDis);
					//AfxMessageBox(str);
					if (!find(ff,index))
					{
						//vecLineSorted.push_back(vecLines[index]);
						//vecLineSorted.InsertAt(q,vecLines[index]);
						vecIndex[q] = index;
						++q;
						ff.push_back(index);
					}
						--flag;
					
				}

				CString str;

				str.Format(L"%d", q);
				//AfxMessageBox(str);

				/*AcDbLine * lineStart = vecLineSorted[0];

				AcDbLine* lineEnd = vecLineSorted[vecLineSorted.size() - 1];

				AcGePoint3d pt = lineStart->startPoint();
				AcGePoint3d pt2;

				lineStart->setColorIndex(1);

				lineEnd->setColorIndex(2);

				lineEnd->getClosestPointTo(pt, pt2, true);

				AcDbLine *InterLine = new AcDbLine(pt, pt2);

				AcGePoint3dArray ptArr,tempPtArr;
				
				

				double distanse = 0.0;

				AcGeVector3d vecMove = (pt2 - pt).normal();

				distanse = pt.distanceTo(pt2);

				double eachDis = distanse / lSize;
				lineStart->close();

				for (int i=1;i<vecLineSorted.size() -1;i++)
				{
					InterLine->intersectWith(vecLineSorted[i], AcDb::kExtendArg, tempPtArr);

					ptArr.append(tempPtArr[0]);

					tempPtArr.removeAll();

				}
				delete InterLine;
				InterLine = NULL;

				ptArr.append(pt2);

				for (int i=0;i<ptArr.length();i++)
				{
					double tempDis = ptArr[i].distanceTo(pt);

					AcDbLine * linew = new AcDbLine(ptArr[i], pt);
					CDwgDataBaseUtil::PostToModelSpace(linew);

					linew->close();

					double moveDis = (i + 1)*eachDis - tempDis;

					AcDbLine * tempLine = vecLineSorted[i + 1];

					AcGeMatrix3d mtx;
					mtx.setTranslation(vecMove*moveDis);
					tempLine->transformBy(mtx);

					tempLine->close();

				}
				*/

				//AcDbLine * lineStart = vecLineSorted[0];
				AcDbLine * lineStart = vecLines[vecIndex[0]];

				//AcDbLine* lineEnd = vecLineSorted[vecLineSorted.GetSize() - 1];
				AcDbLine* lineEnd = vecLines[vecIndex[q-1]];
				lineStart->setColorIndex(1);

				lineEnd->setColorIndex(2);

				double distanse = 0.0;

				AcGePoint3d pt = lineStart->startPoint();
				AcGePoint3d pt2;
				lineEnd->getClosestPointTo(pt, pt2, true);

				AcGeVector3d vecMove = (pt2 - pt).normal();

				distanse = pt.distanceTo(pt2);

				acutPrintf(L"dis=%f\n", distanse);

				double eachDis = distanse / (int)vecLines.size();

				lineStart->close();

				for (int i = 1; i < (int)vecLines.size(); i++)
				{
					AcDbLine * tempLine = vecLines[vecIndex[i]];

					AcGePoint3d tempPt;

					tempLine->getClosestPointTo(pt, tempPt, true);

					double 	tempDis = pt.distanceTo(tempPt);

					double moveDis = i*eachDis - tempDis;

					AcGeMatrix3d mtx;
					mtx.setTranslation(vecMove*moveDis);
					tempLine->transformBy(mtx);

					tempLine->close();

				}
			}
			else {
				for (int i = 0; i < (int)lSize; i++)
				{
					vecLines[i]->close();
				}
			}

			vecLines.clear();
			entIds.removeAll();

		/*	if (vecLines.size() > 2) {

				sort(vecLines.begin(), vecLines.end(), compAcDbLine);

				AcDbLine * lineStart = vecLines[0];

				AcDbLine* lineEnd = vecLines[vecLines.size() - 1];

				lineStart->setColorIndex(1);

				lineEnd->setColorIndex(2);

				double distanse = 0.0;

				AcGePoint3d pt = lineStart->startPoint();
				AcGePoint3d pt2;
				lineEnd->getClosestPointTo(pt, pt2, true);

				AcGeVector3d vecMove = (pt2 - pt).normal();

				distanse = pt.distanceTo(pt2);

				acutPrintf(L"dis=%f\n", distanse);

				double eachDis = distanse / (int)vecLines.size();

				lineStart->close();

				for (int i = 1; i < (int)vecLines.size(); i++)
				{
					AcDbLine * tempLine = vecLines[i];

					AcGePoint3d tempPt;

					tempLine->getClosestPointTo(pt, tempPt, true);

					double 	tempDis = pt.distanceTo(tempPt);

					double moveDis = i*eachDis - tempDis;

					AcGeMatrix3d mtx;
					mtx.setTranslation(vecMove*moveDis);
					tempLine->transformBy(mtx);

					tempLine->close();

				}

			}
			else {

				for (int i=0;i<(int)vecLines.size();i++)
				{
					vecLines[i]->close();
				}
			}*/
			 if (vecPls.size() > 2) {
				sort(vecPls.begin(), vecPls.end(), compAcDbPl);

				AcDbPolyline *pl1 = vecPls[0];
				AcDbPolyline *pl2 = vecPls[vecPls.size() - 1];

				pl1->setColorIndex(1);
				pl2->setColorIndex(2);

				AcGePoint2d pt0, pt01, pt1, pt11;
				AcGePoint3d ptd0, ptd01, ptd1, ptd11;
				pl1->getPointAt(0, pt0);
				pl1->getPointAt(1, pt01);


				pl2->getPointAt(0, pt1);
				pl2->getPointAt(1, pt11);

				AcGePoint3d  pt3d1;

				ptd0 = AcGePoint3d(pt0.x, pt0.y, 0);
				ptd01 = AcGePoint3d(pt01.x, pt01.y, 0);
				ptd1 = AcGePoint3d(pt1.x, pt1.y, 0);
				ptd11 = AcGePoint3d(pt11.x, pt11.y, 0);

				
				AcDbLine *l2dEnd =new  AcDbLine(ptd1, ptd11);

				l2dEnd->getClosestPointTo(ptd0, pt3d1, true);

				double dis = pt3d1.distanceTo(ptd0);
				acutPrintf(L"dis=%f\n", dis);
				double eachDis = dis / vecPls.size();

				AcGeVector3d vecMove = (pt3d1 - ptd0).normal();

				pl1->close();

				delete l2dEnd;
				l2dEnd = NULL;

				for (int i=1;i<(int)vecPls.size();i++)
				{

					AcDbPolyline *tempPl = vecPls[i];
					
					tempPl->getPointAt(0, pt0);
					tempPl->getPointAt(1, pt01);

					ptd1 = AcGePoint3d(pt0.x, pt0.y, 0);
					ptd11 = AcGePoint3d(pt01.x, pt01.y, 0);

					AcDbLine *l1 = new AcDbLine(ptd1, ptd11);
					
					AcGePoint3d  temp3d1;

					l1->getClosestPointTo(ptd0, temp3d1, true);

					double 	tempDis = ptd0.distanceTo(temp3d1);

					double moveDis = i*eachDis - tempDis;


					delete l1;
					l1 = NULL;


					AcGeMatrix3d mtx;
					mtx.setTranslation(vecMove*moveDis);
					tempPl->transformBy(mtx);

					tempPl->close();

				}

				//如果多段线反转的情况
				/*AcGeVector2d vec1, vec2;

				vec1 = pt0 - pt01;
				vec2 = pt1 - pt11;

				double angle = vec1.angleTo(vec2);

				

				if (fabs(angle / 3.1415926 * 180) <= 1e-2) {
				}*/

			}
			 else {

				 for (int i = 0; i < (int)vecPls.size(); i++)
				 {
					 vecPls[i]->close();
				 }
			 }
		}


	}

	static bool SelectEnts(ACHAR *prompt, resbuf *rb, AcDbObjectIdArray& entIds) {

		ads_name selName;

		acutPrintf(prompt);

		int rc = acedSSGet(NULL, NULL, NULL, rb, selName);
		acutPrintf(L"rc=%d\n", rc);
		if (rc == RTNORM) {

			int length;

			acedSSLength(selName, &length);

			for (int i = 0; i < length; i++)
			{
				ads_name name;

				acedSSName(selName, i, name);

				AcDbObjectId oId;

				if (acdbGetObjectId(oId, name) == ErrorStatus::eOk) {

					entIds.append(oId);
				}
			}
			acedSSFree(selName);
			return true;
		}
		else {
			acedSSFree(selName);
			return false;
		}

	}

	static bool compAcDbLine(AcDbLine* line1, AcDbLine* line2) {

		AcGePoint3d tempPt1;
		AcGePoint3d startPt1 = line1->startPoint();
		AcGePoint3d endPt1 = line1->endPoint();

		AcGePoint3d tempPt2;
		AcGePoint3d startPt2 = line2->startPoint();
		AcGePoint3d endPt2 = line2->endPoint();

		//左边和下边的点为起点，
		if ((startPt1.x > endPt1.x) || (startPt1.y > endPt1.y)) {

			tempPt1 = startPt1;
			startPt1 = endPt1;
			endPt1 = tempPt1;

		}
		else if (startPt1.x == endPt1.x) {

			if (startPt1.y > endPt1.y) {

				tempPt1 = startPt1;
				startPt1 = endPt1;
				endPt1 = tempPt1;

			}
		}
		else if (startPt1.y == endPt1.y) {

			if (startPt1.x > endPt1.x) {

				tempPt1 = startPt1;
				startPt1 = endPt1;
				endPt1 = tempPt1;

			}
		}

		if ((startPt2.x > endPt2.x) || (startPt2.y > endPt2.y)) {

			tempPt2 = startPt2;
			startPt2 = endPt2;
			endPt2 = tempPt2;

		}
		else if (startPt2.x == endPt2.x) {

			if (startPt2.y > endPt2.y) {

				tempPt2 = startPt2;
				startPt2 = endPt2;
				endPt2 = tempPt2;

			}
		}
		else if (startPt2.y == endPt2.y) {

			if (startPt2.x > endPt2.x) {

				tempPt2 = startPt2;
				startPt2 = endPt2;
				endPt2 = tempPt2;

			}
		}

		if (startPt1.x < startPt2.x) {

			return true;

		}
		else {
			return false;
		}

	}

	static bool compAcDbPl(AcDbPolyline* line1, AcDbPolyline* line2) {

		AcGePoint2d pt0, pt1;
		AcGePoint3d tempPt1;
		AcGePoint3d startPt1;
		AcGePoint3d endPt1;

		line1->getPointAt(0, pt0);
		line1->getPointAt(1, pt1);

		startPt1 = AcGePoint3d(pt0.x, pt0.y, 0);
		endPt1 = AcGePoint3d(pt1.x, pt1.y, 0);


		AcGePoint3d tempPt2;
		AcGePoint3d startPt2;
		AcGePoint3d endPt2;

		line2->getPointAt(0, pt0);
		line2->getPointAt(1, pt1);

		startPt2 = AcGePoint3d(pt0.x, pt0.y, 0);
		endPt2 = AcGePoint3d(pt1.x, pt1.y, 0);

		if (startPt1.x == startPt2.x) {

			return startPt1.y < startPt2.y;


		}
		if (startPt1.y == startPt2.y) {

			return startPt1.x < startPt2.x;
		}
		
		return((startPt1.y < startPt2.y) || (startPt1.x < startPt2.x));

	}
	static bool find(vector<int> &arr, int f) {

		for (int i=0;i<(int)arr.size();i++)
		{
			if (arr[i] == f) {
				return true;
			}

		}
		return false;


	}

	static bool find(CArray<int> &arr, int f) {

		for (int i = 0; i < (int)arr.GetSize(); i++)
		{
			if (arr.GetAt(i)== f) {
				return true;
			}

		}
		return false;


	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CECDZhiXianDengJuApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CECDZhiXianDengJuApp, ECDMyGroup, EcdZxDj, EcdZxDj, ACRX_CMD_MODAL, NULL)


