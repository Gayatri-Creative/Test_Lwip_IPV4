#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "lwip/ip_addr.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void start_tcp_client(ip_addr_t *server_ip, uint16_t server_port);
bool tcp_client_is_connected(void);
void tcp_client_poll_reconnect_ipv4(ip_addr_t *server_ip, uint16_t server_port);

#ifdef __cplusplus
}
#endif

#endif // TCP_CLIENT_H
