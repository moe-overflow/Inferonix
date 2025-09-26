#pragma once

#include <vector>
#include <string_view>
#include <memory>

#include <angelscript.h>

namespace Inferonix::Script
{
    struct MethodArgument
    {
        // todo: implement me
    };

    class ScriptObject final
    {
    public:
        ScriptObject() = default;

        void CallMethod(std::vector<MethodArgument> const& arguments, std::string_view implementation);

    private:
        std::unique_ptr<asIScriptObject> _instance;

    };
}