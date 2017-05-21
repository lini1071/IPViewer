
// IPViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "IPViewer.h"
#include "IPViewerDlg.h"
#include "afxdialogex.h"


// winsock2, IP helper
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIPViewerDlg 대화 상자



CIPViewerDlg::CIPViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IPVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editbox[0]);
	DDX_Control(pDX, IDC_EDIT2, m_editbox[1]);
	DDX_Control(pDX, IDC_EDIT3, m_editbox[2]);
}

BEGIN_MESSAGE_MAP(CIPViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CIPViewerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIPViewerDlg 메시지 처리기

BOOL CIPViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	this->GetNetworkAddress();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIPViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIPViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIPViewerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CIPViewerDlg::GetNetworkAddress()
{
	int iResult;
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// If an error occurred, refuse continue of application.
	if (iResult)
	{
		// LPTSTR is not constant, LPCTSTR is constant.
		LPTSTR ptrMsg;
		LPCTSTR errMsg = L"WSAStartup failed: ";

		size_t errMaxMsgLength, baseErrMsgLength;
		const size_t wsaErrMaxLength = 6; // 5 + 1 (for null-terminating)

		baseErrMsgLength = _tcsclen(errMsg);
		errMaxMsgLength = baseErrMsgLength + wsaErrMaxLength;
		ptrMsg = new TCHAR[errMaxMsgLength];

		_tcscpy_s(ptrMsg, errMaxMsgLength, errMsg);
		_stprintf_s(ptrMsg + baseErrMsgLength, errMaxMsgLength, L"%d", iResult);
		MessageBox(ptrMsg, L"Error", MB_ICONERROR | MB_OK);
		
		delete[] ptrMsg;
		this->OnCancel();
	}

	// Else, get IP address of the device and print it.
	else
	{
		// First, get the IP address.
		MIB_IPADDRTABLE	*pIPAddrTable;
		DWORD			dwSize = 0;
		DWORD			dwRetVal;

		pIPAddrTable = (MIB_IPADDRTABLE *) malloc(sizeof(MIB_IPADDRTABLE));
		if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
		{
			free(pIPAddrTable);
			pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);
			if ((dwRetVal = GetIpAddrTable((MIB_IPADDRTABLE *) pIPAddrTable, &dwSize, 0)) != NO_ERROR)
			{
				LPTSTR ptrMsg;
				LPCTSTR errMsg = L"GetIpAddrTable call failed with ";

				size_t errMaxMsgLength, baseErrMsgLength;
				const size_t wsaErrMaxLength = 6; // 5 + 1 (for null-terminating)

				baseErrMsgLength = _tcsclen(errMsg);
				errMaxMsgLength = baseErrMsgLength + wsaErrMaxLength;
				ptrMsg = new TCHAR[errMaxMsgLength];

				_tcscpy_s(ptrMsg, errMaxMsgLength, errMsg);
				_stprintf_s(ptrMsg + baseErrMsgLength, errMaxMsgLength, L"%d", iResult);
				MessageBox(ptrMsg, L"Error", MB_ICONERROR | MB_OK);
			}
			else if (pIPAddrTable != NULL)
			{
				// At this point, IP address is stored in 'pIPAddrTable->table[0].dwAddr'.
				// Then we are going to convert this value to character string.
				size_t	i, p;
				DWORD	dwIPAddrVal = pIPAddrTable->table[0].dwAddr;
				unsigned char	valBuf[sizeof(dwIPAddrVal)];
				TCHAR			strPtr[16];

				// Update editboxs' text.
				UpdateData(TRUE);

				_stprintf_s(strPtr, sizeof(strPtr) / sizeof(*strPtr), L"%u", dwIPAddrVal);
				this->m_editbox[1].SetWindowText(strPtr);

				_stprintf_s(strPtr, sizeof(strPtr) / sizeof(*strPtr), L"%x", dwIPAddrVal);
				this->m_editbox[2].SetWindowText(strPtr);

				for (i = 0 ; i < sizeof(dwIPAddrVal) ; i++)
				{
					valBuf[i] = (dwIPAddrVal & 0xFF);
					dwIPAddrVal >>= 8;
				};
				for (i = 0, p = 0 ; i < sizeof(dwIPAddrVal) ; i++)
				{
					p += _stprintf_s(strPtr + p, 16 - p,
						(sizeof(dwIPAddrVal) - 1) - i ? L"%hhu." : L"%hhu", valBuf[i]);
				};
				this->m_editbox[0].SetWindowText(strPtr);

				UpdateData(FALSE);
			};
		};

		free(pIPAddrTable);
		WSACleanup();
	}
}