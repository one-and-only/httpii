#ifndef HTTPII_FILESYSTEM_H
#define HTTPII_FILESYSTEM_H

#include <string>

class Filesystem
{
public:
    Filesystem();
    ~Filesystem();
    std::string readFileFromWebroot(std::string filePath);
    enum class ErrorMap {
        OK = 200,
        CREATED = 201,
        NOT_FOUND = 404,
        INTERNAL_ERROR = 500,
        FORBIDDEN = 403
    };
    ErrorMap GetError(std::string readFileRet);
    std::string GetErrorString(ErrorMap err);

private:
    bool can_open_webroot();
    void die(const char *msg = "An unknown error occurred, exiting...", int sleepSec = 3);
};

#endif //! HTTPII_FILESYSTEM_H