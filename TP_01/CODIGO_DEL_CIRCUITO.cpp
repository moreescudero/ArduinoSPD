
/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *
 *	El numero 15 (0000001111) se representaria encendiendo los
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos
 *	los led anteriores y encenderse uno de los leds VERDES.
 *	Notese, en el ejemplo los 0 representan los led apagados
 *	y los 1 los led encendidos).
 *
 *	-------------------------------------------------------
 *
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga.
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los
 *	leds encendidos sea perceptible para el ojo humano y
 *	documentar cada función creada en el código.
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
 */

//##### Una Ayudita #####
//--- Defines ---//
#define ZERO_TO_HERO 0 //? To start the counter from 0
#define BUTTON_START 2
#define BUTTON_RESET 3
#define FIRST_LED 4             //? First pin of the leds
#define LAST_LED 13             //? Last pin of the leds
#define BASE_MILLI_SECONDS 1000 //? Secods expresed in miliseconds
#define MAX_SECONDS 1023        //! Max amount of secods to show
#define TAM 10
//--- End Defines ---//

// Declaracion de Variables
short led = FIRST_LED;
unsigned long millisB = 0;
short ultimoLed = LAST_LED;
short boton_On = 0;
short boton_Antes = 0;
short boton_Antes_Reset = 0;
short estado = HIGH;
short estado_Reset = HIGH;
short contador = ZERO_TO_HERO;
int arrayEnteros[TAM];
int cont = 0;

/**
 * @brief Set the pins object
 *
 */
void config_Pins()
{
  for (int i = FIRST_LED; i <= LAST_LED; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_RESET, INPUT);
}

void setup()
{
  Serial.begin(9600);
  config_Pins();
}

/**
 * @brief Calls function to convert int numbers to binary.
 * @note We use this function to avoid use WHILE & FOR.
 *
 * @param array
 * @param ultimoNum
 */
void decimal_Binario(int array[], int ultimoNum)
{
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
  ultimoNum = calculo_Binario(arrayEnteros, ultimoNum);
}

/**
 * @brief This print message in the console.
 * @note Seconds numbers & their Binary.
 *
 */
void imprimirCadena()
{
  Serial.print("Segundos: ");
  Serial.print(contador);
  Serial.print(" | ");
  Serial.print("Binario: [");
  for (int i = 0; i < TAM; i++)
  {
    Serial.print(arrayEnteros[i]);
  }
  Serial.println("]");
}

/**
 * @brief Function to turn on the leds.
 *
 */
void secuencia()
{
  if (contador > 0)
  {
    for (int i = 0; i < TAM; i++)
    {
      digitalWrite(ultimoLed, arrayEnteros[i]);
      ultimoLed--;
      if (led > ultimoLed)
      {
        ultimoLed = LAST_LED;
      }
    }
  }
  imprimirCadena();
}

/**
 * @brief
 *
 */
void boton_Start()
{
  short button_now = digitalRead(BUTTON_START);

  // Antirrebote
  if (button_now == HIGH && boton_Antes == LOW)
  {
    if (boton_On == 1)
    {
      boton_On = 0;
    }
    else
    {
      boton_On = 1;
    }
    estado = !estado;
  }
  boton_Antes = button_now;
}

/**
 * @brief
 *
 */
void boton_Reset()
{
  short button_now = digitalRead(BUTTON_RESET);

  // Antirrebote
  if (button_now == HIGH && boton_Antes_Reset == LOW)
  {
    contador = ZERO_TO_HERO;
    estado_Reset = !estado_Reset;
  }
  boton_Antes_Reset = button_now;
}

/**
 * @brief
 *
 * @param array
 * @param ultimoNum
 * @return int
 */
int calculo_Binario(int array[], int ultimoNum)
{
  short numBin = 0;
  numBin = cont % 2;
  cont /= 2;
  array[ultimoNum] = numBin;
  ultimoNum--;
  return ultimoNum;
}

/**
 * @brief
 *
 */
void apagar_Led()
{
  for (int x = FIRST_LED; x <= LAST_LED; x++)
  {
    digitalWrite(x, LOW);
  }
}

void loop()
{
  cont = contador;
  int ultimoNum = TAM - 1;
  unsigned long millisA = millis();
  boton_Start();
  boton_Reset();
  if (contador == 0)
  {
    apagar_Led();
  }
  if (millisA - millisB >= BASE_MILLI_SECONDS)
  {
    if (contador <= MAX_SECONDS && boton_On == 1)
    {
      decimal_Binario(arrayEnteros, ultimoNum);
      secuencia();
      contador++;
    }
    millisB = millisA;
  }
  // Delay para la simulación
  delay(10);
}
