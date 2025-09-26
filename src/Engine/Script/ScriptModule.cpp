#include "ScriptModule.hpp"

#include <utility>


using namespace Inferonix::Script;

ScriptModule::ScriptModule(std::shared_ptr<asIScriptModule> module) : _instance(std::move(module))
{

}



