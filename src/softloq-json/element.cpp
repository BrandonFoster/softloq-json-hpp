#include "softloq-json/element.hpp"

namespace Softloq::JSON
{
    SOFTLOQ_JSON_API const std::string Object::toString() const
    {
        std::string json_text = "{";
        for (const auto &item : *this)
        {
            if (json_text.length() > 1)
                json_text += ",";
            json_text += " ";
            json_text += "\"";
            json_text += item.first; // string key
            json_text += "\"";
            json_text += " : ";
            json_text += item.second->toString();
        }
        json_text += " }";
        return json_text;
    }

    SOFTLOQ_JSON_API const std::string Array::toString() const
    {
        std::string json_text = "[";
        for (const auto &value : *this)
        {
            if (json_text.length() > 1)
                json_text += ",";
            json_text += " ";
            json_text += value->toString();
        }
        json_text += " ]";
        return json_text;
    }

    SOFTLOQ_JSON_API String::String() : value() {}
    SOFTLOQ_JSON_API String::String(const std::string &value) : value(value) {}

    SOFTLOQ_JSON_API Number::Number() : value(0.0f) {}
    SOFTLOQ_JSON_API Number::Number(const float value) : value(value) {}

    SOFTLOQ_JSON_API Bool::Bool() : value(false) {}
    SOFTLOQ_JSON_API Bool::Bool(const bool value) : value(value) {}
}