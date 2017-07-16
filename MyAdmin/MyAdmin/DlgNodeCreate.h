#pragma once


// CDlgNodeCreate 对话框

class CDlgNodeCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgNodeCreate)

public:
	CDlgNodeCreate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNodeCreate();

// 对话框数据
	enum { IDD = IDD_DIALOG_NODECREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNodeAuth;
	CString m_strNodeDesc;
	CString m_strNodeId;
	CString m_strNodeName;
	CString m_strNodeStaus;
	afx_msg void OnBnClickedOk();
};
