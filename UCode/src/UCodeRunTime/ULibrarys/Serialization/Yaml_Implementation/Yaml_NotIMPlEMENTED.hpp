#pragma once

#define Yaml_NotIMPlEMENTED(Type)\
namespace YAML {\
template<> struct convert<Type> {\
	\
		static Node encode(const Type& rhs) {\
		\
			UCodeGEToDo();\
			return {};\
	   }\
		\
			static bool decode(const Node& node, Type& rhs) {\
			\
				UCodeGEToDo();\
			return false; \
		}\
}; \
} \

