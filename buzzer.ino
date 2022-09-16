// Alarme se um botão de uma
// casa ocupada é acionado
void buzzer() {
  tone(pinoBuzzer, NOTE_F6);
  delay(100);
  noTone(pinoBuzzer);
  return;
}

void conta(int vezes) {
  int i;
  int notas[] = { NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_DS5 };
  for (i = 0; i < vezes; i++) {
    tone(pinoBuzzer, notas[vezes - 1]);
    delay(100);
    noTone(pinoBuzzer);
    delay(100);
  }
}

// Toca uma musica no buzzer
// Baseado em: https://github.com/robsoncouto/arduino-songs
void toca(int melodia[], int notas) {
  int notaInteira = (60000 * 4) / tempo,
      divisor = 0, duracaoNota = 0, notaAtual;

  for (notaAtual = 0; notaAtual < notas * 2; notaAtual += 2) {
    // Calcula a duração de cada nota
    divisor = melodia[notaAtual + 1];
    if (divisor > 0) {
      duracaoNota = (notaInteira) / divisor;
    } else if (divisor < 0) {
      // dotted notes are represented with negative durations!!
      duracaoNota = (notaInteira) / abs(divisor);
      duracaoNota *= 1.5; // increases the duration in half for dotted notes
    }

    // Toca apenas 90% da duração da nota
    tone(pinoBuzzer, melodia[notaAtual], duracaoNota * 0.9);
    delay(duracaoNota);
    noTone(pinoBuzzer);
  }
}