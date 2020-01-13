#pragma once
#include"DatabaseJigEntity.h"
class CMoveJig:public AcEdJig
{
public:
		CMoveJig(const AcGePoint3d & fromPoint) : m_pEnt(NULL), m_FromPoint(fromPoint), m_ToPoint(fromPoint) {}
		~CMoveJig();
		void	doIt(const AcDbObjectIdArray & ids, bool bCopy = false);
		virtual AcEdJig::DragStatus sampler();
		virtual Adesk::Boolean update();
		virtual AcDbEntity* entity() const;
private:
	CDatabaseJigEntity	*m_pEnt;
	AcGePoint3d			m_FromPoint;
	AcGePoint3d			m_ToPoint;
	AcGeMatrix3d		m_Xform;
};


