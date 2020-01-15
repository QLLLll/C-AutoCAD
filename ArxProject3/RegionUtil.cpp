#include "stdafx.h"
#include "RegionUtil.h"
//#include"C:Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brbrep.h"
//#include"C:Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brbftrav.h"
//#include"C:Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brfltrav.h"
//#include"C:Autodesk_ObjectARX_2018_Win_64_and_32_Bit\utils\brep\inc\brletrav.h"
//#include"DwgDataBaseUtil.h"

CRegionUtil::CRegionUtil()
{
}


CRegionUtil::~CRegionUtil()
{
}


void CRegionUtil::GetRegionPoints(AcDbRegion* pRegion,AcGePoint3dArray &points)

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