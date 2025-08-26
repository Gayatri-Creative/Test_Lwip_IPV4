#include "lwip/tcp.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include <string.h>
#include "Ifx_Lwip.h"
#include "Ifx_Types.h"
#include <stdbool.h>

#define TCP_CLIENT_RECONNECT_INTERVAL_MS 5000

static struct tcp_pcb *g_pcb_v4 = NULL;
static struct tcp_pcb *g_pcb_v6 = NULL;
static bool is_connected_v4 = false;
static bool is_connected_v6 = false;
static uint32_t lastReconnectAttempt_v4 = 0;
static uint32_t lastReconnectAttempt_v6 = 0;

/* -------------------- Shared Callback Logic -------------------- */

static void close_tcp_connection(struct tcp_pcb **pcb_ptr, bool *is_connected)
{
    Ifx_Lwip_printf("[TCP_CLIENT] Closing connection.\n");

    if (*pcb_ptr != NULL) {
        tcp_recv(*pcb_ptr, NULL);
        tcp_sent(*pcb_ptr, NULL);
        tcp_err(*pcb_ptr, NULL);
        tcp_arg(*pcb_ptr, NULL);
        tcp_close(*pcb_ptr);
    }

    *pcb_ptr = NULL;
    *is_connected = false;
}

static void on_tcp_error_v4(void *arg, err_t err)
{
    Ifx_Lwip_printf("[TCPv4] Error: %d\n", err);
    close_tcp_connection(&g_pcb_v4, &is_connected_v4);
}

static void on_tcp_error_v6(void *arg, err_t err)
{
    Ifx_Lwip_printf("[TCPv6] Error: %d\n", err);
    close_tcp_connection(&g_pcb_v6, &is_connected_v6);
}

static err_t on_tcp_recv_common(struct tcp_pcb *tpcb, struct pbuf *p, bool *is_connected)
{
    if (!p) {
        Ifx_Lwip_printf("[TCP_CLIENT] Server closed connection.\n");
        close_tcp_connection(&tpcb, is_connected);
        return ERR_OK;
    }

    char buffer[128] = {0};
    memcpy(buffer, p->payload, LWIP_MIN(p->tot_len, sizeof(buffer) - 1));
    Ifx_Lwip_printf("[TCP_CLIENT] Received: %s\n", buffer);

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

static err_t on_tcp_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    Ifx_Lwip_printf("[TCP_CLIENT] Data sent (%d bytes acknowledged)\n", len);
    return ERR_OK;
}

/* -------------------- IPv4 Client -------------------- */

static err_t on_tcp_recv_v4(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    return on_tcp_recv_common(tpcb, p, &is_connected_v4);
}

static err_t on_tcp_connected_v4(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if (err != ERR_OK) {
        Ifx_Lwip_printf("[TCPv4] Connection failed: err=%d\n", err);
        close_tcp_connection(&g_pcb_v4, &is_connected_v4);
        return err;
    }

    Ifx_Lwip_printf("[TCPv4] Connected to server!\n");
    is_connected_v4 = true;

    tcp_recv(tpcb, on_tcp_recv_v4);
    tcp_sent(tpcb, on_tcp_sent);

    const char *msg = "Hello from AURIX (IPv4)!\r\n";
    err_t write_err = tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
    if (write_err == ERR_OK) {
        tcp_output(tpcb);
    }

    return ERR_OK;
}

void start_tcp_client_ipv4(ip_addr_t *server_ip, uint16_t server_port)
{
    g_pcb_v4 = tcp_new();
    if (!g_pcb_v4) {
        Ifx_Lwip_printf("[TCPv4] Failed to create PCB\n");
        return;
    }

    tcp_err(g_pcb_v4, on_tcp_error_v4);
    err_t err = tcp_connect(g_pcb_v4, server_ip, server_port, on_tcp_connected_v4);

    if (err != ERR_OK) {
        Ifx_Lwip_printf("[TCPv4] tcp_connect() failed: err=%d\n", err);
        close_tcp_connection(&g_pcb_v4, &is_connected_v4);
    } else {
        Ifx_Lwip_printf("[TCPv4] Connecting to %s:%u\n", ipaddr_ntoa(server_ip), server_port);
    }
}

void tcp_client_poll_reconnect_ipv4(ip_addr_t *server_ip, uint16_t server_port)
{
    if (is_connected_v4) return;

    if (g_pcb_v4 == NULL) {
        uint32_t now = g_TickCount_1ms;

        if ((now - lastReconnectAttempt_v4) >= TCP_CLIENT_RECONNECT_INTERVAL_MS) {
            Ifx_Lwip_printf("[TCPv4] Attempting reconnect...\n");
            start_tcp_client_ipv4(server_ip, server_port);
            lastReconnectAttempt_v4 = now;
        }
    }
}



