// DlgInitCfg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgInitCfg.h"
#include "afxdialogex.h"
#include "cfg_op.h"


// CDlgInitCfg �Ի���

IMPLEMENT_DYNAMIC(CDlgInitCfg, CDialog)

CDlgInitCfg::CDlgInitCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInitCfg::IDD, pParent)
	, m_strDbPw(_T(""))
	, m_strDbSource(_T(""))
	, m_strDbUser(_T(""))
{

}

CDlgInitCfg::~CDlgInitCfg()
{
}

void CDlgInitCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBPW, m_strDbPw);
	DDX_Text(pDX, IDC_EDIT_DBSOURCE, m_strDbSource);
	DDX_Text(pDX, IDC_EDIT_DBUSER, m_strDbUser);
}


BEGIN_MESSAGE_MAP(CDlgInitCfg, CDialog)
	
	ON_BN_CLICKED(IDOK, &CDlgInitCfg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgInitCfg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CDlgInitCfg ��Ϣ�������





void CDlgInitCfg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ѽ�������� ���뵽 C++������

	int		ret = 0;
	UpdateData(TRUE);

	m_strDbPw.TrimLeft();
	m_strDbPw.TrimRight();

	m_strDbSource.TrimLeft();
	m_strDbSource.TrimRight();

	m_strDbUser.TrimLeft();
	m_strDbUser.TrimRight();

	if (m_strDbPw.IsEmpty() || m_strDbSource.IsEmpty() || m_strDbUser.IsEmpty())
	{
		AfxMessageBox("�û����벻��Ϊ��");
		return;
	}


	//��exe���ڵ�·��֮�� �������ļ�
	char strFileName[4096];
	int strFileNameLen = 4096;
	char		tmpBuf[2048] = { 0 };
	int			tmpBufLen = 2048;
	//api  
	GetModuleFileName(AfxGetInstanceHandle(), strFileName, strFileNameLen);
	//AfxMessageBox(strFileName);

	CString strFilePath = strFileName;
	int i = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(i);

	strFilePath = strFilePath + "\\" + "secmngadmin.ini";
	//AfxMessageBox(strFilePath);

	//

	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath /*in*/, "DSN" /*in*/, (LPTSTR)(LPCTSTR)m_strDbSource/*in*/, m_strDbSource.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("���������� DSN ʧ��");
		return;
	}

	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath /*in*/, "UID" /*in*/, (LPTSTR)(LPCTSTR)m_strDbUser/*in*/, m_strDbUser.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("���������� UID ʧ��");
		return;
	}

	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath /*in*/, "PWD" /*in*/, (LPTSTR)(LPCTSTR)m_strDbPw/*in*/, m_strDbPw.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("���������� PWD ʧ��");
		return;
	}
	CDialog::OnOK();
}


void CDlgInitCfg::OnBnClickedButtonTest()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDatabase myDB;
	int iTag = 0; //û���쳣
	UpdateData(TRUE);

	if (m_strDbSource.IsEmpty() ||
		m_strDbUser.IsEmpty() ||
		m_strDbPw.IsEmpty())
	{
		AfxMessageBox("�û������벻���ǿ�");
		return;
	}

	CString		strCon;
	strCon.Format("DSN=%s;UID=%s;PWD=%s", m_strDbSource, m_strDbUser, m_strDbPw);

	TRY
	{
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("ͨ������Դ�������ݿ�ʧ��, ��������Դ�����Ƿ���ȷ!  ");
			return;
		}
		if (myDB.IsOpen())
		{
			myDB.Close();
		}
	}
	CATCH_ALL(e)
	{
		iTag = 1;
		e->ReportError();
	}
	END_CATCH_ALL


	if (iTag == 0)
	{
		AfxMessageBox("db����ͨ��");

	}
	else
	{
		AfxMessageBox("db���� err");
	}
}
