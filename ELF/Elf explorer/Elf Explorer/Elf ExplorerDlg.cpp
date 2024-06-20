
// Elf ExplorerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Elf Explorer.h"
#include "Elf ExplorerDlg.h"
#include "afxdialogex.h"
#include <string>
#include "common/common.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CElfExplorerDlg 对话框



CElfExplorerDlg::CElfExplorerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ELF_EXPLORER_DIALOG, pParent)
	, m_StrTips(_T(""))
	, m_ElfIdent(_T(""))
	, m_ElfType(_T(""))
	, m_ElfMachine(_T(""))
	, m_ElfCpuFlag(_T(""))
	, m_ElfPHDROff(_T(""))
	, m_ELFSHDROff(_T(""))
	, M_ElfEntryPoint(_T(""))
	, m_ElfEntryFunc(_T(""))
	, m_ElfHDRSize(_T(""))
	, m_ElfPHDRSize(_T(""))
	, m_ElfPHDRNum(_T(""))
	, m_ElfVersion(_T(""))
	, m_ElfSHDRSize(_T(""))
	, m_ElfSHDRNum(_T(""))
	, m_ElfSNameIdx(_T(""))
	, m_ElfABI(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CElfExplorerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEINFO, m_TreeCtl);
	DDX_Control(pDX, IDC_LISTINFO, m_ListCtl);
	DDX_Control(pDX, IDC_TIPS, m_TipCtl);
	DDX_Text(pDX, IDC_TIPS, m_StrTips);
	DDX_Text(pDX, IDC_IDENT, m_ElfIdent);
	DDX_Text(pDX, IDC_TYPE, m_ElfType);
	DDX_Text(pDX, IDC_MACHINE, m_ElfMachine);
	DDX_Text(pDX, IDC_CPUFLAG, m_ElfCpuFlag);
	DDX_Text(pDX, IDC_POFF, m_ElfPHDROff);
	DDX_Text(pDX, IDC_SOFF, m_ELFSHDROff);
	DDX_Text(pDX, IDC_ENTRYPOINT, M_ElfEntryPoint);
	DDX_Text(pDX, IDC_ENTRYFUNCNAME, m_ElfEntryFunc);
	DDX_Text(pDX, IDC_HEADSIZE, m_ElfHDRSize);
	DDX_Text(pDX, IDC_PROGRAMSIZE, m_ElfPHDRSize);
	DDX_Text(pDX, IDC_PROGRAMNUM, m_ElfPHDRNum);
	DDX_Text(pDX, IDC_VERSION, m_ElfVersion);
	DDX_Text(pDX, IDC_SECTIONSIZE, m_ElfSHDRSize);
	DDX_Text(pDX, IDC_SECTIONNUM, m_ElfSHDRNum);
	DDX_Text(pDX, IDC_SECSTRINDEX, m_ElfSNameIdx);
	DDX_Text(pDX, IDC_OSABI, m_ElfABI);
}

BEGIN_MESSAGE_MAP(CElfExplorerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_IDENT, &CElfExplorerDlg::OnEnChangeIdent)
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_TREEINFO, &CElfExplorerDlg::OnNMClickTreeinfo)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CElfExplorerDlg 消息处理程序

BOOL CElfExplorerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CElfExplorerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CElfExplorerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CElfExplorerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CElfExplorerDlg::OnEnChangeIdent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


BOOL CElfExplorerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//屏蔽回车键
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CElfExplorerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, _MAX_PATH);
	if (nCount != 1)
	{
		UpdateTips("ERR  拖曳文件大于1!");
		//m_TipCtl.SetWindowText("ERR  拖曳文件大于1!";
		return;
	}
	finitRes();
	char bufTmp[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, bufTmp, _MAX_PATH);
	DragFinish(hDropInfo);
	m_FilePath = bufTmp;
	m_FilePath.Insert(0,"当前加载路径：");
	m_TipCtl.SetWindowText("");
	m_TipCtl.SetWindowText(m_FilePath);
	m_FilePath = bufTmp;
	LoadFileMap();
	CDialogEx::OnDropFiles(hDropInfo);
}

VOID CElfExplorerDlg::InitListControl(int number, LISTITEM* Item)
{


	return;
}

VOID CElfExplorerDlg::LoadFileMap()
{
	if (m_FilePath.IsEmpty())
	{
		UpdateTips("文件句柄为空！");
		return;
	}
	//加载文件映射
	HANDLE hFile = CreateFile(m_FilePath, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		UpdateTips("打开文件失败");
		return;
	}

	//char bufTmp[0x1024] = {0};
	//DWORD dwRead;
	//int n = ReadFile(hFile, bufTmp, 0x1024, &dwRead, NULL);

	//创建文件映射内核对象，句柄保存hFileMapping

	m_hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0x4000000, NULL);
	if (m_hFileMapping == INVALID_HANDLE_VALUE)
	{
		UpdateTips("文件映射失败");
		return;
	}
	// 设定大小、偏移量等参数
	__int64 qwFileSize = 0x4000000;
	__int64 qwFileOffset = 0;
	DWORD dwBytesInBlock = 0;
	// 将文件数据映射到进程的地址空间
	m_ElfBase = (PBYTE)MapViewOfFile(m_hFileMapping,FILE_MAP_ALL_ACCESS,0,0, dwBytesInBlock);
	if (m_ElfBase == nullptr)
	{
		UpdateTips("内存映射分配的地址是空指针！");
	}
	CloseHandle(hFile);
	//开始解析elf文件
	FormatELF(m_ElfBase);
	return;
}

VOID CElfExplorerDlg::UpdateTips(const char* Tips)
{
	m_StrTips = Tips;
	UpdateData(FALSE);
	return;
}

VOID CElfExplorerDlg::FormatELF(PBYTE ElfBase)
{

	//解析elf文件头32位程序和64位程序分开解析
	int is64Bit = 0;

	Elf32_Ehdr* EhdrTmp = (Elf32_Ehdr*)ElfBase;
	char* pCheckMagic = (char*)EhdrTmp;
	if (pCheckMagic[EI_MAG0] != ELFMAG0 &&
		pCheckMagic[EI_MAG1] != ELFMAG1 &&
		pCheckMagic[EI_MAG2] != ELFMAG2 &&
		pCheckMagic[EI_MAG3] != ELFMAG3)
	{
		UpdateTips("仅支持解析ELF文件！");
		return;
	}
	m_SectionTabOff = new SECTIONTAB();
	if (EhdrTmp->e_ident[4] == 0 )
	{
		is64Bit = 1;
	}
	else if (EhdrTmp->e_ident[4] == 1 )
	{
		is64Bit = 1;
	}
	else if (EhdrTmp->e_ident[4] == 2)
	{
		is64Bit = 2;
	}
	else if (EhdrTmp->e_ident[4] == 3)
	{
		is64Bit = 3;
	}

	if (is64Bit ==1 )
	{
		FormatELF32(ElfBase);
	}
	else if (is64Bit == 2)
	{
		FormatELF64(ElfBase);
	}
	else
	{
		UpdateTips("不支持解析非32bit或非64bit文件！");
	}

	
	 
	return;
}

VOID CElfExplorerDlg::FormatELF32(PBYTE ElfBase)
{
	//解析文件头
	Elf32_Ehdr* Elf32Ehdr = (Elf32_Ehdr*)ElfBase;
	unsigned char* e_ident = Elf32Ehdr->e_ident;
	Elf32_Half e_type = Elf32Ehdr->e_type;
	Elf32_Half e_machine = Elf32Ehdr->e_machine;
	Elf32_Word e_version = Elf32Ehdr->e_version;
	Elf32_Addr e_entry = Elf32Ehdr->e_entry;
	Elf32_Off e_phoff = Elf32Ehdr->e_phoff;
	Elf32_Off e_shoff = Elf32Ehdr->e_shoff;
	Elf32_Word e_flags = Elf32Ehdr->e_flags;
	Elf32_Half e_ehsize = Elf32Ehdr->e_ehsize;
	Elf32_Half e_phentsize = Elf32Ehdr->e_phentsize;
	Elf32_Half e_phnum = Elf32Ehdr->e_phnum;
	Elf32_Half e_shentsize = Elf32Ehdr->e_shentsize;
	Elf32_Half e_shnum = Elf32Ehdr->e_shnum;
	Elf32_Half e_shstrndx = Elf32Ehdr->e_shstrndx;
	//解析文件头内容到控件
	FormatELF32EHDR(Elf32Ehdr);     //√
	FormatELF32PHDR(Elf32Ehdr);		//√
	FormatELF32SHDR(Elf32Ehdr);


	return ;
}

VOID CElfExplorerDlg::FormatELF64(PBYTE ElfBase)
{
	return;
}

VOID CElfExplorerDlg::FormatELF32EHDR(Elf32_Ehdr* ehdr)
{
	Elf32_Ehdr* Elf32Ehdr = ehdr;
	unsigned char* e_ident = Elf32Ehdr->e_ident;
	Elf32_Half e_type = Elf32Ehdr->e_type;
	Elf32_Half e_machine = Elf32Ehdr->e_machine;
	Elf32_Word e_version = Elf32Ehdr->e_version;
	Elf32_Addr e_entry = Elf32Ehdr->e_entry;
	Elf32_Off e_phoff = Elf32Ehdr->e_phoff;
	Elf32_Off e_shoff = Elf32Ehdr->e_shoff;
	Elf32_Word e_flags = Elf32Ehdr->e_flags;
	Elf32_Half e_ehsize = Elf32Ehdr->e_ehsize;
	Elf32_Half e_phentsize = Elf32Ehdr->e_phentsize;
	Elf32_Half e_phnum = Elf32Ehdr->e_phnum;
	Elf32_Half e_shentsize = Elf32Ehdr->e_shentsize;
	Elf32_Half e_shnum = Elf32Ehdr->e_shnum;
	Elf32_Half e_shstrndx = Elf32Ehdr->e_shstrndx;
	//更新控件值ident
	m_ElfIdent.Format("\\%o", e_ident[0]);
	string strElfmagic = (char*)e_ident;
	string strElfmagic2 = strElfmagic.substr(1, 3);
	m_ElfIdent += strElfmagic2.c_str();
	//更新控件值OS/ABI//m_ElfABI
	m_ElfABI.Format("%s", ei_osabiSz[e_ident[EI_OSABI]]);
	//更新控件值e_type
	m_ElfType.Format("%s", e_typeSz[e_type]);
	//更新控件值e_machine
	m_ElfMachine.Format("%s", e_machineSz[e_machine]);
	//更新控件值e_version
	m_ElfVersion.Format("%s", e_versionSz[e_version]);
	//更新控件值e_entry
	M_ElfEntryPoint.Format("0x%x", e_entry);
	//更新控件值e_phoff
	m_ElfPHDROff.Format("0x%x", e_phoff);
	//更新控件值e_shoff
	m_ELFSHDROff.Format("0x%x", e_shoff);
	//更新控件值e_flags
	m_ElfCpuFlag.Format("0x%x", e_flags);
	//更新控件值e_ehsize
	m_ElfHDRSize.Format("%d", e_ehsize);
	//更新控件值e_phentsize
	m_ElfPHDRSize.Format("0x%x", e_phentsize);
	//更新控件值e_phnum
	m_ElfPHDRNum.Format("%d", e_phnum);
	//更新控件值e_shentsize
	m_ElfSHDRSize.Format("0x%x", e_shentsize);
	//更新控件值e_shnum
	m_ElfSHDRNum.Format("%d", e_shnum);
	//更新控件值e_shstrndx
	m_ElfSNameIdx.Format("%d", e_shstrndx);
	UpdateData(FALSE);
	return VOID();
}

VOID CElfExplorerDlg::FormatELF32PHDR(Elf32_Ehdr* ehdr)
{

	Elf32_Addr base = (Elf32_Addr)ehdr;

	Elf32_Off e_phoff = ehdr->e_phoff;
	Elf32_Half e_phentsize = ehdr->e_phentsize;
	Elf32_Half e_phnum = ehdr->e_phnum;
	Elf32_Half e_shstrndx = ehdr->e_shstrndx;

	Elf32_Phdr* phdr = (Elf32_Phdr*)(base + e_phoff);
	Elf32_Phdr* phdrTmp = phdr;
	for (size_t  i = 0; i < e_phnum;  i++, phdrTmp++)
	{
		CString strType = GetType2CString(GetDefProgramType,phdrTmp->p_type);
		HTREEITEM hRoot =  m_TreeCtl.InsertItem(strType);
		ITEMDATA* pdata = new ITEMDATA();
		pdata->dwStructType = ProgramHeader32;
		pdata->pData = (DWORD_PTR)phdrTmp;
		m_TreeCtl.SetItemData(hRoot, (DWORD_PTR)pdata);
	}

	return VOID();
}

VOID CElfExplorerDlg::FormatELF32SHDR(Elf32_Ehdr* ehdr)
{
	//解析节头

	Elf32_Addr base = (Elf32_Addr)ehdr;


	Elf32_Off e_shoff = ehdr->e_shoff;
	Elf32_Half e_shentsize = ehdr->e_shentsize;
	Elf32_Half e_shnum = ehdr->e_shnum;
	Elf32_Half e_shstrndx = ehdr->e_shstrndx;

	Elf32_Shdr* shdr = (Elf32_Shdr*)(base + e_shoff);
	Elf32_Shdr* shdrTmp = shdr;
	Elf32_Addr SecNameTabAddr = base + shdrTmp[e_shstrndx].sh_offset;
	m_SectionTabOff->base = (char*)base;
	//找到节名称表
	for (size_t i = 0; i < e_shnum; i++, shdrTmp++)
	{
		CString strType = (char*)(SecNameTabAddr + shdrTmp->sh_name);
		HTREEITEM hRoot = m_TreeCtl.InsertItem(strType);
		ITEMDATA* pdata = new ITEMDATA();
		pdata->dwStructType = SectionHeader32;
		pdata->pData = (DWORD_PTR)shdrTmp;
		m_TreeCtl.SetItemData(hRoot, (DWORD_PTR)pdata);


		if (memcmp(strType.GetString(), ".strtab", sizeof(".strtab")) == 0)
		{
			m_SectionTabOff->dwStrTabOff = (DWORD)shdrTmp;
			HTREEITEM hChild = NULL;
			ITEMDATA* pDataChild = new ITEMDATA();
			pDataChild->dwStructType = SectionHeader32;
			pDataChild->dwSectionType = SET_STRTAB;
			hChild = m_TreeCtl.InsertItem("字符串表", hRoot);
			m_TreeCtl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtl.SetItemData(hChild, (DWORD_PTR)pDataChild);
		}
		else if (memcmp(strType.GetString(), ".dynsym", sizeof(".dynsym")) == 0) {
			m_SectionTabOff->dwDynsymOff = (DWORD)shdrTmp;

			HTREEITEM hChild = NULL;
			ITEMDATA* pDataChild = new ITEMDATA();
			pDataChild->dwStructType = SectionHeader32;
			pDataChild->dwSectionType = SET_DYNSYM;
			hChild = m_TreeCtl.InsertItem("动态符号表", hRoot);
			m_TreeCtl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtl.SetItemData(hChild, (DWORD_PTR)pDataChild);
			

		}
		else if (memcmp(strType.GetString(), ".symtab", sizeof(".symtab")) == 0) {
			m_SectionTabOff->dwSymTabOff = (DWORD)shdrTmp;

			HTREEITEM hChild = NULL;
			ITEMDATA* pDataChild = new ITEMDATA();
			pDataChild->dwStructType = SectionHeader32;
			pDataChild->dwSectionType = SET_SYMTAB;
			hChild = m_TreeCtl.InsertItem("符号表", hRoot);
			m_TreeCtl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtl.SetItemData(hChild, (DWORD_PTR)pDataChild);

		}
		else if (memcmp(strType.GetString(), ".hash", sizeof(".hash")) == 0) {

			m_SectionTabOff->dwHashTabOff = (DWORD)shdrTmp;
		}
		else if (memcmp(strType.GetString(), ".gnu.hash", sizeof(".gnu.hash")) == 0) {

			m_SectionTabOff->dwGnuHashTabOff = (DWORD)shdrTmp;

		}
		else if (memcmp(strType.GetString(), ".rel.dyn", sizeof(".rel.dyn")) == 0) {

			m_SectionTabOff->dwRelDynTabOff = (DWORD)shdrTmp;

			HTREEITEM hChild = NULL;
			ITEMDATA* pDataChild = new ITEMDATA();
			pDataChild->dwStructType = SectionHeader32;
			pDataChild->dwSectionType = SET_RELDYN;
			hChild = m_TreeCtl.InsertItem("重定位表（1）", hRoot);
			m_TreeCtl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtl.SetItemData(hChild, (DWORD_PTR)pDataChild);

		}
		else if (memcmp(strType.GetString(), ".rel.plt", sizeof(".rel.dyn")) == 0) {

			m_SectionTabOff->dwRelPltTabOff = (DWORD)shdrTmp;

			HTREEITEM hChild = NULL;
			ITEMDATA* pDataChild = new ITEMDATA();
			pDataChild->dwStructType = SectionHeader32;
			pDataChild->dwSectionType = SET_RELPLT;
			hChild = m_TreeCtl.InsertItem("重定位表（2）", hRoot);
			m_TreeCtl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtl.SetItemData(hChild, (DWORD_PTR)pDataChild);

		}
		else if (memcmp(strType.GetString(), ".dynstr", sizeof(".dynstr")) == 0) {

			m_SectionTabOff->dwDynStrTabOff = (DWORD)shdrTmp;
		}

	}
	return VOID();
}

VOID CElfExplorerDlg::FormatELF64EHDR(PBYTE ElfBase)
{
	return VOID();
}

VOID CElfExplorerDlg::FormatELF64PHDR(PBYTE ElfBase)
{
	return VOID();
}

VOID CElfExplorerDlg::FormatELF64SHDR(PBYTE ElfBase)
{
	return VOID();
}








CString CElfExplorerDlg::GetType2CString(int type, int formatData)
{
	CString StrTmp ="Unknown PROGRAM";
	if (type == GetDefProgramType)
	{
		switch (formatData) {
		case PT_NULL:
			return StrTmp = "NULL";
		case PT_LOAD:
			return StrTmp = "Loadable Segment";
		case PT_DYNAMIC:
			return StrTmp = "Dynamic Segment";
		case PT_INERP:
			return StrTmp = "Interpreter Path";
		case PT_NOTE:
			return StrTmp = "Note";
		case PT_SHLIB:
			return StrTmp = "PT_SHLIB";
		case PT_PHDR:
			return StrTmp = "Program Header";
		case PT_TLS:
			return StrTmp = "Thread-local Storage";
		case PT_NUM:
			return StrTmp = "Number of defined sections";
		case PT_LOOS:
			return StrTmp = "OS-specific start";
		case PT_GNU_EH_FRAME:
			return StrTmp = "GCC .eh_frame_hdr Segment";
		case PT_GNU_STACK:
			return StrTmp = "GNU Stack (executability)";
		case PT_GNU_RELRO:
			return StrTmp = "GNU Read-only After Relocation";
		case PT_SHT_ARM_EXIDX:
			return StrTmp = "Exception Index table";
		case PT_SHT_ARM_PREEMPTMAP:
			return StrTmp = "BPABI DLL dynamic linking pre-emption map";
		case PT_SHT_ARM_ATTRIBUTES:
			return StrTmp = "Object file compatibility attributes";
		case PT_SHT_ARM_DEBUGOVERLAY:
			return StrTmp = "Debug Overlay (1)";
		case PT_SHT_ARM_OVERLAYSECTION:
			return StrTmp = "Debug Overlay (2)";
		default:
			return StrTmp = "Unknown Section";
		}

	}
	else if (type == GetProgramType) {

		switch (formatData) {
		case PT_NULL:
			return StrTmp = "PT_NULL";
		case PT_LOAD:
			return StrTmp = "PT_LOAD";
		case PT_DYNAMIC:
			return StrTmp = "PT_DYNAMIC";
		case PT_INERP:
			return StrTmp = "PT_INERP";
		case PT_NOTE:
			return StrTmp = "PT_NOTE";
		case PT_SHLIB:
			return StrTmp = "PT_SHLIB";
		case PT_PHDR:
			return StrTmp = "PT_PHDR";
		case PT_TLS:
			return StrTmp = "PT_TLS";
		case PT_NUM:
			return StrTmp = "PT_NUM";
		case PT_LOOS:
			return StrTmp = "PT_LOOS";
		case PT_GNU_EH_FRAME:
			return StrTmp = "PT_GNU_EH_FRAME";
		case PT_GNU_STACK:
			return StrTmp = "PT_GNU_STACK";
		case PT_GNU_RELRO:
			return StrTmp = "PT_GNU_RELRO";
		case PT_SHT_ARM_EXIDX:
			return StrTmp = "PT_SHT_ARM_EXIDX";
		case PT_SHT_ARM_PREEMPTMAP:
			return StrTmp = "PT_SHT_ARM_PREEMPTMAP";
		case PT_SHT_ARM_ATTRIBUTES:
			return StrTmp = "PT_SHT_ARM_ATTRIBUTES";
		case PT_SHT_ARM_DEBUGOVERLAY:
			return StrTmp = "PT_SHT_ARM_DEBUGOVERLAY";
		case PT_SHT_ARM_OVERLAYSECTION:
			return StrTmp = "PT_SHT_ARM_OVERLAYSECTION";
		default:
			return StrTmp = "Unknown Type";
		}

	}
	else if (type == GetProgramMemType) {
		switch (formatData) {
		case PF_None:
			return StrTmp = "None";
		case PF_Exec:
			return StrTmp = "Exec";
		case PF_Write:
			return StrTmp = "Write";
		case PF_Write_Exec:
			return StrTmp = "Write_Exec";
		case PF_Read:
			return StrTmp = "Read";
		case PF_Read_Exec:
			return StrTmp = "Read_Exec";
		case PF_Read_Write:
			return StrTmp = "Read_Write";
		case PF_Read_Write_Exec:
			return StrTmp = "Read_Write_Exec";
		default:
			return StrTmp = "Unknown Type";
		}
	}
	else if (type == GetSectionType) {
			switch (formatData) {
			case SHT_NULL:
				return StrTmp = "SHT_NULL";
			case SHT_PROGBITS:
				return StrTmp = "SHT_PROGBITS";
			case SHT_SYMTAB:
				return StrTmp = "SHT_SYMTAB";
			case SHT_STRTAB:
				return StrTmp = "SHT_STRTAB";
			case SHT_RELA:
				return StrTmp = "SHT_RELA";
			case SHT_HASH:
				return StrTmp = "SHT_HASH";
			case SHT_GNUHASH:
				return StrTmp = "SHT_GNUHASH";
			case SHT_DYNAMIC:
				return StrTmp = "SHT_DYNAMIC";
			case SHT_NOTE:
				return StrTmp = "SHT_NOTE";
			case SHT_NOBITS:
				return StrTmp = "SHT_NOBITS";
			case SHT_REL:
				return StrTmp = "SHT_REL";
			case SHT_SHLIB:
				return StrTmp = "SHT_SHLIB";
			case SHT_DYNSYM:
				return StrTmp = "SHT_DYNSYM";
			case SHT_NUM:
				return StrTmp = "SHT_NUM";
			case SHT_LOPROC:
				return StrTmp = "SHT_LOPROC";
			case SHT_HIPROC:
				return StrTmp = "SHT_HIPROC";
			case SHT_LOUSER:
				return StrTmp = "SHT_LOUSER";
			case SHT_HIUSER:
				return StrTmp = "SHT_HIUSER";
			case SHF_RELA_LIVEPATCH:
				return StrTmp = "SHF_RELA_LIVEPATCH";
			case SHF_RO_AFTER_INIT:
				return StrTmp = "SHF_RO_AFTER_INIT";
			case SHN_LOPROC:
				return StrTmp = "SHN_LOPROC";
			case SHN_HIPROC:
				return StrTmp = "SHN_HIPROC";
			case SHN_LIVEPATCH:
				return StrTmp = "SHN_LIVEPATCH";
			case SHN_ABS:
				return StrTmp = "SHN_ABS";
			case SHN_COMMON:
				return StrTmp = "SHN_COMMON";
			case SHN_HIRESERVE:
				return StrTmp = "SHN_HIRESERVE";
			default:
				return StrTmp = "Unknown Section Type";
			}

	}
	else if (type == GetSectionMemType) {
	switch (formatData) {
	case SHF_WRITE:
		return StrTmp = "WRITE";
	case SHF_ALLOC:
		return StrTmp = "ALLOC";
	case SHF_EXECINSTR:
		return StrTmp = "EXECINSTR";
	case SHF_MASKPROC:
		return StrTmp = "MASKPROC";
	default:
		return StrTmp = "Unknown Section Mem Type";
			}

	}
	else if (type == GETSECTIONBIND) {
	switch (formatData) {
	case STB_LOCAL:
		return StrTmp = "STB_LOCAL";
	case STB_GLOBAL:
		return StrTmp = "STB_GLOBAL";
	case STB_WEAK:
		return StrTmp = "STB_WEAK";
	case STB_LOOS:
		return StrTmp = "STB_LOOS";
	case STB_HIOS:
		return StrTmp = "STB_HIOS";
	case STB_LOPROC:
		return StrTmp = "STB_LOPROC";
	case STB_HIPROC:
		return StrTmp = "STB_HIPROC";
	default:
		return StrTmp = "Unknown Section Bind";
	}

	}
	else if (type == GETSECTIONTYPE) {
	switch (formatData) {
	case STT_NOTYPE:
		return StrTmp = "STT_NOTYPE";
	case STT_OBJECT:
		return StrTmp = "STT_OBJECT";
	case STT_FUNC:
		return StrTmp = "STT_FUNC";
	case STT_SECTION:
		return StrTmp = "STT_SECTION";
	case STT_FILE:
		return StrTmp = "STT_FILE";
	case STT_COMMON:
		return StrTmp = "STT_COMMON";
	case STT_TLS:
		return StrTmp = "STT_TLS";
	case STT_LOOS:
		return StrTmp = "STT_LOOS";
	case STT_HIOS:
		return StrTmp = "STT_HIOS";
	case STT_LOPROC:
		return StrTmp = "STT_LOPROC";
	case STT_HIPROC:
		return StrTmp = "STT_HIPROC";
	default:
		return StrTmp = "Unknown Section Type";
	}

	}
	
}

VOID CElfExplorerDlg::SetListCtlPHDR32Info(Elf32_Phdr* ehdr)
{
	//清除上一次展示的数据
	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));
	m_ListCtl.InsertColumn(0, "项目", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(1, "值", LVCFMT_LEFT, 150);
	CString StrTmp = "";

	int nRow = 0;
	m_ListCtl.InsertItem( LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow,0, "p_type				段类型");
	m_ListCtl.SetItemText(nRow++,1, GetType2CString(GetProgramType, ehdr->p_type));

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_offset			文件偏移地址");
	StrTmp.Format("0x%x", ehdr->p_offset);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_vaddr			虚拟内存地址");
	StrTmp.Format("0x%x", ehdr->p_vaddr);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_paddr			物理内存地址");
	StrTmp.Format("0x%x", ehdr->p_paddr);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_filesz			文件中的长度");
	StrTmp.Format("0x%x", ehdr->p_filesz);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_memsz			内存中的长度");
	StrTmp.Format("0x%x", ehdr->p_memsz);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_flags			内存加载属性");
	m_ListCtl.SetItemText(nRow++, 1, GetType2CString(GetProgramMemType, ehdr->p_flags));

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "p_align			内存对齐粒度");
	StrTmp.Format("0x%x", ehdr->p_align);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);
	m_ListCtl.SetRedraw(TRUE);
	m_ListCtl.Invalidate();
	m_ListCtl.UpdateWindow();
	return VOID();
}

VOID CElfExplorerDlg::SetListCtlSHDR32Info(Elf32_Shdr* shdr)
{
	if (shdr == NULL)
	{
		return;
	}
	//清除上一次展示的数据
	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));
	m_ListCtl.InsertColumn(0, "项目", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(1, "值", LVCFMT_LEFT, 150);
	CString StrTmp = "";

	int nRow = 0;
	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_name				节名称索引");
	StrTmp.Format("%d", shdr->sh_name);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_type			类型");
	m_ListCtl.SetItemText(nRow++, 1, GetType2CString(GetSectionType,shdr->sh_type));

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_flags			内存属性");
	m_ListCtl.SetItemText(nRow++, 1, GetType2CString(GetSectionType, shdr->sh_flags));

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_addr			内存地址");
	StrTmp.Format("0x%x", shdr->sh_addr);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_offset			文件内偏移");
	StrTmp.Format("0x%x", shdr->sh_offset);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_sizez			节的长度");
	StrTmp.Format("0x%x", shdr->sh_size);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_link		链接节的索引");
	StrTmp.Format("%d", shdr->sh_link);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_info			信息");
	StrTmp.Format("%d", shdr->sh_info);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_addralign			对齐方式");
	StrTmp.Format("0x%x", shdr->sh_addralign);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);

	m_ListCtl.InsertItem(LVIF_TEXT | LVIF_PARAM, nRow, _T(""), 0, 0, 0, 0);
	m_ListCtl.SetItemText(nRow, 0, "sh_entsize			表元素大小");
	StrTmp.Format("0x%x", shdr->sh_entsize);
	m_ListCtl.SetItemText(nRow++, 1, StrTmp);
	m_ListCtl.SetRedraw(TRUE);
	m_ListCtl.Invalidate();
	m_ListCtl.UpdateWindow();
	return VOID();
}

VOID CElfExplorerDlg::SetStrTab32(ITEMDATA* data)
{
	char* base = m_SectionTabOff->base;
	Elf32_Shdr* Strshdr = (Elf32_Shdr*)m_SectionTabOff->dwStrTabOff;

	char* pStrTabAddr = (char*)(base + Strshdr->sh_offset);

	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));


	m_ListCtl.InsertColumn(0, "序号", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(1, "字符串内容", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(2, "字符串内容索引", LVCFMT_LEFT, 150);

	DWORD dwStrRow = 0;
	DWORD dwStrIdx = 1;
	CString StrTmp = "";
	pStrTabAddr++;
	while (*pStrTabAddr != NULL)
	{
		int nRow = 1;
		StrTmp.Format("%d", dwStrRow);
		m_ListCtl.InsertItem(dwStrRow, StrTmp);   //插入序号
		m_ListCtl.SetItemText(dwStrRow, nRow++, pStrTabAddr);  //插入字符串
		StrTmp.Format("%d", dwStrIdx);
		m_ListCtl.SetItemText(dwStrRow, nRow, StrTmp);  //插入索引

		int nStrlen = strlen(pStrTabAddr) + 1;            
		dwStrIdx += nStrlen;
		pStrTabAddr += nStrlen;
		dwStrRow++;

	}
	return VOID();
}

VOID CElfExplorerDlg::SetDynSymTab32()
{
	UpdateTips("动态符号表（保存了程序用到的导入导出函数的名字信息！）");
	char* base = m_SectionTabOff->base;
	Elf32_Shdr* dynSymTab = (Elf32_Shdr*)(m_SectionTabOff->dwDynsymOff);
	Elf32_Sym* dynSym = (Elf32_Sym*)(base + dynSymTab->sh_offset);
	Elf32_Shdr* dynstrTab = (Elf32_Shdr*)(m_SectionTabOff->dwDynStrTabOff);
	char* dynstr = (char*)(base + dynstrTab->sh_offset);

	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));

	m_ListCtl.InsertColumn(0, "序号", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(1, "符号名称", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(2, "符号地址", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(3, "长度", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(4, "符号属性", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(5, "引用节头序号", LVCFMT_LEFT, 120);
	//插入list表头
	CString strTmp = "";

	char bufTmp[] = "< Undefined >";
	//char* symstr = dynstr + (++dynSym->st_name);

	//为什么内存中序号是1；
	for (size_t i = 0; i < dynSymTab->sh_size / dynSymTab->sh_entsize;  i++)
	{
		int nRow = 1;
		char* symstr = dynstr + dynSym[i].st_name;
		if (symstr == "\0")
		{
			symstr = bufTmp;
		}
		strTmp.Format("%d", i);
		m_ListCtl.InsertItem(i, strTmp);
		m_ListCtl.SetItemText(i, nRow++, symstr);

		strTmp.Format("0x%x", dynSym[i].st_value);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("0x%x", dynSym[i].st_size);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		//拼接属性字符串
		int nBind = ELF32_ST_BIND(dynSym[i].st_info);
		int nType = ELF32_ST_TYPE(dynSym[i].st_info);
		CString strBind = GetType2CString(GETSECTIONBIND, nBind);
		CString strType = GetType2CString(GETSECTIONTYPE, nType);
		CString strAttribute = strBind +" | " + strType;
		m_ListCtl.SetItemText(i, nRow++, strAttribute);

		strTmp.Format("%d", dynSym[i].st_shndx);
		m_ListCtl.SetItemText(i, nRow++, strTmp);


	}


	return VOID();
}

VOID CElfExplorerDlg::SetRelDynTab32()
{
	//获取hash表
	char* base = m_SectionTabOff->base;
	Elf32_Shdr* hashTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwHashTabOff;
	Elf32_Addr hashTabHead =(Elf32_Addr)(base +  hashTabShdr->sh_offset);
	Elf32_Addr nbucket = hashTabHead;
	Elf32_Addr nchain = hashTabHead + 4;
	Elf32_Addr bucket = hashTabHead + 8;
	Elf32_Addr chain = hashTabHead + 8 + nbucket * 4;
	//xh_elf_hash();
	//获取符号表

	Elf32_Shdr* SymTabShdr =(Elf32_Shdr*) m_SectionTabOff->dwSymTabOff;
	Elf32_Sym* SymTabHead = (Elf32_Sym*)(base + SymTabShdr->sh_offset);

	//获取字符串表

	Elf32_Shdr* StrTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwStrTabOff;
	char* StrTabHead = base + StrTabShdr->sh_offset;

	//获取重定位表（1）

	Elf32_Shdr* RelDynTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwRelDynTabOff;
	Elf32_Rel* RelDynTabHead = (Elf32_Rel*)(base + RelDynTabShdr->sh_offset);


	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));

	m_ListCtl.InsertColumn(0, "序号", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(1, "offset", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(2, "符号序号", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(3, "类型", LVCFMT_LEFT, 100);
	CString strTmp = "";
	for (size_t i = 0; i < RelDynTabShdr->sh_size / RelDynTabShdr->sh_entsize; i++)
	{
		int nRow = 1;
		strTmp.Format("%d", i);
		m_ListCtl.InsertItem(i, strTmp);

		strTmp.Format("0x%x", RelDynTabHead[i].r_offset);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("%d", ELF32_R_SYM(RelDynTabHead[i].r_info));
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("%d", ELF32_R_TYPE(RelDynTabHead[i].r_info));
		m_ListCtl.SetItemText(i, nRow++, strTmp);

	}

	return VOID();
}

VOID CElfExplorerDlg::SetRelPltTab32()
{


	char* base = m_SectionTabOff->base;

	//动态符号表
	Elf32_Shdr* DynSymTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwDynsymOff;
	Elf32_Sym* DynSymTabHead = (Elf32_Sym*)(base + DynSymTabShdr->sh_offset);

	//获取动态字符串表

	Elf32_Shdr* DynStrTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwDynStrTabOff;
	char* DynStrTabHead = base + DynStrTabShdr->sh_offset;

	//获取重定位表（2）

	Elf32_Shdr* RelPltTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwRelPltTabOff;
	Elf32_Rel* RelPltTabHead = (Elf32_Rel*)(base + RelPltTabShdr->sh_offset);


	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));

	m_ListCtl.InsertColumn(0, "序号", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(1, "符号名", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(2, "符号偏移", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(3, "符号表索引", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(4, "符号类型", LVCFMT_LEFT, 100);

	CString strTmp = "";
	for (size_t i = 0; i < RelPltTabShdr->sh_size / RelPltTabShdr->sh_entsize; i++)
	{
		int nRow = 1;
		strTmp.Format("%d", i);
		m_ListCtl.InsertItem(i, strTmp);

		int symIdx = ELF32_R_SYM(RelPltTabHead[i].r_info);
		char* SymName = DynStrTabHead + DynSymTabHead[symIdx].st_name;
		m_ListCtl.SetItemText(i, nRow++, SymName);

		strTmp.Format("0x%x", RelPltTabHead[i].r_offset);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("%d", symIdx);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("%d", ELF32_R_TYPE(RelPltTabHead[i].r_info));
		m_ListCtl.SetItemText(i, nRow++, strTmp);

	}

	return VOID();
}

VOID CElfExplorerDlg::SetSymTab32()
{
	char* base = m_SectionTabOff->base;
	//获取字符串表
	Elf32_Shdr* StrTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwStrTabOff;
	char* StrTab = base + StrTabShdr->sh_offset;

	//获取符号表

	Elf32_Shdr* SymTabShdr = (Elf32_Shdr*)m_SectionTabOff->dwSymTabOff;
	Elf32_Sym* SymTab = (Elf32_Sym*)(base + SymTabShdr->sh_offset);


	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));

	m_ListCtl.InsertColumn(0, "序号", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(1, "符号名称", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(2, "符号地址", LVCFMT_LEFT, 100);
	m_ListCtl.InsertColumn(3, "长度", LVCFMT_LEFT, 80);
	m_ListCtl.InsertColumn(4, "符号属性", LVCFMT_LEFT, 200);
	m_ListCtl.InsertColumn(5, "引用节头序号", LVCFMT_LEFT, 120);
	//插入list表头
	CString strTmp = "";

	char bufTmp[] = "< Undefined >";

	for (size_t i = 0; i < SymTabShdr->sh_size / SymTabShdr->sh_entsize; i++)
	{
		int nRow = 1;
		char* symstr = StrTab + SymTab[i].st_name;
		if (symstr == "\0")
		{
			symstr = bufTmp;
		}
		strTmp.Format("%d", i);
		m_ListCtl.InsertItem(i, strTmp);
		m_ListCtl.SetItemText(i, nRow++, symstr);

		strTmp.Format("0x%x", SymTab[i].st_value);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		strTmp.Format("0x%x", SymTab[i].st_size);
		m_ListCtl.SetItemText(i, nRow++, strTmp);

		//拼接属性字符串
		int nBind = ELF32_ST_BIND(SymTab[i].st_info);
		int nType = ELF32_ST_TYPE(SymTab[i].st_info);
		CString strBind = GetType2CString(GETSECTIONBIND, nBind);
		CString strType = GetType2CString(GETSECTIONTYPE, nType);
		CString strAttribute = strBind + " | " + strType;
		m_ListCtl.SetItemText(i, nRow++, strAttribute);

		strTmp.Format("%d", SymTab[i].st_shndx);
		m_ListCtl.SetItemText(i, nRow++, strTmp);
	}




	return VOID();
}

VOID CElfExplorerDlg::finitRes()
{

	m_FilePath = "";
	UnmapViewOfFile(m_ElfBase);
	CloseHandle(m_hFileMapping);

	//清空list控件内容
	m_ListCtl.DeleteAllItems();
	while (m_ListCtl.DeleteColumn(0));
	//清空tree控件内容
	m_TreeCtl.DeleteAllItems();
	if (m_SectionTabOff != NULL )
	{
		delete[] m_SectionTabOff;

	}

	return VOID();
}

void CElfExplorerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	finitRes();
	CDialogEx::OnClose();
}





void CElfExplorerDlg::OnNMClickTreeinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	//获得当前节点  
	CPoint point;
	GetCursorPos(&point);
	::ScreenToClient(m_TreeCtl.GetSafeHwnd(), &point);
	UINT uFlags;
	uFlags = TVHT_ONITEM;
	HTREEITEM    hCurrentItem = m_TreeCtl.HitTest(point, &uFlags);
	if (!(hCurrentItem && (TVHT_ONITEM & uFlags)))
	{
		return;
	}
	UpdateTips(m_TreeCtl.GetItemText(hCurrentItem));
	ITEMDATA* pdata = (ITEMDATA*)m_TreeCtl.GetItemData(hCurrentItem);

	
	/*
	dwStructType = 4代表32位程序头
	dwStructType = 5代表64位程序头
	dwStructType = 6代表32位节头
	dwStructType = 7代表64位节头
	*/
	if (pdata->dwStructType == 4)
	{
		//32位处理
		Elf32_Phdr* phdr = (Elf32_Phdr*)pdata->pData;
		SetListCtlPHDR32Info(phdr);

	}
	else if(pdata->dwStructType == 5)
	{
		//64位处理

	}
	if (pdata->dwStructType == 6)
	{
		//32位处理
		Elf32_Shdr* Shdr = (Elf32_Shdr*)pdata->pData;
		switch (pdata->dwSectionType)
		{
		case SET_STRTAB:
			SetStrTab32(pdata);
			break;
		case SET_DYNSYM:
			SetDynSymTab32();
			break;
		case SET_RELDYN:
			SetRelDynTab32();
			break;
		case SET_RELPLT:
			SetRelPltTab32();
			break;
		case SET_SYMTAB:
			SetSymTab32();
			break;
		default:
			SetListCtlSHDR32Info(Shdr);
			break;
		}
		

	}
	else if (pdata->dwStructType == 7)
	{
		//64位处理

	}
	*pResult = 0;
}
//列表内容进行大数据量更新时，避免闪烁
//m_list.SetRedraw(FALSE);
////更新内容
//m_list.SetRedraw(TRUE);
//m_list.Invalidate();
//m_list.UpdateWindow();

void CElfExplorerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);
}
