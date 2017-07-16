// DlgNodeCreate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgNodeCreate.h"
#include "afxdialogex.h"


// CDlgNodeCreate �Ի���

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


// CDlgNodeCreate ��Ϣ�������





void CDlgNodeCreate::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strNodeAuth.IsEmpty() ||
		m_strNodeDesc.IsEmpty() ||
		m_strNodeId.IsEmpty() ||
		m_strNodeName.IsEmpty() ||
		m_strNodeStaus.IsEmpty())
	{
		AfxMessageBox("�û����벻��Ϊ��");
		return;
	}
	CDialog::OnOK();
}
