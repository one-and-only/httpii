#include "httpii_filesystem.h"
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <fat.h>
#include <fstream>
#include <cstring>
#include "utils.h"

using HTTPii::Utils::hash;

Filesystem::Filesystem()
{
    if (!fatInitDefault())
        die("Unable to initialise FAT subsystem, exiting...\n");
    if (!can_open_webroot())
        die("Unable to open root filesystem, exiting...\n");
    if (chdir("/"))
        die("Could not change to root directory, exiting...\n");
}

Filesystem::~Filesystem()
{
    fatUnmount(0);
}

Filesystem::ErrorMap Filesystem::GetError(std::string readFileRet) {
    switch (hash(readFileRet.c_str(), strlen(readFileRet.c_str()))) {
        case hash("FS_ERR_NOT_FOUND", strlen("FS_ERR_NOT_FOUND")):
            return Filesystem::ErrorMap::NOT_FOUND;
        case hash("FS_CREATED", strlen("FS_CREATED")):
            return Filesystem::ErrorMap::CREATED;
        case hash("FS_FORBIDDEN", strlen("FS_FORBIDDEN")):
            return Filesystem::ErrorMap::FORBIDDEN;
        case hash("FS_ERR_INTERNAL", strlen("FS_ERR_INTERNAL")):
            return Filesystem::ErrorMap::INTERNAL_ERROR;
        default:
            return Filesystem::ErrorMap::OK;
    }
}

std::string Filesystem::GetErrorString(Filesystem::ErrorMap err) {
    switch (err) {
        case Filesystem::ErrorMap::OK:
            return "OK";
        case Filesystem::ErrorMap::CREATED:
            return "File Created";
        case Filesystem::ErrorMap::NOT_FOUND:
            return "404 Not Found";
        case Filesystem::ErrorMap::FORBIDDEN:
            return "403 Forbidden";
        case Filesystem::ErrorMap::INTERNAL_ERROR:
            return "500 Internal Server Error";
        default:
            return "Invalid Error Code";
    }
}

std::string Filesystem::readFileFromWebroot(std::string filePath)
{
    std::string line;
    std::string ret;
    std::ifstream file("/apps/HTTPii/webroot/" + filePath);
    if (!file)
        return "FS_ERR_NOT_FOUND";
    while (file)
    {
        std::getline(file, line);
        ret += line + "\n";
    }
    file.close();
    return ret;
}

bool Filesystem::can_open_webroot()
{
    DIR *webroot = opendir("/apps/HTTPii/webroot");
    if (webroot)
    {
        closedir(webroot);
        return true;
    }
    return false;
}

void Filesystem::die(const char *msg, int sleepSec)
{
    perror(msg);
    sleep(5);
    fatUnmount(0);
    exit(0);
}