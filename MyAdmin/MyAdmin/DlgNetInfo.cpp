// DlgNetInfo.cpp : 实现文件
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


// CDlgNetInfo 诊断

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


// CDlgNetInfo 消息处理程序

//将数据挂载到界面上
int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
{
	LVITEM		lvi;
	//设置第0列
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iImage = 4;
	lvi.iItem = 0; //头插 //在第几行插入数据
	lvi.iSubItem = 0; //第几列
	lvi.pszText = (LPTSTR)(LPCTSTR)ID;
	m_listSecNode.InsertItem(&lvi);

	//设置1列
	lvi.iSubItem = 1; //第几列
	lvi.pszText = (LPTSTR)(LPCTSTR)Name;
	m_listSecNode.SetItem(&lvi);

	//设置2列
	lvi.iSubItem = 2; //第几列
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	//设置3列

	lvi.iSubItem = 3; //第几列
	if (state == 0)
	{
		lvi.pszText = "正常";
	}
	else
	{
		lvi.pszText = "不正常";
	}
	m_listSecNode.SetItem(&lvi);

	//设置4列
	CString str = "";
	str.Format("%d", authcode);

	lvi.iSubItem = 4;
	lvi.pszText = (LPTSTR)(LPCTSTR)str;
	m_listSecNode.SetItem(&lvi);

	return 0;
}

//初始化网点信息界面
void CDlgNetInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO:  在此添加专用代码和/或调用基类
	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);


	//通过编程的方式 或者 是属性界面方式去修改 CWnd类资源属性
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);
	m_listSecNode.InsertColumn(0, "网点编号", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "网点名称", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "网点创建时间", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "网点状态", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "网点授权码", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);
}

extern CDatabase	*g_pDB;

void CDlgNetInfo::OnBnClickedButtonNodecreate()
{
	// TODO:  在此添加控件通知处理程序代码
	int flag = 0;//标志位   0正常

	CDlgNodeCreate		dlgNodeCreate;
	if (dlgNodeCreate.DoModal()==IDCANCEL)
	{
		return;
	}
	//把网点信息保存到数据库中
	CSECMNGSECNODE		secmngSecNode(g_pDB);

	//开启事物
	g_pDB->BeginTrans();
	TRY
	{
		if (!secmngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开网点信息表失败");
			return;
		}
		//打开数据库的表，向里面新增数据
		secmngSecNode.AddNew();

		secmngSecNode.m_ID = dlgNodeCreate.m_strNodeId;
		secmngSecNode.m_NAME = dlgNodeCreate.m_strNodeName;
		secmngSecNode.m_NODEDESC = dlgNodeCreate.m_strNodeDesc;
		secmngSecNode.m_CREATETIME = CTime::GetCurrentTime();
		secmngSecNode.m_AUTHCODE = atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeAuth);  //编译器给我自动生成的 我们可以修改 微调
		secmngSecNode.m_STATE = atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeStaus);

		secmngSecNode.Update();//更新数据
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

	//把数据挂载到界面上
	DbInitListSecNode(dlgNodeCreate.m_strNodeId, dlgNodeCreate.m_strNodeName,
		CTime::GetCurrentTime(), atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeStaus),
		atoi((LPTSTR)(LPCTSTR)dlgNodeCreate.m_strNodeAuth));
}


void CDlgNetInfo::OnBnClickedButtonSearch()
{
	// TODO:  在此添加控件通知处理程序代码
	//把界面的输入 更新到C++变量中
	UpdateData(TRUE);

	CWnd		*myWnd = NULL;
	CButton		*But = NULL;

	//通过一个物理资源，直接得到一个对象的指针
	myWnd = CWnd::GetDlgItem(IDC_CHECK_TIME);

	But = (CButton*)myWnd;

	if (But->GetCheck()==BST_UNCHECKED)
	{
		AfxMessageBox("没有选择check复选框");
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
			AfxMessageBox("打开网点信息表失败");
			return;
		}
		//将控件里的数据全部删除
		m_listSecNode.DeleteAllItems();
		//设置  禁止m_listSecNode控件 重画
		m_listSecNode.SetRedraw(FALSE);
		while (!secmngSecNode.IsBOF())
		{
			
			if (!(secmngSecNode.m_CREATETIME >= sqlTime1 &&
				secmngSecNode.m_CREATETIME <= sqlTime2))
			{
				//如果不在这个时间段的数据，获取下一个数据，继续判断
				secmngSecNode.MoveNext();
				continue;
			}
			////如果有在这个时间段的数据，逐个取出挂在到界面上
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
	// TODO:  在此添加控件通知处理程序代码
	int						flag = 0;
	UpdateData(TRUE);

	//获取第一次选的位置
	POSITION pos = m_listSecNode.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		MessageBox("你没有选取记录, 请你选中其中的一条记录！", "修改网点信息", MB_MODEMASK);
		return;
	}
	//获取选取的第几行
	int  nItem = m_listSecNode.GetNextSelectedItem(pos);

	// 获取第几行，第0列的元素
	CString myId = m_listSecNode.GetItemText(nItem, 0);
	CString myname = m_listSecNode.GetItemText(nItem, 1);

	//获取选中行的行数
	int	selCont = m_listSecNode.GetSelectedCount();
	if (selCont > 1) {
		MessageBox("你选中了多行记录, 请你选中其中的一条记录！", "删除网点信息", MB_MODEMASK);
		return;
	}

	CString strTmp;
	strTmp.Format("是否要删除编号为【%s】的网点信息吗？", myId);
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
			AfxMessageBox("打开网点信息表失败");
			return;
		}

		if (!rsetMngSecNode.IsEOF())
		{
			rsetMngSecNode.Delete();
		}
		else
		{
			AfxMessageBox("根据网点编号 没有查找到对应记录 myid:" + myId);
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
		m_listSecNode.DeleteItem(nItem); //从界面上删除数据
		AfxMessageBox("删除数据ok");

	}
	if (rsetMngSecNode.IsOpen())
	{
		rsetMngSecNode.Close();
	}

}
