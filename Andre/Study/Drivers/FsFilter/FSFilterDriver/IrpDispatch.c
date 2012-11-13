#include "FsFilter.h"


/************* GLOBAL DATA ***********************/
ULONG g_ruleLength = 0;

/************* PROTOTYPES of service FUNCTIONS ****************/
// set STATUS, status information and complite IRP
NTSTATUS CompleteIrp( PIRP Irp, NTSTATUS status, ULONG info);
// hiding files algorithm
int FileHider (__in PVOID buffer);
// Completion routine for IRP_MJ_DIRECTORY_CONTROL handler
NTSTATUS QueryDirCompletionRoutine(
								IN PDEVICE_OBJECT   DeviceObject,
								IN PIRP             Irp,
								IN PVOID            Context
								);
// Send IRP down through the stack and waiting by FileSystem complite it
NTSTATUS WaitForComplite (
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP           Irp
    );
// true IRP_MJ_DIRECTORY_CONTROL handler
NTSTATUS DispatchQueryDir (
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP           Irp
    );

/***********  D I S P A T C H  Pass through IRP ****************/
NTSTATUS FsFilterDispatchPassThrough(
    __in PDEVICE_OBJECT DeviceObject, 
    __in PIRP           Irp
    )
{
    PFSFILTER_DEVICE_EXTENSION	pDevExt		= (PFSFILTER_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
	PIO_STACK_LOCATION 			irpStack 	= IoGetCurrentIrpStackLocation(Irp);
	
	if(!pDevExt->AttachedToDeviceObject)
	 		return CompleteIrp(Irp,STATUS_SUCCESS,0);
	
	IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(pDevExt->AttachedToDeviceObject, Irp);
}

/***** D I S P A T C H  MJ_DIRECTORY_CONTROL IRP (Switcher OFF/ON hiding) ******/
NTSTATUS FsFilterDispatchDirCtrl(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP           Irp
    )
{
    if (g_HidingOn) 		
		return DispatchQueryDir(DeviceObject, Irp); // Hiding mode
	else
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
}

/*** D I S P A T C H  IRP_MN_QUERY_DIRECTORY & FileBothDirectoryInformation IRP ***/
NTSTATUS DispatchQueryDir (
							__in PDEVICE_OBJECT DeviceObject,
							__in PIRP           Irp
							)
{
//del    ULONG 						info;
	NTSTATUS 					status;
	PVOID 					irp_UserBuffer 	= Irp->UserBuffer;
	PIO_STACK_LOCATION 			irpStack 	= IoGetCurrentIrpStackLocation(Irp);
	//UNICODE_STRING			DirectoryName	= irpStack->FileObject->FileName; //del
	
	if ( // if invalid query don't handle
		(irpStack->MinorFunction != IRP_MN_QUERY_DIRECTORY) || 
		(irpStack->Parameters.QueryDirectory.FileInformationClass != FileBothDirectoryInformation)
		)
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
//del
// 	DbgPrint("\n\n============= Directory: %wZ=================\n", &DirectoryName);
// 	DbgPrint("irpStack->Flags == %d\n", irpStack->Flags);
// 	DbgPrint("irpStack->FileObject->FileName == %wZ\n", irpStack->FileObject->FileName);
// 	DbgPrint("irpStack->Parameters.QueryDirectory.FileName == %wZ\n", irpStack->Parameters.QueryDirectory.FileName);
	
	status = WaitForComplite (DeviceObject, Irp);
	if( !NT_SUCCESS(status))
		return status;
	
	FileHider (irp_UserBuffer);
//del	DbgPrint ("End of folder:%wZ\n\n", &DirectoryName);
//del	info = Irp->IoStatus.Information;
	return CompleteIrp( Irp, status, Irp->IoStatus.Information);
}
/************  D I S P A T C H  IRP_MJ_DEVICE_CONTROL *******************/
NTSTATUS FsFilterDispatchDeviceIoControl(
										__in PDEVICE_OBJECT DeviceObject,
										__in PIRP           Irp
										)
{
	NTSTATUS 					status 		= STATUS_SUCCESS;
	PIO_STACK_LOCATION 			IrpStack 	= IoGetCurrentIrpStackLocation(Irp);
	ULONG    					ControlCode	= IrpStack->Parameters.DeviceIoControl.IoControlCode;
	PWCHAR     					pBuffer		= NULL;
	ULONG 						bufferLen	= 0;
	ULONG	  					BytesTxd	= 0;
	PFSFILTER_DEVICE_EXTENSION	pDevExt		= (PFSFILTER_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
	
	if(pDevExt->AttachedToDeviceObject)
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
	
	switch( ControlCode) 
	{
		case IOCTL_SEND_RULE_TO_DRV: //get rule from user
		{
			g_ruleLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			if ( g_ruleLength < 1)
			{
				status = STATUS_INVALID_PARAMETER;
				break;		
			}   
			
			pBuffer = (PWCHAR)Irp->AssociatedIrp.SystemBuffer;
			InitRule(pBuffer);
			g_ruleLength = RuleSerializerLength();
			BytesTxd = g_ruleLength;
			
			break;
		}
		
		case IOCTL_GET_RULE_LENGTH:
		{
			BytesTxd = RuleSerializerLength() - 2;
			break;
		}


		case IOCTL_GET_RULE_FROM_DRV:
		{	    
 			pBuffer = (PWCHAR)Irp->AssociatedIrp.SystemBuffer;
			bufferLen = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
			if(!AssembleRule (pBuffer, bufferLen))
			{
				BytesTxd = 0;
				break;
			}
			break;
		}

		case IOCTL_HIDING_ON:
		{
			g_HidingOn = 1;
			break;
		}

		case IOCTL_HIDING_OFF:
		{
			g_HidingOn = 0;
			break;
		}
		
		case IOCTL_CHECK_HIDING:
		{
			BytesTxd = (ULONG) g_HidingOn;
			break;
		}

		default: 
			return FsFilterDispatchPassThrough(DeviceObject, Irp);
	}
	
	return CompleteIrp(Irp,status,BytesTxd); // Завершение IRP
}

/***************** -= CompliteIrp =- (service routine) *********************/
NTSTATUS CompleteIrp( PIRP Irp, NTSTATUS status, ULONG info)
{
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);
	return status;
}

/***************** Hiding algorithm **********************/
int FileHider (__in PVOID buffer)
{
	PFILE_BOTH_DIR_INFORMATION prevFile = NULL;
	PFILE_BOTH_DIR_INFORMATION currFile = (PFILE_BOTH_DIR_INFORMATION) buffer;
	PFILE_BOTH_DIR_INFORMATION nextFile = NULL;
	ULONG						offset	=	0;
	USHORT						i;
	
	WCHAR						currentNameW [FILE_NAME_LENGTH];
			
	nextFile = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)currFile + currFile->NextEntryOffset);
	
	while (currFile)
	{
		memcpy (currentNameW, currFile->FileName, currFile->FileNameLength);
		currentNameW [currFile->FileNameLength/sizeof(WCHAR)] = '\0';
		for ( i = 0 ; i < currFile->FileNameLength/sizeof(WCHAR); ++i )
		{
			currentNameW[i] = RtlUpcaseUnicodeChar( currentNameW[i]);
		}
		if (CheckEqual (currentNameW))
		{
			if (!prevFile) // first file
			{
				if (currFile == nextFile) // alone file
				{
					break;
				}
				else //first, not last file
				{
					offset = currFile->NextEntryOffset;
					if (nextFile->NextEntryOffset)
					{
						memcpy ( currFile, nextFile, nextFile->NextEntryOffset);
						currFile->NextEntryOffset += offset;
					}
					else
					{
						memcpy ( currFile, nextFile, sizeof (FILE_BOTH_DIR_INFORMATION ) + nextFile->FileNameLength);
					}
					nextFile = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)currFile + currFile->NextEntryOffset);
					continue;
				}				
			}
			else // not first file
			{
				if (currFile == nextFile) // last file
				{
					prevFile->NextEntryOffset=0; 
					break;
				}
				else //middle file (not first, not last)
				{
					prevFile->NextEntryOffset += currFile->NextEntryOffset;
				}
			}		
		}
		if (!currFile->NextEntryOffset) break;
		prevFile = currFile;
		currFile = nextFile;
		nextFile = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)currFile + currFile->NextEntryOffset);
	}
	return 1;
}

/****************** QueryDirCompletionRoutine (set event for wait) ***********************/
NTSTATUS QueryDirCompletionRoutine(
								IN PDEVICE_OBJECT   DeviceObject,
								IN PIRP             Irp,
								IN PVOID            Context
								)
{ 
	if (Irp->PendingReturned == TRUE)
		KeSetEvent ((PKEVENT) Context, IO_NO_INCREMENT, FALSE);
	return STATUS_MORE_PROCESSING_REQUIRED;  
} 

/*********************** send IRP down and wait for get it again *****************************/
NTSTATUS WaitForComplite (
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP           Irp
    )
{
	KEVENT						event;
	PFSFILTER_DEVICE_EXTENSION 	pDevExt = (PFSFILTER_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
	NTSTATUS 					status;
	
	KeInitializeEvent(&event, NotificationEvent, FALSE);
	IoCopyCurrentIrpStackLocationToNext(Irp);
	IoSetCompletionRoutine(Irp, QueryDirCompletionRoutine, &event, TRUE, TRUE, TRUE);
	status = 	IoCallDriver(pDevExt->AttachedToDeviceObject, Irp);
	if ( status == STATUS_PENDING) 
	{
		KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
		status = Irp->IoStatus.Status;
    }
	return status;
}

