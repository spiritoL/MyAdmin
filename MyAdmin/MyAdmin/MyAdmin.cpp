
// MyAdmin.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyAdmin.h"
#include "MainFrm.h"

#include "MyAdminDoc.h"
#include "MyAdminView.h"
#include "DlgInitCfg.h"
#include "cfg_op.h"
#include "secmng_globvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyAdminApp

BEGIN_MESSAGE_MAP(CMyAdminApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMyAdminApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMyAdminApp ����

CMyAdminApp::CMyAdminApp()
{
	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyAdmin.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMyAdminApp ����

CMyAdminApp theApp;

// CMyAdminApp ��ʼ��

int CMyAdminApp::ReadConfig()
{
	int				ret = 0;
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

	//�������ļ�
	//int readCfg1(char *Path, char *key, char *valude);
	//int readCfg2(FILE *fp, char *key, char *valude);

	//int writeCfg(char *Path, char *key, char *valude);

	//DSN ������Դ
	//CString ===> char *
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "DSN", tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("�������ļ� DSN�� ʧ��");
		return 1;
	}
	g_dbSource = tmpBuf;


	//�� ������UID
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "UID", tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("�������ļ� UID �� ʧ��");
		return 1;
	}
	g_dbUse = tmpBuf;

	//�� ������ PWD
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "PWD", tmpBuf, &tmpBufLen);
	if (ret != 0)
	{
		AfxMessageBox("�������ļ� PWD�� ʧ��");
		return 1;
	}
	g_dbpw = tmpBuf;

	return ret;
}

extern CDatabase	myDB;
extern CDatabase	*g_pDB;

//win odbc���� ���� oracle���ݿ�
int CMyAdminApp::connectDbByOdbc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	int ret = 0;
	int iTag = 0; //û���쳣

	CString		strCon;
	strCon.Format("DSN=%s;UID=%s;PWD=%s", g_dbSource, g_dbUse, g_dbpw);

	TRY
	{
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("ͨ������Դ�������ݿ�ʧ��, ��������Դ�����Ƿ���ȷ!  ");
			return -1;
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
		ret = 0;
	}
	else
	{
		ret = -1;

	}

	g_pDB = &myDB;
	return ret;
}


// CMyAdminApp ��ʼ��

BOOL CMyAdminApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	int				ret = 0;
	CDlgInitCfg  dlgInitCfg;

	ret = ReadConfig();
	if (ret != 0)
	{
		//AfxMessageBox("�������ļ�ʧ��");
		if (dlgInitCfg.DoModal() == IDCANCEL)
		{
			return FALSE;
		}
		else
		{
			g_dbpw = dlgInitCfg.m_strDbPw;
			g_dbSource = dlgInitCfg.m_strDbSource;
			g_dbUse = dlgInitCfg.m_strDbUser;
		}
	}

	//
	ret = connectDbByOdbc();
	if (ret != 0)
	{
		AfxMessageBox("�������ݿ�ʧ��");
		return FALSE;
	}

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMyAdminView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CMyAdminApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMyAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyAdminApp ��Ϣ�������



