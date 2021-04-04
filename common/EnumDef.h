#ifndef ENUMDEF_H_INCLUDED
#define ENUMDEF_H_INCLUDED

enum _SYSTEM_BASE_CLASS {
	S_BASE_CLASS_CPU = 0,
	S_BASE_CLASS_PROCESS,
	S_BASE_CLASS_MEMORY
};

enum eProcessListView {
	eTotalList = 0,
	eCPUList,
	eMemoryList,
};

#endif // ENUMDEF_H_INCLUDED
