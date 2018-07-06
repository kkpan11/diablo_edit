
// Diablo Edit2.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Diablo Edit2.h"
#include "MainFrm.h"

#include "Diablo Edit2Doc.h"
#include "Diablo Edit2View.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiabloEdit2App

BEGIN_MESSAGE_MAP(CDiabloEdit2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDiabloEdit2App::OnAppAbout)
	// 基于文件的标准文档命令
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CDiabloEdit2App 构造

CDiabloEdit2App::CDiabloEdit2App() noexcept
	: m_nLangIndex(0)
	, m_nItemNameBase(0)
	, m_nPropertyName(0)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DiabloEdit2.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CDiabloEdit2App 对象

CDiabloEdit2App theApp;

static PTCHAR ExtractFilePath(PTCHAR pszFilePathName){
	PTCHAR cur = pszFilePathName,last = 0;
	for(;*cur != 0;++cur)
		if(*cur == _T('\\') || *cur == _T('/') || *cur == _T(':'))
			last = cur;
	if(last)
		*++last = 0;
	return pszFilePathName;
}

// CDiabloEdit2App 初始化

BOOL CDiabloEdit2App::InitInstance()
{
	if(!ReadLangRes()){
		::MessageBox(0,_T("读取语言数据出错"),_T("错误"),MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if(!ReadItemRes()){
		::MessageBox(0,_T("读取物品数据出错"),_T("错误"),MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if(!ReadPropRes()){
		::MessageBox(0,_T("读取属性数据出错"),_T("错误"),MB_OK | MB_ICONERROR);
		return FALSE;
	}
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDiabloEdit2Doc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CDiabloEdit2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生

	//得到程序的运行目录路径
	TCHAR path[1024];
	::GetModuleFileName(0,path,1024);
	m_sAppPath = ExtractFilePath(path);

	return TRUE;
}

template<typename T,class Array>
static inline T & Interpret(Array & p,int i){
	return *reinterpret_cast<T *>(&p[i]);
}

BOOL CDiabloEdit2App::ReadLangRes(void)
{
	try{
		//从文件中读取数据
		CFile inf(_T("language.dat"),CFile::modeRead);
		std::vector<BYTE> data(size_t(inf.GetLength()));
		inf.Read(&data[0],UINT(data.size()));
		if(Interpret<WORD>(data,0) != 0x11DE ||
			Interpret<DWORD>(data,2) != 0x474E414C ||
			Interpret<DWORD>(data,10) != data.size())
			throw 0;
		DWORD dw1 = Interpret<DWORD>(data,14);	//CRC
		//用CRC为数据解码
		const int STEP = 3;
		for(BYTE *cur = &data[0] + 18, *end = &data[0] + data.size() - 4;cur < end;cur += STEP)
			*reinterpret_cast<DWORD *>(cur) ^= dw1;
		//验证CRC
		Interpret<DWORD>(data,14) = 0;
		const int MASK = (1 << STEP) - 1;
		const int SHIFT = (sizeof(DWORD) << 3) - STEP;
		DWORD dw2 = 0;
		for(int i = int(data.size()) - 1;i >= 0;--i){
			DWORD add = (dw2 & MASK) << SHIFT;
			dw2 = (dw2 << STEP) + data[i];
		}
		if(dw1 != dw2)
			throw 0;
		//得到语言数和消息数
		dw1 = Interpret<DWORD>(data,6);
		dw2 = dw1 & 0xFFFFFF;	//msg number
		dw1 >>= 24;				//lang number
		//得到语言和消息
		if(dw1){
			char buf[0xFFFF];
			m_saLanguage.resize(dw1,std::vector<CString>(dw2));		//index 0是语言名称
			for(DWORD i = 0,pos = 18;i < dw1;++i){
				::ZeroMemory(buf,sizeof(buf));
				DWORD len = Interpret<BYTE>(data,pos++);
				::CopyMemory(buf,&data[pos],len);
				m_saLanguage[i][0] = buf;
				pos += len;
				for(DWORD j = 1;j < dw2;++j){
					::ZeroMemory(buf,sizeof(buf));
					DWORD len = Interpret<WORD>(data,pos);
					pos += 2;
					if(len){
						::CopyMemory(buf,&data[pos],len);
						m_saLanguage[i][j] = buf;
						pos += len;
					}else{
						if(!m_nItemNameBase)
							m_nItemNameBase = j + 1;
						else if(!m_nPropertyName)
							m_nPropertyName = j + 1;
                        else if(!m_nMonsterNameBase)
                            m_nMonsterNameBase = j + 1;
                        else if(!m_nMagicPrefixBase)
                            m_nMagicPrefixBase = j + 1;
                        else if(!m_nMagicSuffixBase)
                            m_nMagicSuffixBase = j + 1;
                        else if(!m_nRareCraftedNameBase)
                            m_nRareCraftedNameBase = j + 1;
                        else if(!m_nUniqueNameBase)
                            m_nUniqueNameBase = j + 1;
					}
				}
			}
		}
	}catch(...){
		return FALSE;
	}
	m_nLangCount = INT(m_saLanguage.size());
	return TRUE;
}

BOOL CDiabloEdit2App::ReadItemRes(void)
{
	try{
		//从文件中读取数据
		CFile inf(_T("itemdata.dat"),CFile::modeRead);
		std::vector<BYTE> data(size_t(inf.GetLength()));
		inf.Read(&data[0],UINT(data.size()));
		if(Interpret<WORD>(data,0) != 0x11DE ||
			Interpret<DWORD>(data,2) != 0x4D455449 ||
			Interpret<DWORD>(data,6) != data.size())
			throw 0;
		DWORD dw1 = Interpret<DWORD>(data,10);	//CRC
		//用CRC为数据解码
		const int STEP = 5;
		for(BYTE *cur = &data[0] + 14, *end = &data[0] + data.size() - 4;cur < end;cur += STEP)
			*reinterpret_cast<DWORD *>(cur) ^= dw1;
		//验证CRC
		Interpret<DWORD>(data,10) = 0;
		const int MASK = (1 << STEP) - 1;
		const int SHIFT = (sizeof(DWORD) << 3) - STEP;
		DWORD dw2 = 0;
		for(int i = int(data.size()) - 1;i >= 0;--i){
			DWORD add = (dw2 & MASK) << SHIFT;
			dw2 = (dw2 << STEP) + data[i];
		}
		if(dw1 != dw2)
			throw 0;
		//得到物品信息数据
		m_mapItemData.clear();
		for(size_t off = 16;off < data.size();off += CItemDataStruct::SIZE){
			const CItemDataStruct & is = *reinterpret_cast<CItemDataStruct *>(&data[off]);
			m_mapItemData[is.TypeID()] = is;
		}
	}catch(...){
		return FALSE;
	}
	return TRUE;
}

int CDiabloEdit2App::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

BOOL CDiabloEdit2App::ReadPropRes()
{
	try{
		//从文件中读取数据
		CFile inf(_T("property.dat"),CFile::modeRead);
		std::vector<BYTE> data(size_t(inf.GetLength()));
		inf.Read(&data[0],UINT(data.size()));
		if(Interpret<WORD>(data,0) != 0x11DE ||
			Interpret<DWORD>(data,2) != 0x504F5250 ||
			Interpret<DWORD>(data,6) != data.size())
			throw 0;
		DWORD dw1 = Interpret<DWORD>(data,10);	//CRC
		//用CRC为数据解码
		const int STEP = 5;
		for(BYTE *cur = &data[0] + 14, *end = &data[0] + data.size() - 4;cur < end;cur += STEP)
			*reinterpret_cast<DWORD *>(cur) ^= dw1;
		//验证CRC
		Interpret<DWORD>(data,10) = 0;
		const int MASK = (1 << STEP) - 1;
		const int SHIFT = (sizeof(DWORD) << 3) - STEP;
		DWORD dw2 = 0;
		for(int i = int(data.size()) - 1;i >= 0;--i){
			DWORD add = (dw2 & MASK) << SHIFT;
			dw2 = (dw2 << STEP) + data[i];
		}
		if(dw1 != dw2)
			throw 0;
		//得到物品信息数据
        dw1 = *(WORD *)&data[14]; //max ID
        m_vProperty.resize(dw1 + 1);
        memset(&m_vProperty[0],0,dw1 * sizeof(CProperty));
        for(dw1 = 16;dw1 < data.size();++dw1){
            dw2 = *(WORD *)&data[dw1];
            m_vProperty[dw2].bits = data[dw1 += 2];
            m_vProperty[dw2].bias = data[++dw1];
        }
	}catch(...){
		return FALSE;
	}
	return TRUE;
}

#define VALUE_1(a)                  int(value & ((DWORD(1) << a) - 1))
#define VALUE_2(a,b)                int((value >> a) & ((DWORD(1) << b) - 1))
#define VALUE_3(a,b,c)              int((value >> (a + b)) & ((DWORD(1) << c) - 1))
#define VALUE_4(a,b,c,d)            int((value >> (a + b + c)) & ((DWORD(1) << d) - 1))
#define FORMAT_RET_1(v1)            ret.Format(::theApp.PropertyName(id),(v1))
#define FORMAT_RET_2(v1,v2)         ret.Format(::theApp.PropertyName(id),(v1),(v2))
#define FORMAT_RET_3(v1,v2,v3)      ret.Format(::theApp.PropertyName(id),(v1),(v2),(v3))
#define FORMAT_RET_4(v1,v2,v3,v4)   ret.Format(::theApp.PropertyName(id),(v1),(v2),(v3),(v4))

CString CDiabloEdit2App::PorpertyDescription(WORD id,DWORD value) const
{
    value -= ::theApp.PropertyData(id).bias;
    CString ret;
    switch(id){
        case 0:case 1:case 2:case 7:case 8:case 9:
        case 11:case 16:case 18:case 19:
        case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:
        case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:
        case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 49:
        case 51:case 53:case 55:case 56:case 58:case 59:
        case 60:case 62:case 67:case 68:
        case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:
        case 80:case 81:case 82:case 85:case 86:case 87:case 88:case 89:
        case 90:case 91:case 92:case 93:case 94:case 96:case 99:
        case 102:case 105:case 108:case 109:
        case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:
        case 121:case 122:case 123:case 124:case 127:case 128:
        case 134:case 135:case 136:case 137:case 138:case 139:
        case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:
        case 150:case 154:case 156:case 157:case 158:case 159:
        case 160:
        case 181:
        case 254:
        case 324:case 329:case 330:case 331:case 332:case 337:case 338:case 339:
        case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:
        case 356:case 357:case 358:        
            FORMAT_RET_1(int(value));
            break;
        case 17:{
            FORMAT_RET_2(VALUE_1(9),VALUE_2(9,9));
            break;}
        case 48:{
            FORMAT_RET_2(VALUE_1(8),VALUE_2(8,9));
            break;}
        case 50:{
            FORMAT_RET_2(VALUE_1(6),VALUE_2(6,10));
            break;}
        case 52:{
            FORMAT_RET_2(VALUE_1(8),VALUE_2(8,9));
            break;}
        case 54:
            FORMAT_RET_3(VALUE_1(8),VALUE_2(8,9),VALUE_3(8,9,8) / 25);
            break;
        case 57:{
            int len = VALUE_3(10,10,9);
            FORMAT_RET_3((VALUE_1(10) * len) >> 8,(VALUE_2(10,10) * len) >> 8,len / 25);
            break;}
        case 83:{
            FORMAT_RET_2(VALUE_2(3,3),ClassName(VALUE_1(3)));
            break;}
        case 97:{
            FORMAT_RET_2(VALUE_2(9,6),SkillName(VALUE_1(9)));
            break;}
        case 98:{
            int index = VALUE_1(8);
            FORMAT_RET_1(index >= 1 && index <= 5 ? ::theApp.String(403 + index) : ::theApp.String(409));
            break;}
        case 107:{
            UINT class_id;
            CString skill_name = SkillName(VALUE_1(9),&class_id);
            FORMAT_RET_3(VALUE_2(9,3),skill_name,ClassName(class_id));
            break;}
        case 126:{
            FORMAT_RET_2(VALUE_2(3,3),VALUE_1(3));
            break;}
        case 151:{
            FORMAT_RET_2(VALUE_2(9,5),SkillName(VALUE_1(9)));
            break;}
        case 155:{
            FORMAT_RET_2(VALUE_2(10,7),MonsterName(VALUE_1(10)));
            break;}
        case 179:case 180:{
            FORMAT_RET_2(VALUE_2(10,9),MonsterName(VALUE_1(10)));
            break;}
        case 188:{
            FORMAT_RET_3(VALUE_3(3,13,3),SkillTabName(VALUE_1(3),VALUE_2(3,13)),ClassName(VALUE_2(3,13)));
            break;}
        case 195:case 196:case 197:case 198:case 199:case 201:{
            FORMAT_RET_3(VALUE_3(6,10,7),VALUE_1(6),SkillName(VALUE_2(6,10)));
            break;}
        case 204:{
            FORMAT_RET_4(VALUE_1(6),SkillName(VALUE_2(6,10)),VALUE_3(6,10,8),VALUE_4(6,10,8,8));
            break;}
        case 214:case 215:case 216:case 217:case 218:case 219:
        case 220:case 221:case 222:case 223:case 226:case 227:case 228:case 229:
        case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:
        case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:
        case 250:{
            FORMAT_RET_1(int(value) >> 3);
            break;}
        case 224:case 225:{
            FORMAT_RET_1(int(value) >> 1);
            break;}
        case 252:case 253:{
            FORMAT_RET_1(100 / int(value));
            break;}
        case 268:case 269:
        case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:
        case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:
        case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:
        case 300:case 301:case 302:case 303:{
            FORMAT_RET_3(VALUE_2(2,10),VALUE_3(2,10,10),TimeName(VALUE_1(2)));
            break;}
        case 305:case 306:case 307:case 308:
        case 333:case 334:case 335:case 336:{
            FORMAT_RET_1(-int(value));
            break;}
        default:    //no param
            ret = id <= 358 ? ::theApp.PropertyName(id) : ::theApp.String(15);
    }
    return ret;
}

#undef FORMAT_RET_1
#undef FORMAT_RET_2
#undef FORMAT_RET_3
#undef FORMAT_RET_4
#undef VALUE_1
#undef VALUE_2
#undef VALUE_3
#undef VALUE_4

CString CDiabloEdit2App::SkillName(UINT skill_id,UINT * class_id) const
{
    const int SKILL_OFFSET[] = {134,168,202,236,270,304,338,0};  //各角色首技能名字在Language中的总偏移
    const int SKILL_INDEX1[] = {
        385,394,395,396,397,398,    //0-5
        400,401,402,403             //217-220
    };
    const int SKILL_INDEX2[] = {
        1,2,11,12,21,3,4,13,22,23,5,14,15,24,25,6,7,16,26,27,8,9,17,18,28,10,19,20,29,30,   //Ama,6-35
        1,2,11,21,22,3,12,13,23,24,4,5,14,15,25,6,7,16,17,26,8,18,19,27,28,9,10,20,29,30,   //Sor,36-65
        1,11,12,21,22,2,3,13,14,23,4,5,15,24,25,6,7,16,17,26,8,9,18,27,28,10,19,20,29,30,   //Nec,66-95
        1,2,11,21,22,3,12,13,23,24,4,5,14,25,26,6,7,15,16,27,8,9,17,18,28,10,19,20,29,30,   //Pal,96-125
        1,11,12,13,21,22,2,3,14,15,16,23,24,4,5,17,25,6,7,18,26,8,19,27,28,9,10,20,29,30,   //Bar,126-155
    };
    const int SKILL_INDEX3[] = {
        1,2,11,12,21,3,4,13,22,23,5,14,15,24,25,6,7,16,17,26,8,18,19,27,28,9,10,20,29,30,   //Dru,221-250
        1,11,12,21,22,2,3,13,23,24,4,5,14,15,25,6,16,17,26,27,7,8,18,28,29,9,10,19,20,30    //Ass,251-280
    };
    int index = 399; //"An Evil Force"
    int cid = 7;
    if(skill_id <=5)
        index = SKILL_INDEX1[skill_id];
    else if(skill_id >= 217 && skill_id <= 220)
        index = SKILL_INDEX1[skill_id - 211];
    else if(skill_id <= 155){
        cid = (skill_id - 6) / 30;
        index = SKILL_INDEX2[skill_id - 6] + SKILL_OFFSET[cid];
    }else if(skill_id >= 221 && skill_id <= 280){
        cid = (skill_id - 221) / 30 + 5;
        index = SKILL_INDEX3[skill_id - 221] + SKILL_OFFSET[cid];
    }
    if(class_id)
        *class_id = cid;
    return ::theApp.String(index);
}

// CDiabloEdit2App 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDiabloEdit2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDiabloEdit2App 消息处理程序



