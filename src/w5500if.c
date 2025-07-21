//#include "lwipif.h"
//#include "lwip/init.h"
//#include "lwip/netif.h"
//#include "lwip/tcp.h"
//#include "lwip/udp.h"
//#include "lwip/dhcp.h"
//#include "netif/ethernet.h"
//#include "ethernetdrv.h"  // your PHY driver
//#include <string.h>
//
//static struct netif myNetif;
//
//void Network_Init(void) {
//    lwip_init();
//    ip4_addr_t ip = IPADDR4_INIT_BYTES(192,168,1,100);
//    ip4_addr_t gw = IPADDR4_INIT_BYTES(192,168,1,1);
//    ip4_addr_t nm = IPADDR4_INIT_BYTES(255,255,255,0);
//    netif_add(&myNetif, &ip, &nm, &gw, NULL, ethernetif_init, ethernet_input);
//    netif_set_default(&myNetif);
//    netif_set_up(&myNetif);
//    dhcp_start(&myNetif);
//}
//
//void Network_PeriodicTasks(void) {
//    sys_check_timeouts();
//    ethernetif_input(&myNetif);
//}
//
//int Network_Send(const uint8_t *data, int len, const char *ip, uint16_t port) {
//    struct tcp_pcb *pcb = tcp_new();
//    if (!pcb) return -1;
//    ip4_addr_t dst;
//    ip4addr_aton(ip, &dst);
//    if (tcp_connect(pcb, &dst, port, NULL) != ERR_OK) {
//        tcp_close(pcb);
//        return -2;
//    }
//    if (tcp_write(pcb, data, len, TCP_WRITE_FLAG_COPY) != ERR_OK) {
//        tcp_close(pcb);
//        return -3;
//    }
//    tcp_output(pcb);
//    tcp_close(pcb);
//    return 0;
//}
//
//int Network_Receive(uint8_t *buf, int maxlen) {
//    // Simplest polling stub; fill in with your recv callback logic
//    return 0;
//}
