#include "../ginc.h"
#include "PanelCPU.h"
#include "./views/ViewCommon.h"
#include "./views/cpu/CPUUsageGraphView.h"
#include "./views/cpu/CPUSVSegmentView.h"
#include "../system/cpu/CPUMonitoring.h"

CPanelCPU::CPanelCPU(wxWindow* _parent)
	: wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN)
{
	InitPanel();
}

CPanelCPU::~CPanelCPU()
{
	wxVector<CViewCommon *>::iterator iter = m_vecViewCPUCore.begin();
	while(iter != m_vecViewCPUCore.end())
	{
		CViewCommon* pView = *iter;
		delete pView;

		pView = nullptr;

		++iter;
	}

	m_vecViewCPUCore.clear();
}

void CPanelCPU::InitPanel()
{
	int iSevenSegmentWidth = 80;
	wxBusyInfo info
    (
        wxBusyInfoFlags()
            .Parent(this)
            .Icon(wxArtProvider::GetIcon(wxART_HARDDISK,
                                         wxART_OTHER, wxSize(64, 64)))
            .Title("<b>CPU Information gathering...</b>")
            .Text("please wait...")
            .Foreground(*wxWHITE)
            .Background(wxColour(100, 100, 100))
            .Transparency(5 * wxALPHA_OPAQUE / 5)
    );

	//CPU모니터링 클래스 생성
	m_pCPUMonitor = std::make_unique<CCPUMonitoring>();
	//시스템 항목등록
//	theSystem->SetCPUInfo(m_pCPUMonitor.get());
	theSystem->SetMonitoringResource(m_pCPUMonitor.get(), S_BASE_CLASS_CPU);

	wxBoxSizer* sizer_CPU;
	sizer_CPU = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizer_CPUTotal;
	sizer_CPUTotal = new wxBoxSizer( wxHORIZONTAL );

	m_viewCPUTotal = new CCPUUsageGraphView(this, wxSize(-1, theSettings->GetCPUViewHeignt()));
	m_viewCPUTotal->SetDispText(wxT("Total"));
	m_viewCPUTotal->SetCPUCoreNumber(0);

	m_viewCPUSVSegment = new CCPUSVSegmentView(this, wxSize(iSevenSegmentWidth, -1));
	m_viewCPUSVSegment->SetCPUCoreNumber(0);

	m_vecViewCPUCore.push_back(m_viewCPUTotal);
	m_vecViewCPUCore.push_back(m_viewCPUSVSegment);

	m_pCPUMonitor->AddItem(m_viewCPUTotal);
	m_pCPUMonitor->AddItem(m_viewCPUSVSegment);

	sizer_CPUTotal->Add( m_viewCPUTotal, 1, wxEXPAND | wxBOTTOM | wxRIGHT | wxTOP, 5 );
	sizer_CPUTotal->Add( m_viewCPUSVSegment, 0, wxEXPAND | wxBOTTOM | wxTOP, 5 );

	sizer_CPU->Add( sizer_CPUTotal, 1, wxEXPAND, 5 );

	wxBoxSizer* sizer_ONECPU;
	sizer_ONECPU = new wxBoxSizer( wxVERTICAL );

	int iCPUCoreDisplayInRow = theSettings->GetCPUCoreDisplayInRow();

	//CPU Core 수
	int iCpuCoreCount = m_pCPUMonitor->GetCPUCoreCount();
	//CPU Core 표시 행수
	int iCPUCoreRow = (iCpuCoreCount < iCPUCoreDisplayInRow) ? 1 : (iCpuCoreCount / iCPUCoreDisplayInRow);
	for(int iRow = 0; iRow < iCPUCoreRow; iRow++)
	{
		int iCPUCore = iCpuCoreCount < iCPUCoreDisplayInRow ? iCpuCoreCount :
							                      iRow == 0 ? iCPUCoreDisplayInRow : (iCpuCoreCount - (iRow * iCPUCoreDisplayInRow));

		wxBoxSizer* sizer_CPUCORE = new wxBoxSizer( wxHORIZONTAL );

		for(int i = 0; i < iCPUCore; i++)
		{
			wxString strDisp = wxT("Core ") + wxString::Format(wxT("%d"), ((iRow * iCPUCoreDisplayInRow) + i) + 1);

			CCPUUsageGraphView* pGraphView = new CCPUUsageGraphView(this, wxSize(-1, theSettings->GetCPUViewHeignt() - 5));
			pGraphView->SetDispText(strDisp);
			pGraphView->SetCPUCoreNumber(((iRow * iCPUCoreDisplayInRow) + i) + 1);

			CCPUSVSegmentView* pCPUSement = new CCPUSVSegmentView(this, wxSize(iSevenSegmentWidth, -1));
			pCPUSement->SetCPUCoreNumber(((iRow * iCPUCoreDisplayInRow) + i) + 1);

			int iSizerFlags = wxEXPAND | wxLEFT;
			if((i % iCPUCoreDisplayInRow) == 0)
				iSizerFlags &= ~wxLEFT;

			sizer_CPUCORE->Add(pGraphView, 1, iSizerFlags, 5);
			sizer_CPUCORE->Add(pCPUSement, 0, wxEXPAND | wxLEFT, 5);

			m_vecViewCPUCore.push_back(pGraphView);
			m_vecViewCPUCore.push_back(pCPUSement);

			m_pCPUMonitor->AddItem(pGraphView);
			m_pCPUMonitor->AddItem(pCPUSement);
		}

		int iCPURowSizerFlag = iRow != 0 ? wxEXPAND | wxTOP : wxEXPAND;
		sizer_ONECPU->Add( sizer_CPUCORE, 1, iCPURowSizerFlag, 5 );
	}

	sizer_CPU->Add( sizer_ONECPU, 0, wxEXPAND, 5 );

	this->SetSizer( sizer_CPU );
	this->Layout();

	m_pCPUMonitor->StartMonitoring();
}
