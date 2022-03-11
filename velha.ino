/*
 * Implementações de Algoritmos de jogo da velha
 * tabuleiro: 9 casas, numeradas de 1 a 9
 *  1 | 2 | 3
 * ---+---+---
 *  4 | 5 | 6
 * ---+---+---
 *  7 | 8 | 9
 * Cada casa pode conter:
 * 0: Casa vazia
 * 1: X - Computador
 * 2: O - Humano
 * Retorno: número da casa (entre 1 e 9) onde o computador jogará
 */
#define ERRO -1

int faltaUm(int tabuleiro[], int lado) {
  int i;
  // Retorna a primeira casa que encontrar em que falte
  // uma peça para o lado escolhido
  // Horizontal
  for (i = 0; i < 9; i+=3) {
    if (tabuleiro[i] == VAZIO &&
        tabuleiro[i+1] == lado &&
        tabuleiro[i+2] == lado)
      return i+1;
    if (tabuleiro[i+1] == VAZIO &&
        tabuleiro[i] == lado &&
        tabuleiro[i+2] == lado)
      return i+2;
    if (tabuleiro[i+2] == VAZIO &&
        tabuleiro[i+1] == lado &&
        tabuleiro[i] == lado)
      return i+3;
  }
  // Vertical
  for (i = 0; i < 3; i++) {
    if (tabuleiro[i] == VAZIO &&
        tabuleiro[i+3] == lado &&
        tabuleiro[i+6] == lado)
      return i+1;
    if (tabuleiro[i+3] == VAZIO &&
        tabuleiro[i] == lado &&
        tabuleiro[i+6] == lado)
      return i+4;
    if (tabuleiro[i+6] == VAZIO &&
        tabuleiro[i+3] == lado &&
        tabuleiro[i] == lado)
      return i+7;
  }
  // Diagonal principal
  if (tabuleiro[0] == VAZIO &&
      tabuleiro[4] == lado &&
      tabuleiro[8] == lado)
    return 1;
  if (tabuleiro[4] == VAZIO &&
      tabuleiro[0] == lado &&
      tabuleiro[8] == lado)
    return 5;
  if (tabuleiro[8] == VAZIO &&
      tabuleiro[4] == lado &&
      tabuleiro[0] == lado)
    return 9;
  // Diagonal secundária
  if (tabuleiro[2] == VAZIO &&
      tabuleiro[4] == lado &&
      tabuleiro[6] == lado)
    return 3;
  if (tabuleiro[4] == VAZIO &&
      tabuleiro[2] == lado &&
      tabuleiro[6] == lado)
    return 5;
  if (tabuleiro[6] == VAZIO &&
      tabuleiro[4] == lado &&
      tabuleiro[2] == lado)
    return 7;
  // Não tem casa vazia para completar
  return ERRO;
}

int cantoOposto(int tabuleiro[], int lado) {
  int outroLado;
  // Cantos 1, 3, 7, 9
  outroLado = (lado == X ? O : X);
  if (tabuleiro[0] == VAZIO && tabuleiro[8] == outroLado)
    return 1;
  if (tabuleiro[8] == VAZIO && tabuleiro[0] == outroLado)
    return 9;
  if (tabuleiro[2] == VAZIO && tabuleiro[6] == outroLado)
    return 3;
  if (tabuleiro[6] == VAZIO && tabuleiro[2] == outroLado)
    return 7;
  return ERRO;
}

int cantoVazio(int tabuleiro[]) {
  // Cantos 1, 3, 7, 9
  if (tabuleiro[0] == VAZIO)
    return 1;
  if (tabuleiro[8] == VAZIO)
    return 9;
  if (tabuleiro[2] == VAZIO)
    return 3;
  if (tabuleiro[6] == VAZIO)
    return 7;
  return ERRO;
}

int velhaRandomico(int tabuleiro[]) {
  int casa;
  // Cuidado: risco de loop infinito
  do {
    casa = random(NUMCASAS);
  } while (tabuleiro[casa]);
  return casa+1;
}

int velhaGanha(int tabuleiro[]) {
  // Se puder ganhar ganha
  // Senão joga randomicamente
  int casa;

  if ((casa = faltaUm(tabuleiro, X)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
}

int velhaGanhaBloqueia(int tabuleiro[]) {
  // Se puder ganhar ganha
  // Se puder bloquear, bloqueia
  // Senão joga randomicamente
  int casa;

  if ((casa = faltaUm(tabuleiro, X)) != ERRO)
    return casa;
  if ((casa = faltaUm(tabuleiro, O)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
}

int velhaNewellESimon(int tabuleiro[]) {
  /*
   * Implementação do algoritmo da velha de Newell e Simon de 1972
   * Referências: https://en.wikipedia.org/wiki/Tic-tac-toe#Strategy e
   *              https://pt.wikipedia.org/wiki/Jogo_da_velha#Estrat%C3%A9gias
   * O jogador deve escolher a primeira opção disponível entre:
   * 1. Ganhar: Se você tem duas peças numa linha, ponha a terceira.
   * 2. Bloquear: Se o oponente tiver duas peças em linha, ponha a terceira para bloqueá-lo.
   * 3. Triângulo: Crie uma oportunidade em que você poderá ganhar de duas maneiras.
   * 4. Bloquear o Triângulo do oponente:
   *    Opção 1: Crie 2 peças em linha para forçar o oponente a se defender, contanto que não resulte nele criando um triângulo ou vencendo.
                 Por exemplo, se 'X' tem dois cantos opostos do tabuleiro e 'O' tem o centro, 'O' não pode jogar num canto
                 (Jogar no canto nesse cenário criaria um triângulo em que 'X' vence).
   *    Opção 2: Se existe uma configuração em que o oponente pode formar um triângulo, bloqueiem-no.
   * 5. Centro: Jogue no centro.
   * 6. Canto oposto: Se o oponente está no canto, jogo no canto oposto.
   * 7. Canto vazio: jogue num canto vazio.
   * 8. Lado vazio: jogue na casa do meio de qualquer dos lados.
   */
  int casa;

  // Regra 1
  if ((casa = faltaUm(tabuleiro, X)) != ERRO)
    return casa;
  // Regra 2
  if ((casa = faltaUm(tabuleiro, O)) != ERRO)
    return casa;
  // Regra 5
  if (tabuleiro[4] == VAZIO)
    return 5;
  // Regra 6
  if ((casa = cantoOposto(tabuleiro, X)) != ERRO)
    return casa;
  // Regra 7
  if ((casa = cantoVazio(tabuleiro)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
  
}