#pragma once
#include "afxwin.h"



// CCfgView ������ͼ

class CCfgView : public CFormView
{
	DECLARE_DYNCREATE(CCfgView)

protected:
	CCfgView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCfgView();

public:
	enum { IDD = IDD_DIALOG_CFG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonSerinfosave();
	virtual void OnInitialUpdate();
	CString m_strsrvip;
//	CEdit m_strsrvmaxnodenum;
	CString m_strsrvport;
	CString m_strsrvmaxnodenum;
};


