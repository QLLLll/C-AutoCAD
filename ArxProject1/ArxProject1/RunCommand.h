#pragma once
class RunCommand
{
public:
	RunCommand();
	~RunCommand();
	AcDbObjectId CreateLine();
	Acad::ErrorStatus ChangeColor(AcDbObjectId entId, Adesk::UInt16 coloridx);
	 

};

