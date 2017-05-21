# IPViewer
Local IP-collecting MFC application for Windows.  
  
Precompiled binary executable file is in Release folder.  
There are no significant differences between x86 and x64.  
  
User-defined function GetNetworkAddress is called on OnInitDialog of the dialog.  
GetNetworkAddress calls **WSAStartup** & **WSACleanup** (winsock2), **GetIpAddrTable** (iphlpapi), **_stprintf_s** (corecrt_wstdio), and some MFC-element functions.
  
This project is created by Microsoft Visual Studio 2017 Community edition.
