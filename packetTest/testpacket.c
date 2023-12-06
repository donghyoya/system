#include <pcap.h>
#include <stdio.h>

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // 네트워크 장치 선택
    char *dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "장치를 찾을 수 없음: %s\n", errbuf);
        return 2;
    }

    // 네트워크 장치 열기
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "장치 %s를 열 수 없음: %s\n", dev, errbuf);
        return 2;
    }

    // 패킷 처리 루프
    // ...

    pcap_close(handle);
    return 0;
}
