//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"dbJoinEntityPE.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdXiHeApp : public AcRxArxApp {

public:
	CEcdXiHeApp() : AcRxArxApp() {}

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


	static void ECDMyGroupEcdXiHe() {


		ads_name entName1, entName2;
		ads_point selPt1, selPt2;
		ErrorStatus es;
		while (acedEntSel(L"请选择polyline1：\n", entName1, selPt1) == RTNORM) {

			AcDbObjectId oId1, oId2;
			es = acdbGetObjectId(oId1, entName1);

			if (es != ErrorStatus::eOk) {

				acutPrintf(L"选择实体1错误。error=%d\n", es);
				return;
			}

			AcDbPolyline *pl1 = NULL, *pl2 = NULL;

			es = acdbOpenObject(pl1, oId1, AcDb::kForWrite);

			if (es != ErrorStatus::eOk) {

				acutPrintf(L"打开实体1错误，可能不是选择的polyline。error=%d\n", es);
				return;

			}

			if (acedEntSel(L"请选择polyline2：\n", entName2, selPt2) != RTNORM) {

				pl1->close();
				return;

			}
			es = acdbGetObjectId(oId2, entName2);

			if (es != ErrorStatus::eOk) {

				acutPrintf(L"选择实体2错误。error=%d\n", es);
				pl1->close();
				return;
			}
			es = acdbOpenObject(pl2, oId2, AcDb::kForWrite);

			if (es != ErrorStatus::eOk) {
				pl1->close();
				acutPrintf(L"打开实体2错误，可能不是选择的polyline。error=%d\n", es);
				return;

			}

			AcGePoint3dArray ptArr;

			es = pl1->intersectWith(pl2, AcDb::kOnBothOperands, ptArr);

			if (es != ErrorStatus::eOk) {

				pl1->close();
				pl2->close();
				acutPrintf(L"实体一和实体二相交时出错，error=%d\n", es);
				continue;
			}

			if (ptArr.length() < 2) {

				pl1->close();
				pl2->close();
				acutPrintf(L"实体一和实体二交点只能是2个,当前个数：%d\n", ptArr.length());
				continue;

			}
			AcGePoint3d maxpt1, maxpt2;
			if (ptArr.length() > 2) {

				double distance1 = 0;

				for (int i = 0; i < ptArr.length(); i++)
				{

					for (int j = 1; j < ptArr.length(); j++)
					{

						double d = ptArr[j].distanceTo(ptArr[i]);

						if (d > distance1) {

							distance1 = d;
							maxpt1 = ptArr[i];

							maxpt2 = ptArr[j];

						}


					}

				}

				ptArr.removeAll();

				ptArr.append(maxpt1);
				ptArr.append(maxpt2);


			}

			AcGePoint3dArray ptArr2;
			ptArr2.append(ptArr[0]);
			ptArr2.append(ptArr[1]);


			AcGePoint2dArray pt2dArr1, pt2dArr2;

			getPtArrs(pt2dArr1, pl1);
			getPtArrs(pt2dArr2, pl2);

			int index1 = 0, index2 = 0, index3 = 0, index4 = 0;

			changePt(pl1, ptArr);

			changePt(pl2, ptArr2);

			insertPt(pl1, pt2dArr1, ptArr[0], index1);
			insertPt(pl1, pt2dArr1, ptArr[1], index2);

			insertPt(pl2, pt2dArr2, ptArr2[0], index3);
			insertPt(pl2, pt2dArr2, ptArr2[1], index4);

			int indexPt1 = 0, indexPt2 = 0;

			ptArr.find(ptArr2[0], indexPt1, 0);
			ptArr.find(ptArr2[1], indexPt2, 0);



			/*
						if (indexPt1 < indexPt2) {

							for (int i= index2,j=0;i>index1 +1;i--,j++)
							{
								double b = 0.0;
								pl1->getBulgeAt(i, b);

								pl2->addVertexAt(index3 + j, pt2dArr1[i], b, 0, 0);
							}
						}
						else {
							for (int i = index1, j = 0; i < index2 + 1; i++, j++)
							{
								double b = 0.0;
								pl1->getBulgeAt(i, b);

								pl2->addVertexAt(index3 + j, pt2dArr1[i], b, 0, 0);
							}

						}
						*/
			AcDbPolyline *pl3 = new AcDbPolyline();
			AcDbPolyline *pl4 = new AcDbPolyline();
			AcDbPolyline *pl5 = new AcDbPolyline();
			AcDbPolyline *pl6 = new AcDbPolyline();

			pl3->setColorIndex(2);
			pl4->setColorIndex(1);
			pl5->setColorIndex(3);
			pl6->setColorIndex(4);
			
			int tempIndex1, tempIndex2;


			if (index1 > index2) {
				tempIndex1 = index1;
				index1 =index2;
				index2 = tempIndex1;
			}

			if (index3 > index4) {

				tempIndex2 = index3;
				index3 = index4;
				index4 = tempIndex2;
			}


			for (int i = index1,j=0; i <= index2; i++)
			{
				AcGePoint2d pt;

				pl1->getPointAt(i, pt);

				double b;
				pl1->getBulgeAt(i, b);

				pl5->addVertexAt(j++, pt, b, 0, 0, 0);

			}
			int sum = pl1->numVerts();
			int syPtCount0 = sum - index2 + index1 + 1;

			for (int i = 0; i < syPtCount0; i++)
			{

				AcGePoint2d pt;
				double b = 0;
				int m = (index2 + i) % sum;
				pl1->getPointAt(m, pt);
				pl1->getBulgeAt(m, b);

				pl6->addVertexAt(i, pt, b, 0, 0);

			}

			

				for (int i = index3,j=0; i <= index4; i++)
				{
					AcGePoint2d pt;

					pl2->getPointAt(i, pt);

					double b;
					pl2->getBulgeAt(i, b);

					pl3->addVertexAt(j++, pt, b, 0, 0, 0);



				}
			
				int syPtCount = pl2->numVerts() - index4+index3+1;


				for (int i=0;i<syPtCount;i++)
				{

					AcGePoint2d pt;
					double b = 0;

					pl2->getPointAt((index4 + i) % pl2->numVerts(), pt);
					pl2->getBulgeAt((index4 + i) % pl2->numVerts(), b);

					pl4->addVertexAt(i, pt, b, 0, 0);

				}

				AcGePoint3d pt1 = ptArr[0];
				AcGePoint3d pt2 = ptArr[1];

				AcGePoint3d ptMiddle = AcGePoint3d((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2, 0);

				AcGePoint3d ptMiddleEnd=ptMiddle+ AcGeVector3d::kXAxis*100;

				AcDbLine *l = new AcDbLine(ptMiddle, ptMiddleEnd);

				AcGePoint3dArray tArr1,tArr2;

				pl5->intersectWith(l, AcDb::kExtendArg, tArr1);
				pl6->intersectWith(l, AcDb::kExtendArg, tArr2);

				AcDbCurve * tpl1 = NULL,*tpl2=NULL;

				if (tArr1[0].x > tArr2[0].x) {

					tpl1 = pl5;

					delete pl6;

					pl6 = NULL;
				}
				else {
					tpl1 = pl6;
					delete pl5;
					pl5 = NULL;
				}
				tArr1.removeAll();
				tArr2.removeAll();

				pl3->intersectWith(l, AcDb::kExtendArg, tArr1);
				pl4->intersectWith(l, AcDb::kExtendArg, tArr2);


				if (tArr1[0].x < tArr2[0].x) {

					tpl2 = pl4;

					delete pl3;
					pl3 = NULL;
				}
				else {
					tpl2 = pl3;

					delete pl4;
					pl4 = NULL;
				}
				AcDbJoinEntityPE* pJoinPE = AcDbJoinEntityPE::cast(tpl2->queryX(AcDbJoinEntityPE::desc()));


				if (pJoinPE != NULL) {

					Acad::ErrorStatus es = pJoinPE->joinEntity(tpl2, tpl1);

					if (Acad::eOk == es) {

						tpl2->setColorIndex(2);
						PostToModelSpace(tpl2, acdbHostApplicationServices()->workingDatabase());
						tpl2->close();

					}
					else {
						AfxMessageBox(TEXT("修改失败"));
						delete tpl2;
						tpl2 = NULL;

						delete tpl1;
						tpl1 = NULL;
					}
				} 
				else {
					AfxMessageBox(TEXT("合成对象构建失败"));
					delete tpl2;
					tpl2 = NULL;

					delete tpl1;
					tpl1 = NULL;
				
				}

				/*PostToModelSpace(pl4, acdbHostApplicationServices()->workingDatabase());

				pl4->close();

				PostToModelSpace(pl5, acdbHostApplicationServices()->workingDatabase());

				pl5->close();
				PostToModelSpace(pl6, acdbHostApplicationServices()->workingDatabase());

				pl6->close();*/

			pl1->close();
			pl2->close();
			/*PostToModelSpace(pl3, acdbHostApplicationServices()->workingDatabase());
			pl3->close();
*/
			/*


						AcDbVoidPtrArray voidArr1, voidArr2;

						es = pl1->getSplitCurves(ptArr, voidArr1);

						if (es != ErrorStatus::eOk) {

							pl1->close();
							pl2->close();

							acutPrintf(L"划分pl1时出错。error=%d", es);
							return;
						}
						es = pl2->getSplitCurves(ptArr, voidArr2);

						if (es != ErrorStatus::eOk) {

							pl1->close();
							pl2->close();

							acutPrintf(L"划分pl2时出错。error=%d", es);
							return;
						}

						AcGePoint3d pt1 = ptArr[0];
						AcGePoint3d pt2 = ptArr[1];

						AcGePoint3d ptMiddle = AcGePoint3d((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2, 0);

						if (voidArr1.length() != 2) {

							for (int i = 0; i < voidArr1.length(); i++)
							{
								delete voidArr1[i];
								voidArr1[i] = NULL;

							}
							voidArr1.removeAll();

							ptArr.reverse();

							pl1->getSplitCurves(ptArr, voidArr1);

							if (voidArr1.length() != 2) {

								pl1->close();
								pl2->close();

								for (int i = 0; i < voidArr1.length(); i++)
								{
									delete voidArr1[i];
									voidArr1[i] = NULL;

								}

								acutPrintf(L"错误，分解pl1得到的curve只能是两个，当前个数：%d\n", voidArr1.length());
								return;
							}
						}

						if (voidArr2.length() != 2) {

							for (int i = 0; i < voidArr2.length(); i++)
							{
								delete voidArr2[i];
								voidArr2[i] = NULL;

							}
							voidArr2.removeAll();

							ptArr.reverse();
							pl2->getSplitCurves(ptArr, voidArr2);
							if (voidArr2.length() != 2) {

								pl1->close();
								pl2->close();

								for (int i = 0; i < voidArr2.length(); i++)
								{
									//delete voidArr2[i];
									//voidArr2[i] = NULL;

									AcDbCurve * ct = (AcDbCurve*)voidArr2[i];

									ct->setColorIndex(i + 1);

									PostToModelSpace(ct, acdbHostApplicationServices()->workingDatabase());
									ct->close();


								}

								acutPrintf(L"错误，分解pl2得到的curve只能是2个,当前个数：%d\n", voidArr2.length());
								return;
							}
						}

						AcDbCurve *c1 = (AcDbCurve*)voidArr1[0];
						AcDbCurve *c2 = (AcDbCurve*)voidArr1[1];

						AcGePoint3d tempPt1, tempPt2;

						c1->getClosestPointTo(ptMiddle, tempPt1, true);
						c2->getClosestPointTo(ptMiddle, tempPt2, true);

						double dis1 = 0.0, dis2 = 0.0;
						dis1 = tempPt1.distanceTo(ptMiddle);
						dis2 = tempPt2.distanceTo(ptMiddle);

						AcDbCurve* tempCurve = NULL;

						if (dis1 < dis2) {

							delete c2;
							c2 = NULL;
							tempCurve = c1;

						}
						else {
							delete c1;
							c1 = NULL;
							tempCurve = c2;
						}

						AcDbCurve *c3 = (AcDbCurve*)voidArr2[0];
						AcDbCurve *c4 = (AcDbCurve*)voidArr2[1];

						AcGePoint3d tempPt3, tempPt4;

						c3->getClosestPointTo(ptMiddle, tempPt3, true);
						c4->getClosestPointTo(ptMiddle, tempPt4, true);

						double dis3 = 0.0, dis4 = 0.0;
						dis3 = tempPt3.distanceTo(ptMiddle);
						dis4 = tempPt4.distanceTo(ptMiddle);

						AcDbCurve* tempCurve2 = NULL;

						if (dis3 < dis4) {

							delete c3;
							c3 = NULL;
							tempCurve2 = c4;

						}
						else {
							delete c4;
							c4 = NULL;
							tempCurve2 = c3;
						}



						AcDbJoinEntityPE* pJoinPE = AcDbJoinEntityPE::cast(tempCurve2->queryX(AcDbJoinEntityPE::desc()));


						if (pJoinPE != NULL) {

							Acad::ErrorStatus es = pJoinPE->joinEntity(tempCurve2, tempCurve);

							if (Acad::eOk == es) {

								// myEntity1 should updated
								tempCurve2->setColorIndex(2);

								PostToModelSpace(tempCurve2, acdbHostApplicationServices()->workingDatabase());

								tempCurve2->close();

								pl2->erase();
								pl2->close();

								pl1->close();

							}
							else {
								AfxMessageBox(TEXT("修改失败"));

								//delete tempCurve2;
								//delete tempCurve;
								tempCurve2->setColorIndex(1);
								tempCurve->setColorIndex(3);

								PostToModelSpace(tempCurve,acdbHostApplicationServices()->workingDatabase());
								PostToModelSpace(tempCurve2,acdbHostApplicationServices()->workingDatabase());

								tempCurve->close();
								tempCurve2->close();


								pl2->close();

								pl1->close();
							}


						}
						else {

							delete tempCurve2;
							delete tempCurve;

							pl2->close();
							pl1->close();

						}

						*/
		}

	}

	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb) {

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

	static void getPtArrs(AcGePoint2dArray&arr, AcDbPolyline *pl) {

		for (int i = 0; i < (int)pl->numVerts(); i++)
		{
			AcGePoint2d pt;

			pl->getPointAt(i, pt);
			arr.append(pt);
		}

	}

	static void changePt(AcDbPolyline *pl, AcGePoint3dArray& arr) {

		double p1 = 0.0, p2 = 0.0;

		pl->getDistAtPoint(arr[0], p1);
		pl->getDistAtPoint(arr[1], p2);

		if (p1 > p2) {

			arr.reverse();

		}




	}

	static void insertPt(AcDbPolyline *pl, AcGePoint2dArray& plArr, AcGePoint3d& pt, int &index) {

		double dis = 0.0;
		double ptDis = 0.0;

		pl->getDistAtPoint(pt, ptDis);


		int i = 1;
		while (dis < ptDis&&i < plArr.length()) {

			AcGePoint3d tempPt = AcGePoint3d(plArr[i].x, plArr[i].y, 0);

			pl->getDistAtPoint(tempPt, dis);
			++i;
		}

		//index = (i + plArr.length() - 1) % plArr.length();
		index = i;
		pl->addVertexAt(index, AcGePoint2d(pt.x, pt.y), 0, 0, 0);

		plArr.insertAt(index, AcGePoint2d(pt.x, pt.y));

	}

	

};



//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdXiHeApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdXiHeApp, ECDMyGroup, EcdXiHe, EcdXiHe, ACRX_CMD_MODAL, NULL)


