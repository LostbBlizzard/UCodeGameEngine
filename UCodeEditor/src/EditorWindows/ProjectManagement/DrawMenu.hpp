#include "Typedefs.hpp"

EditorStart


template<typename T>
Optional<size_t> DrawMenu(const Span<T> items, StringView(*getname)(const T&))
{
    Optional<size_t> ClickedIndex;
    char menuspilter = '/';

    StringView spliterstr = StringView(&menuspilter, 1);

    Vector<StringView> splitstr;
    Vector<StringView> MenuScopesplits;
    Vector<bool> MenuScopelist;
    String tep;
    for (size_t i = 0; i < items.Size(); i++)
    {
        auto& Item = items[i];
        auto Item_MenuName = getname(Item);
        
        StringView ItemMenuScope = Item_MenuName;
        {
            for (int i = ItemMenuScope.size() - 1; i >= 0; i--)
            {
                char V = ItemMenuScope[i];

                if (V == menuspilter || i == 0)
                {
                    ItemMenuScope = ItemMenuScope.substr(0, i);
                    break;
                }
            }
        }

        StringHelper::Split(ItemMenuScope, spliterstr, splitstr);
        if (splitstr.size() == 0 && ItemMenuScope.size())
        {
            splitstr.push_back(ItemMenuScope);
        }

        Vector<bool> myboollist;
        Vector<StringView> mysplitstr;
        for (size_t i = 0; i < std::max(splitstr.size(), MenuScopesplits.size()); i++)
        {
            Optional<StringView> MenuScope = i < MenuScopesplits.size() ? MenuScopesplits[i] : Optional<StringView>();
            Optional<StringView> ItemScope = i < splitstr.size() ? splitstr[i] : Optional<StringView>();

            bool lastopen = myboollist.size() ? myboollist.back() == true : true;

            if (MenuScope.has_value() && ItemScope.has_value())
            {
                if (MenuScope.value() == ItemScope.value())
                {
                    myboollist.push_back(lastopen ? MenuScopelist[i] : false);
                }
                else
                {
                    tep = String(ItemScope.value());
                    //ImGui::EndMenu();
                    //myboollist.push_back(lastopen ? ImGui::BeginMenu(tep.c_str()) : false);
                }
                mysplitstr.push_back(ItemScope.value());
            }
            else if (MenuScope.has_value())
            {
                myboollist.push_back(lastopen ? MenuScopelist[i] : false);

                if (MenuScopelist[i])
                {
                    //ImGui::EndMenu();
                }
            }
            else if (ItemScope.has_value())
            {
                tep = String(ItemScope.value());
                //myboollist.push_back(lastopen ? ImGui::BeginMenu(tep.c_str()) : false);
                mysplitstr.push_back(ItemScope.value());
            }


        }

        {
            bool lastopen = myboollist.size() ? myboollist.back() == true : true;

            if (lastopen)
            {
                StringView FirstName = "";

                for (int i = Item_MenuName.size() - 1; i >= 0; i--)
                {
                    char V = Item_MenuName[i];

                    if (V == menuspilter || i == 0)
                    {
                        FirstName = StringView(Item_MenuName).substr(i + 1);
                        break;
                    }
                }

                //tep = FirstName;
                tep = Item_MenuName;
                if (ImGui::MenuItem(tep.c_str()))
                {
                    ClickedIndex = i;
                }
            }
        }
        MenuScopesplits = std::move(mysplitstr);
        MenuScopelist = std::move(myboollist);
    }

    for (size_t i = 0; i < MenuScopelist.size(); i++)
    {
        if (MenuScopelist[i]) {
            //ImGui::EndMenu();
        }
    }

    return ClickedIndex;
}

EditorEnd
 