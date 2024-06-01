#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "../LangNamespace.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
LangStart 


UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Object<T> export:
      private:
        byte[/16] padding;
        |GetHandle[imut this&]:
          uintptr _this = unsafe bitcast<uintptr>(this);
          ret _this;
      public:
        
        |new[this&]: ObjectAPI::New(GetHandle());
        |new[this&,imut this& other]: ObjectAPI::New_Copy(GetHandle(),other.GetHandle());
        |new[this&,moved this other]: ObjectAPI::New_Move(GetHandle(),other.GetHandle());
        |drop[this&]: ObjectAPI::Drop(GetHandle());

        |Get[this&] -> T&:
          eval uintptr typehint = type(T).TypeID();
          uintptr pointer = ObjectAPI::Get(GetHandle(),typehint);
          if pointer == 0:
            //panic("Object is a Null Reference");
          ret unsafe bitcast<T&>(pointer);
        
        |Get[imut this&] -> T&:
          eval uintptr typehint = type(T).TypeID();
          uintptr pointer = ObjectAPI::Get(GetHandle(),typehint);
          if pointer == 0:
            //panic("Object is a Null Reference");
          ret unsafe bitcast<T&>(pointer);

        |~>[this&] -> T&:ret Get();
        |~>[imut this&] -> imut T&:ret Get();
    ")");

UCodeLangExportSymbol("UCodeGameEngine") struct ObjectAPI
{
    using Base = ScencPtr;
    static Base& Cast(uintptr_t Handle)
    {
        return *(Base*)Handle;
    }
    static const Base& iCast(uintptr_t Handle)
    {
        return *(const Base*)Handle;
    }
    UCodeLangExport static void New(uintptr_t Handle);
    UCodeLangExport static void New_Copy(uintptr_t Handle,uintptr_t Source);
    UCodeLangExport static void New_Move(uintptr_t Handle,uintptr_t Source);
    UCodeLangExport static void Drop(uintptr_t Handle);

    UCodeLangExport static uintptr_t Get(uintptr_t Handle, uintptr_t typehint);
    
};

LangEnd
