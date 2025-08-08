#pragma once

#include <string>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

namespace Inferonix::UUID
{
    [[nodiscard]] inline std::string Generate()
    {
        static boost::uuids::random_generator generator;
        auto const id_object = generator();
        return boost::uuids::to_string(id_object);
    }
}