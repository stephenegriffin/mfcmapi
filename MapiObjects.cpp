// MapiObjects.cpp: implementation of the CMapiObjects class.

#include "stdafx.h"
#include "MapiObjects.h"

static wstring GCCLASS = L"CGlobalCache"; // STRING_OK

// A single instance cache of objects available to all
class CGlobalCache
{
public:
	CGlobalCache();
	virtual ~CGlobalCache();

	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	void MAPIInitialize(ULONG ulFlags);
	void MAPIUninitialize();
	_Check_return_ bool bMAPIInitialized() const;

	void SetABEntriesToCopy(_In_ LPENTRYLIST lpEBEntriesToCopy);
	_Check_return_ LPENTRYLIST GetABEntriesToCopy() const;

	void SetMessagesToCopy(_In_ LPENTRYLIST lpMessagesToCopy, _In_ LPMAPIFOLDER lpSourceParent);
	_Check_return_ LPENTRYLIST GetMessagesToCopy() const;

	void SetFolderToCopy(_In_ LPMAPIFOLDER lpFolderToCopy, _In_ LPMAPIFOLDER lpSourceParent);
	_Check_return_ LPMAPIFOLDER GetFolderToCopy() const;

	void SetPropertyToCopy(ULONG ulPropTag, _In_ LPMAPIPROP lpSourcePropObject);
	_Check_return_ ULONG GetPropertyToCopy() const;
	_Check_return_ LPMAPIPROP GetSourcePropObject() const;

	void SetAttachmentsToCopy(_In_ LPMESSAGE lpMessage, ULONG ulNumAttachments, _In_ ULONG* lpAttNumList);
	_Check_return_ ULONG* GetAttachmentsToCopy() const;
	_Check_return_ ULONG GetNumAttachments() const;

	void SetProfileToCopy(_In_ string szProfileName);
	_Check_return_ string GetProfileToCopy() const;

	_Check_return_ LPMAPIFOLDER GetSourceParentFolder() const;

	_Check_return_ ULONG GetBufferStatus() const;

private:
	void EmptyBuffer();

	LONG m_cRef;
	LPENTRYLIST m_lpAddressEntriesToCopy;
	LPENTRYLIST m_lpMessagesToCopy;
	LPMAPIFOLDER m_lpFolderToCopy;
	ULONG m_ulPropTagToCopy;
	ULONG* m_lpulAttachmentsToCopy;
	ULONG m_ulNumAttachments;
	string m_szProfileToCopy;
	LPMAPIFOLDER m_lpSourceParent;
	LPMAPIPROP m_lpSourcePropObject;
	bool m_bMAPIInitialized;
};

CGlobalCache::CGlobalCache()
{
	TRACE_CONSTRUCTOR(GCCLASS);
	m_cRef = 1;
	m_bMAPIInitialized = false;

	m_lpMessagesToCopy = nullptr;
	m_lpFolderToCopy = nullptr;
	m_lpAddressEntriesToCopy = nullptr;
	m_ulPropTagToCopy = 0;

	m_lpSourceParent = nullptr;
	m_lpSourcePropObject = nullptr;

	m_lpulAttachmentsToCopy = nullptr;
	m_ulNumAttachments = 0;
}

CGlobalCache::~CGlobalCache()
{
	TRACE_DESTRUCTOR(GCCLASS);
	EmptyBuffer();
	CGlobalCache::MAPIUninitialize();
}

STDMETHODIMP_(ULONG) CGlobalCache::AddRef()
{
	auto lCount = InterlockedIncrement(&m_cRef);
	TRACE_ADDREF(GCCLASS, lCount);
	return lCount;
}

STDMETHODIMP_(ULONG) CGlobalCache::Release()
{
	auto lCount = InterlockedDecrement(&m_cRef);
	TRACE_RELEASE(GCCLASS, lCount);
	if (!lCount) delete this;
	return lCount;
}

void CGlobalCache::MAPIInitialize(ULONG ulFlags)
{
	auto hRes = S_OK;
	if (!m_bMAPIInitialized)
	{
		MAPIINIT_0 mapiInit = { MAPI_INIT_VERSION, ulFlags };
		WC_MAPI(::MAPIInitialize(&mapiInit));
		if (SUCCEEDED(hRes))
		{
			m_bMAPIInitialized = true;
		}
		else
		{
			ErrDialog(__FILE__, __LINE__,
				IDS_EDMAPIINITIALIZEFAILED,
				hRes,
				ErrorNameFromErrorCode(hRes).c_str());
		}
	}
}

void CGlobalCache::MAPIUninitialize()
{
	if (m_bMAPIInitialized)
	{
		::MAPIUninitialize();
		m_bMAPIInitialized = false;
	}
}

_Check_return_ bool CGlobalCache::bMAPIInitialized() const
{
	return m_bMAPIInitialized;
}

void CGlobalCache::EmptyBuffer()
{
	if (m_lpAddressEntriesToCopy) MAPIFreeBuffer(m_lpAddressEntriesToCopy);
	if (m_lpMessagesToCopy) MAPIFreeBuffer(m_lpMessagesToCopy);
	if (m_lpulAttachmentsToCopy) MAPIFreeBuffer(m_lpulAttachmentsToCopy);
	m_szProfileToCopy.clear();
	if (m_lpFolderToCopy) m_lpFolderToCopy->Release();
	if (m_lpSourceParent) m_lpSourceParent->Release();
	if (m_lpSourcePropObject) m_lpSourcePropObject->Release();

	m_lpAddressEntriesToCopy = nullptr;
	m_lpMessagesToCopy = nullptr;
	m_lpFolderToCopy = nullptr;
	m_ulPropTagToCopy = 0;
	m_lpSourceParent = nullptr;
	m_lpSourcePropObject = nullptr;
	m_lpulAttachmentsToCopy = nullptr;
	m_ulNumAttachments = 0;
}

void CGlobalCache::SetABEntriesToCopy(_In_ LPENTRYLIST lpEBEntriesToCopy)
{
	EmptyBuffer();
	m_lpAddressEntriesToCopy = lpEBEntriesToCopy;
}

_Check_return_ LPENTRYLIST CGlobalCache::GetABEntriesToCopy() const
{
	return m_lpAddressEntriesToCopy;
}

void CGlobalCache::SetMessagesToCopy(_In_ LPENTRYLIST lpMessagesToCopy, _In_ LPMAPIFOLDER lpSourceParent)
{
	EmptyBuffer();
	m_lpMessagesToCopy = lpMessagesToCopy;
	m_lpSourceParent = lpSourceParent;
	if (m_lpSourceParent) m_lpSourceParent->AddRef();
}

_Check_return_ LPENTRYLIST CGlobalCache::GetMessagesToCopy() const
{
	return m_lpMessagesToCopy;
}

void CGlobalCache::SetFolderToCopy(_In_ LPMAPIFOLDER lpFolderToCopy, _In_ LPMAPIFOLDER lpSourceParent)
{
	EmptyBuffer();
	m_lpFolderToCopy = lpFolderToCopy;
	if (m_lpFolderToCopy) m_lpFolderToCopy->AddRef();
	m_lpSourceParent = lpSourceParent;
	if (m_lpSourceParent) m_lpSourceParent->AddRef();
}

_Check_return_ LPMAPIFOLDER CGlobalCache::GetFolderToCopy() const
{
	if (m_lpFolderToCopy) m_lpFolderToCopy->AddRef();
	return m_lpFolderToCopy;
}

_Check_return_ LPMAPIFOLDER CGlobalCache::GetSourceParentFolder() const
{
	if (m_lpSourceParent) m_lpSourceParent->AddRef();
	return m_lpSourceParent;
}

void CGlobalCache::SetPropertyToCopy(ULONG ulPropTag, _In_ LPMAPIPROP lpSourcePropObject)
{
	EmptyBuffer();
	m_ulPropTagToCopy = ulPropTag;
	m_lpSourcePropObject = lpSourcePropObject;
	if (m_lpSourcePropObject) m_lpSourcePropObject->AddRef();
}

_Check_return_ ULONG CGlobalCache::GetPropertyToCopy() const
{
	return m_ulPropTagToCopy;
}

_Check_return_ LPMAPIPROP CGlobalCache::GetSourcePropObject() const
{
	if (m_lpSourcePropObject) m_lpSourcePropObject->AddRef();
	return m_lpSourcePropObject;
}

void CGlobalCache::SetAttachmentsToCopy(_In_ LPMESSAGE lpMessage, ULONG ulNumAttachments, _In_ ULONG* lpAttNumList)
{
	EmptyBuffer();
	m_lpulAttachmentsToCopy = lpAttNumList;
	m_ulNumAttachments = ulNumAttachments;
	m_lpSourcePropObject = lpMessage;
	if (m_lpSourcePropObject) m_lpSourcePropObject->AddRef();
}

_Check_return_ ULONG* CGlobalCache::GetAttachmentsToCopy() const
{
	return m_lpulAttachmentsToCopy;
}

_Check_return_ ULONG CGlobalCache::GetNumAttachments() const
{
	return m_ulNumAttachments;
}

void CGlobalCache::SetProfileToCopy(_In_ string szProfileName)
{
	m_szProfileToCopy = szProfileName;
}

_Check_return_ string CGlobalCache::GetProfileToCopy() const
{
	return m_szProfileToCopy;
}

_Check_return_ ULONG CGlobalCache::GetBufferStatus() const
{
	auto ulStatus = BUFFER_EMPTY;
	if (m_lpMessagesToCopy) ulStatus |= BUFFER_MESSAGES;
	if (m_lpFolderToCopy) ulStatus |= BUFFER_FOLDER;
	if (m_lpSourceParent) ulStatus |= BUFFER_PARENTFOLDER;
	if (m_lpAddressEntriesToCopy) ulStatus |= BUFFER_ABENTRIES;
	if (m_ulPropTagToCopy) ulStatus |= BUFFER_PROPTAG;
	if (m_lpSourcePropObject) ulStatus |= BUFFER_SOURCEPROPOBJ;
	if (m_lpulAttachmentsToCopy) ulStatus |= BUFFER_ATTACHMENTS;
	if (!m_szProfileToCopy.empty()) ulStatus |= BUFFER_PROFILE;
	return ulStatus;
}

static wstring CLASS = L"CMapiObjects";
// Pass an existing CMapiObjects to make a copy, pass NULL to create a new one from scratch
CMapiObjects::CMapiObjects(_In_opt_ CMapiObjects *OldMapiObjects)
{
	TRACE_CONSTRUCTOR(CLASS);
	DebugPrintEx(DBGConDes, CLASS, CLASS, L"OldMapiObjects = %p\n", OldMapiObjects);
	m_cRef = 1;

	m_lpAddrBook = nullptr;
	m_lpMDB = nullptr;
	m_lpMAPISession = nullptr;

	// If we were passed a valid object, make copies of its interfaces.
	if (OldMapiObjects)
	{
		m_lpMAPISession = OldMapiObjects->m_lpMAPISession;
		if (m_lpMAPISession) m_lpMAPISession->AddRef();

		m_lpMDB = OldMapiObjects->m_lpMDB;
		if (m_lpMDB) m_lpMDB->AddRef();

		m_lpAddrBook = OldMapiObjects->m_lpAddrBook;
		if (m_lpAddrBook) m_lpAddrBook->AddRef();

		m_lpGlobalCache = OldMapiObjects->m_lpGlobalCache;
		if (m_lpGlobalCache) m_lpGlobalCache->AddRef();
	}
	else
	{
		m_lpGlobalCache = new CGlobalCache();
	}
}

CMapiObjects::~CMapiObjects()
{
	TRACE_DESTRUCTOR(CLASS);
	if (m_lpAddrBook) m_lpAddrBook->Release();
	if (m_lpMDB) m_lpMDB->Release();
	if (m_lpMAPISession) m_lpMAPISession->Release();

	// Must be last - uninitializes MAPI
	if (m_lpGlobalCache) m_lpGlobalCache->Release();
}

STDMETHODIMP_(ULONG) CMapiObjects::AddRef()
{
	auto lCount = InterlockedIncrement(&m_cRef);
	TRACE_ADDREF(CLASS, lCount);
	return lCount;
}

STDMETHODIMP_(ULONG) CMapiObjects::Release()
{
	auto lCount = InterlockedDecrement(&m_cRef);
	TRACE_RELEASE(CLASS, lCount);
	if (!lCount) delete this;
	return lCount;
}

void CMapiObjects::MAPILogonEx(_In_ HWND hwnd, _In_opt_z_ LPTSTR szProfileName, ULONG ulFlags)
{
	auto hRes = S_OK;
	DebugPrint(DBGGeneric, L"Logging on with MAPILogonEx, ulFlags = 0x%X\n", ulFlags);

	this->MAPIInitialize(NULL);
	if (!bMAPIInitialized()) return;

	if (m_lpMAPISession) m_lpMAPISession->Release();
	m_lpMAPISession = nullptr;

	EC_H_CANCEL(::MAPILogonEx(
		reinterpret_cast<ULONG_PTR>(hwnd),
		szProfileName,
		nullptr,
		ulFlags,
		&m_lpMAPISession));

	DebugPrint(DBGGeneric, L"\tm_lpMAPISession set to %p\n", m_lpMAPISession);
}

void CMapiObjects::Logoff(_In_ HWND hwnd, ULONG ulFlags)
{
	auto hRes = S_OK;
	DebugPrint(DBGGeneric, L"Logging off of %p, ulFlags = 0x%08X\n", m_lpMAPISession, ulFlags);

	if (m_lpMAPISession)
	{
		EC_MAPI(m_lpMAPISession->Logoff(reinterpret_cast<ULONG_PTR>(hwnd), ulFlags, NULL));
		m_lpMAPISession->Release();
		m_lpMAPISession = nullptr;
	}
}

_Check_return_ LPMAPISESSION CMapiObjects::GetSession() const
{
	return m_lpMAPISession;
}

_Check_return_ LPMAPISESSION CMapiObjects::LogonGetSession(_In_ HWND hWnd)
{
	if (m_lpMAPISession) return m_lpMAPISession;

	CMapiObjects::MAPILogonEx(hWnd, nullptr, MAPI_EXTENDED | MAPI_LOGON_UI | MAPI_NEW_SESSION);

	return m_lpMAPISession;
}

void CMapiObjects::SetMDB(_In_opt_ LPMDB lpMDB)
{
	DebugPrintEx(DBGGeneric, CLASS, L"SetMDB", L"replacing %p with %p\n", m_lpMDB, lpMDB);
	if (m_lpMDB) m_lpMDB->Release();
	m_lpMDB = lpMDB;
	if (m_lpMDB) m_lpMDB->AddRef();
}

_Check_return_ LPMDB CMapiObjects::GetMDB() const
{
	return m_lpMDB;
}

void CMapiObjects::SetAddrBook(_In_opt_ LPADRBOOK lpAddrBook)
{
	DebugPrintEx(DBGGeneric, CLASS, L"SetAddrBook", L"replacing %p with %p\n", m_lpAddrBook, lpAddrBook);
	if (m_lpAddrBook) m_lpAddrBook->Release();
	m_lpAddrBook = lpAddrBook;
	if (m_lpAddrBook) m_lpAddrBook->AddRef();
}

_Check_return_ LPADRBOOK CMapiObjects::GetAddrBook(bool bForceOpen)
{
	// if we haven't opened the address book yet and we have a session, open it now
	if (!m_lpAddrBook && m_lpMAPISession && bForceOpen)
	{
		auto hRes = S_OK;
		EC_MAPI(m_lpMAPISession->OpenAddressBook(
			NULL,
			NULL,
			NULL,
			&m_lpAddrBook));
	}
	return m_lpAddrBook;
}

void CMapiObjects::MAPIInitialize(ULONG ulFlags) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->MAPIInitialize(ulFlags);
	}
}

void CMapiObjects::MAPIUninitialize() const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->MAPIUninitialize();
	}
}

_Check_return_ bool CMapiObjects::bMAPIInitialized() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->bMAPIInitialized();
	}
	return false;
}

void CMapiObjects::SetABEntriesToCopy(_In_ LPENTRYLIST lpEBEntriesToCopy) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetABEntriesToCopy(lpEBEntriesToCopy);
	}
}

_Check_return_ LPENTRYLIST CMapiObjects::GetABEntriesToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetABEntriesToCopy();
	}
	return nullptr;
}

void CMapiObjects::SetMessagesToCopy(_In_ LPENTRYLIST lpMessagesToCopy, _In_ LPMAPIFOLDER lpSourceParent) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetMessagesToCopy(lpMessagesToCopy, lpSourceParent);
	}
}

_Check_return_ LPENTRYLIST CMapiObjects::GetMessagesToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetMessagesToCopy();
	}
	return nullptr;
}

void CMapiObjects::SetFolderToCopy(_In_ LPMAPIFOLDER lpFolderToCopy, _In_ LPMAPIFOLDER lpSourceParent) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetFolderToCopy(lpFolderToCopy, lpSourceParent);
	}
}

_Check_return_ LPMAPIFOLDER CMapiObjects::GetFolderToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetFolderToCopy();
	}
	return nullptr;
}

_Check_return_ LPMAPIFOLDER CMapiObjects::GetSourceParentFolder() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetSourceParentFolder();
	}
	return nullptr;
}

void CMapiObjects::SetPropertyToCopy(ULONG ulPropTag, _In_ LPMAPIPROP lpSourcePropObject) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetPropertyToCopy(ulPropTag, lpSourcePropObject);
	}
}

_Check_return_ ULONG CMapiObjects::GetPropertyToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetPropertyToCopy();
	}
	return 0;
}

_Check_return_ LPMAPIPROP CMapiObjects::GetSourcePropObject() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetSourcePropObject();
	}
	return nullptr;
}

void CMapiObjects::SetAttachmentsToCopy(_In_ LPMESSAGE lpMessage, ULONG ulNumAttachments, _In_ ULONG* lpAttNumList) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetAttachmentsToCopy(lpMessage, ulNumAttachments, lpAttNumList);
	}
}

_Check_return_ ULONG* CMapiObjects::GetAttachmentsToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetAttachmentsToCopy();
	}
	return nullptr;
}

_Check_return_ ULONG CMapiObjects::GetNumAttachments() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetNumAttachments();
	}
	return 0;
}

void CMapiObjects::SetProfileToCopy(_In_ string szProfileName) const
{
	if (m_lpGlobalCache)
	{
		m_lpGlobalCache->SetProfileToCopy(szProfileName);
	}
}

_Check_return_ string CMapiObjects::GetProfileToCopy() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetProfileToCopy();
	}
	return "";
}

_Check_return_ ULONG CMapiObjects::GetBufferStatus() const
{
	if (m_lpGlobalCache)
	{
		return m_lpGlobalCache->GetBufferStatus();
	}
	return BUFFER_EMPTY;
}
