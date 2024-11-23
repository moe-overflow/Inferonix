#pragma once

#include <string>
#include <utility>

namespace Inferonix::UI
{
    class panel
    {
    protected:
        explicit panel(std::string name) :
            _name(std::move(name))
        {}

    public:
        virtual ~panel() = default;

        panel(const panel&) = delete;
        panel& operator=(const panel&) = delete;

        virtual void display() = 0;

    private:
        std::string _name;


    };
}