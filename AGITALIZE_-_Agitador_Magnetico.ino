// Author - Ayslenon
// made in 2018 in arduino ide for arduino mega

//==============================================================================================
// libs includes
//==============================================================================================
#include <LiquidCrystal.h>
#include <Keypad.h>



//==============================================================================================
// i/o declarations
//==============================================================================================

LiquidCrystal lcd(52, 50, 48, 46, 44, 42); //rs, e, d4, d5, d6, d7 pins

// i/o arduino pins

const int led1 = 2;
const int led2 = 3;
const int led3 = 4;

const int ledProcess = 5;

const int stirrer = 7;
const int stirrer2 = 6;


//==============================================================================================
// variables declarations 
//==============================================================================================

// time variables 
unsigned long tempo_at; // actual time 
unsigned long tempo_ant; // last time

// velocity control potentiometer variables

int potag1 = A1;
int potag2 = A2;
int softstart;

// time writing assistance
int vector_position;
int lastWritenPosition;

// time variables in processes
int hours;  // first process hours
int hours2; // second process hours
int hoursp; // program hours of the first process
int hoursp2;// program hours of the second process

int minutes;  // first process minutes
int minutes2; // second process minutes
int minutesp; // program minutes of the first process
int minutesp2;// program minutes of the second process

int seconds;  // first process seconds
int seconds2; // second process seconds 
int secondsp; // program seconds of the first process
int secondsp2;// program seconds of the second process

// logic variables of processes control
boolean started; // indicates system started
boolean stirred1;// indicates process one finished or was forced to stop 
boolean stirred2;// indicates process two finished or was forced to stop
boolean process_started; // indicate one process started

// vectors that stores time process data

int time_vector[] = {0, 0, 0, 0, 0, 0}; //auxiliar vector of incoming data
int time_vector2[] = {0, 0, 0, 0, 0, 0}; //auxiliar vector of incoming data

char aux; //auxiliar variable of incoming data

// layout matrix keyboard
char keyboardMatrix[4][4] = { //matrix rows x columns
  {'1', '2', '3', 'c'},
  {'4', '5', '6', 'e'},
  {'7', '8', '9', 'a'},
  {'l', '0', 'e', 'd'}
};

//rows and columns wiring to arduino 

byte rows[4] = {22, 24, 26, 28};
byte columns[4] = {30, 32, 34, 36}; 

Keypad numbersKeypad = Keypad( makeKeymap(keyboardMatrix), rows, columns, 4, 4);

//==============================================================================================
// functions declarations
//==============================================================================================


int showTime() { //function to show the time1 on lcd

  if (hours > 99) {
    lcd.setCursor(3, 1);
  }
  else {
    lcd.setCursor(4, 1);
  }
  if (hours < 10) {
    lcd.print("0");
  }
  lcd.print(hours);
  lcd.print("h");
  if (minutes < 10) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print("m");
  if (seconds < 10) {
    lcd.print("0");
  }
  lcd.print(seconds);
  lcd.print("s");
}

int showTime2() { //function to show the time2 on lcd
  if (hours2 > 99) {
    lcd.setCursor(3, 1);
  }
  else {
    lcd.setCursor(4, 1);
  }
  if (hours2 < 10) {
    lcd.print("0");
  }
  lcd.print(hours2);
  lcd.print("h");
  if (minutes2 < 10) {
    lcd.print("0");
  }
  lcd.print(minutes2);
  lcd.print("m");
  if (seconds2 < 10) {
    lcd.print("0");
  }
  lcd.print(seconds2);
  lcd.print("s");
}

// function to configure the time that the user entered for time 1
int setTime() {
  hours = time_vector[0] * 10 + time_vector[1];
  minutes = time_vector[2] * 10 + time_vector[3];
  seconds = time_vector[4] * 10 + time_vector[5];

  if (seconds > 59) {
    seconds = seconds - 60;
    minutes = minutes + 1;
  }

  if (minutes > 59) {
    minutes = minutes - 60;
    hours = hours + 1;
  }
  lcd.clear();

}

// function to configure the time that the user entered for time 2
int setTime2() {
  hours2 = time_vector2[0] * 10 + time_vector2[1];
  minutes2 = time_vector2[2] * 10 + time_vector2[3];
  seconds2 = time_vector2[4] * 10 + time_vector2[5];

  if (seconds2 > 59) {
    seconds2 = seconds2 - 60;
    minutes2 = minutes2 + 1;
  }

  if (minutes2 > 59) {
    minutes2 = minutes2 - 60;
    hours2 = hours2 + 1;
  }
  lcd.clear();
}


void setup() {
  // inputs declaration
  pinMode (potag1, INPUT);
  pinMode (potag2, INPUT);

  // outputs declaration
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  pinMode (ledProcess, OUTPUT);
  pinMode (stirrer, OUTPUT);
  pinMode (stirrer2, OUTPUT);


  // display initialization
  lcd.begin(16, 2); 
  lcd.clear();
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
  Serial.begin(9600); // starts serial communication on arduino
}

void loop() {
  started = false;
  digitalWrite(led1, HIGH);
  // we start with both stirrers turned off
  while (started == false) {

    delay(1000);

    lcd.setCursor(0, 1);
    lcd.print("Aperte * p ligar");
    delay(500);
    // wait until user press '*' button on physical keyboard, and is 'l' for the code
    aux = numbersKeypad.waitForKey();

    stirred1 = false;
    stirred2 = false;
    // if '*' is pressed...
    if (aux == 'l') {
      digitalWrite(led1, LOW);
      digitalWrite(ledProcess, LOW);
      digitalWrite(led2, HIGH);
      // set 0 for all values on the auxiliar vectors
      for (vector_position = 0; vector_position < 6; vector_position++) { 
        time_vector[vector_position] = 0;
      }

      for (vector_position = 0; vector_position < 6; vector_position++) { 
        time_vector2[vector_position] = 0;
      }

      // insert 6 values for the first timer, and in the last run expect a "confirm" button
      for (vector_position = 0; vector_position < 7; vector_position++)  { 
        
        setTime(); // make all values from time equal 0, because the auxiliar vector has 0 in all positions 
        lcd.print(" Insira o tempo");
        showTime();

        // indicates with "_" where the user is writing on LCD

        if (vector_position < 2) {
          lcd.setCursor (vector_position + 4, 1);
          lcd.print("_");
        }
        if ((vector_position > 1) && (vector_position < 4)) {
          lcd.setCursor (vector_position + 5, 1);
          lcd.print("_");
        }
        if ((vector_position > 3) && (vector_position < 6)) {
          lcd.setCursor (vector_position + 6, 1);
          lcd.print("_");
        }

        if (vector_position == 6) { // verify if is the last run of for loop
          setTime(); // set the values for timer 1
          // NOTE: IN HARDWARE, C AND A BUTTON ARE DIFFERENT THAN THE CODE
          // PHYSIC 'A' CORRESPOND TO VIRTUAL C, AND VICE-VERSA 
          lcd.print(" C-conf / A-apag"); 
          showTime();
          // wait for a confirm button 'c' or a erase button 'a'
          aux = numbersKeypad.waitForKey();

          if (aux == 'a') { // if confirm, exit for and start the process
            vector_position = 8;
            process_started = true;
          }
          else if (aux == 'c') { // if erase, go back in one position to rewrite the number
            time_vector[lastWritenPosition] = 0;
            vector_position = lastWritenPosition - 1;
            lastWritenPosition = lastWritenPosition - 1;
          }
          else { // if other, show an error message and wait a new button press
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            vector_position = vector_position - 1;
            delay(700);
          }
        }

        // if it aint the last for run, waits numbers to the timer 1
        else {
          aux = numbersKeypad.waitForKey(); 
          if ((aux - '0' > -1) && (aux - '0' < 10)) { // verify if its a valid number
            time_vector[vector_position] = aux - '0'; // save in auxiliar vector its number
            lastWritenPosition = vector_position; // and set the lastWritenposition
          }

          else if (aux == 'c') {  // if the button pressed is 'c', it erase the last number Writen
            if (vector_position < 1) {
              vector_position = vector_position - 1;
            }
            else if (vector_position > 0) {
              // erase the last Writen number
              // and go back in one position in the vector 
              time_vector[lastWritenPosition] = 0; 
              vector_position = lastWritenPosition - 1;
              lastWritenPosition = lastWritenPosition - 1;
            }
            // lastWritenposition is always vector_position - 1
          }

          // if the button pressed is 'a', it confirms the actual vector value, and skip the next inputs configuration 
          else if (aux == 'a') { 
            if (vector_position < 1) { // but only is valid if you write at least one number
              vector_position = vector_position - 1;
            }
            else {
              vector_position = 5;
            }
          }
          else {  // if you type anything else, an error message is shown 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            delay(700);
            vector_position = vector_position - 1;
          }
          // then we show on the lcd the values seted
          setTime();
          showTime();
        }
      }


      // reapeat the same, but for timer 2
      // insert 6 values for the second timer, and in the last run expect a "confirm" button
      for (vector_position = 0; vector_position < 7; vector_position++) { 
        
        // make all values from time2 equal 0, because the auxiliar vector2 has 0 in all positions
        setTime2();
        lcd.print("Insira o tempo2");
        showTime2();

        // indicates with "_" where the user is writing on LCD

        if (vector_position < 2) {
          lcd.setCursor (vector_position + 4, 1);
          lcd.print("_");
        }
        if ((vector_position > 1) && (vector_position < 4)) {
          lcd.setCursor (vector_position + 5, 1);
          lcd.print("_");
        }
        if ((vector_position > 3) && (vector_position < 6)) {
          lcd.setCursor (vector_position + 6, 1);
          lcd.print("_");
        }

        if (vector_position == 6) { // verify if is the last run of for loop
          setTime2(); // set the values for timer 2
          // NOTE: IN HARDWARE, C AND A BUTTON ARE DIFFERENT THAN THE CODE
          // PHYSIC 'A' CORRESPOND TO VIRTUAL C, AND VICE-VERSA 
          lcd.print(" C-conf / A-apag"); 
          showTime2();
          // wait for a confirm button 'c' or a erase button 'a'
          aux = numbersKeypad.waitForKey(); 

          if (aux == 'a') { // if confirm, exit for and start the process
            vector_position = 8;
            process_started = true;
            started = true;
          }
          else if (aux == 'c') { // if erase, go back in one position to rewrite the number
            time_vector2[lastWritenPosition] = 0;
            vector_position = lastWritenPosition - 1;
            lastWritenPosition = lastWritenPosition - 1;
          }
          else { // if other, show an error message and wait a new button press
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            vector_position = vector_position - 1;
            delay(700);
          }
        }
        
        // if it aint the last for run, waits numbers to the timer 1
        else { 
          aux = numbersKeypad.waitForKey();
          if ((aux - '0' > -1) && (aux - '0' < 10)) { // verify if its a valid number
            time_vector2[vector_position] = aux - '0'; // save in auxiliar vector its number
            lastWritenPosition = vector_position; // and set the lastWritenposition
          }

          else if (aux == 'c') {  // if the button pressed is 'c', it erase the last number Writen
            if (vector_position < 1) {
              vector_position = vector_position - 1;
            }
            else if (vector_position > 0) {
              // erase the last Writen number
              // and go back in one position in the vector 
              time_vector2[lastWritenPosition] = 0; 
              vector_position = lastWritenPosition - 1;
              lastWritenPosition = lastWritenPosition - 1;
            }
            // lastWritenposition is always vector_position - 1
          }
          
          // if the button pressed is 'a', it confirms the actual vector value, and skip the next inputs configuration 
          else if (aux == 'a') { 
            if (vector_position < 1) { // but only is valid if you write at least one number
              vector_position = vector_position - 1;
            }
            else {
              vector_position = 5;
            }
          }
          else {  // if you type anything else, an error message is shown
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("digite novamente");
            delay(700);
            vector_position = vector_position - 1;
          }
          // then we show on the lcd the values seted
          setTime2();
          showTime2();
        }
      }
    }
    // indicates that the started processes still running
    stirred1 = false;
    stirred2 = false;

    // this is a simple try to soft start to the DC motors
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Espere um pouco");
    delay (250);

    softstart = 0;
    while ((softstart < analogRead(potag1) / 4) or (softstart < analogRead(potag2) / 4)) {
      if (softstart < analogRead(potag1) / 4) {
        analogWrite (stirrer, softstart);
      }
      if (softstart < analogRead(potag2) / 4) {
        analogWrite (stirrer2, softstart);
      }
      softstart = softstart + 10;
      delay(500);
    }
    digitalWrite(led2, LOW);
    analogWrite (stirrer, analogRead(potag1) / 4);
    analogWrite (stirrer2, analogRead(potag2) / 4);

    while (process_started) { // here is the main loop
      digitalWrite(led3, HIGH);
      // if a process is running, we can change the dc motor velocity in real time (1s delay)
      if (stirred1 == false) { 
        analogWrite (stirrer, analogRead(potag1) / 4); 
      }

      if (stirred2 == false) { 
        analogWrite (stirrer2, analogRead(potag2) / 4);
      }

      aux = numbersKeypad.getKey();  
      // if we press 'd' button, we stop both processes
      if (aux == 'd') { 
        process_started = false;
        stirred1 = true;
        stirred2 = true;
      }

      // simple time counting  
      tempo_at = millis() - tempo_ant;

      // when passes 1 second, we atualize the LCD
      if (tempo_at >= 1000) {

        lcd.clear();
        lcd.setCursor(4, 0);
        if (hoursp > 99) {
          lcd.setCursor(3, 0);
        }
        if (hoursp < 10) {
          lcd.print("0");
        }
        lcd.print(hoursp);
        lcd.print("h");
        if (minutesp < 10) {
          lcd.print("0");
        }
        lcd.print(minutesp);
        lcd.print("m");
        if (secondsp < 10) {
          lcd.print("0");
        }
        lcd.print(secondsp);
        lcd.print("s");


        lcd.setCursor(4, 1);
        if (hoursp2 > 99) {
          lcd.setCursor(3, 1);
        }
        if (hoursp2 < 10) {
          lcd.print("0");
        }
        lcd.print(hoursp2);
        lcd.print("h");
        if (minutesp2 < 10) {
          lcd.print("0");
        }
        lcd.print(minutesp2);
        lcd.print("m");
        if (secondsp2 < 10) {
          lcd.print("0");
        }
        lcd.print(secondsp2);
        lcd.print("s");

        if ((minutesp > 58) && (secondsp > 59)) { 
          hoursp = hoursp + 1;
          minutesp = 0;
          secondsp = 0;
        }
        if ((minutesp < 59) && (secondsp > 59)) { 
          minutesp = minutesp + 1;
          secondsp = 0;
        }

        if ((minutesp2 > 58) && (secondsp2 > 59)) { 
          hoursp2 = hoursp2 + 1;
          minutesp2 = 0;
          secondsp2 = 0;
        }
        if ((minutesp2 < 59) && (secondsp2 > 59)) { 
          minutesp2 = minutesp2 + 1;
          secondsp2 = 0;
        }

        // verify if its time to stop for both counters, comparing the program time with user time
        if ((hoursp == hours) && (minutesp == minutes) && (secondsp == seconds)) {
          stirred1 = true;
          analogWrite (stirrer, 0);
          lcd.setCursor(0, 0);
          lcd.print ("processo acabado");
        }
        else {
          secondsp = secondsp + 1; 
        }

        if ((hoursp2 == hours2) && (minutesp2 == minutes2) && (secondsp2 == seconds2)) {
          stirred2 = true;
          analogWrite (stirrer2, 0);
          lcd.setCursor(0, 1);
          lcd.print ("processo acabado");
        }
        else { 
          secondsp2 = secondsp2 + 1;
        }
        // wait both timers stop to proceed 
        if ((stirred1) && (stirred2)) { 
          process_started = false;
        }
        tempo_ant = millis(); 
      }
    }
    
    //verify if the stop reason was the 'd' button pressed or the time reached
    if (aux == 'd') { 
      digitalWrite (led1, HIGH);
      digitalWrite (led2, HIGH);
      digitalWrite (led3, HIGH);
      lcd.clear();
      digitalWrite (ledProcess, LOW);
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
    else { 
      delay (500);
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite (led3, LOW);
      digitalWrite (ledProcess, HIGH);
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

    // reset all values

    seconds = 0;
    minutes = 0;
    hours = 0;
    secondsp = 0;
    minutesp = 0;
    hoursp = 0;

    seconds2 = 0;
    minutes2 = 0;
    hours2 = 0;
    secondsp2 = 0;
    minutesp2 = 0;
    hoursp2 = 0;

    analogWrite (stirrer, 0);
    analogWrite (stirrer2, 0);
  }
}
