/*++

Module Name:

    FsFilter1.c

Abstract:

    This is the main module of the FsFilter1 miniFilter driver.

Environment:

    Kernel mode

--*/

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <stdio.h>


PFLT_PORT port = NULL;
PFLT_PORT ClientPort = NULL;
PFLT_FILTER FilterHandle = NULL;

BOOLEAN enabled = FALSE;		// Start I/O


NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags);
FLT_POSTOP_CALLBACK_STATUS MiniPostCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
FLT_PREOP_CALLBACK_STATUS MiniPreCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext);
FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext);


const FLT_OPERATION_REGISTRATION Callbacks[] = {
    //{IRP_MJ_CREATE, 0, MiniPreCreate, MiniPostCreate},
    //{IRP_MJ_WRITE, 0, MiniPreWrite, NULL},
    {IRP_MJ_OPERATION_END}
};


const FLT_REGISTRATION FilterRegistration = {
    sizeof(FLT_REGISTRATION),           //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags
    NULL,                               //  Context
    Callbacks,                          //  Operation callbacks
    MiniUnload,                         //  MiniFilterUnload
    NULL,                               //  InstanceSetup
    NULL,                               //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete
    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL,                               //  NormalizeNameComponent
    NULL
};


NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags)
{
    KdPrint(("[NastywareDriver] Mini unloaded.\n"));
    FltCloseCommunicationPort(port);
    FltUnregisterFilter(FilterHandle);

    return STATUS_SUCCESS;
}


FLT_POSTOP_CALLBACK_STATUS MiniPostCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags)
{
    KdPrint(("MiniPostCeate running.\r\n"));
    return FLT_POSTOP_FINISHED_PROCESSING;
}


FLT_PREOP_CALLBACK_STATUS MiniPreCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext)
{
    PFLT_FILE_NAME_INFORMATION FileNameInfo;
    NTSTATUS status;
    WCHAR Name[200] = { 0 };

    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED|FLT_FILE_NAME_QUERY_DEFAULT, &FileNameInfo);

    if (NT_SUCCESS(status)) {

        status = FltParseFileNameInformation(FileNameInfo);

        if (NT_SUCCESS(status)) {

            if (FileNameInfo->Name.MaximumLength < 260) {
                RtlCopyMemory(Name, FileNameInfo->Name.Buffer, FileNameInfo->Name.MaximumLength);
                KdPrint(("Create file: %ws\r\n", Name));
            }
        }

        FltReleaseFileNameInformation(FileNameInfo);
    }

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}


FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext)
{
    PFLT_FILE_NAME_INFORMATION FileNameInfo;
    NTSTATUS status;
    WCHAR Name[200] = { 0 };

    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &FileNameInfo);

    if (NT_SUCCESS(status)) {

        status = FltParseFileNameInformation(FileNameInfo);

        if (NT_SUCCESS(status)) {

            if (FileNameInfo->Name.MaximumLength < 260) {
                
                RtlCopyMemory(Name, FileNameInfo->Name.Buffer, FileNameInfo->Name.MaximumLength);
                _wcsupr(Name);
                if (wcsstr(Name, L"OPENME.TXT") != NULL) {
                    KdPrint(("Write file: %ws blocked.\r\n", Name));
                    Data->IoStatus.Status = STATUS_INVALID_PARAMETER;
                    Data->IoStatus.Information = 0;
                    FltReleaseFileNameInformation(FileNameInfo);
                    return FLT_PREOP_COMPLETE;
                }
                KdPrint(("Create file: %ws\r\n", Name));
            }
        }

        FltReleaseFileNameInformation(FileNameInfo);
    }

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}


NTSTATUS MiniConnect(PFLT_PORT clientport, PVOID serverportcookie, PVOID Context, ULONG size, PVOID Connectioncookie)
{
    ClientPort = clientport;
    enabled = TRUE;                                 // enable I/O
    DbgPrint("[NastywareDriver] MiniConnect enabled.\n");     // notify user
    return STATUS_SUCCESS;
}


VOID MiniDisconnect(PVOID connectioncookie)
{
    DbgPrint("[NastywareDriver] Disconnected.\n");
    FltCloseClientPort(FilterHandle, &ClientPort);
}


NTSTATUS MiniSendRec(PVOID portcookie, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLenght, PULONG RetLength)
{
    PCHAR msg = "Kernel msg";
    DbgPrint("[NastywareDriver] User msg is: %s\n", (PCHAR)InputBuffer);
    strcpy((PCHAR)OutputBuffer, msg);
    return STATUS_SUCCESS;
}


/* Process Callback */
void ProcessCallback(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create) {
    NTSTATUS status;
    UINT64 pid;
    UNREFERENCED_PARAMETER(ParentId);

    /* Only Create Process */
    if (Create) {
        /* Notify User */
        KdPrint(("[NastywareDriver] ProcessCallback processing ProcessId: %i\n", ProcessId));
        /* I/O enabled*/
        if (enabled) {
            /* Send PID */
            pid = (UINT64)ProcessId;
            status = FltSendMessage(FilterHandle, &ClientPort, &pid, sizeof(pid), NULL, 0, 0);
            /* Notify User */
            KdPrint(("[NastywareDriver] ProcessCallback Send Status: %x", status));
        }
    }
}


/* Unload Routine */
VOID DriverUnload(PDRIVER_OBJECT Driverobject)
{
    UNREFERENCED_PARAMETER(Driverobject);
    /* Remove Process Callback */
    PsSetCreateProcessNotifyRoutine(ProcessCallback, TRUE);
    /* Notify Removal */
    KdPrint(("[NastywareDriver] DriverUnload.\n"));
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS status;
    PSECURITY_DESCRIPTOR sd;
    OBJECT_ATTRIBUTES oa = { 0 };
    UNICODE_STRING name = RTL_CONSTANT_STRING(L"\\mf");

    /* Notify it is running */
    DbgPrint("[NastywareDriver] DriverEntry Running.\n");

    /* Register the unload routine */
    DriverObject->DriverUnload = DriverUnload;

    /* Add the process callback */
    status = PsSetCreateProcessNotifyRoutine(ProcessCallback, FALSE);
    
    status = FltRegisterFilter(DriverObject, &FilterRegistration, &FilterHandle);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);
    
    if (NT_SUCCESS(status)) {

        InitializeObjectAttributes(&oa, &name, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, sd);
        status = FltCreateCommunicationPort(FilterHandle, &port, &oa, NULL, MiniConnect, MiniDisconnect, MiniSendRec, 1);

        FltFreeSecurityDescriptor(sd);

        if (NT_SUCCESS(status)) {
            
            status = FltStartFiltering(FilterHandle);

            if (NT_SUCCESS(status)) {

                return status;
            }

            FltCloseCommunicationPort(port);
        }

        FltUnregisterFilter(FilterHandle);
    }
    
    return status;
}


