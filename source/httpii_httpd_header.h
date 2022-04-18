#ifndef HTTPII_HTTPD_HEADER_H
#define HTTPII_HTTPD_HEADER_H
#include <string>

namespace HTTPii::HTTPd
{
    /**
     * @brief Defines an HTTP Header
     * @details this class defines an HTTP Header, used in both request parsing and response generation. A header consists of a key and a value, which can then be formatted to the HTTP standard of `Header: Value`.
     */
    class Header
    {
    public:
        /**
         * @brief Header key/label
         */
        std::string key;
        /**
         * @brief Header value/data
         */
        std::string value;

        /**
         * @brief Construct a new Header
         * 
         * @param key Header key (see declaration above)
         * @param value Header value (see declaration above)
         */
        Header(std::string key, std::string value);
        /**
         * @brief Return HTTP-formatted header string
         * This method returns a formatted string as per the HTTP standard of `Header: Value`
         * @return std::string Formatted header string
         */
        std::string toHTTP();
        /**
         * @brief Get Header key
         * 
         * @return std::string 
         */
        std::string getKey();
        /**
         * @brief Get Header value
         * 
         * @return std::string 
         */
        std::string getValue();
    };
}

#endif //! HTTPII_HTTPD_HEADER_H