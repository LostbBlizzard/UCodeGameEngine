#include "Object.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"
LangStart

void ObjectAPI::New(uintptr_t Handle)
{
    UCodeGEDebugStackFrame("ObjectAPI::New")
    new (&Cast(Handle)) Base();
}
void ObjectAPI::New_Copy(uintptr_t Handle, uintptr_t Source)
{
    UCodeGEDebugStackFrame("ObjectAPI::New_Copy")
    new (&Cast(Handle)) Base(Cast(Source));
}
void ObjectAPI::New_Move(uintptr_t Handle, uintptr_t Source)
{
    UCodeGEDebugStackFrame("ObjectAPI::New_Move")
    new (&Cast(Handle)) Base(std::move(Cast(Source)));
}
void ObjectAPI::Drop(uintptr_t Handle)
{
    UCodeGEDebugStackFrame("ObjectAPI::Drop")
    Cast(Handle).~AssetPtr();
}
UCodeLangExport uintptr_t ObjectAPI::Get(uintptr_t Handle, uintptr_t typehint)
{
    UCodeGEDebugStackFrame("ObjectAPI::Get")
    auto Self = Cast(Handle);

    if (Self.Has_UID() && !Self.Has_Asset())
    {
          //Load Asset Were
    }

    bool IsValue = Self.Has_Asset();
    bool IsCorrectType = true;

    #if UCodeGEDebugMode
    {

    }
    #endif

    bool allowGet = IsValue && IsCorrectType;

    if (allowGet) 
    {
        return (uintptr_t)Self.Get_Asset();
    }
    else
    {
        return uintptr_t(0);
    }
}
LangEnd
