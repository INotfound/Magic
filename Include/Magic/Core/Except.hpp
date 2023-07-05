/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Except.hpp
 * @Date           : 2023-07-03 18:26
 ******************************************************************************
 */
#pragma once

#include <string>
#include <stdexcept>

#include "Magic/Core/StringView.hpp"

namespace Magic{
    class Failure:public std::runtime_error{
    public:
        explicit Failure(const Magic::StringView& error)
            :std::runtime_error(error.data()){
        }
    };
}
