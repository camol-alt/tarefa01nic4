#include "pico/stdlib.h"

// Configurações dos pinos do LED
const uint led_pin_pisca = 13; // Red => GPIO13
#define tempo1 1000           // Tempo de piscar do Led em ms (1 segundo) por 5 vezes
#define tempo2 3000          // tempo de piscar do led em ms (3 segundos) para retornar novamente


int main()
{

    gpio_init(led_pin_pisca);              // Inicializa o pino do LED para piscar
    gpio_set_dir(led_pin_pisca, GPIO_OUT); // Configura o pino como saída
    gpio_put(led_pin_pisca, 0);            // Garante que o LED inicie apagado


    // Loop principal
    while (true) {
        // Lógica para o LED piscar por 5 vezes
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo1);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo1);             // Mantém desligado por "tempo" ms
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo1);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo1);             // Mantém desligado por "tempo" ms
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo1);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo1);             // Mantém desligado por "tempo" ms
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo1);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo1);             // Mantém desligado por "tempo" ms
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo1);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo2);             // Mantém desligado por "tempo" ms
    }

 }