#pragma once
#include"DatabaseJigEntity.h"
class CMoveJig:public AcEdJig
{
public:
		CMoveJig(const AcGePoint3d & fromPoint) : m_pEnt(NULL), m_FromPoint(fromPoint), m_ToPoint(fromPoint) {}
		CMoveJig(const AcGePoint3d & fromPoint, const AcGePoint3d & toPoint) : m_pEnt(NULL), m_FromPoint(fromPoint), m_ToPoint(toPoint) {}
		~CMoveJig();
		void	doIt(const AcDbObjectIdArray & ids, bool bCopy = false);
		void UpdateDoIt(const AcDbObjectIdArray & ids, bool bCopy = false);
		virtual AcEdJig::DragStatus sampler();
		virtual Adesk::Boolean update();
		virtual AcDbEntity* entity() const;
		AcGePoint3d			m_ToPoint;
		AcDbObjectIdArray m_idsC;
private:
	CDatabaseJigEntity	*m_pEnt;
	AcGePoint3d			m_FromPoint;
	
	AcGeMatrix3d		m_Xform;
};


