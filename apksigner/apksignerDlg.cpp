
// apksignerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "apksigner.h"
#include "apksignerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CapksignerDlg 对话框



CapksignerDlg::CapksignerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APKSIGNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CapksignerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MFC_FILEAPK, m_CtlFileApk);
	DDX_Control(pDX, MFC_FILEKEY, m_CtlKetPath);
	DDX_Control(pDX, EDIT_KEYPASS, m_CtlKeyPass);
}

BEGIN_MESSAGE_MAP(CapksignerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_SIGN, &CapksignerDlg::OnBnClickedSign)
END_MESSAGE_MAP()


// CapksignerDlg 消息处理程序

BOOL CapksignerDlg::OnInitDialog()
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
	ClearCtlValue();



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CapksignerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CapksignerDlg::OnPaint()
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
HCURSOR CapksignerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CapksignerDlg::OnBnClickedSign()
{
	// TODO: 在此添加控件通知处理程序代码
	//apksigner sign --ks G:\android\key\20240906key.jks  --ks-pass pass:123456 -out G:\android\360加固分析文件\out2.apk G:\android\360加固分析文件\app-release-yuan_10_jiagu.apk
	//F:\VSProgram\apksigner\Debug\java\java\bin\java.exe -jar F:\VSProgram\apksigner\Debug\java\tools\apksigner.jar  sign --ks G:\android\key\20240906key.jks  
	// --ks-pass pass:123456 -out G:\android\360加固分析文件\out2.apk G:\android\360加固分析文件\app-release-yuan_10_jiagu.apk
	//获取变量名称
	GetDlgItemText(MFC_FILEAPK, m_strFileApk);

	GetDlgItemText(MFC_FILEKEY, m_strKeyPath);

	GetDlgItemText(EDIT_KEYPASS, m_strKeyPass);

	
	if (m_strFileApk.IsEmpty() )
	{
		//SetWindowText("apk路径不能为空！");
		return;
	}

	string strformat = m_strFileApk.GetBuffer();
	strformat.insert(strformat.find_last_of("."), "_sign");
	m_OutApkPath = strformat.c_str();

	DWORD dwDirLen = 256;
	char dirBuff[256] = {};
	GetModuleFileName(NULL, dirBuff, 256);
	string strModPath = dirBuff;
	strModPath = strModPath.substr(0, strModPath.find_last_of("\\") + 1);

	if (m_strKeyPath.IsEmpty() || m_strKeyPass.IsEmpty())
	{
		m_strKeyPath = strModPath.c_str();
		m_strKeyPath.Append("java\\key\\20240906key.jks ");
		m_strKeyPass = "123456 ";

	}




	
	string strJava = strModPath + "\\java\\java\\bin\\java.exe";
	string strapksigner = strModPath + "\\java\\tools\\apksigner.jar";


	string strCMD = "cmd.exe /c " + strJava + " -jar " + strapksigner + " sign " + " --ks " + m_strKeyPath.GetBuffer() + " --ks-pass pass:" + m_strKeyPass.GetBuffer() + " -out " + m_OutApkPath.GetBuffer() + "  " + m_strFileApk.GetBuffer();
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

		//创建一个新进程  
    if(CreateProcess(  
		NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串  
		(LPSTR)strCMD.c_str(), // 命令行字符串  
        NULL, //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。  
        NULL, //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>  
        false,//    指示新进程是否从调用进程处继承了句柄。   
		CREATE_NO_WINDOW,  //  指定附加的、用来控制优先类和进程的创建的标  
            //  CREATE_NEW_CONSOLE  新控制台打开子进程  
            //  CREATE_SUSPENDED    子进程创建后挂起，直到调用ResumeThread函数  
        NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境  
        NULL, //    指定子进程的工作路径  
        &si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体  
        &pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体  
        ))  
    {  
        
  
        //下面两行关闭句柄，解除本进程和新进程的关系，不然有可能不小心调用TerminateProcess函数关掉子进程  
//      CloseHandle(pi.hProcess);  
//      CloseHandle(pi.hThread);  
    }  
    else{  
       
    }  
	CString strTmp = strCMD.c_str();

}

void CapksignerDlg::ClearCtlValue()
{
	SetDlgItemText(MFC_FILEAPK, "");
	SetDlgItemText(MFC_FILEKEY, "");
	SetDlgItemText(EDIT_KEYPASS, "");
}


void CapksignerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
	OnBnClickedSign();
}
