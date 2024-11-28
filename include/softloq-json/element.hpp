#ifndef SOFTLOQ_JSON_ELEMENT_HPP
#define SOFTLOQ_JSON_ELEMENT_HPP

/**
 * @author Brandon Foster
 * @file element.hpp
 * @version 1.0.0
 * @brief JSON Element object based on https://www.json.org
 */

#include "softloq-json/error.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Softloq::JSON
{
    /** @brief JSON Element Types are based on https://www.json.org */
    enum class ElementType
    {
        Object,
        Array,
        String,
        Number,
        Bool,
        Null
    };

    /** @brief Abstract class of a JSON Element object. */
    class Element
    {
    public:
        /** @brief Get the Element Type of the JSON Element object. */
        virtual const ElementType getElementType() const = 0;

        /** @brief Provides the JSON Element object in its corresponding JSON text form. */
        virtual const std::string toString() const = 0;

        /** @brief Clean way of type casting JSON element when the type is known. If the type is not known, please use getElementType(). */
        template <class ELEMENT_TYPE>
        ELEMENT_TYPE *const as() { return dynamic_cast<ELEMENT_TYPE *>(this); }
    };

    /** @brief C++ Representation of a JSON Object element. */
    class Object : public Element, public std::unordered_map<std::string, std::unique_ptr<Element>>
    {
    public:
        inline const ElementType getElementType() const override { return ElementType::Object; }
        SOFTLOQ_JSON_API const std::string toString() const override;
    };

    /** @brief C++ Representation of a JSON Array element. */
    class Array : public Element, public std::vector<std::unique_ptr<Element>>
    {
    public:
        // General
        inline const ElementType getElementType() const override { return ElementType::Array; }
        SOFTLOQ_JSON_API const std::string toString() const override;
    };

    /** @brief C++ Representation of a JSON String element. */
    class String : public Element
    {
    public:
        inline const ElementType getElementType() const override { return ElementType::String; }
        inline const std::string toString() const override { return std::string("\"") + value + '"'; }

        SOFTLOQ_JSON_API String();
        SOFTLOQ_JSON_API String(const std::string &value);

        inline void setString(const std::string &value) { this->value = value; }
        inline const std::string &getString() const { return value; }

    private:
        std::string value;
    };

    /** @brief C++ Representation of a JSON Number element. */
    class Number : public Element
    {
    public:
        inline const ElementType getElementType() const override { return ElementType::Number; }
        inline const std::string toString() const override { return std::to_string(value); }

        SOFTLOQ_JSON_API Number();
        SOFTLOQ_JSON_API Number(const float value);

        inline void setNumber(const float value) { this->value = value; }
        inline constexpr float getNumber() const { return value; }

    private:
        float value;
    };

    /** @brief C++ Representation of a JSON Bool element. */
    class Bool : public Element
    {
    public:
        inline const ElementType getElementType() const override { return ElementType::Bool; }
        inline const std::string toString() const override { return value ? "true" : "false"; }

        SOFTLOQ_JSON_API Bool();
        SOFTLOQ_JSON_API Bool(const bool value);

        inline void setBool(const bool value) { this->value = value; }
        inline constexpr bool getBool() const { return value; }

    private:
        bool value;
    };

    /** @brief C++ Representation of a JSON Null element. */
    class Null : public Element
    {
    public:
        inline const ElementType getElementType() const override { return ElementType::Null; }
        inline const std::string toString() const override { return "null"; }
    };
}

#endif