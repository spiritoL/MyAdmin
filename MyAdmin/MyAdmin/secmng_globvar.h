
//  hpp		.h .cpp
// secmng_globvar.h

//1 ��֤ȫ�ֱ��� ��������ֻ����һ�� �����ڴ�һ��
//2 ͷ�ļ���ͬһ��cpp�б���������  ��  ͷ�ļ�������ط����� ��������ͬ������

#ifndef SECMNG_GLOB_VAR_H_
#define SECMNG_GLOB_VAR_H_

#include"stdafx.h"

//����Դȫ�ֱ���
CString	g_dbSource, g_dbUse, g_dbpw;
 
CDatabase	myDB;
CDatabase	*g_pDB = NULL;

CString		g_SecMngIniName = "secmngadmin.ini";


#endif