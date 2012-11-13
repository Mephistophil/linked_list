#include "FsFilter.h"

// First entry of linked list for store rule
PRULE_ENTRY g_pFirst = NULL;
// Hiding ON/OFF flag
BOOLEAN g_HidingOn = 0;
// Pointer for our driver object structure
PDRIVER_OBJECT g_fsFilterDriverObject = NULL;
// Required length of serializer for current rule
ULONG g_ruleLength = 0;

///////////////////////////////////////////////////////////////////////////
// FastIoDispatch
FAST_IO_DISPATCH g_fastIoDispatch =
{
	sizeof(FAST_IO_DISPATCH),
	FsFilterFastIoCheckIfPossible,
	FsFilterFastIoRead,
	FsFilterFastIoWrite,
	FsFilterFastIoQueryBasicInfo,
	FsFilterFastIoQueryStandardInfo,
	FsFilterFastIoLock,
	FsFilterFastIoUnlockSingle,
	FsFilterFastIoUnlockAll,
	FsFilterFastIoUnlockAllByKey,
	FsFilterFastIoDeviceControl,
	NULL,
	NULL,
	FsFilterFastIoDetachDevice,
	FsFilterFastIoQueryNetworkOpenInfo,
	NULL,
	FsFilterFastIoMdlRead,
	FsFilterFastIoMdlReadComplete,
	FsFilterFastIoPrepareMdlWrite,
	FsFilterFastIoMdlWriteComplete,
	FsFilterFastIoReadCompressed,
	FsFilterFastIoWriteCompressed,
	FsFilterFastIoMdlReadCompleteCompressed,
	FsFilterFastIoMdlWriteCompleteCompressed,
	FsFilterFastIoQueryOpen,
	NULL,
	NULL,
	NULL,
};