// ********** test_gps code *************
// rev 1 - obenchekroun - Jan 2025
//


#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
//#include <cstring>

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

#define GPS_BAUD_RATE 115200
//#define GPS_BAUD_RATE 115200
//Baud rate
#define SET_NMEA_BAUDRATE_115200    "$PMTK251,115200"
#define SET_NMEA_OUTPUT             "$PMTK314,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0"
#define SET_NMEA_OUTPUT_ALL_DATA    "$PMTK314,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0"
#define SET_SYNC_PPS_NMEA_ON        "$PMTK255,1"
#define SET_NORMAL_MODE             "$PMTK225,0"

#define SET_POS_FIX_400MS   "$PMTK220,400"

// #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// #define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// #define PMTK_Q_RELEASE "$PMTK605*31"

/* GPS handle */
lwgps_t hgps;

/* GPS buffer */
lwrb_t hgps_buff;
uint8_t hgps_buff_data[12];

// const char
// gps_rx_data[] = ""
//                 "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
//                 "$GPRMB,A,,,,,,,,,,,,V*71\r\n"
//                 "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
//                 "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
//                 "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
//                 "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
//                 "$PGRME,22.0,M,52.9,M,51.0,M*14\r\n"
//                 "$GPGLL,3907.360,N,12102.481,W,183730,A*33\r\n"
//                 "$PGRMZ,2062,f,3*2D\r\n"
//                 "$PGRMM,WGS84*06\r\n"
//                 "$GPBOD,,T,,M,,*47\r\n"
//                 "$GPRTE,1,1,c,0*07\r\n"
//                 "$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67\r\n"
//                 "$GPRMB,A,,,,,,,,,,,,V*71\r\n";


char buff_t [BUFFSIZE];
char const hexCheck[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

const char* command_NMEA_OUTPUT = SET_NMEA_OUTPUT;
const char* command_NMEA_OUTPUT_ALL_DATA = SET_NMEA_OUTPUT_ALL_DATA;
const char* command_BAUDRATE_115200 = SET_NMEA_BAUDRATE_115200;
const char* command_SET_SYNC_PPS_NMEA_ON = SET_SYNC_PPS_NMEA_ON;
const char* command_SET_NORMAL_MODE = SET_NORMAL_MODE;


typedef struct {
	double Lon;     //GPS Latitude and longitude
	double Lat;
    uint8_t Lon_area;
    uint8_t Lat_area;
    uint8_t Time_H;   //Time
    uint8_t Time_M;
    uint8_t Time_S;
    uint8_t Status;   //1:Successful positioning 0：Positioning failed
}GNRMC;

GNRMC GPS;

static size_t write_ptr;
static void uart_irqhandler(void);
GNRMC L76X_Gat_GNRMC(void);
void L76X_send_command(char *data);
void DEV_Uart_SendByte(char data);
void DEV_Uart_SendString(char *data);

int main() {
    stdio_init_all();
    //stdio_usb_connected();

    sleep_ms(5000);

    uint8_t rx;

    /* Init GPS */
    lwgps_init(&hgps);

    /* Create buffer for received data */
    lwrb_init(&hgps_buff, hgps_buff_data, sizeof(hgps_buff_data));


    sleep_ms(5000);

    printf("Initialising UART0\n");
    uart_init(uart0, GPS_BAUD_RATE);
    if (!uart_is_enabled(uart0)) {
        printf("Failed to init uart0");
        return 1;
    }

    gpio_init(0);
    gpio_init(1);
    gpio_set_function(0, GPIO_FUNC_UART); //TX
    gpio_set_function(1, GPIO_FUNC_UART); //RX
    printf("Now waiting to get data from the uart:\n");
    //std::cout << "Now waiting to get data from the uart:" << std::endl;
    sleep_ms(1000);

    //printf("Set the output data interval to 400ms\r\n");
    //L76X_Send_Command(SET_POS_FIX_400MS);

    //Set output message
    L76X_send_command((char*)command_NMEA_OUTPUT);
    //L76X_send_command((char*)command_SET_SYNC_PPS_NMEA_ON);

    //printf("Change the L76X output baud rate to 115200 \r\n");
    //L76X_Send_Command(SET_NMEA_BAUDRATE_115200);
    sleep_ms(100);

    while (true) {
        //L76X_send_command((char*)command_SET_NORMAL_MODE);
        sleep_ms(200);
        /* Add new character to buffer */
        /* UART interrupt handler on host microcontroller */
        uart_irqhandler();

        /* Process all input data */
        /* Read from buffer byte-by-byte and call processing function */
        if (lwrb_get_full(&hgps_buff)) {        /* Check if anything in buffer now */
            while (lwrb_read(&hgps_buff, &rx, 1) == 1) {
                lwgps_process(&hgps, &rx, 1);   /* Process byte-by-byte */
            }
        } else {
            /* Print all data after successful processing */
            printf("\n--------------------------------------\nData received\n-----------------------------------\n");
            printf("Data received :\n");
            printf("%s\n", buff_t);
            printf("From lwGPS : \n");
            printf("        Valid status :%d\r\n", hgps.is_valid);
            printf("        Latitude: %f degrees\r\n", hgps.latitude);
            printf("        Longitude: %f degrees\r\n", hgps.longitude);
            printf("        Altitude: %f meters\r\n", hgps.altitude);
            //break;
            GPS = L76X_Gat_GNRMC();
            printf("From Waveshare parser :\n");
            printf("          Time: %d:%d:%d \r\n", GPS.Time_H, GPS.Time_M, GPS.Time_S);
            printf("          Latitude and longitude: %lf  %c  %lf  %c\r\n", GPS.Lat, GPS.Lat_area, GPS.Lon, GPS.Lon_area);
            sleep_ms(10000);
        }
    }

}

static void uart_irqhandler(void) {
    /* Make interrupt handler as fast as possible */
    /* Only write to received buffer and process later */
    if (write_ptr < BUFFSIZE) {
        /* Write to buffer only */
        buff_t[write_ptr] = uart_getc(uart0);
        //printf("Datawritten : %c\n", buff_t[write_ptr]);
        lwrb_write(&hgps_buff,&buff_t[write_ptr], 1);
        ++write_ptr;
    }
}

/******************************************************************************
function:
	Analyze GNRMC data in L76x, latitude and longitude, time
******************************************************************************/
GNRMC L76X_Gat_GNRMC()
{
    uint16_t add = 0, x = 0, z = 0, i = 0;
    uint32_t Time = 0, latitude = 0, longitude = 0;

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

void L76X_send_command(char *data)
{
    char check1i = data[1], check_char[3]={0};
    uint8_t i = 0;
    //printf(" 1i = %d Check =%x \n", i, check1i);
    for(i=2; data[i] != '\0'; i++){
        check1i ^= data[i];       //Calculate the check value
    }
    printf(" i = %d Check =%x \n", i, check1i);
    check_char[0] = hexCheck[check1i/16%16];
    check_char[1] = hexCheck[check1i%16];
	check_char[2] = '\0';

    uart_puts(uart0, data);
    uart_putc(uart0,'*');
    uart_puts(uart0, check_char);
    uart_putc(uart0,'\r');
    uart_putc(uart0,'\n');
    //DEV_Uart_SendString(data);
    //DEV_Uart_SendByte('*');
    //DEV_Uart_SendString(check_char);
    //DEV_Uart_SendByte('\r');
    //DEV_Uart_SendByte('\n');
    sleep_ms(200);
}

void DEV_Uart_SendByte(char data)
{
    uart_putc(uart0, data);
}

void DEV_Uart_SendString(char *data)
{
    uint16_t i;
    for(i = 0; data[i] != '\0'; i++){
        uart_putc(uart0, data[i]);
    }

}
