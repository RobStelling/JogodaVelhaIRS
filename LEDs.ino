// Funções básicas de LEDs

// ** Converte casa para o pino do LED correspondente O ou X
int LEDO(int casa) {
  return casa * 2 + (o1-2);
}

int LEDX(int casa) {
  return casa * 2 + (x1-2);
}

// ** Acende e apagaLEDs (por casa)
void acende(int lado, int casa) {
  if (lado & X)
    digitalWrite(LEDX(casa), HIGH);
  if (lado & O)
    digitalWrite(LEDO(casa), HIGH);
}

void apaga(int lado, int casa) {
  if (lado & X)
    digitalWrite(LEDX(casa), LOW);
  if (lado & O)
    digitalWrite(LEDO(casa), LOW);
}

// ** Apaga todos os LEDs
void apagaLEDs() {
  int casa;
  for (casa = 1; casa <= NUMCASAS; casa++) {
    apaga(X|O, casa);
    apaga(X|O, casa);
  }
}

/**
 ** Pisca os LEDs
 ** Se o parâmetro intervalo for muito pequeno temos a
 ** impressão que os LEDs não estão piscando alternadamente
 **/
void pisca(int vezes, int intervalo, int lado, int leds) {
/****
 * Pisca todos os LEDs
 * vezes: Número de vezes que LEDs irão piscar
 * intervalo: Intervalo entre as piscadas de cada LED
 * lado: Lado que irá piscar (X, O ou ambos - [X|O])
 * leds: Bitmap de LEDs que irão piscar (Exemplo em binário: 111 pisca apenas os 3 primeiros leds)
 *
 ****/
  int casa, i, k;

  for (i = 0; i < vezes; i++) {
    for (k = casa = 1; casa <= NUMCASAS; casa++, k<<=1) {
      if (k & leds) {
        if (lado & X) {
          acende(X, casa);
        }
        if (lado&O) {
          acende(O, casa);
        }
        delay(intervalo);
        if (lado & X) {
          apaga(X, casa);
        }
        if (lado & O) {
          apaga(O, casa);
        }
      }
    }
  }
}

// Liga LEDs de acordo com um bitmap
void liga(int lado, int leds) {
  int k, casa;
  for (k = casa = 1; casa <= NUMCASAS; casa++, k<<=1) {
    if (k & leds) {
      if (lado & X)
        acende(X, casa);
      if (lado & O)
        acende(O, casa);
    }
  }
}