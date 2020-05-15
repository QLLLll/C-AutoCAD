#include "StdAfx.h"
#include "resource.h"
//添加头文件和库CAD2010及其以上新增
#if _MSC_VER == 1500
#include "acplmisc.h"
#pragma comment(lib,"AcPublish.lib")
#else if _MSC_VER > 1500
#include "acplmisc.h"
#pragma comment(lib,"AcPublish_crx.lib")
#endif
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CPublishPrinterApp : public AcRxArxApp {

public:
	CPublishPrinterApp () : AcRxArxApp () {}

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
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CPublishPrinterApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CPublishPrinterApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupPT () {
		// Put your command code here
		/*AcDbDatabase *pDb = new AcDbDatabase();
		pDb->readDwgFile(_T("D:\\TestPublish.dwg"));
		plot(pDb);
		if (pDb != NULL)
		{
			delete pDb;
			pDb = NULL;
		}*/
		plot(acdbHostApplicationServices()->workingDatabase());
		
	}

	
	static void plot(AcDbDatabase *pDb)
	{
		//设置pdf保存路径
		CString pdfPath = _T("d:\\myPDF.pdf");
		//设置打印机设备名
		CString deviceName = _T("DWG To PDF.pc3");
		Acad::ErrorStatus es = Acad::eOk;

		const TCHAR *docName;
		//获取数据的路径和文件名,注意pdb如果是当前文件，
		//一定要保存后再打印，否者这个路径可能是临时文件
		//pDb即使的后台数据库依然能打印
		es = pDb->getFilename(docName);

		//获取ACAD应用程序布局管理器
		AcDbLayoutManager *layoutManager = acdbHostApplicationServices()->layoutManager();
		AcDbDictionary *layoutDict = NULL;
		//获取数据库的布局词典
		es = pDb->getLayoutDictionary(layoutDict, AcDb::kForRead);

		//开始收集DSD数据的信息
		// Start collecting info for the DSD data
		AcPlDSDEntries dsdEntries;

		//迭代器遍历布局词典
		AcDbDictionaryIterator *layoutIterator = layoutDict->newIterator();

		//如果我们只想打印图纸空间而不是模型布局,获取pDb模型空间的ID
		// If we only want to plot paper spaces and not the model layout
		AcDbObjectId msId = acdbSymUtil()->blockModelSpaceId(pDb);

		for (; layoutIterator && !layoutIterator->done(); layoutIterator->next())
		{
			AcDbObjectPointer<AcDbLayout> layout(layoutIterator->objectId(), AcDb::kForRead);
			//跳过模型空间
			if (layout->getBlockTableRecordId() == msId)
				continue;

			//!!!注意，布局的打印设置要提前设置好，如纸张，打印线型，等
			//布局使用的是保存的布局的页面设置，也可以设置窗口打印和区域等
			//此处是读模式，是不能修改layout设置
			const ACHAR *layoutName;
			layout->getLayoutName(layoutName);
			//测试，修改输出的标题名加前缀，pdf单页模式会作为文件名
			CString strPre = _T("sk_");
			strPre += layoutName;
			const ACHAR *szOutLayoutName = (LPCTSTR)strPre;
			//设置DSD条目
			AcPlDSDEntry dsdEntry;

			dsdEntry.setLayout(layoutName);
			dsdEntry.setDwgName(docName);
			//测试 此处修改dsdEntry.setTitle(layoutName);
			//条目标题，单页pdf会使用这个名字来保存文件名
			//并且条目的标题如果相同，原有的条目可能会被覆盖
			dsdEntry.setTitle(szOutLayoutName);
			//添加条目到dsd条目集合
			dsdEntries.append(dsdEntry);
		}
		//删除(释放)布局遍历迭代器
		delete layoutIterator;
		layoutDict->close();

		//将dsd条目添加到dsd数据
		AcPlDSDData dsdData;
		dsdData.setDSDEntries(dsdEntries);

		//设置dsd数据项目路径
		dsdData.setProjectPath(_T("d:\\"));
		//设置dsd数据的日志路径，pdf打印机可能无效，没有生成日志
		dsdData.setLogFilePath(_T("d:\\logdwf.log"));

		//设置单页还是多页，如果是单页，将会使用布局名来保存文件名
		//kSinglePDF kMultiPDF
		dsdData.setSheetType(AcPlDSDEntry::kMultiPDF);
		//设置打印的份数,这个pdf打印机不能设置,始终是1
		dsdData.setNoOfCopies(1);
		//设置目标名,保存的文件名(单页无效)
		dsdData.setDestinationName(pdfPath);
		//设置提示输入dwf名，如果为true可能会提示输入文件名
		dsdData.setPromptForDwfName(false);
		//设置图纸集的名字,这个会显示在打印和发布详细信息中
		dsdData.setSheetSetName(_T("PublisherSet"));

		//设置当前配置和打印设备名
		AcPlPlotConfig *plotConfig;
		acplPlotConfigManager->setCurrentConfig(plotConfig, deviceName);


		//后面的acplPublishExecute需要加载AcPublish.arx
		// We need this for acplPublishExecute
		acedArxLoad(_T("AcPublish.arx"));

		//可能需要#include "acplmisc.h"
		//可能需要2010-2012#pragma comment(lib,"AcPublish.lib")
		//可能需要2013及其以上#pragma comment(lib,"AcPublish_crx.lib")
		//执行发布程序
		acplPublishExecute(dsdData, plotConfig, false);
	}
	

	
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CPublishPrinterApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CPublishPrinterApp, ECDMyGroup, PT, PT, ACRX_CMD_MODAL, NULL)


