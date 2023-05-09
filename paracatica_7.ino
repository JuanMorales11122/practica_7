#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>		// incluye libreria para interfaz I2C
#include <RTClib.h>		// incluye libreria para el manejo del modulo RTC
#include <LiquidCrystal_I2C.h>

Servo puerta; //variable para poder usar el servomotor

RTC_DS3231 rtc;			// crea objeto del tipo RTC_DS3231

#define direccion_lcd 0x27
#define filas 2
#define columnas 16
LiquidCrystal_I2C lcd(direccion_lcd, columnas, filas);

const int FILAS = 4;
const int COLUMNAS= 4;
char keys[FILAS][COLUMNAS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFila[FILAS]={9,8,7,6};
byte pinesColumna[COLUMNAS]={5,4,3,2};

Keypad teclado=Keypad(makeKeymap(keys), pinesFila, pinesColumna,FILAS,COLUMNAS);

char teclaPrecionada;

char pass[6]="5812B"; //Poner en el arreglo 1 posicion mas a la que tendra la contraseña correcta
char ingresaPass[6]; // tiene que tener el mismo tamaño del arreglo de la contraseña correcta
int indice=0;

void setup()
{
  Serial.begin(9600);		// inicializa comunicacion serie a 9600 bps

  rtc.begin();	//Inicializa el modulo RTC

 rtc.adjust(DateTime(__DATE__, __TIME__));	// funcion que permite establecer fecha y horario

 lcd.init();
 lcd.backlight();

 puerta.attach(10);//activamos el pin del servo
}

void loop()
{
  DateTime tiempo = rtc.now();			// funcion que obtiene la hora de la fecha completa
  Serial.print(tiempo.hour());     /*muestra las horas, minutos y segundos en el monitor serial*/
  Serial.print(":");
  Serial.print(tiempo.minute());
  Serial.print(":");
  Serial.println(tiempo.second()); /**/
  lcd.setCursor(12,1);
  lcd.print(tiempo.hour());        /*muestra las horas y minutos en el LCD*/
  lcd.print(":");				
  lcd.print(tiempo.minute());      /**/
  delay(100);
  
  teclaPrecionada=teclado.getKey(); //detecta que en el teclado se pulso un boton
  if(teclaPrecionada){
    ingresaPass[indice]=teclaPrecionada; // le asigna lo que ingresamos a el arreglo que va a ser comparado
    indice++; //cambiara la posicion del arreglo para poder asignar los caracteres
    lcd.setCursor(0,0);
    lcd.print(teclaPrecionada);
    delay(100);
    
  }

  if(indice==5){ //cuando se llegue al limite del arreglo verificara si esta correcta o no la contraseña
    if(strcmp(pass,ingresaPass)==0){ //Verifica que la contraseña esta correcta
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print(" puerta abierta");
      lcd.setCursor(0,1);
      lcd.print("abierto ");
      puerta.write(180);
      delay(1500);
      lcd.clear();
    }
    else{
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print(" puerta cerrada");
      lcd.setCursor(0,1);
      lcd.print("cerrado ");
      puerta.write(0);
      delay(1500);
      lcd.clear();
    }
    indice=0;
  }
}
