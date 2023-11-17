#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

// 패킷이 캡처 될때마다 호출되는 함수
void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet){
    printf("Packet capture: length %d\n", header->len);
}

int main(){
    //에러 메시지를 저장하기 위한 버퍼와 네트워크 장치 리스트를 위한 포인터선언
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;

    // libpcap 핸들을 위한 포인터 선언
    //이 헨들은 네트워크 장치를 열고 패킷을 캡처하는데 사용
    pcap_t *handle;


    //사용 가능한 모든 네트워크 장치를 찾습니다. 만약 에러가 발생함녀 메시지를 출력하고 프로그램을 종료
    if(pcap_findalldevs(&alldevs, errbuf) < 0){
        fprintf(stderr, "Error in pcap_findalldevs: %s\n",errbuf);
        return 1;
    }

    // 첫 번째 네트워크 장치를 선택합니다. 장치가 없으면 에러메시지 출력하고 프로그램 종료
    device = alldevs;
    if (device == NULL) {
        printf("No devices found.\n");
        return 1;
    }

    // 선택된 네트워크 장치를 열고 패킷 캡처를 준비합니다,
    // 실패하면 에러 메시지를 출력하고 프로그램을 종료
    handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device->name, errbuf);
        return 1;
    }

    // pcap_loop 함수를 사용하여 무한 루프에서 패킷을 캡처하고 packet_handler 함수 종료
    pcap_loop(handle, 0, packet_handler, NULL);

    // 사용이 끝난 후 모든 장치를 해제하고 핸들을 닫는다
    pcap_freealldevs(alldevs);
    pcap_close(handle);

    return 0;
}