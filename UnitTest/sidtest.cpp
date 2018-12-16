#include <StdAfx.h>
#include <CppUnitTest.h>
#include <UnitTest/UnitTest.h>
#include <Interpret/Sid.h>

namespace sidtest
{
	TEST_CLASS(sidtest)
	{
	public:
		// Without this, clang gets weird
		static const bool dummy_var = true;

		TEST_CLASS_INITIALIZE(Initialize)
		{
			// Set up our property arrays or nothing works
			addin::MergeAddInArrays();

			registry::RegKeys[registry::regkeyDO_SMART_VIEW].ulCurDWORD = 1;
			registry::RegKeys[registry::regkeyUSE_GETPROPLIST].ulCurDWORD = 1;
			registry::RegKeys[registry::regkeyPARSED_NAMED_PROPS].ulCurDWORD = 1;
			registry::RegKeys[registry::regkeyCACHE_NAME_DPROPS].ulCurDWORD = 1;

			strings::setTestInstance(GetModuleHandleW(L"UnitTest.dll"));
		}

		TEST_METHOD(Test_GetTextualSid)
		{
			Assert::AreEqual(std::wstring{}, sid::GetTextualSid({}));

			auto simpleSidBin = strings::HexStringToBin(L"010500000000000515000000A065CF7E784B9B5FE77C8770091C0100");
			Assert::AreEqual(
				std::wstring{L"S-1-5-21-2127521184-1604012920-1887927527-72713"}, sid::GetTextualSid(simpleSidBin));
			auto simpleSidAccount = sid::LookupAccountSid(simpleSidBin);
			Assert::AreEqual(std::wstring{L"(no domain)"}, simpleSidAccount.getDomain());
			Assert::AreEqual(std::wstring{L"(no name)"}, simpleSidAccount.getName());

			Assert::AreEqual(
				std::wstring{L"S-1-000102030405-21-2127521184-1604012920-1887927527-72713"},
				sid::GetTextualSid(
					strings::HexStringToBin(L"010500010203040515000000A065CF7E784B9B5FE77C8770091C0100")));

			auto authenticatedUsersSidBin = strings::HexStringToBin(L"01 01 000000000005 0B000000");
			Assert::AreEqual(std::wstring{L"S-1-5-11"}, sid::GetTextualSid(authenticatedUsersSidBin));
			auto authenticatedUsersSidAccount = sid::LookupAccountSid(authenticatedUsersSidBin);
			Assert::AreEqual(std::wstring{L"NT AUTHORITY"}, authenticatedUsersSidAccount.getDomain());
			Assert::AreEqual(std::wstring{L"Authenticated Users"}, authenticatedUsersSidAccount.getName());
		}

		TEST_METHOD(Test_SidAccount)
		{
			auto emptyAccount = sid::SidAccount{};
			Assert::AreEqual(std::wstring{L"(no domain)"}, emptyAccount.getDomain());
			Assert::AreEqual(std::wstring{L"(no name)"}, emptyAccount.getName());

			auto account = sid::SidAccount{L"foo", L"bar"};
			Assert::AreEqual(std::wstring{L"foo"}, account.getDomain());
			Assert::AreEqual(std::wstring{L"bar"}, account.getName());
		}

		TEST_METHOD(Test_SDToString)
		{
			auto nullsd = SDToString({}, sid::acetypeContainer);
			Assert::AreEqual(std::wstring{L""}, nullsd.dacl);
			Assert::AreEqual(std::wstring{L""}, nullsd.info);

			auto invalid = SDToString(strings::HexStringToBin(L"B606B07ABB6079AB2082C760"), sid::acetypeContainer);
			Assert::AreEqual(std::wstring{L"This is not a valid security descriptor."}, invalid.dacl);
			Assert::AreEqual(std::wstring{L""}, invalid.info);

			auto sd = SDToString(
				strings::HexStringToBin(L"0800030000000000010007801C000000280000000000000014000000020008000000000001010"
										L"000000000051200000001020000000000052000000020020000"),
				sid::acetypeContainer);
			Assert::AreEqual(std::wstring{L""}, sd.dacl);
			Assert::AreEqual(std::wstring{L"0x0"}, sd.info);

			auto sd1 = SDToString(
				strings::HexStringToBin(
					L"080003000000000001000780F40000000001000000000000140000000200E0000600000000092400A9081200010500000"
					L"000000515000000271A6C07352F372AAD20FA5B019301000109240016071F00010500000000000515000000271A6C0735"
					L"2F372AAD20FA5B0193010001092400BF0F1F00010500000000000515000000271A6C07352F372AAD20FA5BAA830B00000"
					L"22400A9081200010500000000000515000000271A6C07352F372AAD20FA5B019301000102240016C90D00010500000000"
					L"000515000000271A6C07352F372AAD20FA5B0193010001022400BFC91F00010500000000000515000000271A6C07352F3"
					L"72AAD20FA5BAA830B0001010000000000051200000001020000000000052000000020020000"),
				sid::acetypeContainer);
			unittest::AreEqualEx(
				std::wstring{
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-103169\r\n"
					L"Access Type: 0x00000000 = ACCESS_ALLOWED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000009 = OBJECT_INHERIT_ACE | INHERIT_ONLY_ACE\r\n"
					L"Access Mask: 0x001208A9 = fsdrightListContents | fsdrightReadProperty | fsdrightExecute | "
					L"fsdrightReadAttributes | fsdrightViewItem | fsdrightReadControl | fsdrightSynchronize\r\n"
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-103169\r\n"
					L"Access Type: 0x00000001 = ACCESS_DENIED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000009 = OBJECT_INHERIT_ACE | INHERIT_ONLY_ACE\r\n"
					L"Access Mask: 0x001F0716 = fsdrightCreateItem | fsdrightCreateContainer | fsdrightWriteProperty | "
					L"fsdrightWriteAttributes | fsdrightWriteSD | fsdrightDelete | fsdrightWriteOwner | "
					L"fsdrightReadControl | fsdrightSynchronize | 0x600\r\n"
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-754602\r\n"
					L"Access Type: 0x00000001 = ACCESS_DENIED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000009 = OBJECT_INHERIT_ACE | INHERIT_ONLY_ACE\r\n"
					L"Access Mask: 0x001F0FBF = fsdrightListContents | fsdrightCreateItem | fsdrightCreateContainer | "
					L"fsdrightReadProperty | fsdrightWriteProperty | fsdrightExecute | fsdrightReadAttributes | "
					L"fsdrightWriteAttributes | fsdrightViewItem | fsdrightWriteSD | fsdrightDelete | "
					L"fsdrightWriteOwner | fsdrightReadControl | fsdrightSynchronize | 0x600\r\n"
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-103169\r\n"
					L"Access Type: 0x00000000 = ACCESS_ALLOWED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000002 = CONTAINER_INHERIT_ACE\r\n"
					L"Access Mask: 0x001208A9 = fsdrightListContents | fsdrightReadProperty | fsdrightExecute | "
					L"fsdrightReadAttributes | fsdrightViewItem | fsdrightReadControl | fsdrightSynchronize\r\n"
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-103169\r\n"
					L"Access Type: 0x00000001 = ACCESS_DENIED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000002 = CONTAINER_INHERIT_ACE\r\n"
					L"Access Mask: 0x000DC916 = fsdrightCreateItem | fsdrightCreateContainer | fsdrightWriteProperty | "
					L"fsdrightWriteAttributes | fsdrightViewItem | fsdrightOwner | fsdrightContact | fsdrightWriteSD | "
					L"fsdrightDelete | fsdrightWriteOwner\r\n"
					L"Account: (no domain)\\(no name)\r\n"
					L"SID: S-1-5-21-124525095-708259637-1543119021-754602\r\n"
					L"Access Type: 0x00000001 = ACCESS_DENIED_ACE_TYPE\r\n"
					L"Access Flags: 0x00000002 = CONTAINER_INHERIT_ACE\r\n"
					L"Access Mask: 0x001FC9BF = fsdrightListContents | fsdrightCreateItem | fsdrightCreateContainer | "
					L"fsdrightReadProperty | fsdrightWriteProperty | fsdrightExecute | fsdrightReadAttributes | "
					L"fsdrightWriteAttributes | fsdrightViewItem | fsdrightOwner | fsdrightContact | fsdrightWriteSD | "
					L"fsdrightDelete | fsdrightWriteOwner | fsdrightReadControl | fsdrightSynchronize"},
				sd1.dacl);
			unittest::AreEqualEx(std::wstring{L"0x0"}, sd1.info);
		}
	};
} // namespace sidtest