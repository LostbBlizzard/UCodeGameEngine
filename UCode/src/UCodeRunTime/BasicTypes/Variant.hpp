/*
Copyright (c) MapBox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
- Neither the name "MapBox" nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <algorithm>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/UDefs.hpp>
CoreStart

//based on https://github.com/mapbox/variant/blob/master/include/mapbox/variant.hpp#L93

using VariantTagType = unsigned char;
static constexpr VariantTagType invalid_value = VariantTagType(-1);

template <typename T>
class recursive_wrapper
{

	T* p_;

	void assign(T const& rhs)
	{
		this->get() = rhs;
	}

public:
	using type = T;

	/**
	 * Default constructor default initializes the internally stored value.
	 * For POD types this means nothing is done and the storage is
	 * uninitialized.
	 *
	 * @throws std::bad_alloc if there is insufficient memory for an object
	 *         of type T.
	 * @throws any exception thrown by the default constructur of T.
	 */
	recursive_wrapper()
		: p_(new T) {}

	~recursive_wrapper() noexcept { delete p_; }

	recursive_wrapper(recursive_wrapper const& operand)
		: p_(new T(operand.get())) {}

	recursive_wrapper(T const& operand)
		: p_(new T(operand)) {}

	recursive_wrapper(recursive_wrapper&& operand)
		: p_(new T(std::move(operand.get()))) {}

	recursive_wrapper(T&& operand)
		: p_(new T(std::move(operand))) {}

	inline recursive_wrapper& operator=(recursive_wrapper const& rhs)
	{
		assign(rhs.get());
		return *this;
	}

	inline recursive_wrapper& operator=(T const& rhs)
	{
		assign(rhs);
		return *this;
	}

	inline void swap(recursive_wrapper& operand) noexcept
	{
		T* temp = operand.p_;
		operand.p_ = p_;
		p_ = temp;
	}

	recursive_wrapper& operator=(recursive_wrapper&& rhs) noexcept
	{
		swap(rhs);
		return *this;
	}

	recursive_wrapper& operator=(T&& rhs)
	{
		get() = std::move(rhs);
		return *this;
	}

	T& get()
	{
		assert(p_);
		return *get_pointer();
	}

	T const& get() const
	{
		assert(p_);
		return *get_pointer();
	}

	T* get_pointer() { return p_; }

	const T* get_pointer() const { return p_; }

	operator T const& () const { return this->get(); }

	operator T& () { return this->get(); }

}; // class recursive_wrapper

template <typename T>
inline void swap(recursive_wrapper<T>& lhs, recursive_wrapper<T>& rhs) noexcept
{
	lhs.swap(rhs);
}

template <typename T, typename... Types>
struct direct_type;

template <typename T, typename First, typename... Types>
struct direct_type<T, First, Types...>
{
	static constexpr VariantTagType index = std::is_same<T, First>::value
		? sizeof...(Types)
		: direct_type<T, Types...>::index;
};

template <typename T>
struct direct_type<T>
{
	static constexpr VariantTagType index = invalid_value;
};


template <typename...>
struct conjunction : std::true_type {};

template <typename B1>
struct conjunction<B1> : B1 {};

template <typename B1, typename B2>
struct conjunction<B1, B2> : std::conditional<B1::value, B2, B1>::type {};

template <typename B1, typename... Bs>
struct conjunction<B1, Bs...> : std::conditional<B1::value, conjunction<Bs...>, B1>::type {};

template <typename...>
struct disjunction : std::false_type {};

template <typename B1>
struct disjunction<B1> : B1 {};

template <typename B1, typename B2>
struct disjunction<B1, B2> : std::conditional<B1::value, B1, B2>::type {};

template <typename B1, typename... Bs>
struct disjunction<B1, Bs...> : std::conditional<B1::value, B1, disjunction<Bs...>>::type {};


template <typename T, typename... Types>
struct convertible_type;

template <typename T, typename First, typename... Types>
struct convertible_type<T, First, Types...>
{
	static constexpr VariantTagType index = std::is_convertible<T, First>::value
		? disjunction<std::is_convertible<T, Types>...>::value ? invalid_value : sizeof...(Types)
		: convertible_type<T, Types...>::index;
};



template <typename T>
struct convertible_type<T>
{
	static constexpr VariantTagType index = invalid_value;
};


template <typename T, typename... Types>
struct value_traits
{
	using value_type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
	using value_type_wrapper = recursive_wrapper<value_type>;
	static constexpr VariantTagType direct_index = direct_type<value_type, Types...>::index;
	static constexpr bool is_direct = direct_index != invalid_value;
	static constexpr VariantTagType index_direct_or_wrapper = is_direct ? direct_index : direct_type<value_type_wrapper, Types...>::index;
	static constexpr bool is_direct_or_wrapper = index_direct_or_wrapper != invalid_value;
	static constexpr VariantTagType index = is_direct_or_wrapper ? index_direct_or_wrapper : convertible_type<value_type, Types...>::index;
	static constexpr bool is_valid = index != invalid_value;
	static constexpr VariantTagType tindex = is_valid ? sizeof...(Types) - index : 0;
	using target_type = typename std::tuple_element<tindex, std::tuple<void, Types...>>::type;
};


template <typename... Types>
struct variant_helper;

template <typename T, typename... Types>
struct variant_helper<T, Types...>
{
	UCodeGameEngineForceinlne static void construct(const VariantTagType type_index, void* data)
	{
		if (type_index == sizeof...(Types))
		{
			new (data) T();
		}
		else
		{
			variant_helper<Types...>::construct(type_index,data);
		}
	}

	UCodeGameEngineForceinlne static void destroy(const VariantTagType type_index, void* data)
	{
		if (type_index == sizeof...(Types))
		{
			reinterpret_cast<T*>(data)->~T();
		}
		else
		{
			variant_helper<Types...>::destroy(type_index, data);
		}
	}

	UCodeGameEngineForceinlne static void move(const VariantTagType old_type_index, void* old_value, void* new_value)
	{
		if (old_type_index == sizeof...(Types))
		{
			new (new_value) T(std::move(*reinterpret_cast<T*>(old_value)));
		}
		else
		{
			variant_helper<Types...>::move(old_type_index, old_value, new_value);
		}
	}

	UCodeGameEngineForceinlne static void copy(const VariantTagType old_type_index, const void* old_value, void* new_value)
	{
		if (old_type_index == sizeof...(Types))
		{
			new (new_value) T(*reinterpret_cast<const T*>(old_value));
		}
		else
		{
			variant_helper<Types...>::copy(old_type_index, old_value, new_value);
		}
	}
};

template <>
struct variant_helper<>
{
	UCodeGameEngineForceinlne static void construct(const VariantTagType, void*) {}
	UCodeGameEngineForceinlne static void destroy(const VariantTagType, void*) {}
	UCodeGameEngineForceinlne static void move(const VariantTagType, void*, void*) {}
	UCodeGameEngineForceinlne static void copy(const VariantTagType, const void*, void*) {}
};


template<typename... Types>
class Variant
{
private:
public:
	template<typename T>
	struct ThisElm
	{
		ThisElm()
		{

		}
	};


	using TagType = VariantTagType;
	static constexpr TagType NullTag = invalid_value;
	using ThisType = Variant<Types...>;
	static constexpr size_t ItemSize = std::max({ sizeof(Types)... });
	static constexpr size_t ItemsInListCount = sizeof...(Types);
	using helper_type = variant_helper<Types...>;

	union Pack
	{
		char Items[ItemSize];
	};
	static_assert(sizeof...(Types) > 0, "Must have at one Type");
	Variant() noexcept :_Tag(NullTag)
	{
		
	}
	~Variant()
	{
		if (_Tag != NullTag)
		{
			helper_type::destroy(_Tag,&_Items);
		}
	}

	Variant(const ThisType& source) noexcept
	{
		_Tag = source._Tag;
		_Items = source._Items;
	}

	Variant(ThisType&& source) noexcept
	{
		_Tag = source._Tag;
		_Items = source._Items;
		//
		new (&source) ThisType();
	}
	template<typename T> Variant(const T& source) noexcept
	{
		MustBeMyType<T>();

		_Tag = GetTypeValue<T>();
		new ((T*)&_Items) T(source);
	}

	/*
	template<typename T> Variant(T&& source) noexcept
	{
		MustBeMyType<T>();

		_Tag = GetTypeValue<T>();
		new ((T*)&_Items) T(std::move(source));
	}
	*/

	ThisType& operator=(const ThisType& source) noexcept
	{
		this->~Variant();
		new (this) ThisType(source);

		return *this;
	}
	ThisType& operator=(ThisType&& source) noexcept
	{
		new (this) ThisType(source);

		return *this;
	}
	template<typename T> ThisType& operator=(const T& source) noexcept
	{
		this->~Variant();
		new (this) ThisType(source);

		return *this;
	}
	template<typename T> ThisType& operator=(T&& source) noexcept
	{
		this->~Variant();
		new (this) ThisType(std::move(source));

		return *this;
	}
	
	template<typename T>
	bool IsType() const
	{
		MustBeMyType<T>();
		return _Tag == GetTypeValue<T>();
	}
	template<typename T> T& GetType()
	{
		MustBeMyType<T>();
		if (!IsType<T>())
		{
		UCodeGameEngineThrowException("bad type cast");
		}
		return *(T*)&_Tag;
	}
	template<typename T> const T& GetType() const
	{
		MustBeMyType<T>();
		if (!IsType<T>())
		{
			UCodeGameEngineThrowException("bad type cast");
		}
		return *(T*)&_Tag;
	}

	template<typename T>
	T GetTypeOr(const T& Or) const
	{
		MustBeMyType<T>();
		if (!IsType<T>())
		{
			return Or;
		}
		return GetType<T>();
	}
	template<typename T>
	T* IfType() 
	{
		MustBeMyType<T>();
		if (!IsType<T>())
		{
			return nullptr;
		}
		return &GetType<T>();
	}
	template<typename T>
	const T* IfType() const
	{
		MustBeMyType<T>();
		if (!IsType<T>())
		{
			return nullptr;
		}
		return &GetType<T>();
	}

	void* Get_Items()
	{
		return &_Items;
	}
	TagType Get_Tag() const
	{
		return _Tag;
	}



	template<typename T> static TagType GetTypeValue()
	{
		MustBeMyType<T>();
		return value_traits<T, Types...>::index;
	}
private:
	TagType _Tag;
	Pack _Items;


	template<typename T>
	static void MustBeMyType()
	{
		using Traits = value_traits<T, Types...>;


		static_assert(Traits::is_valid, "This is not Part of the Types");
	}
	
};



CoreEnd