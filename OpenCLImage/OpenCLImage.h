
// OpenCLImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COpenCLImageApp: 
// �йش����ʵ�֣������ OpenCLImage.cpp
//

class COpenCLImageApp : public CWinApp
{
public:
	COpenCLImageApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COpenCLImageApp theApp;