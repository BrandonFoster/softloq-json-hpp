#include "softloq-json/decoder.hpp"
#include "softloq-unicode/unicode.hpp"
#include <regex>
#include <cmath>

namespace Softloq::JSON
{
    const bool parseMatch(std::string_view &json_segment, const std::string &match)
    {
        if (json_segment.substr(0, match.length()) == match)
        {
            json_segment = json_segment.substr(match.length());
            return true;
        }
        return false;
    }

    SOFTLOQ_JSON_API const Element *Decoder::decodeJSON(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_element;
        if (!parseElement(json_segment, json_element))
            return nullptr;
        return json_segment.empty() ? nullptr : json_element.release();
    }
    SOFTLOQ_JSON_API const Object *Decoder::decodeObject(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_object;
        if (!(parseWS(json_segment) && parseObject(json_segment, json_object) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<Object *>(json_object.release());
    }
    SOFTLOQ_JSON_API const Array *Decoder::decodeArray(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_array;
        if (!(parseWS(json_segment) && parseArray(json_segment, json_array) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<Array *>(json_array.release());
    }
    SOFTLOQ_JSON_API const String *Decoder::decodeString(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_string_element;
        if (!(parseWS(json_segment) && parseString(json_segment, json_string_element) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<String *>(json_string_element.release());
    }
    SOFTLOQ_JSON_API const Number *Decoder::decodeNumber(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_number;
        if (!(parseWS(json_segment) && parseNumber(json_segment, json_number) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<Number *>(json_number.release());
    }
    SOFTLOQ_JSON_API const Bool *Decoder::decodeBool(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_bool;
        if (!(parseWS(json_segment) && parseBool(json_segment, json_bool) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<Bool *>(json_bool.release());
    }
    SOFTLOQ_JSON_API const Null *Decoder::decodeNull(const std::string &json_text)
    {
        std::string_view json_segment(json_text);
        std::unique_ptr<Element> json_null;
        if (!(parseWS(json_segment) && parseNull(json_segment, json_null) && parseWS(json_segment)))
            return nullptr;
        return json_segment.empty() ? nullptr : dynamic_cast<Null *>(json_null.release());
    }

    SOFTLOQ_JSON_API const bool Decoder::parseValue(std::string_view &json_segment, std::unique_ptr<Element> &json_value)
    {
        return parseObject(json_segment, json_value) || parseArray(json_segment, json_value) || parseString(json_segment, json_value) || parseNumber(json_segment, json_value) || parseBool(json_segment, json_value) || parseNull(json_segment, json_value);
    }
    SOFTLOQ_JSON_API const bool Decoder::parseObject(std::string_view &json_segment, std::unique_ptr<Element> &json_object)
    {
        const std::string_view base_segment(json_segment);

        if (parseMatch(json_segment, "{") && parseWS(json_segment) && parseMatch(json_segment, "}"))
        {
            json_object = std::unique_ptr<Element>(new Object);
            return true;
        }

        json_segment = base_segment;
        if (parseMatch(json_segment, "{") && parseMembers(json_segment, json_object) && parseMatch(json_segment, "}"))
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseMembers(std::string_view &json_segment, std::unique_ptr<Element> &json_object)
    {
        const std::string_view base_segment(json_segment);

        json_object = std::unique_ptr<Element>(new Object);
        std::unique_ptr<Element> key;
        std::unique_ptr<Element> value;
        while (parseMember(json_segment, key, value))
        {
            const std::string key_string(key->as<String>()->getString());
            if (json_object->as<Object>()->contains(key_string))
            {
                json_segment = base_segment;
                return false;
            }
            json_object->as<Object>()->insert_or_assign(key_string, std::unique_ptr<Element>(value.release()));
            if (!parseMatch(json_segment, ","))
                break;
        }
        if (!json_object->as<Object>()->empty())
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseMember(std::string_view &json_segment, std::unique_ptr<Element> &key, std::unique_ptr<Element> &json_value)
    {
        const std::string_view base_segment(json_segment);

        if (parseWS(json_segment) && parseString(json_segment, key) && parseWS(json_segment) && parseMatch(json_segment, ":") && parseElement(json_segment, json_value))
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseArray(std::string_view &json_segment, std::unique_ptr<Element> &json_array)
    {
        const std::string_view base_segment(json_segment);

        if (parseMatch(json_segment, "[") && parseWS(json_segment) && parseMatch(json_segment, "]"))
        {
            json_array = std::unique_ptr<Element>(new Array);
            return true;
        }

        json_segment = base_segment;
        if (parseMatch(json_segment, "[") && parseElements(json_segment, json_array) && parseMatch(json_segment, "]"))
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseElements(std::string_view &json_segment, std::unique_ptr<Element> &json_array)
    {
        const std::string_view base_segment(json_segment);

        json_array = std::unique_ptr<Element>(new Array);
        std::unique_ptr<Element> element;
        while (parseElement(json_segment, element))
        {
            json_array->as<Array>()->push_back(std::unique_ptr<Element>(element.release()));
            if (!parseMatch(json_segment, ","))
                break;
        }
        if (!json_array->as<Array>()->empty())
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseElement(std::string_view &json_segment, std::unique_ptr<Element> &json_element)
    {
        const std::string_view base_segment(json_segment);

        if (parseWS(json_segment) && parseValue(json_segment, json_element) && parseWS(json_segment))
            return true;

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseString(std::string_view &json_segment, std::unique_ptr<Element> &json_string)
    {
        const std::string_view base_segment(json_segment);

        std::string json_characters;
        if (parseMatch(json_segment, "\"") && parseStringCharacters(json_segment, json_characters) && parseMatch(json_segment, "\""))
        {
            json_string = std::unique_ptr<Element>(new String(json_characters));
            return true;
        }

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseStringCharacters(std::string_view &json_segment, std::string &json_characters)
    {
        const std::string_view base_segment(json_segment);

        bool issue = false;
        while (!issue && !json_segment.empty())
        {
            size_t byte_count;
            char32_t utf8_codepoint;

            if (!Unicode::convertUTF8ToCodepoint(json_segment, utf8_codepoint, byte_count))
                break;

            switch (utf8_codepoint)
            {
            case '"':
                // ends the string of characters
                return true;

            case '\\':
            {
                // escape sequence
                json_segment = json_segment.substr(1);
                if (!parseStringEscape(json_segment, json_characters))
                    issue = true;
                break;
            }

            default:
                if (Unicode::convertCodepointToUTF8(utf8_codepoint, json_characters))
                    json_segment = json_segment.substr(byte_count);
                else
                    issue = true;
                break;
            }
        }

        json_segment = base_segment;
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseStringEscape(std::string_view &json_segment, std::string &json_characters)
    {
        if (json_segment.empty())
            return false;

        switch (json_segment[0])
        {
        case '"':
            json_characters += '"';
            json_segment = json_segment.substr(1);
            return true;
        case '\\':
            json_characters += '\\';
            json_segment = json_segment.substr(1);
            return true;
        case '/':
            json_characters += '/';
            json_segment = json_segment.substr(1);
            return true;
        case 'b':
            json_characters += '\b';
            json_segment = json_segment.substr(1);
            return true;
        case 'f':
            json_characters += '\f';
            json_segment = json_segment.substr(1);
            return true;
        case 'n':
            json_characters += '\n';
            json_segment = json_segment.substr(1);
            return true;
        case 'r':
            json_characters += '\r';
            json_segment = json_segment.substr(1);
            return true;
        case 't':
            json_characters += '\t';
            json_segment = json_segment.substr(1);
            return true;
        case 'u':
        {
            const std::string_view hex_segment(json_segment.substr(1));
            if (hex_segment.length() < 4)
                return false;
            if (!(isxdigit(hex_segment[0]) && isxdigit(hex_segment[1]) && isxdigit(hex_segment[2]) && isxdigit(hex_segment[3])))
                return false;
            uint32_t accum = 0;
            for (int i = 0; i < 4; ++i)
            {
                accum *= 16;
                if ('0' <= hex_segment[i] && hex_segment[i] <= '9')
                    accum += static_cast<uint8_t>(hex_segment[i] - '0');
                else if ('a' <= hex_segment[i] && hex_segment[i] <= 'f')
                    accum += static_cast<uint8_t>(hex_segment[i] - 'a' + 10);
                else if ('A' <= hex_segment[i] && hex_segment[i] <= 'F')
                    accum += static_cast<uint8_t>(hex_segment[i] - 'A' + 10);
            }
            if (!Unicode::convertCodepointToUTF8(static_cast<char32_t>(accum), json_characters))
                return false;

            json_segment = hex_segment.substr(4);
            return true;
        }
        }

        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseNumber(std::string_view &json_segment, std::unique_ptr<Element> &json_number)
    {
        static const std::regex number_pattern("^([-])?(0|[1-9][0-9]*)(?:[.](0|[1-9][0-9]*))?(?:[Ee]([-+]?(?:0|[1-9][0-9]*)))?");
        std::cmatch matches;
        if (std::regex_search(json_segment.data(), matches, number_pattern))
        {
            const float sign = (matches[1].matched ? -1.0f : 1.0f);
            const float integer = (matches[2].matched ? std::stof(matches[2].str()) : 0.0f);
            const float fractional = (matches[3].matched ? std::stof(std::string("0.") + matches[3].str()) : 0.0f);
            const float exponent = (matches[4].matched ? std::pow(10.0f, std::stof(matches[4].str())) : 1);
            json_segment = json_segment.substr(matches.length());
            json_number = std::unique_ptr<Element>(new Number(sign * (integer + fractional) * exponent));
            return true;
        }
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseBool(std::string_view &json_segment, std::unique_ptr<Element> &json_bool)
    {
        if (parseMatch(json_segment, "true"))
        {
            json_bool = std::unique_ptr<Element>(new Bool(true));
            return true;
        }
        else if (parseMatch(json_segment, "false"))
        {
            json_bool = std::unique_ptr<Element>(new Bool(false));
            return true;
        }
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseNull(std::string_view &json_segment, std::unique_ptr<Element> &json_null)
    {
        if (parseMatch(json_segment, "null"))
        {
            json_null = std::unique_ptr<Element>(new Null);
            return true;
        }
        return false;
    }
    SOFTLOQ_JSON_API const bool Decoder::parseWS(std::string_view &json_segment)
    {
        while (!json_segment.empty())
            switch (json_segment[0])
            {
            case 0x20:
            case 0xA:
            case 0xD:
            case 0x9:
                json_segment = json_segment.substr(1);
                break;
            }
        return true;
    }
}