#include "stdafx.h"
#include "..\stdafx.h"
#include "FlatEntryList.h"
#include "..\String.h"

FlatEntryList::FlatEntryList(ULONG cbBin, _In_count_(cbBin) LPBYTE lpBin) : SmartViewParser(cbBin, lpBin)
{
	m_cEntries = 0;
	m_cbEntries = 0;
	m_pEntryIDs = NULL;
}

FlatEntryList::~FlatEntryList()
{
	if (m_pEntryIDs)
	{
		DWORD iFlatEntryList = 0;
		for (iFlatEntryList = 0; iFlatEntryList < m_cEntries; iFlatEntryList++)
		{
			DeleteEntryIdStruct(m_pEntryIDs[iFlatEntryList].lpEntryID);
			delete[] m_pEntryIDs[iFlatEntryList].JunkData;
		}
	}
	delete[] m_pEntryIDs;
}

void FlatEntryList::Parse()
{
	m_Parser.GetDWORD(&m_cEntries);

	// We read and report this, but ultimately, it's not used.
	m_Parser.GetDWORD(&m_cbEntries);

	if (m_cEntries && m_cEntries < _MaxEntriesLarge)
	{
		m_pEntryIDs = new FlatEntryIDStruct[m_cEntries];
		if (m_pEntryIDs)
		{
			memset(m_pEntryIDs, 0, m_cEntries * sizeof(FlatEntryIDStruct));

			DWORD iFlatEntryList = 0;
			for (iFlatEntryList = 0; iFlatEntryList < m_cEntries; iFlatEntryList++)
			{
				// Size here will be the length of the serialized entry ID
				// We'll have to round it up to a multiple of 4 to read off padding
				m_Parser.GetDWORD(&m_pEntryIDs[iFlatEntryList].dwSize);
				size_t ulSize = m_Parser.RemainingBytes();
				ulSize = min(m_pEntryIDs[iFlatEntryList].dwSize, ulSize);

				m_pEntryIDs[iFlatEntryList].lpEntryID = BinToEntryIdStruct(
					(ULONG)ulSize,
					m_Parser.GetCurrentAddress());
				m_Parser.Advance(ulSize);

				DWORD dwPAD = 3 - ((m_pEntryIDs[iFlatEntryList].dwSize + 3) % 4);
				if (dwPAD > 0)
				{
					m_pEntryIDs[iFlatEntryList].JunkDataSize = dwPAD;
					m_Parser.GetBYTES(m_pEntryIDs[iFlatEntryList].JunkDataSize, m_pEntryIDs[iFlatEntryList].JunkDataSize, &m_pEntryIDs[iFlatEntryList].JunkData);
				}
			}
		}
	}
}

_Check_return_ wstring FlatEntryList::ToStringInternal()
{
	wstring szFlatEntryList;
	wstring szTmp;

	szFlatEntryList = formatmessage(
		IDS_FELHEADER,
		m_cEntries,
		m_cbEntries);

	if (m_pEntryIDs)
	{
		DWORD iFlatEntryList = 0;
		for (iFlatEntryList = 0; iFlatEntryList < m_cEntries; iFlatEntryList++)
		{
			szTmp = formatmessage(
				IDS_FELENTRYHEADER,
				iFlatEntryList,
				m_pEntryIDs[iFlatEntryList].dwSize);
			szFlatEntryList += szTmp;
			if (m_pEntryIDs[iFlatEntryList].lpEntryID)
			{
				LPWSTR szEID = EntryIdStructToString(m_pEntryIDs[iFlatEntryList].lpEntryID);
				szFlatEntryList += szEID;
				delete[] szEID;
			}

			if (m_pEntryIDs[iFlatEntryList].JunkDataSize)
			{
				szTmp = formatmessage(
					IDS_FELENTRYPADDING,
					iFlatEntryList);
				szFlatEntryList += szTmp;
				szFlatEntryList += JunkDataToString(m_pEntryIDs[iFlatEntryList].JunkDataSize, m_pEntryIDs[iFlatEntryList].JunkData);
			}
			szFlatEntryList += L"\r\n"; // STRING_OK
		}
	}

	return szFlatEntryList;
}