// Nathan Hinton

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <cstring>

#include "lwgps/lwgps.h"
#include "lwrb/lwrb.h"

#define GPS_UART_TX 0
#define GPS_UART_RX 1

#define BUFFSIZE 800

// #define PICO_DEFAULT_UART_TX_PIN GPS_UART_RX
// #define PICO_DEFAULT_UART_RX_PIN GPS_UART_TX

#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif

#define GPS_BAUD_RATE 9600
//Baud rate
#define SET_NMEA_BAUDRATE           "$PMTK251"
#define SET_NMEA_BAUDRATE_115200    "$PMTK251,115200"
#define SET_NMEA_BAUDRATE_57600     "$PMTK251,57600"
#define SET_NMEA_BAUDRATE_38400     "$PMTK251,38400"
#define SET_NMEA_BAUDRATE_19200     "$PMTK251,19200"
#define SET_NMEA_BAUDRATE_14400     "$PMTK251,14400"
#define SET_NMEA_BAUDRATE_9600      "$PMTK251,9600"
#define SET_NMEA_BAUDRATE_4800      "$PMTK251,4800"

#define SET_NMEA_OUTPUT "$PMTK314,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0"

#define SET_POS_FIX_400MS   "$PMTK220,400"

#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_Q_RELEASE "$PMTK605*31"

/* GPS handle */
lwgps_t hgps;

/* GPS buffer */
lwrb_t hgps_buff;
uint8_t hgps_buff_data[12];

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

const char
gps_rx_data[] = ""
                "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
                "$GPRMB,A,,,,,,,,,,,,V*71\r\n"
                "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
                "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
                "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
                "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
                "$PGRME,22.0,M,52.9,M,51.0,M*14\r\n"
                "$GPGLL,3907.360,N,12102.481,W,183730,A*33\r\n"
                "$PGRMZ,2062,f,3*2D\r\n"
                "$PGRMM,WGS84*06\r\n"
                "$GPBOD,,T,,M,,*47\r\n"
                "$GPRTE,1,1,c,0*07\r\n"
                "$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67\r\n"
                "$GPRMB,A,,,,,,,,,,,,V*71\r\n";
char buff_t [BUFFSIZE];
char const Temp[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

typedef struct {
	double Lon;     //GPS Latitude and longitude
	double Lat;
    UBYTE Lon_area;
    UBYTE Lat_area;
    UBYTE Time_H;   //Time
    UBYTE Time_M;
    UBYTE Time_S;
    UBYTE Status;   //1:Successful positioning 0：Positioning failed
}GNRMC;

GNRMC GPS;

static size_t write_ptr;
static void uart_irqhandler(void);
GNRMC L76X_Gat_GNRMC(void);
void L76X_Send_Command(char *data);
void DEV_Uart_SendByte(char data);
void DEV_Uart_SendString(char *data);

int main() {
    stdio_init_all();
    stdio_usb_connected();

    sleep_ms(5000);

    uint8_t rx;

    /* Init GPS */
    lwgps_init(&hgps);

    /* Process all input data */
    //lwgps_process(&hgps, gps_rx_data, strlen(gps_rx_data));

    /* Create buffer for received data */
    lwrb_init(&hgps_buff, hgps_buff_data, sizeof(hgps_buff_data));

    // /* Print messages */
    // printf("Valid status: %d\r\n", hgps.is_valid);
    // printf("Latitude: %f degrees\r\n", hgps.latitude);
    // printf("Longitude: %f degrees\r\n", hgps.longitude);
    // printf("Altitude: %f meters\r\n", hgps.altitude);


    //L76X_Exit_BackupMode();

    sleep_ms(5000);
    std::cout << "Printing setup info:" << std::endl;
    std::cout << "Init: " << uart_init(uart0, GPS_BAUD_RATE) << std::endl;
    std::cout << "is uart0 enabled: " << uart_is_enabled(uart0) << std::endl;
    std::cout << "uart instance: " << uart_get_index(uart0) <<std::endl;
    // std::cout << "Sending commands to get info from the module:" << std::endl;
    // uart_puts(uart0, PMTK_SET_NMEA_UPDATE_10HZ);
    // uart_puts(uart0, PMTK_SET_NMEA_OUTPUT_ALLDATA);
    // uart_puts(uart0, PMTK_Q_RELEASE);
    // std::string gps_line;
    gpio_init(0);
    gpio_init(1);
    gpio_set_function(0, GPIO_FUNC_UART); //TX
    gpio_set_function(1, GPIO_FUNC_UART); //RX
    std::cout << "Now waiting to get data from the uart:" << std::endl;
    sleep_ms(1000);

    printf("Set the output data interval to 400ms\r\n");
    //L76X_Send_Command(SET_POS_FIX_400MS);

    //Set output message
    L76X_Send_Command(SET_NMEA_OUTPUT);

    //printf("Change the L76X output baud rate to 115200 \r\n");
    //L76X_Send_Command(SET_NMEA_BAUDRATE_115200);
    sleep_ms(100);

    while (true) {
        // Pass through the gps strings to the computer
        //std::cout << "Got: " << uart_getc(uart0) << std::endl;
        // uart_put(uart0, 'c');

        // sleep_ms(100);
                /* Add new character to buffer */
        /* Fake UART interrupt handler on host microcontroller */
        uart_irqhandler();

        /* Process all input data */
        /* Read from buffer byte-by-byte and call processing function */
        if (lwrb_get_full(&hgps_buff)) {        /* Check if anything in buffer now */
            while (lwrb_read(&hgps_buff, &rx, 1) == 1) {
                lwgps_process(&hgps, &rx, 1);   /* Process byte-by-byte */
            }
        } else {
            /* Print all data after successful processing */
            printf("\n--------------------------------------\nData revceived\n-----------------------------------\n");
            printf("Data received :\n");
            printf("%s\n", buff_t);
            printf("From lwGPS : \n");
            printf("        Latitude: %f degrees\r\n", hgps.latitude);
            printf("        Longitude: %f degrees\r\n", hgps.longitude);
            printf("        Altitude: %f meters\r\n", hgps.altitude);
            //break;
            GPS = L76X_Gat_GNRMC();
            printf("From Wavshare parser :\n");
            printf("          Time: %d:%d:%d \r\n", GPS.Time_H, GPS.Time_M, GPS.Time_S);
            printf("          Latitude and longitude: %lf  %c  %lf  %c\r\n", GPS.Lat, GPS.Lat_area, GPS.Lon, GPS.Lon_area);
            sleep_ms(10000);
        }
    }

}

// static void
// uart_irqhandler(void) {
//     /* Make interrupt handler as fast as possible */
//     /* Only write to received buffer and process later */
//     printf("Data written to buffer\n");
//     if (write_ptr < strlen(gps_rx_data)) {
//         /* Write to buffer only */

//         lwrb_write(&hgps_buff, &gps_rx_data[write_ptr], 1);
//         ++write_ptr;
//     }
// }

static void uart_irqhandler(void) {
    /* Make interrupt handler as fast as possible */
    /* Only write to received buffer and process later */
    if (write_ptr < BUFFSIZE) {
        /* Write to buffer only */
        buff_t[write_ptr] = uart_getc(uart0);
        //printf("Datawritten : %c\n", buff_t[write_ptr]);
        lwrb_write(&hgps_buff, &buff_t[write_ptr], 1);
        ++write_ptr;
    }
}

/******************************************************************************
function:
	Analyze GNRMC data in L76x, latitude and longitude, time
******************************************************************************/
GNRMC L76X_Gat_GNRMC()
{
    UWORD add = 0, x = 0, z = 0, i = 0;
    UDOUBLE Time = 0, latitude = 0, longitude = 0;

    GPS.Status = 0;

	GPS.Time_H = 0;
    GPS.Time_M = 0;
    GPS.Time_S = 0;

    //DEV_Uart_ReceiveString(buff_t, BUFFSIZE);
    //printf("%s\r\n", buff_t);
    add = 0;
    while(add < BUFFSIZE-71){
        if(buff_t[add] == '$' && buff_t[add+1] == 'G' && (buff_t[add+2] == 'N' || buff_t[add+2] == 'P')\
            && buff_t[add+3] == 'R' && buff_t[add+4] == 'M' && buff_t[add+5] == 'C'){
            x = 0;
            for(z = 0; x < 12; z++){
                if(buff_t[add+z]=='\0'){
                    break;
                }
                if(buff_t[add+z]==','){
                    x++;
                    if(x == 1){//The first comma is followed by time
                        Time = 0;
                        for(i = 0; buff_t[add+z+i+1] != '.'; i++){
                            if(buff_t[add+z+i+1]=='\0'){
                                break;
                            }
                            if(buff_t[add+z+i+1] == ',')
                                break;
                            Time = (buff_t[add+z+i+1]-'0') + Time*10;
                        }

                        GPS.Time_H = Time/10000+8;
                        GPS.Time_M = Time/100%100;
                        GPS.Time_S = Time%100;
                        if(GPS.Time_H >= 24)
                            GPS.Time_H = GPS.Time_H - 24;
                    }else if(x == 2){
                     //A indicates that it has been positioned
                     //V indicates that there is no positioning.
                        if(buff_t[add+z+1] == 'A'){
                             GPS.Status = 1;
                        }else{
                             GPS.Status = 0;
                        }
                    }else if(x == 3){
                        latitude = 0;
                        //If you need to modify, please re-edit the calculation method below.
                        for(i = 0; buff_t[add+z+i+1] != ','; i++){
                            if(buff_t[add+z+i+1] == '\0'){
                                break;
                            }
                            if(buff_t[add+z+i+1] == '.'){
                                continue;
                            }
                            latitude = (buff_t[add+z+i+1]-'0') + latitude*10;
                        }
                        GPS.Lat = latitude/1000000.0;
                    }else if(x == 4){
                        GPS.Lat_area = buff_t[add+z+1];
                    }
                    else if(x == 5){
                        longitude = 0;
                        for(i = 0; buff_t[add+z+i+1] != ','; i++){
                            if(buff_t[add+z+i+1] == '\0'){
                                break;
                            }
                            if(buff_t[add+z+i+1] == '.')
                                continue;
                            longitude = (buff_t[add+z+i+1]-'0') + longitude*10;
                        }
                        GPS.Lon = longitude/1000000.0;
                    }else if(x == 6){
                        GPS.Lon_area = buff_t[add+z+1];
                    }
                }
            }
            add = 0;
            break;
        }
        if(buff_t[add+5] == '\0'){
            add = 0;
						break;
        }
        add++;
        if(add > BUFFSIZE){
            add = 0;
            break;
        }
    }
    return GPS;
}

void L76X_Send_Command(char *data)
{
    char Check = data[1], Check_char[3]={0};
    UBYTE i = 0;
    //printf(" 1i = %d Check =%x \n", i, Check);
    for(i=2; data[i] != '\0'; i++){
        Check ^= data[i];       //Calculate the check value
    }
    //printf(" i = %d Check =%x \n", i, Check);
    Check_char[0] = Temp[Check/16%16];
    Check_char[1] = Temp[Check%16];
		Check_char[2] = '\0';

    DEV_Uart_SendString(data);
    DEV_Uart_SendByte('*');
    DEV_Uart_SendString(Check_char);
    DEV_Uart_SendByte('\r');
    DEV_Uart_SendByte('\n');
    sleep_ms(200);
}

void DEV_Uart_SendByte(char data)
{
    uart_putc(uart0, data);
}

void DEV_Uart_SendString(char *data)
{
    UWORD i;
    for(i = 0; data[i] != '\0'; i++){
        uart_putc(uart0, data[i]);
    }

}
