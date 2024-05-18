#pragma once
#include "EditorEventsNameSpace.hpp"

#if UCodeGEDebugMode
UCodeEditorEventStart

using AssetUpdatedCallBack = std::function<void()>;
EditorEventID AddAssetUpdatedEditorEvent(AssetUpdatedCallBack reupdate,AssetUpdatedCallBack updated,UID assetid);
void RemoveAssetUpdateEditorEvent(EditorEventID id);

void Editor_Only_CallAssetUpdatedPre(UID id);
void Editor_Only_CallAssetUpdated(UID id);

UCodeEditorEventEnd
#endif
