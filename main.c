#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/timer.h"

#define NUM_LEDS 25 // Quantidade de leds
#define WS2812_PIN 7 // Pino de dados do ws2812
#define TEMPO_LED 100 // Tempo em ms para acender e apagar o led
#define BOTAO_PIN_A 5 // Pino do botão A
#define BOTAO_PIN_B 6 // Pino do botão B
#define LED_PIN_R 13 // Pino do led de status (Vermelho)

static int frame = 0; // Armazena o frame atual

uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0;   // Intensidade do vermelho
uint8_t led_g = 0;   // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

/*  24,23,22,21,20
    15,16,17,18,19
    14,13,12,11,10
    05,06,07,08,09
    04,03,02,01,00 */

// Matriz para armazenar as animações dos leds 5x5
bool led_animacoes[10][NUM_LEDS] = {
    //  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
    {0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,0},    // 0
    {0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0},    // 1
    {0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,1,1,1,0},    // 2
    {0,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,1,1,1,0},    // 3
    {0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,0,1,0},    // 4
    {0,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0},    // 5
    {0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0},    // 6
    {0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0},    // 7
    {0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0},    // 8
    {0,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0}     // 9

};

// Função para ativar um LED com cor específica
static inline void ligar_led(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Faz o envio do valor do LED para o PIO
}

// Função para conversão de RGB -> GRB
static inline uint32_t rgb_para_grb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Conversão de RGB -> GRB
}

// Função acende os leds de acordo o frame passado
void config_cor_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Conversão das intensidades de cor para um valor uint32_t
    uint32_t color = rgb_para_grb(r, g, b); // Conversão de RGB -> GRB
    for (int i = 0; i < NUM_LEDS; i++) // Iteração sobre todos os leds
    {
        ligar_led(led_animacoes[frame][i]?color:0); // Acende o LED se o valor for 1, caso contrário apaga
    }
}

// Função de interrupção dos botões com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
   
    // Verifica se passou tempo suficiente para realizar o debouncing
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {   
        last_time = current_time; // Atualiza o tempo do último evento
        // printf("Botão %d pressionado\n", gpio);
        //Verifica qual botão foi pressionado ao trocar o frame
        if (gpio == BOTAO_PIN_A){
            frame = frame == 9?0 : frame + 1; // Passa para o próximo frame se o botão A for pressionado ou reinicia
        }
        if (gpio == BOTAO_PIN_B){
            frame = frame == 0?9 : frame - 1; // Passa para o frame anterior se o botão B for pressionado ou vai para o último
        }
        config_cor_led(led_r, led_g, led_b); // Atualiza cor dos leds
    }

    // printf("Pressionado fora do if \n", gpio);
}

// Função para configurar os pinos (entrada ou saída, com ou sem pullup)
void Configurar_Pins(uint pin, bool saida, bool pullup){
    gpio_init(pin); // Inicia o pino
    gpio_set_dir(pin, saida); // Defini o pino como entrada ou saída
    if(pullup)gpio_pull_up(pin); // Ativa o pullup
    if(!pullup)gpio_put(pin, 0); // Sem o pullup, desliga o pino
    
}

int main()
{
    stdio_init_all(); // Inicia a serial

// Inicialização do PIO e o ws2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

// Configura os pinos dos botoes e leds
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
    Configurar_Pins(BOTAO_PIN_A, GPIO_IN, true);
    Configurar_Pins(BOTAO_PIN_B, GPIO_IN, true);
    Configurar_Pins(LED_PIN_R, GPIO_OUT, false);

// Ativa as interrupções dos botoes com callback
    gpio_set_irq_enabled_with_callback(BOTAO_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_PIN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    config_cor_led(led_r, led_g, led_b); // Os leds são configurados para cor inicial

    while (1)
    {
        gpio_put(LED_PIN_R, 1); // Acende o LED de status
        sleep_ms(TEMPO_LED); // Aguarda tempo definido em TEMPO_LED 
        gpio_put(LED_PIN_R, 0); // Apaga o LED 
        sleep_ms(TEMPO_LED); // Aguarda noavmente 
    }
    return 0;
}