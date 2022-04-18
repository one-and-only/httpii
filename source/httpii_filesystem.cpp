#include "httpii_filesystem.h"
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <fat.h>
#include <fstream>

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

std::string Filesystem::readFileFromWebroot(std::string filePath) {
    std::string line;
    std::string ret;
    std::ifstream file("/apps/HTTPii/webroot/" + filePath);
    if (!file) return "FS_ERR_NOT_FOUND";
    while (file) {
        std::getline(file, line);
        ret += line + "\n";
    }
    file.close();
    return ret;
}

bool Filesystem::can_open_webroot()
{
    DIR *webroot = opendir("/apps/HTTPii/webroot");
    if (webroot) {
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