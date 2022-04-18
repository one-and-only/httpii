#ifndef HTTPII_NETWORK_H
#define HTTPII_NETWORK_H
#include <network.h>
#include <string>
#include <ogcsys.h>
namespace HTTPii::HTTPd
{
    static char LISTENING_HOST[16] = "0.0.0.0";
    constexpr static u16 LISTENING_PORT = 80;

    /**
     * @brief Start httpd thread
     * 
     */
    extern void start();
    /**
     * @brief httpd code
     * @details This is the main entry point for the httpd thread
     */
    extern void *httpd_server_thread(void *arg);
}

#endif //! HTTPII_NETWORK_H