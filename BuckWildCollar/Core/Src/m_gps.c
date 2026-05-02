/*
 * m_gps.c
 *
 *  Created on: May 1, 2026
 *      Author: eickhoffp
 */

#include "m_gps.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


extern UART_HandleTypeDef huart2;

#define RX_LINE_MAX 128
#define RX_QUEUE_LINES 16

static uint8_t rx_byte;

static char build_line[RX_LINE_MAX];
static uint16_t build_idx = 0;

static char line_q[RX_QUEUE_LINES][RX_LINE_MAX];
static volatile uint8_t q_head = 0, q_tail = 0;

static volatile uint32_t lines_ok = 0;
static volatile uint32_t lines_drop = 0;

typedef struct {
    char utc[16];
    float lat;
    float lon;
    int sats;
    float hdop;
    int valid;
} gps_data_t;

static gps_data_t gps;

void gps_acquire(char *buffer, int size);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
static int queue_push(const char *s);
static int queue_pop(char *out);
static void parse_nmea(char *line);
static float nmea_to_deg(const char *s);



void gps_acquire(char *buffer, int size){
	/*
	 * Place GPS string into buffer
	 * Ensure sufficient buffer size: >128 should suffice
	 */
    char line[RX_LINE_MAX];

    while (queue_pop(line))
    {
        parse_nmea(line);
        int lat_i = (int)(gps.lat * 1000000);
        int lon_i = (int)(gps.lon * 1000000);
        int hdop_i = (int)(gps.hdop * 100);

        snprintf(buffer, size, "UTC:%s LATi:%d LONi:%d SAT:%d HDOPi:%d VALID:%d\r\n", gps.utc, lat_i, lon_i, gps.sats, hdop_i, gps.valid);

    }
    HAL_Delay(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
   if (huart->Instance == USART2)
   {
     // Grab received byte and
     uint8_t b = rx_byte;

     // Build a line until '\n' (NMEA typical end is "\r\n")
     if (b == '\r' || b == '\n')
     {
         if (build_idx > 0)
         {
             build_line[build_idx] = '\0';

             if (queue_push(build_line))
                 lines_ok++;
             else
                 lines_drop++;

             build_idx = 0;
         }
     }
     else
     {
         if (build_idx < RX_LINE_MAX - 1)
             build_line[build_idx++] = (char)b;
         else
         {
             build_idx = 0;
             lines_drop++;
         }
     }

     // Re-arm next byte
     HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
   }
 }

 static int queue_push(const char *s)
 {
   uint8_t next = (uint8_t)((q_head + 1) % RX_QUEUE_LINES);
   if (next == q_tail) return 0; // full

   strncpy(line_q[q_head], s, RX_LINE_MAX);
   line_q[q_head][RX_LINE_MAX - 1] = '\0';
   q_head = next;
   return 1;
 }

 static int queue_pop(char *out)
 {
   if (q_tail == q_head) return 0; // empty

   strncpy(out, line_q[q_tail], RX_LINE_MAX);
   out[RX_LINE_MAX - 1] = '\0';
   q_tail = (uint8_t)((q_tail + 1) % RX_QUEUE_LINES);
   return 1;
 }

 static float nmea_to_deg(const char *s)
 {
     if (!s || !*s) return 0.0f;

     float v = (float)atof(s);
     int deg = (int)(v / 100.0f);
     float min = v - (deg * 100.0f);
     return deg + (min / 60.0f);
 }

 static void parse_nmea(char *line)
 {
     char *f[20] = {0};
     int n = 0;

     char *tok = strtok(line, ",");
     while (tok && n < 20) {
         f[n++] = tok;
         tok = strtok(NULL, ",");
     }

     if (n == 0) return;

     if (!strncmp(f[0], "$GNRMC", 6) || !strncmp(f[0], "$GPRMC", 6))
     {
         if (n > 6) {
             strncpy(gps.utc, f[1] ? f[1] : "", sizeof(gps.utc)-1);
             gps.utc[sizeof(gps.utc)-1] = '\0';

             gps.valid = (f[2] && f[2][0] == 'A');

             gps.lat = nmea_to_deg(f[3]);
             if (f[4] && f[4][0] == 'S') gps.lat = -gps.lat;

             gps.lon = nmea_to_deg(f[5]);
             if (f[6] && f[6][0] == 'W') gps.lon = -gps.lon;
         }
     }
     else if (!strncmp(f[0], "$GNGGA", 6) || !strncmp(f[0], "$GPGGA", 6))
     {
         if (n > 8) {
             gps.sats = f[7] ? atoi(f[7]) : 0;
             gps.hdop = f[8] ? (float)atof(f[8]) : 0.0f;
         }
     }
 }
