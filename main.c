#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define NUM_LEDS 25
#define WS2812_PIN 7
#define TEMPO_LED 400

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0;   // Intensidade do vermelho
uint8_t led_g = 0;   // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

// Buffer para armazenar quais LEDs estão ligados matriz 5x5
bool led_animacoes[NUM_LEDS] = {
    //  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
    0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0};
/*
24,23,22,21,20
15,16,17,18,19
14,13,12,11,10
05,06,07,08,09
04,03,02,01,00
*/
// Função para ativar um LED
static inline void ligar_led(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para conversão de RGB -> GRB
static inline uint32_t rgb_para_grb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função acende os leds de acordo o frame passado
void config_cor_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Conversão das intensidades de cor para um valor uint32_t
    uint32_t color = rgb_para_grb(r, g, b);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (led_animacoes[i])
        {
            ligar_led(color);
        }
        else
        {
            ligar_led(0);
        }
    }
}

int main()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    while (1)
    {
        config_cor_led(led_r, led_g, led_b);
        sleep_ms(TEMPO_LED);
        config_cor_led(0, 0, 0);
        sleep_ms(TEMPO_LED);
    }

    return 0;
}