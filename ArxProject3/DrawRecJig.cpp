#include "stdafx.h"
#include "DrawRecJig.h"
#include"GePointUtil.h"
#include"MathUtil.h"
#include"ConvertUtil.h"
#include"DwgDataBaseUtil.h"
DrawRecJig::DrawRecJig()
{
	pl = NULL;
}


DrawRecJig::~DrawRecJig()
{
}

AcEdJig::DragStatus DrawRecJig::sampler()
{
	setUserInputControls((AcEdJig::UserInputControls)(

		AcEdJig::kAccept3dCoordinates |
		AcEdJig::kNoNegativeResponseAccepted |
		AcEdJig::kNullResponseAccepted
		));
	AcGePoint3d ptTemp;

	AcEdJig::DragStatus status = acquirePoint(ptTemp);

	if (ptTemp != ptCurrent) {

		ptCurrent= ptTemp;

	}
	else if (status == DragStatus::kNormal) {

		return AcEdJig::kNoChange;
	}
	return status;


}

bool DrawRecJig::update()
{
	double dist = CConvertUtil::ToPoint2d(ptCenter).distanceTo(CConvertUtil::ToPoint2d(ptCurrent));
	for (int i = 0; i < 4; i++)
	{
		double angle = i*CMathUtil::PI()*0.5 + CMathUtil::PI()*0.25;
		AcGePoint2d pt = CGePointUtil::PolarPoint(CConvertUtil::ToPoint2d(ptCenter), angle, dist);
		pl->setPointAt(i, pt);
	}
	return true;

}

AcDbEntity * DrawRecJig::entity() const
{
	return pl;
}

bool DrawRecJig::DoIt(AcGePoint3d & ptCenter, AcDbObjectId &plId)
{
	
	this->ptCenter = ptCenter;

	pl = new AcDbPolyline();

	for (int i = 0; i < 4; i++)
	{
		pl->addVertexAt(i, CConvertUtil::ToPoint2d(ptCenter));
	}
	pl->setClosed(true);

	setDispPrompt(L"«ÎÕœ∂Ø Û±Í£∫");

	if (drag()==kNormal) {

		plId=CDwgDataBaseUtil::PostToModelSpace(pl);

		return true;
	}
	else {
		delete pl;
		return false;

	}

}
