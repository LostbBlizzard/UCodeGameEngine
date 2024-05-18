#include "AssetUpdateEvent.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
#if UCodeGEDebugMode
UCodeEditorEventStart

struct AssetUpdatedStruct
{
	EditorEventID nextid = {};
	EditorEventID GetNextID()
	{
		auto r = nextid;
		nextid++;
		return r;
	}

	struct CallBackWithEditorEvent
	{
		AssetUpdatedCallBack precallback;
		AssetUpdatedCallBack callback;
		EditorEventID id;
	};
	UnorderedMap<UID, Vector<CallBackWithEditorEvent>> CallBacks;
};
static AssetUpdatedStruct AssetUpdatedData;

EditorEventID AddAssetUpdatedEditorEvent(AssetUpdatedCallBack preupdate,AssetUpdatedCallBack updated,UID assetid)
{
	//return 0;
	Threads::ThrowErrIfNotOnMainThread();
	
	auto newid = AssetUpdatedData.GetNextID();

	AssetUpdatedStruct::CallBackWithEditorEvent item;
	item.id = newid;
	item.precallback = preupdate;
	item.callback = updated;

	AssetUpdatedData.CallBacks.GetOrAdd(assetid, {}).push_back(std::move(item));
	return newid;
}

Vector<EditorEventID> toremovefromlist;
void RemoveAssetUpdateEditorEvent(EditorEventID id)
{
	//return;
	Threads::ThrowErrIfNotOnMainThread();

	toremovefromlist.push_back(id);	
}
void Editor_Only_CallAssetUpdatedPre(UID id)
{
	Threads::ThrowErrIfNotOnMainThread();

	for (auto& Item : AssetUpdatedData.CallBacks)
	{
		auto& list = Item.second;

		list.erase(std::remove_if(
			list.begin(), list.end(),
			[id](const AssetUpdatedStruct::CallBackWithEditorEvent& x) {
				for (auto& Item : toremovefromlist) {
					return x.id == Item;
				}
				return false;
			}), list.end());

		toremovefromlist.clear();
	}
	if (AssetUpdatedData.CallBacks.HasValue(id))
	{
		auto& calls = AssetUpdatedData.CallBacks.GetValue(id);

		for (auto& Item : calls)
		{
			Item.precallback();
		}
	}
}
void Editor_Only_CallAssetUpdated(UID id)
{
	Threads::ThrowErrIfNotOnMainThread();
	for (auto& Item : AssetUpdatedData.CallBacks)
	{
		auto& list = Item.second;

		list.erase(std::remove_if(
			list.begin(), list.end(),
			[id](const AssetUpdatedStruct::CallBackWithEditorEvent& x) {
				for (auto& Item : toremovefromlist) {
					return x.id == Item;
				}
				return false;
			}), list.end());

		toremovefromlist.clear();
	}

	if (AssetUpdatedData.CallBacks.HasValue(id))
	{
		auto& calls = AssetUpdatedData.CallBacks.GetValue(id);

		auto count = calls.size();
		for (size_t i = 0; i < count; i++)
		{
			auto& Item = calls[i];

			Item.callback();
		}
	}


}
UCodeEditorEventEnd
#endif
