/*
 * @Author: INotFound
 * @Date: 2022-11-08 11:56:20
 * @LastEditTime: 2022-11-08 11:56:20
 */
#pragma once

#include <string>
#include <stdexcept>

#include "Magic/Core/StringView.hpp"

namespace Magic{
    class Failure:public std::runtime_error{
    public:
        explicit Failure(const std::string_view& error)
            :std::runtime_error(error.data()){
        }
    };
}
