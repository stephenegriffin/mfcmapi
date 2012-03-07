#pragma once

// ErrorArray.h: where our error array is defined

#define E_ACCT_NOT_FOUND 0x800C8101
#define E_ACCT_WRONG_SORT_ORDER 0x800C8105
#define E_OLK_REGISTRY  0x800C8001
#define E_OLK_ALREADY_INITIALIZED  0x800C8002
#define E_OLK_PARAM_NOT_SUPPORTED 0x800C8003
#define E_OLK_NOT_INITIALIZED 0x800C8005
#define E_OLK_PROP_READ_ONLY  0x800C800D

#ifndef MAPI_E_STORE_FULL
#define MAPI_E_STORE_FULL MAKE_MAPI_E( 0x60C )
#endif

// [MS-OXCDATA]
#ifndef MAPI_E_LOCKID_LIMIT
#define MAPI_E_LOCKID_LIMIT MAKE_MAPI_E( 0x60D )
#endif
#ifndef MAPI_E_NAMED_PROP_QUOTA_EXCEEDED
#define MAPI_E_NAMED_PROP_QUOTA_EXCEEDED MAKE_MAPI_E( 0x900 )
#endif

#ifndef MAPI_E_PROFILE_DELETED
#define MAPI_E_PROFILE_DELETED MAKE_MAPI_E( 0x204 )
#endif

#ifndef SYNC_E_CYCLE
#define SYNC_E_CYCLE MAKE_SYNC_E(0x804)
#endif

#define E_FILE_NOT_FOUND HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#define E_ERROR_PROC_NOT_FOUND HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND)

#define MAPI_E_RECONNECTED MAKE_MAPI_E( 0x125 )
#define MAPI_E_OFFLINE MAKE_MAPI_E( 0x126 )
#define MAIL_E_NAMENOTFOUND MAKE_SCODE(SEVERITY_ERROR, 0x0100, 10054)

#define ERROR_ENTRY(_fName) {(ULONG)_fName,L#_fName},

// These are sorted in increasing order, with no duplicates
// We have no logic for handling duplicate error codes
ERROR_ARRAY_ENTRY g_ErrorArray[] =
{
	ERROR_ENTRY(S_FALSE)

	// IStorage success codes
	ERROR_ENTRY(STG_S_CONVERTED)
	ERROR_ENTRY(STG_S_BLOCK)
	ERROR_ENTRY(STG_S_RETRYNOW)
	ERROR_ENTRY(STG_S_MONITORING)
	ERROR_ENTRY(STG_S_MULTIPLEOPENS)
	ERROR_ENTRY(STG_S_CANNOTCONSOLIDATE)

	ERROR_ENTRY(OLEOBJ_S_CANNOT_DOVERB_NOW)

	ERROR_ENTRY(MAPI_W_NO_SERVICE)
	ERROR_ENTRY(MAPI_W_ERRORS_RETURNED)
	ERROR_ENTRY(MAPI_W_POSITION_CHANGED)
	ERROR_ENTRY(MAPI_W_APPROX_COUNT)
	ERROR_ENTRY(MAPI_W_CANCEL_MESSAGE)
	ERROR_ENTRY(MAPI_W_PARTIAL_COMPLETION)

	ERROR_ENTRY(SYNC_W_PROGRESS)
	ERROR_ENTRY(SYNC_W_CLIENT_CHANGE_NEWER)

	ERROR_ENTRY(MAPI_E_INTERFACE_NOT_SUPPORTED)
	ERROR_ENTRY(MAPI_E_CALL_FAILED)

	// IStorage errors
	ERROR_ENTRY(STG_E_INVALIDFUNCTION)
	ERROR_ENTRY(STG_E_FILENOTFOUND)
	ERROR_ENTRY(STG_E_PATHNOTFOUND)
	ERROR_ENTRY(STG_E_TOOMANYOPENFILES)
	ERROR_ENTRY(STG_E_ACCESSDENIED)
	ERROR_ENTRY(STG_E_INVALIDHANDLE)
	ERROR_ENTRY(STG_E_INSUFFICIENTMEMORY)
	ERROR_ENTRY(STG_E_INVALIDPOINTER)
	ERROR_ENTRY(STG_E_NOMOREFILES)
	ERROR_ENTRY(STG_E_DISKISWRITEPROTECTED)
	ERROR_ENTRY(STG_E_SEEKERROR)
	ERROR_ENTRY(STG_E_WRITEFAULT)
	ERROR_ENTRY(STG_E_READFAULT)
	ERROR_ENTRY(STG_E_SHAREVIOLATION)
	ERROR_ENTRY(STG_E_LOCKVIOLATION)
	ERROR_ENTRY(STG_E_FILEALREADYEXISTS)
	ERROR_ENTRY(STG_E_INVALIDPARAMETER)
	ERROR_ENTRY(STG_E_MEDIUMFULL)
	ERROR_ENTRY(STG_E_PROPSETMISMATCHED)
	ERROR_ENTRY(STG_E_ABNORMALAPIEXIT)
	ERROR_ENTRY(STG_E_INVALIDHEADER)
	ERROR_ENTRY(STG_E_INVALIDNAME)
	ERROR_ENTRY(STG_E_UNKNOWN)
	ERROR_ENTRY(STG_E_UNIMPLEMENTEDFUNCTION)
	ERROR_ENTRY(STG_E_INVALIDFLAG)
	ERROR_ENTRY(STG_E_INUSE)
	ERROR_ENTRY(STG_E_NOTCURRENT)
	ERROR_ENTRY(STG_E_REVERTED)
	ERROR_ENTRY(STG_E_CANTSAVE)
	ERROR_ENTRY(STG_E_OLDFORMAT)
	ERROR_ENTRY(STG_E_OLDDLL)
	ERROR_ENTRY(STG_E_SHAREREQUIRED)
	ERROR_ENTRY(STG_E_NOTFILEBASEDSTORAGE)
	ERROR_ENTRY(STG_E_EXTANTMARSHALLINGS)
	ERROR_ENTRY(STG_E_DOCFILECORRUPT)
	ERROR_ENTRY(STG_E_BADBASEADDRESS)
	ERROR_ENTRY(STG_E_DOCFILETOOLARGE)
	ERROR_ENTRY(STG_E_NOTSIMPLEFORMAT)
	ERROR_ENTRY(STG_E_INCOMPLETE)
	ERROR_ENTRY(STG_E_TERMINATED)

	ERROR_ENTRY(MAPI_E_NO_SUPPORT)
	ERROR_ENTRY(MAPI_E_BAD_CHARWIDTH)
	ERROR_ENTRY(MAPI_E_STRING_TOO_LONG)
	ERROR_ENTRY(MAPI_E_UNKNOWN_FLAGS)
	ERROR_ENTRY(MAPI_E_INVALID_ENTRYID)
	ERROR_ENTRY(MAPI_E_INVALID_OBJECT)
	ERROR_ENTRY(MAPI_E_OBJECT_CHANGED)
	ERROR_ENTRY(MAPI_E_OBJECT_DELETED)
	ERROR_ENTRY(MAPI_E_BUSY)
	ERROR_ENTRY(MAPI_E_NOT_ENOUGH_DISK)
	ERROR_ENTRY(MAPI_E_NOT_ENOUGH_RESOURCES)
	ERROR_ENTRY(MAPI_E_NOT_FOUND)
	ERROR_ENTRY(MAPI_E_VERSION)
	ERROR_ENTRY(MAPI_E_LOGON_FAILED)
	ERROR_ENTRY(MAPI_E_SESSION_LIMIT)
	ERROR_ENTRY(MAPI_E_USER_CANCEL)
	ERROR_ENTRY(MAPI_E_UNABLE_TO_ABORT)
	ERROR_ENTRY(MAPI_E_NETWORK_ERROR)
	ERROR_ENTRY(MAPI_E_DISK_ERROR)
	ERROR_ENTRY(MAPI_E_TOO_COMPLEX)
	ERROR_ENTRY(MAPI_E_BAD_COLUMN)
	ERROR_ENTRY(MAPI_E_EXTENDED_ERROR)
	ERROR_ENTRY(MAPI_E_COMPUTED)
	ERROR_ENTRY(MAPI_E_CORRUPT_DATA)
	ERROR_ENTRY(MAPI_E_UNCONFIGURED)
	ERROR_ENTRY(MAPI_E_FAILONEPROVIDER)
	ERROR_ENTRY(MAPI_E_UNKNOWN_CPID)
	ERROR_ENTRY(MAPI_E_UNKNOWN_LCID)

	// Flavors of E_ACCESSDENIED, used at logon
	ERROR_ENTRY(MAPI_E_PASSWORD_CHANGE_REQUIRED)
	ERROR_ENTRY(MAPI_E_PASSWORD_EXPIRED)
	ERROR_ENTRY(MAPI_E_INVALID_WORKSTATION_ACCOUNT)
	ERROR_ENTRY(MAPI_E_INVALID_ACCESS_TIME)
	ERROR_ENTRY(MAPI_E_ACCOUNT_DISABLED)

	// Reconnect
	ERROR_ENTRY(MAPI_E_RECONNECTED)
	ERROR_ENTRY(MAPI_E_OFFLINE)

	// COM errors (for CLSIDFromString)
	ERROR_ENTRY(REGDB_E_WRITEREGDB)
	ERROR_ENTRY(REGDB_E_CLASSNOTREG)
	ERROR_ENTRY(CO_E_CLASSSTRING)

	// MAPI base function and status object specific errors and warnings
	ERROR_ENTRY(MAPI_E_END_OF_SESSION)
	ERROR_ENTRY(MAPI_E_UNKNOWN_ENTRYID)
	ERROR_ENTRY(MAPI_E_MISSING_REQUIRED_COLUMN)

	ERROR_ENTRY(MAPI_E_PROFILE_DELETED)

	// Property specific errors and warnings
	ERROR_ENTRY(MAPI_E_BAD_VALUE)
	ERROR_ENTRY(MAPI_E_INVALID_TYPE)
	ERROR_ENTRY(MAPI_E_TYPE_NO_SUPPORT)
	ERROR_ENTRY(MAPI_E_UNEXPECTED_TYPE)
	ERROR_ENTRY(MAPI_E_TOO_BIG)
	ERROR_ENTRY(MAPI_E_DECLINE_COPY)
	ERROR_ENTRY(MAPI_E_UNEXPECTED_ID)

	// Table specific errors and warnings
	ERROR_ENTRY(MAPI_E_UNABLE_TO_COMPLETE)
	ERROR_ENTRY(MAPI_E_TIMEOUT)
	ERROR_ENTRY(MAPI_E_TABLE_EMPTY)
	ERROR_ENTRY(MAPI_E_TABLE_TOO_BIG)

	ERROR_ENTRY(MAPI_E_INVALID_BOOKMARK)

	// Transport specific errors and warnings
	ERROR_ENTRY(MAPI_E_WAIT)
	ERROR_ENTRY(MAPI_E_CANCEL)
	ERROR_ENTRY(MAPI_E_NOT_ME)

	// Message Store, Folder, and Message specific errors and warnings
	ERROR_ENTRY(MAPI_E_CORRUPT_STORE)
	ERROR_ENTRY(MAPI_E_NOT_IN_QUEUE)
	ERROR_ENTRY(MAPI_E_NO_SUPPRESS)
	ERROR_ENTRY(MAPI_E_COLLISION)
	ERROR_ENTRY(MAPI_E_NOT_INITIALIZED)
	ERROR_ENTRY(MAPI_E_NON_STANDARD)
	ERROR_ENTRY(MAPI_E_NO_RECIPIENTS)
	ERROR_ENTRY(MAPI_E_SUBMITTED)
	ERROR_ENTRY(MAPI_E_HAS_FOLDERS)
	ERROR_ENTRY(MAPI_E_HAS_MESSAGES)
	ERROR_ENTRY(MAPI_E_FOLDER_CYCLE)
	ERROR_ENTRY(MAPI_E_STORE_FULL)
	ERROR_ENTRY(MAPI_E_LOCKID_LIMIT)

	// Address Book specific errors and warnings
	ERROR_ENTRY(MAPI_E_AMBIGUOUS_RECIP)

	ERROR_ENTRY(SYNC_E_OBJECT_DELETED)
	ERROR_ENTRY(SYNC_E_IGNORE)
	ERROR_ENTRY(SYNC_E_CONFLICT)
	ERROR_ENTRY(SYNC_E_NO_PARENT)
	ERROR_ENTRY(SYNC_E_CYCLE)
	ERROR_ENTRY(SYNC_E_UNSYNCHRONIZED)

	ERROR_ENTRY(MAPI_E_NAMED_PROP_QUOTA_EXCEEDED)
	ERROR_ENTRY(E_FILE_NOT_FOUND)
	ERROR_ENTRY(MAPI_E_NO_ACCESS)
	ERROR_ENTRY(MAPI_E_NOT_ENOUGH_MEMORY)

	// StrSafe.h error codes:
	ERROR_ENTRY(STRSAFE_E_END_OF_FILE)
	ERROR_ENTRY(MAPI_E_INVALID_PARAMETER)
	ERROR_ENTRY(STRSAFE_E_INSUFFICIENT_BUFFER)
	ERROR_ENTRY(E_ERROR_PROC_NOT_FOUND)

	ERROR_ENTRY(E_OLK_REGISTRY)
	ERROR_ENTRY(E_OLK_ALREADY_INITIALIZED)
	ERROR_ENTRY(E_OLK_PARAM_NOT_SUPPORTED)
	ERROR_ENTRY(E_OLK_NOT_INITIALIZED)
	ERROR_ENTRY(E_OLK_PROP_READ_ONLY)
	ERROR_ENTRY(E_ACCT_NOT_FOUND)
	ERROR_ENTRY(E_ACCT_WRONG_SORT_ORDER)

	ERROR_ENTRY(MAIL_E_NAMENOTFOUND)
}; // g_ErrorArray

ULONG g_ulErrorArray = _countof(g_ErrorArray);
