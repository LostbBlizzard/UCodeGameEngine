#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UDefs.hpp"

#include <inttypes.h>

#include <string>
#include <string_view>
#include <array>
#include <optional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include <cmath>
#include <filesystem>

#include "BasicTypes/Variant.hpp"
//#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
CoreStart



using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

//For Clarity reasons
using Byte = u8;

#if __cplusplus >= 202002L
using utf8 = char8_t;
#else
using utf8 = unsigned char;
#endif // DEBUG

using utf16 = char16_t;
using utf32 = char32_t;

template<typename T, size_t Size>
using Array = std::array<T, Size>;

struct Color32;

struct Color
{
	f32 R, G, B, A;
	constexpr Color() :R(1), G(1), B(1), A(1) {}
	constexpr Color(f32 r, f32 g, f32 b) : R(r), G(g), B(b), A(1) {}
	constexpr Color(f32 r, f32 g, f32 b, f32 a) : R(r), G(g), B(b), A(a) {}
	constexpr static Color White() { return Color(1, 1, 1, 1); }
	constexpr bool operator==(const Color other)const
	{
		return (R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr bool operator!=(const Color other)const
	{
		return !(R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr Color operator*(const Color& other)const
	{
		return Color(R * other.R, G * other.G, B * other.B, A * other.A);
	}

	constexpr explicit operator Color32() const;
};

struct Color32
{
	Byte R, G, B, A;
	constexpr Color32() :R(255), G(255), B(255), A(255) {}
	constexpr Color32(Byte r, Byte g, Byte b) : R(r), G(g), B(b), A(255) {}
	constexpr Color32(Byte r, Byte g, Byte b, Byte a) : R(r), G(g), B(b), A(a) {}

	constexpr bool operator==(const Color32 other)const
	{
		return (R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr bool operator!=(const Color32 other)const
	{
		return !(R == other.R && G == other.G && B == other.B && A == other.A);
	}

	constexpr explicit operator Color() const
	{
		return	{
			((f32)R / (f32)255),
			((f32)G / (f32)255),
			((f32)B / (f32)255),
			((f32)A / (f32)255) };
	}

};

constexpr Color::operator Color32() const
{
	return	{
			(Byte)((i32)(R * 255)),
			(Byte)((i32)(G * 255)),
			(Byte)((i32)(B * 255)),
			(Byte)((i32)(A * 255))
	};
}

struct Color24;
struct ColorRGB
{
	f32 R, G, B;
	constexpr ColorRGB() :R(1), G(1), B(1) {}
	constexpr ColorRGB(f32 r, f32 g, f32 b) : R(r), G(g), B(b) {}
	constexpr static ColorRGB White() { return ColorRGB(1, 1, 1); }
	constexpr bool operator==(const ColorRGB other)const
	{
		return (R == other.R && G == other.G && B == other.B);
	}
	constexpr bool operator!=(const ColorRGB other)const
	{
		return !(R == other.R && G == other.G && B == other.B);
	}
	constexpr Color operator*(const ColorRGB& other)const
	{
		return Color(R * other.R, G * other.G, B * other.B);
	}

	constexpr explicit operator Color24() const;
};

struct Color24
{
	Byte R, G, B;
	constexpr Color24() :R(255), G(255), B(255) {}
	constexpr Color24(Byte r, Byte g, Byte b) : R(r), G(g), B(b) {}

	constexpr bool operator==(const Color32 other)const
	{
		return (R == other.R && G == other.G && B == other.B);
	}
	constexpr bool operator!=(const Color32 other)const
	{
		return !(R == other.R && G == other.G && B == other.B);
	}

	constexpr explicit operator ColorRGB() const
	{
		return	{
			((f32)R / (f32)255),
			((f32)G / (f32)255),
			((f32)B / (f32)255),
		};
	}

};
constexpr ColorRGB::operator Color24() const
{
	return	{
			(Byte)((i32)(R * 255)),
			(Byte)((i32)(G * 255)),
			(Byte)((i32)(B * 255))
	};
}
using ColorRGBA= Color;



template<typename T>
using Optional = std::optional<T>;

template<typename Key, typename T>
using UnorderedMap = std::unordered_map<Key, T>;

template<typename T>
using Vector = std::vector<T>;

using String =std::string;
using StringView = std::string_view;

using String8 = std::basic_string<utf8>;
using String8View = std::basic_string_view<utf8>;

using String16 = std::basic_string<utf16>;
using String16View = std::basic_string_view<utf16>;

using String32 = std::basic_string<utf32>;
using String32View = std::basic_string_view<utf32>;

using Path = std::filesystem::path;
using PathChar = Path::value_type;
using PathString = std::filesystem::path::string_type;
using PathSpan = std::basic_string_view<PathChar>;


using PathString = std::filesystem::path::string_type;


template<typename T>
struct Vec2_t
{
public:
	T X, Y;
	constexpr Vec2_t() :X(0), Y(0) {};
	constexpr Vec2_t(T x, T y) : X(x), Y(y) { };

	constexpr Vec2_t operator+(const Vec2_t& other)const
	{
		return Vec2_t(X + other.X, Y + other.Y);
	}
	constexpr Vec2_t operator-(const Vec2_t& other)const
	{
		return Vec2_t(X - other.X, Y - other.Y);
	}
	constexpr Vec2_t operator/(const T other)const
	{
		return Vec2_t(X / other, Y / other);
	}
	constexpr Vec2_t operator*(const T other)const
	{
		return Vec2_t(X * other, Y * other);
	}
	constexpr void operator+=(const Vec2_t& other)
	{
		X += other.X;
		Y += other.Y;
	}
	constexpr void operator-=(const Vec2_t& other)
	{
		X -= other.X;
		Y -= other.Y;
	}
	constexpr bool operator==(const Vec2_t& other)const
	{
		return (X == other.X && Y == other.Y);
	}
	constexpr bool operator!=(const Vec2_t& other)const
	{
		return !(X == other.X && Y == other.Y);
	}

	Vec2_t GetRotated(T radians) const
	{
		return { X * std::cos(radians) - Y * std::sin(radians),
				 X * std::sin(radians) + Y * std::cos(radians) };
	}


	T Get_Magnitude()
	{
		return sqrtf(X * X + Y * Y);
	}
	static T Distance(const Vec2_t& a, const Vec2_t& b)
	{
		return (a - b).Get_Magnitude();
	}
	constexpr static Vec2_t Lerp(const Vec2_t& a, const Vec2_t& b, f32 t)
	{
		return a + (b - a) * t;
	}

	Vec2_t Get_Normalize()
	{
		T m = Get_Magnitude();
		if (m == 0)
		{
			return { X,Y };
		}
		else
		{
			return { X / m,Y / m };
		}
	}
	constexpr static Vec2_t One() { return Vec2(1, 1); }
	constexpr static Vec2_t Zero() { return Vec2(0, 0); }



	/*
	constexpr inline explicit operator Vec2Int() const
	{
		return Vec2Int((int)X, (int)Y);
	}
	*/
};

template<typename T>
struct Vec3_t
{
	T X, Y, Z;
	using Vec2 = Vec2_t<T>;
	constexpr Vec3_t() :X(0), Y(0), Z(0) {}
	constexpr Vec3_t(T x, T y, T z) : X(x), Y(y), Z(z) {}
	constexpr Vec3_t(T x, T y) : X(x), Y(y), Z(0) {}
	constexpr Vec3_t(const Vec2& vector) : X(vector.X), Y(vector.Y), Z(0) {}

	constexpr static Vec3_t One() { return Vec3_t(1, 1, 1); }
	constexpr static Vec3_t Zero() { return Vec3_t(0, 0, 0); }

	constexpr Vec3_t operator+(const Vec3_t& other)const
	{
		return Vec3(X + other.X, Y + other.Y, Z + other.Z);
	}
	constexpr Vec3_t operator-(const Vec3_t& other)const
	{
		return Vec3_t(X - other.X, Y - other.Y, Z - other.Z);
	}
	constexpr Vec3_t operator/(T other)const
	{
		return Vec3_t(X / other, Y / other, Z / other);
	}
	constexpr Vec3_t operator*(T other)const
	{
		return Vec3_t(X * other, Y * other, Z * other);
	}
	constexpr void operator+=(const Vec3_t& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
	}
	constexpr void operator-=(const Vec3_t& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
	}
	constexpr bool operator==(const Vec3_t other)const
	{
		return (X == other.X && Y == other.Y && Z == other.Z);
	}
	constexpr bool operator!=(const Vec3_t other)const
	{
		return !(X == other.X && Y == other.Y && Z == other.Z);
	}

	T Get_Magnitude()
	{
		return sqrtf(X * X + Y * Y + Z + Z);
	}
	Vec3_t Normalize()
	{
		T m = Get_Magnitude();
		if (m == 0)
		{
			return { X,Y };
		}
		else
		{
			return { X / m,Y / m };
		}
	}
	static T Distance(const Vec3_t& a, const Vec3_t& b)
	{
		return (a - b).Get_Magnitude();
	}
	constexpr static Vec3_t Lerp(const Vec3_t& a, const Vec3_t& b, T t)
	{
		return a + (b - a) * t;
	}
};

template<typename T>
struct Vec2i_t
{
	T X, Y;

	using ftype = f32;
	using Vec2f = Vec2_t<ftype>;
	constexpr Vec2i_t() :X(0), Y(0) {}
	constexpr Vec2i_t(T x, T y) : X(x), Y(y) {}

	constexpr Vec2i_t operator+(const Vec2i_t& other) const
	{
		return Vec2i_t(X + other.X, Y + other.Y);
	}
	constexpr Vec2i_t operator-(const Vec2i_t& other) const
	{
		return Vec2i_t(X - other.X, Y - other.Y);
	}
	constexpr Vec2i_t operator/(T other) const
	{
		return Vec2i_t(X / other, Y / other);
	}
	constexpr Vec2i_t operator*(T other) const
	{
		return Vec2i_t(X * other, Y * other);
	}


	constexpr void operator+=(const Vec2i_t& other)
	{
		X += other.X;
		Y += other.Y;
	}
	constexpr void operator-=(const Vec2i_t& other)
	{
		X -= other.X;
		Y -= other.Y;
	}
	constexpr bool operator==(const Vec2i_t& other)const
	{
		return (X == other.X && Y == other.Y);
	}
	constexpr bool operator!=(const Vec2i_t& other)const
	{
		return !(X == other.X && Y == other.Y);
	}

	ftype Get_Magnitude()
	{
		ftype _x = (ftype)X;
		ftype _Y = (ftype)Y;
		return sqrtf(_x * _x + _Y * _Y);
	}
	static T Distance(const Vec2i_t& a, const Vec2i_t& b)
	{
		return (a - b).Get_Magnitude();
	}


	
	constexpr Vec2f AsVec2() const
	{
		return Vec2f((ftype)X, (ftype)Y);
	}





	constexpr static Vec2i_t One() { return Vec2i_t(1, 1); }
	constexpr static Vec2i_t Zero() { return Vec2i_t(0, 0); }
};

template<typename T>
struct Vec3i_t
{
	T X, Y, Z;

	using ftype = f32;
	using Vec3f = Vec3_t<ftype>;

	constexpr Vec3i_t() : X(0), Y(0), Z(0) {}
	constexpr Vec3i_t(T x, T y, T z) : X(x), Y(y), Z(z) {}
	constexpr Vec3i_t operator+(const Vec3i_t& other) const
	{
		return Vec3i_t(X + other.X, Y + other.Y, Z + other.Z);
	}
	constexpr Vec3i_t operator-(const Vec3i_t& other) const
	{
		return Vec3i_t(X - other.X, Y - other.Y, Z - other.Z);
	}
	constexpr Vec3i_t operator/(T other) const
	{
		return Vec3i_t(X / other, Y / other, Z / other);
	}
	constexpr Vec3i_t operator*(T other) const
	{
		return Vec3i_t(X * other, Y * other, Z * other);
	}
	constexpr void operator+=(const Vec3i_t& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
	}
	constexpr void operator-=(const Vec3i_t& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
	}

	constexpr bool operator==(const Vec3i_t& other)const
	{
		return (X == other.X && Y == other.Y && Z == other.Z);
	}
	constexpr bool operator!=(const Vec3i_t& other)const
	{
		return !(X == other.X && Y == other.Y && Z == other.Z);
	}

	constexpr void ToVec3() const
	{
		return Vec3f((ftype)X, (ftype)Y, (ftype)Z);
	}

	constexpr static Vec3i_t One() { return Vec3i_t(1, 1, 1); }
	constexpr static Vec3i_t Zero() { return Vec3i_t(0, 0, 0); }

	ftype Get_Magnitude()
	{
		ftype _x = (ftype)X;
		ftype _Y = (ftype)Y;
		ftype _z = (ftype)Z;

		return sqrtf(_x * _x + _Y * _Y + _z + _z);
	}
	static ftype Distance(const Vec3i_t& a, const Vec3i_t& b)
	{
		return (a - b).Get_Magnitude();
	}

};


using Vec2 = Vec2_t<f32>;
using Vec3 = Vec3_t<f32>;
using Vec2i = Vec2i_t<i32>;
using Vec3i =Vec3i_t<i32>;

template<typename T>
struct Span
{
	constexpr Span() :_Data(nullptr), _Size(0) {}
	constexpr Span(T* data, size_t Size) : _Data(data), _Size(Size) {}

	constexpr UCodeGameEngineForceinlne T& operator[](size_t Index)
	{
#if UCodeGameEngineDEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
#endif // DEBUG

		return _Data[Index];
	}

	constexpr UCodeGameEngineForceinlne const T& operator[](size_t Index) const
	{
#if UCodeGameEngineDEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
#endif // DEBUG

		return _Data[Index];
	}

	constexpr UCodeGameEngineForceinlne const T* Data()const
	{
		return _Data;
	}
	constexpr UCodeGameEngineForceinlne T* Data()
	{
		return _Data;
	}
	constexpr UCodeGameEngineForceinlne size_t Size()const
	{
		return _Size;
	}

	constexpr UCodeGameEngineForceinlne static Span<T> Make(T* data, size_t Size)
	{
		return Span(data, Size);
	}
	constexpr UCodeGameEngineForceinlne static const Span<T> Make(const T* data, size_t Size)
	{
		return Span((T*)data, Size);
	}
private:
	T* _Data;
	size_t _Size;
};


template<typename T> using Weak_ptr = std::weak_ptr<T>;//this need it's own implementation


template<typename T>
using Unique_ptr = std::unique_ptr<T>;

template<typename T>
using Unique_array = std::unique_ptr<T[]>;

template<typename T>
using ARef = std::shared_ptr<T>;

template<typename T>
struct Unique_Span
{
	using This = Unique_Span<T>;
	using Span_t = Span<T>;

	Unique_Span() :Pointer(nullptr), _Size(0)
	{

	}


	Unique_Span(This&& Other) :Pointer(std::move(Other.Pointer)), _Size(Other._Size)
	{
		Other._Size = 0;
	}
	This& operator=(This&& Other) noexcept
	{
		Pointer = std::move(Other.Pointer);
		_Size = Other._Size;

		Other._Size = 0;
		return *this;
	}

	constexpr UCodeGameEngineForceinlne T& operator[](size_t Index)
	{
#if UCodeGameEngineDEBUG
		if (Index >= _Size)
		{
			throw std::exception("Index out of range");
		}
#endif // DEBUG

		return Pointer[Index];
	}

	constexpr UCodeGameEngineForceinlne const T& operator[](size_t Index) const
	{
#if UCodeGameEngineDEBUG
		if (Index >= _Size)
		{
			throw std::exception("Index out of range");
		}
#endif // DEBUG

		return Pointer[Index];
	}

	const Span_t AsView() const
	{
		return Span_t::Make(Pointer.get(), _Size);
	}
	Span_t AsView()
	{
		return Span_t::Make(Pointer.get(), _Size);
	}
	void Resize(size_t Count)
	{
		Pointer.reset(new T[Count]);
		_Size = Count;
	}
	void Copyfrom(const Span_t& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			Pointer[i] = Values[i];
		}
	}
	void Copyfrom(Span_t&& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			Pointer[i] = std::move(Values[i]);
		}

		Values._Size = 0;
	}

	void Copyfrom(const Vector<T>& Values)
	{
		Resize(Values.size());
		for (size_t i = 0; i < Values.size(); i++)
		{
			Pointer[i] = Values[i];
		}
	}
	void Copyfrom(Vector<T>&& Values)
	{
		Resize(Values.size());
		for (size_t i = 0; i < Values.size(); i++)
		{
			Pointer[i] = std::move(Values[i]);
		}

		Values.resize(0);
	}


	Vector<T> ToVector() const
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = this->operator[](i);
		}

		return R;
	}
	Vector<T> MoveToVector()
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = std::move(this->operator[](i));
		}

		_Size = 0;

		return R;
	}

	size_t Size() const
	{
		return _Size;
	}
	T* Data()
	{
		return Pointer.get();
	}

	const T* Data() const
	{
		return Pointer.get();
	}

	T* Release()
	{
		auto r = Pointer.release();
		_Size = 0;
		return r;
	}
	void SetReset(T* Value, size_t Size)
	{
		Pointer.reset(Value);
		_Size = Size;
	}
private:
	Unique_array<T> Pointer;
	size_t _Size;
};

using BytesView = Span<Byte>;
using Unique_Bytes = Unique_Span<Byte>;



template<typename T>
struct Ref
{

public:
	struct RefData
	{
		T Data;
		size_t RefCount;

		template<typename... Pars>
		RefData(Pars... parameters) :Data(parameters...), RefCount(0) {}

		~RefData() = default;
	};
	Ref() :_Data(nullptr)
	{

	}
	Ref(const T& Data) : _Data(new RefData(Data))
	{
		AddPointerRef();
	}
	Ref(T&& Data) : _Data(new RefData(Data))
	{
		AddPointerRef();
	}

	template<typename... Pars>
	static Ref Create(Pars...  parameters)
	{
		Ref r = Ref();
		r._Data = new RefData(parameters...);
		r.AddPointerRef();
		return  r;
	}
	static Ref Move(T&& Ptr)
	{
		return  Ref(Ptr);
	}
	static Ref Bind(RefData* Ptr)
	{
		Ref r = Ref();
		r._Data = Ptr;
		r.AddPointerRef();
		return  r;
	}

	~Ref()
	{
		ReMovePointerRef();
	}

	Ref(const Ref& ToCopy) :_Data(ToCopy._Data)
	{
		AddPointerRef();
	}
	UCodeGameEngineForceinlne Ref& operator=(const Ref& ToCopy)
	{
		ReMovePointerRef();

		_Data = ToCopy._Data;
		AddPointerRef();

		return *this;
	}

	UCodeGameEngineForceinlne void SetToNull()
	{
		ReMovePointerRef();
		_Data = nullptr;
	}

	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne const T* get() const
	{
		if (_Data == nullptr)
		{
			return nullptr;
		}

		return &_Data->Data;
	}
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne const T* operator->() const { return get(); }
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne const T& operator*() const { return *get(); }
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne const T& operator[](size_t Index) const { return get()[Index]; }

	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne T* get()
	{
		if (_Data == nullptr)
		{
			return nullptr;
		}

		return &_Data->Data;
	}
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne T* operator->() { return get(); }
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne T& operator*() { return *get(); }
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne T& operator[](size_t Index) { return get()[Index]; }

	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne size_t get_RefCount() const { return _Data->RefCount; }
	UCodeGameEngineNoDiscard UCodeGameEngineForceinlne RefData* get_DatatPtr() { return _Data; }
private:
	RefData* _Data;
	void AddPointerRef()
	{
		if (_Data == nullptr) { return; }
		_Data->RefCount++;
	}
	void ReMovePointerRef()
	{
		if (_Data == nullptr) { return; }

		_Data->RefCount--;
		if (_Data->RefCount == 0)
		{
			delete _Data;
		}
	}
};



using Mat4 = glm::mat4;

class Bounds2d
{
public:
	Bounds2d() :center(), extents() {};
	Bounds2d(Vec2 c, Vec2 e) :center(c), extents(e) {};
	inline Vec2 max() { return center + extents; }
	inline Vec2 min() { return center - extents; }
	inline Vec2 size() { return extents * 2; }//https://docs.unity3d.com/ScriptReference/Bounds-size.html

	//
	Vec2 ClosestPoint(Vec2 v);
	bool Contains(Vec2 v);
	void Encapsulate(Vec2 v);
	void Expand(f32 amount);
	bool Intersects(const Bounds2d& other) const;



	Vec2 center;
	Vec2 extents;
};
class Bounds3d
{
	Bounds3d() {};
	Bounds3d(Vec3 c, Vec3 e) :center(c), extents(e) {};
	inline Vec3 max() { return center + extents; }
	inline Vec3 min() { return center - extents; }
	inline Vec3 size() { return extents * 2; }

	//
	Vec3 ClosestPoint(Vec3 v);
	bool Contains(Vec3 v);
	void Encapsulate(Vec3 v);
	void Expand(f32 amount);
	bool Intersects(const Bounds3d& B);

	Vec3 center;
	Vec3 extents;
};

class Bounds2dInt
{
	Bounds2dInt() {};
	Bounds2dInt(Vec2i c, Vec2i e) :center(c), extents(e) {};
	inline Vec2i max() { return center + extents; }
	inline Vec2i min() { return center - extents; }
	inline Vec2i size() { return extents * 2; }

	//
	Vec2i ClosestPoint(Vec2i v);
	bool Contains(Vec2i v);
	void Encapsulate(Vec2i v);
	void Expand(i32 amount);
	bool Intersects(const Bounds2dInt& B);

	Vec2i center;
	Vec2i extents;
};
class Bounds3dInt
{
	Bounds3dInt() {};
	Bounds3dInt(Vec3i c, Vec3i e) :center(c), extents(e) {};
	inline Vec3i max() { return center + extents; }
	inline Vec3i min() { return center - extents; }
	inline Vec3i size() { return extents * 2; }

	//
	Vec3i ClosestPoint(Vec3i v);
	bool Contains(Vec3i v);
	void Encapsulate(Vec3i v);
	void Expand(i32 amount);
	bool Intersects(const Bounds3dInt& B);

	Vec3i center;
	Vec3i extents;
};
template<typename R, typename... Pars>
using Delegate = std::function<R(Pars...)>;

struct Version
{
	u64 Major;
	u64 Minor;
	u64 Revision;
	Version()
	{
		Major = 0;
		Minor = 0;
		Revision = 0;
	}
	Version(u64 major, u64 minor, u64 revision)
	{
		Major = major;
		Minor = minor;
		Revision = revision;
	}
};

template<typename T, typename E>
class Result
{
public:
	Result() {}
	~Result() {}

	Result(const T& Value)
		:_Base(Value) {}

	Result(T&& Value)
		:_Base(std::move(Value)) {}

	Result(const E& Value)
		:_Base(Value) {}

	Result(E&& Value)
		:_Base(std::move(Value)) {}

	bool IsError() const
	{
		return _Base.template  Is<E>();
	}
	bool IsValue() const
	{
		return _Base.template Is<T>();
	}
	E* IfError()
	{
		return _Base.template Get_If<E>();
	}
	T* IfValue()
	{
		return _Base.template Get_If<T>();
	}

	const E* IfError() const
	{
		return _Base.template Get_If<E>();
	}
	const T* IfValue() const
	{
		return _Base.template Get_If<T>();
	}

	const E& GetError() const
	{
		return _Base.template Get<E>();
	}
	const T& GetValue() const
	{
		return _Base.template Get<T>();
	}
	E& GetError()
	{
		return _Base.template Get<E>();
	}
	T& GetValue()
	{
		return _Base.template Get<T>();
	}

	T ValueOr(const T& Or)
	{
		if (IsValue())
		{
			return GetValue();
		}
		return Or;
	}
	const T ValueOr(const T& Or) const
	{
		if (IsValue())
		{
			return GetValue();
		}
		return Or;
	}
	T ErrorOr(const T Or)
	{
		if (IsError())
		{
			return GetError();
		}
		return Or;
	}
	const T ErrorOr(const T& Or) const
	{
		if (IsError())
		{
			return GetError();
		}
		return Or;
	}
	Optional<T> AsOption()
	{
		if (IsValue())
		{
			return GetValue();
		}
		return {};
	}
	Optional<E> AsOptionError()
	{
		if (IsError())
		{
			return GetError();
		}
		return {};
	}
private:
	Variant<T, E> _Base;
};
CoreEnd