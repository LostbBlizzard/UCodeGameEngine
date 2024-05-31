#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "../LangNamespace.hpp"
#include "UCodeRunTime/Core/Entity.hpp"
LangStart 

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Component export trait:
	  uintptr _Handle = 0;
	  |entity[this&]  => ComponentAPI::entity(_Handle);
	  |entity[imut this&] => ComponentAPI::ientity(_Handle);
    ")");

UCodeLangExportSymbol("UCodeGameEngine") struct ComponentAPI
{
	static Compoent& Cast(uintptr_t _Handle)
	{
		return *(Compoent*)_Handle;
	}
	static const Compoent& iCast(uintptr_t _Handle)
	{
		return *(const Compoent*)_Handle;
	}

	UCodeLangExport static UCode::Entity& entity(uintptr_t _Handle)
	{
		return *Cast(_Handle).NativeEntity();
	}
	UCodeLangExport static const UCode::Entity& ientity(uintptr_t _Handle)
	{
		return *iCast(_Handle).NativeEntity();
	}
};
UCodeLangExportSymbol("UCodeGameEngine") struct Entity
{
	UCode::Entity* Cast()
	{
		return (UCode::Entity*)this;
	}
	const UCode::Entity* Cast() const
	{
		return (const UCode::Entity*)this;
	}

	UCodeLangExport const StringView Name() const { return Cast()->NativeName(); }
	UCodeLangExport void Name(StringView Value) { Cast()->NativeName() = Value; }

	UCodeLangExport void Destroy()
	{
		UCode::Entity::Destroy(Cast());
	}
	UCodeLangExport void Enable() { Cast()->Enable(); }
	UCodeLangExport void Disable() { Cast()->Disable(); }
	UCodeLangExport bool Active() const
	{
		return Cast()->Active();
	}
	UCodeLangExport void Active(bool value) 
	{
		Cast()->SetActive(value);
	}
	

	UCodeLangExport Vec3& LocalPosition()
	{
		return  Cast()->LocalPosition();
	}
	UCodeLangExport void LocalPosition(const Vec3& value)
	{
		Cast()->LocalPosition(value);
	}

	UCodeLangExport Vec2 LocalPosition2D() const
	{
		return Cast()->LocalPosition2D();
	}
	UCodeLangExport void LocalPosition2D(const Vec2& value)
	{
		Cast()->LocalPosition2D(value);
	}

	UCodeLangExport Vec3 LocalRotation() const
	{
		return Cast()->LocalRotation();
	}
	UCodeLangExport void LocalRotation(const Vec3& value)
	{
		return Cast()->LocalRotation(value);
	}

	UCodeLangExport Vec2 LocalRotation2D() const
	{
		return Cast()->LocalRotation2D();
	}
	UCodeLangExport void LocalRotation2D(const Vec2& value) 
	{
		Cast()->LocalPosition2D(value);
	}	

	UCodeLangExport void LocalScale(const Vec3& value)
	{
		Cast()->LocalScale(value);
	}
	UCodeLangExport Vec3 LocalScale() const
	{
		return	Cast()->LocalScale();
	}

	UCodeLangExport void LocalScale2D(const Vec2 value)
	{
		Cast()->LocalScale2D(value);
	}
	UCodeLangExport Vec2 LocalScale2D() const
	{
		return Cast()->LocalScale2D();
	}



	inline UCodeLangExport Vec3 WorldPosition() const
	{
		return Cast()->WorldPosition();
	}
	inline UCodeLangExport Vec2 WorldPosition2D() const
	{
		return Cast()->WorldPosition2D();
	}

	inline  UCodeLangExport Vec3 WorldRotation() const
	{
		return Cast()->WorldRotation();
	}
	inline  UCodeLangExport Vec2 WorldRotation2D() const
	{
		return Cast()->WorldRotation2D();
	}

	inline  UCodeLangExport Vec3 WorldScale() const
	{
		return Cast()->WorldScale();
	}
	inline  UCodeLangExport Vec2 WorldScale2D() const
	{
		return Cast()->WorldScale2D();
	}

	inline  UCodeLangExport void WorldPosition(const Vec3& Value)
	{
		Cast()->WorldPosition(Value);
	}
	inline  UCodeLangExport void WorldPosition2D(const Vec2& Value)
	{
		Cast()->WorldPosition2D(Value);
	}

	inline UCodeLangExport void WorldRotation(const Vec3& Value)
	{
		Cast()->WorldRotation(Value);
	}
	inline UCodeLangExport void WorldRotation2D(const Vec2& Value)
	{
		Cast()->WorldRotation(Value);
	}

	inline UCodeLangExport void WorldScale(const Vec3& Value)
	{
		Cast()->WorldScale(Value);
	}
	inline UCodeLangExport void WorldScale2D(const Vec2& Value)
	{
		Cast()->WorldScale2D(Value);
	}

};

LangEnd
