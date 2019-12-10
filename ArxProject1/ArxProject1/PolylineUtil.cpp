#include "stdafx.h"
#include "PolylineUtil.h"
#include"DwgDataBaseUtil.h"
#include"ConvertUtil.h"
#include"MathUtil.h"
CPolylineUtil::CPolylineUtil()
{
}


CPolylineUtil::~CPolylineUtil()
{
}
AcDbObjectId  CPolylineUtil::Add(const AcGePoint2dArray &points, double width) {

	int numVerts = points.length();
	AcDbPolyline *pPoly = new AcDbPolyline(numVerts);

	for (int i = 0; i < numVerts; i++)
	{
		pPoly->addVertexAt(i, points.at(i), 0, width, width);
	}
	return  CDwgDataBaseUtil::PostToModelSpace(pPoly, acdbHostApplicationServices()->workingDatabase());


}

AcDbObjectId  CPolylineUtil::Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width) {

	AcGePoint2dArray points;
	points.append(ptStart);
	points.append(ptEnd);

	return CPolylineUtil::Add(points, width);
}
AcDbObjectId CPolylineUtil::Add3dPolyline(const AcGePoint3dArray &points)

{

	AcGePoint3dArray verts = points;

	AcDb3dPolyline *pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, verts);

	return CDwgDataBaseUtil::PostToModelSpace(pPoly3d);

}

AcDbObjectId  CPolylineUtil::AddPolygon(const AcGePoint2d &ptCenter, int number, double radius, double rotation, double width)

{

	double angle = 2 * CMathUtil::PI() / (double)number;
	
	AcGePoint2dArray points;

	for (int i = 0; i<number; i++)

	{

		AcGePoint2d pt;

		pt.x = ptCenter.x + radius*cos(i*angle);

		pt.y = ptCenter.y + radius*sin(i*angle);

		points.append(pt);



	}

	AcDbObjectId polyId = Add(points, width);



	AcDbEntity *pEnt = NULL;

	if (acdbOpenObject(pEnt, polyId, AcDb::kForWrite) == Acad::eOk)

	{

		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);

		if (pPoly != NULL)

		{

			pPoly->setClosed(Adesk::kTrue);

		}

		AcGeVector3d vecNormal = AcGeVector3d(0, 0, 1);



		pEnt->transformBy(AcGeMatrix3d::rotation(rotation, vecNormal, CConvertUtil::ToPoint3d(ptCenter)));

		pEnt->close();

	}

	//EntityUtil::Rotate(polyId, ptCenter, rotation);




	return polyId;

}

AcDbObjectId CPolylineUtil::AddRectangle(const AcGePoint2d &pt1, const AcGePoint2d &pt2, double width)

{



	double x1 = pt1.x, x2 = pt2.x;

	double y1 = pt1.y, y2 = pt2.y;



	AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));

	AcGePoint2d ptRightBottom(max(x1, x2), min(y1, y2));

	AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));

	AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));



	AcDbPolyline *pPoly = new AcDbPolyline(4);

	pPoly->addVertexAt(0, ptLeftBottom, 0, width, width);

	pPoly->addVertexAt(1, ptRightBottom, 0, width, width);

	pPoly->addVertexAt(2, ptRightTop, 0, width, width);

	pPoly->addVertexAt(3, ptLeftTop, 0, width, width);

	pPoly->setClosed(true);

	return CDwgDataBaseUtil::PostToModelSpace(pPoly);



}

AcDbObjectId CPolylineUtil::AddPolyCircle(const AcGePoint2d &ptCenter, double radius, double width)

{

	AcGePoint2d pt1, pt2, pt3;

	pt1.x = ptCenter.x + radius;

	pt1.y = ptCenter.y;

	pt2.x = ptCenter.x - radius;

	pt2.y = ptCenter.y;

	pt3.x = ptCenter.x + radius;

	pt3.y = ptCenter.y;



	AcDbPolyline *pPoly = new AcDbPolyline(3);

	pPoly->addVertexAt(0, pt1, 1, width, width);

	pPoly->addVertexAt(1, pt2, 1, width, width);

	pPoly->addVertexAt(2, pt3, 1, width, width);

	pPoly->setClosed(Adesk::kTrue);



	return CDwgDataBaseUtil::PostToModelSpace(pPoly);

}

