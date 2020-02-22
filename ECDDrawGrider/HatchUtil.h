#pragma once
class CHatchUtil
{
public:
	CHatchUtil();
	~CHatchUtil();
	static AcDbObjectId CHatchUtil::Add(const AcDbObjectIdArray &loopIds, 
		const ACHAR*patName= TEXT("SOLID"), 
		double patternScale = 1
		);
};

