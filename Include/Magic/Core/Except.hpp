/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Except.hpp
 * @Date           : 2023-07-03 18:26
 ******************************************************************************
 */
#pragma once

#include <stdexcept>

#include "Magic/Core/StringView.hpp"

namespace Magic{
    class Failure:public std::runtime_error{
    public:
        explicit Failure(const StringView& error);
    };
}
