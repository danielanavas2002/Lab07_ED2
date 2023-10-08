//****************************************************************
// Definición de etiquetas
//****************************************************************
//Definicion Pin Analogico
#define pinPot2 PB_4 //Potenciometro 2
//Definicion Pines de Botones
#define pinBotonI PF_0 //Boton Incremento
#define pinBotonD PF_4 //Boton Decremento
//Definicion Pines LED
#define pinLedR PF_1 //Led Rojo
#define pinLedG PF_3 //Led Verde
#define pinLedB PF_2 //Led Azul
//****************************************************************
// Variables Globales
//****************************************************************
//Lectura Analogica
int pot2Analogico;
int pot2AnalogicoA;
//Valores RGB
int red;
int redA;
int green;
int greenA;
int blue;
int blueA;
//Convertir a Cadena
String greenStr;
String blueStr;
//Representacion en Caracteres
char charGreen[5];
char charBlue[5];
//Funcionamiento Boton Incremento
int estadoBotonI;
int estadoAnteriorBotonI = 1;
bool banderaLecturaI = false;
//Funcionamiento Boton Decremento
int estadoBotonD;
int estadoAnteriorBotonD = 1;
bool banderaLecturaD = false;
//Filtro EMA
int adcRaw = 0;
double adcFiltradoEMA = adcRaw; // S(0) = Y(0)
double alpha = 0.8;//0.05; // Factor de suavizado (0-1)
//*****************************************************************************
// Prototipos de Funciones
//*****************************************************************************
void filtroEMA(void);
//*****************************************************************************
// Configuración
//*****************************************************************************
void setup(){
  //Definicion de Entradas
  pinMode(pinPot2, INPUT); //Potenciometro 2
  pinMode(pinBotonI, INPUT_PULLUP); //Entrada Boton Incremento
  pinMode(pinBotonD, INPUT_PULLUP); //Entrada Boton Decremento
  //Definicion de Salidas
  pinMode(pinLedR, OUTPUT); //Led Rojo
  pinMode(pinLedG, OUTPUT); //Led Verde
  pinMode(pinLedB, OUTPUT); //Led Azul
  //Monitor Serial 
  Serial.begin(115200); //Con Computadora
  Serial1.begin(115200); //Con ESP32
}
void loop(){
  //Lectura analógica de Potenciometro
  filtroEMA(); //Filtrar Señal Analogica
  if(blue != blueA){
    String blueStr = "b" + String(blue); //Arreglar a String
    blueStr.toCharArray(charBlue,5); //Convertir String a Char
    Serial1.write(charBlue); //Escrbir en Monitor Serial 1
    Serial1.print('\n');
    Serial.println("Azul Actualizado"); //Mostrar en computadora con UART en Monitor Serial 0
    Serial.print("R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.println(blue);
    Serial.println(" ");
    analogWrite(pinLedB, blue); //Actualizar LED Azul
    blueA = blue;
  }
  //Lectura de los Botones
  estadoBotonI = digitalRead(pinBotonI); //Lectura de Boton Incremento
  estadoBotonD = digitalRead(pinBotonD); //Lectura de Boton Decremento
 
  //Antirebote de Boton Incremento
  if (estadoBotonI == 0 && estadoAnteriorBotonI == 1) {
    banderaLecturaI = true; //Activar Bandera
  }
  estadoAnteriorBotonI = estadoBotonI;
  //Antirebote de Boton Decremento
  if (estadoBotonD == 0 && estadoAnteriorBotonD == 1) {
    banderaLecturaD = true; //Activar Bandera
  }
  estadoAnteriorBotonD = estadoBotonD;
  if (estadoBotonD == 1 && estadoAnteriorBotonD == 0) {
    banderaLecturaD = true; //Activar Bandera
  }
  estadoAnteriorBotonD = estadoBotonD;
  //Incremento y Decremento Green
  //Se incrementa solo si el botón de incrementar ha sido presionado
  if (banderaLecturaI) {
    green++;
    if (green > 255) {
      green = 0; // Limitamos el valor máximo a 255, si se supera regresar a 0
    }
    banderaLecturaI = false;
  }
  //Se decrementa solo si el botón de decrementar ha sido presionado
  if (banderaLecturaD) {
    green--;
    if (green < 0) {
      green = 255; // Limitamos el valor minimo a 0, si se supera regresar a 255
    }
    banderaLecturaD = false;
  }
  if(green != greenA){
    String greenStr = "g" + String(green); //Arreglar a String
    greenStr.toCharArray(charGreen,5); //Convertir String a Char
    Serial1.write(charGreen); //Escrbir en Monitor Serial 1
    Serial1.print('\n');
    Serial.println("Verde Actualizado"); //Mostrar en computadora con UART en Monitor Serial 0
    Serial.print("R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.println(blue);
    Serial.println(" ");
    analogWrite(pinLedG, green); //Actualizar LED Verde
    greenA = green;
  }
   // Leer la variable red enviada por el ESP32
  if(Serial1.available()>0)
  {
    String cadena = Serial1.readStringUntil('\n');  // lectura de datos
    red = cadena.toInt(); //Convertir string a int
  } 
  if(red != redA){
    Serial.println("Rojo Actualizado"); //Mostrar en computadora con UART en Monitor Serial 0
    Serial.print("R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.println(blue);
    Serial.println(" ");
    analogWrite(pinLedR, red); //Actualizar LED Rojo
    redA = red;
  }
 }
// ****************************************************************************
// Funcion Filtro EMA
// ****************************************************************************
void filtroEMA(void){ //Filtrado de Señal Analogica
  adcRaw = analogRead(pinPot2);
  pot2Analogico = (alpha * adcRaw) + ((1.0 - alpha) * adcFiltradoEMA);
  blue = map(pot2Analogico, 0, 3275, 0, 255);
}
