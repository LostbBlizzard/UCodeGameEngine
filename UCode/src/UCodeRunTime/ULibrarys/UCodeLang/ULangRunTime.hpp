#pragma once
#include "LangNamespace.hpp"
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include <UCodeLang/UCodeLang/UCodeLang.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
CoreStart
#define UScript_Attribute "UScript"
#define UAsset_Attribute "UAsset"
#define NoClassScript  "[NoClass]"

struct MemData
{
	MemData(): _Mem(0)
	{

	}
	size_t _Mem;
};

class ULangScript;
class ULangRunTime  final :private System
{
	
	friend ULangScript;
public:
	static ULangRunTime* Get(Gamelibrary* e);
	static ULangRunTime* Find(const Gamelibrary* e);

	;
	using LibID = uintptr_t;
	struct Lib
	{
		Unique_ptr<UCodeLang::UClib> Ptr;
		Unique_ptr<UCodeLang::RunTimeLib> RunPtr;
		LibID _ID;
	};
	using ScriptInfoKey = uintptr_t;
	struct ScriptInfo
	{
		void** ObjPtr = nullptr;
		const UCodeLang::Class_Data** ScriptType;
		std::function<void(const UCodeLang::AssemblyNode* nod)> SetScriptType;
		std::function<void* ()> PreReload;
		std::function<void(void*)> PostReload;
	};

	bool HotReLoadScripts();
	void ReLoadScripts();

	UCodeGEForceinlne void* Lang_Malloc(size_t Size)
	{
		return (void*)_State.Malloc((UCodeLang::NSize_t)Size);
	}
	UCodeGEForceinlne void* Lang_Realloc(void* oldPtr,size_t Size)
	{
		return (void*)_State.Realloc((UCodeLang::PtrType)oldPtr,(UCodeLang::NSize_t)Size);
	}
	UCodeGEForceinlne void* Lang_Calloc(size_t Size)
	{
		return (void*)_Interpreter.Calloc((UCodeLang::NSize_t)Size);
	}
	UCodeGEForceinlne void Lang_Free(void* Ptr)
	{
		_State.Free((UCodeLang::PtrType)Ptr);
	}

	template<typename... Args> void LangCall(const UCodeLang::ClassMethod* Func, Args&&... parameters)
	{
		_Interpreter.Call(Func, parameters...);
	}

	template<typename... Args>void LangThisCall(const UCodeLang::ClassMethod* Func, void* This, Args&&... parameters)
	{
		_Interpreter.ThisCall(Func, This, parameters...);
	}

	template<typename T, typename... Args> T R_LangCall(const UCodeLang::ClassMethod* Func, Args&&... parameters)
	{
		return _Interpreter.RCall<T>(Func, parameters...);
	}
	template<typename T, typename... Args> T R_LangThisCall(const UCodeLang::ClassMethod* Func,void* This, Args&&... parameters)
	{
		return _Interpreter.RThisCall<T>(This,Func, parameters...);
	}


	UCodeGEForceinlne const UCodeLang::ClassAssembly& Get_Assembly()
	{
		return _State.Get_Assembly();
	}
	void OpenLibs(const Path& PathDir);

	UCodeGEForceinlne UCodeLang::Allocator& Get_Allocator()
	{
		return _State.Get_Allocator();
	}

	UCodeGEForceinlne auto& Get_State()
	{
		return _State;
	}
	LibID AddLib(Lib&& lib);
	void RemoveLib(LibID ID);
	Lib& GetLib(LibID ID);

	ScriptInfoKey AddScript(ScriptInfo&& val)
	{
		auto newkey = _ScriptNextID;
		_ScriptNextID++;

		_ScriptInfo.AddValue(newkey,std::move(val));
		return newkey;
	}
	void RemoveScript(ScriptInfoKey key)
	{
		_ScriptInfo.erase(key);
	}

	inline UCodeLang::Interpreter& GetCurrentInterpreter()
	{
		return _Interpreter;
	}
	inline static const char* MainFile = "bin/ucode.lib";
private:
	ULangRunTime(Gamelibrary* e);
	~ULangRunTime();

	void Add(ULangScript* Script);
	void Remove(ULangScript* Script);


	Vector<Lib> Libs;
	Vector<ULangScript*> _Scripts;
	UnorderedMap<ScriptInfoKey,ScriptInfo> _ScriptInfo;
	//
	
	UCodeLang::RunTimeLangState _State;
	UCodeLang::Interpreter _Interpreter;
	LibID _NextID = 0;
	ScriptInfoKey _ScriptNextID = 0;
	//
	MemData _MemData;
	static UComponentData type_Data;
};

class CoreModule;
class ULangScript :public Compoent
{
	friend CoreModule;
public:
	ULangScript(Entity* e);
	~ULangScript() override;
	
	void Update() override;
	void FixedUpdate() override;


	void UnLoad();
	void UnLoadUObject();
	void UnLoadClassInfo();
	void HotReLoad();
	void ReLoad(bool callAwake =true);

	void CallAwake();

	UCodeGEForceinlne ULangRunTime* GetULang() const
	{
		return ULangRunTime::Get(Get_Scene()->Get_RunTime()->Get_Library_Edit());
	}

	UCodeGEForceinlne String GetClassName() const
	{
		if (_ClassData)
		{
			return _ClassName;
		}
		else
		{
			return NoClassScript;
		}
	}
	UCodeGEForceinlne void SetClassName(const String& Name)
	{
		_ClassName = Name;
	}

	UCodeGEForceinlne bool HasScript()const
	{ 
		return _UObj;
	}
	UCodeGEForceinlne bool HasClass()const
	{
		return _ClassData;
	}
	UCodeGEForceinlne const UCodeLang::Class_Data* Get_ClassData() const
	{
		return  _ClassData;
	}
	UCodeGEForceinlne void* Get_UObj() const
	{
		return _UObj;
	}
	void LoadScript(const UCodeLang::AssemblyNode* ClassData,bool callAwake =true);

	void GetCallBinds();
	

	void Serialize(USerializer& Serializer)  const override;

	void Deserialize(UDeserializer& Serializer) override;

	static UComponentsID Get_TypeID();
private:
	static UComponentData type_Data;
	const UCodeLang::Class_Data* _ClassData = nullptr;
	String _ClassName;
	UCodeLang::PtrType _UObj =nullptr;

	bool _HasCalledStart = false;
	bool _HasCalledAwake = false;
	//Tep Method
	const UCodeLang::ClassMethod* _LangConstructor = nullptr;
	const UCodeLang::ClassMethod* _LangDestructor = nullptr;
	const UCodeLang::ClassMethod* _LangAwake = nullptr;
	const UCodeLang::ClassMethod* _LangStart = nullptr;
	const UCodeLang::ClassMethod* _LangUpdate =nullptr;
	const UCodeLang::ClassMethod* _LangFixedUpdate = nullptr;
};


struct UCodeRunTimeState
{
public:

	// returns whatever the T in Object<T> is
	static Optional<UCodeLang::ReflectionTypeInfo> IsTypeUCodeObject(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static Optional<UCodeLang::ReflectionTypeInfo> IsTypeUCodeObjectAndAsset(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsTypeAssetTrait(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	
	static bool IsTile(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsSprite(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsTexture(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsAudio(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsSceneAsset(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	static bool IsEntityAsset(UCodeLang::ReflectionTypeInfo Type,const UCodeLang::ClassAssembly& Assembly);
	
	template<typename... Args> static void LangCall(const UCodeLang::ClassMethod* Func, Args&&... parameters)
	{
		if (Has_Current()) 
		{
			Get_Current()->LangCall(Func, parameters...);
		}
	}
	template<typename... Args> static void LangThisCall(const UCodeLang::ClassMethod* Func, void* This, Args&&... parameters)
	{
		if (Has_Current())
		{
			Get_Current()->LangThisCall(Func,This, parameters...);
		}
	}
	template<typename T, typename... Args> static T R_LangCall(const UCodeLang::ClassMethod* Func, Args&&... parameters)
	{
		if (Has_Current())
		{
			return Get_Current()->R_LangCall<T>(Func, parameters...);
		}
		return {};
	}
	template<typename T, typename... Args> static T R_LangThisCall(const UCodeLang::ClassMethod* Func, void* This, Args&&... parameters)
	{
		if (Has_Current())
		{
			return Get_Current()->R_LangThisCall<T>(Func,This, parameters...);
		}
		return {};
	}

	static UCodeGEForceinlne void Set_Current(ULangRunTime* Value){Current = Value;}
	static UCodeGEForceinlne ULangRunTime* Get_Current() {
		UCodeGEAssert(Current);
		return Current;
	}
	static UCodeGEForceinlne bool Has_Current(){return  Get_Current();}
private:
	inline static thread_local ULangRunTime* Current = nullptr;
};


class ULangHelper
{
public:

	static void Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::Enum_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::Class_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Serialize(USerializer& Serializer, const void* Pointer, const UCodeLang::StaticArray_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::Class_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::Enum_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
	static void Deserialize(UDeserializer& Serializer, void* Pointer, const UCodeLang::StaticArray_Data& Type, const UCodeLang::ClassAssembly& Assembly, bool Is32Mode);
};

CoreEnd
