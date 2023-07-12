#include "ULangRunTime.hpp"
#include <filesystem>
#include "UCodeAPI_Lib.hpp"
LangStart
using namespace UCodeLang;
namespace fs = std::filesystem;
ULangRunTime* ULangRunTime::Get(Gamelibrary* e)
{
	const auto index = (size_t)StaticBooksIndex::ULangRunTime;
	auto item = e->Get_StaticComponent(index);
	if (item == nullptr)
	{
		auto r = new ULangRunTime(e);
		e->MoveBook(r);
		e->SetStaticComponent(index, r);
		return r;
	}
	return (ULangRunTime*)item;
}

ULangRunTime* ULangRunTime::Find(const Gamelibrary* e)
{
	const auto index = (size_t)StaticBooksIndex::ULangRunTime;
	auto item = e->Get_StaticComponent(index);
	return (ULangRunTime*)item;
}

void ULangRunTime::HotReLoadScripts()
{
}

void ULangRunTime::ReLoadScripts()
{
}

void ULangRunTime::OpenLibs(const Path& PathDir)
{
	Libs.clear();
	_State.ClearRunTimeState();


	for (const auto& dirEntry : fs::recursive_directory_iterator(PathDir))
	{
		if (!dirEntry.is_regular_file()) { continue; }

		auto Ext = dirEntry.path().extension();
		if (Ext != FileExt::LibWithDot
			&& Ext != FileExt::DllWithDot)
		{
			continue;
		}


		String FilePath = dirEntry.path().generic_u8string();
		auto NewPtr =std::make_unique<UClib>();
		if (UClib::FromFile(NewPtr.get(), FilePath))
		{
			auto NewRunTimeLib = std::make_unique<RunTimeLib>();
			NewRunTimeLib->Init(NewPtr.get());
			Libs.push_back({std::move(NewPtr),std::move(NewRunTimeLib) });
		}

	}
	_State.ClearRunTimeState();
	for (auto& Item : Libs)
	{
		_State.AddLib(Item.RunPtr.get());
	}
	_State.AddLib(UCodeAPI_Lib::Get());
	_State.LinkLibs();

	for (size_t i = 0; i < _Scripts.size(); i++)
	{
		auto Item = _Scripts[i];
		Item->ReLoad();
	}
}

ULangRunTime::ULangRunTime(Gamelibrary* e) :libraryBook(e), _State()
{
	_Interpreter.Init(&_State);
}

ULangRunTime::~ULangRunTime()
{
	for (size_t i = 0; i < _Scripts.size(); i++)
	{
		auto Item = _Scripts[i];
		Item->UnLoad();
	}
}

void ULangRunTime::Add(ULangScript* Script)
{
	_Scripts.push_back(Script);
}

void ULangRunTime::Remove(ULangScript* Script)
{
	for (auto it = _Scripts.begin(); it != _Scripts.end();)
	{

		if (*it == Script)
		{
			_Scripts.erase(it);
			return;
		}
		++it;
	}
}


ULangScript::ULangScript(Entity* e) :Compoent(e, &type_Data)
{
	auto RunTime = GetULang();
	RunTime->Add(this);
}

ULangScript::~ULangScript()
{
	UnLoad();
	auto RunTime = GetULang();
	RunTime->Remove(this);
}


UComponentsID ULangScript::Get_TypeID()
{
	return "ULangScript";
}
UComponentData ULangRunTime::type_Data = { "ULangRunTime",nullptr};
UComponentData ULangScript::type_Data = { ULangScript::Get_TypeID(),[](Entity* E) {return (Compoent*)new ULangScript(E); } };

void ULangScript::Update()
{
	if (_UObj)
	{
		auto Lang = GetULang();
		if (_HasCalledStart == false)
		{
			_HasCalledStart = true;
			if (_LangStart) {
				Lang->LangThisCall(*_LangStart, _UObj);
			}
		}
		else
		{
			if (_LangUpdate) 
			{
				Lang->LangThisCall(*_LangUpdate, _UObj);
			}
		}
	}
}

void ULangScript::FixedUpdate()
{
	if (_UObj && _LangFixedUpdate)
	{
		GetULang()->LangThisCall(*_LangFixedUpdate, _UObj);
	}
}
void ULangScript::UnLoad()
{
	if (_ClassData)
	{
		UnLoadClassInfo();
	}
	if (_UObj)
	{
		UnLoadUObject();
	}
}
void ULangScript::UnLoadUObject()
{
	auto ULang = GetULang();
	ULang->Lang_Free(_UObj);
	_UObj = nullptr;
	_HasCalledStart = false;
	_HasCalledAwake = false;
}
void ULangScript::UnLoadClassInfo()
{
	_ClassData = nullptr;
	_LangConstructor = nullptr;
	_LangDestructor = nullptr;
	_LangAwake = nullptr;
	_LangStart = nullptr;
	_LangUpdate = nullptr;
	_LangFixedUpdate = nullptr;
}
void ULangScript::HotReLoad()
{
	UnLoadClassInfo(); GetCallBinds();
}
void ULangScript::ReLoad(bool callAwake)
{
	UnLoad();
	auto ULang = GetULang();
	for (const auto& Item : ULang->Get_Assembly().Classes)
	{
		if (_ClassName == Item->FullName)
		{
			LoadScript(Item.get(),callAwake);
			break;
		}
	}
}
void ULangScript::CallAwake()
{
	if (_UObj && _HasCalledAwake == false && _LangAwake)
	{
		_HasCalledAwake = true;
		GetULang()->LangThisCall(*_LangAwake, _UObj);
	}
}
void ULangScript::LoadScript(const UCodeLang::AssemblyNode* ClassData, bool callAwake)
{
	auto ULang = GetULang();
	if (_UObj) { UnLoad(); }


	_ClassData = &ClassData->Get_ClassData();
	_ClassName = ClassData->FullName;
	GetCallBinds(); 
	size_t ObjSize = _ClassData->Size;

	_UObj = ULang->Lang_Malloc(ObjSize);
		
	if (_LangConstructor) 
	{
		ULang->LangThisCall(*_LangConstructor, _UObj);
	}
	if (callAwake) {CallAwake();}
}

void ULangScript::GetCallBinds()
{
	auto& Class = *_ClassData;
	_LangConstructor = Class.Get_ClassInit();
	_LangDestructor = Class.Get_ClassDestructor();
	_LangAwake = Class.Get_ClassMethod("Awake");
	_LangStart = Class.Get_ClassMethod("Start");
	_LangUpdate = Class.Get_ClassMethod("Update");
	_LangFixedUpdate = Class.Get_ClassMethod("FixedUpdate");
}

void ULangScript::Serialize(USerializer& Serializer) const
{
	Serializer.Write("_ScriptType", GetClassName());
	if (HasScript())
	{
		void* Ptr = Get_UObj();
		auto ClassType = Get_ClassData();

		bool Is32Mode = sizeof(void*) == 4;

		ULangHelper::Serialize(Serializer, Ptr, *ClassType, GetULang()->Get_Assembly(),Is32Mode);
	}
}

void ULangScript::Deserialize(UDeserializer& Serializer)
{
	String ScriptType = NoClassScript;
	Serializer.ReadType("_ScriptType", ScriptType, ScriptType);
	if (ScriptType != NoClassScript)
	{
		SetClassName(ScriptType);
		ReLoad(false);
		if (HasScript())
		{

			void* Ptr = Get_UObj();
			auto ClassType = Get_ClassData();

			bool Is32Mode = sizeof(void*) == 4;

			ULangHelper::Deserialize(Serializer, Ptr, *ClassType, GetULang()->Get_Assembly(), Is32Mode);
			CallAwake();
		}

	}
}

#define BadValueSerialize "n/a" 
#define EnumUnionTagFieldName "_Tag"
#define EnumUnionObjectFieldName "_Union"
void ULangHelper::Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::Enum_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{
	if (Type.EnumVariantUnion.has_value())
	{
		auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
		if (TagSize.has_value())
		{
			size_t Size = TagSize.value();
			if (Serializer.Get_Type() == USerializerType::Bytes)
			{
				auto& Bits = Serializer.Get_BitMaker();
				Bits.WriteBytes((const Byte*)Pointer, Size);//tag


				const EnumValues* Value = nullptr;

				for (auto& Item : Type.Values)
				{
					if (!memcmp(Item._Data.Get_Data(), Pointer, Size))
					{
						Value = &Item;
					}
				}

				if (Value && Value->EnumVariantType.has_value())
				{
					const void* UnionPointer = (const void*)((uintptr_t)Pointer + (uintptr_t)Size);
					Serialize(Serializer, UnionPointer, Value->EnumVariantType.value(), Assembly, Is32Mode);//Union
				}
			}
			else
			{
				auto& Bits = Serializer.Get_TextMaker();


				const EnumValues* Value = nullptr;

				for (auto& Item : Type.Values)
				{
					if (!memcmp(Item._Data.Get_Data(), Pointer, Size))
					{
						Value = &Item;
					}
				}

				if (Value) 
				{
					Bits << YAML::BeginMap;
					Bits << YAML::Key << EnumUnionTagFieldName << YAML::Value << Value->Name.c_str();


					Bits << YAML::Key << EnumUnionObjectFieldName;

					if (Value->EnumVariantType.has_value())
					{
						Serialize(Serializer, Pointer, Value->EnumVariantType.value(), Assembly, Is32Mode);
					}
					else
					{
						Bits << "";
					}

					Bits << YAML::EndMap;
				}
				else
				{
					Bits << YAML::BeginMap;
					Bits << YAML::Key << EnumUnionTagFieldName << YAML::Value << BadValueSerialize;
					Bits << YAML::Key << EnumUnionObjectFieldName << YAML::Value << "";
					Bits << YAML::EndMap;
				}
			}
		}
	}
	else
	{
		auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
		if (TagSize.has_value())
		{
			size_t Size = TagSize.value();
			const EnumValues* Value = nullptr;

			for (auto& Item : Type.Values)
			{
				if (!memcmp(Item._Data.Get_Data(), Pointer, Size))
				{
					Value = &Item;
				}
			}

			if (Value == nullptr && Type.Values.size())
			{
				Value = &Type.Values.front();
			}

			if (Value)
			{
				if (Serializer.Get_Type() == USerializerType::Bytes)
				{
					auto& Bits = Serializer.Get_BitMaker();
					Bits.WriteBytes((const Byte*)Value->_Data.Get_Data(),Size);
				}
				else
				{
					auto& Bits = Serializer.Get_TextMaker();
					Bits << YAML::Value << Value->Name;
				}
			}
			else
			{
				if (Serializer.Get_Type() == USerializerType::Bytes)
				{
					auto& Bits = Serializer.Get_BitMaker();

					for (size_t i = 0; i < Size; i++)
					{
						Byte V = 0;
						Bits.WriteBytes(&V,1);
					}
				}
				else
				{
					auto& Bits = Serializer.Get_TextMaker();
					Bits << YAML::Value << BadValueSerialize;
				}
			}
		}
	}
}

void ULangHelper::Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{
	if (Type.IsAddress() || Type.IsAddressArray()
		|| Type.IsDynamicTrait()
		|| Type.IsMovedType())
	{
		return;
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		auto& Val = *(Int8*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else 
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << UCode_YAML_CharFix(Val);
		}
	}	
	break;
	case ReflectionTypes::sInt16:
	{
		auto& Val = *(Int64*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	sInt32Case:
	case ReflectionTypes::sInt32:
	{
		auto& Val = *(Int32*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	sInt64Case:
	case ReflectionTypes::sInt64:
	{
		auto& Val = *(Int64*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	case ReflectionTypes::uInt8:
	{
		auto& Val = *(UInt8*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << UCode_YAML_UCharFix(Val);
		}
	}
	break;
	case ReflectionTypes::uInt16:
	{
		auto& Val = *(UInt16*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << UCode_YAML_UCharFix(Val);
		}
	}
		break;
		uInt32Case:
	case ReflectionTypes::uInt32:
	{
		auto& Val = *(UInt32*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
		break;
		uInt64Case:
	case ReflectionTypes::uInt64:
	{
		auto& Val = *(UInt32*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	case ReflectionTypes::Char:
	{
		auto& Val = *(char*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	case ReflectionTypes::Bool:
	{
		auto& Val = *(bool*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	case ReflectionTypes::float32:
	{
		auto& Val = *(float32*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;
	case ReflectionTypes::float64:
	{
		auto& Val = *(float64*)Pointer;
		if (Serializer.Get_Type() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitMaker();
			Bits.WriteType(Val);
		}
		else
		{
			auto& Bits = Serializer.Get_TextMaker();
			Bits << YAML::Value << Val;
		}
	}
	break;

	case ReflectionTypes::sIntPtr:
	{
		if (Is32Mode)
		{
			goto sInt32Case;
		}
		else
		{
			goto sInt64Case;
		}
	}
	break;
	case ReflectionTypes::uIntPtr:
	{
		if (Is32Mode)
		{
			goto uInt32Case;
		}
		else
		{

			goto uInt32Case;
		}
	}
	break;

	case ReflectionTypes::CustomType:
	{
		auto Node = Assembly.Find_Node(Type._CustomTypeID);
		if (Node)
		{
			switch (Node->Get_Type())
			{
			case ClassType::Alias:
			{
				auto& Alias = Node->Get_AliasData();
				Serialize(Serializer, Pointer, Alias.Type, Assembly,Is32Mode);
			}
			break;
			case ClassType::Class:
			{
				auto& Class = Node->Get_ClassData();

				if (Serializer.Get_Type() == USerializerType::YAML) 
				{
					Serializer.Get_TextMaker() << YAML::BeginMap;
				}

				Serialize(Serializer, Pointer, Class, Assembly, Is32Mode);

				if (Serializer.Get_Type() == USerializerType::YAML) 
				{
					Serializer.Get_TextMaker() << YAML::EndMap;
				}
			}
			break;
			case ClassType::Enum:
			{
				auto& Enum = Node->Get_EnumData();
				Serialize(Serializer, Pointer, Enum, Assembly, Is32Mode);
			}
			break;
			default:
				break;
			}
		}
	}
	break;
	default:
		break;
	}
}

void ULangHelper::Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::Class_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{

	for (const auto& Item : Type.Fields)
	{
		uintptr_t FieldPtr = (uintptr_t)Pointer + (uintptr_t)Item.offset;
		const void* FieldP = (void*)FieldPtr;

		if (Item.Type.IsAddress() || Item.Type.IsAddressArray()
			|| Item.Type.IsDynamicTrait()
			|| Item.Type.IsMovedType())
		{
			continue;
		}

		if (Serializer.Get_Type() == USerializerType::YAML)
		{
			Serializer.Get_TextMaker() << YAML::Key << Item.Name;
		}
		Serialize(Serializer, FieldP, Item.Type, Assembly, Is32Mode);

	}
}
void ULangHelper::Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{
	if (Type.IsAddress() || Type.IsAddressArray()
		|| Type.IsDynamicTrait()
		|| Type.IsMovedType())
	{
		return;
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();
			
			auto& FPtr = *(SInt8*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{	
			auto& Bits = Serializer.Get_TextReader();
			
			auto& FPtr = *(SInt8*)Pointer;
			FPtr = Bits.as<SInt8>(FPtr);
		}
	}
	break;
	case ReflectionTypes::sInt16:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(SInt16*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(SInt16*)Pointer;
			FPtr = Bits.as<SInt16>(FPtr);
		}
	}
	break;
	sInt32Case:
	case ReflectionTypes::sInt32:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(SInt32*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(SInt32*)Pointer;
			FPtr = Bits.as<SInt32>(FPtr);
		}
	}
	break;
sInt64Case:
	case ReflectionTypes::sInt64:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(SInt64*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(SInt64*)Pointer;
			FPtr = Bits.as<SInt64>(FPtr);
		}
	}
	break;
	case ReflectionTypes::uInt8:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(UInt8*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(UInt8*)Pointer;
			FPtr = Bits.as<UInt8>(FPtr);
		}
	}
	break;
	case ReflectionTypes::uInt16:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(UInt16*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(UInt16*)Pointer;
			FPtr = Bits.as<UInt16>(FPtr);
		}
	}
	break;
	uInt32Case:
	case ReflectionTypes::uInt32:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(UInt32*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(UInt32*)Pointer;
			FPtr = Bits.as<UInt32>(FPtr);
		}
	}
	break;
uInt64Case:
	case ReflectionTypes::uInt64:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(UInt64*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(UInt64*)Pointer;
			FPtr = Bits.as<UInt64>(FPtr);
		}
	}
	break;
	case ReflectionTypes::Bool:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(bool*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(bool*)Pointer;
			FPtr = Bits.as<bool>(FPtr);
		}
	}
	break;
	case ReflectionTypes::Char:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(char*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(char*)Pointer;
			FPtr = Bits.as<char>(FPtr);
		}
	}
	break;
	case ReflectionTypes::float32:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(float32*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(float32*)Pointer;
			FPtr = Bits.as<float32>(FPtr);
		}
	}
	break;
	case ReflectionTypes::float64:
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto& FPtr = *(float64*)Pointer;
			Bits.ReadType(FPtr, FPtr);
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			auto& FPtr = *(float64*)Pointer;
			FPtr = Bits.as<float64>(FPtr);
		}
	}
	break;

	case ReflectionTypes::uIntPtr:
	{
		if (Is32Mode)
		{
			goto uInt32Case;
		}
		else
		{
			goto uInt64Case;

		}
	}
	break;
	case ReflectionTypes::sIntPtr:
	{
		if (Is32Mode)
		{
			goto sInt32Case;
		}
		else
		{
			goto sInt64Case;

		}
	}
	break;
	case ReflectionTypes::CustomType:
	{
		auto Node = Assembly.Find_Node(Type._CustomTypeID);
		if (Node)
		{
			switch (Node->Get_Type())
			{
			case ClassType::Alias:
			{
				auto& Alias = Node->Get_AliasData();
				Deserialize(Serializer, Pointer, Alias.Type, Assembly, Is32Mode);
			}
			break;
			case ClassType::Class:
			{
				auto& Class = Node->Get_ClassData();
				Deserialize(Serializer, Pointer, Class, Assembly, Is32Mode);
			}
			break;
			case ClassType::Enum:
			{
				auto& Enum = Node->Get_EnumData();
				Deserialize(Serializer, Pointer, Enum, Assembly, Is32Mode);
			}
			break;
			default:
				break;
			}
		}
	}
	break;
	default:
		break;
	}
}
void ULangHelper::Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::Class_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{
	for (const auto& Item : Type.Fields)
	{
		uintptr_t FieldPtr = (uintptr_t)Pointer + (uintptr_t)Item.offset;
		void* FieldP = (void*)FieldPtr;

		if (Item.Type.IsAddress() || Item.Type.IsAddressArray()
			|| Item.Type.IsDynamicTrait()
			|| Item.Type.IsMovedType())
		{
			continue;
		}

		if (Serializer.Get_Mode() == USerializerType::YAML)
		{
			auto oldYaml =std::move(Serializer.Get_TextReader());

			Serializer.Get_TextReader() = oldYaml[Item.Name];
			

			Deserialize(Serializer, FieldP, Item.Type, Assembly, Is32Mode);

			Serializer.Get_TextReader() = std::move(oldYaml);
		}
		else
		{
			Deserialize(Serializer, FieldP, Item.Type, Assembly, Is32Mode);
		}
	}
}

void ULangHelper::Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::Enum_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode)
{
	if (Type.EnumVariantUnion.has_value())
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto& Bits = Serializer.Get_BitReader();

			auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
			if (TagSize.has_value())
			{
				size_t Size = TagSize.value();
				void* UnionPointer =(void*)((uintptr_t)Pointer + (uintptr_t)Size);


				{//tag
					const void* FallBack;
					Unique_Array<Byte> Bytes;
					if (Type.Values.size())
					{
						FallBack = Type.Values.front()._Data.Get_Data();
					}
					else
					{
						constexpr size_t BigObject = 64;
						if (Size < BigObject)
						{
							Bytes.reset(new Byte[Size]);
							FallBack = Bytes.get();
						}
						else
						{
							FallBack = alloca(Size);
							new ((void*)FallBack) Byte[Size];
						}


					}
					Bits.ReadBytes(Pointer, FallBack, Size);
				}

				{//union

					const EnumValues* Value = nullptr;
					for (auto& Item : Type.Values)
					{
						if (!memcmp(Pointer,Item._Data.Get_Data(),Size))
						{
							Value = &Item;
						}
					}

					if (Value && Value->EnumVariantType.has_value())
					{
						Deserialize(Serializer, UnionPointer, Value->EnumVariantType.value(), Assembly, Is32Mode);
					}
				}
			}
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();
		
			auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
			if (TagSize.has_value())
			{
				size_t Size = TagSize.value();
				auto EnumKey = Bits[EnumUnionTagFieldName].as<String>(BadValueSerialize);

				const EnumValues* Value = nullptr;

				for (auto& Item : Type.Values)
				{
					if (Item.Name == EnumKey)
					{
						Value = &Item;
					}
				}

				if (Value && Value->EnumVariantType.has_value())
				{
					void* UnionPointer = (void*)((uintptr_t)Pointer + (uintptr_t)Size);
					{
						auto oldYaml = std::move(Serializer.Get_TextReader());

						Serializer.Get_TextReader() = oldYaml[EnumUnionTagFieldName];


						Deserialize(Serializer, UnionPointer,Value->EnumVariantType.value(), Assembly, Is32Mode);

						Serializer.Get_TextReader() = std::move(oldYaml);
					}
				}
			}

		}
	}
	else
	{
		if (Serializer.Get_Mode() == USerializerType::Bytes)
		{
			auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
			if (TagSize.has_value())
			{
				size_t Size = TagSize.value();
				auto& Bits = Serializer.Get_BitReader();

			
				const void* FallBack;
				Unique_Array<Byte> Bytes;
				if (Type.Values.size())
				{
					FallBack = Type.Values.front()._Data.Get_Data();
				}
				else
				{
					constexpr size_t BigObject = 64;
					if (Size < BigObject)
					{
						Bytes.reset(new Byte[Size]);
						FallBack = Bytes.get();
					}
					else
					{
						FallBack = alloca(Size);
						new ((void*)FallBack) Byte[Size];
					}


				}

				const EnumValues* Value = nullptr;

				

				Bits.ReadBytes(Pointer, FallBack, Size);
			}
		}
		else
		{
			auto& Bits = Serializer.Get_TextReader();

			String StringV = BadValueSerialize;
			StringV = Bits.as<String>(StringV);


			const EnumValues* Value = nullptr;

			for (auto& Item : Type.Values)
			{
				if (Item.Name == StringV)
				{
					Value = &Item;
				}
			}

			if (StringV == BadValueSerialize && Type.Values.size())
			{
				Value = &Type.Values.front();
			}

			auto TagSize = Assembly.GetSize(Type.BaseType, Is32Mode);
			if (TagSize.has_value())
			{
				size_t Size = TagSize.value();
				if (Value)
				{
					memcpy(Pointer, Value->_Data.Get_Data(), Size);
				}
				else
				{
					memset(Pointer, 0, Size);
				}
			}
		}
	}
}

LangEnd

