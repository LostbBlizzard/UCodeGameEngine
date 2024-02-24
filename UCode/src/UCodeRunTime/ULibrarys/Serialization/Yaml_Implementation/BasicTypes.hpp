#pragma once
#include <yaml-cpp/yaml.h>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/UID.hpp>


namespace YAML {
	template<> struct convert<UCode::Vec2> {
		static Node encode(const UCode::Vec2& rhs) {
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			return node;
		}

		static bool decode(const Node& node, UCode::Vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			return true;
		}
	};
	template<> struct convert<UCode::Vec3> {
		static Node encode(const UCode::Vec3& rhs) {
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			node.push_back(rhs.Z);
			return node;
		}

		static bool decode(const Node& node, UCode::Vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();
			return true;
		}

	};

	template<> struct convert<UCode::Vec2i> {
		static Node encode(const UCode::Vec2i& rhs) {
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			return node;
		}

		static bool decode(const Node& node, UCode::Vec2i& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();
			return true;
		}
	};
	template<> struct convert<UCode::Vec3i> {
		static Node encode(const UCode::Vec3i& rhs) {
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			node.push_back(rhs.Z);
			return node;
		}

		static bool decode(const Node& node, UCode::Vec3i& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();
			rhs.Z = node[2].as<int>();
			return true;
		}

	};

	template<> struct convert<UCode::Color> {
		static Node encode(const UCode::Color& rhs) {
			Node node;
			node.push_back(rhs.R);
			node.push_back(rhs.G);
			node.push_back(rhs.B);
			node.push_back(rhs.A);
			return node;
		}

		static bool decode(const Node& node, UCode::Color& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}


			rhs.R = node[0].as<float>();
			rhs.G = node[1].as<float>();
			rhs.B = node[2].as<float>();
			rhs.A = node[3].as<float>();
			return true;
		}
	};
	template<> struct convert<UCode::Color32> {
		static Node encode(const UCode::Color32& rhs) {
			Node node;
			node.push_back(rhs.R);
			node.push_back(rhs.G);
			node.push_back(rhs.B);
			node.push_back(rhs.A);
			return node;
		}

		static bool decode(const Node& node, UCode::Color32& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.R = node[0].as<UCode::Byte>();
			rhs.G = node[1].as<UCode::Byte>();
			rhs.B = node[2].as<UCode::Byte>();
			rhs.A = node[3].as<UCode::Byte>();
			return true;
		}

	};

	template<> struct convert<UCode::ColorRGB> {
		static Node encode(const UCode::ColorRGB& rhs) {
			Node node;
			node.push_back(rhs.R);
			node.push_back(rhs.G);
			node.push_back(rhs.B);
			return node;
		}

		static bool decode(const Node& node, UCode::ColorRGB& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}


			rhs.R = node[0].as<float>();
			rhs.G = node[1].as<float>();
			rhs.B = node[2].as<float>();
			return true;
		}
	};
	template<> struct convert<UCode::Color24> {
		static Node encode(const UCode::Color24& rhs) {
			Node node;
			node.push_back(rhs.R);
			node.push_back(rhs.G);
			node.push_back(rhs.B);
			return node;
		}

		static bool decode(const Node& node, UCode::Color24& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.R = node[0].as<UCode::Byte>();
			rhs.G = node[1].as<UCode::Byte>();
			rhs.B = node[2].as<UCode::Byte>();
			return true;
		}

	};


	template<> struct convert<UCode::Path> {
		static Node encode(const UCode::Path& rhs) {
			Node node;
			node.push_back(rhs.generic_string());
			return node;
		}

		static bool decode(const Node& node, UCode::Path& rhs) {
			if (!node.IsSequence() || node.size() != 1) {
				return false;
			}

			rhs = node[0].as<UCode::Path>();
			return true;
		}

	};

	template <typename T_>
	YAML::Emitter& operator<<(YAML::Emitter& out, const T_& rhs) {
		out << convert<T_>::encode(rhs);
		return out;
	}
	template<> struct convert<UCode::UID> {
		static Node encode(const UCode::UID& rhs) {
			Node node;
			node.push_back(rhs.Get_Value());
			return node;
		}

		static bool decode(const Node& node, UCode::UID& rhs) {
			if (!node.IsSequence() || node.size() != 1) {
				return false;
			}

			rhs.Set_Value(node[0].as<UCode::UID::UID_t>());
			return true;
		}
	};

	template<> struct convert<UCode::Version> {
		static Node encode(const UCode::Version& rhs) {
			Node node;
			node.push_back(rhs.Major);
			node.push_back(rhs.Minor);
			node.push_back(rhs.Patch);
			return node;
		}

		static bool decode(const Node& node, UCode::Version& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.Major = node[0].as<UCode::u32>();
			rhs.Minor = node[1].as<UCode::u32>();
			rhs.Patch = node[2].as<UCode::u32>();
			return true;
		}
	};
	
}