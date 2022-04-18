#include "httpii_httpd_header.h"

namespace HTTPii::HTTPd {
    Header::Header(std::string key, std::string value) {
        this->key = key;
        this->value = value;
    }

    std::string Header::toHTTP() {
        return this->key + ": " + this->value;
    }

    std::string Header::getKey() {
        return this->key;
    }

    std::string Header::getValue() {
        return this->value;
    }
}