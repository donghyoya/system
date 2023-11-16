#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>



int main(){
    pcap_if_t *alldevs;
    pcap_if_t *device;

    char errbuf[PCAP_ERRBUF_SIZE];
    int status = pcap_findalldevs(&alldevs, errbuf);

    if(pcap_findalldevs(&alldevs, errbuf) < 0){
        printf("pcap_findalldevs error\n");
        return 1;
    }

    if(!alldevs){
        printf("%s\n", errbuf);
    }

    for(d=alldevs; d; d=d -> next){
        printf("%p : %d %s",d, ++i, d->name);
        if(d->description) printf(" (%s)", d->description);
        printf("\n");
    }

    pcap_freealldevs(alldevs);

    return 0;
}