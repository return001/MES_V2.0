#include "stdafx.h"
#include "Task.h"
#include "MFCP3SIMPORTDlg.h"
#include "ReSimDataDownload.h"

//全局变量
bool s_bSingleExit;//单串口主控线程
bool s_bExit;//多串口主控线程
bool s_bReExit;//返工位主控线程

//串口单文件下载
bool m_TreadPort1SINGLEDownloadWrite1 = true;
bool m_TreadPort1SINGLEDownloadWrite2 = true;
bool m_TreadPort1SINGLEDownloadWrite3 = true;
bool m_TreadPort1SINGLEDownloadWrite4 = true;
bool m_TreadPort1SINGLEDownloadRead1 = true;
bool m_TreadPort1SINGLEDownloadRead2 = true;
bool m_TreadPort1SINGLEDownloadRead3 = true;
bool m_TreadPort1SINGLEDownloadRead4 = true;

//串口1
bool m_TreadPort1DownloadWrite1 = true;
bool m_TreadPort1DownloadWrite2 = true;
bool m_TreadPort1DownloadWrite3 = true;
bool m_TreadPort1DownloadWrite4 = true;
bool m_TreadPort1DownloadRead1 = true;
bool m_TreadPort1DownloadRead2 = true;
bool m_TreadPort1DownloadRead3 = true;
bool m_TreadPort1DownloadRead4 = true;

//串口2
bool m_TreadPort2DownloadWrite1 = true;
bool m_TreadPort2DownloadWrite2 = true;
bool m_TreadPort2DownloadWrite3 = true;
bool m_TreadPort2DownloadWrite4 = true;
bool m_TreadPort2DownloadRead1 = true;
bool m_TreadPort2DownloadRead2 = true;
bool m_TreadPort2DownloadRead3 = true;
bool m_TreadPort2DownloadRead4 = true;

//串口3
bool m_TreadPort3DownloadWrite1 = true;
bool m_TreadPort3DownloadWrite2 = true;
bool m_TreadPort3DownloadWrite3 = true;
bool m_TreadPort3DownloadWrite4 = true;
bool m_TreadPort3DownloadRead1 = true;
bool m_TreadPort3DownloadRead2 = true;
bool m_TreadPort3DownloadRead3 = true;
bool m_TreadPort3DownloadRead4 = true;

//串口4
bool m_TreadPort4DownloadWrite1 = true;
bool m_TreadPort4DownloadWrite2 = true;
bool m_TreadPort4DownloadWrite3 = true;
bool m_TreadPort4DownloadWrite4 = true;
bool m_TreadPort4DownloadRead1 = true;
bool m_TreadPort4DownloadRead2 = true;
bool m_TreadPort4DownloadRead3 = true;
bool m_TreadPort4DownloadRead4 = true;

//返工位串口
bool m_TreadRePortDownloadWrite1 = true;
bool m_TreadRePortDownloadWrite2 = true;
bool m_TreadRePortDownloadRead1 = true;
bool m_TreadRePortDownloadRead2 = true;


CTask::CTask()
{
}

CTask::~CTask()
{
}

void CTask::Run()
{
	CMFCP3SIMPORTDlg mainform;
	CReSimDataDownload reDownloadform;
	CReSimDataDownload *CReSimDataDownload;

	switch (threadCommand)
	{
	case PORT_AUTO_THREAD:
		mainform.DownloadMainContralThread(AfxGetApp()->GetMainWnd());
		break;
	case PORT_REAUTO_THREAD:
		reDownloadform.ReDownloadMainThread(CReSimDataDownload->m_hWnd);
		break;
	//单文件下载
	case  PORT1_SINGLEWRITE1_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadWrite1)
		{
			m_TreadPort1SINGLEDownloadWrite1 = false;
			mainform.SingleDownloadWrite1Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case  PORT1_SINGLEWRITE2_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadWrite2)
		{
			m_TreadPort1SINGLEDownloadWrite2 = false;
			mainform.SingleDownloadWrite2Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case  PORT1_SINGLEWRITE3_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadWrite3)
		{
			m_TreadPort1SINGLEDownloadWrite3 = false;
			mainform.SingleDownloadWrite3Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case  PORT1_SINGLEWRITE4_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadWrite4)
		{
			m_TreadPort1SINGLEDownloadWrite4 = false;
			mainform.SingleDownloadWrite4Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_SINGLEREAD1_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadRead1)
		{
			m_TreadPort1SINGLEDownloadRead1 = false;
			mainform.SingleDownloadRead1Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_SINGLEREAD2_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadRead2)
		{
			m_TreadPort1SINGLEDownloadRead2 = false;
			mainform.SingleDownloadRead2Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_SINGLEREAD3_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadRead3)
		{
			m_TreadPort1SINGLEDownloadRead3 = false;
			mainform.SingleDownloadRead3Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_SINGLEREAD4_THREAD:
	{
		if (m_TreadPort1SINGLEDownloadRead4)
		{
			m_TreadPort1SINGLEDownloadRead4 = false;
			mainform.SingleDownloadRead4Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	//文件夹下载
	//串口1
	case PORT1_WRITE1_THREAD:
	{
		if (m_TreadPort1DownloadWrite1)
		{
			m_TreadPort1DownloadWrite1 = false;
			mainform.DownloadWrite1Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_WRITE2_THREAD:
	{
		if (m_TreadPort1DownloadWrite2)
		{
			m_TreadPort1DownloadWrite2 = false;
			mainform.DownloadWrite2Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_WRITE3_THREAD:
	{
		if (m_TreadPort1DownloadWrite3)
		{
			m_TreadPort1DownloadWrite3 = false;
			mainform.DownloadWrite3Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_WRITE4_THREAD:
	{
		if (m_TreadPort1DownloadWrite4)
		{
			m_TreadPort1DownloadWrite4 = false;
			mainform.DownloadWrite4Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_READ1_THREAD:
	{
		if (m_TreadPort1DownloadRead1)
		{
			m_TreadPort1DownloadRead1 = false;
			mainform.DownloadRead1Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_READ2_THREAD:
	{
		if (m_TreadPort1DownloadRead2)
		{
			m_TreadPort1DownloadRead2 = false;
			mainform.DownloadRead2Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_READ3_THREAD:
	{
		if (m_TreadPort1DownloadRead3)
		{
			m_TreadPort1DownloadRead3 = false;
			mainform.DownloadRead3Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT1_READ4_THREAD:
	{
		if (m_TreadPort1DownloadRead4)
		{
			m_TreadPort1DownloadRead4 = false;
			mainform.DownloadRead4Port1Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	//串口2
	case PORT2_WRITE1_THREAD:
	{
		if (m_TreadPort2DownloadWrite1)
		{
			m_TreadPort2DownloadWrite1 = false;
			mainform.DownloadWrite1Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_WRITE2_THREAD:
	{
		if (m_TreadPort2DownloadWrite2)
		{
			m_TreadPort2DownloadWrite2 = false;
			mainform.DownloadWrite2Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_WRITE3_THREAD:
	{
		if (m_TreadPort2DownloadWrite3)
		{
			m_TreadPort2DownloadWrite3 = false;
			mainform.DownloadWrite3Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_WRITE4_THREAD:
	{
		if (m_TreadPort2DownloadWrite4)
		{
			m_TreadPort2DownloadWrite4 = false;
			mainform.DownloadWrite4Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_READ1_THREAD:
	{
		if (m_TreadPort2DownloadRead1)
		{
			m_TreadPort2DownloadRead1 = false;
			mainform.DownloadRead1Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_READ2_THREAD:
	{
		if (m_TreadPort2DownloadRead2)
		{
			m_TreadPort2DownloadRead2 = false;
			mainform.DownloadRead2Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_READ3_THREAD:
	{
		if (m_TreadPort2DownloadRead3)
		{
			m_TreadPort2DownloadRead3 = false;
			mainform.DownloadRead3Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT2_READ4_THREAD:
	{
		if (m_TreadPort2DownloadRead4)
		{
			m_TreadPort2DownloadRead4 = false;
			mainform.DownloadRead4Port2Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	//串口3
	case PORT3_WRITE1_THREAD:
	{
		if (m_TreadPort3DownloadWrite1)
		{
			m_TreadPort3DownloadWrite1 = false;
			mainform.DownloadWrite1Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_WRITE2_THREAD:
	{
		if (m_TreadPort3DownloadWrite2)
		{
			m_TreadPort3DownloadWrite2 = false;
			mainform.DownloadWrite2Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_WRITE3_THREAD:
	{
		if (m_TreadPort3DownloadWrite3)
		{
			m_TreadPort3DownloadWrite3 = false;
			mainform.DownloadWrite3Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_WRITE4_THREAD:
	{
		if (m_TreadPort3DownloadWrite4)
		{
			m_TreadPort3DownloadWrite4 = false;
			mainform.DownloadWrite4Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_READ1_THREAD:
	{
		if (m_TreadPort3DownloadRead1)
		{
			m_TreadPort3DownloadRead1 = false;
			mainform.DownloadRead1Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_READ2_THREAD:
	{
		if (m_TreadPort3DownloadRead2)
		{
			m_TreadPort3DownloadRead2 = false;
			mainform.DownloadRead2Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_READ3_THREAD:
	{
		if (m_TreadPort3DownloadRead3)
		{
			m_TreadPort3DownloadRead3 = false;
			mainform.DownloadRead3Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT3_READ4_THREAD:
	{
		if (m_TreadPort3DownloadRead4)
		{
			m_TreadPort3DownloadRead4 = false;
			mainform.DownloadRead4Port3Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	//串口4
	case PORT4_WRITE1_THREAD:
	{
		if (m_TreadPort4DownloadWrite1)
		{
			m_TreadPort4DownloadWrite1 = false;
			mainform.DownloadWrite1Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_WRITE2_THREAD:
	{
		if (m_TreadPort4DownloadWrite2)
		{
			m_TreadPort4DownloadWrite2 = false;
			mainform.DownloadWrite2Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_WRITE3_THREAD:
	{
		if (m_TreadPort4DownloadWrite3)
		{
			m_TreadPort4DownloadWrite3 = false;
			mainform.DownloadWrite3Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_WRITE4_THREAD:
	{
		if (m_TreadPort4DownloadWrite4)
		{
			m_TreadPort4DownloadWrite4 = false;
			mainform.DownloadWrite4Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_READ1_THREAD:
	{
		if (m_TreadPort4DownloadRead1)
		{
			m_TreadPort4DownloadRead1 = false;
			mainform.DownloadRead1Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_READ2_THREAD:
	{
		if (m_TreadPort4DownloadRead2)
		{
			m_TreadPort4DownloadRead2 = false;
			mainform.DownloadRead2Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;
	case PORT4_READ3_THREAD:
	{
		if (m_TreadPort4DownloadRead3)
		{
			m_TreadPort4DownloadRead3 = false;
			mainform.DownloadRead3Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	case PORT4_READ4_THREAD:
	{
		if (m_TreadPort4DownloadRead4)
		{
			m_TreadPort4DownloadRead4 = false;
			mainform.DownloadRead4Port4Thread(AfxGetApp()->GetMainWnd());
		}
	}
	break;

	//返工串口
	case REPORT_WRITE1_THREAD:
	{
		if (m_TreadRePortDownloadWrite1)
		{
			m_TreadRePortDownloadWrite1 = false;
			reDownloadform.ReDownloadWrite1PortThread(CReSimDataDownload->m_hWnd);
		}
	}
	break;
	case REPORT_WRITE2_THREAD:
	{
		if (m_TreadRePortDownloadWrite2)
		{
			m_TreadRePortDownloadWrite2 = false;
			reDownloadform.ReDownloadWrite2PortThread(CReSimDataDownload->m_hWnd);
		}
	}
	break;
	case REPORT_READ1_THREAD:
	{
		if (m_TreadRePortDownloadRead1)
		{
			m_TreadRePortDownloadRead1 = false;
			reDownloadform.ReDownloadRead2PortThread(CReSimDataDownload->m_hWnd);
		}
	}
	break;
	case REPORT_READ2_THREAD:
	{
		if (m_TreadRePortDownloadRead2)
		{
			m_TreadRePortDownloadRead2 = false;
			reDownloadform.ReDownloadRead2PortThread(CReSimDataDownload->m_hWnd);
		}
	}
	break;

	default:
		break;
	}
}