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

int velhaRandomico(int tabuleiro[]) {
  int casa;
  // Cuidado: risco de loop infinito
  do {
    casa = random(NUMCASAS);
  } while (tabuleiro[casa]);
  return casa+1;
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
   *    Opção 1: Crie 2 peças em linha para forçar o oponente a se defender, contanto que não resulte nele criando um triângulo ou vencendo. Por exemplo, se 'X' tem dois cantos opostos do tabuleiro e 'O' tem o centro, 'O' não pode jogar num canto (Jogar no canto nesse cenário criaria um triângulo em que 'X' vence).
   *    Opção 2: Se existe uma configuração em que o oponente pode formar um triângulo, bloqueiem-no.
   * 5. Centro: Jogue no centro.
   * 6. Canto oposto: Se o oponente está no canto, jogo no canto oposto.
   * 7. Canto vazio: jogue num canto vazio.
   * 8. Lado vazio: jogue na casa do meio de qualquer dos lados.
   */

}