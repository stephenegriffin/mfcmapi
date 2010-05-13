#pragma once

#undef PR_CONVERSION_STATE
#undef PR_DOTSTUFF_STATE
#undef PR_USER_SID

#define PR_CONVERSION_STATE			PROP_TAG(PT_LONG, 0x6778)
#define PR_DOTSTUFF_STATE			PROP_TAG(PT_LONG, 0x6001)
#define PR_USER_SID					PROP_TAG(PT_BINARY, 0x6783)

// http://support.microsoft.com/kb/898835
#define PR_ROH_FLAGS                PROP_TAG(PT_LONG,0x6623)
#define PR_ROH_PROXY_SERVER         PROP_TAG(PT_UNICODE,0x6622)
#define PR_ROH_PROXY_PRINCIPAL_NAME PROP_TAG(PT_UNICODE,0x6625)
#define PR_ROH_PROXY_AUTH_SCHEME    PROP_TAG(PT_LONG,0x6627)
#define	PR_RULE_VERSION				PROP_TAG(PT_I2, pidSpecialMin+0x1D)

#define PR_FREEBUSY_NT_SECURITY_DESCRIPTOR (PROP_TAG(PT_BINARY,0x0F00))

// http://support.microsoft.com/kb/171670
// Entry ID for the Calendar
#define PR_IPM_APPOINTMENT_ENTRYID (PROP_TAG(PT_BINARY,0x36D0))
// Entry ID for the Contact Folder
#define PR_IPM_CONTACT_ENTRYID (PROP_TAG(PT_BINARY,0x36D1))
// Entry ID for the Journal Folder
#define PR_IPM_JOURNAL_ENTRYID (PROP_TAG(PT_BINARY,0x36D2))
// Entry ID for the Notes Folder
#define PR_IPM_NOTE_ENTRYID (PROP_TAG(PT_BINARY,0x36D3))
// Entry ID for the Task Folder
#define PR_IPM_TASK_ENTRYID (PROP_TAG(PT_BINARY,0x36D4))
// Entry IDs for the Reminders Folder
#define PR_REM_ONLINE_ENTRYID (PROP_TAG(PT_BINARY,0x36D5))
#define PR_REM_OFFLINE_ENTRYID PROP_TAG(PT_BINARY, 0x36D6)
// Entry ID for the Drafts Folder
#define PR_IPM_DRAFTS_ENTRYID (PROP_TAG(PT_BINARY,0x36D7))

#define PR_DEF_POST_MSGCLASS PROP_TAG(PT_STRING8, 0x36E5)
#define PR_DEF_POST_DISPLAYNAME	PROP_TAG(PT_TSTRING, 0x36E6)
#define PR_FREEBUSY_ENTRYIDS PROP_TAG(PT_MV_BINARY, 0x36E4)
#ifndef PR_RECIPIENT_TRACKSTATUS
#define PR_RECIPIENT_TRACKSTATUS PROP_TAG(PT_LONG, 0x5FFF)
#endif
#define PR_RECIPIENT_FLAGS PROP_TAG(PT_LONG, 0x5FFD)
#define PR_RECIPIENT_ENTRYID PROP_TAG(PT_BINARY, 0x5FF7)
#define PR_RECIPIENT_DISPLAY_NAME PROP_TAG(PT_TSTRING, 0x5FF6)
#define PR_ICON_INDEX PROP_TAG(PT_LONG, 0x1080)
#define PR_OST_OSTID PROP_TAG(PT_BINARY, 0x7c04)
#define PR_OFFLINE_FOLDER PROP_TAG(PT_LONG, 0x7c05)
#define PR_FAV_PARENT_SOURCE_KEY PROP_TAG(PT_BINARY, 0x7d02)

#ifndef PR_USER_X509_CERTIFICATE
#define PR_USER_X509_CERTIFICATE (PROP_TAG(PT_MV_BINARY,0x3a70))
#endif
#ifndef PR_NT_SECURITY_DESCRIPTOR
#define PR_NT_SECURITY_DESCRIPTOR (PROP_TAG(PT_BINARY,0x0E27))
#endif
#ifndef PR_BODY_HTML
#define PR_BODY_HTML (PROP_TAG(PT_TSTRING,0x1013))
#endif
#ifndef PR_BODY_HTML_A
#define PR_BODY_HTML_A (PROP_TAG(PT_STRING8,0x1013))
#endif
#ifndef PR_BODY_HTML_W
#define PR_BODY_HTML_W (PROP_TAG(PT_UNICODE,0x1013))
#endif

// http://support.microsoft.com/kb/816477
#ifndef PR_MSG_EDITOR_FORMAT
#define PR_MSG_EDITOR_FORMAT PROP_TAG( PT_LONG, 0x5909 )
#endif
#ifndef PR_INTERNET_MESSAGE_ID
#define PR_INTERNET_MESSAGE_ID PROP_TAG(PT_TSTRING, 0x1035)
#endif
#ifndef PR_INTERNET_MESSAGE_ID_A
#define PR_INTERNET_MESSAGE_ID_A PROP_TAG(PT_STRING8, 0x1035)
#endif
#ifndef PR_INTERNET_MESSAGE_ID_W
#define PR_INTERNET_MESSAGE_ID_W PROP_TAG(PT_UNICODE, 0x1035)
#endif
#ifndef PR_SMTP_ADDRESS
#define PR_SMTP_ADDRESS PROP_TAG(PT_TSTRING,0x39FE)
#endif
#ifndef PR_SMTP_ADDRESS_A
#define PR_SMTP_ADDRESS_A PROP_TAG(PT_STRING8,0x39FE)
#endif
#ifndef PR_SMTP_ADDRESS_W
#define PR_SMTP_ADDRESS_W PROP_TAG(PT_UNICODE,0x39FE)
#endif
#ifndef PR_INTERNET_ARTICLE_NUMBER
#define PR_INTERNET_ARTICLE_NUMBER PROP_TAG(PT_LONG, 0x0E23)
#endif
#ifndef PR_SEND_INTERNET_ENCODING
#define PR_SEND_INTERNET_ENCODING PROP_TAG(PT_LONG, 0x3A71)
#endif
#ifndef PR_IN_REPLY_TO_ID
#define PR_IN_REPLY_TO_ID PROP_TAG(PT_TSTRING, 0x1042)
#endif
#ifndef PR_IN_REPLY_TO_ID_A
#define PR_IN_REPLY_TO_ID_A PROP_TAG(PT_STRING8, 0x1042)
#endif
#ifndef PR_IN_REPLY_TO_ID_W
#define PR_IN_REPLY_TO_ID_W PROP_TAG(PT_UNICODE, 0x1042)
#endif
#ifndef PR_ATTACH_MIME_SEQUENCE
#define PR_ATTACH_MIME_SEQUENCE PROP_TAG(PT_LONG, 0x3710)
#endif
#ifndef PR_ATTACH_CONTENT_BASE
#define PR_ATTACH_CONTENT_BASE PROP_TAG(PT_TSTRING, 0x3711)
#endif
#ifndef PR_ATTACH_CONTENT_BASE_A
#define PR_ATTACH_CONTENT_BASE_A PROP_TAG(PT_STRING8, 0x3711)
#endif
#ifndef PR_ATTACH_CONTENT_BASE_W
#define PR_ATTACH_CONTENT_BASE_W PROP_TAG(PT_UNICODE, 0x3711)
#endif
#ifndef PR_ATTACH_CONTENT_ID
#define PR_ATTACH_CONTENT_ID PROP_TAG(PT_TSTRING, 0x3712)
#endif
#ifndef PR_ATTACH_CONTENT_ID_A
#define PR_ATTACH_CONTENT_ID_A PROP_TAG(PT_STRING8, 0x3712)
#endif
#ifndef PR_ATTACH_CONTENT_ID_W
#define PR_ATTACH_CONTENT_ID_W PROP_TAG(PT_UNICODE, 0x3712)
#endif
#ifndef PR_ATTACH_CONTENT_LOCATION
#define PR_ATTACH_CONTENT_LOCATION PROP_TAG(PT_TSTRING, 0x3713)
#endif
#ifndef PR_ATTACH_CONTENT_LOCATION_A
#define PR_ATTACH_CONTENT_LOCATION_A PROP_TAG(PT_STRING8, 0x3713)
#endif
#ifndef PR_ATTACH_CONTENT_LOCATION_W
#define PR_ATTACH_CONTENT_LOCATION_W PROP_TAG(PT_UNICODE, 0x3713)
#endif
#ifndef PR_ATTACH_FLAGS
#define PR_ATTACH_FLAGS PROP_TAG(PT_LONG, 0x3714)
#endif

// http://support.microsoft.com/kb/837364
#ifndef PR_CONFLICT_ITEMS
#define PR_CONFLICT_ITEMS PROP_TAG(PT_MV_BINARY,0x1098)
#endif
#ifndef PR_INTERNET_APPROVED
#define PR_INTERNET_APPROVED PROP_TAG(PT_TSTRING,0x1030)
#endif
#ifndef PR_INTERNET_APPROVED_A
#define PR_INTERNET_APPROVED_A PROP_TAG(PT_STRING8,0x1030)
#endif
#ifndef PR_INTERNET_APPROVED_W
#define PR_INTERNET_APPROVED_W PROP_TAG(PT_UNICODE,0x1030)
#endif
#ifndef PR_INTERNET_CONTROL
#define PR_INTERNET_CONTROL PROP_TAG(PT_TSTRING,0x1031)
#endif
#ifndef PR_INTERNET_CONTROL_A
#define PR_INTERNET_CONTROL_A PROP_TAG(PT_STRING8,0x1031)
#endif
#ifndef PR_INTERNET_CONTROL_W
#define PR_INTERNET_CONTROL_W PROP_TAG(PT_UNICODE,0x1031)
#endif
#ifndef PR_INTERNET_DISTRIBUTION
#define PR_INTERNET_DISTRIBUTION PROP_TAG(PT_TSTRING,0x1032)
#endif
#ifndef PR_INTERNET_DISTRIBUTION_A
#define PR_INTERNET_DISTRIBUTION_A PROP_TAG(PT_STRING8,0x1032)
#endif
#ifndef PR_INTERNET_DISTRIBUTION_W
#define PR_INTERNET_DISTRIBUTION_W PROP_TAG(PT_UNICODE,0x1032)
#endif
#ifndef PR_INTERNET_FOLLOWUP_TO
#define PR_INTERNET_FOLLOWUP_TO PROP_TAG(PT_TSTRING,0x1033)
#endif
#ifndef PR_INTERNET_FOLLOWUP_TO_A
#define PR_INTERNET_FOLLOWUP_TO_A PROP_TAG(PT_STRING8,0x1033)
#endif
#ifndef PR_INTERNET_FOLLOWUP_TO_W
#define PR_INTERNET_FOLLOWUP_TO_W PROP_TAG(PT_UNICODE,0x1033)
#endif
#ifndef PR_INTERNET_LINES
#define PR_INTERNET_LINES PROP_TAG(PT_LONG,0x1034)
#endif
#ifndef PR_INTERNET_NEWSGROUPS
#define PR_INTERNET_NEWSGROUPS PROP_TAG(PT_TSTRING,0x1036)
#endif
#ifndef PR_INTERNET_NEWSGROUPS_A
#define PR_INTERNET_NEWSGROUPS_A PROP_TAG(PT_STRING8,0x1036)
#endif
#ifndef PR_INTERNET_NEWSGROUPS_W
#define PR_INTERNET_NEWSGROUPS_W PROP_TAG(PT_UNICODE,0x1036)
#endif
#ifndef PR_INTERNET_NNTP_PATH
#define PR_INTERNET_NNTP_PATH PROP_TAG(PT_TSTRING,0x1038)
#endif
#ifndef PR_INTERNET_NNTP_PATH_A
#define PR_INTERNET_NNTP_PATH_A PROP_TAG(PT_STRING8,0x1038)
#endif
#ifndef PR_INTERNET_NNTP_PATH_W
#define PR_INTERNET_NNTP_PATH_W PROP_TAG(PT_UNICODE,0x1038)
#endif
#ifndef PR_INTERNET_ORGANIZATION
#define PR_INTERNET_ORGANIZATION PROP_TAG(PT_TSTRING,0x1037)
#endif
#ifndef PR_INTERNET_ORGANIZATION_A
#define PR_INTERNET_ORGANIZATION_A PROP_TAG(PT_STRING8,0x1037)
#endif
#ifndef PR_INTERNET_ORGANIZATION_W
#define PR_INTERNET_ORGANIZATION_W PROP_TAG(PT_UNICODE,0x1037)
#endif
#ifndef PR_INTERNET_PRECEDENCE
#define PR_INTERNET_PRECEDENCE PROP_TAG(PT_TSTRING,0x1041)
#endif
#ifndef PR_INTERNET_PRECEDENCE_A
#define PR_INTERNET_PRECEDENCE_A PROP_TAG(PT_STRING8,0x1041)
#endif
#ifndef PR_INTERNET_PRECEDENCE_W
#define PR_INTERNET_PRECEDENCE_W PROP_TAG(PT_UNICODE,0x1041)
#endif
#ifndef PR_INTERNET_REFERENCES
#define PR_INTERNET_REFERENCES PROP_TAG(PT_TSTRING,0x1039)
#endif
#ifndef PR_INTERNET_REFERENCES_A
#define PR_INTERNET_REFERENCES_A PROP_TAG(PT_STRING8,0x1039)
#endif
#ifndef PR_INTERNET_REFERENCES_W
#define PR_INTERNET_REFERENCES_W PROP_TAG(PT_UNICODE,0x1039)
#endif
#ifndef PR_NEWSGROUP_NAME
#define PR_NEWSGROUP_NAME PROP_TAG(PT_TSTRING,0x0E24)
#endif
#ifndef PR_NEWSGROUP_NAME_A
#define PR_NEWSGROUP_NAME_A PROP_TAG(PT_STRING8,0x0E24)
#endif
#ifndef PR_NEWSGROUP_NAME_W
#define PR_NEWSGROUP_NAME_W PROP_TAG(PT_UNICODE,0x0E24)
#endif
#ifndef PR_NNTP_XREF
#define PR_NNTP_XREF PROP_TAG(PT_TSTRING,0x1040)
#endif
#ifndef PR_NNTP_XREF_A
#define PR_NNTP_XREF_A PROP_TAG(PT_STRING8,0x1040)
#endif
#ifndef PR_NNTP_XREF_W
#define PR_NNTP_XREF_W PROP_TAG(PT_UNICODE,0x1040)
#endif
#ifndef PR_POST_FOLDER_ENTRIES
#define PR_POST_FOLDER_ENTRIES PROP_TAG(PT_BINARY,0x103B)
#endif
#ifndef PR_POST_FOLDER_NAMES
#define PR_POST_FOLDER_NAMES PROP_TAG(PT_TSTRING,0x103C)
#endif
#ifndef PR_POST_FOLDER_NAMES_A
#define PR_POST_FOLDER_NAMES_A PROP_TAG(PT_STRING8,0x103C)
#endif
#ifndef PR_POST_FOLDER_NAMES_W
#define PR_POST_FOLDER_NAMES_W PROP_TAG(PT_UNICODE,0x103C)
#endif
#ifndef PR_POST_REPLY_DENIED
#define PR_POST_REPLY_DENIED PROP_TAG(PT_BOOL,0x103F)
#endif
#ifndef PR_POST_REPLY_FOLDER_ENTRIES
#define PR_POST_REPLY_FOLDER_ENTRIES PROP_TAG(PT_BINARY,0x103D)
#endif
#ifndef PR_POST_REPLY_FOLDER_NAMES
#define PR_POST_REPLY_FOLDER_NAMES PROP_TAG(PT_TSTRING,0x103E)
#endif
#ifndef PR_POST_REPLY_FOLDER_NAMES_A
#define PR_POST_REPLY_FOLDER_NAMES_A PROP_TAG(PT_STRING8,0x103E)
#endif
#ifndef PR_POST_REPLY_FOLDER_NAMES_W
#define PR_POST_REPLY_FOLDER_NAMES_W PROP_TAG(PT_UNICODE,0x103E)
#endif
#ifndef PR_SUPERSEDES
#define PR_SUPERSEDES PROP_TAG(PT_TSTRING,0x103A)
#endif
#ifndef PR_SUPERSEDES_A
#define PR_SUPERSEDES_A PROP_TAG(PT_STRING8,0x103A)
#endif
#ifndef PR_SUPERSEDES_W
#define PR_SUPERSEDES_W PROP_TAG(PT_UNICODE,0x103A)
#endif

// Outlook 2003 Integration API - http://msdn2.microsoft.com/en-us/library/aa193120(office.11).aspx
#ifndef PR_PRIMARY_SEND_ACCT
#define PR_PRIMARY_SEND_ACCT PROP_TAG(PT_UNICODE,0x0E28)
#endif
#ifndef PR_NEXT_SEND_ACCT
#define PR_NEXT_SEND_ACCT PROP_TAG(PT_UNICODE,0x0E29)
#endif

// http://support.microsoft.com/kb/225009
#ifndef PR_PROCESSED
#define PR_PROCESSED PROP_TAG(PT_BOOLEAN, 0x7d01)
#endif

// http://support.microsoft.com/kb/278321
#ifndef PR_INETMAIL_OVERRIDE_FORMAT
#define PR_INETMAIL_OVERRIDE_FORMAT PROP_TAG(PT_LONG,0x5902)
#endif

// http://support.microsoft.com/kb/312900
#ifndef PR_SECURITY_PROFILES
#define PR_SECURITY_PROFILES PROP_TAG(PT_MV_BINARY, 0x355)
#endif
#define PR_CERT_PROP_VERSION            PROP_TAG(PT_LONG,       0x0001)
#define PR_CERT_MESSAGE_ENCODING        PROP_TAG(PT_LONG,       0x0006)
#define PR_CERT_DEFAULTS                PROP_TAG(PT_LONG,       0x0020)
#define PR_CERT_DISPLAY_NAME_A          PROP_TAG(PT_STRING8,    0x000B)
#define PR_CERT_KEYEX_SHA1_HASH         PROP_TAG(PT_BINARY,     0x0022)
#define PR_CERT_SIGN_SHA1_HASH          PROP_TAG(PT_BINARY,     0x0009)
#define PR_CERT_ASYMETRIC_CAPS          PROP_TAG(PT_BINARY,     0x0002)
// Values for PR_CERT_DEFAULTS
#define MSG_DEFAULTS_NONE               0
#define MSG_DEFAULTS_FOR_FORMAT         1 // Default certificate for S/MIME.
#define MSG_DEFAULTS_GLOBAL             2 // Default certificate for all formats.
#define MSG_DEFAULTS_SEND_CERT          4 // Send certificate with message.

// http://support.microsoft.com/kb/912237
#ifndef PR_ATTACHMENT_CONTACTPHOTO
#define PR_ATTACHMENT_CONTACTPHOTO PROP_TAG(PT_BOOLEAN, 0x7fff)
#endif

// http://support.microsoft.com/kb/194955
#define PR_AGING_PERIOD         PROP_TAG(PT_LONG,0x36EC)
#define PR_AGING_GRANULARITY    PROP_TAG(PT_LONG,0x36EE)

// http://msdn2.microsoft.com/en-us/library/bb176434.aspx
#define PR_AGING_AGE_FOLDER       PROP_TAG(PT_BOOLEAN,0x6857)
#define PR_AGING_DELETE_ITEMS     PROP_TAG(PT_BOOLEAN,0x6855)
#define PR_AGING_FILE_NAME_AFTER9 PROP_TAG(PT_STRING8,0x6859)
#define PR_AGING_DEFAULT          PROP_TAG(PT_LONG,0x685E)

#define AG_DEFAULT_FILE			0x01
#define AG_DEFAULT_ALL			0x02
#define AG_DEFAULT_BOTH			(AG_DEFAULT_FILE | AG_DEFAULT_ALL)

// http://msdn2.microsoft.com/en-us/library/bb820938.aspx
#define PR_PROVIDER_ITEMID        PROP_TAG(PT_BINARY, 0x0EA3)
// http://msdn2.microsoft.com/en-us/library/bb820939.aspx
#define PR_PROVIDER_PARENT_ITEMID PROP_TAG(PT_BINARY, 0x0EA4)

// PH props
#define PR_SEARCH_ATTACHMENTS_W      PROP_TAG(PT_UNICODE, 0x0EA5)
#define PR_SEARCH_RECIP_EMAIL_TO_W   PROP_TAG(PT_UNICODE, 0x0EA6)
#define PR_SEARCH_RECIP_EMAIL_CC_W   PROP_TAG(PT_UNICODE, 0x0EA7)
#define PR_SEARCH_RECIP_EMAIL_BCC_W  PROP_TAG(PT_UNICODE, 0x0EA8)
#define PR_SEARCH_OWNER_ID           PROP_TAG(PT_LONG,    0x3419)

// IMPORTANT NOTE: This property holds additional Ren special folder EntryIDs.
// The EntryID is for the special folder is located at sf* - sfRenMVEntryIDs
// This is the only place you should add new special folder ids, and the order
// of these entry ids must be preserved for legacy clients.
// Also, all new (as of Office.NET) special folders should have the extended
// folder flag XEFF_SPECIAL_FOLDER to tell the folder tree data to check if
// this folder is actually special or not.
// See comment above for places in Outlook that will need modification if you
// add a new sf* index.
// It currently contains:
//       sfConflicts       0
//       sfSyncFailures    1
//       sfLocalFailures   2
//       sfServerFailures  3
//       sfJunkEmail       4
//       sfSpamTagDontUse  5
//
// NOTE: sfSpamTagDontUse is not the real special folder but used #5 slot
// Therefore, we need to skip it when enum through sf* special folders.
#define PR_ADDITIONAL_REN_ENTRYIDS    PROP_TAG(PT_MV_BINARY, 0x36D8)

// http://msdn2.microsoft.com/en-us/library/bb820966.aspx
#define	PR_PROFILE_SERVER_FULL_VERSION PROP_TAG( PT_BINARY, pidProfileMin+0x3b)

// http://msdn2.microsoft.com/en-us/library/bb820973.aspx
// Additional display attributes, to supplement PR_DISPLAY_TYPE.
#define PR_DISPLAY_TYPE_EX PROP_TAG( PT_LONG, 0x3905)

// PR_DISPLAY_TYPE_EX has the following format
//
// 33222222222211111111110000000000
// 10987654321098765432109876543210
//
// FAxxxxxxxxxxxxxxRRRRRRRRLLLLLLLL
//
// F = 1 if remote is valid, 0 if it is not
// A = 1 if the user is ACL-able, 0 if the user is not
// x - unused at this time, do not interpret as this may be used in the future
// R = display type from

#define DTE_FLAG_REMOTE_VALID 0x80000000
#define DTE_FLAG_ACL_CAPABLE  0x40000000
#define DTE_MASK_REMOTE       0x0000ff00
#define DTE_MASK_LOCAL        0x000000ff

#define DTE_IS_REMOTE_VALID(v) (!!((v) & DTE_FLAG_REMOTE_VALID))
#define DTE_IS_ACL_CAPABLE(v)  (!!((v) & DTE_FLAG_ACL_CAPABLE))
#define DTE_REMOTE(v)          (((v) & DTE_MASK_REMOTE) >> 8)
#define DTE_LOCAL(v)           ((v) & DTE_MASK_LOCAL)

#define DT_ROOM         ((ULONG) 0x00000007)
#define DT_EQUIPMENT    ((ULONG) 0x00000008)
#define DT_SEC_DISTLIST ((ULONG) 0x00000009)

// [MS-NSPI].pdf
#define DT_CONTAINER    ((ULONG) 0x00000100)
#define DT_TEMPLATE     ((ULONG) 0x00000101)
#define DT_ADDRESS_TEMPLATE ((ULONG) 0x00000102)
#define DT_SEARCH       ((ULONG) 0x00000200)

// http://msdn2.microsoft.com/en-us/library/bb821036.aspx
#define PR_FLAG_STATUS PROP_TAG( PT_LONG, 0x1090 )
enum FollowUpStatus {
	flwupNone = 0,
	flwupComplete,
	flwupMarked
};

// http://msdn2.microsoft.com/en-us/library/bb821062.aspx
#define PR_FOLLOWUP_ICON PROP_TAG( PT_LONG, 0x1095 )
typedef enum OlFlagIcon {
	olNoFlagIcon=0,
	olPurpleFlagIcon=1,
	olOrangeFlagIcon=2,
	olGreenFlagIcon=3,
	olYellowFlagIcon=4,
	olBlueFlagIcon=5,
	olRedFlagIcon=6,
} OlFlagIcon;

// http://msdn2.microsoft.com/en-us/library/bb821130.aspx
enum Gender {
	genderMin = 0,
	genderUnspecified = genderMin,
	genderFemale,
	genderMale,
	genderCount,
	genderMax = genderCount - 1
};

// [MS-OXCFOLD]
// Use CI searches exclusively (never use old-style search)
#define CONTENT_INDEXED_SEARCH		((ULONG) 0x00010000)

// Never use CI search (old-style search only)
#define NON_CONTENT_INDEXED_SEARCH	((ULONG) 0x00020000)

// Make the search static (no backlinks/dynamic updates).  This is independent
// of whether or not the search uses CI.
#define STATIC_SEARCH				((ULONG) 0x00040000)

// The search used the content index (CI) in some fashion, and is
// non-dynamic
// NOTE: If SEARCH_REBUILD is set, the query is still being processed, and
// the static-ness may not yet have been determined (see SEARCH_MAYBE_STATIC).
#define SEARCH_STATIC			((ULONG) 0x00010000)

// The search is still being evaluated (SEARCH_REBUILD should always
// be set when this bit is set), and could become either static or dynamic.
// This bit is needed to distinguish this in-progress state separately from
// static-only (SEARCH_STATIC) or dynamic-only (default).
#define SEARCH_MAYBE_STATIC		((ULONG) 0x00020000)

// [MS-OXPROPS].pdf
#define PR_7BIT_DISPLAY_NAME_A PROP_TAG( PT_STRING8, 0x39ff)
#define PR_7BIT_DISPLAY_NAME_W PROP_TAG( PT_UNICODE, 0x39ff)

// [MS-OXCSPAM].pdf
#define PR_SENDER_ID_STATUS PROP_TAG( PT_LONG, 0x4079)
// Values that PR_SENDER_ID_STATUS can take
#define SENDER_ID_NEUTRAL				0x1
#define SENDER_ID_PASS					0x2
#define SENDER_ID_FAIL					0x3
#define SENDER_ID_SOFT_FAIL				0x4
#define SENDER_ID_NONE					0x5
#define SENDER_ID_TEMP_ERROR			0x80000006
#define SENDER_ID_PERM_ERROR			0x80000007

#define PR_JUNK_THRESHOLD PROP_TAG(PT_LONG, 0x6101)
#define SPAM_FILTERING_NONE						0xFFFFFFFF
#define SPAM_FILTERING_LOW						0x00000006
#define SPAM_FILTERING_MEDIUM					0x00000005
#define SPAM_FILTERING_HIGH						0x00000003
#define SPAM_FILTERING_TRUSTED_ONLY				0x80000000

// [MS-OXOCAL].pdf
#define RECIP_UNSENDABLE	(int) 0x0000
#define RECIP_SENDABLE		(int) 0x0001
#define RECIP_ORGANIZER		(int) 0x0002   // send bit plus this one
#define RECIP_EXCEPTIONAL_RESPONSE	(int) 0x0010 // recipient has exceptional response
#define RECIP_EXCEPTIONAL_DELETED	(int) 0x0020 // recipient is NOT in this exception
#define RECIP_ORIGINAL		(int) 0x0100	// this was an original recipient on the meeting request

#define	respNone			0
#define	respOrganized		1
#define respTentative		2
#define respAccepted		3
#define respDeclined		4
#define respNotResponded	5

// [MS-OXOFLAG].pdf
#define PR_TODO_ITEM_FLAGS PROP_TAG(PT_LONG, 0x0e2b)
#define TDIP_None			0x00000000
#define TDIP_Active			0x00000001 // Object is time flagged
#define TDIP_ActiveRecip	0x00000008 // SHOULD only be set on a draft message object, and means that the object is flagged for recipients.

// [MS-OXORule].pdf
#define PR_RULE_MSG_STATE PROP_TAG(PT_LONG, 0x65e9)

// [MS-OXOSRCH].pdf
#define PR_WB_SF_STORAGE_TYPE PROP_TAG(PT_LONG, 0x6846)
enum t_SmartFolderStorageType {
	SFST_NUMBER = 0x01,			// for template's data (numbers)
	SFST_TEXT = 0x02,			// for template's data (strings)
	SFST_BINARY = 0x04,			// for template's data (binary form, such as entry id, etc.)
	SFST_MRES = 0x08,			// for condition in MRES format
	SFST_FILTERSTREAM = 0x10,	// for condition in IStream format
	SFST_FOLDERNAME = 0x20,		// for folder list's names
	SFST_FOLDERLIST = 0x40,		// for the binary folder entrylist

	SFST_TIMERES_MONTHLY = 0x1000,	// monthly update
	SFST_TIMERES_WEEKLY = 0x2000,	// weekly update
	SFST_TIMERES_DAILY = 0x4000,	// the restriction(or filter) has a time condition in it
	SFST_DEAD= 0x8000,				// used to indicate there is not a valid SPXBIN
};

// [MS-OXOCAL].pdf
#define dispidBusyStatus 0x8205
typedef enum OlBusyStatus {
	olFree=0,
	olTentative=1,
	olBusy=2,
	olOutOfOffice=3,
} OlBusyStatus;
#define dispidApptAuxFlags 0x8207
#define	auxApptFlagCopied 0x0001
#define	auxApptFlagForceMtgResponse 0x0002
#define	auxApptFlagForwarded 0x0004

#define dispidApptColor 0x8214
#define dispidApptStateFlags 0x8217
#define	asfNone			0x0000
#define	asfMeeting		0x0001
#define asfReceived		0x0002
#define asfCancelled	0x0004
#define asfForward		0x0008

#define dispidResponseStatus 0x8218
#define dispidRecurType 0x8231
#define rectypeNone		(int) 0
#define rectypeDaily	(int) 1
#define rectypeWeekly	(int) 2
#define rectypeMonthly	(int) 3
#define rectypeYearly	(int) 4

#define dispidConfType 0x8241
typedef enum {
	confNetMeeting = 0,
	confNetShow,
	confExchange
} confType;

#define dispidChangeHighlight 0x8204
#define BIT_CH_START		0x00000001
#define BIT_CH_END			0x00000002
#define BIT_CH_RECUR		0x00000004
#define BIT_CH_LOCATION		0x00000008
#define BIT_CH_SUBJECT		0x00000010
#define BIT_CH_REQATT		0x00000020
#define BIT_CH_OPTATT		0x00000040
#define BIT_CH_BODY			0x00000080
#define BIT_CH_CUSTOM		0x00000100
#define BIT_CH_SILENT		0x00000200
#define BIT_CH_ALLOWPROPOSE 0x00000400
#define BIT_CH_CONF			0x00000800
#define BIT_CH_ATT_REM		0x00001000
#define BIT_CH_NOTUSED		0x80000000

#define dispidMeetingType 0x0026
#define mtgEmpty         0x00000000
#define mtgRequest       0x00000001
#define mtgFull          0x00010000
#define mtgInfo          0x00020000
#define mtgOutofDate     0x00080000
#define mtgDelegatorCopy 0x00100000

#define dispidIntendedBusyStatus 0x8224

#define PR_ATTACHMENT_FLAGS PROP_TAG(PT_LONG, 0x7FFD)
#define afException				0x02   // This is an exception to a recurrence

#define dispidNonSendToTrackStatus 0x8543
#define dispidNonSendCcTrackStatus 0x8544
#define dispidNonSendBccTrackStatus 0x8545

#define LID_CALENDAR_TYPE 0x001C
#define CAL_DEFAULT           0
#define CAL_JAPAN_LUNAR      14
#define CAL_CHINESE_LUNAR    15
#define CAL_SAKA             16
#define CAL_LUNAR_KOREAN     20

// [MS-OXCMSG].pdf
#define dispidSideEffects 0x8510
#define seOpenToDelete		0x0001	// Additional processing is required on the Message object when deleting.
#define seNoFrame			0x0008	// No UI is associated with the Message object.
#define seCoerceToInbox		0x0010	// Additional processing is required on the Message object when moving or
									// copying to a Folder object with a PR_CONTAINER_CLASS of 'IPF.Note'.
#define seOpenToCopy		0x0020	// Additional processing is required on the Message object when copying to
									// another folder.
#define seOpenToMove		0x0040	// Additional processing is required on the Message object when moving to
									// another folder.
#define seOpenForCtxMenu    0x0100  // Additional processing is required on the Message object when displaying verbs
									// to the end-user.
#define seCannotUndoDelete	0x0400	// Cannot undo delete operation, MUST NOT be set unless seOpenToDelete is set.
#define seCannotUndoCopy	0x0800	// Cannot undo copy operation, MUST NOT be set unless seOpenToCopy is set.
#define seCannotUndoMove	0x1000	// Cannot undo move operation, MUST NOT be set unless seOpenToMove is set.
#define seHasScript			0x2000	// The Message object contains end-user script.
#define seOpenToPermDelete	0x4000	// Additional processing is required to permanently delete the Message object.

// [MS-OXOCNTC].pdf
#define dispidFileUnderId 0x8006
#define dispidFileUnderList 0x8026
enum {
	FILEUNDERID_NONE = 0,
	FILEUNDERID_CUSTOM = 0xffffffff,
	FILEUNDERID_CALLINIT = 0xfffffffe,
	FILEUNDERID_CALCULATE = 0xfffffffd
};
#define dispidLastNameAndFirstName 0x8017
#define dispidCompanyAndFullName 0x8018
#define dispidFullNameAndCompany 0x8019
#define dispidLastFirstNoSpace			0x8030
#define dispidLastFirstSpaceOnly		0x8031
#define dispidCompanyLastFirstNoSpace	0x8032
#define dispidCompanyLastFirstSpaceOnly	0x8033
#define dispidLastFirstNoSpaceCompany	0x8034
#define dispidLastFirstSpaceOnlyCompany	0x8035
#define dispidLastFirstAndSuffix		0x8036
#define dispidFirstMiddleLastSuffix		0x8037
#define dispidLastFirstNoSpaceAndSuffix	0x8038

#define dispidPostalAddressId 0x8022
enum PostalAddressIndex
{ ADDRESS_NONE = 0, ADDRESS_HOME, ADDRESS_WORK, ADDRESS_OTHER };

// [MS_OXOTASK].pdf
#define dispidTaskMode 0x8518
enum TaskDelegMsgType {
	tdmtNothing = 0,	// The task object is not assigned.
	tdmtTaskReq,		// The task object is embedded in a task request.
	tdmtTaskAcc,		// The task object has been accepted by the task assignee.
	tdmtTaskDec,		// The task object was rejected by the task assignee.
	tdmtTaskUpd,		// The task object is embedded in a task update.
	tdmtTaskSELF		// The task object was assigned to the task assigner (self-delegation).
};

#define dispidTaskStatus 0x8101
enum TaskStatusValue {
	tsvNotStarted = 0, // The user has not started work on the task object. If this value is set,
	                   // dispidPercentComplete MUST be 0.0.
	tsvInProgress,     // The user�s work on this task object is in progress. If this value is set,
	                   // dispidPercentComplete MUST be greater than 0.0 and less than 1.0
	tsvComplete,       // The user�s work on this task object is complete. If this value is set,
	                   // dispidPercentComplete MUST be 1.0, dispidTaskDateCompleted
	                   // MUST be the current date, and dispidTaskComplete MUST be true.
	tsvWaiting,        // The user is waiting on somebody else.
	tsvDeferred        // The user has deferred work on the task object.
};

#define dispidTaskState 0x8113
enum TaskDelegState {
	tdsNOM = 0, // This task object was created to correspond to a task object that was
	            // embedded in a task rejection but could not be found locally.
	tdsOWNNEW,  // The task object is not assigned.
	tdsOWN,     // The task object is the task assignee�s copy of an assigned task object.
	tdsACC,     // The task object is the task assigner�s copy of an assigned task object.
	tdsDEC      // The task object is the task assigner�s copy of a rejected task object.
};

#define dispidTaskHistory 0x811A
enum TaskHistory {
	thNone = 0,       // No changes were made.
	thAccepted,       // The task assignee accepted this task object.
	thDeclined,       // The task assignee rejected this task object.
	thUpdated,        // Another property was changed.
	thDueDateChanged, // The dispidTaskDueDate property changed.
	thAssigned        // The task object has been assigned to a task assignee.
};

#define dispidTaskMultRecips 0x8120
enum TaskMultRecips {
	tmrNone				= 0x0000,	// none
	tmrSent				= 0x0001,	// The task object has multiple primary recipients.
	tmrRcvd				= 0x0002,	// Although the 'Sent' hint was not present, the client detected
									// that the task object has multiple primary recipients.
	tmrTeamTask			= 0x0004,	// This value is reserved.
};

#define dispidTaskOwnership 0x8129
enum TaskOwnershipValue {
	tovNew,   // The task object is not assigned.
	tovDeleg, // The task object is the task assigner�s copy of the task object.
	tovMe     // The task object is the task assignee�s copy of the task object.
};

#define dispidTaskDelegValue 0x812A
enum TaskDelegValue {
	tdvNone,      // The task object is not assigned.
	tdvUnknown,   // The task object�s acceptance status is unknown.
	tdvAccepted,  // The task assignee has accepted the task object. This value is set when
	// the client processes a task acceptance.
	tdvDeclined   // The task assignee has rejected the task object. This value is set when the
	// client processes a task rejection.
};

#define dispidLogFlags 0x870C
#define lfContactLog ((ULONG) 0x40000000L) // This journal object has a journal associated attachment

// [MS-OXOMSG].pdf
#define dispidSniffState 0x851A
enum SniffState {
	ssNone,    // Don't auto-process the message.
	ssOnSniff, // Process the message automatically or when the message is opened.
	ssOnOpen   // Process when the message is opened only.
};

#define dispidNoteColor 0x8B00
#define ncBlue 0
#define ncGreen 1
#define ncPink 2
#define ncYellow 3
#define ncWhite	4

// http://msdn.microsoft.com/en-us/library/bb821181.aspx
#define dispidTimeZoneStruct 0x8233
#define dispidApptTZDefStartDisplay 0x825E
#define dispidApptTZDefEndDisplay 0x825F
#define dispidApptTZDefRecur 0x8260

#define PR_EXTENDED_FOLDER_FLAGS PROP_TAG(PT_BINARY, 0x36DA)
typedef enum ExtendedFolderPropByte
{
	EFPB_FLAGS						= 1,
	EFPB_CLSIDID					= 2,
	EFPB_SFTAG						= 3,
	EFPB_TODO_VERSION				= 5,
} EFPB;
// possible values for PR_EXTENDED_FOLDER_FLAGS
enum
{
	XEFF_NORMAL					= 0x00000000,
	XEFF_SHOW_UNREAD_COUNT		= 0x00000001,
	XEFF_SHOW_CONTENT_COUNT		= 0x00000002,
	XEFF_SHOW_NO_POLICY			= 0x00000020,
};

#define dispidFlagStringEnum 0x85C0

// [MS-OXOCAL].pdf
#define dispidApptRecur 0x8216
#define ARO_SUBJECT          0x0001
#define ARO_MEETINGTYPE      0x0002
#define ARO_REMINDERDELTA    0x0004
#define ARO_REMINDER         0x0008
#define ARO_LOCATION         0x0010
#define ARO_BUSYSTATUS       0x0020
#define ARO_ATTACHMENT       0x0040
#define ARO_SUBTYPE          0x0080
#define ARO_APPTCOLOR        0x0100
#define ARO_EXCEPTIONAL_BODY 0x0200

enum IdGroup
{
	IDC_RCEV_PAT_ORB_DAILY = 0x200A,
	IDC_RCEV_PAT_ORB_WEEKLY,
	IDC_RCEV_PAT_ORB_MONTHLY,
	IDC_RCEV_PAT_ORB_YEARLY,
	IDC_RCEV_PAT_ERB_END=0x2021,
	IDC_RCEV_PAT_ERB_AFTERNOCCUR,
	IDC_RCEV_PAT_ERB_NOEND,
};

enum
{
	rptMinute = 0,
	rptWeek,
	rptMonth,
	rptMonthNth,
	rptMonthEnd,
	rptHjMonth = 10,
	rptHjMonthNth,
	rptHjMonthEnd
};

const ULONG rpn1st = 1;
const ULONG rpn2nd = 2;
const ULONG rpn3rd = 3;
const ULONG rpn4th = 4;
const ULONG rpnLast = 5;

const ULONG rdfSun = 0x01;
const ULONG rdfMon = 0x02;
const ULONG rdfTue = 0x04;
const ULONG rdfWed = 0x08;
const ULONG rdfThu = 0x10;
const ULONG rdfFri = 0x20;
const ULONG rdfSat = 0x40;

// [MS-OXOTASK].pdf
#define dispidTaskRecur 0x8116
#define dispidTaskMyDelegators 0x8117

// [MS-OXOCAL].pdf
#define LID_GLOBAL_OBJID 0x0003
#define LID_CLEAN_GLOBAL_OBJID 0x0023

// [MS-OXOSRCH].pdf
#define PR_WB_SF_DEFINITION PROP_TAG(PT_BINARY, 0x6845)
#define PR_WB_SF_LAST_USED PROP_TAG(PT_LONG, 0x6834)
#define PR_WB_SF_EXPIRATION PROP_TAG(PT_LONG, 0x683A)
#define PT_SVREID ((ULONG) 0x00FB)

// [MS-OXOPFFB].pdf
#define PR_FREEBUSY_PUBLISH_START PROP_TAG(PT_LONG, 0x6847)
#define PR_FREEBUSY_PUBLISH_END PROP_TAG(PT_LONG, 0x6848)

// [MS-OSCDATA].pdf
#define OOP_DONT_LOOKUP ((ULONG) 0x10000000)

// [MS-OXCDATA].pdf
#define eitLTPrivateFolder		(0x1) // PrivateFolder
#define eitLTPublicFolder		(0x3) // PublicFolder
#define eitLTWackyFolder		(0x5)	// MappedPublicFolder
#define eitLTPrivateMessage		(0x7) // PrivateMessage
#define eitLTPublicMessage		(0x9) // PublicMessage
#define eitLTWackyMessage		(0xB) // MappedPublicMessage
#define eitLTPublicFolderByName	(0xC) // PublicNewsgroupFolder

// Exchange Address Book Version
#define EMS_VERSION         0x000000001

// Wrapped Message Store Version
#define MAPIMDB_VERSION	((BYTE) 0x00)

// Wrapped Message Store Flag
#define MAPIMDB_NORMAL	((BYTE) 0x00)

// Contact Address Book Version
#define CONTAB_VERSION    0x000000003

// Contact Addess Book Types
#define CONTAB_USER       0x00000004
#define CONTAB_DISTLIST   0x00000005

// Contact Address Book Index
enum EEmailIndex
{
	EEI_EMAIL_1 = 0,
	EEI_EMAIL_2,
	EEI_EMAIL_3,
	EEI_FAX_1,
	EEI_FAX_2,
	EEI_FAX_3,
};
#define EMAIL_TYPE_UNDEFINED 0xFF

enum
{
	BFLAGS_INTERNAL_MAILUSER	= 0x03,		// Outlook Contact
	BFLAGS_INTERNAL_DISTLIST,				// Outlook Distribution List
	BFLAGS_EXTERNAL_MAILUSER,				// external (MAPI) Contact
	BFLAGS_EXTERNAL_DISTLIST,				// external (MAPI) Distribution List
	BFLAGS_MASK_OUTLOOK			= 0x80,		// bit pattern 1000 0000
};

#define dispidEmail1OriginalEntryID 0x8085
#define dispidEmail2OriginalEntryID 0x8095
#define dispidEmail3OriginalEntryID 0x80A5
#define dispidFax1EntryID 0x80B5
#define dispidFax2EntryID 0x80C5
#define dispidFax3EntryID 0x80D5
#define dispidSelectedOriginalEntryID 0x800A
#define dispidAnniversaryEventEID 0x804E
#define dispidOrigStoreEid 0x8237
#define dispidReferenceEID 0x85BD
#define dispidSharingInitiatorEid 0x8A09
#define dispidSharingFolderEid 0x8A15
#define dispidSharingOriginalMessageEid 0x8A29
#define dispidSharingBindingEid 0x8A2D
#define dispidSharingIndexEid 0x8A2E
#define dispidSharingParentBindingEid 0x8A5C

// Property Definition Stream
#define	PropDefV1			0x102
#define PropDefV2			0x103

#define	PDO_IS_CUSTOM		0x00000001	// This FieldDefinition structure contains a definition of a user-defined field.
#define	PDO_REQUIRED		0x00000002	// For a form control bound to this field, the checkbox for A value is required for this field is selected in the Validation tab of the Properties dialog box.
#define PDO_PRINT_SAVEAS	0x00000004	// For a form control bound to this field, the checkbox for Include this field for printing and Save As is selected in the Validation tab of the Properties dialog box.
#define PDO_CALC_AUTO		0x00000008	// For a form control bound to this field, the checkbox for Calculate this formula automatically option is selected in the Value tab of the Properties dialog box.
#define PDO_FT_CONCAT		0x00000010	// This is a field of type Combination and it has the 'Joining fields and any text fragments with each other' option selected in its Combination Formula Field dialog.
#define PDO_FT_SWITCH		0x00000020	// This field is of type Combination and has the Showing only the first non-empty field, ignoring subsequent ones option selected in the Combination Formula Field dialog box.
#define PDO_PRINT_SAVEAS_DEF	0x000000040	// This flag is not used

typedef enum
{
	iTypeUnknown = -1,
	iTypeString,		// 0
	iTypeNumber,		// 1
	iTypePercent,		// 2
	iTypeCurrency,		// 3
	iTypeBool,			// 4
	iTypeDateTime,		// 5
	iTypeDuration,		// 6
	iTypeCombination,	// 7
	iTypeFormula,		// 8
	iTypeIntResult,		// 9
	iTypeVariant,		// 10
	iTypeFloatResult,	// 11
	iTypeConcat,		// 12
	iTypeKeywords,		// 13
	iTypeInteger,		// 14
} iTypeEnum;

// [MS-OXOSFLD].pdf
#define PR_ADDITIONAL_REN_ENTRYIDS_EX PROP_TAG( PT_BINARY, 0x36d9)

#define		RSF_PID_TREAT_AS_SF			0x8000
#define		RSF_PID_RSS_SUBSCRIPTION	(RSF_PID_TREAT_AS_SF | 1)
#define		RSF_PID_SEND_AND_TRACK		(RSF_PID_TREAT_AS_SF | 2)
#define		RSF_PID_TODO_SEARCH			(RSF_PID_TREAT_AS_SF | 4)
#define		RSF_PID_CONV_ACTIONS		(RSF_PID_TREAT_AS_SF | 6)
#define		RSF_PID_COMBINED_ACTIONS	(RSF_PID_TREAT_AS_SF | 7)
#define		RSF_PID_SUGGESTED_CONTACTS	(RSF_PID_TREAT_AS_SF | 8)

#define		RSF_ELID_ENTRYID			1
#define		RSF_ELID_HEADER				2

#define PR_TARGET_ENTRYID PROP_TAG( PT_BINARY, 0x3010)

#define PR_SCHDINFO_DELEGATE_ENTRYIDS PROP_TAG( PT_MV_BINARY, 0x6845)

#define PR_EMSMDB_SECTION_UID PROP_TAG( PT_BINARY, 0x3d15)

#define EDK_PROFILEUISTATE_ENCRYPTNETWORK 0x4000

#define PR_CONTAB_FOLDER_ENTRYIDS PROP_TAG( PT_MV_BINARY, 0x6620)
#define PR_CONTAB_STORE_ENTRYIDS PROP_TAG( PT_MV_BINARY, 0x6626)
#define PR_CONTAB_STORE_SUPPORT_MASKS PROP_TAG( PT_MV_LONG, 0x6621)
#define PR_DELEGATE_FLAGS PROP_TAG( PT_MV_LONG, 0x686b)

#define PR_EXTENDED_RULE_MSG_CONDITION PROP_TAG( PT_BINARY, 0x0E9A)

// [MS-OXOCNTC].pdf
#define dispidDLOneOffMembers 0x8054
#define dispidDLMembers 0x8055
#define dispidABPEmailList 0x8028
#define dispidABPArrayType 0x8029

#define PR_CONVERSATION_ID PROP_TAG( PT_BINARY, 0x3013)
#define PR_FOLDER_WEBVIEWINFO PROP_TAG( PT_BINARY, 0x36DF)

#define WEBVIEW_PERSISTENCE_VERSION 0x000000002
#define WEBVIEWURL 0x00000001
#define WEBVIEW_FLAGS_SHOWBYDEFAULT 0x00000001

#define dispidContactLinkEntry 0x8585

#define dispidApptUnsendableRecips 0x825D
#define dispidForwardNotificationRecipients 0x8261

// http://msdn.microsoft.com/en-us/library/ee218029(EXCHG.80).aspx
#define PR_NATIVE_BODY_INFO PROP_TAG( PT_LONG, 0x1016)
typedef enum
{
	nbiUndefined = 0,
	nbiPlainText,
	nbiRtfCompressed,
	nbiHtml,
	nbiClearSigned,
} NBI;

#define ptagSenderFlags PROP_TAG( PT_LONG, 0x4019)
#define ptagSentRepresentingFlags PROP_TAG( PT_LONG, 0x401A)

#define PR_LAST_VERB_EXECUTED PROP_TAG( PT_LONG, 0x1081)
#define NOTEIVERB_OPEN				0
#define NOTEIVERB_REPLYTOSENDER		102
#define NOTEIVERB_REPLYTOALL		103
#define NOTEIVERB_FORWARD			104
#define NOTEIVERB_PRINT				105
#define NOTEIVERB_SAVEAS			106
#define NOTEIVERB_REPLYTOFOLDER		108
#define NOTEIVERB_SAVE				500
#define NOTEIVERB_PROPERTIES		510
#define NOTEIVERB_FOLLOWUP			511
#define NOTEIVERB_ACCEPT			512
#define NOTEIVERB_TENTATIVE			513
#define NOTEIVERB_REJECT			514
#define NOTEIVERB_DECLINE			515
#define NOTEIVERB_INVITE			516
#define NOTEIVERB_UPDATE			517
#define NOTEIVERB_CANCEL			518
#define NOTEIVERB_SILENTINVITE		519
#define NOTEIVERB_SILENTCANCEL		520
#define NOTEIVERB_RECALL_MESSAGE	521
#define NOTEIVERB_FORWARD_RESPONSE	522
#define NOTEIVERB_FORWARD_CANCEL	523
#define NOTEIVERB_FOLLOWUPCLEAR		524
#define NOTEIVERB_FORWARD_APPT		525
#define NOTEIVERB_OPENRESEND		526
#define NOTEIVERB_STATUSREPORT		527
#define NOTEIVERB_JOURNALOPEN		528
#define NOTEIVERB_JOURNALOPENLINK	529
#define NOTEIVERB_COMPOSEREPLACE	530
#define NOTEIVERB_EDIT				531
#define NOTEIVERB_DELETEPROCESS		532
#define NOTEIVERB_TENTPNTIME		533
#define NOTEIVERB_EDITTEMPLATE		534
#define NOTEIVERB_FINDINCALENDAR	535
#define NOTEIVERB_FORWARDASFILE		536
#define NOTEIVERB_CHANGE_ATTENDEES	537
#define NOTEIVERB_RECALC_TITLE		538
#define NOTEIVERB_PROP_CHANGE		539
#define NOTEIVERB_FORWARD_AS_VCAL	540
#define NOTEIVERB_FORWARD_AS_ICAL	541
#define NOTEIVERB_FORWARD_AS_BCARD	542
#define NOTEIVERB_DECLPNTIME		543
#define NOTEIVERB_PROCESS			544
#define NOTEIVERB_OPENWITHWORD		545
#define NOTEIVERB_OPEN_INSTANCE_OF_SERIES	546
#define NOTEIVERB_FILLOUT_THIS_FORM	547
#define NOTEIVERB_FOLLOWUP_DEFAULT	548
#define NOTEIVERB_REPLY_WITH_MAIL	549
#define NOTEIVERB_TODO_TODAY		566
#define NOTEIVERB_TODO_TOMORROW		567
#define NOTEIVERB_TODO_THISWEEK		568
#define NOTEIVERB_TODO_NEXTWEEK		569
#define NOTEIVERB_TODO_THISMONTH	570
#define NOTEIVERB_TODO_NEXTMONTH	571
#define NOTEIVERB_TODO_NODATE		572
#define NOTEIVERB_FOLLOWUPCOMPLETE	573
#define NOTEIVERB_COPYTOPOSTFOLDER	574
#define NOTEIVERB_PARTIALRECIP_SILENTINVITE		575
#define NOTEIVERB_PARTIALRECIP_SILENTCANCEL		576
