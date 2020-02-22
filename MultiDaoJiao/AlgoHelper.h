#pragma once
class AlgoHelper
{
public:
	AlgoHelper();
	~AlgoHelper();
	static void Fillet(AcGePoint2d vertex,
		AcGeVector2d vec1, AcGeVector2d vec2,
		double radius, AcGePoint2d &pt1, AcGePoint2d &pt2);
};

