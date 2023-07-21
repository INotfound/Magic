/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Except.cpp
 * @Date           : 2023-07-19 12:33
 ******************************************************************************
 */
#include "Magic/Core/Except.hpp"

namespace Magic{
    Failure::Failure(const Magic::StringView& error)
        :std::runtime_error(error.data()){
    }
}