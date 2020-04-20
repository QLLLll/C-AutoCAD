

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include<vector>
#define PI 3.1415926
#define ReadType 1
using namespace std;

int queKou = 201;
int queKou2 = 301;
int quekou3 = 351;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CQiangToLiangApp : public AcRxArxApp {

public:
	struct mySt
	{
		AcDbLine *line;
		bool isStart;
		AcGePoint3d pt;
	};

	CQiangToLiangApp() : AcRxArxApp() {}

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

	static void ECDQiangToLiangEcdWW(void) {


		ErrorStatus es = ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

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
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				double len = 0;
				l->getDistAtPoint(l->endPoint(), len);

				if (len <queKou||len<queKou2||len<quekou3) {
					l->erase();
					l->close();
					l = NULL;
				}
				else
				{
					vecLines.push_back(l);
				}
			}
		}
		acutPrintf(L"before=%d", vecLines.size());

		AcGePoint3dArray temp1;
		AcGePoint3dArray temp2;
		temp1.append(AcGePoint3d::kOrigin);
		temp2.append(AcGePoint3d::kOrigin);
		temp1.removeAll();
		temp2.removeAll();
		//联通
		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];
			

			AcDbLine * l = NULL;
		
			
			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{
				AcDbLine *l2 = vecLines[j];

				l1->intersectWith(l2, AcDb::kOnBothOperands, temp1, 0, 0);

				if (1 == temp1.length()) {

					/*ptJdAll.append(temp1[0]);
					CutLine(l1, temp1[0]);
					CutLine(l2, temp1[0]);*/


					temp1.removeAll();

				}
				else {
					l1->intersectWith(l2, AcDb::kExtendBoth, temp2, 0, 0);
					//ptNJdAll.append(temp2[0]);
					if (temp2.length() > 0) {
						ExtendLine(l1, temp2[0]);
						ExtendLine(l2, temp2[0]);
						temp2.removeAll();
					}
				}
			}
		}
//削减
#if 1


		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];


			AcDbLine * l = NULL;

			
			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{
				AcDbLine *l2 = vecLines[j];
				l1->intersectWith(l2, AcDb::kExtendBoth, temp2, 0, 0);
				l1->intersectWith(l2, AcDb::kOnBothOperands, temp1, 0, 0);
				
				if (1 == temp1.length()) {
					CutLine(l1, temp1[0]);
					CutLine(l2, temp1[0]);
					
					temp1.removeAll();
				}
				else if (0) {
					AcGePoint3d pt11 = l1->startPoint();
					AcGePoint3d pt12 = l1->endPoint();
					double disO = pt11.distanceTo(pt12);

					double dis1 = pt11.distanceTo(temp2[0]);
					double dis2 = pt12.distanceTo(temp2[0]);

					double max1 = (dis2 > dis1 ? dis2 : dis1);

					AcGePoint3d pt21 = l2->startPoint();
					AcGePoint3d pt22 = l2->endPoint();
					double disO2 = pt21.distanceTo(pt22);

					double dis21 = pt21.distanceTo(temp2[0]);
					double dis22 = pt22.distanceTo(temp2[0]);

					double max21 = (dis22 > dis21 ? dis22 : dis21);
					if (max21 > disO2&&max1 > disO) {

					}
					else {
						CutLine(l1, temp2[0]);
						CutLine(l2, temp2[0]);
						
					}
					temp2.removeAll();
				}
				

				
				
			}
		}
#endif
		for (int i = 0; i < (int)vecLines.size(); i++)
		{


			vecLines[i]->setColorIndex(1);

			vecLines[i]->close();

		}
		//补充
#if 0
		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];


			AcDbLine * l = NULL;

			
			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{
				AcDbLine *l2 = vecLines[j];

				l1->intersectWith(l2, AcDb::kOnBothOperands, temp1, 0, 0);



				if (1 == temp1.length()) {

					temp1.removeAll();
					continue;

				}
				else {
					l1->intersectWith(l2, AcDb::kExtendBoth, temp2, 0, 0);
					//ptNJdAll.append(temp2[0]);
					if (temp2.length() > 0) {
						AcGePoint3d pt11 = l1->startPoint();
						AcGePoint3d pt12 = l1->endPoint();
						double disO = pt11.distanceTo(pt12);

						double dis1 = pt11.distanceTo(temp2[0]);
						double dis2 = pt12.distanceTo(temp2[0]);

						double max1 = (dis2 > dis1 ? dis2 : dis1);

						AcGePoint3d pt21 = l2->startPoint();
						AcGePoint3d pt22 = l2->endPoint();
						double disO2 = pt21.distanceTo(pt22);

						double dis21 = pt21.distanceTo(temp2[0]);
						double dis22 = pt22.distanceTo(temp2[0]);

						double max21 = (dis22 > dis21 ? dis22 : dis21);
						if (max21 > disO2&&max1 > disO) {

							/*ExtendLine(l1, temp2[0]);
							ExtendLine(l2, temp2[0]);*/
							if (max1 < disO + 0.5*(queKou+queKou2+quekou3))
							{
								if (max1 == dis2) {

									l1->setStartPoint(temp2[0]);
								}
								else {
									l1->setEndPoint(temp2[0]);
								}
							}
							if (max21 < disO2 + 0.5*(queKou + queKou2 + quekou3)) {
								if (max21 == dis22) {

									l2->setStartPoint(temp2[0]);
								}
								else {
									l2->setEndPoint(temp2[0]);
								}
							}
						}
						temp2.removeAll();
					}
				}
			}
		}
#endif
		

	}
	static void ECDQiangToLiangEcdEE(void) {
		ErrorStatus es = ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

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
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				double len = 0;
				l->getDistAtPoint(l->endPoint(), len);

				if (len <queKou || len<queKou2 || len<quekou3) {
					l->erase();
					l->close();
					l = NULL;
				}
				else
				{
					vecLines.push_back(l);
				}
			}
		}
		acutPrintf(L"before=%d", vecLines.size());	
		AcGePoint3dArray temp1;
		AcGePoint3dArray temp2;
		temp1.append(AcGePoint3d::kOrigin);
		temp2.append(AcGePoint3d::kOrigin);
		temp1.removeAll();
		temp2.removeAll();

		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];


			AcDbLine * l = NULL;


			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{
				AcDbLine *l2 = vecLines[j];

				l1->intersectWith(l2, AcDb::kOnBothOperands, temp1, 0, 0);



				if (1 == temp1.length()) {

					temp1.removeAll();
					continue;

				}
				else {
					l1->intersectWith(l2, AcDb::kExtendBoth, temp2, 0, 0);
					//ptNJdAll.append(temp2[0]);
					if (temp2.length() > 0) {
						AcGePoint3d pt11 = l1->startPoint();
						AcGePoint3d pt12 = l1->endPoint();
						double disO = pt11.distanceTo(pt12);

						double dis1 = pt11.distanceTo(temp2[0]);
						double dis2 = pt12.distanceTo(temp2[0]);

						double max1 = (dis2 > dis1 ? dis2 : dis1);

						AcGePoint3d pt21 = l2->startPoint();
						AcGePoint3d pt22 = l2->endPoint();
						double disO2 = pt21.distanceTo(pt22);

						double dis21 = pt21.distanceTo(temp2[0]);
						double dis22 = pt22.distanceTo(temp2[0]);

						double max21 = (dis22 > dis21 ? dis22 : dis21);
						if (max21 >= disO2&&max1 >= disO) {

							/*ExtendLine(l1, temp2[0]);
							ExtendLine(l2, temp2[0]);*/
							if (max1 < disO + 0.5*(queKou + queKou2 + quekou3))
							{
								if (max1 == dis2) {

									l1->setStartPoint(temp2[0]);
								}
								else {
									l1->setEndPoint(temp2[0]);
								}
							}
							if (max21 < disO2 + 0.5*(queKou + queKou2 + quekou3)) {
								if (max21 == dis22) {

									l2->setStartPoint(temp2[0]);
								}
								else {
									l2->setEndPoint(temp2[0]);
								}
							}
						}
						temp2.removeAll();
					}
				}
			}
		}
		for (int i = 0; i < (int)vecLines.size(); i++)
		{


			vecLines[i]->setColorIndex(1);

			vecLines[i]->close();

		}
	}
	static void ECDQiangToLiangEcdEE2(void) {
		ErrorStatus es = ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

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
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				double len = 0;
				l->getDistAtPoint(l->endPoint(), len);

				if (len <queKou || len<queKou2 || len<quekou3) {
					l->erase();
					l->close();
					l = NULL;
				}
				else
				{
					vecLines.push_back(l);
				}
			}
		}
		acutPrintf(L"before=%d", vecLines.size());
		AcGePoint3dArray temp1;
		AcGePoint3dArray temp2;
		temp1.append(AcGePoint3d::kOrigin);
		temp2.append(AcGePoint3d::kOrigin);
		temp1.removeAll();
		temp2.removeAll();

		AcGePoint3dArray ptArrAll;
		ptArrAll.append(AcGePoint3d::kOrigin);
		ptArrAll.removeAll();

		for (int i = 0; i < (int)vecLines.size(); i++)
		{
			AcDbLine * l1 = vecLines[i];

			ptArrAll.append(l1->startPoint());
			ptArrAll.append(l1->endPoint());


		}

		//acutPrintf(L"prePt=%d", ptArrAll.length());
		double r = (queKou + queKou2 + quekou3) / 3*2;
		for (int i=0;i<ptArrAll.length();i++)
		{
			AcGePoint3d ptCenter=ptArrAll[i];
			
			AcDbCircle *cir = new AcDbCircle(ptCenter, AcGeVector3d::kZAxis, r);

			for (int j = i+1; j < ptArrAll.length(); j++) {
			
				AcGePoint3d pt2 = ptArrAll[j];

				double dis = pt2.distanceTo(ptCenter);

				if (dis <= r) {

					ptArrAll.removeAt(j);

				}
			
			}
			delete cir;
			cir = NULL;
		}
		//acutPrintf(L"afterPt=%d", ptArrAll.length());

		for (int i = 0; i < ptArrAll.length(); i++)
		{
			AcGePoint3d ptCenter = ptArrAll[i];

			AcDbCircle *cir = new AcDbCircle(ptCenter, AcGeVector3d::kZAxis, r);
			vector<AcDbLine*>vecLL;
			for (int ww = 0; ww < (int)vecLines.size(); ww++)
			{

				AcDbLine * l1 = vecLines[ww];

				l1->intersectWith(cir, AcDb::kOnBothOperands, temp1, 0, 0);

				if (temp1.length() > 0) {

					vecLL.push_back(l1);
					temp1.removeAll();

				}

			}
			
			delete cir;
			cir = NULL;

			AcGeIntArray intArr;

			for (int m = 0; m < (int)vecLL.size(); m++)
			{
				AcDbLine * lT1 = vecLL[m];

				for (int s = 0; s < (int)vecLL.size(); s++)
				{
					AcDbLine * lT2 = vecLL[s];

					lT1->intersectWith(lT2, AcDb::kOnBothOperands, temp2, 0, 0);

					if (temp2.length() > 0) {

						intArr.append(m);
						intArr.append(s);

						temp2.removeAll();


					}
					
				}

			}
			
			for (int m = 0; m < (int)vecLL.size(); m++)
			{
				if (intArr.contains(m))
				{
					continue;
				}

				AcDbLine * lT1 = vecLL[m];

				for (int s = 0; s < (int)vecLL.size() ; s++)
				{
					if (intArr.contains(s)) {
					
					continue;
					}

					AcDbLine * lT2 = vecLL[s];

					
						lT1->intersectWith(lT2, AcDb::kExtendBoth, temp2, 0, 0);
						//ptNJdAll.append(temp2[0]);
						if (temp2.length() > 0) {
							AcGePoint3d pt11 = lT1->startPoint();
							AcGePoint3d pt12 = lT1->endPoint();
							double disO = pt11.distanceTo(pt12);

							double dis1 = pt11.distanceTo(temp2[0]);
							double dis2 = pt12.distanceTo(temp2[0]);

							double max1 = (dis2 > dis1 ? dis2 : dis1);

							AcGePoint3d pt21 = lT2->startPoint();
							AcGePoint3d pt22 = lT2->endPoint();
							double disO2 = pt21.distanceTo(pt22);

							double dis21 = pt21.distanceTo(temp2[0]);
							double dis22 = pt22.distanceTo(temp2[0]);

							double max21 = (dis22 > dis21 ? dis22 : dis21);

								if (max1 < disO + 0.5*(queKou + queKou2 + quekou3))
								{
									if (max1 == dis2) {

										lT1->setStartPoint(temp2[0]);
									}
									else {
										lT1->setEndPoint(temp2[0]);
									}
								}
								if (max21 < disO2 + 0.5*(queKou + queKou2 + quekou3)) {
									if (max21 == dis22) {

										lT2->setStartPoint(temp2[0]);
									}
									else {
										lT2->setEndPoint(temp2[0]);
									}
								}
						
								temp2.removeAll();
						}
						

					
				}

			}
			intArr.removeAll();

			vecLL.clear();
		}
		for (int i = 0; i < (int)vecLines.size(); i++)
		{


			vecLines[i]->setColorIndex(1);

			vecLines[i]->close();

		}
	}
	static void ECDQiangToLiangEcdQ2L(void)
	{

		ErrorStatus es = ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

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
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				double len = 0;
				l->getDistAtPoint(l->endPoint(), len);

				if (len <= 900) {
					l->erase();
					l->close();
					l = NULL;
				}
				else
				{
					vecLines.push_back(l);
				}
			}
		}
		acutPrintf(L"before=%d", vecLines.size());
		AcGeIntArray  delIndex;
		AcGeIntArray changeIndex;

		vector<AcDbLine*>vvL;
		vector<AcGePoint3d>vecPt;
		//十字路口連綫
		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];
			AcGePoint3d pt11 = l1->startPoint();
			AcGePoint3d pt12 = l1->endPoint();

			AcDbLine * l = NULL;
			vecPt.push_back(pt11);
			vecPt.push_back(pt12);
			//记录被删除的元素个数
			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{

				AcDbLine *l2 = vecLines[j];

				/*AcGePoint3dArray temp;

				l1->intersectWith(l2, AcDb::kOnBothOperands, temp, 0, 0);

				if (temp.length() >= 1) {
					continue;
				}*/



				AcGePoint3d pt21 = l2->startPoint();
				AcGePoint3d pt22 = l2->endPoint();


				vecPt.push_back(pt21);
				vecPt.push_back(pt22);

				//寻找最短
				int indexF = 0, indexE = 1;
				double minDis = vecPt[0].distanceTo(vecPt[1]);


				for (int k = 0; k < (int)vecPt.size(); k++)
				{

					for (int m = k + 1; m < (int)vecPt.size(); m++)
					{

						double d = vecPt[k].distanceTo(vecPt[m]);
						if (d < minDis&&d>10) {
							indexF = k;
							indexE = m;
							minDis = d;
						}
					}
				}



				AcGeVector3d vec3 = vecPt[indexF] - vecPt[indexE];

				AcGeVector3d vec4 = l1->startPoint() - l1->endPoint();

				if (vec3 == vec4) {
					vec4 = l2->startPoint() - l2->endPoint();
				}

				double angle2 = vec3.angleTo(vec4);
				double aa2 = angle2 / PI * 180;

				//在同一直綫上

				if (fabs(aa2) < 1 || fabs(aa2 - 180) < 1) {

					AcGePoint3d temp1, temp2;

					if (fabs(vecPt[0].x - vecPt[1].x) < 15) {

						if (vecPt[0].y > vecPt[1].y) {
							temp1 = vecPt[0];
							vecPt[0] = vecPt[1];
							vecPt[1] = temp1;
						}

					}
					else {
						if (vecPt[0].x > vecPt[1].x)
						{
							temp1 = vecPt[0];
							vecPt[0] = vecPt[1];
							vecPt[1] = temp1;
						}
					}

					if (fabs(vecPt[2].x - vecPt[3].x) < 15) {

						if (vecPt[2].y > vecPt[3].y) {
							temp2 = vecPt[2];
							vecPt[2] = vecPt[3];
							vecPt[3] = temp2;
						}

					}
					else {
						if (vecPt[2].x > vecPt[3].x)
						{
							temp2 = vecPt[2];
							vecPt[2] = vecPt[3];
							vecPt[3] = temp2;
						}
					}

					AcGeVector3d vecT1 = vecPt[0] - vecPt[1];
					AcGeVector3d vecT2 = vecPt[1] - vecPt[2];
					if (vecPt[1] == vecPt[2])
						vecT2 = vecPt[1] - vecPt[3];


					double angle3 = vecT1.angleTo(vecT2);
					double aa3 = angle3 / PI * 180;

					if ((vecPt[0] != vecPt[2] && vecPt[1] != vecPt[3]) && (fabs(aa3) < 5 || fabs(aa3 - 180) < 5))
						//寻找最长
					{
						int inF = 0, inE = 1;
						double maxDis = vecPt[0].distanceTo(vecPt[1]);

						for (int k = 0; k < (int)vecPt.size(); k++)
						{

							for (int m = k + 1; m < (int)vecPt.size(); m++)
							{

								double d = vecPt[k].distanceTo(vecPt[m]);
								if (d > maxDis) {
									inF = k;
									inE = m;
									maxDis = d;
								}
							}
						}
						if (l == NULL) {

							l = new AcDbLine();
						}

						l->setStartPoint(vecPt[inE]);
						l->setEndPoint(vecPt[inF]);

						AcGePoint3d tt1 = vecPt[inE];
						AcGePoint3d tt2 = vecPt[inF];

						delIndex.append(j);

						vecPt.clear();

						vecPt.push_back(tt1);
						vecPt.push_back(tt2);
					}
				}

				//vecPt.clear();
			}
			if (l != NULL) {
				changeIndex.append(ww);
				vvL.push_back(l);

			}
			vecPt.clear();
		}

		acutPrintf(L"\ndelIndex=%d\n", delIndex.length());
		acutPrintf(L"vvL=%d\n", vvL.size());
		acutPrintf(L"after=%d", vecLines.size());

		int cI = 0;
		for (int i = 0; i < (int)vecLines.size(); i++)
		{

			if (changeIndex.contains(i) && cI < vvL.size()) {
				vecLines[i]->setColorIndex(3);
				vecLines[i]->setStartPoint(vvL[cI]->startPoint());
				vecLines[i]->setEndPoint(vvL[cI]->endPoint());
				delete vvL[cI];
				vvL[cI] = NULL;

				vecLines[i]->close();
				++cI;
			}
			else if (delIndex.contains(i)) {
				vecLines[i]->setColorIndex(2);
				//vecLines[i]->erase();
				vecLines[i]->close();
				vecLines[i] = NULL;
			}
			else {
				vecLines[i]->setColorIndex(1);

				vecLines[i]->close();
			}
		}
#if 0


		vector<AcDbLine*>vecL2;
		for (size_t i = 0; i < vecLines.size(); i++)
		{

			AcDbLine * l = NULL;

			if (vecLines[i] != NULL) {
				if (acdbOpenObject(l, vecLines[i]->objectId(), AcDb::kForWrite) == Acad::eOk) {

					vecL2.push_back(l);
					l->setColorIndex(1);
					l->close();
				}
			}
		}
#endif

#if 0


		AcGePoint3dArray ptArr;
		vector<mySt>vecSt;

		acutPrintf(L"\nvecL2=%d", vecL2.size());

		for (int i = 0; i < (int)vecL2.size(); i++)
		{
			AcDbLine * l1 = vecL2[i];
			AcGePoint3d l1S = l1->startPoint();
			AcGePoint3d l1E = l1->endPoint();
			AcGeVector3d vecL1 = l1S - l1E;
			for (int j = i + 1; j < (int)vecL2.size(); j++)
			{
				AcDbLine *l2 = vecL2[j];

				AcGePoint3dArray temp;

				l1->intersectWith(l2, AcDb::kOnBothOperands, temp, 0, 0);

				if (temp.length() >= 1) {
					continue;
				}

				AcGePoint3d l2S = l2->startPoint();
				AcGePoint3d l2E = l2->endPoint();
				AcGeVector3d vecL2 = l2S - l2E;

				double angle2 = vecL1.angleTo(vecL2);
				double aa2 = angle2 / PI * 180;

				l1->intersectWith(l2, AcDb::kOnBothOperands, ptArr, 0, 0);

				int n = ptArr.length();
				//平行且没得交点
				if ((fabs(aa2) < 1 || fabs(aa2 - 180) < 1) && n == 0)
				{
					continue;
				}

				l1->intersectWith(l2, AcDb::kExtendBoth, ptArr, 0, 0);

				if (ptArr.length() >= 1) {

					for (int n = 0; n < ptArr.length(); n++)
					{
						double dis1 = l1S.distanceTo(l1E);
						double dis2 = l1S.distanceTo(ptArr[n]);
						double dis3 = l1E.distanceTo(ptArr[n]);
						double min1 = (dis2 > dis3 ? dis3 : dis2);
						double max1 = (dis2 > dis3 ? dis2 : dis3);

						double dis21 = l2S.distanceTo(l2E);
						double dis22 = l2S.distanceTo(ptArr[n]);
						double dis23 = l2E.distanceTo(ptArr[n]);
						double min2 = (dis22 > dis23 ? dis23 : dis22);
						double max2 = (dis22 > dis23 ? dis22 : dis23);
						if (!((min1 > queKou + queKou2 + quekou3) && (dis1 < max1) || (min2 >queKou + queKou2 + quekou3) && (dis21 < max2))) {
							InLine(l1, ptArr[n], vecSt);
							InLine(l2, ptArr[n], vecSt);
						}
						else {
							acutPrintf(L"\nInLine=false\n");
						}
					}

				}
				ptArr.removeAll();
			}
		}

		for (int i = 0; i < (int)vecL2.size(); i++)
		{
			for (int j = 0; j < (int)vecSt.size(); j++)
			{
				if (vecSt[j].line == vecL2[i]) {

					if (vecSt[j].isStart) {
						vecL2[i]->setStartPoint(vecSt[j].pt);
					}
					else {
						vecL2[i]->setEndPoint(vecSt[j].pt);
					}
					break;
				}

			}

			vecL2[i]->close();
		}

#endif			
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
	static bool comp(AcGePoint3d pt1, AcGePoint3d pt2) {
		double ax = pt1.x - pt2.x;
		double ay = pt1.y - pt2.y;
		if (fabs(ax) < fabs(ay)) {
			return pt1.y < pt2.y;
		}
		else {
			return pt1.x < pt2.x;
		}
	}

private:
	static void CutLine(AcDbLine * l1, const  AcGePoint3d & pt) {
		AcGePoint3d pt11 = l1->startPoint();
		AcGePoint3d pt12 = l1->endPoint();
		double disO = pt11.distanceTo(pt12);

		double dis1 = pt11.distanceTo(pt);
		double dis2 = pt12.distanceTo(pt);

		if (disO > dis1&&disO > dis2) {

			if (dis1 <= (queKou + queKou2 + quekou3)/3&&dis2 <= (queKou + queKou2 + quekou3) / 3) {

				AfxMessageBox(L"某个地方处理不了");

			}
			else {

				if (dis1 < dis2) {

					l1->setStartPoint(pt);

				}
				else {
					l1->setEndPoint(pt);
				}

			}

		}

	}
	static void ExtendLine(AcDbLine * l1, const  AcGePoint3d & pt) {
		AcGePoint3d pt11 = l1->startPoint();
		AcGePoint3d pt12 = l1->endPoint();
		double disO = pt11.distanceTo(pt12);

		double dis1 = pt11.distanceTo(pt);
		double dis2 = pt12.distanceTo(pt);

		double max = (dis2 > dis1 ? dis2 : dis1);

		if (max > disO + 1.5*(queKou + queKou2 + quekou3) / 3 || max < disO) {

			return;
		}
		else {

			if (max==dis2) {

				l1->setStartPoint(pt);

			}
			else {
				l1->setEndPoint(pt);
			}

		}

	

}
static void InLine(AcDbLine * l1, const  AcGePoint3d & pt, vector<mySt>&vecSt)
{
	static int colorI = 1;

	AcGePoint3d pt1 = l1->startPoint();
	AcGePoint3d pt2 = l1->endPoint();

	if (pt1 == pt || pt2 == pt) {
		return;
	}

	double dis1 = pt1.distanceTo(pt2);
	double dis2 = pt1.distanceTo(pt);
	double dis3 = pt2.distanceTo(pt);

	AcGeVector3d vec1 = pt1 - pt2;
	AcGeVector3d vec2 = pt1 - pt;
	AcGeVector3d vec3 = pt2 - pt;

	double minLen = (dis2 > dis3 ? dis3 : dis2);

	//缺口大小
	if (minLen > (queKou + queKou2 + quekou3) / 3) {
		return;
	}

	double angle2 = vec1.angleTo(vec2);
	double aa2 = angle2 / PI * 180;

	double angle3 = vec1.angleTo(vec3);
	double aa3 = angle3 / PI * 180;
	double max1 = (dis2 > dis3 ? dis2 : dis3);

	if ((fabs(aa3) < 1 || fabs(aa3 - 180) < 1) && (fabs(aa2) < 1 || fabs(aa2 - 180) < 1))
	{
		if (dis2 > dis1) {

			if (max1 > 10 * dis1) {

				return;

			}

			mySt st;
			st.line = l1;
			st.isStart = false;
			st.pt = pt;

			vecSt.push_back(st);

		}
		else if (dis3 > dis1) {

			if (max1 > 10 * dis1) {
				return;
			}
			mySt st;
			st.line = l1;
			st.isStart = true;
			st.pt = pt;
			vecSt.push_back(st);

		}
	}

}
static bool GetReal(const TCHAR * prompt, double defaultVal, int precision, double & ret)
{
	CString strPrompt = prompt;
	strPrompt.TrimRight();
	strPrompt.TrimRight(TEXT(":"));
	CString strDefaultValue;
	strDefaultValue.Format(TEXT("<%%.%df>:"), precision);

	strPrompt.Format(strPrompt + strDefaultValue, defaultVal);
	ret = defaultVal;
	int rc = acedGetReal(strPrompt, &ret);

	if (rc == RTNORM || rc == RTNONE) {

		return true;
	}
	else {
		return false;
	}

}



};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CQiangToLiangApp)

//ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDQiangToLiang, EcdQ2L, EcdQ2L, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDQiangToLiang, EcdWW, EcdWW, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDQiangToLiang, EcdEE2, EcdEE2, ACRX_CMD_TRANSPARENT, NULL)