
// apksignerDlg.h: 头文件
//

#pragma once
#include <string>
using namespace std;

// CapksignerDlg 对话框
class CapksignerDlg : public CDialogEx
{
// 构造
public:
	CapksignerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APKSIGNER_DIALOG };
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
public:
	afx_msg void OnBnClickedSign();


public:
	//全局变量


	CMFCEditBrowseCtrl m_CtlFileApk;
	CMFCEditBrowseCtrl m_CtlKetPath;
	CEdit m_CtlKeyPass;
	CString m_OutApkPath;
	CString m_strFileApk;
	CString m_strKeyPath;
	CString m_strKeyPass;


public:
	//自定义函数
	void ClearCtlValue();
	virtual void OnOK();
};
