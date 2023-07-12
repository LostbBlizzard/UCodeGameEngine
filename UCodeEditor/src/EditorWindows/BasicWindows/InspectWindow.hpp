#pragma once



#include <EditorWindows/EditorWindow.hpp>

#include <Typedefs.hpp>


#include <UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
EditorStart


class InspectWindow :public EditorWindow
{
public:
	struct InspectDrawer
	{
		friend InspectWindow;
	public:
	UCODE_EDITOR_FORCEINLINE void SetPtrNull() { win->_Inspect._Data =nullptr; }
	UCODE_EDITOR_FORCEINLINE AnyManagedPtr GetPtr() { return  win->_Inspect._Data; }
	UCODE_EDITOR_FORCEINLINE void* GetData() { return  win->_Inspect._Data2; }

	UCODE_EDITOR_FORCEINLINE void DrawText(const String&  text) { win->DrawText(text.c_str()); }

	UCODE_EDITOR_FORCEINLINE bool AsssetField(const char* FieldName,UCode::TexturePtr&Value) { return win->AsssetField(FieldName, (void*&)Value, AsssetType::Texture); }
	UCODE_EDITOR_FORCEINLINE bool AsssetField(const char* FieldName,UCode::SpritePtr& Value) { return win->AsssetField(FieldName, (void*&)Value, AsssetType::Sprite); }
	UCODE_EDITOR_FORCEINLINE bool AsssetField(const char* FieldName,UCode::ShaderPtr& Value) { return win->AsssetField(FieldName, (void*&)Value, AsssetType::Shader); }

	UCODE_EDITOR_FORCEINLINE bool ToggleField(const char* FieldName, bool& Value) { return win->ToggleField(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool ColorField(const char* FieldName, UCode::Color& Value) { return win->ColorField(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool ColorField(const char* FieldName, UCode::ColorRGB& Value) { return win->ColorField(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool ColorField(const char* FieldName, UCode::Color32& Value) { return win->ColorField(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool ColorField(const char* FieldName, UCode::Color24& Value) { return win->ColorField(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool StringField(const char* FieldName, String&  Value) { return win->StringField(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool Buttion(const char* FieldName) { return win->Buttion(FieldName); }

	UCODE_EDITOR_FORCEINLINE bool Vec2Field(const char* FieldName, UCode::Vec2& Value) { return win->Vec2Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool Vec2iField(const char* FieldName, UCode::Vec2i& Value) { return win->Vec2Field(FieldName, Value); }
		
	UCODE_EDITOR_FORCEINLINE bool Vec3Field(const char* FieldName, UCode::Vec3& Value) { return win->Vec3Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool Vec3iField(const char* FieldName, UCode::Vec3i& Value) { return win->Vec3Field(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool float32Field(const char* FieldName,float& Value) { return win->float32Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool float64Field(const char* FieldName, double& Value) { return win->float64Field(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool uInt64Field(const char* FieldName, UInt64& Value) { return win->uInt64Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool uInt32Field(const char* FieldName, UInt32& Value) { return win->uInt32Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool uInt16Field(const char* FieldName, UInt16& Value) { return win->uInt16Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool uInt8Field(const char* FieldName, UInt8& Value) { return win->uInt8Field(FieldName, Value); }


	UCODE_EDITOR_FORCEINLINE bool Int64Field(const char* FieldName, SInt64& Value) { return win->Int64Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool Int32Field(const char* FieldName, SInt32& Value) { return win->Int32Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool Int16Field(const char* FieldName, SInt16& Value) { return win->Int16Field(FieldName, Value); }
	UCODE_EDITOR_FORCEINLINE bool Int8Field(const char* FieldName, SInt8& Value) {return win->Int8Field(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool CharField(const char* FieldName, char& Value) { return win->CharField(FieldName,Value); }
	UCODE_EDITOR_FORCEINLINE bool BoolField(const char* FieldName, bool& Value) { return win->BoolField(FieldName, Value); }

	UCODE_EDITOR_FORCEINLINE bool DrawLayerField(const char* FieldName, UInt8& Value) { return win->DrawLayerField(FieldName, Value); }
	private:
		InspectDrawer(InspectWindow* w) { win = w; }
		InspectWindow* win;
	};
	typedef void(*DrawData)(InspectDrawer& data);
	typedef void(*OnInspecChange)(void* _OldData);
	struct InspectData
	{
		
		AnyManagedPtr _Data;
		void* _Data2 = nullptr;
		DrawData _Draw;
		OnInspecChange _Change;
		InspectData(AnyManagedPtr Data, DrawData Draw, OnInspecChange Change =nullptr)
			: _Data(Data), _Draw(Draw), _Change(Change)
		{

		}
		InspectData(): _Data(), _Draw(nullptr), _Change(nullptr)
		{

		}
	};
	friend InspectDrawer;

	InspectWindow(const NewEditorWindowData& windowdata);
	~InspectWindow() override;
	void UpdateWindow() override;

	void Inspect(const InspectData& Data) { _Inspect = Data; _Updated = true; }
	static EditorWindowData GetEditorData();

	typedef UInt8 AsssetType_t;
	enum class AsssetType :AsssetType_t
	{
		Null,
		Texture,
		Sprite,
		Shader,
	};
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	InspectData _Inspect;
	InspectDrawer _InspectDrawer;
	bool _Updated = false;

	void DrawText(const char* text);
	
	bool Buttion(const char* FieldName);

	bool AsssetField(const char* FieldName,void*& Value,AsssetType Type);

	bool ToggleField(const char* FieldName, bool& Value);

	bool StringField(const char* FieldName, String&  Value);

	bool Vec2Field(const char* FieldName,UCode::Vec2& Value);
	bool Vec2Field(const char* FieldName,UCode::Vec2i& Value);
	bool Vec3Field(const char* FieldName,UCode::Vec3& Value);
	bool Vec3Field(const char* FieldName,UCode::Vec3i& Value);

	bool ColorField(const char* FieldName, UCode::ColorRGBA& Value);
	bool ColorField(const char* FieldName, UCode::ColorRGB& Value);
	bool ColorField(const char* FieldName, UCode::Color32& Value);
	bool ColorField(const char* FieldName, UCode::Color24& Value);
	
	bool uInt64Field(const char* FieldName, UInt64& Value);
	bool uInt32Field(const char* FieldName, UInt32& Value);
	bool uInt16Field(const char* FieldName, UInt16& Value);
	bool uInt8Field(const char* FieldName,  UInt8& Value);

	bool Int64Field(const char* FieldName, SInt64& Value);
	bool Int32Field(const char* FieldName, SInt32& Value);
	bool Int16Field(const char* FieldName, SInt16& Value);
	bool Int8Field(const char* FieldName, SInt8& Value);
	
	bool float32Field(const char* FieldName, float32& Value);
	bool float64Field(const char* FieldName, float64& Value);

	bool BoolField(const char* FieldName, bool& Value);
	bool CharField(const char* FieldName, char& Value);


	bool DrawLayerField(const char* FieldName, UInt8& Value);
};

EditorEnd 