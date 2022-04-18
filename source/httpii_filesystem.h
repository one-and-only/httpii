#ifndef HTTPII_FILESYSTEM_H
#define HTTPII_FILESYSTEM_H

#include <string>

class Filesystem
{
public:
    Filesystem();
    ~Filesystem();
    std::string readFileFromWebroot(std::string filePath);

private:
    bool can_open_webroot();
    void die(const char *msg = "An unknown error occurred, exiting...", int sleepSec = 3);
};

#endif //! HTTPII_FILESYSTEM_H