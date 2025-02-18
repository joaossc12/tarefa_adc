#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "./includes/ssd1306.h"
#include "./includes/font.h"



#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define LED_RED 13
#define LED_BLUE 12
#define LED_GREEN 11
#define BUTTON_A 5
#define JOYSTICK_X 27  // GPIO para eixo X
#define JOYSTICK_Y 26  // GPIO para eixo Y
#define JOYSTICK_BT 22 // GPIO para botão do Joystick

uint16_t adc_value_x;
uint16_t adc_value_y;  
uint slice_led; //GPIO 13 e 12 controladas pelo mesmo slice
uint16_t wrap = 2048; //Escolhido para facilitar as contas
float div_clk = 30; //Para aproximadamente 2khz

uint16_t center_value = 2000;//Medido Experimentalmente
uint16_t level_blue = 2048;
uint16_t level_red = 2048;
bool state_pwm = 1;

ssd1306_t ssd; //Display de 128 x 64
uint8_t top = 29; //posicao central
uint8_t left = 61; //posicao central
bool edge_flag = 0;


static void callback_button(uint gpio, uint32_t events);
void init_pinos();
void control_display(int top, int left, bool flag);

void control_led(int adc_value_x, int adc_value_y);

int main() {
  stdio_init_all();
  init_pinos();

  //ISR
  gpio_set_irq_enabled_with_callback(JOYSTICK_BT, GPIO_IRQ_EDGE_FALL, true, callback_button);
  gpio_set_irq_enabled(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);

  //ADC
  adc_init();
  adc_gpio_init(JOYSTICK_X);
  adc_gpio_init(JOYSTICK_Y); 

  //PWM
  pwm_set_clkdiv(slice_led, div_clk); //define o divisor de clock do PWM
  pwm_set_wrap(slice_led, wrap); //definir o valor de wrap – valor máximo do contador PWM
  pwm_set_enabled(slice_led, true); //habilitar o pwm no slice correspondent


  //Display
  i2c_init(I2C_PORT, 1000 * 1000); //Frquencia de 400Khz
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display
  ssd1306_fill(&ssd, false); //Apaga todos pixels
  ssd1306_send_data(&ssd); //Envia o comando



  while (true) {
    adc_select_input(1); // Seleciona o ADC para eixo X. O pino 27 como entrada analógica
    adc_value_x = adc_read();
    adc_select_input(0); // Seleciona o ADC para eixo Y. O pino 26 como entrada analógica
    adc_value_y = adc_read();    
    printf(" EIXO x: %d EIXO y: %d \n", adc_value_x, adc_value_y);
    control_led(adc_value_x, adc_value_y);
    top = (64 - 8) - (adc_value_y * 0.0137);
    left = adc_value_x * 0.0294;
    control_display(top, left,  edge_flag);
  }
}

static void callback_button(uint gpio, uint32_t events) {
    static absolute_time_t last_time_A = 0; // Tempo do último evento do botão A
    static absolute_time_t last_time_B = 0; // Tempo do último evento do botão B
    absolute_time_t now = get_absolute_time();

    if (gpio == BUTTON_A) { // Interrupção do botão A
        if (absolute_time_diff_us(last_time_A, now) > 200000) { // Debounce de 200ms
            state_pwm = !state_pwm;
            pwm_set_enabled(slice_led, state_pwm); 
            last_time_A = now; // Atualiza o tempo do último evento do botão A
        }
    } else if (gpio == JOYSTICK_BT) { // Interrupção do botão B
        if (absolute_time_diff_us(last_time_B, now) > 200000) { // Debounce de 200ms
          gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
          edge_flag = !edge_flag;
          last_time_B = now; // Atualiza o tempo do último evento do botão B
        }
    }
}

void init_pinos(){
  gpio_set_function(LED_RED, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
  slice_led = pwm_gpio_to_slice_num(LED_RED); //obter o canal (slice) PWM da GPIO
  //Pino 12 e 13 compartilham o mesmo slice
  gpio_set_function(LED_BLUE, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
  //uint slice_blue = pwm_gpio_to_slice_num(LED_BLUE); //obter o canal (slice) PWM da GPIO
  gpio_init(LED_GREEN);//inicializa o pino 11 do microcontrolador
  gpio_set_dir(LED_GREEN, GPIO_OUT);//configura o pino como entrada


  gpio_init(BUTTON_A);//inicializa o pino 5 do microcontrolador
  gpio_set_dir(BUTTON_A, GPIO_IN);//configura o pino como entrada
  gpio_pull_up(BUTTON_A); //Pull up pino 5

  gpio_init(JOYSTICK_BT);//inicializa o pino 22 do microcontrolador
  gpio_set_dir(JOYSTICK_BT, GPIO_IN);//configura o pino como entrada
  gpio_pull_up(JOYSTICK_BT); //Pull up pino 22
}
void control_display(int top, int left, bool flag){
    ssd1306_fill(&ssd, false); //Apaga todos pixels
  
  if (flag){
    //Borda grossa
    ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 1); // Desenha um retângulo
    ssd1306_rect(&ssd, 4, 4, 120, 56, 0, 1); // Desenha um retângulo
  }
  else{
    ssd1306_rect(&ssd, 3, 3, 122, 58, 1, 0); // Desenha um retângulo

  }
    ssd1306_rect(&ssd, top, left, 8, 8, 1, 1); // Desenha o quadrado
    ssd1306_send_data(&ssd); // Atualiza o display
 }
 void control_led(int adc_value_x, int adc_value_y){
    uint8_t tolerancia = 100;
    if (abs(adc_value_y - center_value) > tolerancia){
    level_red = abs(adc_value_y - center_value);}
    else{level_red = 0;}

    if (abs(adc_value_x - center_value) > tolerancia){
    level_blue = abs(adc_value_x - center_value);}
    else{level_blue = 0;}

    pwm_set_gpio_level(LED_RED, level_red);
    pwm_set_gpio_level(LED_BLUE, level_blue);
}
