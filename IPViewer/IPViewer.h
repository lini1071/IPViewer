
// IPViewer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CIPViewerApp:
// �� Ŭ������ ������ ���ؼ��� IPViewer.cpp�� �����Ͻʽÿ�.
//

class CIPViewerApp : public CWinApp
{
public:
	CIPViewerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CIPViewerApp theApp;
