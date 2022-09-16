#include "pinosArduino.h"
#include "notas.h"
#include "musicas.h"

// Número de casas do jogo da velha
#define NUMCASAS 9

// Bitmap de resultados
// 1 2 3
// 4 5 6
// 7 8 9
// Bitmap de todos os leds acesos (2^NUMCASAS - 1)
#define TODOS (1<<NUMCASAS)-1
// Bitmap dos diferentes resultados.
// Usados como resultado e também como LEDs a acender
// DEUVELHA - V
// * 2 *
// * 5 *
// 7 * 9
#define DEUVELHA 0b010101101
// XGANHOU - X
// * 2 *
// 4 * 6
// * 8 *
#define XGANHOU  0b101010101
// OGANHOU = O
// * * *
// * 5 *
// * * *
#define OGANHOU  0b111101111

// Vazio, X e O.
// X e O devem ser potências de 2
#define VAZIO 0
#define X 1
#define O 4

// Variáveis globais:
// casas: tabuleiro, vetor de 9 posições, casa uma com VAZIO, X ou O
// vez: vez de quem joga, X ou O
// lance: número do lance da partida atual
int casas[NUMCASAS], vez, lance, nivel = 1;

// Operações em casas
// ** Consulta a peça de uma casa
int consulta(int casa) {
  return casas[casa-1];
}

// ** Indica se uma casa está vazia
bool vazio(int casa) {
  return consulta(casa) == VAZIO;
}

// ** Coloca uma peça em uma casa (peça pode ser X, O ou VAZIO)
void ocupa(int casa, int valor) {
  casas[casa-1] = valor;
}

// Mostra o resultado de um jogo
// resultado: Bitmap do resultado
void mostraResultado(int resultado) {
  switch (resultado) {
    case DEUVELHA:
      delay(1000);
      apagaLEDs();
      liga(X|O, resultado);
      toca(pacMan, sizeof(pacMan) / sizeof(int) / 2);
      break;
    case XGANHOU:
      apagaLEDs();
      liga(X, resultado);
      toca(starWars, sizeof(starWars) / sizeof(int) /2);
      break;
    case OGANHOU:
      apagaLEDs();
      liga(O, resultado);
      toca(starTrek, sizeof(starTrek) / sizeof(int) / 2);
      break;
  }
}

// Converte 3 casas no bitmap correspondente
int bits(int a, int b, int c) {
  return 1<<(a-1) | 1<<(b-1) | 1<<(c-1);
}

// Verifica se o jogo acabou e retorna
// quem ganhou ou velha
int acabou() {
  const int vezes = 20, intervalo = 50;
  int i, lado;
  // Verifica horizontais
  for (i = 1; i <= 9; i+=3) {
    if (lado = (consulta(i) & consulta(i+1) & consulta(i+2))) {
      pisca(vezes, intervalo, lado, bits(i, i+1, i+2));
      return lado == X ? XGANHOU : OGANHOU;
    }
  }
  // Verifica verticais
  for (i = 1; i <= 3; i++) {
    lado = consulta(i);
    if (lado = (consulta(i) & consulta(i+3) & consulta(i+6))) {
      pisca(vezes, intervalo, lado, bits(i, i+3, i+6));
      return lado == X ? XGANHOU : OGANHOU;
    }
  }
  // Verifica diagonais
  if (lado = (consulta(1) & consulta(5) & consulta(9))) {
    pisca(vezes, intervalo, lado, bits(1, 5, 9));
    return lado == X ? XGANHOU : OGANHOU;
  }
  if (lado = (consulta(3) & consulta(5) & consulta(7))) {
    pisca(vezes, intervalo, lado, bits(3, 5, 7));
    return lado == X ? XGANHOU : OGANHOU;
  }
  return lance == NUMCASAS ? DEUVELHA : VAZIO;
}

// Resposta a um botão pressionado
bool aciona(int casa) {
  if (!vazio(casa)) {
    buzzer();
    return false;
  }
  acende(O, casa);
  ocupa(casa, O);
  delay(1000);
  return true;
}

// Prepara início do jogo
void comecaJogo() {
  int casa, jogadores[] = {X, O};

  // Esvazia tabuleiro
  for (casa = 1; casa <= NUMCASAS; casa++)
    ocupa(casa, VAZIO);

  // Pisca todos LEDs vermelhos e verdes e espera um segundo
  pisca(5, 25, X|O, TODOS);
  delay(1000);

  // Sorteia e acende leds de quem começa
  vez = jogadores[random(2)];
  pisca(150, 1, vez, TODOS);
  lance = 0;
  conta(nivel);
}

// Setup e loop
void setup() {
  // Configura pinos
  int led, botao;
  // **Leds como saídas
  for (led = o1; led <= x9; led++) {
    pinMode(led, OUTPUT);    
  }
  // Botões como entradas pullup
  for (botao = botao1; botao <= botao9; botao++) {
    pinMode(botao, INPUT_PULLUP);
  }

  // Botão de nível de jogo
  pinMode(botaoNivel, INPUT_PULLUP);

  pinMode(pinoBuzzer, OUTPUT);
  // Prepara serial
  Serial.begin(9600);
  // Gera semente
  randomSeed(analogRead(0));
  comecaJogo();
}

/*
 * Loop do jogo
 */
void loop() {
  int casa, resultado;


  // Se for a vez do jogador, vê que botão apertou
  if (vez == O) {
    while (vez == O) {
      for (casa = 1; casa <= NUMCASAS; casa++) {
        if (leBotao(casa)) {
            // Verifica se é um botão válido
            if (aciona(casa)) {
              vez = X;
              lance++;
              break;
            }
        }
      }
    }
  } else if (vez == X) {
    // Senão é a vez do Arduino...
    casa = velhaNewellESimon(casas, nivel);
    acende(X, casa);
    ocupa(casa, X);
    lance++;
    vez = O;
  }

  if (resultado = acabou()) {
    mostraResultado(resultado);
    // Joga de novo
    nivel = (nivel + 1) % 9;
    comecaJogo();
  }
}