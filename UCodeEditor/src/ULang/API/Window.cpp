#include "Window.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
UCodeAPIEditorStart

const char* GetCString(const StringView Val)
{
    thread_local std::string str;

    str = Val;

    return str.c_str();
}

bool WindowDrawAPI::Field(const StringView FieldName, uint8_t& Value)
{
    return ImGuIHelper::uInt8Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, uint16_t& Value)
{
    return ImGuIHelper::uInt16Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, uint32_t& Value)
{
    return ImGuIHelper::uInt32Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, uint64_t& Value)
{
    return ImGuIHelper::uInt64Field(GetCString(FieldName), Value);
}

bool WindowDrawAPI::Field(const StringView FieldName, int8_t& Value)
{
    return ImGuIHelper::Int8Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, int16_t& Value)
{
    return ImGuIHelper::Int16Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, int32_t& Value)
{
    return ImGuIHelper::Int32Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, int64_t& Value)
{
    return ImGuIHelper::Int64Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Fieldptr(const StringView FieldName, uintptr_t& Value)
{
    return ImGuIHelper::InputSize_t(GetCString(FieldName),&Value);
}
bool WindowDrawAPI::Fieldptr(const StringView FieldName, intptr_t& Value)
{
    if (sizeof(void*) == 4)
    {
        return ImGuIHelper::Int32Field(GetCString(FieldName), *(i32*)&Value);
    }
    else
    {
        return ImGuIHelper::Int64Field(GetCString(FieldName), *(i64*)&Value);
    }
}
bool WindowDrawAPI::Field(const StringView FieldName, bool& Value)
{
    return ImGuIHelper::BoolEnumField(GetCString(FieldName), Value);
}

bool WindowDrawAPI::Field(const StringView FieldName, float& Value)
{
    return ImGuIHelper::f32Field(GetCString(FieldName), Value);
}
bool WindowDrawAPI::Field(const StringView FieldName, double& Value)
{
    return ImGuIHelper::f64Field(GetCString(FieldName), Value);
}

bool WindowDrawAPI::Field(const StringView FieldName, void* Value, UCodeLang::ReflectionCustomTypeID id)
{
    return false;
}

bool WindowDrawAPI::FieldRawView(const StringView FieldName, void* Value, UCodeLang::ReflectionCustomTypeID id)
{
    return false;
}



bool WindowDrawAPI::iField(const StringView FieldName,const uint8_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<uint8_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const uint16_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<uint16_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const uint32_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<uint32_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const uint64_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<uint64_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const int8_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<int8_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const int16_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<int16_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const int32_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<int32_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName,const int64_t& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<int64_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iFieldptr(const StringView FieldName,const uintptr_t& Value)
{
    BeginReadOnly();
    auto r = Fieldptr(FieldName, const_cast<uintptr_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iFieldptr(const StringView FieldName,const intptr_t& Value)
{
    BeginReadOnly();
    auto r = Fieldptr(FieldName, const_cast<intptr_t&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName, const bool& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<bool&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName, const float& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<float&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName, const double& Value)
{
    BeginReadOnly();
    auto r = Field(FieldName, const_cast<double&>(Value));
    EndReadOnly();
    return r;
}

bool WindowDrawAPI::iField(const StringView FieldName, const void* Value, UCodeLang::ReflectionCustomTypeID id)
{
    return false;
}

bool WindowDrawAPI::iFieldRawView(const StringView FieldName, const void* Value, UCodeLang::ReflectionCustomTypeID id)
{
    return false;
}


void WindowDrawAPI::BeginReadOnly()
{
    ImGui::BeginDisabled();
}

void WindowDrawAPI::EndReadOnly()
{
    ImGui::EndDisabled();
}

UCodeAPIEditorEnd