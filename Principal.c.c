#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define LED_PIN 13 // GPIO 13 para o LED

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 20; // Intensidade do vermelho
uint8_t led_g = 0;  // Intensidade do verde
uint8_t led_b = 0;  // Intensidade do azul

// Buffer para armazenar quais LEDs estão ligados matriz 5x5
bool led_buffer0[NUM_PIXELS] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

bool led_buffer1[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 1,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
};

bool led_buffer2[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

bool led_buffer3[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

bool led_buffer4[NUM_PIXELS] = {
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
};

bool led_buffer5[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
};

bool led_buffer6[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
};

bool led_buffer7[NUM_PIXELS] = {
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

bool led_buffer8[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

bool led_buffer9[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
};

bool* led_buffers[10] = {
    led_buffer0, led_buffer1, led_buffer2, led_buffer3, led_buffer4, led_buffer5,
    led_buffer6, led_buffer7, led_buffer8, led_buffer9
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void animate_leds(int start, int end, int step) {
    for (int i = start; (step > 0) ? i <= end : i >= end; i += step) {
        for (int j = 0; j < NUM_PIXELS; j++) {
            if (led_buffers[i][j]) {
                put_pixel(urgb_u32(led_r, led_g, led_b));
            } else {
                put_pixel(0); // Desliga o LED
            }
        }
        sleep_ms(500); // Tempo de espera entre as animações
    }
}

// Variáveis para controlar o piscar do LED
bool led_state = false;
uint64_t last_toggle_time = 0;
uint64_t blink_duration = 500000; // 500ms em microssegundos
uint64_t blink_start_time = 0;
bool blinking = false;

void update_led_gpio13() {
    uint64_t current_time = time_us_64();

    if (blinking) {
        if (current_time - last_toggle_time >= blink_duration) {
            led_state = !led_state; // Alterna o estado do LED
            gpio_put(LED_PIN, led_state);
            last_toggle_time = current_time;
        }

        // Verifica se o tempo de piscar (5 segundos) acabou
        if (current_time - blink_start_time >= 5000000) { // 1 segundos em microssegundos
            blinking = false;
            gpio_put(LED_PIN, 0); // Desliga o LED
        }
    } else {
        // Verifica se é hora de começar a piscar novamente
        if (current_time - blink_start_time >= 10) { //  segundos em microssegundos (5 piscando + 1 de intervalo)
            blinking = true;
            blink_start_time = current_time;
        }
    }
}

int main() {
    stdio_init_all();

    // Inicializa o PIO para controlar os LEDs WS2812
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, IS_RGBW);

    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    blink_start_time = time_us_64(); // Inicia o tempo de piscar

    while (true) {
        if (!gpio_get(BTN_A_PIN)) {
            animate_leds(0, 9, 1); // Botão A: animação de 0 a 9
        } else if (!gpio_get(BTN_B_PIN)) {
            animate_leds(9, 0, -1); // Botão B: animação de 9 a 0
        } else {
            // Desliga todos os LEDs
            for (int i = 0; i < NUM_PIXELS; i++) {
                put_pixel(0);
            }
        }

        update_led_gpio13(); // Atualiza o estado do LED na GPIO 13
        sleep_ms(10); // Pequeno delay para evitar uso excessivo da CPU
    }
}