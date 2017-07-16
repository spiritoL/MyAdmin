#pragma once


// CDlgInitCfg 对话框

class CDlgInitCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgInitCfg)

public:
	CDlgInitCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInitCfg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INITDBINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDbPw;
	CString m_strDbSource;
	CString m_strDbUser;
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonTest();
};
