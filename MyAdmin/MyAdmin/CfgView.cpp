// CfgView.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "CfgView.h"
#include "SECMNGSRVCFG.h"


// CCfgView

IMPLEMENT_DYNCREATE(CCfgView, CFormView)

CCfgView::CCfgView()
	: CFormView(CCfgView::IDD)
	, m_strsrvip(_T(""))
	, m_strsrvport(_T(""))
	, m_strsrvmaxnodenum(_T(""))
{

}

CCfgView::~CCfgView()
{
}

void CCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERIP, m_strsrvip);
	//  DDX_Control(pDX, IDC_EDIT_SERMAXNODE, m_strsrvmaxnodenum);
	DDX_Text(pDX, IDC_EDIT_SERPORT, m_strsrvport);
	DDX_Text(pDX, IDC_EDIT_SERMAXNODE, m_strsrvmaxnodenum);
}

BEGIN_MESSAGE_MAP(CCfgView, CFormView)
	
	ON_BN_CLICKED(IDC_BUTTON_SERINFOSAVE, &CCfgView::OnBnClickedButtonSerinfosave)
END_MESSAGE_MAP()


// CCfgView 诊断

#ifdef _DEBUG
void CCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCfgView 消息处理程序
extern CDatabase	*g_pDB;



void CCfgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSECMNGSRVCFG srvCfgSet(g_pDB);

	//用记录类打开一张表 //相当于: select * from secmng.srvcfg where key='secmng_server_ip'

	//反复查询数据
	TRY
	{
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		if (!srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开表 err");
			return;
		}
		if (!srvCfgSet.IsEOF()) //有记录 
		{
			m_strsrvip = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strsrvip = "";
		}


		//反复查询
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();

		if (!srvCfgSet.IsEOF()) //有记录 
		{
			m_strsrvport = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strsrvport = "";
		}

		//反复查询 secmng_server_maxnetnum
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();

		if (!srvCfgSet.IsEOF()) //有记录 
		{
			m_strsrvmaxnodenum = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strsrvmaxnodenum = "";
		}



		if (srvCfgSet.IsOpen())
		{
			srvCfgSet.Close();
		}


	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

	UpdateData(FALSE);
}


//1 若记录存在 则修改  
//2 若记录不存在 则新增
void CCfgView::OnBnClickedButtonSerinfosave()
{
	UpdateData(TRUE);
	int		iTag = 0; //0 正常 
	if (m_strsrvip.IsEmpty() ||
		m_strsrvmaxnodenum.IsEmpty() ||
		m_strsrvport.IsEmpty())
	{
		AfxMessageBox("用户输入不能为空");
		return;
	}

	CSECMNGSRVCFG srvCfgSet(g_pDB);
	g_pDB->BeginTrans();
	TRY
	{
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		if (!srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			AfxMessageBox("打开表 err");
			goto MyEnd;
		}
		if (!srvCfgSet.IsEOF()) //有记录 
		{
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strsrvip;
			srvCfgSet.Update();

		}
		else
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strsrvip;
			srvCfgSet.Update();
		}


		//反复查询
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();

		if (!srvCfgSet.IsEOF()) //有记录 
		{
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strsrvport;
			srvCfgSet.Update();
		}
		else
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strsrvport;
			srvCfgSet.Update();
		}

		//反复查询 secmng_server_maxnetnum
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();

		if (!srvCfgSet.IsEOF()) //有记录 
		{
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_maxnetnum";
			srvCfgSet.m_VALUDE = m_strsrvmaxnodenum;
			srvCfgSet.Update();
		}
		else
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_maxnetnum";
			srvCfgSet.m_VALUDE = m_strsrvmaxnodenum;
			srvCfgSet.Update();
		}
	}
	CATCH_ALL(e)
	{
		iTag = 1;
		e->ReportError();
	}
	END_CATCH_ALL

	MyEnd :
	if (iTag == 1)
	{
		g_pDB->Rollback();
		AfxMessageBox("保存数据失败");
	}
	else
	{
		g_pDB->CommitTrans();
		AfxMessageBox("保存数据ok");
	}

	if (srvCfgSet.IsOpen())
	{
		srvCfgSet.Close();
	}
}



