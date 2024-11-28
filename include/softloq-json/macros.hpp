#ifndef SOFTLOQ_JSON_MACROS_HPP
#define SOFTLOQ_JSON_MACROS_HPP

/**
 * @author Brandon Foster
 * @file macros.hpp
 * @version 1.0.0
 * @brief JSON Macros.
 */

// Static/Shared Build Macros

#if _MSC_VER
#define SOFTLOQ_JSON_EXPORT __declspec(dllexport)
#define SOFTLOQ_JSON_IMPORT __declspec(dllimport)
#elif __GNUC__
#define SOFTLOQ_JSON_EXPORT __attribute__((visibility("default")))
#define SOFTLOQ_JSON_IMPORT
#endif

// For Standalone Builds

#ifdef SOFTLOQ_JSON_NO_API
#define SOFTLOQ_JSON_API
#elif SOFTLOQ_JSON_BUILD_API
#define SOFTLOQ_JSON_API SOFTLOQ_JSON_EXPORT
#elif SOFTLOQ_JSON_USE_API
#define SOFTLOQ_JSON_API SOFTLOQ_JSON_IMPORT
#endif

// For Monolithic Builds

#ifndef SOFTLOQ_JSON_API
#ifdef SOFTLOQ_NO_API
#define SOFTLOQ_JSON_API
#elif SOFTLOQ_BUILD_API
#define SOFTLOQ_JSON_API SOFTLOQ_JSON_EXPORT
#elif SOFTLOQ_USE_API
#define SOFTLOQ_JSON_API SOFTLOQ_JSON_IMPORT
#endif
#endif

// Static Build By Default

#ifndef SOFTLOQ_JSON_API
/** @brief Static/Shared Build Macro. */
#define SOFTLOQ_JSON_API
#endif

#endif