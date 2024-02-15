#include "Typedefs.hpp"

EditorStart

struct HttpHelper
{
    using Url = String;
    static Optional<String> DonloadString(const Url& host, const Url& file);
};

EditorEnd
