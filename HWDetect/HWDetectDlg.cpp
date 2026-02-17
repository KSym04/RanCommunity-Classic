
// HWDetectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./HWDetect.h"
#include "./HWDetectDlg.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/RPCID/RPCID.h"
#include "../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../Lib_Helper/RPCID/RPCIDMAC.h"
#include "../Lib_Helper/SMBiosParser/SMBiosParser.h"

#include "../Lib_Engine/Hash/MD52.h"
#include "../Lib_Engine/Hash/CHash.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHWDetectDlg dialog


CHWDetectDlg::CHWDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHWDetectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHWDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_cEdit);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_cSelect );
}

BEGIN_MESSAGE_MAP(CHWDetectDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CHWDetectDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHWDetectDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CHWDetectDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CHWDetectDlg message handlers

BOOL CHWDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_cEdit.SetLimitText(0x7FFFFFFE);

	const int nSelect = 4;
	std::string strComboSelect[nSelect] = 
	{
		"PCID Basic",
		"SMBIOS",
		"PCID SMBIOS",
		"PCID MAC",
	};

	for ( int i=0; i<nSelect; ++i )
	{
		m_cSelect.InsertString( i, strComboSelect[i].c_str() );
		m_cSelect.SetItemData( i, i );
	}

	if ( m_cSelect.GetCount() > 0 )
	{
		m_cSelect.SetCurSel(0);
	}

	InfoReset();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHWDetectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHWDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHWDetectDlg::InfoAdd( const char *szFormat, ... )
{
	va_list argList;
	char szBuffer[256] = "";
	va_start(argList, szFormat);
	StringCbVPrintf( szBuffer, 256, szFormat, argList );
	va_end(argList);

	int nlen = m_cEdit.GetWindowTextLength();

	if ( m_cEdit.GetWindowTextLength() >= (int)m_cEdit.GetLimitText() )
	{
		m_cEdit.SetSel( 0 , m_cEdit.GetWindowTextLength() );
	}else{
		m_cEdit.SetSel( nlen, nlen );
	}

	m_cEdit.ReplaceSel( szBuffer );	

	nlen = m_cEdit.GetWindowTextLength();
	m_cEdit.SetSel( nlen, nlen );
	m_cEdit.ReplaceSel( "\r\n" );
}

void CHWDetectDlg::InfoReset()
{
	m_cEdit.SetWindowTextA( "" );
}

void CHWDetectDlg::InfoPCIDBasic()
{
	std::string strText = "";
	std::string strHash = "";

	InfoAdd( "==============================================================");
	strText = RPCID::getStringCPU();
	InfoAdd( "CPU String:%s", strText.c_str() );
	strHash = md5( strText );
	InfoAdd( "CPU Hash:%s", strHash.c_str() );

	InfoAdd( "==============================================================");
	strText = RPCID::getStringLAN();
	InfoAdd( "LAN String:%s", strText.c_str() );
	strHash = md5( strText );
	InfoAdd( "LAN Hash:%s", strHash.c_str() );

	InfoAdd( "==============================================================");
	strText = RPCID::getStringDISK();
	InfoAdd( "DISK String:%s", strText.c_str() );
	strHash = md5( strText );
	InfoAdd( "DISK Hash:%s", strHash.c_str() );

	InfoAdd( "==============================================================");
	strText = RPCID::getStringPCNAME();
	InfoAdd( "PCNAME String:%s", strText.c_str() );
	strHash = md5( strText );
	InfoAdd( "PCNAME Hash:%s", strHash.c_str() );

	InfoAdd( "==============================================================");
	strText = RPCID::getStringCombined();
	InfoAdd( "ALL String:%s", strText.c_str() );
	strHash = md5( strText );
	InfoAdd( "ALL Hash:%s", strHash.c_str() );
}

void CHWDetectDlg::InfoSMBIOS()
{
	std::vector<uint8_t> buffer;

	const BYTE byteSignature[] = { 'B', 'M', 'S', 'R' };
	const DWORD signature = *((DWORD*)byteSignature);

	// get the size of SMBIOS table
	DWORD size = GetSystemFirmwareTable(signature, 0, NULL, 0);
	if (size == 0)		return;

	buffer.resize(size, 0);
	// retrieve the SMBIOS table

	if (size != GetSystemFirmwareTable(signature, 0, buffer.data(), size))
	{
		buffer.clear();
		return;
	}

	smbios::Parser parser(&buffer.front(), buffer.size());

	int version = parser.version();
	const smbios::Entry *entry = NULL;
	while (true)
	{
		entry = parser.next();
		if (entry == NULL) break;

		if (entry->type == DMI_TYPE_BIOS)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "BIOS Vendor: %s", entry->data.bios.Vendor );
				InfoAdd( "BIOS Version: %s", entry->data.bios.BIOSVersion );
				InfoAdd( "BIOS StartingSegment: 0x%X",entry->data.bios.BIOSStartingSegment );
				InfoAdd( "BIOS ReleaseDate: %s",entry->data.bios.BIOSReleaseDate );
				InfoAdd( "BIOS ROMSize: %u", (((int) entry->data.bios.BIOSROMSize + 1) * 64) );
			}
			if (version >= smbios::SMBIOS_2_4)
			{
				InfoAdd( "BIOS SystemBIOSMajorRelease: %u",(int) entry->data.bios.SystemBIOSMajorRelease );
				InfoAdd( "BIOS SystemBIOSMinorRelease: %u",(int) entry->data.bios.SystemBIOSMinorRelease );
				InfoAdd( "BIOS EmbeddedFirmwareMajorRelease: %u", (int) entry->data.bios.EmbeddedFirmwareMajorRelease );
				InfoAdd( "BIOS EmbeddedFirmwareMinorRelease: %u", (int) entry->data.bios.EmbeddedFirmwareMinorRelease );
			}

		}

		if (entry->type == DMI_TYPE_SYSINFO)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "SysInfo Manufacturer: %s", entry->data.sysinfo.Manufacturer );
				InfoAdd( "SysInfo ProductName: %s", entry->data.sysinfo.ProductName );
				InfoAdd( "SysInfo Version: %s", entry->data.sysinfo.Version  );
				InfoAdd( "SysInfo SerialNumber: %s", entry->data.sysinfo.SerialNumber );
			}

			if (version >= smbios::SMBIOS_2_1)
			{
				InfoAdd( "SysInfo UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
					entry->data.sysinfo.UUID[0], entry->data.sysinfo.UUID[1], entry->data.sysinfo.UUID[2], entry->data.sysinfo.UUID[3],
					entry->data.sysinfo.UUID[4], entry->data.sysinfo.UUID[5], entry->data.sysinfo.UUID[6], entry->data.sysinfo.UUID[7],
					entry->data.sysinfo.UUID[8], entry->data.sysinfo.UUID[9], entry->data.sysinfo.UUID[10], entry->data.sysinfo.UUID[11],
					entry->data.sysinfo.UUID[12],entry->data.sysinfo.UUID[13], entry->data.sysinfo.UUID[14], entry->data.sysinfo.UUID[15]);
			}

			if (version >= smbios::SMBIOS_2_4)
			{
				InfoAdd( "SysInfo SKUNumber: %s", entry->data.sysinfo.SKUNumber );
				InfoAdd( "SysInfo Family: %s", entry->data.sysinfo.Family );
			}
		}

		if (entry->type == DMI_TYPE_BASEBOARD)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "Board Manufacturer: %s", entry->data.baseboard.Manufacturer );
				InfoAdd( "Board Product: %s", entry->data.baseboard.Product );
				InfoAdd( "Board Version: %s", entry->data.baseboard.Version );
				InfoAdd( "Board SerialNumber: %s", entry->data.baseboard.SerialNumber );
				InfoAdd( "Board AssetTag: %s", entry->data.baseboard.AssetTag );
				InfoAdd( "Board LocationInChassis: %s", entry->data.baseboard.LocationInChassis );
				InfoAdd( "Board ChassisHandle: %u", (int) entry->data.baseboard.ChassisHandle );
				InfoAdd( "Board Type: %u", (int) entry->data.baseboard.BoardType );
			}
		}

		if (entry->type == DMI_TYPE_SYSENCLOSURE)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "Enclosure Manufacturer: %s", entry->data.sysenclosure.Manufacturer );
				InfoAdd( "Enclosure Version: %s", entry->data.sysenclosure.Version );
				InfoAdd( "Enclosure SerialNumber: %s", entry->data.sysenclosure.SerialNumber );
				InfoAdd( "Enclosure AssetTag: %s", entry->data.sysenclosure.AssetTag );
			}
			if (version >= smbios::SMBIOS_2_3)
			{
				InfoAdd( "Enclosure Contained Count: %u", (int) entry->data.sysenclosure.ContainedElementCount );
				InfoAdd( "Enclosure Contained Length: %u", (int) entry->data.sysenclosure.ContainedElementRecordLength );
			}
			if (version >= smbios::SMBIOS_2_7)
			{
				InfoAdd( "Enclosure SKUNumber: %s", entry->data.sysenclosure.SKUNumber );
			}
		}

		if (entry->type == DMI_TYPE_PROCESSOR)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "Processor SocketDesignation: %s", entry->data.processor.SocketDesignation );
				InfoAdd( "Processor Family: %u", (int) entry->data.processor.ProcessorFamily );
				InfoAdd( "Processor Manufacturer: %s", entry->data.processor.ProcessorManufacturer );
				InfoAdd( "Processor Version: %s", entry->data.processor.ProcessorVersion );

				InfoAdd( "Processor ID: %02X%02X%02X%02X%02X%02X%02X%02X\n",
					(int) entry->data.processor.ProcessorID[0], (int) entry->data.processor.ProcessorID[1], 
					(int) entry->data.processor.ProcessorID[2], (int) entry->data.processor.ProcessorID[3], 
					(int) entry->data.processor.ProcessorID[4], (int) entry->data.processor.ProcessorID[5], 
					(int) entry->data.processor.ProcessorID[6], (int) entry->data.processor.ProcessorID[7] );
			}

			if (version >= smbios::SMBIOS_2_5)
			{
				InfoAdd( "Processor CoreCount: %u", (int) entry->data.processor.CoreCount );
				InfoAdd( "Processor CoreEnabled: %u", (int) entry->data.processor.CoreEnabled );
				InfoAdd( "Processor ThreadCount: %u", (int) entry->data.processor.ThreadCount );
			}

			if (version >= smbios::SMBIOS_2_6)
			{
				InfoAdd( "Processor ProcessorFamily2: %u", (int)entry->data.processor.ProcessorFamily2 );
			}
		}

		/*if (entry->type == DMI_TYPE_SYSSLOT)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_0)
			{
				InfoAdd( "System Slot Designation: %s", entry->data.sysslot.SlotDesignation );
				InfoAdd( "System Slot Type: %u", (int) entry->data.sysslot.SlotType );
				InfoAdd( "System Slot DataBusWidth: %u", (int) entry->data.sysslot.SlotDataBusWidth );
				InfoAdd( "System Slot ID: %u", (int) entry->data.sysslot.SlotID );
			}
			if (version >= smbios::SMBIOS_2_6)
			{
				InfoAdd( "System Slot SegmentGroupNumber: %u", (int) entry->data.sysslot.SegmentGroupNumber );
				InfoAdd( "System Slot BusNumber: %u", (int) entry->data.sysslot.BusNumber );
			}
		}*/

		/*if (entry->type == DMI_TYPE_PHYSMEM)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_1)
			{
				InfoAdd( "Physical Memory Use: 0x%X", (int) entry->data.physmem.Use );
				InfoAdd( "Physical Memory NumberDevices: %u", (int)entry->data.physmem.NumberDevices );
				InfoAdd( "Physical Memory MaximumCapacity: %u KiB", (int)entry->data.physmem.MaximumCapacity );
				InfoAdd( "Physical Memory ExtMaximumCapacity: %I64u KiB", entry->data.physmem.ExtendedMaximumCapacity );
			}
		}*/

		if (entry->type == DMI_TYPE_MEMORY)
		{
			InfoAdd( "==============================================================");
			InfoAdd( "DMI Handle: 0x%X DMI Type: %u DMI Size: %u", (int) entry->handle, (int) entry->type, entry->length );

			if (version >= smbios::SMBIOS_2_1)
			{
				InfoAdd( "Memory DeviceLocator: %s", entry->data.memory.DeviceLocator );
				InfoAdd( "Memory BankLocator: %s", entry->data.memory.BankLocator );
			}
			if (version >= smbios::SMBIOS_2_3)
			{
				InfoAdd( "Memory Speed: %u MHz", entry->data.memory.Speed );
				InfoAdd( "Memory Manufacturer: %s", entry->data.memory.Manufacturer );
				InfoAdd( "Memory SerialNumber: %s", entry->data.memory.SerialNumber );
				InfoAdd( "Memory AssetTagNumber: %s", entry->data.memory.AssetTagNumber );
				InfoAdd( "Memory PartNumber: %s", entry->data.memory.PartNumber );
				InfoAdd( "Memory Size: %u MiB", entry->data.memory.Size );
				InfoAdd( "Memory ExtendedSize: %u MiB", entry->data.memory.ExtendedSize );
			}
			if (version >= smbios::SMBIOS_2_7)
			{
				InfoAdd( "Memory ConfiguredClockSpeed: %u MHz", entry->data.memory.ConfiguredClockSpeed );
			}
		}







	}
}


void CHWDetectDlg::InfoPCIDSMBIOS()
{
	std::string strText = "";
	std::string strHash = "";

	SPCID_SMBIOS sPCID_SMBIOS;
	bool b = RPCID::getSMBIOS( sPCID_SMBIOS );
	if ( b )
	{
		InfoAdd( "==============================================================");
		InfoAdd( "System UUID:%s", sPCID_SMBIOS.strSystemUUID.c_str() );
		InfoAdd( "System UUID Hash:%s", RPCID::getHashSMBiosSystemUUID(sPCID_SMBIOS).c_str() );
		InfoAdd( "==============================================================");
		InfoAdd( "Board Serial:%s", sPCID_SMBIOS.strBoardSerialNumber.c_str() );
		InfoAdd( "Board Serial Hash:%s", RPCID::getHashSMBiosBoardSerial(sPCID_SMBIOS).c_str() );
		InfoAdd( "==============================================================");
		InfoAdd( "CPU Version:%s", sPCID_SMBIOS.strProcessorVersion.c_str() );
		InfoAdd( "CPU Version Hash:%s", RPCID::getHashSMBiosProcessorVersion(sPCID_SMBIOS).c_str() );
		InfoAdd( "==============================================================");
		InfoAdd( "CPU ID:%s", sPCID_SMBIOS.strProcessorID.c_str() );
		InfoAdd( "CPU ID Hash:%s", RPCID::getHashSMBiosProcessorID(sPCID_SMBIOS).c_str() );
		InfoAdd( "==============================================================");
		InfoAdd( "ALL Hash:%s", RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str() );
	}
	

}

void CHWDetectDlg::InfoPCIDMAC()
{
	InfoAdd( "==============================================================");
	InfoAdd( "Windows MAC:%s", RPCID::getWindowsMAC().c_str() );
	InfoAdd( "Windows MAC HASH:%s", RPCID::getHashWindowsMAC().c_str() );
}

void CHWDetectDlg::OnBnClickedButtonRun()
{
	int nIndex = m_cSelect.GetCurSel();
	if ( nIndex == CB_ERR )	return;

	switch ( nIndex )
	{
	case 0:
		InfoPCIDBasic();
		break;

	case 1:
		InfoSMBIOS();
		break;

	case 2:
		InfoPCIDSMBIOS();
		break;

	case 3:
		InfoPCIDMAC();
		break;
	};
}

void CHWDetectDlg::OnBnClickedButtonSave()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	CString strAppPath = szPath;

	if ( !strAppPath.IsEmpty() )
	{
		DWORD dwFind = strAppPath.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			CString strAppPath2 = strAppPath.Left ( dwFind );

			if ( !strAppPath2.IsEmpty() )
				if ( strAppPath2.GetAt(0) == '"' )
					strAppPath2 = strAppPath2.Right ( strAppPath2.GetLength() - 1 );

			strAppPath = strAppPath2;
		}
	}

	std::string filepath = strAppPath;
	filepath += "\\";
	filepath += "hwdetect.txt";

	FILE* file=NULL;
	file = fopen ( filepath.c_str(), "wt" );
	if ( !file )	return;

	CString strText;
	m_cEdit.GetWindowText(strText);
	fprintf ( file, "%s", strText.GetString() );

	fclose ( file );

	MessageBox( filepath.c_str(), "saved" );

	
}

void CHWDetectDlg::OnBnClickedButtonClear()
{
	InfoReset();
}
