#pragma once

#define Yaml_NotIMPlEMENTED(Type)\
namespace YAML {\
template<> struct convert<Type> {\
	\
		static Node encode(const Type& rhs) {\
		\
			UCODE_ENGINE_IMPlEMENTED_LATER;\
			return {};\
	   }\
		\
			static bool decode(const Node& node, Type& rhs) {\
			\
				UCODE_ENGINE_IMPlEMENTED_LATER;\
			return false; \
		}\
}; \
} \

