#include "stdafx.h"
#include "AlgoHelper.h"
#include "GePointUtil.h"

AlgoHelper::AlgoHelper()
{
}


AlgoHelper::~AlgoHelper()
{
}
//pt2ÔÚÇ°
void AlgoHelper::Fillet(AcGePoint2d vertex, AcGeVector2d vec1, AcGeVector2d vec2, double radius, AcGePoint2d & pt1, AcGePoint2d & pt2)
{
	AcGeVector2d uvec1 = vec1.normal();
	AcGeVector2d uvec2 = vec2.normal();
	AcGeVector2d vecToCenterUnit = (uvec1 + uvec2).normal();

	AcGeVector2d vecToCenter = vecToCenterUnit * radius /
		sin(min(vecToCenterUnit.angleTo(uvec1),
			vecToCenterUnit.angleTo(uvec2)));

	AcGeVector2d projVec1 = uvec1 * uvec1.dotProduct(vecToCenter);
	AcGeVector2d projVec2 = uvec2 * uvec2.dotProduct(vecToCenter);

	pt1 = vertex + projVec1;
	pt2 = vertex + projVec2;
	AcGePoint2d tempPt=pt1;


	/*if (pt1.x > pt2.x) {

		pt1 = pt2;

		pt2 = tempPt;
	}
	else if (fabs(pt1.x - pt2.x) < 1e-4) {

		if (pt1.y > pt2.y) {

			pt1 = pt2;
			pt2 = tempPt;

		}

	}*/
}
