# Projeto: Controle de LEDs e Animações com Interrupções no Raspberry Pi Pico

## Descrição

Este projeto utiliza o microcontrolador Raspberry Pi Pico para controlar LEDs WS2812 e LEDs RGB, criando animações e exibindo números de 0 a 9 em uma matriz de LEDs 5x5. O controle da animação é feito por meio de interrupções nos botões A e B, com debouncing via software para evitar múltiplos acionamentos indesejados.

## Funcionalidades

1. **Controle de LED RGB:**
   - O LED vermelho pisca continuamente 5 vezes por segundo.
   
2. **Controle da Matriz de LEDs WS2812:**
   - A matriz 5x5 de LEDs WS2812 é usada para criar animações visuais representando números de 0 a 9.
   - A exibição dos números é feita no estilo digital (com segmentos iluminados que formam o número, ou alternativa criativa).

3. **Botões A e B:**
   - O botão A incrementa o número exibido na matriz de LEDs.
   - O botão B decrementa o número exibido na matriz de LEDs.

4. **Debouncing de Botões:**
   - Implementação de debouncing via software para garantir que os botões A e B não gerem múltiplos eventos devido ao fenômeno do bouncing.

5. **Interrupções:**
   - As funcionalidades de controle dos botões A e B são implementadas utilizando interrupções (IRQ).

## Requisitos

- **Hardware:**
  - Raspberry Pi Pico
  - Matriz 5x5 de LEDs WS2812 conectada à GPIO 7
  - LED RGB (pinos 11, 12, 13)
  - Botão A (GPIO 5)
  - Botão B (GPIO 6)
  - Resistores de pull-up internos nos botões de acionamento

- **Software:**
  - Raspberry Pi Pico SDK
  - Visual Studio Code com a extensão para Raspberry Pi Pico

## Configuração do Hardware

| Componente        | Conexão        |
|-------------------|----------------|
| Matriz WS2812     | GPIO 7         |
| LED RGB (vermelho)| GPIO 11        |
| LED RGB (verde)   | GPIO 12        |
| LED RGB (azul)    | GPIO 13        |
| Botão A           | GPIO 5         |
| Botão B           | GPIO 6         |

## Como Executar

### 1. Configuração do Ambiente

- Certifique-se de que o SDK do Raspberry Pi Pico está configurado corretamente no seu ambiente de desenvolvimento.
- Instale o Visual Studio Code e a extensão **Raspberry Pi Pico Project**.

### 2. Compilação e Execução

1. Compile o programa no Visual Studio Code utilizando a extensão **Raspberry Pi Pico Project**.
2. Coloque o Raspberry Pi Pico em modo de boot e copie o arquivo `main.uf2` para o microcontrolador conectado via USB.

### 3. Emulação com Wokwi 

1. Para emular o projeto sem hardware físico, utilize o **Wokwi**.
2. Instale a extensão **Wokwi for VS Code** e inicie a emulação.
3. Teste o funcionamento do programa no ambiente emulado.

## Demonstração

Confira a demonstração do projeto em vídeo: [Demonstração do Projeto](https://youtube.com/shorts/ti1Q9QjH6d0)

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
