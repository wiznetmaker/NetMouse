#include <stdio.h>
#include "pico/multicore.h"
#include <string.h>
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "mode0/mode0.h"
#include "port_common.h"
#include "wizchip_conf.h"
#include "w5x00_spi.h"
#include "socket.h"
#include "ili9341.h"


#define SERVER_IP {192, 168, 11, 197} // 서버 IP 주소
#define SERVER_PORT 5000 // 서버 포트 번호
#define CLIENT_SOCKET 1 // 클라이언트 소켓 번호
#define PLL_SYS_KHZ (133 * 1000)
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)
#define MAX_TEXT 100  // 최대 문자 수

static wiz_NetInfo g_net_info = {
    .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x57},
    .ip = {192, 168, 11, 3},
    .sn = {255, 255, 255, 0},
    .gw = {192, 168, 11, 1},
    .dns = {8, 8, 8, 8},
    .dhcp = NETINFO_STATIC
};

static void set_clock_khz(void) {
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        PLL_SYS_KHZ * 1000,
        PLL_SYS_KHZ * 1000
    );
}


static uint8_t g_buf[1024] = {0,};

// core1에서 실행될 함수
static void display_received_data_core1() {
    ili9341_init(); // Initialize the mode0 library
    mode0_set_cursor(0, 0);
    mode0_set_foreground(MODE0_WHITE);  // Set default text color to white
    mode0_set_background(MODE0_BLACK);  // Set default background color to black

    while (true) {
        char* received_data = (char*)multicore_fifo_pop_blocking();

        // 받은 데이터를 LCD에 출력
        mode0_print(received_data);
        mode0_print("\n");
    }
}

static void send_message_to_core1(const char* message) {
    multicore_fifo_push_blocking((uint32_t)message);
}

void adc_init_pins() {
    adc_init();
    adc_gpio_init(26); // GP26을 ADC0으로 초기화
    adc_gpio_init(27); // GP27을 ADC1으로 초기화
}

// ADC 값을 읽고 서버로 전송하는 함수
void send_adc_values_to_server(uint8_t sn, uint8_t* buf) {
    adc_select_input(0); // ADC0 선택
    uint16_t adc_value0 = adc_read(); // ADC0 값 읽기

    adc_select_input(1); // ADC1 선택
    uint16_t adc_value1 = adc_read(); // ADC1 값 읽기

    // 값 포매팅 및 버퍼에 저장
    int len = snprintf((char*)buf, ETHERNET_BUF_MAX_SIZE, "ADC0: %u, ADC1: %u\n", adc_value0, adc_value1);

    // 서버로 데이터 전송
    send(sn, buf, len);
}

static void connect_to_server(uint8_t sn, uint8_t* buf, uint8_t* server_ip, uint16_t port) {
    int32_t ret;
    static bool isConnected = false;

    switch(getSn_SR(sn)) {
        case SOCK_INIT:
            if ((ret = connect(sn, server_ip, port)) != SOCK_OK) {
                if (!isConnected) {
                    send_message_to_core1("No connection\n");
                    isConnected = false;
                }
                return;
            }
            break;
        case SOCK_ESTABLISHED:
            if (!isConnected) {
                send_message_to_core1("Server Connected\n");
                isConnected = true;
            }

            while (isConnected) { // 서버 연결이 유지되는 동안 반복
                send_adc_values_to_server(sn, buf);

                // 서버로 데이터 전송
                if ((ret = send(sn, buf, strlen((char*)buf))) <= 0) {
                    close(sn);
                    isConnected = false;
                    break;
                }
                sleep_ms(5); // 데이터 전송 간격 (1초)
            }
            break;
        case SOCK_CLOSED:
            if (isConnected) {
                send_message_to_core1("Lost connection\n");
                isConnected = false;
            }
            if ((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) {
                return;
            }
            break;
    }
}

int main() {
    set_clock_khz();
    stdio_init_all();
	adc_init_pins();
    // Core1에서 display_received_data_core1 함수 실행
    multicore_launch_core1(display_received_data_core1); 
    send_message_to_core1("IP : 192.168.11.3\n");
    sleep_ms(10);
    memset(g_buf, 0, sizeof(g_buf)); // g_buf 초기화
    send_message_to_core1("PORT : 5000\n");
    sleep_ms(10);
    memset(g_buf, 0, sizeof(g_buf)); // g_buf 초기화

    wizchip_spi_initialize();
    wizchip_cris_initialize();
    wizchip_reset();
    wizchip_initialize();
    wizchip_check();

    network_initialize(g_net_info);


    
    uint8_t server_ip[] = SERVER_IP;

    while (1) {

        connect_to_server(CLIENT_SOCKET, g_buf, server_ip, SERVER_PORT);
		send_adc_values_to_server(CLIENT_SOCKET, g_buf);
    }
}