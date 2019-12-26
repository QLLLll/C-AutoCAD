#pragma once

#ifdef CUSTOMOBJDBX_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP ECDDwgScaleCO : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(ECDDwgScaleCO) ;
	Adesk::Int32 m_infoScale;
	Adesk::Int32 m_lableScale;


protected:
	static Adesk::UInt32 kCurrentVersionNumber ;


public:
	ECDDwgScaleCO();
	virtual ~ECDDwgScaleCO () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;


	void Set(int infoScale, int lableScale);
	int GetInfoScale() const;
	int GetLableScale()const;

} ;

#ifdef CUSTOMOBJDBX_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ECDDwgScaleCO)
#endif
