#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet){
    printf("Packet capture: length %d\n", header->len);
}

int main(){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;
    pcap_t *handle;

    if(pcap_findalldevs(&alldevs, errbuf) < 0){
        fprintf(stderr, "Error in pcap_findalldevs: %s\n",errbuf);
        return 1;
    }

    device = alldevs;

    if (device == NULL) {
        printf("No devices found.\n");
        return 1;
    }

    handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device->name, errbuf);
        return 1;
    }

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_freealldevs(alldevs);
    pcap_close(handle);

    return 0;
}