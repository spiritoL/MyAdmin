// DlgInitCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgInitCfg.h"
#include "afxdialogex.h"
#include "cfg_op.h"


// CDlgInitCfg 对话框

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


// CDlgInitCfg 消息处理程序





void CDlgInitCfg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//把界面的输入 存入到 C++变量中

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
		AfxMessageBox("用户输入不能为空");
		return;
	}


	//在exe所在的路径之下 读配置文件
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
		AfxMessageBox("保存配置项 DSN 失败");
		return;
	}

	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath /*in*/, "UID" /*in*/, (LPTSTR)(LPCTSTR)m_strDbUser/*in*/, m_strDbUser.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("保存配置项 UID 失败");
		return;
	}

	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath /*in*/, "PWD" /*in*/, (LPTSTR)(LPCTSTR)m_strDbPw/*in*/, m_strDbPw.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("保存配置项 PWD 失败");
		return;
	}
	CDialog::OnOK();
}


void CDlgInitCfg::OnBnClickedButtonTest()
{
	// TODO:  在此添加控件通知处理程序代码
	CDatabase myDB;
	int iTag = 0; //没有异常
	UpdateData(TRUE);

	if (m_strDbSource.IsEmpty() ||
		m_strDbUser.IsEmpty() ||
		m_strDbPw.IsEmpty())
	{
		AfxMessageBox("用户的输入不能是空");
		return;
	}

	CString		strCon;
	strCon.Format("DSN=%s;UID=%s;PWD=%s", m_strDbSource, m_strDbUser, m_strDbPw);

	TRY
	{
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("通过数据源连接数据库失败, 请检查数据源配置是否正确!  ");
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
		AfxMessageBox("db测试通过");

	}
	else
	{
		AfxMessageBox("db测试 err");
	}
}
