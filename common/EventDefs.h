#ifndef EVENTDEFS_H_INCLUDED
#define EVENTDEFS_H_INCLUDED

//그래프뷰 이벤트 처리
wxDECLARE_EVENT(wxEVT_EVENT_NOTIFY_GRAPHVIEW, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_EVENT_NOTIFY_ADD_DELPROCESS, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_EVENT_NOTIFY_UPDATE_PROCESSINFO, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_EVENT_PROCESS_RELOAD, wxCommandEvent);

#endif // EVENTDEFS_H_INCLUDED
