#include "stickers.h"

#include "skinchanger.h"

typedef float(__fastcall *AttributeFunction)(void*, void*, int, Stickers::EStickerAttributeType, float);
typedef unsigned int(__fastcall *IndexFunction)(void*, void*, int, Stickers::EStickerAttributeType, unsigned fl);

AttributeFunction oAttribFn;
IndexFunction oIndexFn;

enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_iwoff = 0;

static void* o_float_fn;

static float __fastcall hooked_float_fn(void* thisptr, void*, int slot, EStickerAttributeType attribute, float fl)
{
	auto item = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_iwoff);

	switch (attribute)
	{
	case EStickerAttributeType::Wear:
		return FLT_MIN;
	case EStickerAttributeType::Scale:
		return 1.f;
	case EStickerAttributeType::Rotation:
		return 0.f;
	}

	return reinterpret_cast<decltype(hooked_float_fn)*>(o_float_fn)(thisptr, nullptr, slot, attribute, fl);
}

static void* o_uint_fn;

static unsigned int __fastcall hooked_uint_fn(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned fl)
{
	/*auto item = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_iwoff);*/

	switch (attribute)
	{
	case EStickerAttributeType::Index:
		return Settings::Skinchanger::Skins::sticker1;
	}

	return reinterpret_cast<decltype(hooked_uint_fn)*>(o_uint_fn)(thisptr, nullptr, slot, attribute, fl);
}

void Stickers::ApplyStickerHooks(C_BaseAttributableItem* item)
{
	if (offsets.CEconEntity.m_Item == 0xC)
		return;

	void**& stickerInfo = *reinterpret_cast<void***>((uintptr_t)item + offsets.CEconEntity.m_Item);

	static void** stickerInfoHook = nullptr;

	if (!stickerInfoHook)
	{
		size_t length = 0;

		for (; stickerInfo[length]; ++length);

		stickerInfoHook = new void*[length];
		memcpy(stickerInfoHook, stickerInfo, length * sizeof(void*));

		oAttribFn = reinterpret_cast<AttributeFunction>(stickerInfoHook[4]);
		stickerInfoHook[4] = &hooked_float_fn;

		oIndexFn = reinterpret_cast<IndexFunction>(stickerInfoHook[5]);
		stickerInfoHook[5] = &hooked_uint_fn;
	}

	stickerInfo = stickerInfoHook;

}
