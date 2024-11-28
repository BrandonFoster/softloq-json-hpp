#ifndef SOFTLOQ_JSON_DECODER_HPP
#define SOFTLOQ_JSON_DECODER_HPP

/**
 * @author Brandon Foster
 * @file decoder.hpp
 * @version 1.0.0
 * @brief Contains the JSON Decoder Class.
 */

#include "softloq-json/element.hpp"

namespace Softloq::JSON
{
    /**
     * @brief Decoder converts JSON text into a modifiable object using several decode functions.
     * The error state of the decoder is based on the most recent decode function.
     */
    class SOFTLOQ_JSON_API Decoder
    {
    public:
        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Element object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Element or nullptr on failure.
         */
        const Element *decodeJSON(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Object object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Object or nullptr on failure.
         */
        const Object *decodeObject(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Array object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Array or nullptr on failure.
         */
        const Array *decodeArray(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON String object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON String or nullptr on failure.
         */
        const String *decodeString(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Number object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Number or nullptr on failure.
         */
        const Number *decodeNumber(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Bool object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Bool or nullptr on failure.
         */
        const Bool *decodeBool(const std::string &json_text);

        /**
         * @brief Converts the entire JSON text into a modifiable C++ JSON Null object.
         *
         * @param json_text The JSON text.
         * @return A pointer to the allocated JSON Null or nullptr on failure.
         */
        const Null *decodeNull(const std::string &json_text);

    private:
        const bool parseValue(std::string_view &json_segment, std::unique_ptr<Element> &json_value);
        const bool parseObject(std::string_view &json_segment, std::unique_ptr<Element> &json_object);
        const bool parseMembers(std::string_view &json_segment, std::unique_ptr<Element> &json_object);
        const bool parseMember(std::string_view &json_segment, std::unique_ptr<Element> &key, std::unique_ptr<Element> &json_value);
        const bool parseArray(std::string_view &json_segment, std::unique_ptr<Element> &json_array);
        const bool parseElements(std::string_view &json_segment, std::unique_ptr<Element> &json_array);
        const bool parseElement(std::string_view &json_segment, std::unique_ptr<Element> &json_element);
        const bool parseString(std::string_view &json_segment, std::unique_ptr<Element> &json_string);
        const bool parseStringCharacters(std::string_view &json_segment, std::string &json_characters);
        const bool parseStringEscape(std::string_view &json_segment, std::string &json_characters);
        const bool parseNumber(std::string_view &json_segment, std::unique_ptr<Element> &json_number);
        const bool parseBool(std::string_view &json_segment, std::unique_ptr<Element> &json_bool);
        const bool parseNull(std::string_view &json_segment, std::unique_ptr<Element> &json_null);
        const bool parseWS(std::string_view &json_segment);
    };
}

#endif