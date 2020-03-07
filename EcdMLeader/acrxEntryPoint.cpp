#include "StdAfx.h"
#include "resource.h"
#include "dbmleader.h"
#include <vector>

using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//----- ObjectARX EntryPoint
class CEcdMLeaderApp : public AcRxArxApp {

public:
	struct MyAttr
	{
		ACHAR* attrName;
		ACHAR* attrVal;

	};

public:
	CEcdMLeaderApp () : AcRxArxApp () {}

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
	
	
	static void ECDMyGroupEcdPLBJ () {
		// Put your command code here

		ads_name sName;
		AcDbObjectIdArray idArr;

		AcGeVector3d v(30,30, 0);


		if (acedSSGet(NULL, NULL, NULL, NULL, sName) != RTNORM) {
			return;
		}

		int length = 0;

		acedSSLength(sName, &length);

		ads_name temp;
		AcDbObjectId oId;

		for (int i=0;i<length;i++)
		{

			acedSSName(sName, i, temp);

			acdbGetObjectId(oId, temp);

			idArr.append(oId);

		}

		AcDbPolyline * pl=NULL;

		for (int i=0;i<idArr.length();i++)
		{

			if (acdbOpenObject(pl, idArr[i], AcDb::kForRead) != ErrorStatus::eOk) {

				continue;
			}

			AcGePoint2d temp2d;
			CString str;
			CString str2;
			for (unsigned int i=0;i<pl->numVerts();i++)
			{
				pl->getPointAt(i, temp2d);
			
				str.Format(L"A_%d", i);


				AcGePoint3d ptBs(temp2d.x, temp2d.y, 0);

				AcGePoint3d ptTail = ptBs + v;

				double dis=0.0;
				pl->getDistAtPoint(ptBs, dis);

				str2.Format(L"%.2f", dis);

				AcDbObjectId recId = CreateBlk(L"LL_BLK");
				acutPrintf(L"OK");

				AcDbMLeader *mLeader = new AcDbMLeader();

				

				mLeader->setContentType(AcDbMLeaderStyle::ContentType::kBlockContent);

				ACHAR* juli = str2.AllocSysString();
				ACHAR* bh = str.AllocSysString();


				MyAttr attr1 = { L"æ‡¿Î", juli };
				MyAttr attr2 = { L"±‡∫≈", bh };

				vector<MyAttr>vec;
				vec.push_back(attr1);
				vec.push_back(attr2);

				AcDbObjectId refId = InsertBlkWidthAttr(recId, ptTail, vec);

				mLeader->setBlockContentId(refId);

				mLeader->setBlockPosition(ptTail);

				int m;
				mLeader->addLeaderLine(ptBs, m);
				mLeader->addLastVertex(m, ptTail);
				mLeader->setLastVertex(m, ptTail);

				AcDbObjectId id = PostToModelSpace(mLeader);
				mLeader->close();

			}


		}




	}


	static void ECDMyGroupECDAttrBlk() {
		// Put your command code here

		AcDbObjectId recId=CreateBlk(L"LL_BLK");
		acutPrintf(L"OK");

		AcDbMLeader *mLeader = new AcDbMLeader();

		
		AcGePoint3d ptBs(0, 0, 0);

		AcGePoint3d ptTail(30, 30, 0);

		MyAttr attr1 = { L"æ‡¿Î", L"222.2" };
		MyAttr attr2 = { L"±‡∫≈", L"A2" };

		vector<MyAttr>vec;
		vec.push_back(attr1);
		vec.push_back(attr2);

		AcDbObjectId refId = InsertBlkWidthAttr(recId, ptTail, vec);

		mLeader->setBlockContentId(refId);

		mLeader->setBlockPosition(ptTail);

		int i;
		mLeader->addLeaderLine(ptBs, i);
		mLeader->addLastVertex(i, ptTail);
		mLeader->setLastVertex(i, ptTail);

		AcDbObjectId id = PostToModelSpace(mLeader);

	}
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase()) {

		if (pEnt == NULL) {
			return AcDbObjectId::kNull;
		}
		AcDbBlockTable *pTable;

		AcDbBlockTableRecord *blkTblRec;

		AcDbObjectId objId;

		pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
		pTable->getAt(ACDB_MODEL_SPACE, blkTblRec, AcDb::OpenMode::kForWrite);

		pTable->close();

		Acad::ErrorStatus status = blkTblRec->appendAcDbEntity(objId, pEnt);

		if (status != Acad::eOk) {

			blkTblRec->close();

			delete pEnt;
			pEnt = NULL;

			return AcDbObjectId::kNull;

		}

		blkTblRec->close();
		pEnt->close();

		return objId;
	}
static 	AcDbObjectId CreateBlk(ACHAR *blkName,AcDbDatabase * db=acdbHostApplicationServices()->workingDatabase())
	{
		AcDbBlockTable *pBlkTable;

		if (db->getSymbolTable(pBlkTable, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
			return AcDbObjectId::kNull;

		if (pBlkTable->has(blkName)) {

			pBlkTable->close();
			AcDbObjectId recId;

			pBlkTable->getAt(blkName, recId);

			 return recId;
		}


		AcDbBlockTableRecord * pBlkRec = new AcDbBlockTableRecord();

		pBlkRec->setName(blkName);
/*
		bool isNameOK = true;
		wchar_t  buff[40];
		if (acedGetString(1, L"\n«Î ‰»ÎøÈ√˚", buff) == RTNORM)
		{
			while (pBlkTable->has(buff)) {

				CString s;
				s.Format(_T("%s"), buff);

				AfxMessageBox(TEXT("“—¥Ê‘⁄: " + s + "µƒøÈ"));

				//«Â¡„
				memset(buff, 0, sizeof(buff));

				s.Format(_T("%s"), buff);

				acutPrintf(" «∑Ò«Â¡„" + s + "\n");

				if (acedGetString(1, L"\n«Î ‰»ÎøÈ√˚", buff) == RTNORM)
				{
					continue;
				}
				else
				{
					isNameOK = false;
					break;
				}

			}
		}

		if (!isNameOK)
		{

			delete pBlkRec;

			pBlkTable->close();

			return AcDbObjectId::kNull;

		}
		pBlkRec->setName(buff);
*/
		AcDbObjectId recId;

		//pBlkTable->add(recId, pBlkRec);

		AcGePoint3d pt1(0, 0, 0);
		AcGePoint3d pt2(20, 0, 0);

		AcGePoint3d ptUp(8, 5, 0);
		AcGePoint3d ptDown(8, -5, 0);

		AcDbLine * line = new AcDbLine(pt1, pt2);

		pBlkRec->appendAcDbEntity(line);

		//‘ˆº”øÈ∂®“Â
		AcDbAttributeDefinition *attrDef1 = new AcDbAttributeDefinition(ptUp, L"0", L"æ‡¿Î", L"«Î ‰»Îæ‡¿Î£∫");
		AcDbAttributeDefinition *attrDef2 = new AcDbAttributeDefinition(ptDown, L"0", L"±‡∫≈", L"«Î ‰»Î±‡∫≈£∫");

		pBlkRec->appendAcDbEntity(attrDef1);
		pBlkRec->appendAcDbEntity(attrDef2);

		pBlkTable->add(recId,pBlkRec);

		line->close();

		attrDef1->close();
		attrDef2->close();
		pBlkRec->close();
		pBlkTable->close();

		return recId;
	}
static AcDbObjectId InsertBlkWidthAttr(AcDbObjectId blkRecId, AcGePoint3d &ptInsert,vector<MyAttr> attrs, AcDbDatabase * db = acdbHostApplicationServices()->workingDatabase())
{
	if (blkRecId.isNull())
		return blkRecId;
	AcDbBlockTableRecord *rec;

	AcDbBlockReference *br = new AcDbBlockReference(ptInsert, blkRecId);

	if (acdbOpenObject(rec, blkRecId, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
	{
		delete br;

		return AcDbObjectId::kNull;
	}

	if (rec->hasAttributeDefinitions()) {

		AcDbBlockTableRecordIterator *pItr = NULL;

		rec->newIterator(pItr);

		for (pItr->start(); !pItr->done(); pItr->step()) {

			AcDbEntity* ent = NULL;


			if (pItr->getEntity(ent) == ErrorStatus::eOk) {

				AcDbAttributeDefinition *def = AcDbAttributeDefinition::cast(ent);

				if (def != NULL) {

					AppendAttrDef2Br(br, attrs, def);

					ent->close();
				}
			}
		}
		delete pItr;
	}
	rec->close();

	AcDbObjectId brId = PostToModelSpace(br);

	br->close();
	return brId;


}
static void AppendAttrDef2Br(AcDbBlockReference * br, vector<MyAttr> attrs, AcDbAttributeDefinition * def)
{

	AcDbAttribute *attr = new AcDbAttribute();	
	attr->setPropertiesFrom(br);
	attr->setLayer(br->layerId());
	attr->setInvisible(def->isInvisible());
	attr->setPosition(def->position());
	attr->setHeight(def->height());
	attr->setWidthFactor(def->widthFactor());
	attr->setRotation(def->rotation());
	attr->setHorizontalMode(def->horizontalMode());
	attr->setVerticalMode(def->verticalMode());
	attr->setAlignmentPoint(def->alignmentPoint());
	attr->setTextStyle(def->textStyle());
	attr->setAttributeFromBlock(br->blockTransform());


	ACHAR *pStr = def->tag();

	for (size_t i=0;i<attrs.size();i++)
	{

		//pStr = attrs[i].attrName;
		if (_tcscmp(attrs[i].attrName, pStr) == 0) {

			attr->setTag(pStr);
			acutDelString(pStr);
			attr->setTextString(attrs[i].attrVal);
			break;
		}
	}

	attr->setFieldLength(def->fieldLength());
	br->appendAttribute(attr);

	attr->close();

}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdMLeaderApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMLeaderApp, ECDMyGroup, EcdPLBJ, EcdPLBJ, ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdMLeaderApp, ECDMyGroup, ECDAttrBlk, ECDAttrBlk, ACRX_CMD_MODAL, NULL)
