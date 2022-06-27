

#include<LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define TAM 27
#define TAMLETRA 10
#define BUTTONRIGHT 8
#define BUTTONOK 9
#define BUTTONLEFT 10


//LiquidCrystal(rs, enable, d4, d5, d6, d7)

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

char abc[TAM]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char lifes[4]="***";
char palabraTres[3][TAMLETRA]={"DAVIDGOOD","HOLAMUNDO","PROGRAMAR"};
char palabra[TAMLETRA];
char palabraDos[TAMLETRA]="_________";
unsigned long millisB = 0;

int contador = 0;
int buttonRightBefore = 0;
int buttonLeftBefore = 0;
int buttonOkBefore = 0;
int buttonOn=0;
int contadorDos=1;
int flag=0;
int flagWin=0;
int flagLose=0;
int flagSelect=0;
int contLife=3;
int r=1;
int flagReset=0;
unsigned long millisC=0;

void setup()
{
  lcd.begin(16, 2);
  pinMode(8,INPUT); //DERECHA
  pinMode(9,INPUT);//OK
  pinMode(10,INPUT);//IZQUIERDA
}

//	@brief	Setea las vidas al iniciar el juego
void setLifes(){
  lcd.setCursor(10,0);
  lcd.print("LP:");
  lcd.print(lifes);
}

//	@brief Setea el abc al iniciar el juego
void setAbc(){
  lcd.setCursor(6,0);
  lcd.print(abc[contador]);
}

//	@brief Setea la palabra mostrando solamente la primer y ultima letra al comiendo del juego
void setWord(){
  strcpy(palabra,palabraTres[r]);
  int len = strlen(palabra);
  palabraDos[0]=palabra[0];
  palabraDos[len-1]=palabra[len-1];
  lcd.setCursor(4,1);
  lcd.print(palabraDos);
}

//	@brief Limpia la pantalla
void cleanScreen(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

//	@brief Permite deslizarse en el abc o en el las opciones de palabras con el boton derecho
void buttonRight()
{
  short buttonNow = digitalRead(BUTTONRIGHT);

  if (buttonNow == HIGH && buttonRightBefore == LOW)
  {
    if(flagSelect==0){
      if(r == 2)
      {
        r=0;
      }
      else{
        r++;
      }
    }
    else if(contador == 25)
    {
      contador = 0;
    }
    else{
      contador++;
    }
  }
  buttonRightBefore = buttonNow;
}

//	@brief Permite deslizarse en el abc o en el las opciones de palabras con el boton izquierdo
void buttonLeft()
{
  short buttonNow = digitalRead(BUTTONLEFT);

  if (buttonNow == HIGH && buttonLeftBefore == LOW)
  {
    if(flagSelect == 0)
    {
      if(r==0)
      {
        r=2;
      }
      else{
        r--;
      }

    }
    else{

      if(contador==0){
         contador=25;
      }
      else{
        contador--;
      }
    }
  }
  buttonLeftBefore = buttonNow;
}

//	@brief Permite seleccionar una opcion
void buttonOk()
{
  short buttonNow = digitalRead(BUTTONOK);

  if (buttonNow == HIGH && buttonOkBefore == LOW)
  {
    if(flagSelect == 0)
      {
        flagSelect = 1;
    	}
    else{
      buttonOn = 1;
      flag=0;
    }
  }
  buttonOkBefore = buttonNow;
}

//	@brief Resetea el juego para poder iniciar una vez ganes o pierdas
//	@param int millis contador de millis para reiniciar
void reset(int millis){
  if (millis %3000==0)
  {
    cleanScreen();
    flag=0;
    flagWin=0;
    flagLose=0;
    flagSelect=0;
    contador=0;
    contLife=3;
    contadorDos=1;
    r=1;
    strcpy(lifes,"***");
    strcpy(palabraDos,"_________");
    flagReset=0;
  }
}

//	@brief mensaje y llama a reset una vez ganado el juego
//	@param int millis contador de millis para reiniciar
void win(int millis){

 lcd.setCursor(0,0);
 lcd.print("================");
 lcd.setCursor(3,1);
 lcd.print("ganaste owo");
 reset(millis);
}

//	@brief mensaje y llama a reset una vez perdido el juego
//	@param	int millis contador de millis para reiniciar
void lose(int millis){

 lcd.setCursor(2,0);
 lcd.print("perdiste unu");
 lcd.setCursor(2,1);
 lcd.print(palabra);
 reset(millis);
}

//	@brief permite elegir la palabra, setea los botones y las vidas, palabra y abc
void setAll(){
 if(flagSelect == 0){
    buttons();
    lcd.setCursor(2,0);
    lcd.print ("Elegi: ");
   	lcd.print(r);
 }
  else if(flagSelect==1){
      lcd.setCursor(0,0);
    lcd.print("                ");
    flagSelect=2;
  }
  else{

   setLifes();
   setWord();
   setAbc();
  }
}

//	@brief setea los botones
void buttons(){
 buttonRight();
 buttonLeft();
 buttonOk();
}

//	@brief descuenta vidas segun cada error
void discountLifes(){
   switch(contLife){
   case 3:
     strcpy(lifes,"** ");
     break;
   case 2:
     strcpy(lifes,"*  ");
     break;
   case 1:
     flagLose = 1;
     cleanScreen();
     break;
  }
}

//	@brief busca coincidencia de la palabra
void reader(){
 if(abc[contador]==palabra[contadorDos]){
    palabraDos[contadorDos]=palabra[contadorDos];
    contadorDos++;
    flag=1;
 }else if(contadorDos==9){
    buttonOn=0;
    contadorDos=1;
    if(flag==0){
      discountLifes();
      contLife--;
        }
      }
      else{
      contadorDos++;
      }
}

//	@brief secuencia de millis para reiniciar el juego
void sequence(){
 unsigned long millisA=millis();
 if (millisA - millisB >= 3000 &&(flagWin==1||flagLose==1))
 {
   if(flagReset==1)
   {
     millisA=0;
     flagReset=0;
   }
   millisC=millisA-millisB;
   millisB = millisA;
  }
}

void loop()
{
  sequence();
  if(flagWin==0 && flagLose == 0){
	setAll();
    buttons();

    if(buttonOn==1){
     reader();
    }
  }
  else{
    if(flagWin == 1)
    {
       flagReset=1;
       win(millisC);

    }
    else{
       if(flagLose == 1)
       {
          flagReset=1;
          lose(millisC);
       }
    }
  }
  if(strcmp(palabra,palabraDos)==0){
      	flagWin=1;
  }

  delay(10);
}
