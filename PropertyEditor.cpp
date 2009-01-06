// PropertyEditor.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor.h"
#include "InterpretProp.h"
#include "InterpretProp2.h"
#include "MAPIFunctions.h"

enum __PropertyEditorTypes
{
	EDITOR_SINGLE,
		EDITOR_MULTI,
};

static TCHAR* CLASS = _T("CPropertyEditor");

// Create an editor for a MAPI property
CPropertyEditor::CPropertyEditor(
								 CWnd* pParentWnd,
								 UINT uidTitle,
								 UINT uidPrompt,
								 BOOL bIsAB,
								 LPVOID lpAllocParent):
CEditor(pParentWnd,uidTitle,uidPrompt,0,CEDITOR_BUTTON_OK|CEDITOR_BUTTON_CANCEL)
{
	TRACE_CONSTRUCTOR(CLASS);

	m_bIsAB = bIsAB;
	m_lpAllocParent = lpAllocParent;
	m_ulEditorType = EDITOR_SINGLE; // default to this, we'll change it later if we find we've got a MV prop
	m_lpMAPIProp = NULL;
	m_lpsInputValue = NULL;
	m_lpsOutputValue = NULL;
	m_bShouldFreeOutputValue = false;
	m_bDirty = false;
	m_bShouldFreeInputValue = false;
}


// Takes LPMAPIPROP and ulPropTag as input - will pull SPropValue from the LPMAPIPROP
// Takes LPSPropValue as input - determines property tag from this
void CPropertyEditor::InitPropValue(
									LPMAPIPROP lpMAPIProp,
									ULONG ulPropTag,
									LPSPropValue lpsPropValue)
{
	HRESULT hRes = S_OK;
	if (m_lpMAPIProp) m_lpMAPIProp->Release();
	m_lpMAPIProp = lpMAPIProp;
	if (m_lpMAPIProp) m_lpMAPIProp->AddRef();
	m_ulPropTag = ulPropTag;
	m_lpsInputValue = lpsPropValue;

	// We got a MAPI prop object and no input value, go look one up
	if (m_lpMAPIProp && !m_lpsInputValue)
	{
		SPropTagArray sTag = {0};
		sTag.cValues = 1;
		sTag.aulPropTag[0] = (PT_ERROR == PROP_TYPE(m_ulPropTag))?CHANGE_PROP_TYPE(m_ulPropTag,PT_UNSPECIFIED):m_ulPropTag;
		ULONG ulValues = NULL;

		WC_H(m_lpMAPIProp->GetProps(&sTag,NULL,&ulValues,&m_lpsInputValue));

		// Suppress MAPI_E_NOT_FOUND error when the source type is non error
		if (m_lpsInputValue && 
			PT_ERROR == PROP_TYPE(m_lpsInputValue->ulPropTag) &&
			MAPI_E_NOT_FOUND == m_lpsInputValue->Value.err &&
			PT_ERROR != PROP_TYPE(m_ulPropTag)
			)
		{
			MAPIFreeBuffer(m_lpsInputValue);
			m_lpsInputValue = NULL;
		}

		// In all cases where we got a value back, we need to reset our property tag to the value we got
		// This will address when the source is PT_UNSPECIFIED, when the returned value is PT_ERROR,
		// or any other case where the returned value has a different type than requested
		if (SUCCEEDED(hRes) && m_lpsInputValue)
			m_ulPropTag = m_lpsInputValue->ulPropTag;

		m_bShouldFreeInputValue = true;
	}
	else if (m_lpsInputValue && !m_ulPropTag)
	{
		m_ulPropTag = m_lpsInputValue->ulPropTag;
	}

	if (PROP_TYPE(m_ulPropTag) & MV_FLAG) m_ulEditorType = EDITOR_MULTI;
	CString szPromptPostFix;
	szPromptPostFix.Format(_T("\r\n%s"),TagToString(m_ulPropTag,m_lpMAPIProp,m_bIsAB,false)); // STRING_OK

	SetPromptPostFix(szPromptPostFix);

	// Let's crack our property open and see what kind of controls we'll need for it
	CreatePropertyControls();

	InitPropertyControls();
}

CPropertyEditor::~CPropertyEditor()
{
	TRACE_DESTRUCTOR(CLASS);

	if (m_bShouldFreeInputValue)
		MAPIFreeBuffer(m_lpsInputValue);

	if (m_bShouldFreeOutputValue)
		MAPIFreeBuffer(m_lpsOutputValue);

	if (m_lpMAPIProp) m_lpMAPIProp->Release();
}

BOOL CPropertyEditor::OnInitDialog()
{
	BOOL bRet = CEditor::OnInitDialog();

	if (EDITOR_MULTI == m_ulEditorType)
	{
		ReadMultiValueStringsFromProperty(0); // TODO: handle multiple lists
		UpdateListButtons();
	}

	m_bDirty = false;
	return bRet;
}

void CPropertyEditor::OnOK()
{
	// This is where we write our changes back
	if (EDITOR_SINGLE == m_ulEditorType)
	{
		WriteStringsToSPropValue();
	}
	else
	{
		WriteMultiValueStringsToSPropValue(0); // todo: handle multiple lists
	}
	WriteSPropValueToObject();
	CDialog::OnOK(); // don't need to call CEditor::OnOK
}

void CPropertyEditor::CreatePropertyControls()
{
	if (EDITOR_MULTI == m_ulEditorType)
	{
		CreateControls(1);
		return;
	}
	switch (PROP_TYPE(m_ulPropTag))
	{
	case(PT_APPTIME):
	case(PT_BOOLEAN):
	case(PT_DOUBLE):
	case(PT_OBJECT):
	case(PT_R4):
		CreateControls(1);
		break;
	case(PT_ERROR):
		CreateControls(2);
		break;
	case(PT_BINARY):
		CreateControls(4);
		break;
	case(PT_CURRENCY):
	case(PT_I8):
	case(PT_LONG):
	case(PT_I2):
	case(PT_SYSTIME):
	case(PT_STRING8):
	case(PT_UNICODE):
		CreateControls(3);
		break;
	case(PT_CLSID):
		CreateControls(1);
		break;
	case(PT_ACTIONS):
		CreateControls(1);
		break;
	case(PT_SRESTRICTION):
		CreateControls(1);
		break;
	default:
		CreateControls(2);
		break;
	}
}

void CPropertyEditor::InitPropertyControls()
{
	if (EDITOR_MULTI == m_ulEditorType)
	{
		InitList(0,IDS_PROPVALUES,false,false);
		return;
	}

	LPTSTR szSmartView = NULL;

	InterpretProp(m_lpsInputValue,
		m_ulPropTag,
		m_lpMAPIProp,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&szSmartView, // Built from lpProp & lpMAPIProp
		NULL,
		NULL,
		NULL);

	CString szTemp1;
	CString szTemp2;
	switch (PROP_TYPE(m_ulPropTag))
	{
	case(PT_APPTIME):
		InitSingleLine(0,IDS_UNSIGNEDDECIMAL,NULL,false);
		if (m_lpsInputValue)
		{
			SetStringf(0,_T("%u"),m_lpsInputValue->Value.at); // STRING_OK
		}
		else
		{
			SetDecimal(0,0);
		}
		break;
	case(PT_BOOLEAN):
		InitCheck(0,IDS_BOOLEAN,m_lpsInputValue?m_lpsInputValue->Value.b:false,false);
		break;
	case(PT_DOUBLE):
		InitSingleLine(0,IDS_DOUBLE,NULL,false);
		if (m_lpsInputValue)
		{
			SetStringf(0,_T("%f"),m_lpsInputValue->Value.dbl); // STRING_OK
		}
		else
		{
			SetDecimal(0,0);
		}
		break;
	case(PT_OBJECT):
		InitSingleLine(0,IDS_OBJECT,IDS_OBJECTVALUE,true);
		break;
	case(PT_R4):
		InitSingleLine(0,IDS_FLOAT,NULL,false);
		if (m_lpsInputValue)
		{
			SetStringf(0,_T("%f"),m_lpsInputValue->Value.flt); // STRING_OK
		}
		else
		{
			SetDecimal(0,0);
		}
		break;
	case(PT_STRING8):
		InitMultiLine(0,IDS_ANSISTRING,NULL,false);
		InitSingleLine(1,IDS_CB,NULL,true);
		SetSize(1,0);
		InitMultiLine(2,IDS_BIN,NULL,false);
		if (m_lpsInputValue && CheckStringProp(m_lpsInputValue,PT_STRING8))
		{
			SetStringA(0,m_lpsInputValue->Value.lpszA);

			size_t cbStr = 0;
			HRESULT hRes = S_OK;
			EC_H(StringCbLengthA(m_lpsInputValue->Value.lpszA,STRSAFE_MAX_CCH * sizeof(char),&cbStr));

			SetSize(1, cbStr);

			SetBinary(
				2,
				(LPBYTE) m_lpsInputValue->Value.lpszA,
				cbStr);
		}

		break;
	case(PT_UNICODE):
		InitMultiLine(0,IDS_UNISTRING,NULL,false);
		InitSingleLine(1,IDS_CB,NULL,true);
		SetSize(1,0);
		InitMultiLine(2,IDS_BIN,NULL,false);
		if (m_lpsInputValue && CheckStringProp(m_lpsInputValue,PT_UNICODE))
		{
			SetStringW(0, m_lpsInputValue->Value.lpszW);

			size_t cbStr = 0;
			HRESULT hRes = S_OK;
			EC_H(StringCbLengthW(m_lpsInputValue->Value.lpszW,STRSAFE_MAX_CCH * sizeof(WCHAR),&cbStr));

			SetSize(1, cbStr);

			SetBinary(
				2,
				(LPBYTE) m_lpsInputValue->Value.lpszW,
				cbStr);
		}

		break;
	case(PT_CURRENCY):
		InitSingleLine(0,IDS_HI,NULL,false);
		InitSingleLine(1,IDS_LO,NULL,false);
		InitSingleLine(2,IDS_CURRENCY,NULL,false);
		if (m_lpsInputValue)
		{
			SetHex(0,m_lpsInputValue->Value.cur.Hi);
			SetHex(1,m_lpsInputValue->Value.cur.Lo);
			SetString(2,CurrencyToString(m_lpsInputValue->Value.cur));
		}
		else
		{
			SetHex(0,0);
			SetHex(1,0);
			SetString(2,_T("0.0000")); // STRING_OK
		}
		break;
	case(PT_ERROR):
		InitSingleLine(0,IDS_ERRORCODEHEX,NULL,true);
		InitSingleLine(1,IDS_ERRORNAME,NULL,true);
		if (m_lpsInputValue)
		{
			SetHex(0, m_lpsInputValue->Value.err);
			SetString(1,ErrorNameFromErrorCode(m_lpsInputValue->Value.err));
		}
		break;
	case(PT_I2):
		InitSingleLine(0,IDS_SIGNEDDECIMAL,NULL,false);
		InitSingleLine(1,IDS_HEX,NULL,false);
		InitMultiLine(2,IDS_COLSMART_VIEW,NULL,true);
		if (m_lpsInputValue)
		{
			SetDecimal(0,m_lpsInputValue->Value.i);
			SetHex(1,m_lpsInputValue->Value.i);

			if (szSmartView) SetString(2,szSmartView);
		}
		else
		{
			SetDecimal(0,0);
			SetHex(1,0);
			SetHex(2,0);
		}
		break;
	case(PT_I8):
		InitSingleLine(0,IDS_HIGHPART,NULL,false);
		InitSingleLine(1,IDS_LOWPART,NULL,false);
		InitSingleLine(2,IDS_DECIMAL,NULL,false);

		if (m_lpsInputValue)
		{
			SetHex(0,(int) m_lpsInputValue->Value.li.HighPart);
			SetHex(1,(int) m_lpsInputValue->Value.li.LowPart);
			SetStringf(2,_T("%I64d"),m_lpsInputValue->Value.li.QuadPart); // STRING_OK
		}
		else
		{
			SetHex(0,0);
			SetHex(1,0);
			SetDecimal(2,0);
		}
		break;
	case(PT_BINARY):
		InitSingleLine(0,IDS_CB,NULL,true);
		SetHex(0,0);
		if (m_lpsInputValue)
		{
			SetSize(0,m_lpsInputValue->Value.bin.cb);
			InitMultiLine(1,IDS_BIN,BinToHexString(&m_lpsInputValue->Value.bin,false),false);
			InitMultiLine(2,IDS_TEXT,BinToTextString(&m_lpsInputValue->Value.bin,true),true);
			InitMultiLine(3,IDS_COLSMART_VIEW,szSmartView,true);
		}
		else
		{
			InitMultiLine(1,IDS_BIN,NULL,false);
			InitMultiLine(2,IDS_TEXT,NULL,true);
			InitMultiLine(3,IDS_COLSMART_VIEW,szSmartView,true);
		}
		break;
	case(PT_LONG):
		InitSingleLine(0,IDS_UNSIGNEDDECIMAL,NULL,false);
		InitSingleLine(1,IDS_HEX,NULL,false);
		InitMultiLine(2,IDS_COLSMART_VIEW,NULL,true);
		if (m_lpsInputValue)
		{
			SetStringf(0,_T("%u"),m_lpsInputValue->Value.l); // STRING_OK
			SetHex(1,m_lpsInputValue->Value.l);
			if (szSmartView) SetString(2,szSmartView);
		}
		else
		{
			SetDecimal(0,0);
			SetHex(1,0);
			SetHex(2,0);
		}
		break;
	case(PT_SYSTIME):
		InitSingleLine(0,IDS_LOWDATETIME,NULL,false);
		InitSingleLine(1,IDS_HIGHDATETIME,NULL,false);
		InitSingleLine(2,IDS_DATE,NULL,true);
		if (m_lpsInputValue)
		{
			SetHex(0,(int) m_lpsInputValue->Value.ft.dwLowDateTime);
			SetHex(1,(int) m_lpsInputValue->Value.ft.dwHighDateTime);
			FileTimeToString(&m_lpsInputValue->Value.ft,&szTemp1,NULL);
			SetString(2,szTemp1);
		}
		else
		{
			SetHex(0,0);
			SetHex(1,0);
		}
		break;
	case(PT_CLSID):
		{
			InitSingleLine(0,IDS_GUID,NULL,false);
			LPTSTR szGuid = NULL;
			if (m_lpsInputValue)
			{
				szGuid = GUIDToStringAndName(m_lpsInputValue->Value.lpguid);
			}
			else
			{
				szGuid = GUIDToStringAndName(0);
			}
			SetString(0,szGuid);
			delete[] szGuid;
		}
		break;
	case(PT_SRESTRICTION):
		InitMultiLine(0,IDS_RESTRICTION,NULL,true);
		InterpretProp(m_lpsInputValue,&szTemp1,NULL);
		SetString(0,szTemp1);
		break;
	case(PT_ACTIONS):
		InitMultiLine(0,IDS_ACTIONS,NULL,true);
		InterpretProp(m_lpsInputValue,&szTemp1,NULL);
		SetString(0,szTemp1);
		break;
	default:
		InterpretProp(
			m_lpsInputValue,
			&szTemp1,
			&szTemp2);
		InitMultiLine(0,IDS_VALUE,szTemp1,true);
		InitMultiLine(1,IDS_ALTERNATEVIEW,szTemp2,true);
		break;
	}
	delete[] szSmartView;
}

// Function must be called AFTER dialog controls have been created, not before
void CPropertyEditor::ReadMultiValueStringsFromProperty(ULONG ulListNum)
{
	if (EDITOR_MULTI != m_ulEditorType) return;
	if (!IsValidList(ulListNum)) return;

	InsertColumn(ulListNum,0,IDS_ENTRY);
	InsertColumn(ulListNum,1,IDS_VALUE);
	InsertColumn(ulListNum,2,IDS_ALTERNATEVIEW);

	if (!m_lpsInputValue) return;
	if (!(PROP_TYPE(m_lpsInputValue->ulPropTag) & MV_FLAG)) return;

	CString szTmp;
	CString	szAltTmp;
	ULONG iMVCount = 0;
	// All the MV structures are basically the same, so we can cheat when we pull the count
	ULONG cValues = m_lpsInputValue->Value.MVi.cValues;
	for (iMVCount = 0; iMVCount < cValues; iMVCount++)
	{
		szTmp.Format(_T("%d"),iMVCount); // STRING_OK
		SortListData* lpData = InsertListRow(ulListNum,iMVCount,szTmp);

		InterpretMVProp(m_lpsInputValue,iMVCount,&szTmp,&szAltTmp);
		SetListString(ulListNum,iMVCount,1,szTmp);
		SetListString(ulListNum,iMVCount,2,szAltTmp);
		if (lpData)
		{
			lpData->ulSortDataType = SORTLIST_MVPROP;
			switch(PROP_TYPE(m_lpsInputValue->ulPropTag))
			{
			case(PT_MV_I2):
				lpData->data.MV.val.i = m_lpsInputValue->Value.MVi.lpi[iMVCount];
				break;
			case(PT_MV_LONG):
				lpData->data.MV.val.l = m_lpsInputValue->Value.MVl.lpl[iMVCount];
				break;
			case(PT_MV_DOUBLE):
				lpData->data.MV.val.dbl = m_lpsInputValue->Value.MVdbl.lpdbl[iMVCount];
				break;
			case(PT_MV_CURRENCY):
				lpData->data.MV.val.cur = m_lpsInputValue->Value.MVcur.lpcur[iMVCount];
				break;
			case(PT_MV_APPTIME):
				lpData->data.MV.val.at = m_lpsInputValue->Value.MVat.lpat[iMVCount];
				break;
			case(PT_MV_SYSTIME):
				lpData->data.MV.val.ft = m_lpsInputValue->Value.MVft.lpft[iMVCount];
				break;
			case(PT_MV_I8):
				lpData->data.MV.val.li = m_lpsInputValue->Value.MVli.lpli[iMVCount];
				break;
			case(PT_MV_R4):
				lpData->data.MV.val.flt = m_lpsInputValue->Value.MVflt.lpflt[iMVCount];
				break;
			case(PT_MV_STRING8):
				lpData->data.MV.val.lpszA = m_lpsInputValue->Value.MVszA.lppszA[iMVCount];
				break;
			case(PT_MV_UNICODE):
				lpData->data.MV.val.lpszW = m_lpsInputValue->Value.MVszW.lppszW[iMVCount];
				break;
			case(PT_MV_BINARY):
				lpData->data.MV.val.bin = m_lpsInputValue->Value.MVbin.lpbin[iMVCount];
				break;
			case(PT_MV_CLSID):
				lpData->data.MV.val.lpguid = &m_lpsInputValue->Value.MVguid.lpguid[iMVCount];
				break;
			default:
				break;
			}
			lpData->bItemFullyLoaded = true;
		}
	}
	ResizeList(ulListNum,false);
} // CPropertyEditor::ReadMultiValueStringsFromProperty

void CPropertyEditor::WriteMultiValueStringsToSPropValue(ULONG ulListNum)
{
	if (EDITOR_MULTI != m_ulEditorType) return;
	if (!IsValidList(ulListNum)) return;

	// If we're not dirty, don't write
	if (!ListDirty(ulListNum)) return;

	HRESULT hRes = S_OK;
	// Take care of allocations first
	if (!m_lpsOutputValue)
	{
		if (m_lpAllocParent)
		{
			EC_H(MAPIAllocateMore(
				sizeof(SPropValue),
				m_lpAllocParent,
				(LPVOID*) &m_lpsOutputValue));
		}
		else
		{
			EC_H(MAPIAllocateBuffer(
				sizeof(SPropValue),
				(LPVOID*) &m_lpsOutputValue));
			m_lpAllocParent = m_lpsOutputValue;
		}
	}
	if (m_lpsOutputValue)
	{
		ULONG ulNumVals = GetListCount(ulListNum);
		ULONG iMVCount = 0;

		m_lpsOutputValue->ulPropTag = m_ulPropTag;
		m_lpsOutputValue->dwAlignPad = NULL;

		switch(PROP_TYPE(m_lpsOutputValue->ulPropTag))
		{
		case(PT_MV_I2):
			EC_H(MAPIAllocateMore(sizeof(short int) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVi.lpi));
			m_lpsOutputValue->Value.MVi.cValues = ulNumVals;
			break;
		case(PT_MV_LONG):
			EC_H(MAPIAllocateMore(sizeof(LONG) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVl.lpl));
			m_lpsOutputValue->Value.MVl.cValues = ulNumVals;
			break;
		case(PT_MV_DOUBLE):
			EC_H(MAPIAllocateMore(sizeof(double) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVdbl.lpdbl));
			m_lpsOutputValue->Value.MVdbl.cValues = ulNumVals;
			break;
		case(PT_MV_CURRENCY):
			EC_H(MAPIAllocateMore(sizeof(CURRENCY) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVcur.lpcur));
			m_lpsOutputValue->Value.MVcur.cValues = ulNumVals;
			break;
		case(PT_MV_APPTIME):
			EC_H(MAPIAllocateMore(sizeof(double) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVat.lpat));
			m_lpsOutputValue->Value.MVat.cValues = ulNumVals;
			break;
		case(PT_MV_SYSTIME):
			EC_H(MAPIAllocateMore(sizeof(FILETIME) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVft.lpft));
			m_lpsOutputValue->Value.MVft.cValues = ulNumVals;
			break;
		case(PT_MV_I8):
			EC_H(MAPIAllocateMore(sizeof(LARGE_INTEGER) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVli.lpli));
			m_lpsOutputValue->Value.MVli.cValues = ulNumVals;
			break;
		case(PT_MV_R4):
			EC_H(MAPIAllocateMore(sizeof(float) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVflt.lpflt));
			m_lpsOutputValue->Value.MVflt.cValues = ulNumVals;
			break;
		case(PT_MV_STRING8):
			EC_H(MAPIAllocateMore(sizeof(LPSTR) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVszA.lppszA));
			m_lpsOutputValue->Value.MVszA.cValues = ulNumVals;
			break;
		case(PT_MV_UNICODE):
			EC_H(MAPIAllocateMore(sizeof(LPWSTR) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVszW.lppszW));
			m_lpsOutputValue->Value.MVszW.cValues = ulNumVals;
			break;
		case(PT_MV_BINARY):
			EC_H(MAPIAllocateMore(sizeof(SBinary) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVbin.lpbin));
			m_lpsOutputValue->Value.MVbin.cValues = ulNumVals;
			break;
		case(PT_MV_CLSID):
			EC_H(MAPIAllocateMore(sizeof(GUID) * ulNumVals,m_lpAllocParent,(LPVOID*)&m_lpsOutputValue->Value.MVguid.lpguid));
			m_lpsOutputValue->Value.MVguid.cValues = ulNumVals;
			break;
		default:
			break;
		}
		// Allocation is now done

		// Now write our data into the space we allocated
		for (iMVCount = 0; iMVCount < ulNumVals; iMVCount++)
		{
			SortListData* lpData = GetListRowData(ulListNum,iMVCount);

			if (lpData)
			{
				switch(PROP_TYPE(m_lpsOutputValue->ulPropTag))
				{
				case(PT_MV_I2):
					m_lpsOutputValue->Value.MVi.lpi[iMVCount] = lpData->data.MV.val.i;
					break;
				case(PT_MV_LONG):
					m_lpsOutputValue->Value.MVl.lpl[iMVCount] = lpData->data.MV.val.l;
					break;
				case(PT_MV_DOUBLE):
					m_lpsOutputValue->Value.MVdbl.lpdbl[iMVCount] = lpData->data.MV.val.dbl;
					break;
				case(PT_MV_CURRENCY):
					m_lpsOutputValue->Value.MVcur.lpcur[iMVCount] = lpData->data.MV.val.cur;
					break;
				case(PT_MV_APPTIME):
					m_lpsOutputValue->Value.MVat.lpat[iMVCount] = lpData->data.MV.val.at;
					break;
				case(PT_MV_SYSTIME):
					m_lpsOutputValue->Value.MVft.lpft[iMVCount] = lpData->data.MV.val.ft;
					break;
				case(PT_MV_I8):
					m_lpsOutputValue->Value.MVli.lpli[iMVCount] = lpData->data.MV.val.li;
					break;
				case(PT_MV_R4):
					m_lpsOutputValue->Value.MVflt.lpflt[iMVCount] = lpData->data.MV.val.flt;
					break;
				case(PT_MV_STRING8):
					m_lpsOutputValue->Value.MVszA.lppszA[iMVCount] = lpData->data.MV.val.lpszA;
					break;
				case(PT_MV_UNICODE):
					m_lpsOutputValue->Value.MVszW.lppszW[iMVCount] = lpData->data.MV.val.lpszW;
					break;
				case(PT_MV_BINARY):
					m_lpsOutputValue->Value.MVbin.lpbin[iMVCount] = lpData->data.MV.val.bin;
					break;
				case(PT_MV_CLSID):
					if (lpData->data.MV.val.lpguid)
					{
						m_lpsOutputValue->Value.MVguid.lpguid[iMVCount] = *lpData->data.MV.val.lpguid;
					}

					break;
				default:
					break;
				}
			}
		}
	}
}

void CPropertyEditor::WriteStringsToSPropValue()
{
	if (EDITOR_SINGLE != m_ulEditorType) return;

	HRESULT hRes = S_OK;
	CString szTmpString;
	ULONG ulStrLen = NULL;

	// Check first if we'll have anything to write
	switch (PROP_TYPE(m_ulPropTag))
	{
	case(PT_OBJECT): // Nothing to write back - not supported
	case(PT_SRESTRICTION):
	case(PT_ACTIONS):
		return;
	case (PT_BINARY):
		// Check that we've got valid binary before we allocate anything. Note that we're
		// reading szTmpString now and will assume it's read when we get to the real PT_BINARY case
		szTmpString = GetStringUseControl(1);
		ulStrLen = szTmpString.GetLength();
		if (ulStrLen & 1) return; // can't use an odd length string
	default: break;
	}

	// If nothing has changed, we're done.
	if (!m_bDirty) return;

	if (!m_lpsOutputValue)
	{
		if (m_lpAllocParent)
		{
			EC_H(MAPIAllocateMore(
				sizeof(SPropValue),
				m_lpAllocParent,
				(LPVOID*)&m_lpsOutputValue));
		}
		else
		{
			EC_H(MAPIAllocateBuffer(
				sizeof(SPropValue),
				(LPVOID*) &m_lpsOutputValue));
			m_lpAllocParent = m_lpsOutputValue;
		}
	}

	if (m_lpsOutputValue)
	{
		BOOL bFailed = false; // set true if we fail to get a prop and have to clean up memory
		m_lpsOutputValue->ulPropTag = m_ulPropTag;
		m_lpsOutputValue->dwAlignPad = NULL;
		switch (PROP_TYPE(m_ulPropTag))
		{
		case(PT_I2): // treat as signed long
			{
				short int iVal = 0;
				szTmpString = GetStringUseControl(0);
				iVal = (short int) _tcstol(szTmpString,NULL,10);
				m_lpsOutputValue->Value.i = iVal;
			}
			break;
		case(PT_LONG): // treat as unsigned long
			{
				LONG lVal = 0;
				szTmpString = GetStringUseControl(0);
				lVal = (LONG) _tcstoul(szTmpString,NULL,10);
				m_lpsOutputValue->Value.l = lVal;
			}
			break;
		case(PT_R4):
			{
				float fVal = 0;
				szTmpString = GetStringUseControl(0);
				fVal = (float) _tcstod(szTmpString,NULL);
				m_lpsOutputValue->Value.flt = fVal;
			}
			break;
		case(PT_DOUBLE):
			{
				double dVal = 0;
				szTmpString = GetStringUseControl(0);
				dVal = (double) _tcstod(szTmpString,NULL);
				m_lpsOutputValue->Value.dbl = dVal;
			}
			break;
		case(PT_CURRENCY):
			{
				CURRENCY curVal = {0};
				szTmpString = GetStringUseControl(0);
				curVal.Hi = _tcstoul(szTmpString,NULL,16);
				szTmpString = GetStringUseControl(1);
				curVal.Lo = _tcstoul(szTmpString,NULL,16);
				m_lpsOutputValue->Value.cur = curVal;
			}
			break;
		case(PT_APPTIME):
			{
				double atVal = 0;
				szTmpString = GetStringUseControl(0);
				atVal = (double) _tcstod(szTmpString,NULL);
				m_lpsOutputValue->Value.at = atVal;
			}
			break;
		case(PT_ERROR): // unsigned
			{
				SCODE errVal = 0;
				szTmpString = GetStringUseControl(0);
				errVal = (SCODE) _tcstoul(szTmpString,NULL,0);
				m_lpsOutputValue->Value.err = errVal;
			}
			break;
		case(PT_BOOLEAN):
			{
				BOOL bVal = false;
				bVal = GetCheckUseControl(0);
				m_lpsOutputValue->Value.b = (unsigned short) bVal;
			}
			break;
		case(PT_I8):
			{
				LARGE_INTEGER liVal = {0};
				szTmpString = GetStringUseControl(0);
				liVal.HighPart = (long) _tcstoul(szTmpString,NULL,0);
				szTmpString = GetStringUseControl(1);
				liVal.LowPart = (long) _tcstoul(szTmpString,NULL,0);
				m_lpsOutputValue->Value.li = liVal;
			}
			break;
		case(PT_STRING8):
			{
				m_lpsOutputValue->Value.lpszA = NULL;

				LPSTR lpszA = GetEditBoxTextA(0); // Get ascii text
				if (lpszA)
				{
					EC_H(CopyStringA(&m_lpsOutputValue->Value.lpszA,lpszA,m_lpAllocParent));
					if (FAILED(hRes)) bFailed = true;
				}
			}
			break;
		case(PT_UNICODE):
			{
				m_lpsOutputValue->Value.lpszW = NULL;

				LPWSTR lpszW = GetEditBoxTextW(0); // Get unicode text
				if (lpszW)
				{
					EC_H(CopyStringW(&m_lpsOutputValue->Value.lpszW,lpszW,m_lpAllocParent));
					if (FAILED(hRes)) bFailed = true;
				}
			}
			break;
		case(PT_SYSTIME):
			{
				FILETIME ftVal = {0};
				szTmpString = GetStringUseControl(0);
				ftVal.dwLowDateTime = _tcstoul(szTmpString,NULL,16);
				szTmpString = GetStringUseControl(1);
				ftVal.dwHighDateTime = _tcstoul(szTmpString,NULL,16);
				m_lpsOutputValue->Value.ft = ftVal;
			}
			break;
		case(PT_CLSID):
			{
				EC_H(MAPIAllocateMore(
					sizeof(GUID),
					m_lpAllocParent,
					(LPVOID*)&m_lpsOutputValue->Value.lpguid));
				if (m_lpsOutputValue->Value.lpguid)
				{
					szTmpString = GetStringUseControl(0);
					EC_H(StringToGUID((LPCTSTR) szTmpString,m_lpsOutputValue->Value.lpguid));
					if (FAILED(hRes)) bFailed = true;
				}
			}
			break;
		case(PT_BINARY):
			{
				// remember we already read szTmpString and ulStrLen and found ulStrLen was even
				ULONG cbBin = ulStrLen / 2;
				m_lpsOutputValue->Value.bin.cb = cbBin;
				if (0 == m_lpsOutputValue->Value.bin.cb)
				{
					m_lpsOutputValue->Value.bin.lpb = 0;
				}
				else
				{
					EC_H(MAPIAllocateMore(
						m_lpsOutputValue->Value.bin.cb,
						m_lpAllocParent,
						(LPVOID*)&m_lpsOutputValue->Value.bin.lpb));
					if (FAILED(hRes)) bFailed = true;
					else
					{
						MyBinFromHex(
							(LPCTSTR) szTmpString,
							m_lpsOutputValue->Value.bin.lpb,
							m_lpsOutputValue->Value.bin.cb);
					}
				}
			}
			break;
		default:
			// We shouldn't ever get here unless some new prop type shows up
			bFailed = true;
			break;
		}
		if (bFailed)
		{
			// If we don't have a parent or we are the parent, then we can free here
			if (!m_lpAllocParent || m_lpAllocParent == m_lpsOutputValue)
			{
				MAPIFreeBuffer(m_lpsOutputValue);
				m_lpsOutputValue = NULL;
				m_lpAllocParent = NULL;
			}
			else
			{
				// If m_lpsOutputValue was allocated off a parent, we can't free it here
				// Just drop the reference and m_lpAllocParent's free will clean it up
				m_lpsOutputValue = NULL;
			}
		}
	}
}

void CPropertyEditor::WriteSPropValueToObject()
{
	if (!m_lpsOutputValue || !m_lpMAPIProp) return;

	HRESULT hRes = S_OK;

	LPSPropProblemArray lpProblemArray = NULL;

	EC_H(m_lpMAPIProp->SetProps(
		1,
		m_lpsOutputValue,
		&lpProblemArray));

	EC_PROBLEMARRAY(lpProblemArray);
	MAPIFreeBuffer(lpProblemArray);

	EC_H(m_lpMAPIProp->SaveChanges(KEEP_OPEN_READWRITE));
} // CPropertyEditor::WriteSPropValueToObject

// Callers beware: Detatches and returns the modified prop value - this must be MAPIFreeBuffered!
LPSPropValue CPropertyEditor::DetachModifiedSPropValue()
{
	LPSPropValue m_lpRet = m_lpsOutputValue;
	m_lpsOutputValue = NULL;
	return m_lpRet;
} // CPropertyEditor::DetachModifiedSPropValue

BOOL CPropertyEditor::DoListEdit(ULONG ulListNum, int iItem, SortListData* lpData)
{
	if (!lpData) return false;
	if (!IsValidList(ulListNum)) return false;

	HRESULT hRes = S_OK;
	SPropValue tmpPropVal = {0};
	tmpPropVal.ulPropTag = PROP_TYPE(m_ulPropTag) & ~MV_FLAG;
	tmpPropVal.Value = lpData->data.MV.val;

	CPropertyEditor SingleProp(this, IDS_EDITROW,IDS_EDITROWPROMPT,m_bIsAB,m_lpAllocParent);
	SingleProp.InitPropValue(NULL,NULL,&tmpPropVal);

	WC_H(SingleProp.DisplayDialog());

	if (S_OK == hRes)
	{
		LPSPropValue lpNewValue = SingleProp.DetachModifiedSPropValue();

		if (lpNewValue)
		{
			ULONG ulBufSize = 0;
			// This handles most cases by default - cases needing a buffer copied are handled below
			lpData->data.MV.val = lpNewValue->Value;
			switch (lpNewValue->ulPropTag)
			{
			case(PT_STRING8):
				{
					// When the lpData is ultimately freed, MAPI will take care of freeing this.
					// This will be true even if we do this multiple times off the same lpData!
					size_t cbStr = 0;
					EC_H(StringCbLengthA(lpNewValue->Value.lpszA,STRSAFE_MAX_CCH * sizeof(char),&cbStr));
					cbStr += sizeof(char);

					EC_H(MAPIAllocateMore(
						(ULONG) cbStr,
						lpData,
						(LPVOID*) &lpData->data.MV.val.lpszA));

					if (S_OK == hRes)
					{
						memcpy(lpData->data.MV.val.lpszA,lpNewValue->Value.lpszA,cbStr);
					}
				}
				break;
			case(PT_UNICODE):
				{
					size_t cbStr = 0;
					EC_H(StringCbLengthW(lpNewValue->Value.lpszW,STRSAFE_MAX_CCH * sizeof(WCHAR),&cbStr));
					cbStr += sizeof(WCHAR);

					EC_H(MAPIAllocateMore(
						(ULONG) cbStr,
						lpData,
						(LPVOID*) &lpData->data.MV.val.lpszW));

					if (S_OK == hRes)
					{
						memcpy(lpData->data.MV.val.lpszW,lpNewValue->Value.lpszW,cbStr);
					}
				}
				break;
			case(PT_BINARY):
				ulBufSize = lpNewValue->Value.bin.cb;
				EC_H(MAPIAllocateMore(
					ulBufSize,
					lpData,
					(LPVOID*) &lpData->data.MV.val.bin.lpb));

				if (S_OK == hRes)
				{
					memcpy(lpData->data.MV.val.bin.lpb,lpNewValue->Value.bin.lpb,ulBufSize);
				}
				break;
			case(PT_CLSID):
				ulBufSize = sizeof(GUID);
				EC_H(MAPIAllocateMore(
					ulBufSize,
					lpData,
					(LPVOID*) &lpData->data.MV.val.lpguid));

				if (S_OK == hRes)
				{
					memcpy(lpData->data.MV.val.lpguid,lpNewValue->Value.lpguid,ulBufSize);
				}
				break;
			default:
				break;
			}

			// update the UI
			CString szTmp;
			CString szAltTmp;

			InterpretProp(lpNewValue,&szTmp,&szAltTmp);
			SetListString(ulListNum,iItem,1,szTmp);
			SetListString(ulListNum,iItem,2,szAltTmp);

			return true;
		}
		MAPIFreeBuffer(lpNewValue);
	}
	return false;
} // CPropertyEditor::DoListEdit

ULONG CPropertyEditor::HandleChange(UINT nID)
{
	ULONG i = CEditor::HandleChange(nID);

	if (EDITOR_SINGLE != m_ulEditorType || (ULONG) -1 == i) return (ULONG) -1;

	CString szTmpString;

	// If we get here, something changed - set the dirty flag
	m_bDirty = true;

	switch (PROP_TYPE(m_ulPropTag))
	{
	case(PT_I2): // signed 16 bit
		{
			short int iVal = 0;
			szTmpString = GetStringUseControl(i);
			if (0 == i)
			{
				iVal = (short int) _tcstol(szTmpString,NULL,10);
				SetHex(1, iVal);
			}
			else if (1 == i)
			{
				iVal = (short int) _tcstol(szTmpString,NULL,16);
				SetDecimal(0,iVal);
			}

			LPTSTR szSmartView = NULL;
			SPropValue sProp = {0};
			sProp.ulPropTag = m_ulPropTag;
			sProp.Value.i = iVal;

			InterpretProp(&sProp,
				m_ulPropTag,
				m_lpMAPIProp,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				&szSmartView,
				NULL,
				NULL,
				NULL);

			if (szSmartView) SetString(2,szSmartView);
			delete[] szSmartView;
			szSmartView = NULL;
		}
		break;
	case(PT_LONG): // unsigned 32 bit
		{
			LONG lVal = 0;
			szTmpString = GetStringUseControl(i);
			if (0 == i)
			{
				lVal = (LONG) _tcstoul(szTmpString,NULL,10);
				SetHex(1,lVal);
			}
			else if (1 == i)
			{
				lVal = (LONG) _tcstoul(szTmpString,NULL,16);
				SetStringf(0,_T("%u"),lVal); // STRING_OK
			}

			LPTSTR szSmartView = NULL;
			SPropValue sProp = {0};
			sProp.ulPropTag = m_ulPropTag;
			sProp.Value.l = lVal;

			InterpretProp(&sProp,
				m_ulPropTag,
				m_lpMAPIProp,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				&szSmartView,
				NULL,
				NULL,
				NULL);

			if (szSmartView) SetString(2,szSmartView);
			delete[] szSmartView;
			szSmartView = NULL;
		}
		break;
	case(PT_CURRENCY):
		{
			CURRENCY curVal = {0};
			if (0 == i || 1 == i)
			{
				szTmpString = GetStringUseControl(0);
				curVal.Hi = _tcstoul(szTmpString,NULL,16);
				szTmpString = GetStringUseControl(1);
				curVal.Lo= _tcstoul(szTmpString,NULL,16);
				SetString(2,CurrencyToString(curVal));
			}
			else if (2 == i)
			{
				szTmpString = GetStringUseControl(i);
				szTmpString.Remove(_T('.'));
				curVal.int64 = _ttoi64(szTmpString);
				SetHex(0,(int) curVal.Hi);
				SetHex(1,(int) curVal.Lo);
			}
		}
		break;
	case(PT_I8):
		{
			LARGE_INTEGER liVal = {0};
			if (0 == i || 1 == i)
			{
				szTmpString = GetStringUseControl(0);
				liVal.HighPart = (long) _tcstoul(szTmpString,NULL,0);
				szTmpString = GetStringUseControl(1);
				liVal.LowPart = (long) _tcstoul(szTmpString,NULL,0);
				SetStringf(2,_T("%I64d"),liVal.QuadPart); // STRING_OK
			}
			else if (2 == i)
			{
				szTmpString = GetStringUseControl(i);
				liVal.QuadPart = _ttoi64(szTmpString);
				SetHex(0,(int) liVal.HighPart);
				SetHex(1,(int) liVal.LowPart);
			}
		}
		break;
	case(PT_SYSTIME): // components are unsigned hex
		{
			FILETIME ftVal = {0};
			szTmpString = GetStringUseControl(0);
			ftVal.dwLowDateTime = _tcstoul(szTmpString,NULL,16);
			szTmpString = GetStringUseControl(1);
			ftVal.dwHighDateTime = _tcstoul(szTmpString,NULL,16);

			FileTimeToString(&ftVal,&szTmpString,NULL);
			SetString(2,szTmpString);
		}
		break;
	case(PT_BINARY):
		{
			LPBYTE	lpb = NULL;
			size_t	cb = 0;

			if (GetBinaryUseControl(1,&cb,&lpb))
			{
				SBinary Bin = {0};
				Bin.lpb = lpb;
				Bin.cb = (ULONG) cb;
				SetString(2,BinToTextString(&Bin,true));
				SetSize(0, cb);

				LPTSTR szSmartView = NULL;
				SPropValue sProp = {0};
				sProp.ulPropTag = m_ulPropTag;
				sProp.Value.bin = Bin;

				InterpretProp(&sProp,
					m_ulPropTag,
					m_lpMAPIProp,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					&szSmartView,
					NULL,
					NULL,
					NULL);

				SetString(3,szSmartView);
				delete[] szSmartView;
				szSmartView = NULL;
			}

			delete[] lpb;
		}
		break;
	case(PT_STRING8):
		{
			if (0 == i)
			{
				LPSTR lpszA = GetEditBoxTextA(0);

				HRESULT hRes = S_OK;
				size_t cbStr = 0;

				if (lpszA)
				{
					EC_H(StringCbLengthA(lpszA,STRSAFE_MAX_CCH * sizeof(char),&cbStr));
				}

				// Even if we don't have a string, still make the call to SetBinary
				// This will blank out the binary control when lpszA is NULL
				SetBinary(2,(LPBYTE) lpszA, cbStr);

				SetSize(1, cbStr);
			}
			else if (2 == i)
			{
				LPBYTE	lpb = NULL;
				size_t	cb = 0;

				if (GetBinaryUseControl(2,&cb,&lpb))
				{
					SetStringA(0,(LPCSTR) lpb);
					SetSize(1, cb);
				}
				delete[] lpb;
			}
		}
		break;
	case(PT_UNICODE):
		{
			if (0 == i)
			{
				LPWSTR lpszW = GetEditBoxTextW(0);
				HRESULT hRes = S_OK;
				size_t cbStr = 0;

				if (lpszW)
				{
					EC_H(StringCbLengthW(lpszW,STRSAFE_MAX_CCH * sizeof(WCHAR),&cbStr));
				}

				// Even if we don't have a string, still make the call to SetBinary
				// This will blank out the binary control when lpszW is NULL
				SetBinary(2,(LPBYTE) lpszW, cbStr);

				SetSize(1, cbStr);
			}
			else if (2 == i)
			{
				LPBYTE	lpb = NULL;
				size_t	cb = 0;

				if (GetBinaryUseControl(2,&cb,&lpb))
				{
					if (!(cb % sizeof(WCHAR)))
					{
						SetStringW(0,(LPCWSTR) lpb);
						SetSize(1, cb);
					}
				}
				delete[] lpb;
			}
		}
		break;
	default:
		break;
	}
	return i;
} // CPropertyEditor::HandleChange
