#include "FsFilter.h"

//////////////////////////////////////////////////////////////////////////
// Function prototypes

VOID FsFilterUnload(
    __in PDRIVER_OBJECT DriverObject
    );

//////////////////////////////////////////////////////////////////////////
// Global data
BOOLEAN g_HidingOn = 0;

PDRIVER_OBJECT g_fsFilterDriverObject = NULL;


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




//////////////////////////////////////////////////////////////////////////
// DriverEntry - Entry point of the driver

NTSTATUS DriverEntry(
    __inout PDRIVER_OBJECT  DriverObject,
    __in    PUNICODE_STRING RegistryPath
    )
{    
    NTSTATUS status = STATUS_SUCCESS;
    ULONG    i      = 0;
	PDEVICE_OBJECT  fdo;
	UNICODE_STRING  devName;
	PFSFILTER_DEVICE_EXTENSION fdo_ext;
/*	g_HidingOn 		= 1;*/
	g_fsFilterDriverObject = DriverObject;

	
	RtlInitUnicodeString( &devName, DEVNAME);

	status = IoCreateDevice(DriverObject,
		sizeof(FSFILTER_DEVICE_EXTENSION),
		&devName, // может быть и NULL
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE, // без эксклюзивного доступа
		&fdo);
	if(!NT_SUCCESS(status)) return status;

	fdo_ext = (PFSFILTER_DEVICE_EXTENSION) fdo->DeviceExtension;
	fdo_ext->AttachedToDeviceObject = NULL;
	
	//RtlCopyUnicodeString(&fdo_ext->ustrSymLinkName, &g_FdoSymLink);
	RtlInitUnicodeString(&fdo_ext->ustrSymLinkName, SYM_LINK_NAME );
	status = IoCreateSymbolicLink( &fdo_ext->ustrSymLinkName, &devName );
	if (!NT_SUCCESS(status))
	{ // при неудаче v удалить Device Object и вернуть управление
		IoDeleteDevice( fdo );
		return status;
	}
		
    
    
    //
    //  Initialize the driver object dispatch table.
    //

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; ++i) 
        DriverObject->MajorFunction[i] = FsFilterDispatchPassThrough;
	DriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = FsFilterDispatchDirCtrl;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = FsFilterDispatchDeviceIoControl;

    //
    // Set fast-io dispatch table.
    //

    DriverObject->FastIoDispatch = &g_fastIoDispatch;

    //
    //  Registered callback routine for file system changes.
    //

    status = IoRegisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback); 
    if (!NT_SUCCESS(status)) 
    {
        return status;
    }

    //
    // Set driver unload routine (debug purpose only).
    //

    DriverObject->DriverUnload = FsFilterUnload;

    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// Unload routine

VOID FsFilterUnload(
    __in PDRIVER_OBJECT DriverObject
    )
{
    ULONG           numDevices = 0;
    ULONG           i          = 0;    
    LARGE_INTEGER   interval;
    PDEVICE_OBJECT  devList[DEVOBJ_LIST_SIZE];

    interval.QuadPart = (5 * DELAY_ONE_SECOND); //delay 5 seconds

	RemoveRule (); // Free memory allocated for rule
    //
    //  Unregistered callback routine for file system changes.
    //

    IoUnregisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback);

    //
    //  This is the loop that will go through all of the devices we are attached
    //  to and detach from them.
    //

    for (;;)
    {
        	IoEnumerateDeviceObjectList(
            DriverObject,
            devList,
            sizeof(devList),
            &numDevices);

        if (0 == numDevices)
        {
            break;
        }

        numDevices = min(numDevices, RTL_NUMBER_OF(devList));

        for (i = 0; i < numDevices; ++i) 
        {           		
			FsFilterDetachFromDevice(devList[i]);
            ObDereferenceObject(devList[i]);
        }
        
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }
}

//////////////////////////////////////////////////////////////////////////
// Misc

BOOLEAN FsFilterIsMyDeviceObject(
    __in PDEVICE_OBJECT DeviceObject
    )
{
    return DeviceObject->DriverObject == g_fsFilterDriverObject;
}
