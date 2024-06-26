#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/GpuTypes/Sprite.hpp>
#include "AppFiles.hpp"
#include "ImGui.hpp"
#include <Typedefs.hpp>
#include <UCodeRunTime/TypeName.hpp>
EditorStart
class ImGuIHelper
{
public:
	static ImVec4 ToVec4(const Color& val)
	{
		ImVec4 r;
		r.x = val.R;
		r.y = val.G;
		r.z = val.B;
		r.w = val.A;
		return r;
	}
	static ImVec4 ToVec4(const ColorRGB& val)
	{
		ImVec4 r;
		r.x = val.R;
		r.y = val.G;
		r.z = val.B;
		r.w = 1;
		return r;
	}
	static ImU32 ColorToImguiU32(const Color32& val)
	{
		return ImGui::ColorConvertFloat4ToU32(ToVec4((Color)val));
	}
	static ImU32 ColorToImguiU32(const Color24& val)
	{
		return ImGui::ColorConvertFloat4ToU32(ToVec4((ColorRGB)val));
	}
	static bool BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);


	static void Text(const StringView text)
	{
		ImGui::TextUnformatted(text.data(), text.data() + text.size());
	}
	static void Text(const String& text)
	{
		return Text((StringView)text);
	}
	static const char* ToCStr(const StringView text);
	static const char* ToCStr(const String& text);

	static void ImageDontKeepAspectRatio(UCode::Texture& Sprite, const ImVec2& ButtionSize);
	static void ImageDontKeepAspectRatio(UCode::Sprite& Sprite, const ImVec2& ButtionSize);
	static bool ImageButtonDontKeepAspectRatio(const void* id, UCode::Texture* Sprite, const ImVec2& ButtionSize);
	static bool ImageButtonDontKeepAspectRatio(const void* id, UCode::Sprite* Sprite, const ImVec2& ButtionSize);

	struct TreeNodeText
	{
		bool NodeOpen, TextUpdate;
	};
	static void Image(UCode::Texture* texture, const ImVec2& ButtionSize);
	static void Image(UCode::Sprite* Sprite, const ImVec2& ButtionSize);
	UCODE_EDITOR_FORCEINLINE static void Image(AppFiles::sprite Sprite, const ImVec2& ButtionSize)
	{
		Image(AppFiles::GetSprite(Sprite), ButtionSize);
	}

	static bool ImageButton(const void* id, UCode::Texture* Sprite, const ImVec2& ButtionSize);
	static bool ImageButton(const void* id, UCode::Sprite* Sprite, const ImVec2& ButtionSize);
	static void GetSpriteUV(UCode::Sprite* Sprite, ImVec2& uvo, ImVec2& uv1);
	UCODE_EDITOR_FORCEINLINE static bool ImageButton(const void* id, AppFiles::sprite Sprite, const ImVec2& ButtionSize)
	{
		return ImageButton(id, AppFiles::GetSprite(Sprite), ButtionSize);
	}
	
	enum ItemLabelFlag
	{
		Left = 1u << 0u,
		Right = 1u << 1u,
		Default = Left,
	};
	static ImVec2 CheckBoxSize();
	static ImVec2 CheckBoxSizeWithPadding();
	static void ItemLabel(StringView title, ItemLabelFlag flags);
	static bool InputText(const char* label, String& buffer, ImGuiInputTextFlags flags = 0);
	static bool InputText(String& buffer, ImGuiInputTextFlags flags = 0);
	static bool MultLineText(const char* label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags = 0);

	static bool TreeNode(const void* id, const char* label, UCode::Sprite* Sprite,ImGuiTreeNodeFlags Flags = 0);

	static bool InputText(const StringView label, String& buffer, ImGuiInputTextFlags flags = 0);
	static bool MultLineText(const StringView label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags = 0);

	static bool TreeNode(const void* id, const StringView label, UCode::Sprite* Sprite,ImGuiTreeNodeFlags Flags = 0);


	enum class InputPathType
	{
		Any,
		File,
		Dir,
	};
	static bool InputPath(const StringView label, String& buffer, ImGuiInputTextFlags flags = 0, InputPathType PathType = InputPathType::Any, Span<String> Extension = {});
	static bool InputPath(const StringView label, Path& buffer, ImGuiInputTextFlags flags = 0, InputPathType PathType = InputPathType::Any, Span<String> Extension = {});


	static bool InputPath(const char* label, String& buffer, ImGuiInputTextFlags flags = 0, InputPathType PathType = InputPathType::Any, Span<String> Extension = {});
	static bool InputPath(const char* label, Path& buffer, ImGuiInputTextFlags flags = 0, InputPathType PathType = InputPathType::Any, Span<String> Extension = {});

	UCODE_EDITOR_FORCEINLINE static bool TreeNode(const void* id, const char* label, AppFiles::sprite Sprite, ImGuiTreeNodeFlags Flags = 0)
	{
		return  TreeNode(id, label, AppFiles::GetSprite(Sprite),Flags);
	}

	static TreeNodeText TreeNode(const void* id, String& buffer);
	static TreeNodeText TreeNode(const void* id, String& buffer, UCode::Sprite* Sprite);
	UCODE_EDITOR_FORCEINLINE static TreeNodeText  TreeNode(const void* id, String& buffer, AppFiles::sprite Sprite)
	{
		return  TreeNode(id, buffer, AppFiles::GetSprite(Sprite));
	}

	static bool TreeNode(const void* id, StringView label, AppFiles::sprite Sprite);

	static bool ToggleField(const char* FieldName, bool& Value);

	static bool Vec2Field(const char* FieldName, UCode::Vec2& Value);
	static bool Vec2Field(const char* FieldName, UCode::Vec2i& Value);
	static bool Vec3Field(const char* FieldName, UCode::Vec3& Value);
	static bool Vec3Field(const char* FieldName, UCode::Vec3i& Value);
	static bool ColorField(const char* FieldName, UCode::ColorRGBA& Value);
	static bool ColorField(const char* FieldName, UCode::ColorRGB& Value);
	static bool ColorField(const char* FieldName, UCode::Color32& Value);
	static bool ColorField(const char* FieldName, UCode::Color24& Value);

	static bool uInt64Field(const char* FieldName, u64& Value);
	static bool uInt32Field(const char* FieldName, u32& Value);
	static bool uInt16Field(const char* FieldName, u16& Value);
	static bool uInt8Field(const char* FieldName, u8& Value);

	static bool CharField(const char* FieldName, char& Value);

	static bool Int64Field(const char* FieldName, i64& Value);
	static bool Int32Field(const char* FieldName, i32& Value);
	static bool Int16Field(const char* FieldName, i16& Value);
	static bool Int8Field(const char* FieldName, i8& Value);

	static bool f32Field(const char* FieldName, f32& Value);
	static bool f64Field(const char* FieldName, f64& Value);

	static bool DrawOrdeField(const char* FieldName, unsigned char& Value);


	static bool ToggleField(StringView FieldName, bool& Value)
	{
		return ToggleField(ToCStr(FieldName), Value);
	}

	static bool Vec2Field(StringView FieldName, UCode::Vec2& Value)
	{
		return Vec2Field(FieldName, Value);
	}
	static bool Vec2Field(StringView FieldName, UCode::Vec2i& Value)
	{
		return Vec2Field(FieldName, Value);
	}
	static bool Vec3Field(StringView FieldName, UCode::Vec3& Value)
	{
		return Vec3Field(FieldName, Value);
	}
	static bool Vec3Field(StringView FieldName, UCode::Vec3i& Value)
	{
		return Vec3Field(FieldName, Value);
	}
	static bool ColorField(StringView FieldName, UCode::ColorRGBA& Value)
	{
		return ColorField(ToCStr(FieldName), Value);
	}
	static bool ColorField(StringView FieldName, UCode::ColorRGB& Value)
	{
		return ColorField(ToCStr(FieldName), Value);
	}
	static bool ColorField(StringView FieldName, UCode::Color32& Value)
	{
		return ColorField(ToCStr(FieldName), Value);
	}
	static bool ColorField(StringView FieldName, UCode::Color24& Value)
	{
		return ColorField(ToCStr(FieldName), Value);
	}

	static bool uInt64Field(StringView FieldName, u64& Value)
	{
		return uInt64Field(ToCStr(FieldName), Value);
	}
	static bool uInt32Field(StringView FieldName, u32& Value)
	{
		return uInt32Field(ToCStr(FieldName), Value);
	}
	static bool uInt16Field(StringView FieldName, u16& Value)
	{
		return uInt16Field(ToCStr(FieldName), Value);
	}
	static bool uInt8Field(StringView FieldName, u8& Value)
	{
		return uInt8Field(ToCStr(FieldName), Value);
	}

	static bool CharField(StringView FieldName, char& Value)
	{
		return CharField(ToCStr(FieldName), Value);
	}
	static bool Int64Field(StringView FieldName, i64& Value)
	{
		return Int64Field(ToCStr(FieldName), Value);
	}
	static bool Int32Field(StringView FieldName, i32& Value)
	{
		return Int32Field(ToCStr(FieldName), Value);
	}

	static bool Int16Field(StringView FieldName, i16& Value)
	{
		return Int16Field(ToCStr(FieldName), Value);
	}

	static bool Int8Field(StringView FieldName, i8& Value)
	{
		return Int8Field(ToCStr(FieldName), Value);
	}

	static bool f32Field(StringView FieldName, f32& Value)
	{
		return f32Field(ToCStr(FieldName), Value);
	}

	static bool f64Field(StringView FieldName, f64& Value)
	{
		return f64Field(ToCStr(FieldName), Value);
	}

	static bool DrawOrdeField(StringView FieldName, unsigned char& Value)
	{
		return DrawOrdeField(ToCStr(FieldName), Value);
	}


	template<typename T>
	struct EnumValue
	{
		const char* label = "null";
		T Value = T();
		EnumValue(const char* Label, T value)
		{
			label = Label;
			Value = value;
		}
	};
	static UCODE_EDITOR_FORCEINLINE bool BoolEnumField(const char* label, bool& Value)
	{
		return EnumField<bool>(label, Value, BoolEnumValues, sizeof(BoolEnumValues) / sizeof(BoolEnumValues[1]));
	}

	static UCODE_EDITOR_FORCEINLINE bool BoolEnumField(const StringView label, bool& Value)
	{
		return BoolEnumField(ToCStr(label), Value);
	}

	struct EnumValue2
	{
		const char* label = "null";
		const void* Value = nullptr;
	};

	static bool EnumField(void* Value, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize);

	static bool EnumField(void* Value, const Vector<EnumValue2>& Values, size_t EnumBaseSize)
	{
		return EnumField(Value, Values.data(), Values.size(), EnumBaseSize);
	}

	static bool EnumField(const char* label, void* Value, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize);

	static bool EnumField(const char* label, void* Value, const Vector<EnumValue2>& Values, size_t EnumBaseSize);

	template<typename T>
	static UCODE_EDITOR_FORCEINLINE bool EnumField(const char* label, T& Value, const Vector<EnumValue<T>>& Values)
	{
		return EnumField(label, Value, Values.data(), Values.size());
	}

	template<typename T>
	static bool EnumField(const char* label, T& Value, const EnumValue<T>* Values, size_t Size)
	{
		constexpr size_t MaxStackSize = sizeof(EnumValue2) * 8;
		bool IsEnumValuesBig = (Size * sizeof(EnumValue2)) > MaxStackSize;


		EnumValue2* Ptr = nullptr;
		if (IsEnumValuesBig)
		{
			Ptr = new EnumValue2[Size];
		}
		else
		{
			Ptr = new (alloca(Size * sizeof(EnumValue2))) EnumValue2[Size];
		}

		for (size_t i = 0; i < Size; i++)
		{
			EnumValue2& Enum2Value = Ptr[i];
			const EnumValue<T>& EnumValue = Values[i];

			Enum2Value.label = EnumValue.label;
			Enum2Value.Value = &EnumValue.Value;
		}

		bool V = EnumField(label, &Value, Ptr, Size, sizeof(T));

		if (IsEnumValuesBig)
		{
			delete[] Ptr;
		}
		return V;
	}

	struct EnumVariantFieldUpdate
	{
		bool EnumUpdated = false;
		bool VariantUpdated = false;
	};
	struct VariantInfo
	{
		void* Tag = nullptr;
		void* Union = nullptr;
	};
	static EnumVariantFieldUpdate EnumVariantField(const char* label, VariantInfo Variant, std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> DrawVariant, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize);
	static EnumVariantFieldUpdate EnumVariantField(VariantInfo Variant, std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> DrawVariant, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize);


	template<typename... Types>
	struct Variant_Members
	{
		using Type = Variant<Types...>;

		typename Type::TagType _Tag;
		typename Type::Pack _Objects;
	};

	template <typename... Types2>
	struct EnumVariant_helper;


	template <typename T2, typename... Types2>
	struct EnumVariant_helper<T2, Types2...>
	{

		UCodeGEForceinlne static const char* GetName(const UCode::VariantTagType type_index)
		{
			if (type_index == sizeof...(Types2))
			{
				return UCode::GetTypeName<T2>();
			}
			else
			{
				return EnumVariant_helper<Types2...>::GetName(type_index);
			}
		}
		UCodeGEForceinlne static bool DrawItem(const UCode::VariantTagType type_index, void* Object)
		{
			if (type_index == sizeof...(Types2))
			{
				return DrawValue<T2>((T2*)Object);
			}
			else
			{
				return EnumVariant_helper<Types2...>::DrawItem(type_index, Object);
			}
		}
	};


	template <typename... Types2>
	struct EnumVariant_helper
	{
		UCODE_EDITOR_FORCEINLINE static const char* GetName(const UCode::VariantTagType type_index) { return {}; }
		UCODE_EDITOR_FORCEINLINE static bool DrawItem(const UCode::VariantTagType type_index, void* Object) { return false; }
	};



	template<typename... T>
	static EnumVariantFieldUpdate EnumVariantField(const char* label, Variant<T...>& Value)
	{
		using Type = Variant<T...>;
		using Type_Members = Variant_Members<T...>;
		using variant_helper = typename Type::helper_type;
		using EnumVariant_type = EnumVariant_helper<T...>;
		static bool Init = false;

		constexpr size_t TypesCount = Type::ItemsInListCount;
		static EnumValue2 EnumValues[TypesCount];
		static typename Type::TagType RawValue[TypesCount];





		if (Init == false)
		{
			Init = true;



			for (size_t i = 0; i < TypesCount; i++)
			{
				auto& Item = EnumValues[i];
				auto& RawItem = RawValue[i];

				RawItem = i;
				Item.label = EnumVariant_type::GetName(i);
				Item.Value = &RawItem;
			}
		}

		Type_Members& ValueAsMembers = *(Type_Members*)&Value;

		VariantInfo V;
		V.Tag = &ValueAsMembers._Tag;
		V.Union = &ValueAsMembers._Objects;

		auto Func = [](void* Tag, void* Union, bool UpdatedEnum, bool Draw)
			{
				bool Updated = false;


				UCode::VariantTagType* TagValue = (UCode::VariantTagType*)Tag;

				if (UpdatedEnum)
				{
					variant_helper::construct(*TagValue, Union);
				}

				if (Draw) {
					Updated = EnumVariant_type::DrawItem(*TagValue, Union);
				}
				return Updated;
			};

		auto R = EnumVariantField(label, V, Func, EnumValues, Type::ItemsInListCount, sizeof(typename Type::TagType));


		return R;

	}

	static constexpr float Default_LoadingIndicatorSpeed = 5;
	static constexpr float Default_LoadingIndicatorcircle_count = 8;
	//Made From Alexander Scheid-Rehder on Dec 6, 2018
	//https://github.com/ocornut/imgui/issues/1901
	static void LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count = Default_LoadingIndicatorcircle_count, const float speed = Default_LoadingIndicatorSpeed);
	static void LoadingIndicatorCircle(const char* label, const float indicator_radius, const int circle_count = Default_LoadingIndicatorcircle_count, const float speed = Default_LoadingIndicatorSpeed);
	static void ShowOnMiddleLoadingIndicatorCircle(const char* label, const int circle_count = Default_LoadingIndicatorcircle_count, const float speed = Default_LoadingIndicatorSpeed);
	static bool DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenameing);
	static bool DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenameing, UCode::Sprite* sprite);
	static bool DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenamein, AppFiles::sprite sprite)
	{
		return DrawRenameTree(label, TreeIsOpened, IsRenamein, AppFiles::GetSprite(sprite));
	}
	static bool DrawRenameName(String& label, bool& IsRenameing);
	static bool BufferingBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);

	struct DrawVectorInfo
	{
		size_t ItemSize = 0;

		std::function<void(void* Object, size_t Index)> _OnDrawItem;
		Optional<std::function<void(void* Object, size_t Index)>> _AddNewValue;
		Optional<std::function<void(void* Object, size_t Index)>> _AddNewRemove;
		Optional<std::function<void(void* Object, size_t NewSize)>> _ResizeVector;
	};

	static bool DrawVector(const char* label, void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item)
	{
		ItemLabel(StringView(label), ItemLabelFlag::Left);
		return DrawVector(Object, Buffer, Size, Item);
	}
	static bool DrawVector(void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item);

	static bool InputSize_t(const char* label, size_t* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);

	static bool InputSize_t(const StringView label, size_t* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0)
	{
		return InputSize_t(label, v, step, step_fast, flags);
	}
	template<typename T>
	static bool DrawVector(const char* label, Vector<T>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				auto& Item = Objectbuf[Index];

				DrawValue<T>(&Item);
			};

		Info._AddNewValue = [](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.insert(Objectbuf.begin() + Index, T());
			};

		Info._AddNewRemove = [](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.erase(Objectbuf.begin() + Index);
			};

		Info._ResizeVector = [](void* Object, size_t NewIndex)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.resize(NewIndex);
			};

		return DrawVector(label, &Buffer, Buffer.data(), Buffer.size(), Info);
	}

	template<typename T>
	static bool DrawVector(const char* label, Vector<T>& Buffer, std::function<bool(T& Item)> onDraw)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [onDraw = std::move(onDraw)](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				auto& Item = Objectbuf[Index];

				onDraw(Item);
			};

		Info._AddNewValue = [](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.insert(Objectbuf.begin() + Index, T());
			};

		Info._AddNewRemove = [](void* Object, size_t Index)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.erase(Objectbuf.begin() + Index);
			};

		Info._ResizeVector = [](void* Object, size_t NewIndex)
			{
				UCodeEditor::Vector<T>& Objectbuf = *(UCodeEditor::Vector<T>*)Object;
				Objectbuf.resize(NewIndex);
			};

		return DrawVector(label, &Buffer, Buffer.data(), Buffer.size(), Info);
	}

	template<typename T>
	static bool DrawSpan(const char* label, Span<T>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
			{
				UCodeEditor::Span<T>& Objectbuf = *(UCodeEditor::Span<T>*)Object;
				auto& Item = Objectbuf[Index];

				DrawValue<T>(&Item);
			};


		return DrawVector(label, &Buffer, Buffer.Data(), Buffer.Size(), Info);
	}

	template<typename T, size_t Size>
	static bool DrawArray(const char* label, Array<T, Size>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
			{
				Array<T, Size>& Objectbuf = *(Array<T, Size>*)Object;
				auto& Item = Objectbuf[Index];

				DrawValue<T>(&Item);
			};


		return DrawVector(label, &Buffer, Buffer.data(), Buffer.size(), Info);
	}


	struct DrawUniquePtrInfo
	{
		std::function<void(void* Object)> _OnDrawItem;

		Optional<std::function<void(void* Object)>> _FreeItem;
		Optional<std::function<void(void* Object)>> _AllocateItem;
	};


	template<typename T>
	static bool Draw_UniquePtr(const char* label, Unique_ptr<T>& Item)
	{
		DrawUniquePtrInfo Info;
		Info._OnDrawItem = [](void* Object)
			{
				Unique_ptr<T>& ObjectAsType = *(Unique_ptr<T>*)Object;
				DrawValue<T>(ObjectAsType.get());
			};
		Info._FreeItem = [](void* Object)
			{
				Unique_ptr<T>& ObjectAsType = *(Unique_ptr<T>*)Object;
				ObjectAsType = Unique_ptr<T>();
			};
		Info._AllocateItem = [](void* Object)
			{
				Unique_ptr<T>& ObjectAsType = *(Unique_ptr<T>*)Object;
				ObjectAsType = Unique_ptr<T>(new T());
			};
		return Draw_UniquePtr(label, (void*)&Item, Info);

	}

	static bool Draw_UniquePtr(const char* label, void* Uniqueptr, const DrawUniquePtrInfo& Info);


	struct DrawOptionalInfo
	{
		std::function<void(void* Object)> _OnDrawItem;
		std::function<bool(void* Object)> _HasValue;

		std::function<void(void* Object)> _SetValue;
		std::function<void(void* Object)> _FreeValue;
	};

	static bool Draw_Optional(const char* label, void* Optionalptr, const DrawOptionalInfo& Info);

	template<typename T>
	struct OptionalMembers
	{
		T Value;
		bool HasValue;
	};

	template<typename T>
	static bool Draw_Optional(const char* label, Optional<T>& Item)
	{

		{
			constexpr bool AreSame = sizeof(Optional<T>) == sizeof(OptionalMembers<T>);
			static_assert(AreSame, "Optional and OptionalMembers are not the same");
		}

		DrawOptionalInfo Info;
		Info._OnDrawItem = [](void* Object)
			{
				Optional<T>& ObjectAsType = *(Optional<T>*)Object;
				OptionalMembers<T>& AsMembers = *(OptionalMembers<T>*) & ObjectAsType;

				DrawValue<T>(&AsMembers.Value);
			};

		Info._HasValue = [](void* Object)
			{
				Optional<T>& ObjectAsType = *(Optional<T>*)Object;

				bool Value = ObjectAsType.has_value();

#ifdef DEBUG
				OptionalMembers<T>& AsMembers = *(OptionalMembers<T>*) & ObjectAsType;
				if (Value != AsMembers.Value)
				{
					UCodeGEFatal("Member Miss-Match");


				}
#endif // DEBUG

				return Value;
			};

		Info._SetValue = [](void* Object)
			{
				Optional<T>& ObjectAsType = *(Optional<T>*)Object;
				OptionalMembers<T>& AsMembers = *(OptionalMembers<T>*) & ObjectAsType;
				AsMembers.HasValue = true;
			};

		Info._FreeValue = [](void* Object)
			{
				Optional<T>& ObjectAsType = *(Optional<T>*)Object;
				OptionalMembers<T>& AsMembers = *(OptionalMembers<T>*) & ObjectAsType;
				AsMembers.HasValue = false;
			};

		return Draw_Optional(label, (void*)&Item, Info);
	}

	static bool Draw_StringView(const char* label, StringView& Item);


	struct DrawDictionaryInfo
	{
		std::function<void(void* Object, const DrawDictionaryInfo& Info)> _LoopOverObjects;

		std::function<void(void* Object, void* Item, const DrawDictionaryInfo& Info)> _OnDrawItem;
		std::function<void(void* Object, void* Item)> _OnRemoveItem;


		std::function<void(void* Object, void* Item)> _OnDrawTepItem;
		std::function<void* (void* Object)> _OnMakeTepItem;
		std::function<void(void* TepItem)> _OnFreeTepItem;
		std::function<void(void* Object, void* Item)> _AddTepItemTODictionary;
	};

	static bool Draw_Dictionary(const char* label, void* Dictionary, size_t DictionarySize, const DrawDictionaryInfo& Info);


	template<typename Key, typename U>
	static bool Draw_Dictionary(const char* label, Unordered_map<Key, U>& Item)
	{
		DrawDictionaryInfo Info;
		Info._LoopOverObjects = [](void* Object, const DrawDictionaryInfo& Info)
			{
				Unordered_map<Key, U>& ObjectAsType = *(UCodeEditor::Unordered_map<Key, U>*)Object;
				size_t OldSize = ObjectAsType.size();
				for (auto& Item : ObjectAsType)
				{
					Info._OnDrawItem(&ObjectAsType, &Item, Info);

					if (OldSize != ObjectAsType.size())
					{
						break;
					}
				}
			};

		Info._OnDrawItem = [](void* Map, void* Object, const DrawDictionaryInfo& Info)
			{
				std::pair<const Key, U>& ObjectAsType = *(std::pair<const Key, U>*)Object;

				ImGui::BeginDisabled(true);

				DrawValue<Key>((Key*)&ObjectAsType.first);

				ImGui::EndDisabled();

				ImGui::SameLine();

				DrawValue<U>(&ObjectAsType.second);

				bool RemoveItem = false;
				ImGui::PushID(Object);
				if (ImGuIHelper::BeginPopupContextItem("????"))
				{
					if (ImGui::MenuItem("Remove Item"))
					{
						RemoveItem = true;
					}
					ImGui::EndPopup();
				}
				ImGui::PopID();

				if (RemoveItem)
				{
					Info._OnRemoveItem(Map, Object);
				}
			};

		Info._OnRemoveItem = [](void* Object, void* Item)
			{
				Unordered_map<Key, U>& ObjectAsType = *(UCodeEditor::Unordered_map<Key, U>*)Object;
				std::pair<const Key, U>& ObjectToRemove = *(std::pair<const Key, U>*)Item;

				ObjectAsType.erase(ObjectToRemove.first);
			};

		Info._OnDrawTepItem = [](void* Map, void* Object)
			{
				std::pair<Key, U>& ObjectAsType = *(std::pair<Key, U>*)Object;

				DrawValue<Key>(&ObjectAsType.first);

				ImGui::SameLine();

				DrawValue<U>(&ObjectAsType.second);
			};

		Info._OnMakeTepItem = [](void* Object)
			{
				return new std::pair<Key, U>();
			};
		Info._OnFreeTepItem = [](void* Object)
			{
				std::pair<Key, U>* ObjectAsType = (std::pair<Key, U>*)Object;
				delete ObjectAsType;
			};
		Info._AddTepItemTODictionary = [](void* Object, void* Map)
			{
				Unordered_map<Key, U>& MapAsType = *(UCodeEditor::Unordered_map<Key, U>*)Map;
				std::pair<Key, U>* ObjectAsType = (std::pair<Key, U>*)Object;
				MapAsType.AddValue(ObjectAsType->first, ObjectAsType->second);
			};

		return Draw_Dictionary(label, &Item, Item.size(), Info);
	}



	template<typename T> static bool DrawValueWithLabel(const char* label, T* Item)
	{
		auto r = DrawValue<T>(Item);
		ImGui::SameLine();
		ImGuIHelper::Text((StringView)label);
		return r;
	}
	template<typename T> static bool DrawValue(T* Item)
	{
		return false;
		//static_assert(false,"funcion has no body");
	}

	static bool DrawValue(bool* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::BoolEnumField("", *Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(char* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::CharField("", *Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(i64* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S64, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(u64* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U64, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(i32* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S32, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(u32* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U32, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(i16* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S16, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(u16* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U16, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(i8* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S8, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(u8* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U8, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(float* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_Float, Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(double* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_Float, Item);
		ImGui::PopID();
		return R;
	}
	//OtherTypes
	static bool DrawValue(Vec2* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::Vec2Field("", *Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(Vec2i* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::Vec2Field("", *Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(Vec3* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::Vec3Field("", *Item);
		ImGui::PopID();
		return R;
	}
	static bool DrawValue(Vec3i* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::Vec3Field("", *Item);
		ImGui::PopID();
		return R;
	}

	static bool DrawValue(String* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGuIHelper::InputText("", *Item);
		ImGui::PopID();
		return R;
	}
	template<typename T> static bool DrawValue(Vector<T>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawVector<T>("", *Item);
		ImGui::PopID();
		return R;
	}

	template<typename T, size_t Size> static bool DrawValue(Array<T, Size>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawArray("", *Item);
		ImGui::PopID();
		return R;
	}
	template<typename T> static bool DrawValue(Span<T>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawSpan<T>("", *Item);
		ImGui::PopID();
		return R;
	}
	template<typename T> static bool DrawValue(Unique_ptr<T>* Item)
	{
		ImGui::PushID(Item);
		auto R = Draw_UniquePtr<T>("", *Item);
		ImGui::PopID();
		return R;
	}

	template<typename... T> static bool DrawValue(Variant<T...>* Item)
	{
		ImGui::PushID(Item);
		auto R = EnumVariantField("", *Item);
		ImGui::PopID();
		return R;
	}

	template<typename Key, typename Value> static bool DrawValue(Unordered_map<Key, Value>* Item)
	{
		ImGui::PushID(Item);
		auto R = Draw_Dictionary<Key, Value>("", *Item);
		ImGui::PopID();
		return R;
	}

	static bool DrawValue(StringView* Item)
	{
		ImGui::PushID(Item);
		auto R = Draw_StringView("", *Item);
		ImGui::PopID();
		return R;
	}

	enum class ObjectDropState 
	{
		CanBeDroped,
		OnDroped,
	};
	using ObjectDroped = std::function<bool(const Path& fullpath,void* object,ObjectDropState State)>;
	using ObjectFieldFuncPtr = std::function<bool(void* Ptr, void* Object, bool Listmode, const String& Find)>;
	using ObjectFieldInspect = std::function<bool(void* Object)>;
	using ObjectFieldCopy = std::function<String(void* Object)>;
	using ObjectFieldPatse = std::function<bool(void* Object,const String& Paste)>;
	using ObjectDestroy = std::function<void(void* Object)>;

	struct DrawMenuObjectReturn
	{
		String* Search = nullptr;
		bool ListMode = false;
	};
	static DrawMenuObjectReturn DrawStarOfMenuObjects();
	struct ObjectFieldData
	{
		ObjectFieldFuncPtr OnObjectInList;
		Optional<ObjectDroped> OnFileDroped;
		Optional<ObjectFieldInspect> OnInspect;
		Optional<ObjectFieldCopy> OnCopy;
		Optional<ObjectFieldPatse> OnPatse;
		Optional<ObjectDestroy> OnDestory;
	};

	static bool DrawObjectField(UCode::Sprite* Sprite, void* object,
		const void* ObjectList, size_t ObjectListSize, size_t ItemObjectSize,
		const ObjectFieldData& DrawObject, const String& Name = "Put/Name/Here");

	static bool DrawObjectField(UCode::Sprite* Sprite, const char* Label, void* object,
		const void* ObjectList, size_t ObjectListSize, size_t ItemObjectSize,
		const ObjectFieldData& DrawObject, const String& Name = "Put/Name/Here")
	{
		ItemLabel(StringView(Label),ItemLabelFlag::Left);	
		return DrawObjectField(Sprite, object, ObjectList, ObjectListSize, ItemObjectSize, DrawObject, Name);
	}

	static bool DrawObjectField(const char* Label, void* object,
		const void* ObjectList, size_t ObjectListSize, size_t ItemObjectSize,
		const ObjectFieldData& DrawObject, AppFiles::sprite Sprite = AppFiles::sprite::AppIcon, const String& Name = "Put/Name/Here")
	{
		return DrawObjectField(AppFiles::GetSprite(Sprite), Label, object, ObjectList, ObjectListSize, ItemObjectSize, DrawObject, Name);
	}
	static bool DrawObjectField(void* object,
		const void* ObjectList, size_t ObjectListSize, size_t ItemObjectSize,
		const ObjectFieldData& DrawObject, AppFiles::sprite Sprite = AppFiles::sprite::AppIcon, const String& Name = "Put/Name/Here")
	{
		return DrawObjectField(AppFiles::GetSprite(Sprite), object, ObjectList, ObjectListSize, ItemObjectSize, DrawObject, Name);
	}
private:
	inline static const EnumValue<bool> BoolEnumValues[] =
	{
		EnumValue<bool>("false",false),
		EnumValue<bool>("true",true)
	};
};
EditorEnd