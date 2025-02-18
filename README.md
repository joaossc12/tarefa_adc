
# Tarefa Conversor Analógico Digital

## Descrição
Este projeto integra múltiplos componentes e funcionalidades utilizando a Raspberry Pi Pico. Nele, são realizados os seguintes processos:
- **Leitura do Joystick:** São capturados os valores analógicos dos eixos X e Y do joystick através do ADC.
- **Controle de LEDs com PWM:** Os LEDs vermelho e azul têm sua intensidade ajustada conforme a posição do joystick, enquanto o LED verde é acionado digitalmente.
- **Interrupções e Debounce:** São utilizadas interrupções para os botões (Button A e o botão do joystick), permitindo alternar o estado do PWM e atualizar a interface gráfica.
- **Display SSD1306:** Um display OLED de 128x64 pixels é controlado via I2C para exibir gráficos que incluem retângulos e um quadrado, cujo posicionamento depende dos valores lidos do joystick.

## Objetivos
- Demonstrar a integração de sensores (joystick) e atuadores (LEDs com PWM) na Raspberry Pi Pico.
- Exibir informações visuais e feedback através de um display SSD1306.
- Explorar o uso de ADC, PWM, I2C e interrupções para controle de hardware.
- Implementar técnicas de debounce para garantir a confiabilidade das leituras dos botões.

## Tecnologias Utilizadas
- **Linguagem C:** Para o desenvolvimento do firmware.
- **Raspberry Pi Pico:** Microcontrolador utilizado no projeto.
- **Hardware e Bibliotecas:**
  - **ADC:** Para leitura dos valores dos eixos do joystick.
  - **PWM:** Para controle dos LEDs (vermelho e azul).
  - **I2C:** Para comunicação com o display SSD1306.
  - **Interrupções:** Para o tratamento dos eventos dos botões.

## Vídeo Demonstração 
- [Youtube](https://youtube.com/shorts/vHorKfteJ9Y)

## Como executar o Projeto

### Pré-requisitos
- **Hardware:**
  - Raspberry Pi Pico.
  - Display SSD1306 (128x64) com interface I2C.
  - Joystick com dois eixos (X e Y) e botão integrado.
  - LEDs (vermelho, azul e verde) com resistores apropriados.
  - Botão A.
- **Software:**
  - Ambiente de desenvolvimento configurado com o SDK da Raspberry Pi Pico (suporte a C/C++).

### Passos para execução
1. **Clone o Repositório:**  
   Faça o download ou clone o repositório que contém o código fonte.

2. **Configure o Ambiente de Desenvolvimento:**  
   Certifique-se de que o SDK da Raspberry Pi Pico está instalado e devidamente configurado.

3. **Compilação:**  
   Compile o código utilizando as ferramentas do SDK, como CMake e Make.

4. **Conexões de Hardware:**  
   Configure os pinos conforme as definições do código:
   - **Display SSD1306 (I2C):**
     - SDA: GPIO 14
     - SCL: GPIO 15
     - Endereço I2C: 0x3C
   - **LEDs:**
     - LED Vermelho: GPIO 13 (configurado para PWM)
     - LED Azul: GPIO 12 (configurado para PWM)
     - LED Verde: GPIO 11 (configurado como saída digital)
   - **Joystick:**
     - Eixo X: GPIO 27 (entrada ADC)
     - Eixo Y: GPIO 26 (entrada ADC)
     - Botão do Joystick: GPIO 22 (entrada digital com pull-up)
   - **Botão A:**  
     - GPIO 5 (entrada digital com pull-up)

5. **Carregue o Firmware:**  
   Transfira o binário compilado para a Raspberry Pi Pico.

6. **Execução:**  
   Após a transferência, o sistema:
   - Lê continuamente os valores do joystick e ajusta a intensidade dos LEDs conforme as variações dos eixos.
   - Atualiza a interface gráfica do display, desenhando retângulos e um quadrado cuja posição varia de acordo com os valores do joystick.
   - Permite alternar o estado do PWM dos LEDs pressionando o Botão A e alterna o estado do LED verde (além do desenho da borda no display) ao pressionar o botão do joystick.

Após seguir estes passos, o projeto estará em execução, proporcionando uma integração completa entre sensores, atuadores e uma interface gráfica.
