
// IPViewerDlg.cpp : ���� ����
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


// CIPViewerDlg ��ȭ ����



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


// CIPViewerDlg �޽��� ó����

BOOL CIPViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	this->GetNetworkAddress();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CIPViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CIPViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIPViewerDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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