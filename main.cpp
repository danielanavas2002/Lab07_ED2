//****************************************************************
// Librerías
//****************************************************************
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//****************************************************************
// Definición de etiquetas
//****************************************************************
//Definicion Pin Analogico
#define pinPot1 36 //Potenciometro 1
// Definicion de pines LCD
LiquidCrystal_I2C lcd(0x27,20,4);
//****************************************************************
// Variables Globales
//****************************************************************
//Lectura Analogica
int pot1Analogico;
// Valores RGB
int red;
int redA;
int green;
int greenA;
int blue;
int blueA;
char c;
//Funcionamiento Comunicacion Serial
char charRed[4]; //Representación en Caracteres 
String strRed; //Convertir en Cadena
char charInput; //Caracteres Recibida
String strInput; //Convertir en Cadena
bool stringComplete = false; //Bandera al terminar Recepcion
//*****************************************************************************
// Prototipos de Funciones
//*****************************************************************************
void serialEvent(void);
//****************************************************************
// Configuración
//****************************************************************  
void setup(){
  //Definicion de Entradas
  pinMode(pinPot1, INPUT); //Potenciometro 1
  //LCD
  lcd.init();                      // initialize the lcd 
  lcd.backlight(); 
  lcd.setCursor(0, 0); 
  lcd.print("ROJO: VER: AZUL:");
  Serial.begin(115200);
}
//****************************************************************
// Loop Principal
//****************************************************************
void loop(){
  //Lectura analógica de Potenciometro
  pot1Analogico = analogRead(pinPot1);
  red = map(pot1Analogico, 0, 4095, 0, 255);
  if(red != redA){
    strRed = red; //Convertir int a string
    strRed.toCharArray(charRed,4); //Convertir string a char
    Serial.write(charRed);
    Serial.print('\n');
    lcd.setCursor(0, 1); //Actualizar valor en LCD
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(red);
    redA = red;
  }
  if(stringComplete){ //Al terminar de recibir los datos
    c = Serial.read(); //Identificar a que color corresponde
        if(c=='b'){ //Si inicia con una b es correspondiente a Blue
          blue = Serial.parseInt(); //Guardar en Variable blue el valor entero
        }
        if(c=='g'){ ///Si inicia con una g es correspondiente a Green
          green = Serial.parseInt(); //Guardar en Variable gren el valor entero
        }
  }
  if(blue != blueA){ //Si cambia el valor de blue
    lcd.setCursor(11, 1); //Actulizar en LCD
    lcd.print("   ");
    lcd.setCursor(11, 1);
    lcd.print(blue); 
    blueA = blue;
  }
  if(green != greenA){ //Si cambia el valor de green
    lcd.setCursor(6, 1); //Actulizar en LCD
    lcd.print("   ");
    lcd.setCursor(6, 1);
    lcd.print(green); 
    greenA = green;
  }
}
//****************************************************************
// Lectura Serial
//****************************************************************
void serialEvent(void){
  while(Serial.available()){
    charInput = (char)Serial.read();
    strInput += charInput;
    if(charInput == '\n'){
      stringComplete = true;
    }
  }
}