#pragma once
// Named Property Cache

// NamedPropCacheEntry
// =====================
//   An entry in the named property cache
typedef struct _NamedPropCacheEntry
{
	ULONG ulPropID;         // MAPI ID (ala PROP_ID) for a named property
	LPMAPINAMEID lpmniName; // guid, kind, value
	ULONG cbSig;            // Size and...
	LPBYTE lpSig;           // Value of PR_MAPPING_SIGNATURE
	BOOL bStringsCached;    // We have cached strings
	LPTSTR lpszPropName;    // Cached strings
	LPTSTR lpszPropGUID;    //
	LPTSTR lpszDASL;        //
} NamedPropCacheEntry, * LPNAMEDPROPCACHEENTRY;

// Cache initializes on demand. Uninitialize on program shutdown.
void UninitializeNamedPropCache();

_Check_return_ LPNAMEDPROPCACHEENTRY FindCacheEntry(ULONG ulPropID, _In_ LPGUID lpguid, ULONG ulKind, LONG lID, _In_z_ LPWSTR lpwstrName);

_Check_return_ HRESULT GetNamesFromIDs(_In_ LPMAPIPROP lpMAPIProp,
									   _In_ LPSPropTagArray* lppPropTags,
									   _In_opt_ LPGUID lpPropSetGuid,
									   ULONG ulFlags,
									   _Out_ ULONG* lpcPropNames,
									   _Out_ _Deref_post_cap_(*lpcPropNames) LPMAPINAMEID** lpppPropNames);
_Check_return_ HRESULT GetNamesFromIDs(_In_ LPMAPIPROP lpMAPIProp,
									   _In_opt_ LPSBinary lpMappingSignature,
									   _In_ LPSPropTagArray* lppPropTags,
									   _In_opt_ LPGUID lpPropSetGuid,
									   ULONG ulFlags,
									   _Out_ ULONG* lpcPropNames,
									   _Out_ _Deref_post_cap_(*lpcPropNames) LPMAPINAMEID** lpppPropNames);
_Check_return_ HRESULT GetIDsFromNames(_In_ LPMAPIPROP lpMAPIProp,
									   ULONG cPropNames,
									   _In_opt_count_(cPropNames) LPMAPINAMEID* lppPropNames,
									   ULONG ulFlags,
									   _Out_ _Deref_post_cap_(cPropNames) LPSPropTagArray* lppPropTags);

_Check_return_ inline BOOL fCacheNamedProps()
{
	return RegKeys[regkeyCACHE_NAME_DPROPS].ulCurDWORD != 0;
} // fCacheNamedProps