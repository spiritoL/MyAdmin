#pragma once


// CDlgNodeCreate �Ի���

class CDlgNodeCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgNodeCreate)

public:
	CDlgNodeCreate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNodeCreate();

// �Ի�������
	enum { IDD = IDD_DIALOG_NODECREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNodeAuth;
	CString m_strNodeDesc;
	CString m_strNodeId;
	CString m_strNodeName;
	CString m_strNodeStaus;
	afx_msg void OnBnClickedOk();
};
