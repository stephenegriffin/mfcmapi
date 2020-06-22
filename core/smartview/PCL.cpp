#include <core/stdafx.h>
#include <core/smartview/PCL.h>
#include <core/interpret/guid.h>

namespace smartview
{
	SizedXID::SizedXID(const std::shared_ptr<binaryParser>& parser)
	{
		XidSize = blockT<BYTE>::parse(parser);
		NamespaceGuid = blockT<GUID>::parse(parser);
		const auto cbLocalId = *XidSize - sizeof(GUID);
		if (parser->getSize() >= cbLocalId)
		{
			LocalID = blockBytes::parse(parser, cbLocalId);
		}
	}

	void PCL::parse()
	{
		auto cXID = 0;
		// Run through the parser once to count the number of flag structs
		// Must have at least 1 byte left to have another XID
		while (parser->getSize() > sizeof(BYTE))
		{
			const auto XidSize = blockT<BYTE>::parse(parser);
			if (parser->getSize() >= *XidSize)
			{
				parser->advance(*XidSize);
			}

			cXID++;
		}

		// Now we parse for real
		parser->rewind();

		if (cXID && cXID < _MaxEntriesSmall)
		{
			m_lpXID.reserve(cXID);
			for (auto i = 0; i < cXID; i++)
			{
				const auto oldSize = parser->getSize();
				m_lpXID.emplace_back(std::make_shared<SizedXID>(parser));
				const auto newSize = parser->getSize();
				if (newSize == 0 || newSize == oldSize) break;
			}
		}
	}

	void PCL::parseBlocks()
	{
		setText(L"Predecessor Change List");
		addHeader(L"Count = %1!d!", m_lpXID.size());

		if (!m_lpXID.empty())
		{
			auto i = 0;
			for (const auto& xid : m_lpXID)
			{
				auto xidBlock = create(L"XID[%1!d!]:", i);
				addChild(xidBlock);
				xidBlock->addChild(xid->XidSize, L"XidSize = 0x%1!08X! = %1!d!", xid->XidSize->getData());
				xidBlock->addChild(
					xid->NamespaceGuid,
					L"NamespaceGuid = %1!ws!",
					guid::GUIDToString(xid->NamespaceGuid->getData()).c_str());
				xidBlock->addLabeledChild(L"LocalId =", xid->LocalID);

				i++;
			}
		}
	}
} // namespace smartview