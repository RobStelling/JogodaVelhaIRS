// Lê estado de um botão
bool leBotao(int casa) {
  return leBotaoPorta(porta(casa));
}
// Porta correspondente ao botão de uma casa
int porta(int casa) {
  return casa + botao1 - 1;
}

// Lê o valor de um botão em uma dada porta
bool leBotaoPorta(int porta) {
  return !digitalRead(porta);
}