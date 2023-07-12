#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
#include <yaml-cpp/yaml.h>
namespace YAML
{
    template<typename Asset, typename AssetBase>
    struct convert<UCode::AssetPtr<Asset,AssetBase>>
    {
        using TypePtr = UCode::AssetPtr<Asset,AssetBase>;
        static Node encode(const TypePtr& rhs) {
            Node node;
            auto State = rhs.Get_State();
            bool SetAsAsset = false;
            if (State == TypePtr::State::ManagedPtr)
            {
                if (rhs.Get_Managed().Has_Value()) 
                {
                    if (rhs.Get_Managed().Get_Value()->Uid.has_value()) 
                    {

                        State = TypePtr::State::UID;
                        SetAsAsset = true;
                    }
                }
            }
            node.push_back(UCode_YAML_UCharFix(State));

            switch (rhs.Get_State())
            {
            case TypePtr::State::UID:
                node.push_back(rhs.Get_UID());
                break;
            default:
                break;
            }

            if (SetAsAsset)
            {
                auto& asset = *rhs.Get_Managed().Get_Value();
                node.push_back(asset.Uid.value());
            }

            return node;
        }

        static bool decode(const Node& node, TypePtr& rhs) {
            if (!node.IsSequence()) {
                return false;
            }

            auto State = (TypePtr::State)node[0].as<TypePtr::State_t>();

            switch (State)
            {
            case TypePtr::State::Null:
                break;
            case TypePtr::State::UID:
                rhs = node[1].as<UCode::UID>();
                break;
            default:
                break;
            }
            return true;
        }
    };


}