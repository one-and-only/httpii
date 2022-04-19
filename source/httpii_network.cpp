#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "utils.h"
#include "httpii_network.h"
#include "httpii_httpd_header.h"
#include "httpii_filesystem.h"

static lwp_t serverThreadHandle = (lwp_t)NULL;
namespace HTTPii::HTTPd
{
    void start()
    {
        // initialize the Wii's network connection
        HTTPii::Utils::clear_screen();
        printf("Initializing network...\n");

        s32 ret = if_config(LISTENING_HOST, new char[16]{0}, new char[16]{0}, true, 20);
        if (ret < 0)
        {
            printf("Failed to initialize network\n");
            return;
        }

        LWP_CreateThread(&serverThreadHandle, /* thread handle */
                         httpd_server_thread, /* code */
                         LISTENING_HOST,      /* arg pointer for thread */
                         NULL,                /* stack base */
                         16 * 1024,           /* stack size */
                         50 /* thread priority */);
    }

    void *httpd_server_thread(void *arg)
    {
        s32 sock, csock, ret;
        Filesystem *fs = new Filesystem();
        struct sockaddr_in client;
        u32 clientlen = sizeof(client);
        struct sockaddr_in server;
        char temp[1026];

        printf("Creating server socket...\n");
        sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

        memset(&server, 0, sizeof(server));
        memset(&client, 0, sizeof(client));

        server.sin_family = AF_INET;
        server.sin_port = htons(LISTENING_PORT);
        server.sin_addr.s_addr = INADDR_ANY;

        printf("Binding server socket...\n");
        ret = net_bind(sock, (struct sockaddr *)&server, sizeof(server));
        if (sock == INVALID_SOCKET)
        {
            printf("Unable to create socket, terminating web server thread.\n");
            return NULL;
        }

        if (ret)
        {
            printf("Error %d binding socket, terminating web server thread.\n", ret);
            return NULL;
        }
        else
        {
            printf("Attempting to listen on server socket...\n");
            if ((ret = net_listen(sock, 5)))
            {
                printf("Error %d listening, terminating web server thread.\n", ret);
                return NULL;
            }
            else
            {
                HTTPii::Utils::clear_screen();
                const u32 ip = net_gethostip();
                printf("Successfully initialized server!\n\n");
                printf("Now listening on %u.%u.%u.%u:%u\n",
                       (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff,
                       LISTENING_PORT);

                while (1)
                {
                    std::string res_data, res_status_code = "200 OK", res_status = "HTTP/1.1 " + res_status_code + "\r\n";
                    std::vector<Header *> res_headers;

                    csock = net_accept(sock, (struct sockaddr *)&client, &clientlen);

                    if (csock < 0)
                    {
                        printf("Error %d connecting socket, terminating web server thread.\n", csock);
                    }

                    memset(temp, 0, sizeof(temp));
                    ret = net_recv(csock, temp, sizeof(temp) - 2, 0);

                    res_headers.push_back(new Header("Content-Type", "text/html"));
                    res_headers.push_back(new Header("Connection", "close")); // we don't support keep-alive

                    const char *http_get_index = "GET / HTTP/1.1\r\n";
                    if (!strncmp(temp, http_get_index, strlen(http_get_index)))
                    {
                        printf("Data received:\n\n%s", temp);
                        res_data = fs->readFileFromWebroot("index.html");
                        const Filesystem::ErrorMap fileResCode = fs->GetError(res_data);
                        if (fileResCode != Filesystem::ErrorMap::OK) {
                            std::string errorString = fs->GetErrorString(fileResCode);
                            res_data = errorString;
                            res_status_code = errorString;
                            res_status = "HTTP/1.1 " + res_status_code + "\r\n";
                        }
                        net_send(csock, res_status.c_str(), strlen(res_status.c_str()), 0);
                        for (unsigned int i = 0; i < res_headers.size() - 1; i++)
                        {
                            std::string header = res_headers.at(i)->toHTTP() + "\r\n";
                            net_send(csock, header.c_str(), strlen(header.c_str()), 0);
                        }
                        // we need a double new line for the last header so the HTTP client knows when the "headers" section ends
                        std::string lastHeader = res_headers.at(res_headers.size() - 1)->toHTTP() + "\r\n\r\n";
                        net_send(csock, lastHeader.c_str(), strlen(lastHeader.c_str()), 0);
                        net_send(csock, res_data.c_str(), strlen(res_data.c_str()), 0);
                        net_close(csock);
                        continue;
                    }
                    // process other files other than the index page
                    net_close(csock);
                }
            }
        }
        return NULL;
    }
}