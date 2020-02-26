#include "StdAfx.h"
#include "resource.h"
#include "acedCmdNF.h"
#include<vector>

using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdLunKuoApp : public AcRxArxApp {

public:
	CEcdLunKuoApp() : AcRxArxApp() {}

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

	static void ECDMyGroupEcdLunKuo() {


		ads_name ssName;

		AcDbObjectIdArray oIdArr;

		ErrorStatus es;

		if (acedSSGet(NULL, NULL, NULL, NULL, ssName) == RTNORM) {

			int length = 0;

			acedSSLength(ssName, &length);

			for (int i = 0; i < length; i++)
			{
				ads_name aName;
				acedSSName(ssName, i, aName);

				AcDbObjectId oid = wmfoutIn(aName);
				if (oid != AcDbObjectId::kNull) {
					oIdArr.append(oid);
				}
				AcDbObjectId plId;
				acdbGetObjectId(plId, aName);
				AcDbEntity * ent = NULL;

			}



			for (int i = 0; i < length; i++)
			{
				ads_name aName;
				acedSSName(ssName, i, aName);
				AcDbObjectId plId;
				acdbGetObjectId(plId, aName);

				AcDbEntity *ent = NULL;

				if (acdbOpenObject(ent, plId, AcDb::kForWrite) == Acad::eOk) {
					ent->erase();
					ent->close();
				
				}
			}
		}
		acedSSFree(ssName);

		AcDbBlockReference* pBlk = NULL;
		vector<AcDbLine*>vecLines;
		AcGePoint3d ptBase;

		for (int i = 0; i < oIdArr.length(); i++)
		{


			if (acdbOpenObject(pBlk, oIdArr[i], AcDb::kForWrite) == Acad::eOk) {
				ptBase = pBlk->position();
				AcDbVoidPtrArray voidArr;

				es = pBlk->explode(voidArr);

				if (es == ErrorStatus::eOk) {

					for (int j = 0; j < voidArr.length(); j++)
					{

						AcDb2dPolyline *pl2d = (AcDb2dPolyline*)voidArr[j];

						AcDbVoidPtrArray voidArr2d;

						es = pl2d->explode(voidArr2d);

						if (es == ErrorStatus::eOk) {

							for (int m = 0; m < voidArr2d.length(); m++)
							{


								AcDbLine * line = (AcDbLine*)voidArr2d[m];

								vecLines.push_back(line);
							}


						}
						delete pl2d;
						pl2d = NULL;

					}




				}

				pBlk->erase();

				pBlk->close();

			}


		}

		//左下的点为起点
		for (int i = 0; i < (int)vecLines.size(); i++)
		{

			AcDbLine *l = vecLines[i];

			if (l->startPoint().x > l->endPoint().x) {

				AcGePoint3d tempPt = l->startPoint();

				l->setStartPoint(l->endPoint());

				l->setEndPoint(tempPt);

			}
			else if (l->startPoint().y > l->endPoint().y) {

				AcGePoint3d tempPt = l->startPoint();

				l->setStartPoint(l->endPoint());

				l->setEndPoint(tempPt);

			}

		}


		vector<AcDbLine*>vecL2;

		//int index[3000] = { 0 };

		//memset(index, 0, sizeof(int));
		//int mIndex = 0;
		for (int i = 0; i < (int)vecLines.size(); i++)
		{
			AcDbLine * l1 = vecLines[i];
			if (l1 == NULL) {
				continue;
			}
			//AcGeVector3d v1 = l1->endPoint() - l1->startPoint();
			int m = 0;
			for (int j = i + 1; j < (int)vecLines.size(); j++)
			{
				AcDbLine * l2 = vecLines[j];
				if (l2 == NULL || l1 == NULL) {
					break;
				}
				/*AcGeVector3d v2 = l2->endPoint() - l2->startPoint();

				double angle = v2.angleTo(v1);

				angle = angle / 3.1415926 * 180;

				if (fabs(angle) < 1) {

					deleteVec(vecLines, l1);
					deleteVec(vecLines, l2);

				}*/
				if (IsEqual(l1->startPoint(), l2->startPoint(), 1e-4) && IsEqual(l1->endPoint(), l2->endPoint(), 1e-4)) {

					/*deleteVec(vecLines, l1);
					deleteVec(vecLines, l2);*/
					/*delete  l1;
					delete l2;
					l1 = NULL;
					l2 = NULL;*/
					vecLines.erase(vecLines.begin() + i - m, vecLines.begin() + i + 1 - m);
					m++;
					vecLines.erase(vecLines.begin() + j - m, vecLines.begin() + j + 1 - m);
					m++;

				}

			}

		}


		for (int i = 0; i < (int)vecLines.size(); i++) {

			AcDbLine* l = vecLines[i];

			if (l != NULL) {

				vecL2.push_back(l);
			}

		}


		AcDbObjectIdArray ids;
		//

		for (int i = 0; i < (int)vecLines.size(); i++)
		{

			//curveSegments.append(vecLines[i]);

			//vecLines[i]->transformBy(matrix);
			vecLines[i]->setColorIndex(1);
			ids.append(PostToModelSpace(vecLines[i]));

			vecLines[i]->close();

		}




		ads_name result;
	
		if (acedSSGet(NULL, NULL, NULL, NULL, result) == RTNORM)
		{
			int i = acedCommandS(RTSTR, _T("PEDIT"),
				RTSTR, _T("M"),
				RTPICKS, result,
				RTSTR, _T(""),
				RTSTR, _T("Y"),
				RTSTR, _T("J"),
				RTSTR, _T("1"),
				RTSTR, _T(""),
				RTSTR, _T(""),
				RTNONE);
			acutPrintf(L"%d", i);
		}
		acedSSFree(result);
		//acedGetAcadTextCmdLine()->PostMessage(WM_CHAR, VK_ESCAPE, 0);

		/*int i = acedCommandS(RTSTR, _T("PEDIT"),
			RTSTR, _T("M"),
			RTPICKS, result,
			RTSTR, _T(""),
			RTSTR, _T("Y"),
			RTSTR, _T("J"),
			RTSTR, _T("1"),
			RTNONE);
		acutPrintf(L"%d", i);*/
		/*int i = acedCommandS(RTSTR, _T("PEDIT"),
			RTENAME, result,
			RTSTR, _T("Y"),
			RTSTR, _T("J"),
			RTPICKS, result,
			RTSTR, _T(""), RTSTR, _T(""), RTNONE);*/
		acedSSFree(result);

		/*if (AcDbRegion::createFromCurves(curveSegments, regions) == Acad::eOk) {

				AcDbRegion *region = (AcDbRegion *)regions[0];
				AcGePoint3dArray ptArr;
				GetRegionPoints(region, ptArr);

				region->setColorIndex(2);

				PostToModelSpace(region);
				region->close();


				AcDbPolyline *plTemp1 = new AcDbPolyline();

				for (int i = 0; i < ptArr.length(); i++)
				{

					plTemp1->addVertexAt(plTemp1->numVerts(), AcGePoint2d(ptArr.at(i).x, ptArr.at(i).y), 0, 0, 0);
				}

				//plTemp1->setClosed(Adesk::kTrue);

				plTemp1->setColorIndex(1);

				PostToModelSpace(plTemp1);

				plTemp1->close();

			}*/

	}
	static void ECDMyGroupEcdTest() {

		ads_name ssName;
		if (acedSSGet(NULL, NULL, NULL, NULL, ssName) == RTNORM) {

			ads_name ss1;
			int len;

			acedSSLength(ssName, &len);

			acedSSName(ssName, 0, ss1);

			int j = acedCommandS(RTSTR, _T("Line"), RTSTR, _T("0,0"), RTSTR, _T("111,111"), RTSTR, _T(""), RTNONE);

			acutPrintf(L"%d%n", j);


			/*	int i = acedCommandS(RTSTR, _T("PEDIT"),
					RTENAME,ss1,
					RTSTR, _T("Y"),
					RTSTR, _T("J"),
					RTPICKS, ssName,
					RTSTR, _T(""), RTNONE);*/


			int i = acedCommandS(RTSTR, _T("PEDIT"),
				RTSTR, _T("M"),
				RTPICKS, ssName,
				RTSTR, _T(""),
				RTSTR, _T("Y"),
				RTSTR, _T("J"),
				RTSTR, _T("1"),
				RTNONE);
			//acedGetAcadTextCmdLine()->PostMessage(WM_CHAR, VK_ESCAPE, 0);

			acutPrintf(L"%d", i);

		}

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
	static AcDbObjectId wmfoutIn(ads_name& ssname)
	{
		acedCommandS(RTSTR, L"_.wmfout", RTSTR, L"E:\\temp_1.wmf", RTENAME, ssname, RTSTR, L"", RTNONE);
		acedCommandS(RTSTR, L"_.import", RTSTR, L"E:\\temp_1.wmf", RTNONE);
		acdbEntLast(ssname);
		AcDbObjectId entId;
		acdbGetObjectId(entId, ssname);
		AcDbBlockReference* pBlk = NULL;
		AcGePoint3d ptBase;
		AcGeMatrix3d mat;
		if (acdbOpenObject(pBlk, entId, AcDb::kForWrite) == Acad::eOk)
		{
			ptBase = pBlk->position();
			mat.setToScaling(2.0, ptBase);
			pBlk->transformBy(mat);
			pBlk->close();

			return entId;
		}

		return AcDbObjectId::kNull;
	}
	static bool IsEqual(const AcGePoint3d & firstPoint, const AcGePoint3d & secondPoint, double tol)
	{
		return (fabs(firstPoint.x - secondPoint.x) < tol&&
			fabs(firstPoint.y - secondPoint.y) < tol&&
			fabs(firstPoint.z - firstPoint.z) < tol);
	}


	static void GetRegionPoints(AcDbRegion* pRegion, AcGePoint3dArray &points)

	{
		AcBrBrep*pBrep = new AcBrBrep;

		pBrep->set(*pRegion);

		AcBrBrepFaceTraverser brFaTrav;

		for (brFaTrav.setBrep(*pBrep); !brFaTrav.done(); brFaTrav.next())

		{

			AcBrFaceLoopTraverser faLoTrav;

			AcBrFace face;

			brFaTrav.getFace(face);

			for (faLoTrav.setFace(face); !faLoTrav.done(); faLoTrav.next())

			{

				AcBrLoopEdgeTraverser loEdTrav;
				AcBr::ErrorStatus es;
				es = loEdTrav.setLoop(faLoTrav);
				if (es == AcBr::eOk)

				{

					for (; !loEdTrav.done(); loEdTrav.next())

					{

						AcBrEdge edge;

						loEdTrav.getEdge(edge);

						AcBrVertex start;

						edge.getVertex1(start);

						AcGePoint3d pt;

						start.getPoint(pt);

						points.append(pt);

					}

				} // else its an isolated loop
				else {
					acutPrintf(L"%d", es);


				}
			}

		}

		delete pBrep;

	}

	static void getPoints(

		AcDbObjectId id,

		AcGePoint3dArray &points)

	{

		AcBrBrep*pBrep = new AcBrBrep;

		pBrep->setSubentPath(AcDbFullSubentPath(id, kNullSubentId));

		AcBrBrepFaceTraverser brFaTrav;

		for (brFaTrav.setBrep(*pBrep); !brFaTrav.done(); brFaTrav.next())

		{

			AcBrFaceLoopTraverser faLoTrav;

			AcBrFace face;

			brFaTrav.getFace(face);

			for (faLoTrav.setFace(face); !faLoTrav.done(); faLoTrav.next())

			{

				AcBrLoopEdgeTraverser loEdTrav;

				if (loEdTrav.setLoop(faLoTrav) == AcBr::eOk)

				{

					for (; !loEdTrav.done(); loEdTrav.next())

					{

						AcBrEdge edge;

						loEdTrav.getEdge(edge);

						AcBrVertex start;

						edge.getVertex1(start);

						AcGePoint3d pt;

						start.getPoint(pt);

						points.append(pt);

					}

				} // else its an isolated loop   

			}

		}

		delete pBrep;

	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdLunKuoApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdLunKuoApp, ECDMyGroup, EcdLunKuo, EcdLunKuo, ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdLunKuoApp, ECDMyGroup, EcdTest, EcdTest, ACRX_CMD_MODAL, NULL)

