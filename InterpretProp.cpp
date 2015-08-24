#include "stdafx.h"
#include "InterpretProp.h"
#include "MAPIFunctions.h"
#include "InterpretProp2.h"
#include "ExtraPropTags.h"
#include "NamedPropCache.h"
#include "SmartView\SmartView.h"
#include "ParseProperty.h"
#include "String.h"

static const char pBase64[] = {
	0x3e, 0x7f, 0x7f, 0x7f, 0x3f, 0x34, 0x35, 0x36,
	0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x7f,
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x01,
	0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
	0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33
};

// allocates output buffer with new
// delete with delete[]
// suprisingly, this algorithm works in a unicode build as well
_Check_return_ HRESULT Base64Decode(_In_z_ LPCTSTR szEncodedStr, _Inout_ size_t* cbBuf, _Out_ _Deref_post_cap_(*cbBuf) LPBYTE* lpDecodedBuffer)
{
	HRESULT hRes = S_OK;
	size_t	cchLen = 0;

	EC_H(StringCchLength(szEncodedStr, STRSAFE_MAX_CCH, &cchLen));

	if (cchLen % 4) return MAPI_E_INVALID_PARAMETER;

	// look for padding at the end
	static const TCHAR szPadding[] = _T("=="); // STRING_OK
	const TCHAR* szPaddingLoc = NULL;
	szPaddingLoc = _tcschr(szEncodedStr, szPadding[0]);
	size_t cchPaddingLen = 0;
	if (NULL != szPaddingLoc)
	{
		// check padding length
		EC_H(StringCchLength(szPaddingLoc, STRSAFE_MAX_CCH, &cchPaddingLen));
		if (cchPaddingLen >= 3) return MAPI_E_INVALID_PARAMETER;

		// check for bad characters after the first '='
		if (_tcsncmp(szPaddingLoc, (TCHAR *)szPadding, cchPaddingLen)) return MAPI_E_INVALID_PARAMETER;
	}
	// cchPaddingLen == 0,1,2 now

	size_t	cchDecodedLen = ((cchLen + 3) / 4) * 3; // 3 times number of 4 tuplets, rounded up

	// back off the decoded length to the correct length
	// xx== ->y
	// xxx= ->yY
	// x=== ->this is a bad case which should never happen
	cchDecodedLen -= cchPaddingLen;
	// we have no room for error now!
	*lpDecodedBuffer = new BYTE[cchDecodedLen];
	if (!*lpDecodedBuffer) return MAPI_E_CALL_FAILED;

	*cbBuf = cchDecodedLen;

	LPBYTE	lpOutByte = *lpDecodedBuffer;

	TCHAR c[4] = { 0 };
	BYTE bTmp[3] = { 0 }; // output

	while (*szEncodedStr)
	{
		int i = 0;
		int iOutlen = 3;
		for (i = 0; i < 4; i++)
		{
			c[i] = *(szEncodedStr + i);
			if (c[i] == _T('='))
			{
				iOutlen = i - 1;
				break;
			}
			if ((c[i] < 0x2b) || (c[i] > 0x7a)) return MAPI_E_INVALID_PARAMETER;

			c[i] = pBase64[c[i] - 0x2b];
		}
		bTmp[0] = (BYTE)((c[0] << 2) | (c[1] >> 4));
		bTmp[1] = (BYTE)((c[1] & 0x0f) << 4 | (c[2] >> 2));
		bTmp[2] = (BYTE)((c[2] & 0x03) << 6 | c[3]);

		for (i = 0; i < iOutlen; i++)
		{
			lpOutByte[i] = bTmp[i];
		}
		lpOutByte += 3;
		szEncodedStr += 4;
	}

	return hRes;
} // Base64Decode

static const		// Base64 Index into encoding
char pIndex[] = {	// and decoding table.
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79, 0x7a, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2b, 0x2f
};

// allocates output string with new
// delete with delete[]
_Check_return_ HRESULT Base64Encode(size_t cbSourceBuf, _In_count_(cbSourceBuf) LPBYTE lpSourceBuffer, _Inout_ size_t* cchEncodedStr, _Out_ _Deref_post_cap_(*cchEncodedStr) LPTSTR* szEncodedStr)
{
	HRESULT hRes = S_OK;

	size_t cchEncodeLen = ((cbSourceBuf + 2) / 3) * 4; // 4 * number of size three blocks, round up, plus null terminator
	*szEncodedStr = new TCHAR[cchEncodeLen + 1]; // allocate a touch extra for some NULL terminators
	if (cchEncodedStr) *cchEncodedStr = cchEncodeLen;
	if (!*szEncodedStr) return MAPI_E_CALL_FAILED;

	size_t cbBuf = 0; // General purpose integers.
	TCHAR* szOutChar = NULL;
	szOutChar = *szEncodedStr;

	// Using integer division to round down here
	while (cbBuf < (cbSourceBuf / 3) * 3) // encode each 3 byte octet.
	{
		*szOutChar++ = pIndex[lpSourceBuffer[cbBuf] >> 2];
		*szOutChar++ = pIndex[((lpSourceBuffer[cbBuf] & 0x03) << 4) + (lpSourceBuffer[cbBuf + 1] >> 4)];
		*szOutChar++ = pIndex[((lpSourceBuffer[cbBuf + 1] & 0x0f) << 2) + (lpSourceBuffer[cbBuf + 2] >> 6)];
		*szOutChar++ = pIndex[lpSourceBuffer[cbBuf + 2] & 0x3f];
		cbBuf += 3; // Next octet.
	}

	if (cbSourceBuf - cbBuf) // Partial octet remaining?
	{
		*szOutChar++ = pIndex[lpSourceBuffer[cbBuf] >> 2]; // Yes, encode it.

		if (cbSourceBuf - cbBuf == 1) // End of octet?
		{
			*szOutChar++ = pIndex[(lpSourceBuffer[cbBuf] & 0x03) << 4];
			*szOutChar++ = _T('=');
			*szOutChar++ = _T('=');
		}
		else
		{ // No, one more part.
			*szOutChar++ = pIndex[((lpSourceBuffer[cbBuf] & 0x03) << 4) + (lpSourceBuffer[cbBuf + 1] >> 4)];
			*szOutChar++ = pIndex[(lpSourceBuffer[cbBuf + 1] & 0x0f) << 2];
			*szOutChar++ = _T('=');
		}
	}
	*szOutChar = _T('\0');

	return hRes;
} // Base64Encode

_Check_return_ HRESULT StringToGUID(_In_z_ LPCTSTR szGUID, _Inout_ LPGUID lpGUID)
{
	return StringToGUID(szGUID, false, lpGUID);
} // StringToGUID

_Check_return_ HRESULT StringToGUID(_In_z_ LPCTSTR szGUID, bool bByteSwapped, _Inout_ LPGUID lpGUID)
{
	HRESULT hRes = S_OK;
	if (!szGUID || !lpGUID) return MAPI_E_INVALID_PARAMETER;

	ULONG cbGUID = sizeof(GUID);

	// Now we use MyBinFromHex to do the work.
	(void)MyBinFromHex(szGUID, (LPBYTE)lpGUID, &cbGUID);

	// Note that we get the bByteSwapped behavior by default. We have to work to get the 'normal' behavior
	if (!bByteSwapped)
	{
		LPBYTE lpByte = (LPBYTE)lpGUID;
		BYTE bByte = 0;
		bByte = lpByte[0];
		lpByte[0] = lpByte[3];
		lpByte[3] = bByte;
		bByte = lpByte[1];
		lpByte[1] = lpByte[2];
		lpByte[2] = bByte;
	}
	return hRes;
} // StringToGUID

_Check_return_ wstring CurrencyToString(CURRENCY curVal)
{
	wstring szCur = format(L"%05I64d", curVal.int64); // STRING_OK
	if (szCur.length() > 4)
	{
		szCur.insert(szCur.length() - 4, L"."); // STRING_OK
	}

	return szCur;
}

_Check_return_ CString TagToString(ULONG ulPropTag, _In_opt_ LPMAPIPROP lpObj, bool bIsAB, bool bSingleLine)
{
	CString szRet;
	CString szTemp;
	HRESULT hRes = S_OK;

	LPTSTR szExactMatches = NULL;
	LPTSTR szPartialMatches = NULL;
	LPTSTR szNamedPropName = NULL;
	LPTSTR szNamedPropGUID = NULL;
	LPTSTR szNamedPropDASL = NULL;

	NameIDToStrings(
		ulPropTag,
		lpObj,
		NULL,
		NULL,
		bIsAB,
		&szNamedPropName, // Built from lpProp & lpMAPIProp
		&szNamedPropGUID, // Built from lpProp & lpMAPIProp
		&szNamedPropDASL); // Built from ulPropTag & lpMAPIProp

	PropTagToPropName(ulPropTag, bIsAB, &szExactMatches, &szPartialMatches);

	CString szFormatString;
	if (bSingleLine)
	{
		szFormatString = _T("0x%1!08X! (%2)"); // STRING_OK
		if (!IsNullOrEmpty(szExactMatches)) szFormatString += _T(": %3"); // STRING_OK
		if (!IsNullOrEmpty(szPartialMatches)) szFormatString += _T(": (%4)"); // STRING_OK
		if (szNamedPropName)
		{
			EC_B(szTemp.LoadString(IDS_NAMEDPROPSINGLELINE));
			szFormatString += szTemp;
		}
		if (szNamedPropGUID)
		{
			EC_B(szTemp.LoadString(IDS_GUIDSINGLELINE));
			szFormatString += szTemp;
		}
	}
	else
	{
		EC_B(szFormatString.LoadString(IDS_TAGMULTILINE));
		if (!IsNullOrEmpty(szExactMatches))
		{
			EC_B(szTemp.LoadString(IDS_PROPNAMEMULTILINE));
			szFormatString += szTemp;
		}
		if (!IsNullOrEmpty(szPartialMatches))
		{
			EC_B(szTemp.LoadString(IDS_OTHERNAMESMULTILINE));
			szFormatString += szTemp;
		}
		if (PROP_ID(ulPropTag) < 0x8000)
		{
			EC_B(szTemp.LoadString(IDS_DASLPROPTAG));
			szFormatString += szTemp;
		}
		else if (szNamedPropDASL)
		{
			EC_B(szTemp.LoadString(IDS_DASLNAMED));
			szFormatString += szTemp;
		}
		if (szNamedPropName)
		{
			EC_B(szTemp.LoadString(IDS_NAMEPROPNAMEMULTILINE));
			szFormatString += szTemp;
		}
		if (szNamedPropGUID)
		{
			EC_B(szTemp.LoadString(IDS_NAMEPROPGUIDMULTILINE));
			szFormatString += szTemp;
		}
	}

	szRet.FormatMessage(szFormatString,
		ulPropTag,
		(LPCTSTR)TypeToString(ulPropTag),
		szExactMatches,
		szPartialMatches,
		szNamedPropName,
		szNamedPropGUID,
		szNamedPropDASL);

	delete[] szPartialMatches;
	delete[] szExactMatches;
	FreeNameIDStrings(szNamedPropName, szNamedPropGUID, szNamedPropDASL);

	if (fIsSet(DBGTest))
	{
		static size_t cchMaxBuff = 0;
		size_t cchBuff = szRet.GetLength();
		cchMaxBuff = max(cchBuff, cchMaxBuff);
		DebugPrint(DBGTest, _T("TagToString parsing 0x%08X returned %u chars - max %u\n"), ulPropTag, (UINT)cchBuff, (UINT)cchMaxBuff);
	}

	return szRet;
}

_Check_return_ CString ProblemArrayToString(_In_ LPSPropProblemArray lpProblems)
{
	CString szOut;
	if (lpProblems)
	{
		ULONG i = 0;
		for (i = 0; i < lpProblems->cProblem; i++)
		{
			CString szTemp;
			szTemp.FormatMessage(IDS_PROBLEMARRAY,
				lpProblems->aProblem[i].ulIndex,
				TagToString(lpProblems->aProblem[i].ulPropTag, NULL, false, false),
				lpProblems->aProblem[i].scode,
				ErrorNameFromErrorCode(lpProblems->aProblem[i].scode));
			szOut += szTemp;
		}
	}
	return szOut;
} // ProblemArrayToString

wstring MAPIErrToString(ULONG ulFlags, _In_ LPMAPIERROR lpErr)
{
	wstring szOut;
	if (lpErr)
	{
		szOut = formatmessage(
			ulFlags & MAPI_UNICODE ? IDS_MAPIERRUNICODE : IDS_MAPIERRANSI,
			lpErr->ulVersion,
			lpErr->lpszError,
			lpErr->lpszComponent,
			lpErr->ulLowLevelError,
			ErrorNameFromErrorCode(lpErr->ulLowLevelError),
			lpErr->ulContext);
	}
	return szOut;
}

_Check_return_ CString TnefProblemArrayToString(_In_ LPSTnefProblemArray lpError)
{
	CString szOut;
	if (lpError)
	{
		for (ULONG iError = 0; iError < lpError->cProblem; iError++)
		{
			CString szTemp;
			szTemp.FormatMessage(
				IDS_TNEFPROBARRAY,
				lpError->aProblem[iError].ulComponent,
				lpError->aProblem[iError].ulAttribute,
				TagToString(lpError->aProblem[iError].ulPropTag, NULL, false, false),
				lpError->aProblem[iError].scode,
				ErrorNameFromErrorCode(lpError->aProblem[iError].scode));
			szOut += szTemp;
		}
	}
	return szOut;
} // TnefProblemArrayToString

// There may be restrictions with over 100 nested levels, but we're not going to try to parse them
#define _MaxRestrictionNesting 100

_Check_return_ wstring RestrictionToString(_In_ LPSRestriction lpRes, _In_opt_ LPMAPIPROP lpObj, ULONG ulTabLevel)
{
	ULONG i = 0;
	if (!lpRes)
	{
		return loadstring(IDS_NULLRES);
	}
	if (ulTabLevel > _MaxRestrictionNesting)
	{
		return loadstring(IDS_RESDEPTHEXCEEDED);
	}

	wstring resString;
	wstring szProp;
	wstring szAltProp;

	wstring szTabs;
	for (i = 0; i < ulTabLevel; i++)
	{
		szTabs += L"\t"; // STRING_OK
	}

	wstring szPropNum;
	wstring szFlags = InterpretFlags(flagRestrictionType, lpRes->rt);
	resString += formatmessage(IDS_RESTYPE, szTabs.c_str(), lpRes->rt, szFlags.c_str());

	switch (lpRes->rt)
	{
	case RES_COMPAREPROPS:
		szFlags = InterpretFlags(flagRelop, lpRes->res.resCompareProps.relop);
		resString += formatmessage(
			IDS_RESCOMPARE,
			szTabs.c_str(),
			szFlags.c_str(),
			lpRes->res.resCompareProps.relop,
			TagToString(lpRes->res.resCompareProps.ulPropTag1, lpObj, false, true),
			TagToString(lpRes->res.resCompareProps.ulPropTag2, lpObj, false, true));
		break;
	case RES_AND:
		resString += formatmessage(IDS_RESANDCOUNT, szTabs.c_str(), lpRes->res.resAnd.cRes);
		if (lpRes->res.resAnd.lpRes)
		{
			for (i = 0; i < lpRes->res.resAnd.cRes; i++)
			{
				resString += formatmessage(IDS_RESANDPOINTER, szTabs.c_str(), i);
				resString += RestrictionToString(&lpRes->res.resAnd.lpRes[i], lpObj, ulTabLevel + 1);
			}
		}
		break;
	case RES_OR:
		resString += formatmessage(IDS_RESORCOUNT, szTabs.c_str(), lpRes->res.resOr.cRes);
		if (lpRes->res.resOr.lpRes)
		{
			for (i = 0; i < lpRes->res.resOr.cRes; i++)
			{
				resString += formatmessage(IDS_RESORPOINTER, szTabs.c_str(), i);
				resString += RestrictionToString(&lpRes->res.resOr.lpRes[i], lpObj, ulTabLevel + 1);
			}
		}
		break;
	case RES_NOT:
		resString += formatmessage(
			IDS_RESNOT,
			szTabs.c_str(),
			lpRes->res.resNot.ulReserved);
		resString += RestrictionToString(lpRes->res.resNot.lpRes, lpObj, ulTabLevel + 1);
		break;
	case RES_COUNT:
		// RES_COUNT and RES_NOT look the same, so we use the resNot member here
		resString += formatmessage(
			IDS_RESCOUNT,
			szTabs.c_str(),
			lpRes->res.resNot.ulReserved);
		resString += RestrictionToString(lpRes->res.resNot.lpRes, lpObj, ulTabLevel + 1);
		break;
	case RES_CONTENT:
		szFlags = InterpretFlags(flagFuzzyLevel, lpRes->res.resContent.ulFuzzyLevel);
		resString += formatmessage(
			IDS_RESCONTENT,
			szTabs.c_str(),
			szFlags.c_str(),
			lpRes->res.resContent.ulFuzzyLevel,
			TagToString(lpRes->res.resContent.ulPropTag, lpObj, false, true));
		if (lpRes->res.resContent.lpProp)
		{
			InterpretProp(lpRes->res.resContent.lpProp, &szProp, &szAltProp);
			resString += formatmessage(
				IDS_RESCONTENTPROP,
				szTabs.c_str(),
				TagToString(lpRes->res.resContent.lpProp->ulPropTag, lpObj, false, true),
				szProp.c_str(),
				szAltProp.c_str());
		}
		break;
	case RES_PROPERTY:
		szFlags = InterpretFlags(flagRelop, lpRes->res.resProperty.relop);
		resString += formatmessage(
			IDS_RESPROP,
			szTabs.c_str(),
			szFlags.c_str(),
			lpRes->res.resProperty.relop,
			TagToString(lpRes->res.resProperty.ulPropTag, lpObj, false, true));
		if (lpRes->res.resProperty.lpProp)
		{
			InterpretProp(lpRes->res.resProperty.lpProp, &szProp, &szAltProp);
			resString += formatmessage(
				IDS_RESPROPPROP,
				szTabs.c_str(),
				TagToString(lpRes->res.resProperty.lpProp->ulPropTag, lpObj, false, true),
				szProp.c_str(),
				szAltProp.c_str());
			szPropNum = InterpretNumberAsString(lpRes->res.resProperty.lpProp->Value, lpRes->res.resProperty.lpProp->ulPropTag, NULL, NULL, NULL, false);
			if (!szPropNum.empty())
			{
				resString += formatmessage(IDS_RESPROPPROPFLAGS, szTabs.c_str(), szPropNum.c_str());
			}
		}
		break;
	case RES_BITMASK:
		szFlags = InterpretFlags(flagBitmask, lpRes->res.resBitMask.relBMR);
		resString += formatmessage(
			IDS_RESBITMASK,
			szTabs.c_str(),
			szFlags.c_str(),
			lpRes->res.resBitMask.relBMR,
			lpRes->res.resBitMask.ulMask);
		szPropNum = InterpretNumberAsStringProp(lpRes->res.resBitMask.ulMask, lpRes->res.resBitMask.ulPropTag);
		if (!szPropNum.empty())
		{
			resString += formatmessage(IDS_RESBITMASKFLAGS, szPropNum.c_str());
		}
		resString += formatmessage(
			IDS_RESBITMASKTAG,
			szTabs.c_str(),
			TagToString(lpRes->res.resBitMask.ulPropTag, lpObj, false, true));
		break;
	case RES_SIZE:
		szFlags = InterpretFlags(flagRelop, lpRes->res.resSize.relop);
		resString += formatmessage(
			IDS_RESSIZE,
			szTabs.c_str(),
			szFlags.c_str(),
			lpRes->res.resSize.relop,
			lpRes->res.resSize.cb,
			TagToString(lpRes->res.resSize.ulPropTag, lpObj, false, true));
		break;
	case RES_EXIST:
		resString += formatmessage(
			IDS_RESEXIST,
			szTabs.c_str(),
			TagToString(lpRes->res.resExist.ulPropTag, lpObj, false, true),
			lpRes->res.resExist.ulReserved1,
			lpRes->res.resExist.ulReserved2);
		break;
	case RES_SUBRESTRICTION:
		resString += formatmessage(
			IDS_RESSUBRES,
			szTabs.c_str(),
			TagToString(lpRes->res.resSub.ulSubObject, lpObj, false, true));
		resString += RestrictionToString(lpRes->res.resSub.lpRes, lpObj, ulTabLevel + 1);
		break;
	case RES_COMMENT:
		resString += formatmessage(IDS_RESCOMMENT, szTabs.c_str(), lpRes->res.resComment.cValues);
		if (lpRes->res.resComment.lpProp)
		{
			for (i = 0; i < lpRes->res.resComment.cValues; i++)
			{
				InterpretProp(&lpRes->res.resComment.lpProp[i], &szProp, &szAltProp);
				resString += formatmessage(
					IDS_RESCOMMENTPROPS,
					szTabs.c_str(),
					i,
					TagToString(lpRes->res.resComment.lpProp[i].ulPropTag, lpObj, false, true),
					szProp.c_str(),
					szAltProp.c_str());
			}
		}
		resString += formatmessage(
			IDS_RESCOMMENTRES,
			szTabs.c_str());
		resString += RestrictionToString(lpRes->res.resComment.lpRes, lpObj, ulTabLevel + 1);
		break;
	case RES_ANNOTATION:
		resString += formatmessage(IDS_RESANNOTATION, szTabs.c_str(), lpRes->res.resComment.cValues);
		if (lpRes->res.resComment.lpProp)
		{
			for (i = 0; i < lpRes->res.resComment.cValues; i++)
			{
				InterpretProp(&lpRes->res.resComment.lpProp[i], &szProp, &szAltProp);
				resString += formatmessage(
					IDS_RESANNOTATIONPROPS,
					szTabs.c_str(),
					i,
					TagToString(lpRes->res.resComment.lpProp[i].ulPropTag, lpObj, false, true),
					szProp.c_str(),
					szAltProp.c_str());
			}
		}

		resString += formatmessage(
			IDS_RESANNOTATIONRES,
			szTabs.c_str());
		resString += RestrictionToString(lpRes->res.resComment.lpRes, lpObj, ulTabLevel + 1);
		break;
	}

	return resString;
}

_Check_return_ wstring RestrictionToString(_In_ LPSRestriction lpRes, _In_opt_ LPMAPIPROP lpObj)
{
	return RestrictionToString(lpRes, lpObj, 0);
}

_Check_return_ wstring AdrListToString(_In_ LPADRLIST lpAdrList)
{
	if (!lpAdrList)
	{
		return loadstring(IDS_ADRLISTNULL);
	}

	wstring adrstring;
	wstring szProp;
	wstring szAltProp;
	adrstring = formatmessage(IDS_ADRLISTCOUNT, lpAdrList->cEntries);

	ULONG i = 0;
	for (i = 0; i < lpAdrList->cEntries; i++)
	{
		adrstring += formatmessage(IDS_ADRLISTENTRIESCOUNT, i, lpAdrList->aEntries[i].cValues);

		ULONG j = 0;
		for (j = 0; j < lpAdrList->aEntries[i].cValues; j++)
		{
			InterpretProp(&lpAdrList->aEntries[i].rgPropVals[j], &szProp, &szAltProp);
			adrstring += formatmessage(
				IDS_ADRLISTENTRY,
				i,
				j,
				TagToString(lpAdrList->aEntries[i].rgPropVals[j].ulPropTag, NULL, false, false),
				szProp.c_str(),
				szAltProp.c_str());
		}
	}

	return adrstring;
}

_Check_return_ wstring ActionToString(_In_ ACTION* lpAction)
{
	if (!lpAction)
	{
		return loadstring(IDS_ACTIONNULL);
	}

	wstring actstring;
	wstring szProp;
	wstring szAltProp;
	wstring szFlags = InterpretFlags(flagAccountType, lpAction->acttype);
	wstring szFlags2 = InterpretFlags(flagRuleFlag, lpAction->ulFlags);
	actstring = formatmessage(
		IDS_ACTION,
		lpAction->acttype,
		szFlags.c_str(),
		RestrictionToString(lpAction->lpRes, NULL).c_str(),
		lpAction->ulFlags,
		szFlags2.c_str());

	switch (lpAction->acttype)
	{
	case OP_MOVE:
	case OP_COPY:
	{
		SBinary sBinStore = { 0 };
		SBinary sBinFld = { 0 };
		sBinStore.cb = lpAction->actMoveCopy.cbStoreEntryId;
		sBinStore.lpb = (LPBYTE)lpAction->actMoveCopy.lpStoreEntryId;
		sBinFld.cb = lpAction->actMoveCopy.cbFldEntryId;
		sBinFld.lpb = (LPBYTE)lpAction->actMoveCopy.lpFldEntryId;

		actstring += formatmessage(IDS_ACTIONOPMOVECOPY,
			BinToHexString(&sBinStore, true).c_str(),
			BinToTextString(&sBinStore, false).c_str(),
			BinToHexString(&sBinFld, true).c_str(),
			BinToTextString(&sBinFld, false).c_str());
		break;
	}
	case OP_REPLY:
	case OP_OOF_REPLY:
	{

		SBinary sBin = { 0 };
		sBin.cb = lpAction->actReply.cbEntryId;
		sBin.lpb = (LPBYTE)lpAction->actReply.lpEntryId;
		wstring szGUID = GUIDToStringAndName(&lpAction->actReply.guidReplyTemplate);

		actstring += formatmessage(IDS_ACTIONOPREPLY,
			BinToHexString(&sBin, true).c_str(),
			BinToTextString(&sBin, false).c_str(),
			szGUID.c_str());
		break;
	}
	case OP_DEFER_ACTION:
	{
		SBinary sBin = { 0 };
		sBin.cb = lpAction->actDeferAction.cbData;
		sBin.lpb = (LPBYTE)lpAction->actDeferAction.pbData;

		actstring += formatmessage(IDS_ACTIONOPDEFER,
			BinToHexString(&sBin, true).c_str(),
			BinToTextString(&sBin, false).c_str());
		break;
	}
	case OP_BOUNCE:
	{
		szFlags = InterpretFlags(flagBounceCode, lpAction->scBounceCode);
		actstring += formatmessage(IDS_ACTIONOPBOUNCE, lpAction->scBounceCode, szFlags.c_str());
		break;
	}
	case OP_FORWARD:
	case OP_DELEGATE:
	{
		actstring += formatmessage(IDS_ACTIONOPFORWARDDEL);
		actstring += AdrListToString(lpAction->lpadrlist);
		break;
	}

	case OP_TAG:
	{
		InterpretProp(&lpAction->propTag, &szProp, &szAltProp);
		actstring += formatmessage(IDS_ACTIONOPTAG,
			TagToString(lpAction->propTag.ulPropTag, NULL, false, true),
			szProp.c_str(),
			szAltProp.c_str());
		break;
	}
	}

	switch (lpAction->acttype)
	{
	case OP_REPLY:
	{
		szFlags = InterpretFlags(flagOPReply, lpAction->ulActionFlavor);
		break;
	}
	case OP_FORWARD:
	{
		szFlags = InterpretFlags(flagOpForward, lpAction->ulActionFlavor);
		break;
	}
	}

	actstring += formatmessage(IDS_ACTIONFLAVOR, lpAction->ulActionFlavor, szFlags.c_str());

	if (!lpAction->lpPropTagArray)
	{
		actstring += loadstring(IDS_ACTIONTAGARRAYNULL);
	}
	else
	{
		actstring += formatmessage(IDS_ACTIONTAGARRAYCOUNT, lpAction->lpPropTagArray->cValues);
		ULONG i = 0;
		for (i = 0; i < lpAction->lpPropTagArray->cValues; i++)
		{
			actstring += formatmessage(IDS_ACTIONTAGARRAYTAG,
				i,
				TagToString(lpAction->lpPropTagArray->aulPropTag[i], NULL, false, false));
		}
	}

	return actstring;
}

_Check_return_ wstring ActionsToString(_In_ ACTIONS* lpActions)
{
	if (!lpActions)
	{
		return loadstring(IDS_ACTIONSNULL);
	}

	wstring szFlags = InterpretFlags(flagRulesVersion, lpActions->ulVersion);
	wstring actstring = formatmessage(IDS_ACTIONSMEMBERS,
		lpActions->ulVersion,
		szFlags.c_str(),
		lpActions->cActions);

	UINT i = 0;
	for (i = 0; i < lpActions->cActions; i++)
	{
		actstring += formatmessage(IDS_ACTIONSACTION, i);
		actstring += ActionToString(&lpActions->lpAction[i]);
	}

	return actstring;
}

void FileTimeToString(_In_ FILETIME* lpFileTime, _In_ wstring& PropString, _In_opt_ wstring& AltPropString)
{
	HRESULT hRes = S_OK;
	SYSTEMTIME SysTime = { 0 };

	if (!lpFileTime) return;

	WC_B(FileTimeToSystemTime((FILETIME*)lpFileTime, &SysTime));

	if (S_OK == hRes)
	{
		int iRet = 0;
		wstring szFormatStr;
		wchar_t szTimeStr[MAX_PATH] = { 0 };
		wchar_t szDateStr[MAX_PATH] = { 0 };

		// shove millisecond info into our format string since GetTimeFormat doesn't use it
		szFormatStr = formatmessage(IDS_FILETIMEFORMAT, SysTime.wMilliseconds);

		WC_D(iRet, GetTimeFormatW(
			LOCALE_USER_DEFAULT,
			NULL,
			&SysTime,
			szFormatStr.c_str(),
			szTimeStr,
			MAX_PATH));

		WC_D(iRet, GetDateFormatW(
			LOCALE_USER_DEFAULT,
			NULL,
			&SysTime,
			NULL,
			szDateStr,
			MAX_PATH));

		PropString = format(L"%ws %ws", szTimeStr, szDateStr); // STRING_OK
	}
	else
	{
		PropString = loadstring(IDS_INVALIDSYSTIME);
	}

	AltPropString = formatmessage(IDS_FILETIMEALTFORMAT, lpFileTime->dwLowDateTime, lpFileTime->dwHighDateTime);
}

/***************************************************************************
Name: InterpretProp
Purpose: Evaluate a property value and return a string representing the property.
Parameters:
In:
LPSPropValue lpProp: Property to be evaluated
Out:
wstring* tmpPropString: String representing property value
wstring* tmpAltPropString: Alternative string representation
Comment: Add new Property IDs as they become known
***************************************************************************/
void InterpretProp(_In_ LPSPropValue lpProp, _In_opt_  wstring* PropString, _In_opt_  wstring* AltPropString)
{
	if (!lpProp) return;

	Property parsedProperty = ParseProperty(lpProp);

	if (PropString) *PropString = parsedProperty.toString();
	if (AltPropString) *AltPropString = parsedProperty.toAltString();
}

wstring TypeToWstring(ULONG ulPropTag)
{
	wstring tmpPropType;

	bool bNeedInstance = false;
	if (ulPropTag & MV_INSTANCE)
	{
		ulPropTag &= ~MV_INSTANCE;
		bNeedInstance = true;
	}

	ULONG ulCur = 0;
	bool bTypeFound = false;

	for (ulCur = 0; ulCur < ulPropTypeArray; ulCur++)
	{
		if (PropTypeArray[ulCur].ulValue == PROP_TYPE(ulPropTag))
		{
			tmpPropType = PropTypeArray[ulCur].lpszName;
			bTypeFound = true;
			break;
		}
	}

	if (!bTypeFound)
		tmpPropType = format(L"0x%04x", PROP_TYPE(ulPropTag)); // STRING_OK

	if (bNeedInstance) tmpPropType += L" | MV_INSTANCE"; // STRING_OK
	return tmpPropType;
}

_Check_return_ CString TypeToString(ULONG ulPropTag)
{
	return wstringToCString(TypeToWstring(ulPropTag));
}

// TagToString will prepend the http://schemas.microsoft.com/MAPI/ for us since it's a constant
// We don't compute a DASL string for non-named props as FormatMessage in TagToString can handle those
void NameIDToStrings(_In_ LPMAPINAMEID lpNameID,
	ULONG ulPropTag,
	_In_ wstring& szPropName,
	_In_ wstring& szPropGUID,
	_In_ wstring& szDASL)
{
	HRESULT hRes = S_OK;

	// Can't generate strings without a MAPINAMEID structure
	if (!lpNameID) return;

	LPNAMEDPROPCACHEENTRY lpNamedPropCacheEntry = NULL;

	// If we're using the cache, look up the answer there and return
	if (fCacheNamedProps())
	{
		lpNamedPropCacheEntry = FindCacheEntry(PROP_ID(ulPropTag), lpNameID->lpguid, lpNameID->ulKind, lpNameID->Kind.lID, lpNameID->Kind.lpwstrName);
		if (lpNamedPropCacheEntry && lpNamedPropCacheEntry->bStringsCached)
		{
			szPropName = lpNamedPropCacheEntry->lpszPropName;
			szPropGUID = lpNamedPropCacheEntry->lpszPropGUID;
			szDASL = lpNamedPropCacheEntry->lpszDASL;
			return;
		}

		// We shouldn't ever get here without a cached entry
		if (!lpNamedPropCacheEntry)
		{
			DebugPrint(DBGNamedProp, _T("NameIDToStrings: Failed to find cache entry for ulPropTag = 0x%08X\n"), ulPropTag);
			return;
		}
	}

	DebugPrint(DBGNamedProp, _T("Parsing named property\n"));
	DebugPrint(DBGNamedProp, _T("ulPropTag = 0x%08x\n"), ulPropTag);
	szPropGUID = GUIDToStringAndName(lpNameID->lpguid);
	DebugPrint(DBGNamedProp, _T("lpNameID->lpguid = %ws\n"), szPropGUID.c_str());

	wstring szDASLGuid = GUIDToString(lpNameID->lpguid);

	if (lpNameID->ulKind == MNID_ID)
	{
		DebugPrint(DBGNamedProp, _T("lpNameID->Kind.lID = 0x%04X = %d\n"), lpNameID->Kind.lID, lpNameID->Kind.lID);
		wstring szName = NameIDToPropName(lpNameID);

		if (!szName.empty())
		{
			// Printing hex first gets a nice sort without spacing tricks
			szPropName = format(L"id: 0x%04X=%d = %ws", // STRING_OK
				lpNameID->Kind.lID,
				lpNameID->Kind.lID,
				szName.c_str());

		}
		else
		{
			// Printing hex first gets a nice sort without spacing tricks
			szPropName = format(L"id: 0x%04X=%d", // STRING_OK
				lpNameID->Kind.lID,
				lpNameID->Kind.lID);
		}

		szDASL = format(L"id/%s/%04X%04X", // STRING_OK
			szDASLGuid.c_str(),
			lpNameID->Kind.lID,
			PROP_TYPE(ulPropTag));
	}
	else if (lpNameID->ulKind == MNID_STRING)
	{
		// lpwstrName is LPWSTR which means it's ALWAYS unicode
		// But some folks get it wrong and stuff ANSI data in there
		// So we check the string length both ways to make our best guess
		size_t cchShortLen = NULL;
		size_t cchWideLen = NULL;
		WC_H(StringCchLengthA((LPSTR)lpNameID->Kind.lpwstrName, STRSAFE_MAX_CCH, &cchShortLen));
		WC_H(StringCchLengthW(lpNameID->Kind.lpwstrName, STRSAFE_MAX_CCH, &cchWideLen));

		if (cchShortLen < cchWideLen)
		{
			// this is the *proper* case
			DebugPrint(DBGNamedProp, _T("lpNameID->Kind.lpwstrName = \"%ws\"\n"), lpNameID->Kind.lpwstrName);
			szPropName = format(L"sz: \"%ws\"", lpNameID->Kind.lpwstrName);

			szDASL = format(L"string/%ws/%ws", // STRING_OK
				szDASLGuid.c_str(),
				lpNameID->Kind.lpwstrName);
		}
		else
		{
			// this is the case where ANSI data was shoved into a unicode string.
			DebugPrint(DBGNamedProp, _T("Warning: ANSI data was found in a unicode field. This is a bug on the part of the creator of this named property\n"));
			DebugPrint(DBGNamedProp, _T("lpNameID->Kind.lpwstrName = \"%hs\"\n"), (LPCSTR)lpNameID->Kind.lpwstrName);

			wstring szComment = loadstring(IDS_NAMEWASANSI);
			szPropName = format(L"sz: \"%hs\" %ws", (LPSTR)lpNameID->Kind.lpwstrName, szComment);

			szDASL = format(L"string/%ws/%hs", // STRING_OK
				szDASLGuid.c_str(),
				lpNameID->Kind.lpwstrName);
		}
	}

	// We've built our strings - if we're caching, put them in the cache
	if (lpNamedPropCacheEntry)
	{
		lpNamedPropCacheEntry->lpszPropName = szPropName;
		lpNamedPropCacheEntry->lpszPropGUID = szPropGUID;
		lpNamedPropCacheEntry->lpszDASL = szDASL;
		lpNamedPropCacheEntry->bStringsCached = true;
	}
}

// lpszNamedPropName, lpszNamedPropGUID, lpszNamedPropDASL freed with FreeNameIDStrings
void NameIDToStrings(
	ULONG ulPropTag, // optional 'original' prop tag
	_In_opt_ LPMAPIPROP lpMAPIProp, // optional source object
	_In_opt_ LPMAPINAMEID lpNameID, // optional named property information to avoid GetNamesFromIDs call
	_In_opt_ LPSBinary lpMappingSignature, // optional mapping signature for object to speed named prop lookups
	bool bIsAB, // true if we know we're dealing with an address book property (they can be > 8000 and not named props)
	_Deref_opt_out_opt_z_ LPTSTR* lpszNamedPropName, // Built from ulPropTag & lpMAPIProp
	_Deref_opt_out_opt_z_ LPTSTR* lpszNamedPropGUID, // Built from ulPropTag & lpMAPIProp
	_Deref_opt_out_opt_z_ LPTSTR* lpszNamedPropDASL) // Built from ulPropTag & lpMAPIProp
{
	HRESULT hRes = S_OK;

	// In case we error out, set our returns
	if (lpszNamedPropName) *lpszNamedPropName = NULL;
	if (lpszNamedPropGUID) *lpszNamedPropGUID = NULL;
	if (lpszNamedPropDASL) *lpszNamedPropDASL = NULL;

	// Named Props
	LPMAPINAMEID* lppPropNames = 0;

	// If we weren't passed named property information and we need it, look it up
	// We check bIsAB here - some address book providers return garbage which will crash us
	if (!lpNameID &&
		lpMAPIProp && // if we have an object
		!bIsAB &&
		RegKeys[regkeyPARSED_NAMED_PROPS].ulCurDWORD && // and we're parsing named props
		(RegKeys[regkeyGETPROPNAMES_ON_ALL_PROPS].ulCurDWORD || PROP_ID(ulPropTag) >= 0x8000) && // and it's either a named prop or we're doing all props
		(lpszNamedPropName || lpszNamedPropGUID || lpszNamedPropDASL)) // and we want to return something that needs named prop information
	{
		SPropTagArray tag = { 0 };
		LPSPropTagArray lpTag = &tag;
		ULONG ulPropNames = 0;
		tag.cValues = 1;
		tag.aulPropTag[0] = ulPropTag;

		WC_H_GETPROPS(GetNamesFromIDs(lpMAPIProp,
			lpMappingSignature,
			&lpTag,
			NULL,
			NULL,
			&ulPropNames,
			&lppPropNames));
		if (SUCCEEDED(hRes) && ulPropNames == 1 && lppPropNames && lppPropNames[0])
		{
			lpNameID = lppPropNames[0];
		}
		hRes = S_OK;
	}

	if (lpNameID)
	{
		wstring lpszPropName;
		wstring lpszPropGUID;
		wstring lpszDASL;

		NameIDToStrings(lpNameID,
			ulPropTag,
			lpszPropName,
			lpszPropGUID,
			lpszDASL);

		if (lpszNamedPropName) *lpszNamedPropName = wstringToLPTSTR(lpszPropName);
		if (lpszNamedPropGUID) *lpszNamedPropGUID = wstringToLPTSTR(lpszPropGUID);
		if (lpszNamedPropDASL) *lpszNamedPropDASL = wstringToLPTSTR(lpszDASL);
	}

	// Avoid making the call if we don't have to so we don't accidently depend on MAPI
	if (lppPropNames) MAPIFreeBuffer(lppPropNames);
}

// Free strings from NameIDToStrings if necessary
// If we're using the cache, we don't need to free
// Need to watch out for callers to NameIDToStrings holding the strings
// long enough for the user to change the cache setting!
void FreeNameIDStrings(_In_opt_z_ LPTSTR lpszPropName,
	_In_opt_z_ LPTSTR lpszPropGUID,
	_In_opt_z_ LPTSTR lpszDASL)
{
	delete[] lpszPropName;
	delete[] lpszPropGUID;
	delete[] lpszDASL;
}