#include "f4se/GameData.h"

// 856197F11173AF60E35EBF54A88E7BF43AFC3588+305
RelocPtr <DataHandler*> g_dataHandler(0x0585A300);

// C5B21010DCF340FCDDDC7866C50C3D78AEF34CB5+6B
RelocPtr <bool> g_isGameDataReady(0x059E43B4);

RelocPtr <DefaultObjectMap*> g_defaultObjectMap(0x0585F120);
RelocPtr <SimpleLock> g_defaultObjectMapLock(0x0585FB88);

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return _stricmp(modInfo->name, m_stringToFind) == 0;
	}
};

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

UInt8 DataHandler::GetModIndex(const char* modName)
{
	return modList.modInfoList.GetIndexOf(LoadedModFinder(modName));
}

const ModInfo* DataHandler::LookupLoadedModByName(const char* modName)
{
	for(UInt32 i = 0; i < modList.loadedModCount; i++) {
		ModInfo * modInfo = modList.loadedMods[i];
		if(_stricmp(modInfo->name, modName) == 0)
			return modInfo;
	}

	return nullptr;
}

UInt8 DataHandler::GetLoadedModIndex(const char* modName)
{
	const ModInfo * modInfo = LookupLoadedModByName(modName);
	if(modInfo) {
		return modInfo->modIndex;
	}

	return -1;
}

BGSDefaultObject * DefaultObjectMap::GetDefaultObject(BSFixedString name)
{
	SimpleLocker locker(g_defaultObjectMapLock);
	if(*g_defaultObjectMap) {
		auto entry = (*g_defaultObjectMap)->Find(&name.data);
		if(entry) {
			return entry->defaultObject;
		}
	}
	return nullptr;
}