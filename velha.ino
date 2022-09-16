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
 * 4: O - Humano
 * Retorno: número da casa (entre 1 e 9) onde o computador jogará
 */
#define ERRO -1

void mesclaVetor(int *vetor, int tamanho) {
  // Retorna o vetor com o conteúdo reordenado
  // aleatoriamente
  int i, j, temp;
  
  for (i = 0; i<tamanho; i++) {
    j = rand() % tamanho;
    temp = vetor[j];
    vetor[j] = vetor[i];
    vetor[i] = temp;
  }
}

int opostoDiagonal(int canto) {
  // Retorna o canto oposto de um canto
  switch (canto) {
    case 1: return 9;
    case 3: return 7;
    case 7: return 3;
    case 9: return 1;
  }
  return ERRO;
}

// Usado nas regras 1 e 2
int faltaUm(int tabuleiro[], int lado) {
  // Retorna a primeira casa que encontrar em que falte
  // uma peça para o lado ganhar
  int i;
  // Horizontal
  for (i = 0; i < NUMCASAS; i+=3) {
    if (tabuleiro[i]+tabuleiro[i+1]+tabuleiro[i+2] == 2*lado)
      return tabuleiro[i] == VAZIO ? i+1 : (tabuleiro[i+1] == VAZIO ? i+2 : i+3);
  }
  // Vertical
  for (i = 0; i < 3; i++) {
    if (tabuleiro[i]+tabuleiro[i+3]+tabuleiro[i+6] == 2*lado)
      return tabuleiro[i] == VAZIO ? i+1 : (tabuleiro[i+3] == VAZIO ? i+4 : i+7);
  }
  // Diagonal principal
  if (tabuleiro[0]+tabuleiro[4]+tabuleiro[8] == 2*lado)
    return tabuleiro[0] == VAZIO ? 1 : (tabuleiro[4] == VAZIO ? 5 : 9);
  // Diagonal secundária
  if (tabuleiro[2]+tabuleiro[4]+tabuleiro[6] == 2*lado)
    return tabuleiro[2] == VAZIO ? 3 : (tabuleiro[4] == VAZIO ? 5 : 7);
  // Não tem casa vazia para completar
  return ERRO;
}

// Usado na regra 3
bool pertence(int j, int vetor[]) {
  int i;
  for (i = 0; vetor[i] != ERRO; i++)
    if (vetor[i] == j)
      return true;
  return false;
}

// Usado na regra 3 e 4
void triangulos(int tabuleiro[], int lado, int vertices[]) {
  int i = 0;
  if (tabuleiro[0] == VAZIO &&
     (tabuleiro[1]+tabuleiro[2] == lado && tabuleiro[4]+tabuleiro[8] == lado ||
      tabuleiro[1]+tabuleiro[2] == lado && tabuleiro[3]+tabuleiro[6] == lado ||
      tabuleiro[4]+tabuleiro[8] == lado && tabuleiro[3]+tabuleiro[6] == lado))
    vertices[i++] = 1;

  if (tabuleiro[1] == VAZIO && (tabuleiro[0]+tabuleiro[2] == lado && tabuleiro[4]+tabuleiro[7] == lado))
    vertices[i++] = 2;

  if (tabuleiro[2] == VAZIO &&
     (tabuleiro[0]+tabuleiro[1] == lado && tabuleiro[4]+tabuleiro[6] == lado ||
      tabuleiro[0]+tabuleiro[1] == lado && tabuleiro[5]+tabuleiro[8] == lado ||
      tabuleiro[4]+tabuleiro[6] == lado && tabuleiro[5]+tabuleiro[8] == lado))
    vertices[i++] = 3;

  if (tabuleiro[3] == VAZIO && (tabuleiro[0]+tabuleiro[6] == lado && tabuleiro[4]+tabuleiro[5] == lado))
    vertices[i++] = 4;

  // Não é necessário testar para a casa 5 porque esta casa será sempre ocupada
  // antes de ser possível formar um triângulo (no primeiro lance do humano ou do computador)
  
  if (tabuleiro[5] == VAZIO && (tabuleiro[2]+tabuleiro[8] == lado && tabuleiro[3]+tabuleiro[4] == lado))
    vertices[i++] = 6;
  
  if (tabuleiro[6] == VAZIO &&
     (tabuleiro[0]+tabuleiro[3] == lado && tabuleiro[4]+tabuleiro[2] == lado ||
      tabuleiro[0]+tabuleiro[3] == lado && tabuleiro[7]+tabuleiro[8] == lado ||
      tabuleiro[4]+tabuleiro[2] == lado && tabuleiro[7]+tabuleiro[8] == lado))
    vertices[i++] = 7;

  if (tabuleiro[7] == VAZIO && (tabuleiro[1]+tabuleiro[4] == lado && tabuleiro[6]+tabuleiro[8] == lado))
    vertices[i++] = 8;

  if (tabuleiro[8] == VAZIO &&
     (tabuleiro[2]+tabuleiro[5] == lado && tabuleiro[0]+tabuleiro[4] == lado ||
      tabuleiro[2]+tabuleiro[5] == lado && tabuleiro[6]+tabuleiro[7] == lado ||
      tabuleiro[0]+tabuleiro[4] == lado && tabuleiro[6]+tabuleiro[7] == lado))
    vertices[i++] = 9;

  vertices[i] = ERRO;
}

void printVetor(int vetor[]) {
  int i;
  for (i = 0; vetor[i] != ERRO; i++) {
    Serial.println(vetor[i]);
  }
}

// Usado na regra 4
int completa(int tabuleiro[], int a, int b, int c, int lado, int proibido[]) {
  // Ameaça completar uma linha desde que a ameaça
  // não seja em uma das casas proibidas
  if (tabuleiro[a] == lado) {
    if (!pertence(b+1, proibido))
      return c+1;
    if (!pertence(c+1, proibido))
      return b+1;
  }

  if (tabuleiro[b] == lado) {
    if (!pertence(a+1, proibido))
      return c+1;
    if (!pertence(c+1, proibido))
      return a+1;
  }

  if (tabuleiro[c] == lado) {
    if (!pertence(a+1, proibido))
      return b+1;
    if (!pertence(b+1, proibido))
      return a+1;
  }

  return ERRO;
}

int ameaca3(int tabuleiro[], int lado, int proibido[]) {
  // Retorna uma casa em que o lado ameaça completar 3 mas a casa que completa
  // não está na lista de casas proibidas
  int i, casa;

  // Horizontais
  for (i = 0; i < NUMCASAS; i+=3) {
    if (tabuleiro[i]+tabuleiro[i+1]+tabuleiro[i+2] == lado)
      if ((casa = completa(tabuleiro, i, i+1, i+2, lado, proibido)) != ERRO)
        return casa;
  }
  // Vertical
  for (i = 0; i < 3; i++) {
    if (tabuleiro[i]+tabuleiro[i+3]+tabuleiro[i+6] == lado)
      if ((casa = completa(tabuleiro, i, i+3, i+6, lado, proibido)) != ERRO)
        return casa;
  }
  // Diagonal principal
  if (tabuleiro[0]+tabuleiro[4]+tabuleiro[8] == lado)
      if ((casa = completa(tabuleiro, 0, 4, 8, lado, proibido)) != ERRO)
        return casa;
  // Diagonal secundária
  if (tabuleiro[2]+tabuleiro[4]+tabuleiro[6] == 2*lado)
      if ((casa = completa(tabuleiro, 2, 4, 6, lado, proibido)) != ERRO)
        return casa;
  // Não tem casa vazia para completar
  return ERRO;
}

// Usado na regra 6
int cantoOposto(int tabuleiro[], int lado) {
  // Retorna um canto onde o canto oposto está ocupado pelo oponente
  // Cantos 1, 3, 7, 9
  int i, outroLado, canto[] = {1, 3, 7, 9};

  // Muda a ordem dos cantos, usado para evitar
  // que o algoritmo jogue sempre da mesma forma
  mesclaVetor(canto, 4);
  outroLado = (lado == X ? O : X);
  for (i = 0; i<4; i++) {
    if (tabuleiro[canto[i]-1] == VAZIO && tabuleiro[opostoDiagonal(canto[i])-1] == outroLado)
      return canto[i];
  }
  return ERRO;
}

// Usado na regra 7
int cantoVazio(int tabuleiro[]) {
  // Retorna um canto vazio disponível
  // Cantos 1, 3, 7, 9
  int i, canto[] = {1, 3, 7, 9};

  // Muda a ordem dos cantos, usado para evitar
  // que o algoritmo jogue sempre da mesma forma
  mesclaVetor(canto, 4);
  for (i = 0; i<4; i++) {
    if (tabuleiro[canto[i]-1] == VAZIO)
      return canto[i];
  }
  return ERRO;
}

// Usado na regra 8
int ladoVazio(int tabuleiro[]) {
  // Lados 2, 4, 6, 8
  int i, lados[] = {2, 4, 6, 8};

  // Muda a ordem dos lados, usado para evitar
  // que o algoritmo jogue sempre da mesma forma
  mesclaVetor(lados, 4);
  for (i = 0; i<4; i++) {
    if (tabuleiro[lados[i]-1] == VAZIO)
      return lados[i];
  }
  return ERRO;
}

// Algoritmos do jogo da velha
int velhaRandomico(int tabuleiro[]) {
  // Joga randomicamente

  int casa;
  // Cuidado: risco de loop infinito
  do {
    casa = random(NUMCASAS);
  } while (tabuleiro[casa]);
  return casa+1;
}

int velha1(int tabuleiro[]) {
  // Se puder ganhar ganha
  // Senão joga randomicamente
  // Aplica apenas regra 1 de Newel & Simon
  int casa;

  // Regra 1
  if ((casa = faltaUm(tabuleiro, X)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
}

int velha2(int tabuleiro[]) {
  // Se puder, bloqueia vitória do adversário
  // Senão joga randomicamente
  // Aplica apenas regra 2 de Newel & Simon
  int casa;

  // Regra 2
  if ((casa = faltaUm(tabuleiro, O)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
}

int velha1e2(int tabuleiro[]) {
  // Se puder ganhar ganha
  // Se puder bloquear, bloqueia
  // Senão joga randomicamente
  // Aplica regras 1 e 2 de Newel & Simon
  int casa;

  // Regra 1
  if ((casa = faltaUm(tabuleiro, X)) != ERRO)
    return casa;
  // Regra 2
  if ((casa = faltaUm(tabuleiro, O)) != ERRO)
    return casa;

  return velhaRandomico(tabuleiro);
}

int velha1e2e5(int tabuleiro[]) {
  // Se puder ganhar, ganha
  // Se puder bloquear o adversário bloqueia
  // Joga no centro se estiver disponível
  // Senão joga randomicamente
  // Aplica regras 1, 2 e 5 de Newel e Simon

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

  return velhaRandomico(tabuleiro);
}

int velhaNewellESimon(int tabuleiro[], int nivel) {
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
  int casa, vertices[7];

  if (nivel > 1) {
    // Regra 1
    // 1. Ganhar: Se você tem duas peças numa linha, ponha a terceira.
    if ((casa = faltaUm(tabuleiro, X)) != ERRO)
      return casa;
    if (nivel > 2) {
      // Regra 2
      // 2. Bloquear: Se o oponente tiver duas peças em linha, ponha a terceira para bloqueá-lo.
      if ((casa = faltaUm(tabuleiro, O)) != ERRO)
        return casa;
      if (nivel > 3) {
        // Regra 3
        // 3. Triângulo: Crie uma oportunidade em que você poderá ganhar de duas maneiras.
        triangulos(tabuleiro, X, vertices);
        if (vertices[0] != ERRO) { // Tem pelo menos um triângulo
          return vertices[0];
        }
        if (nivel > 4) {
          // Regra 4
          // 4. Bloquear o Triângulo do oponente
          triangulos(tabuleiro, O, vertices);

          if (vertices[0] != ERRO) { // Oponente tem pelo menos um triângulo em potencial
            // Opção 1: Crie 2 peças em linha para forçar o oponente a se defender, contanto que não resulte nele criando um triângulo ou vencendo.
            if ((casa = ameaca3(tabuleiro, X, vertices)) != ERRO)
              return casa;
            // Opção 2: Se existe uma configuração em que o oponente pode formar um triângulo, bloqueiem-no.
            if (vertices[1] == ERRO) { // Só tem um triangulo em potencial
              // Então bloqueia o triângulo!
              return vertices[0];
            }
          }
          if (nivel > 5) {
            // Regra 5
            // 5. Centro: Jogue no centro.
            if (tabuleiro[4] == VAZIO)
              return 5;
            if (nivel > 6) {
              // Regra 6
              // 6. Canto oposto: Se o oponente está no canto, jogo no canto oposto.
              if ((casa = cantoOposto(tabuleiro, X)) != ERRO)
                return casa;
              if (nivel > 7) {
                // Regra 7
                // 7. Canto vazio: jogue num canto vazio.
                if ((casa = cantoVazio(tabuleiro)) != ERRO)
                  return casa;
                if (nivel > 8) {
                  // Regra 8
                  // 8. Lado vazio: jogue na casa do meio de qualquer dos lados.
                  if ((casa = ladoVazio(tabuleiro)) != ERRO)
                    return casa;
                }
              }
            }
          }
        }
      }
    }
  }
  return velhaRandomico(tabuleiro);
}