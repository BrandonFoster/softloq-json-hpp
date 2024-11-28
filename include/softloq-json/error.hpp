#ifndef SOFTLOQ_JSON_ERROR_HPP
#define SOFTLOQ_JSON_ERROR_HPP

/**
 * @author Brandon Foster
 * @file error.hpp
 * @version 1.0.0
 * @brief Holds an error that occurs in a JSON function.
 */

#include "softloq-json/macros.hpp"
#include <string>

namespace Softloq::JSON
{
    /** @brief JSON error handle. */
    struct Error
    {
        /** @brief The error message. */
        std::string message;
    };
}

#endif