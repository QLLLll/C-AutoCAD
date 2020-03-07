#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdXClipApp : public AcRxArxApp {

public:
	CEcdXClipApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	
	static void ECDMyGroupEcdXClip () {
		
		MyTestApp_MyClip();
	}
	static void ECDMyGroupEcdXClipBy() {

		ads_point pt1;
		ads_name ent;
		if (acedEntSel(_T("选择块:"), ent, pt1) != RTNORM)
			return;
		GetBlockClippingPolyline(ent);
	}

	static void MyTestApp_MyClip()
	{
		ads_point pt1, pt2;
		ads_name ent;
		if (acedEntSel(_T("选择块:"), ent, pt1) != RTNORM)
			return;
		AcDbObjectId idXref;
		if (acdbGetObjectId(idXref, ent) != Acad::eOk)
			return;
		AcDbObjectPointer<AcDbBlockReference> pRef(idXref, AcDb::kForRead);
		if (pRef.openStatus() != Acad::eOk)
		{
			acutPrintf(_T("不是一个块!\n"));
			return;
		}
		AcGePoint2dArray pts;
		if (acedGetPoint(NULL, _T("选择第一个点："), pt1) != RTNORM)
			return;
		
		AcGeMatrix3d mat(pRef->blockTransform());
		mat.invert();
		AcGePoint3d pt3d(asPnt3d(pt1));
		pt3d.transformBy(mat);
		pts.append(AcGePoint2d(pt3d.x, pt3d.y));
		while (acedGetPoint(pt1, _T("选择第二个点："), pt2) == RTNORM)
		{
			acedGrDraw(pt1, pt2, 1, 1);
			pt3d = asPnt3d(pt2);
			pt3d.transformBy(mat);
			pts.append(AcGePoint2d(pt3d.x, pt3d.y));
			memcpy(pt1, pt2, sizeof(ads_point));
		}
		acedRedraw(NULL, 0);
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		AcGeVector3d normal;
		double elev;
		if (pDb->tilemode())
		{
			normal = pDb->ucsxdir().crossProduct(pDb->ucsydir());
			elev = pDb->elevation();
		}
		else
		{
			normal = pDb->pucsxdir().crossProduct(pDb->pucsydir());
			elev = pDb->pelevation();
		}
		normal.normalize();
		Acad::ErrorStatus es = pRef.object()->upgradeOpen();
		if (es != Acad::eOk)
			return;
		
		AcDbSpatialFilter* pFilter = new AcDbSpatialFilter;
		if (pFilter->setDefinition(pts, normal, elev,
			ACDB_INFINITE_XCLIP_DEPTH, -ACDB_INFINITE_XCLIP_DEPTH, true) != Acad::eOk)
		{
			delete pFilter;
			return;
		}
		
		if (AcDbIndexFilterManager::addFilter(pRef.object(), pFilter) != Acad::eOk)
			delete pFilter;
		else
		{
			acutPrintf(_T("剪切块成功\n"));
			pFilter->close();
		}
	}

	

	static BOOL drawAPlineWithPropertiesFromBlockRef(AcGePoint2dArray pts,

		AcDbBlockReference* ref, double elevation, AcGeVector3d& normal)

	{

		AcDbPolyline *pl = new AcDbPolyline;

		AcDbObjectId owner;



		pl->setDatabaseDefaults();

		pl->setClosed(Adesk::kTrue);

		pl->setThickness(0.0);



		if (ref != NULL) {

			owner = ref->ownerId();

			pl->setPropertiesFrom(ref);

		}



		pl->setNormal(normal);



		for (int i = 0; i < pts.length(); i++)

		{

			pl->addVertexAt(i, pts[i]);

		}



		pl->setElevation(elevation);

		pl->setColorIndex(1); // Red



		AcDbBlockTableRecord *rec = NULL;

		acdbOpenObject(rec, owner, AcDb::kForWrite);

		if (rec != NULL)

		{

			AcDbObjectId id;

			rec->appendAcDbEntity(id, pl);

			pl->close();

			rec->close();

		}

		else

		{

			delete pl;

			return false;

		}

		return true;

	}

	
	static BOOL GetBlockClippingPolyline(ads_name blk)

	{

		BOOL ret = FALSE;

		AcDbBlockReference *ref = NULL;

		AcDbObjectId insId = AcDbObjectId::kNull;

		acdbGetObjectId(insId, blk);

		if (acdbOpenObject(ref, insId, AcDb::kForRead) != Acad::eOk)

			return ret;

		AcDbObjectId extDicId = ref->extensionDictionary();

		if (extDicId == AcDbObjectId::kNull)

			return ret;

		AcDbDictionary *extDict = NULL, *acadFilterDict = NULL;

		if (acdbOpenObject(extDict, extDicId, AcDb::kForRead) != Acad::eOk)

			return ret;

		Acad::ErrorStatus err = extDict->getAt(_T("ACAD_FILTER"), (AcDbObject*&)acadFilterDict, AcDb::kForRead);

		extDict->close();

		if (err != Acad::eOk)

			return ret;

		AcDbSpatialFilter *filter = NULL;

		err = acadFilterDict->getAt(_T("SPATIAL"), (AcDbObject*&)filter, AcDb::kForRead);

		acadFilterDict->close();

		if (err != Acad::eOk)

			return ret;

		AcGeMatrix3d xformInBoundary = filter->getClipSpaceToWCSMatrix(xformInBoundary);

		AcGeMatrix3d xformRefOrig = filter->getOriginalInverseBlockXform(xformRefOrig);

		AcGeMatrix3d refMatrix = ref->blockTransform();

		refMatrix = refMatrix.postMultBy(xformRefOrig);

		AcGeMatrix3d finalMatrix = refMatrix.postMultBy(xformInBoundary);

		AcGePoint2dArray pts;

		AcGePoint3dArray pts3d;


		AcGeVector3d normal;

		double elevation = 0, frontClip = 0, backClip = 0;

		Adesk::Boolean enabled = false;

		if (filter->getDefinition(pts, normal, elevation, frontClip, backClip, enabled) == Acad::eOk)

		{

			if (pts.length() == 2)

			{

				AcGePoint2d p1(pts[1].x, pts[0].y);

				AcGePoint2d p3(pts[0].x, pts[1].y);

				pts.insertAt(1, p1);

				pts.insertAt(3, p3);

			}

			for (int i = 0; i < pts.length(); i++)

			{

				AcGePoint2d pt2d;

				AcGePoint3d pt3d;

				pt2d = pts[i];

				pt3d[0] = pt2d[0]; pt3d[1] = pt2d[1]; pt3d[2] = 0;

				pt3d.transformBy(finalMatrix);

				pts3d.append(pt3d);

			}

		}

		AcGeVector3d xfnorm = normal.transformBy(finalMatrix);

		AcGeMatrix3d plineECS;



		AcDbPolyline* pline = new AcDbPolyline();

		pline->setNormal(xfnorm);

		pline->getEcs(plineECS);

		delete pline; pline = NULL;

		AcGeMatrix3d plineECSInv = plineECS.inverse();



		double xfelev = 0.0;

		for (int i = 0; i < pts.length(); i++)

		{

			pts[i].x = pts3d[i].x;

			pts[i].y = pts3d[i].y;



			if (i == 0)

				xfelev = pts3d[i].z;

			assert(fabs(xfelev - pts3d[i].z) < 1.0e-10);

		}

		drawAPlineWithPropertiesFromBlockRef(pts, ref, xfelev, xfnorm);



		filter->close();

		ref->close();



		return true;

	} 
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdXClipApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdXClipApp, ECDMyGroup, EcdXClip, EcdXClip, ACRX_CMD_TRANSPARENT |
	ACRX_CMD_NOINTERNALLOCK, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdXClipApp, ECDMyGroup, EcdXClipBy, EcdXClipBy, ACRX_CMD_MODAL, NULL)

