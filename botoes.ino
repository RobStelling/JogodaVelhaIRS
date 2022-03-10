// Lê estado de um botão
bool leBotao(int casa) {
  return !digitalRead(botao(casa));
}
// ** Botão correspondente de uma casa
int botao(int casa) {
  return casa + botao1 - 1;
}