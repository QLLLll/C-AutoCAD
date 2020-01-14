#include "stdafx.h"
#include "RegionUtil.h"
//#import "C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\lib-x64\acbr22.lib"
//#import "C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\lib-x64\acgex22.lib"
//#include"C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brbrep.h"
//#include"C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brbftrav.h"
//#include"C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brfltrav.h"
//#include"C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brletrav.h"
#include"DwgDataBaseUtil.h"

CRegionUtil::CRegionUtil()
{
}


CRegionUtil::~CRegionUtil()
{
}


void CRegionUtil::r2p(AcDbRegion* pRegion)

{

	

	/*AcBr::ErrorStatus es;

	AcGeVoidPointerArray Curves;

	AcDbCurve* pCurve = NULL;

	AcBrBrep brep;

	es = brep.set(*pRegion);

	AcGeCurve3d * pGeCurve3d = NULL;

	if (es == AcBr::eOk)

	{

		AcBrBrepFaceTraverser faceTrav;

		faceTrav.setBrep(brep);

		while (!faceTrav.done() && es == AcBr::eOk)

		{

			AcBrFace *face=NULL;

			es = faceTrav.getFace(*face);

			if (es == AcBr::eOk)

			{

				AcBrFaceLoopTraverser faceLoopTrav;

				es = faceLoopTrav.setFace(*face);

				if (es == AcBr::eOk)

				{

					while (!faceLoopTrav.done() && es == AcBr::eOk)

					{

						AcBrLoop *loop=NULL;

						es = faceLoopTrav.getLoop(*loop);

						if (es == AcBr::eOk)

						{

							AcBrLoopEdgeTraverser loopEdgeTrav;

							es = loopEdgeTrav.setLoop(*loop);

							if (es == AcBr::eOk)

							{

								while (!loopEdgeTrav.done() && es == AcBr::eOk)

								{

									AcBrEdge edge;

									es = loopEdgeTrav.getEdge(edge);

									if (es == AcBr::eOk)

									{

										es = edge.getCurve(pGeCurve3d);

										if (es == AcBr::eOk)

										{

											AcGeCurve3d *pNativeCurve = NULL;

											AcGeExternalCurve3d* pGeExter3d = (AcGeExternalCurve3d*)pGeCurve3d;

											if (pGeExter3d->isCircArc())

											{

												AcGeCircArc3d* pArc3d = NULL;

												pGeExter3d->isNativeCurve((AcGeCurve3d*&)pArc3d);

												acdbConvertGelibCurveToAcDbCurve(*pArc3d, pCurve);

												CDwgDataBaseUtil::PostToModelSpace(pCurve);

											}

											else

											{

												Adesk::Boolean bRet = ((AcGeExternalCurve3d*)pGeCurve3d)->isNativeCurve(pNativeCurve);

												Curves.append(pNativeCurve);

											}

											delete pGeCurve3d;

										}

									}

									es = loopEdgeTrav.next();

								}

								if (Curves.length() > 0)

								{

									AcGeCompositeCurve3d* pComp = new AcGeCompositeCurve3d(Curves);

									acdbConvertGelibCurveToAcDbCurve(*pComp, pCurve);

									delete pComp;

									if (!pCurve->isClosed())

									{

										for (int i = 0; i < Curves.length(); i++)

										{

											pGeCurve3d = (AcGeCurve3d*)Curves.at(i);

											pGeCurve3d->reverseParam();

										}

										delete pCurve;

										pCurve = NULL;

										pComp = new AcGeCompositeCurve3d(Curves);

										acdbConvertGelibCurveToAcDbCurve(*pComp, pCurve);

										delete pComp;

									}

									CDwgDataBaseUtil::PostToModelSpace(pCurve);

									for (int i = 0; i < Curves.length(); i++)

									{

										delete Curves.at(i);

									}

								}

								Curves.removeAll();

							}

						}

						es = faceLoopTrav.next();

					}

				}

			}

			es = faceTrav.next();

		}

	}*/

	//pRegion->erase();

}