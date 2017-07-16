// DlgNodeCreate.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgNodeCreate.h"
#include "afxdialogex.h"


// CDlgNodeCreate 对话框

IMPLEMENT_DYNAMIC(CDlgNodeCreate, CDialog)

CDlgNodeCreate::CDlgNodeCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNodeCreate::IDD, pParent)
	, m_strNodeAuth(_T(""))
	, m_strNodeDesc(_T(""))
	, m_strNodeId(_T(""))
	, m_strNodeName(_T(""))
	, m_strNodeStaus(_T(""))
{

}

CDlgNodeCreate::~CDlgNodeCreate()
{
}

void CDlgNodeCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NODEAUTH, m_strNodeAuth);
	DDX_Text(pDX, IDC_EDIT_NODEDESC, m_strNodeDesc);
	DDX_Text(pDX, IDC_EDIT_NODEID, m_strNodeId);
	DDX_Text(pDX, IDC_EDIT_NODENAME, m_strNodeName);
	DDX_Text(pDX, IDC_EDITNODESTATE, m_strNodeStaus);
}


BEGIN_MESSAGE_MAP(CDlgNodeCreate, CDialog)
	
	ON_BN_CLICKED(IDOK, &CDlgNodeCreate::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNodeCreate 消息处理程序





void CDlgNodeCreate::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strNodeAuth.IsEmpty() ||
		m_strNodeDesc.IsEmpty() ||
		m_strNodeId.IsEmpty() ||
		m_strNodeName.IsEmpty() ||
		m_strNodeStaus.IsEmpty())
	{
		AfxMessageBox("用户输入不能为空");
		return;
	}
	CDialog::OnOK();
}
