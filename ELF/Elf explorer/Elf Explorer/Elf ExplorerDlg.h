
// Elf ExplorerDlg.h: 头文件
//

#pragma once
#include "common/elf.h"

typedef struct
{
	CHAR*    szTitle;           //列表的名称
	int		  nWidth;            //列表的宽度

}LISTITEM;

typedef struct
{
	DWORD dwStructType;
	DWORD_PTR pData;
	DWORD dwSectionType;
}ITEMDATA;


typedef struct {

	char*		base;
	DWORD dwIs64Bit; 
	DWORD dwStrTabOff;
	DWORD dwDynsymOff;
	DWORD dwSymTabOff;
	DWORD dwHashTabOff;
	DWORD dwGnuHashTabOff;
	DWORD dwRelDynTabOff;
	DWORD dwRelPltTabOff;
	DWORD dwDynStrTabOff;
	DWORD dwShStrTabOff;
}SECTIONTAB;

// CElfExplorerDlg 对话框
class CElfExplorerDlg : public CDialogEx
{
// 构造
public:
	CElfExplorerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ELF_EXPLORER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public: //控件变量

	CTreeCtrl m_TreeCtl;
	CListCtrl m_ListCtl;
	CStatic m_TipCtl;
	CString m_StrTips;
public: //内部使用变量

	CString m_FilePath;
	PBYTE   m_ElfBase;
	HANDLE m_hFileMapping;
	DWORD m_dwFileSize;
	SECTIONTAB* m_SectionTabOff;
public: //控件消息处理函数
	afx_msg void OnEnChangeIdent();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	
	
	CString m_ElfIdent;
	CString m_ElfType;
	CString m_ElfMachine;
	CString m_ElfCpuFlag;
	CString m_ElfPHDROff;
	CString m_ELFSHDROff;
	CString M_ElfEntryPoint;
	CString m_ElfEntryFunc;
	CString m_ElfHDRSize;
	CString m_ElfPHDRSize;
	CString m_ElfPHDRNum;
	CString m_ElfVersion;
	CString m_ElfSHDRSize;
	CString m_ElfSHDRNum;
	CString m_ElfSNameIdx;
	CString m_ElfABI;
	CString m_AddSecName;
public:   //自定义函数
	VOID CElfExplorerDlg::InitListControl(int number, LISTITEM* Item);
	VOID CElfExplorerDlg::LoadFileMap();
	VOID CElfExplorerDlg::UpdateTips(const char* Tips);
	VOID CElfExplorerDlg::FormatELF(PBYTE ElfBase);
	VOID CElfExplorerDlg::FormatELF32(PBYTE ElfBase);
	VOID CElfExplorerDlg::FormatELF64(PBYTE ElfBase);
	VOID CElfExplorerDlg::FormatELF32EHDR(Elf32_Ehdr* ehdr);
	VOID CElfExplorerDlg::FormatELF32PHDR(Elf32_Ehdr* ehdr);
	VOID CElfExplorerDlg::FormatELF32SHDR(Elf32_Ehdr* ehdr);
	VOID CElfExplorerDlg::FormatELF64EHDR(PBYTE ElfBase);
	VOID CElfExplorerDlg::FormatELF64PHDR(PBYTE ElfBase);
	VOID CElfExplorerDlg::FormatELF64SHDR(PBYTE ElfBase);
public:
	CString CElfExplorerDlg::GetType2CString(int type ,int formatData);
	VOID CElfExplorerDlg::SetListCtlPHDR32Info(Elf32_Phdr* phdr);
	VOID CElfExplorerDlg::SetListCtlSHDR32Info(Elf32_Shdr* shdr);
	VOID CElfExplorerDlg::FixSectionTable(char* pNewBase);
	VOID CElfExplorerDlg::SetStrTab32(ITEMDATA* data);
	VOID CElfExplorerDlg::SetDynSymTab32();
	VOID CElfExplorerDlg::SetRelDynTab32();
	VOID CElfExplorerDlg::SetRelPltTab32();
	VOID CElfExplorerDlg::SetSymTab32();
	VOID CElfExplorerDlg::finitRes();
public:
	afx_msg void OnClose();
	afx_msg void OnNMClickTreeinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedAddsection();
	afx_msg void OnBnClickedRebuildsec();
};
