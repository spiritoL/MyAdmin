// DlgNetInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgNetInfo.h"
#include "DlgNodeCreate.h"
#include "SECMNGSECNODE.h"


// CDlgNetInfo

IMPLEMENT_DYNCREATE(CDlgNetInfo, CFormView)

CDlgNetInfo::CDlgNetInfo()
	: CFormView(CDlgNetInfo::IDD)
	, m_dateEnd(COleDateTime::GetCurrentTime())
	, m_dateBegin(COleDateTime::GetCurrentTime())
{

}

CDlgNetInfo::~CDlgNetInfo()
{
}

void CDlgNetInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECNODE, m_listSecNode);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dateEnd);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateBegin);
}

BEGIN_MESSAGE_MAP(CDlgNetInfo, CFormView)
	
	ON_BN_CLICKED(IDC_BUTTON_NODECREATE, &CDlgNetInfo::OnBnClickedButtonNodecreate)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDlgNetInfo::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_NODEDEL, &CDlgNetInfo::OnBnClickedButtonNodedel)
END_MESSAGE_MAP()


// CDlgNetInfo ���

#ifdef _DEBUG
void CDlgNetInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgNetInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgNetInfo ��Ϣ�������

//�����ݹ��ص�������
int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
{
	LVITEM		lvi;
	//���õ�0��
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iImage = 4;
	lvi.iItem = 0; //ͷ�� //�ڵڼ��в�������
	lvi.iSubItem = 0; //�ڼ���
	lvi.pszText = (LPTSTR)(LPCTSTR)ID;
	m_listSecNode.InsertItem(&lvi);

	//����1��
	lvi.iSubItem = 1; //�ڼ���
	lvi.pszText = (LPTSTR)(LPCTSTR)Name;
	m_listSecNode.SetItem(&lvi);

	//����2��
	lvi.iSubItem = 2; //�ڼ���
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	//����3��

	lvi.iSubItem = 3; //�ڼ���
	if (state == 0)
	{
		lvi.pszText = "����";
	}
	else
	{
		lvi.pszText = "������";
	}
	m_listSecNode.SetItem(&lvi);

	//����4��
	CString str = "";
	str.Format("%d", authcode);

	lvi.iSubItem = 4;
	lvi.pszText = (LPTSTR)(LPCTSTR)str;
	m_listSecNode.SetItem(&lvi);

	return 0;
}

//��ʼ��������Ϣ����
void CDlgNetInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO:  �ڴ����ר�ô����/����û���
	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);


	//ͨ����̵ķ�ʽ ���� �����Խ��淽ʽȥ�޸� CWnd����Դ����
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);
	m_listSecNode.InsertColumn(0, "������", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "��������", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "���㴴��ʱ��", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "����״̬", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "������Ȩ��", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);
}

extern CDatabase	*g_pDB;

void CDlgNetInfo::OnBnClickedButtonNodecreate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int flag = 0;//��־λ   0����

	CDlgNodeCreate		dlgNodeCreate;
	if (dlgNodeCreate.DoModal()==IDCANCEL)
	{
		return;
	}
	//��������Ϣ���浽���ݿ���
	CSECMNGSECNODE		secmngSecNode(g_pDB);

	//��������
	g_pDB->BeginTrans();
	TRY
	{
		if (!secmngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("��������Ϣ��ʧ��");
			return;
		}
		//�����ݿ�ı���������������
		secmngSecNode.AddNew();

		secmngSecNode.m_ID = dlgNodeCreate.m_strNodeId;
		secmngSecNode.m_NAME = dlgNodeCreate.m_strNodeName;
		secmngSecNode.m_NODEDESC = dlgNodeCreate.m_strNodeDesc;
		secmngSecNode.m_CREATETIME = CTime::GetCurrentTime();
		secmngSecNode.m_AUTHCODE = atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeAuth);  //�����������Զ����ɵ� ���ǿ����޸� ΢��
		secmngSecNode.m_STATE = atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeStaus);

		secmngSecNode.Update();//��������
	}
	CATCH_ALL(e)
	{
		flag = 1;
		e->ReportError();
		goto MyEnd;
	}
	END_CATCH_ALL
MyEnd :
	if (flag==0)
	{
		g_pDB->CommitTrans();
	}
	else
	{
		if (secmngSecNode.IsOpen())
		{
			secmngSecNode.Close();
		}
		g_pDB->Rollback();
		return;
	}
	if (secmngSecNode.IsOpen())
	{
		secmngSecNode.Close();
	}

	//�����ݹ��ص�������
	DbInitListSecNode(dlgNodeCreate.m_strNodeId, dlgNodeCreate.m_strNodeName,
		CTime::GetCurrentTime(), atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeStaus),
		atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeAuth));
}


void CDlgNetInfo::OnBnClickedButtonSearch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ѽ�������� ���µ�C++������
	UpdateData(TRUE);

	CWnd		*myWnd = NULL;
	CButton		*But = NULL;

	//ͨ��һ��������Դ��ֱ�ӵõ�һ�������ָ��
	myWnd = CWnd::GetDlgItem(IDC_CHECK_TIME);

	But = (CButton*)myWnd;

	if (But->GetCheck()==BST_UNCHECKED)
	{
		AfxMessageBox("û��ѡ��check��ѡ��");
		return;
	}
	int flag = 0;

	CSECMNGSECNODE		secmngSecNode(g_pDB);

	TRY 
	{
		CTime sqlTime1(m_dateBegin.GetYear(), m_dateBegin.GetMonth(), m_dateBegin.GetDay(),
		0, 0, 0);
		CTime sqlTime2(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(),
			23, 59, 59);

		if (!secmngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("��������Ϣ��ʧ��");
			return;
		}
		//���ؼ��������ȫ��ɾ��
		m_listSecNode.DeleteAllItems();
		//����  ��ֹm_listSecNode�ؼ� �ػ�
		m_listSecNode.SetRedraw(FALSE);
		while (!secmngSecNode.IsBOF())
		{
			
			if (!(secmngSecNode.m_CREATETIME >= sqlTime1 &&
				secmngSecNode.m_CREATETIME <= sqlTime2))
			{
				//����������ʱ��ε����ݣ���ȡ��һ�����ݣ������ж�
				secmngSecNode.MoveNext();
				continue;
			}
			////����������ʱ��ε����ݣ����ȡ�����ڵ�������
			DbInitListSecNode(secmngSecNode.m_ID, secmngSecNode.m_NAME, secmngSecNode.m_CREATETIME,
				secmngSecNode.m_STATE, secmngSecNode.m_AUTHCODE);
			
			secmngSecNode.MoveNext();
		}

		m_listSecNode.SetRedraw(TRUE);
	}
	CATCH_ALL (e)
	{
		
	}
	END_CATCH_ALL

	if (secmngSecNode.IsOpen())
	{
		secmngSecNode.Close();
	}

}


void CDlgNetInfo::OnBnClickedButtonNodedel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int						flag = 0;
	UpdateData(TRUE);

	//��ȡ��һ��ѡ��λ��
	POSITION pos = m_listSecNode.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		MessageBox("��û��ѡȡ��¼, ����ѡ�����е�һ����¼��", "�޸�������Ϣ", MB_MODEMASK);
		return;
	}
	//��ȡѡȡ�ĵڼ���
	int  nItem = m_listSecNode.GetNextSelectedItem(pos);

	// ��ȡ�ڼ��У���0�е�Ԫ��
	CString myId = m_listSecNode.GetItemText(nItem, 0);
	CString myname = m_listSecNode.GetItemText(nItem, 1);

	//��ȡѡ���е�����
	int	selCont = m_listSecNode.GetSelectedCount();
	if (selCont > 1) {
		MessageBox("��ѡ���˶��м�¼, ����ѡ�����е�һ����¼��", "ɾ��������Ϣ", MB_MODEMASK);
		return;
	}

	CString strTmp;
	strTmp.Format("�Ƿ�Ҫɾ�����Ϊ��%s����������Ϣ��", myId);
	if (AfxMessageBox(strTmp, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
	{
		return;
	}
	CSECMNGSECNODE  rsetMngSecNode(g_pDB);
	g_pDB->BeginTrans();
	TRY
	{
		rsetMngSecNode.m_strFilter.Format("ID= '%s'", (LPTSTR)(LPCTSTR)myId);
		if (!rsetMngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("��������Ϣ��ʧ��");
			return;
		}

		if (!rsetMngSecNode.IsEOF())
		{
			rsetMngSecNode.Delete();
		}
		else
		{
			AfxMessageBox("���������� û�в��ҵ���Ӧ��¼ myid:" + myId);
		}
	}
	CATCH_ALL(e)
	{
		flag = 1;
		e->ReportError();
		goto MYEnd;
	}
	END_CATCH_ALL

	MYEnd :
	if (flag == 1)
	{
		g_pDB->Rollback();
	}
	else
	{
		g_pDB->CommitTrans();
		m_listSecNode.DeleteItem(nItem); //�ӽ�����ɾ������
		AfxMessageBox("ɾ������ok");

	}
	if (rsetMngSecNode.IsOpen())
	{
		rsetMngSecNode.Close();
	}

}
