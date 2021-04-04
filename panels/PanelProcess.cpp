#include "../ginc.h"
#include "PanelProcess.h"
#include "./views/process/TotalProcessListView.h"
#include "./views/process/CPUProcessListView.h"
#include "./views/process/MemProcessListView.h"
#include "./views/memory/MemorySpectrumView.h"

#include "../system/process/ProcessMonitoring.h"
#include "../system/memory/MemoryMonitoring.h"

CPanelProcess::CPanelProcess(wxWindow* _parent)
	: wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN)
{
	InitPanel();
}

CPanelProcess::~CPanelProcess()
{
}

void CPanelProcess::InitPanel()
{
//	wxBusyInfo wait("Please wait, Process Information gethering...");
	wxBusyInfo info
    (
        wxBusyInfoFlags()
            .Parent(this)
            .Icon(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE,
                                         wxART_OTHER, wxSize(64, 64)))
            .Title("<b>Process Information gathering...</b>")
            .Text("please wait...")
            .Foreground(*wxWHITE)
            .Background(wxColour(100, 100, 100))
            .Transparency(5 * (wxALPHA_OPAQUE / 5))
    );
	//프로세스 모니터링
	m_pProcessMonitor = std::make_unique<CProcessMonitoring>();
	//메모리 모니터링
	m_pMemoryMonitor = std::make_unique<CMemoryMonitoring>();

	//모니터링 자원 등록
	theSystem->SetMonitoringResource(m_pProcessMonitor.get(), S_BASE_CLASS_PROCESS);
	theSystem->SetMonitoringResource(m_pMemoryMonitor.get(), S_BASE_CLASS_MEMORY);
	m_pTotalProcessList = std::make_unique<CTotalProcessListView>(this);

	m_pProcessMonitor->AddItem(m_pTotalProcessList.get());

	wxBoxSizer* m_sizerProcessMain;
	m_sizerProcessMain = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* m_sizerListMain;
	m_sizerListMain = new wxBoxSizer( wxHORIZONTAL );
	m_sizerListMain->Add( m_pTotalProcessList.get(), 1, wxEXPAND, 5 );

	wxBoxSizer* m_sizerList;
	m_sizerList = new wxBoxSizer( wxVERTICAL );

	m_staticCPU = new wxStaticText( this, wxID_ANY, wxT("CPU"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_staticCPU->Wrap( -1 );
	m_staticCPU->SetBackgroundColour( wxColour( 236, 227, 251 ) );

	m_sizerList->Add( m_staticCPU, 0, wxALL|wxEXPAND, 5 );

	int iCPUMemSize =  theSettings->GetCPUMemListWidth();
	m_pCPUProcessList = std::make_unique<CCPUProcessListView>(this, wxSize(iCPUMemSize, -1));
	m_sizerList->Add( m_pCPUProcessList.get(), 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_pProcessMonitor->AddItem(m_pCPUProcessList.get());

	float fPhysicalTotalMem = (float)theSystem->GetTotalMemory() / 1000.0f;
	wxString strTotalMem = wxString::Format(wxT("Memory ( 전체 %3.2fGB )"), fPhysicalTotalMem);
	m_staticMemory = new wxStaticText( this, wxID_ANY, strTotalMem, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_staticMemory->Wrap( -1 );
	m_staticMemory->SetBackgroundColour( wxColour( 201, 253, 254 ) );

	m_sizerList->Add( m_staticMemory, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* m_sizerGraphAll;
	m_sizerGraphAll = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* m_sizerMemGraph;
	m_sizerMemGraph = new wxBoxSizer( wxHORIZONTAL );

	m_pMemSpectrumView = std::make_unique<CMemorySpectrumView>(this, wxSize(-1, 20));
	m_sizerMemGraph->Add( m_pMemSpectrumView.get(), 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	//모니터링 notify 윈도우 등록
	m_pMemoryMonitor->AddItem(m_pMemSpectrumView.get());

	m_sizerGraphAll->Add( m_sizerMemGraph, 0, wxBOTTOM|wxEXPAND, 5 );
	m_sizerList->Add( m_sizerGraphAll, 0, wxEXPAND, 5 );

	m_pMemoryProcessList = std::make_unique<CMemProcessListView>(this, wxSize(iCPUMemSize, -1));
	m_sizerList->Add( m_pMemoryProcessList.get(), 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_pProcessMonitor->AddItem(m_pMemoryProcessList.get());

	m_sizerListMain->Add( m_sizerList, 0, wxEXPAND, 5 );
	m_sizerProcessMain->Add( m_sizerListMain, 1, wxEXPAND, 5 );

	this->SetSizer( m_sizerProcessMain );
	this->Layout();

	CProcessListViewCommon::SetProcessLists();

	m_pTotalProcessList->InitializeProcessList(eTotalList);
	m_pCPUProcessList->InitializeProcessList(eCPUList);
	m_pMemoryProcessList->InitializeProcessList(eMemoryList);
	 //모니터링 시작
	m_pProcessMonitor->StartMonitoring();
	m_pMemoryMonitor->StartMonitoring();
}

void CPanelProcess::SetSelectedItem(const wxString& strSelProcessID)
{
	m_pTotalProcessList->SetItemSelected(strSelProcessID);
}
