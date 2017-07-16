#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "ATLComTime.h"



// CDlgNetInfo ������ͼ

class CDlgNetInfo : public CFormView
{
	DECLARE_DYNCREATE(CDlgNetInfo)

protected:
	CDlgNetInfo();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgNetInfo();

public:
	enum { IDD = IDD_DIALOG_NETMNG };
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
	
	CListCtrl m_listSecNode;
//	CDateTimeCtrl m_dateEnd;
//	CDateTimeCtrl m_dateBegin;

	CImageList m_imageList;
	int DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode);
	afx_msg void OnBnClickedButtonNodecreate();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonNodedel();
	COleDateTime m_dateEnd;
	COleDateTime m_dateBegin;
};


