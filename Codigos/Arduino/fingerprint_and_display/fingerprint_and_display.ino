/******************************************************************************************************
 Projeto: Cofre-IoT
 GitHub:  https://github.com/abrantesasf/cofre-IoT/
 Sobre:   O projeto Cofre-IoT é um projeto acadêmico dos alunos
          de Ciência da Computação e de Sistemas de Informação
          da FAESA Centro Universitário, disciplina de Introdução
          à Computação (2018/1). O objetivo é desenvolver e apresentar um protótipo
          de cofre conectado à internet, como uma prova de conceito da
          Internet das Coisas (Internet of Things - IoT).
*******************************************************************************************************

        Código: Este código faz a leitura de uma impressão digital e exibe
                em um display a informação de "aberto" ou "fechado", de acordo
                com a presença ou ausência da impressão digital: se a impressão
                digital estiver no banco de dados do próprio sensor, o cofre é
                aberto; caso contrário, nada ocorre. Além disso, quando a digital é
                reconhecida, um sinal é enviado para uma trava solenóide e o cofre é aberto.
                Juntamente a isso, um módulo NodeMCU é alimentado, e passa a exercer as funções
                de consulta de saldo, soma e diminuição dos valores do banco de dados.

Pré-requisitos: 1) Arduino
                2) Leitor de impressões digitais (ver seção Hardware)
                3) Display TFT LCD (ver seção Hardware)
                4) Bibliotecas adicionais para o Arduino:
                   - https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
                   - https://github.com/adafruit/Adafruit-GFX-Library
                   - https://github.com/sumotoy/TFT_ILI9163C

 Inputs gerais: A impressão digital lida pelo sensor.

Outputs gerais: 1) Uma mensagem de bem-vindo no display, indicando que
                   o cofre está aberto.
                2) Um sinal elétrico para abrir a tranca do cofre.
                3) Um sinal elétrico para ativar o NodeMCU.

       Retorno: -

    Limitações: 1) O cadastro das impressões digitais deve ser feito por outro
                   sistema, não integrado ao Cofre-IoT.
                2) O código para a abertura da trava do cofre ainda precisa ser criado.

   Referências: O código abaixo foi totalmente obtido e modificado
                do tutorial "How to use a fingerprint sensor with Arduino",
                preparado por Nick Koumaris e disponibilizado em seu site
                "educ8s" (http://educ8s.tv). O tutorial completo, incluindo
                o código e exemplos, pode ser acessado em:
                http://educ8s.tv/arduino-fingerprint-sensor-module-tutorial/

                Os autores do projeto Cofre-IoT agradecem imensamente a Nick Koumaris
                pela disponibilização de tal material. Sem esse tutorial nosso
                trabalho teria sido imensamente maior. Thank you, Nick!
             
      Hardware: 1) Fingerprint Sensor (DY50 / ZFM-20)
                   - Modelo referência: https://www.adafruit.com/product/751
                   - Modelo utilizado:  https://www.usinainfo.com.br/outros-modulos-arduino/leitor-biometrico-para-arduino-dy50-2816.html
                   - Instruções:        https://learn.adafruit.com/adafruit-optical-fingerprint-sensor/

                2) Display TFT LCD 1.8" 128x160 pixels 
                   (Thin-Film Transistor Liquid Crystal Display)
                   - Modelo referência: https://www.amazon.com/Yoochin-serial-128x160-Display-Arduino/dp/B075YV9XWN
                                        https://www.adafruit.com/product/358
                   - Modelo utilizado:  https://www.filipeflop.com/produto/display-lcd-tft-1-8-128x160/
                   - Instruções:        https://learn.adafruit.com/1-8-tft-display/
                                        https://www.hotmcu.com/18-128x160-tft-lcd-with-spi-interface-p-314.html
******************************************************************************************************/


// Inclui bibliotecas necessárias
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

// Define algumas variáveis para apontar para pinos específicos
#define __CS 10
#define __DC 9

// Para ajustar a configuração do tamanho do display, ajuste as seguintes definições
// DIRETAMENTE no arquivo /opt/arduino/libraries/Adafruit_TFT_ILI9163C/_settings/TFT_ILI9163C_settings.h
//#define _TFTWIDTH     128//the REAL W resolution of the TFT
//#define _TFTHEIGHT    160//the REAL H resolution of the TFT
//#define _GRAMWIDTH    128
//#define _GRAMHEIGH    160//160
//#define __OFFSET        0//*see note 2

// Imagem do ícone da impressão digital
static const uint8_t icon [] PROGMEM = {
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x3f,0xc0,0x0,0x0,0x0,
0x0,0x0,0x3,0xff,0xf8,0x0,0x0,0x0,
0x0,0x0,0x7,0xff,0xfe,0x0,0x0,0x0,
0x0,0x0,0x1f,0xc0,0x7f,0x80,0x0,0x0,
0x0,0x0,0x3e,0x0,0x7,0xc0,0x0,0x0,
0x0,0x0,0x7c,0x0,0x3,0xe0,0x0,0x0,
0x0,0x0,0xfd,0xff,0x81,0xf0,0x0,0x0,
0x0,0x0,0xff,0xff,0xe0,0xf0,0x0,0x0,
0x0,0x1,0xff,0xff,0xf8,0x78,0x0,0x0,
0x0,0x1,0xff,0x80,0x7c,0x38,0x0,0x0,
0x0,0x3,0xfc,0x0,0xe,0x3c,0x0,0x0,
0x0,0x3,0xf0,0x0,0x7,0x1c,0x0,0x0,
0x0,0x7,0xc0,0x7f,0x83,0x8e,0x0,0x0,
0x0,0x7,0x83,0xff,0xe0,0xe,0x0,0x0,
0x0,0x7,0xf,0xff,0xf8,0xf,0x0,0x0,
0x0,0x6,0x1f,0x80,0xfc,0x7,0x0,0x0,
0x0,0x4,0x7e,0x0,0x3f,0x7,0x0,0x0,
0x0,0x0,0xf8,0x0,0xf,0x7,0x0,0x0,
0x0,0x0,0xf0,0x3e,0x7,0x87,0x0,0x0,
0x0,0x1,0xe1,0xff,0x83,0x83,0x80,0x0,
0x0,0x3,0xc3,0xff,0xc3,0xc3,0x80,0x0,
0x0,0x3,0xc7,0xc3,0xe1,0xc3,0x80,0x0,
0x0,0x3,0x8f,0x0,0xf1,0xe3,0x80,0x0,
0x0,0x7,0x1e,0x0,0x78,0xe3,0x80,0x0,
0x0,0x7,0x1e,0x3c,0x38,0xe3,0x80,0x0,
0x0,0x7,0x1c,0x7e,0x38,0xe3,0x80,0x0,
0x0,0xf,0x1c,0x7f,0x38,0xe3,0x80,0x0,
0x0,0xe,0x3c,0xf7,0x38,0x71,0x80,0x0,
0x0,0xe,0x38,0xe7,0x38,0x71,0xc0,0x0,
0x0,0xe,0x38,0xe7,0x38,0x71,0xc0,0x0,
0x0,0xe,0x38,0xe7,0x38,0x73,0xc0,0x0,
0x0,0xe,0x38,0xe3,0x98,0xe3,0xc0,0x0,
0x0,0xe,0x38,0xe3,0xb8,0xe3,0x80,0x0,
0x0,0x0,0x38,0xe3,0xf8,0xe3,0x80,0x0,
0x0,0x0,0x38,0xe3,0xf8,0xe3,0x80,0x0,
0x0,0x0,0x3c,0xf1,0xf1,0xe3,0x80,0x0,
0x0,0x6,0x1c,0x70,0x1,0xc7,0x80,0x0,
0x0,0xe,0x1c,0x78,0x3,0xc7,0x80,0x0,
0x0,0xf,0x1c,0x3e,0x7,0x87,0x0,0x0,
0x0,0xf,0x1e,0x3f,0xff,0x8f,0x0,0x0,
0x0,0xf,0x1e,0x1f,0xff,0x1f,0x0,0x0,
0x0,0xf,0xf,0x7,0xfc,0x3e,0x0,0x0,
0x0,0x7,0x87,0x80,0x0,0x7c,0x0,0x0,
0x0,0x7,0x87,0xe0,0x0,0xfc,0x0,0x0,
0x0,0x3,0xc3,0xf8,0x7,0xf8,0x0,0x0,
0x0,0x3,0xe1,0xff,0xff,0xe1,0x0,0x0,
0x0,0x1,0xe0,0x7f,0xff,0x83,0x0,0x0,
0x0,0x1,0xf8,0xf,0xfe,0x7,0x0,0x0,
0x0,0x0,0xfc,0x0,0x0,0xe,0x0,0x0,
0x0,0x0,0x3f,0x0,0x0,0x3c,0x0,0x0,
0x0,0x0,0x1f,0xe0,0x1,0xf8,0x0,0x0,
0x0,0x0,0x7,0xff,0xff,0xf0,0x0,0x0,
0x0,0x0,0x1,0xff,0xff,0xc0,0x0,0x0,
0x0,0x0,0x0,0x1f,0xfc,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};

// Instancia display TFT, com a pinagem especificada
TFT_ILI9163C display = TFT_ILI9163C(__CS, 8, __DC);
SoftwareSerial mySerial(2, 3);

// Instancia leitor de digitais
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;

// Função setup
void setup(void) {
  // Inicia leitor de digitais
  startFingerprintSensor();

  // Inicia display
  display.begin();
  //display.setRotation(2); 

  // Exibe a tela de fechado
  displayLockScreen();

  // Configura pinos para reles
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(4, HIGH);
}

// Variavel para controlar o status do cofre
boolean cofreAberto = 0;  // 0 = fechado; 1 = aberto

// Função loop:
void loop() {

  // Lê impressão digital
  fingerprintID = getFingerprintID();
  delay(50);
  
  if(fingerprintID == 1)
  {
    if (cofreAberto == 0) {
      abreCofre("Brendhom");
    } else {
      fechaCofre();
    }
  }

   if(fingerprintID == 2)
  {
    if (cofreAberto == 0) {
      abreCofre("Carlos");
    } else {
      fechaCofre();
    }
  }

   if(fingerprintID == 3)
  {
    if (cofreAberto == 0) {
      abreCofre("Abrantes");
    } else {
      fechaCofre();
    }
  }

}

void abreCofre(String nome) {
  display.drawBitmap(30,35,icon,60,60,GREEN);
  cofreAberto = 1;
  delay(1000);
  displayUnlockedScreen();
  displayNome(nome);
  delay(1000);
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
  delay(500);
  digitalWrite(4, LOW);
}

void fechaCofre() {
  cofreAberto = 0;
  digitalWrite(4, HIGH);
  display.fillScreen(BLACK);
  displayLockScreen();
}

void displayUnlockedScreen()
{
   display.fillScreen(BLACK);
   display.drawRect(0,0,128,160,WHITE);
   
   display.setCursor(23,10);
   display.setTextColor(RED); 
   display.setTextSize(2);
   display.print("ABERTO");

   display.setCursor(14,50);
   display.setTextColor(WHITE); 
   display.setTextSize(2);
   display.print("BEM VINDO");
}

void displayNome(String nome)
{
  display.setCursor(16,75);
  display.setTextColor(WHITE); 
  display.setTextSize(2);
  display.print(nome);

  display.setCursor(5, 120);
  display.setTextSize(1);
  display.print("Informe novamente a");
  display.setCursor(5, 130);
  display.print("digital para fechar.");
}

void displayLockScreen()
{
  display.drawRect(0,0,128,160,WHITE);
  display.setCursor(20,10);
  display.setTextColor(RED); 
  display.setTextSize(2);
  display.print("FECHADO");

  display.setCursor(10,100);
  display.setTextColor(WHITE); 
  display.setTextSize(1.8);
  display.print("Esperando uma");
  display.setCursor(10,110);
  display.print("digital valida...");

  display.drawBitmap(30,35,icon,60,60,WHITE);
}

void startFingerprintSensor()
{
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
