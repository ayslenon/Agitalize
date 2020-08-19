#include <LiquidCrystal.h>
#include <Keypad.h>

//declaração de E/S

LiquidCrystal lcd(52, 50, 48, 46, 44, 42); //rs, e, d4, d5, d6, d7

//pinos de I/O
const int led1 = 2;
const int led2 = 3;
const int led3 = 4;

const int ledProcesso = 5;

const int agitador = 7;
const int agitador2 = 6;

//declaração de variaveis
//tempo
unsigned long tempo_at; //tempo atual
unsigned long tempo_ant; //tempo anterior

//pot de controle de velocidade
int potag1 = A1;
int potag2 = A2;
int softstart;

//auxilio na escrita de tempo
int posicaoVetor;
int ultPosEscrita;

//variáveis de tempo dos processos
int horas;  // horas do primeiro
int horas2; // horas do segundo
int horasp; // horas que o programa conta do primeiro
int horasp2;// horas que o programa conta do segundo

int minutos;  // minutos do primeiro
int minutos2; // minutos do segundo
int minutosp; // munutos que o programa conta do primeiro
int minutosp2;// minutos que o programa conta do segundo

int segundos;  // segundos do primeiro
int segundos2; // segundos do segundo
int segundosp; // segundos que o programa conta do primeiro
int segundosp2;// segundos que o programa conta do segundo

//variaveis lógicas de controle de processo
boolean digitei;
boolean agitou1;
boolean agitou2;
boolean processo;

//vetores que guardam os dados de tempo de processo

int vetorTempo[] = {0, 0, 0, 0, 0, 0}; //vetor auxiliar no recebimento de dados
int vetorTempo2[] = {0, 0, 0, 0, 0, 0}; //vetor auxiliar no recebimento de dados

char aux; //variavel auxiliar no recebimento de dados;

//configuração teclado matricial
char matrizteclado[4][4] = { //matrix linhas x colunas
  {'1', '2', '3', 'c'},
  {'4', '5', '6', 'e'},
  {'7', '8', '9', 'a'},
  {'l', '0', 'e', 'd'}
};

byte linhas[4] = {22, 24, 26, 28}; //ligação das linhas do teclado com o arduino
byte colunas[4] = {30, 32, 34, 36}; //ligação das colunas do teclado com o arduino

Keypad numeros = Keypad( makeKeymap(matrizteclado), linhas, colunas, 4, 4); //função que cria um teclado virtual, baseado no teclado fisico e algumas variaveis

//declaração de funções
//função de acender os leds indicadores de nivel

int mostraTempo() { //função para mostrar o tempo no lcd

  if (horas > 99) {
    lcd.setCursor(3, 1);
  }
  else {
    lcd.setCursor(4, 1);
  }
  if (horas < 10) {
    lcd.print("0");
  }
  lcd.print(horas);
  lcd.print("h");
  if (minutos < 10) {
    lcd.print("0");
  }
  lcd.print(minutos);
  lcd.print("m");
  if (segundos < 10) {
    lcd.print("0");
  }
  lcd.print(segundos);
  lcd.print("s");
}

int mostraTempo2() { //função para mostrar o tempo no lcd
  if (horas2 > 99) {
    lcd.setCursor(3, 1);
  }
  else {
    lcd.setCursor(4, 1);
  }
  if (horas2 < 10) {
    lcd.print("0");
  }
  lcd.print(horas2);
  lcd.print("h");
  if (minutos2 < 10) {
    lcd.print("0");
  }
  lcd.print(minutos2);
  lcd.print("m");
  if (segundos2 < 10) {
    lcd.print("0");
  }
  lcd.print(segundos2);
  lcd.print("s");
}

int configuraTempo() {
  horas = vetorTempo[0] * 10 + vetorTempo[1];
  minutos = vetorTempo[2] * 10 + vetorTempo[3];
  segundos = vetorTempo[4] * 10 + vetorTempo[5];

  if (segundos > 59) {
    segundos = segundos - 60;
    minutos = minutos + 1;
  }

  if (minutos > 59) {
    minutos = minutos - 60;
    horas = horas + 1;
  }
  lcd.clear();

}

int configuraTempo2() {
  horas2 = vetorTempo2[0] * 10 + vetorTempo2[1];
  minutos2 = vetorTempo2[2] * 10 + vetorTempo2[3];
  segundos2 = vetorTempo2[4] * 10 + vetorTempo2[5];

  if (segundos2 > 59) {
    segundos2 = segundos2 - 60;
    minutos2 = minutos2 + 1;
  }

  if (minutos2 > 59) {
    minutos2 = minutos2 - 60;
    horas2 = horas2 + 1;
  }
  lcd.clear();
}


void setup() {
  //declaração de entradas
  pinMode (potag1, INPUT);
  pinMode (potag2, INPUT);

  //declaração de saidas
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  pinMode (ledProcesso, OUTPUT);
  pinMode (agitador, OUTPUT);
  pinMode (agitador2, OUTPUT);

  //inicialização do display
  lcd.begin(16, 2); //inicia o lcd no tamanho 16x2
  lcd.clear(); //limpa a tela do lcd
  lcd.print("Iniciando");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  delay(100);
  lcd.print(".");
  Serial.begin(9600); //inicia comunicação serial do arduino
}

void loop() {
  digitei = false;
  digitalWrite(led1, HIGH);
  while (digitei == false) {

    delay(1000);

    lcd.setCursor(0, 1);
    lcd.print("Aperte * p ligar");
    delay(500);
    aux = numeros.waitForKey();

    agitou1 = false;
    agitou2 = false;

    if (aux == 'l') {
      digitalWrite(led1, LOW);
      digitalWrite(ledProcesso, LOW);
      digitalWrite(led2, HIGH);
      for (posicaoVetor = 0; posicaoVetor < 6; posicaoVetor++) { // atribui 0 para os valores de todas as posições do vetor
        vetorTempo[posicaoVetor] = 0;
      }

      for (posicaoVetor = 0; posicaoVetor < 6; posicaoVetor++) { // atribui 0 para os valores de todas as posições do vetor
        vetorTempo2[posicaoVetor] = 0;
      }

      for (posicaoVetor = 0; posicaoVetor < 7; posicaoVetor++)  {  //roda 5 ciclos para inserir valor e 1 para confirmar

        configuraTempo();
        lcd.print(" Insira o tempo");
        mostraTempo();

        // indica com um "_" onde o usuario está escrevendo no LCD

        if (posicaoVetor < 2) {
          lcd.setCursor (posicaoVetor + 4, 1);
          lcd.print("_");
        }
        if ((posicaoVetor > 1) && (posicaoVetor < 4)) {
          lcd.setCursor (posicaoVetor + 5, 1);
          lcd.print("_");
        }
        if ((posicaoVetor > 3) && (posicaoVetor < 6)) {
          lcd.setCursor (posicaoVetor + 6, 1);
          lcd.print("_");
        }

        if (posicaoVetor == 6) { // verifica é o ultimo ciclo do for
          configuraTempo();
          lcd.print(" C-conf / A-apag"); // deixa confirmar ou ainda apagar o tempo anterior
          mostraTempo();
          aux = numeros.waitForKey();

          if (aux == 'a') { // se confirmar, sai do for e inicia processo
            posicaoVetor = 8;
            processo = true;
          }
          else if (aux == 'c') { // se apagar, volta para a posição anterior do vetor
            vetorTempo[ultPosEscrita] = 0;
            posicaoVetor = ultPosEscrita - 1;
            ultPosEscrita = ultPosEscrita - 1;
          }
          else { // se digitar outra coisa, indica erro
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            posicaoVetor = posicaoVetor - 1;
            delay(700);
          }
        }

        else { // se não está na etapa de confirma, então está digitando os números para o tempo
          aux = numeros.waitForKey(); // espera o botao ser digitado
          if ((aux - '0' > -1) && (aux - '0' < 10)) { // verifica se foi digitado um numero
            vetorTempo[posicaoVetor] = aux - '0'; // o vetor tempo na posição atual do ciclo do for recebe o valor de aux e converte para inteiro com " - '0' ", segundo a tabela ascii
            ultPosEscrita = posicaoVetor; // ultima posição escrita agora é a posição do vetor
          }

          else if (aux == 'c') {  // apaga numero digitado anteriormente
            if (posicaoVetor < 1) {
              posicaoVetor = posicaoVetor - 1;
            }
            else if (posicaoVetor > 0) {
              vetorTempo[ultPosEscrita] = 0; // vetor na ultima posição escrita vai ter seu valor igual a 0
              posicaoVetor = ultPosEscrita - 1; // posição vetor vai decrementar em 2
              ultPosEscrita = ultPosEscrita - 1; // ultima posicção escrita vai decrementar em 1
            }
            // ultima posição escrita sempre vale 1 a menos que a atual posição do vetor
          }

          else if (aux == 'a') { // se confirmar, igual a posição do vetor em 5, para no final do ciclo do for somar mais 1 e perguntar se quer confirmar
            if (posicaoVetor < 1) { // se ainda estiver no primeiro ciclo do for não deixa dar esse "salto"
              posicaoVetor = posicaoVetor - 1;
            }
            else {
              posicaoVetor = 5;
            }
          }
          else {  // qualquer outra coisa digitada é entendida como 'erro'
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            delay(700);
            posicaoVetor = posicaoVetor - 1;
          }
          //funções para reescrever no lcd os valores atuais;
          configuraTempo();
          mostraTempo();
        }
      }

      for (posicaoVetor = 0; posicaoVetor < 7; posicaoVetor++) {  //roda 5 ciclos para inserir valor e 1 para confirmar
        configuraTempo2();
        lcd.print("Insira o tempo2");
        mostraTempo2();

        // indica com um "_" onde o usuario está escrevendo no LCD

        if (posicaoVetor < 2) {
          lcd.setCursor (posicaoVetor + 4, 1);
          lcd.print("_");
        }
        if ((posicaoVetor > 1) && (posicaoVetor < 4)) {
          lcd.setCursor (posicaoVetor + 5, 1);
          lcd.print("_");
        }
        if ((posicaoVetor > 3) && (posicaoVetor < 6)) {
          lcd.setCursor (posicaoVetor + 6, 1);
          lcd.print("_");
        }

        if (posicaoVetor == 6) { // verifica é o ultimo ciclo do for
          configuraTempo2();
          lcd.print(" C-conf / A-apag"); // deixa confirmar ou ainda apagar o tempo anterior
          mostraTempo2();
          aux = numeros.waitForKey(); // espera o botao ser digitado

          if (aux == 'a') { // se confirmar, sai do for e inicia processo
            posicaoVetor = 8;
            processo = true;
            digitei = true;
          }
          else if (aux == 'c') { // se apagar, volta para a posição anterior do vetor
            vetorTempo2[ultPosEscrita] = 0;
            posicaoVetor = ultPosEscrita - 1;
            ultPosEscrita = ultPosEscrita - 1;
          }
          else { // se digitar outra coisa, indica erro
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            posicaoVetor = posicaoVetor - 1;
            delay(700);
          }
        }

        else { // se não está na etapa de confirma, então está digitando os números para o tempo
          aux = numeros.waitForKey(); // espera o botao ser digitado
          if ((aux - '0' > -1) && (aux - '0' < 10)) { // verifica se foi digitado um numero
            vetorTempo2[posicaoVetor] = aux - '0'; // o vetor tempo na posição atual do ciclo do for recebe o valor de aux e converte para inteiro com " - '0' ", segundo a tabela ascii
            ultPosEscrita = posicaoVetor; // ultima posição escrita agora é a posição do vetor
          }

          else if (aux == 'c') {  // apaga numero digitado anteriormente
            if (posicaoVetor < 1) {
              posicaoVetor = posicaoVetor - 1;
            }
            else if (posicaoVetor > 0) {
              vetorTempo2[ultPosEscrita] = 0; // vetor na ultima posição escrita vai ter seu valor igual a 0
              posicaoVetor = ultPosEscrita - 1; // posição vetor vai decrementar em 2
              ultPosEscrita = ultPosEscrita - 1; // ultima posicção escrita vai decrementar em 1
            }
            // ultima posição escrita sempre vale 1 a menos que a atual posição do vetor
          }

          else if (aux == 'a') { // se confirmar, igual a posição do vetor em 5, para no final do ciclo do for somar mais 1 e perguntar se quer confirmar
            if (posicaoVetor < 1) { // se ainda estiver no primeiro ciclo do for não deixa dar esse "salto"
              posicaoVetor = posicaoVetor - 1;
            }
            else {
              posicaoVetor = 5;
            }
          }
          else {  // qualquer outra coisa digitada é entendida como 'erro'
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            delay(700);
            posicaoVetor = posicaoVetor - 1;
          }
          //funções para reescrever no lcd os valores atuais;
          configuraTempo2();
          mostraTempo2();
        }
      }
    }
    // indica que os processos que foram iniciados ainda não acabaram
    agitou1 = false;
    agitou2 = false;

    // faz um inicio suave dos motores
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Espere um pouco");
    delay (250);

    softstart = 0;
    while ((softstart < analogRead(potag1) / 4) or (softstart < analogRead(potag2) / 4)) {
      if (softstart < analogRead(potag1) / 4) {
        analogWrite (agitador, softstart);
      }
      if (softstart < analogRead(potag2) / 4) {
        analogWrite (agitador2, softstart);
      }
      softstart = softstart + 10;
      delay(500);
    }
    digitalWrite(led2, LOW);
    analogWrite (agitador, analogRead(potag1) / 4);
    analogWrite (agitador2, analogRead(potag2) / 4);

    while (processo) { // inicia o loop principal
      digitalWrite(led3, HIGH);
      if (agitou1 == false) { //verifica se a primeira agitação não acabou ainda
        analogWrite (agitador, analogRead(potag1) / 4); //escreve analogicamente uma tensão para o motor, baseado na leitura do potenciometro
      }

      if (agitou2 == false) { //verifica se a segunda agitação não acabou ainda
        analogWrite (agitador2, analogRead(potag2) / 4); //escreve analogicamente uma tensão para o motor, baseado na leitura do potenciometro
      }

      aux = numeros.getKey();  // verifica se um botão do teclado foi pressionado entre as atualizações periódicas de 1 em 1 segundo;
      if (aux == 'd') { // se D for pressionado, cancela o processo
        processo = false;
        agitou1 = true;
        agitou2 = true;
      }

      tempo_at = millis() - tempo_ant; //define o tempo atual como sendo a contagem da função millis menos o tempo anterior

      //quando se passa 1 segundo entre o tempo anterior e o atual, entramos na etapa de atualizar o LCD
      if (tempo_at >= 1000) {

        lcd.clear();
        lcd.setCursor(4, 0);
        if (horasp > 99) {
          lcd.setCursor(3, 0);
        }
        if (horasp < 10) {
          lcd.print("0");
        }
        lcd.print(horasp);
        lcd.print("h");
        if (minutosp < 10) {
          lcd.print("0");
        }
        lcd.print(minutosp);
        lcd.print("m");
        if (segundosp < 10) {
          lcd.print("0");
        }
        lcd.print(segundosp);
        lcd.print("s");


        lcd.setCursor(4, 1);
        if (horasp2 > 99) {
          lcd.setCursor(3, 1);
        }
        if (horasp2 < 10) {
          lcd.print("0");
        }
        lcd.print(horasp2);
        lcd.print("h");
        if (minutosp2 < 10) {
          lcd.print("0");
        }
        lcd.print(minutosp2);
        lcd.print("m");
        if (segundosp2 < 10) {
          lcd.print("0");
        }
        lcd.print(segundosp2);
        lcd.print("s");

        if ((minutosp > 58) && (segundosp > 59)) { //verifica se os minutos são 60 para o primeiro tempo
          horasp = horasp + 1;
          minutosp = 0;
          segundosp = 0;
        }
        if ((minutosp < 59) && (segundosp > 59)) { //verifica se os segundos são 60 para o primeiro tempo
          minutosp = minutosp + 1;
          segundosp = 0;
        }

        if ((minutosp2 > 58) && (segundosp2 > 59)) { //verifica se os minutos são 60 para o segundo tempo
          horasp2 = horasp2 + 1;
          minutosp2 = 0;
          segundosp2 = 0;
        }
        if ((minutosp2 < 59) && (segundosp2 > 59)) { //verifica se os segundos são 60 para o segundo tempo
          minutosp2 = minutosp2 + 1;
          segundosp2 = 0;
        }

        //verifica se o tempo que o programa conta é maior que o tempo que o usuário escolheu para o primeiro
        if ((horasp == horas) && (minutosp == minutos) && (segundosp == segundos)) {
          agitou1 = true;
          analogWrite (agitador, 0);
          lcd.setCursor(0, 0);
          lcd.print ("processo acabado");
        }
        else {
          segundosp = segundosp + 1; // se nao atingiu o limite, soma mais 1 segundo
        }

        //verifica se o tempo que o programa conta é maior que o tempo que o usuário escolheu para o segundo
        if ((horasp2 == horas2) && (minutosp2 == minutos2) && (segundosp2 == segundos2)) {
          agitou2 = true;
          analogWrite (agitador2, 0);
          lcd.setCursor(0, 1);
          lcd.print ("processo acabado");
        }
        else { // se nao atingiu o limite, soma mais 1 segundo
          segundosp2 = segundosp2 + 1;
        }

        if ((agitou1) && (agitou2)) { // se as duas agitações acabaram, finalizar o processo 'geral'
          processo = false;
        }
        tempo_ant = millis(); //define o tempo anterior como sendo o ultimo milisegundo que executou uma ação na atualização pontual de 1s em 1s
      }
    }
    //verifica se o motivo do processo parar foi o botao D ou o tempo acabar
    if (aux == 'd') { // caso o processo parou forçadamente
      digitalWrite (led1, HIGH);
      digitalWrite (led2, HIGH);
      digitalWrite (led3, HIGH);
      lcd.clear();
      digitalWrite (ledProcesso, LOW);
      lcd.print("Desligando");
      delay(100);
      digitalWrite (led1, LOW);
      digitalWrite (led2, LOW);
      digitalWrite (led3, LOW);
      lcd.print(".");
      delay(100);
      digitalWrite (led1, HIGH);
      digitalWrite (led2, HIGH);
      digitalWrite (led3, HIGH);
      lcd.print(".");
      delay(100);
      digitalWrite (led1, LOW);
      digitalWrite (led2, LOW);
      digitalWrite (led3, LOW);
      lcd.print(".");
      delay(100);
      digitalWrite (led1, HIGH);
      digitalWrite (led2, HIGH);
      digitalWrite (led3, HIGH);
      lcd.print(".");
      delay(100);
      digitalWrite (led1, LOW);
      digitalWrite (led2, LOW);
      digitalWrite (led3, LOW);
      lcd.print(".");
      delay(100);
      digitalWrite (led1, HIGH);
      digitalWrite (led2, HIGH);
      digitalWrite (led3, HIGH);
      lcd.print(".");
      delay(500);
      digitalWrite (led1, LOW);
      digitalWrite (led2, LOW);
      digitalWrite (led3, LOW);
    }
    else { // caso o tempo acabe
      delay (500);
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite (led3, LOW);
      digitalWrite (ledProcesso, HIGH);
      lcd.print("acabou agitacao");
      int buzzer = 11;
      delay (300);
      for (int j = 0; j < 4; j ++) {
        tone(buzzer, 2332);
        delay(80);
        tone(buzzer, 2432);
        delay(80);
      }
      tone(buzzer, 2332);
      delay(100);
      noTone (buzzer);
      delay(500);
    }

    // reset em todos os valores

    segundos = 0;
    minutos = 0;
    horas = 0;
    segundosp = 0;
    minutosp = 0;
    horasp = 0;

    segundos2 = 0;
    minutos2 = 0;
    horas2 = 0;
    segundosp2 = 0;
    minutosp2 = 0;
    horasp2 = 0;

    analogWrite (agitador, 0);
    analogWrite (agitador2, 0);
  }
}
