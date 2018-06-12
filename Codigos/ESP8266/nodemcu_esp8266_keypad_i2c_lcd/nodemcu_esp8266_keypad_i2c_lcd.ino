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
        Código: Este código faz a leitura de um keypad ligado à um módulo NodeMCU para realizar as
                seguintes funções: consulta de saldo em um servidor, soma ou diminuição de valores ao
                saldo armazenado no servidor. À medida em que o usuário digita os números, um display
                LCD 16x2 exibe o número digitado. O display também é utilizado para mostrar informações
                durante o boot do nódulo (conexão à internet e IP).
Pré-requisitos: 1) NodeMCU DevKit
                2) Keypad 16 teclas
                3) Display LCD 16x2
                4) Módulo I2C
                5) Bibliotecas adicionais para o Arduino:
                   - LiquidCrystal_I2C: https://github.com/marcoschwartz/LiquidCrystal_I2C
                   - Keypad: http://playground.arduino.cc/Code/Keypad
                   - ESP8266: https://github.com/esp8266/Arduino
 Inputs gerais: Números no Keypad e opções de ação (soma, diminuição, consulta de saldo).
Outputs gerais: Cada opção de ação apresenta uma mensagem específica de erro ou de conclusão
                no LCD.
       Retorno: -
    Limitações: 1) Falhas na conexão Wireless estão com um tratamento MUITO básico. Esse tratamento
                   de exceção precisa ser muito melhorado ainda.
                2) Se você digitar um número maior do que o saldo e utilizar a função de diminuir, seu
                   saldo ficará negativo. Esse comportamento precisa ser corrigido.
   Referências: De modo geral este código é uma mistura de DIVERSOS FRAGMENTOS de códigos obtidos em 
                tutoriais ou fóruns na internet, para funções específicas, por exemplo: o código para inicializar
                o keypad e mapear as teclas, o código para inicializar e se conectar no wifi, etc. O algoritmo
                principal, que interliga todos os componentes (NomeMCU DevKit, Keypad, I2C, LCD e Keypad),
                e que fornece a funcionalidade principal, foi desenvolvida pelos integrantes do projeto.
                Assim que tivermos um tempo, organizaremos todas as fontes de referências utilizadas e 
                as listaremos aqui.
             
      Hardware: 1) DevKit NodeMCU v3 ESP8266 12e:
                   - Modelo referência: https://github.com/nodemcu/nodemcu-devkit-v1.0
                   - Modelo utilizado:  https://www.usinainfo.com.br/nodemcu/nodemcu-v3-esp8266-esp-12e-iot-com-wifi-4420.html
                   - Instruções:        (ver repositório GitHub do projeto)
                2) Display LCD 16x2:
                   - Modelo referência: https://www.adafruit.com/product/181
                   - Modelo utilizado:  https://www.filipeflop.com/produto/display-lcd-16x2-backlight-azul/
                   - Instruções:        (diversos tutoriais na internet)
                3) Keypad matricial de membrana, 16 teclas:
                   - Modelo referência: https://www.filipeflop.com/produto/teclado-matricial-de-membrana-16-teclas/
                   - Modelo utilizado:  https://www.filipeflop.com/produto/teclado-matricial-de-membrana-16-teclas/
                   - Instruções:        (diversos tutoriais na internet)
                4) Módulo I2C para LCD:
                   - Modelo referência: https://www.filipeflop.com/produto/modulo-serial-i2c-para-display-lcd-arduino/
                   - Modelo utilizado:  https://www.filipeflop.com/produto/modulo-serial-i2c-para-display-lcd-arduino/
                   - Instruções:        https://www.losant.com/blog/how-to-connect-lcd-esp8266-nodemcu
******************************************************************************************************/


/*
 * CARREGA BIBLIOTECAS NECESSÁRIAS (mantenha a ordem especificada abaixo!):
 */
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>


/*
 * CONFIGURAÇÕES PARA O KEYPAD:
 */
// Declara número de linhas e colunas do teclado:
const byte nLinhas = 4;
const byte nColunas = 4;

// Declara matriz para mapear as teclas do teclado (comente ou descomente
// a versão de 16 ou 12 teclas, conforme sua necessidade).
char teclas[nLinhas][nColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
  //{'1', '2', '3'},
  //{'4', '5', '6'},
  //{'7', '8', '9'},
  //{'*', '0', '#'}
};

// Mapeamento entre as linhas e colunas do teclado e os pinos do NodeMCU
// (o pino D8 não é usado pois ele tem um resistor pull-down integrado, o que
// interfere no uso do keypad; o pino D10 somente é usado no keypad com 16 teclas):
byte pinosLinhas[nLinhas]   = {D0, D3, D4, D5};
byte pinosColunas[nColunas] = {D6, D7, D9, D10};

// Cria instância uma instância da classe Keypad, passando o mapeamento
// das teclas
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, nLinhas, nColunas);


/*
 * CONFIGURAÇÕES DO LCD E I2C:
 */
// Constrói objeto LCD, passando endereço I2C e configurações do display:
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Função bemVindo:
void bemVindo() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Bem vindo ao");
  lcd.setCursor(3, 1);
  lcd.print("Cofre-IoT!");
}


/*
 * CONFIGURAÇÕES INICIAIS PARA ACESSO À INTERNET:
 */
// Definições de acesso à rede wifi:
const char* ssid     = "PT_SO_TEM_CORRUPTO";
const char* password = "bolsonaro2018";

// IP do servidor do Cofre-IoT:
String servidor = "52.202.172.134";

// Variáveis para controle:
int wifiStatus;
int executa = 1;


/*
 * VOID SETUP:
 */
void setup() {
  // Altera o modo do D9 para input, ao invés de RX:
  pinMode(D9, INPUT);
  pinMode(D10, INPUT);

  // Ativa LCD:
  lcd.init();
  lcd.backlight();

  // Conecta à internet:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando se");
  lcd.setCursor(0, 1);
  lcd.print("na rede Wi-Fi...");
  delay(2000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado! IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);

  // Dá as boas vindas:
  bemVindo();
}


/*
 * VARIÁVEIS E FUNÇÕES PRINCIPAIS DO PROGRAMA:
 */
// Variável para armazenar o número digitado pelo usuário:
//int numero = 0;
String numero = "";

// Função para codificar string em caracteres HTML:
String urlencode(String str) {
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;   
}

// Função para consultar o saldo
void consultaSaldo() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Consulta saldo:");
  WiFiClient client;
  if (!client.connect(servidor, 80)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FALHOU CONEXAO!");
    lcd.setCursor(0,1);
    lcd.print("ABORTANDO!");
    numero = "";
    bemVindo();
  }
  String url1 = "/select.php";
  url1 += "?id=1";
  url1 += "&numero=";
  url1 += '1';
  String url2;
  url2 += " HTTP/1.1\r\n";
  url2 += "Host: ";
  url2 += servidor;
  url2 += "\r\n";
  url2 += "Connection: close\r\n\r\n";
  String url3;
  url3 = url1 + url2;
  client.print(String("GET ") + url3);
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
  String line = "";
  while (client.available()) {
    line = client.readStringUntil('\r');
  }
  line = line.substring(1); // remove caracteres espúrio do retorno da query
  lcd.setCursor(0,1);
  lcd.print("OK!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saldo atual:");
  lcd.setCursor(16 - line.length(),1);
  lcd.print(line);
  delay(3000);
  bemVindo();
}


/*
 * VOID LOOP:
 */
void loop() {
  char tecla = teclado.getKey();
  if (tecla == '0' && numero == "") {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Entrando valor:");
    //numero = (numero * 10) + tecla -'0';
    numero = numero + tecla;
    lcd.setCursor(0,1);
    lcd.print(numero);
  } else if (tecla == '0' && numero == "0") {
    // Não deixa digitar outros zeros
  } else if (tecla == '*' && numero == "0") {
    if (numero.indexOf('.') == -1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Entrando valor:");
      //numero = (numero * 10) + tecla -'0';
      numero = numero + '.';
      lcd.setCursor(0,1);
      lcd.print(numero);
    }
  } else if (tecla >= '0' && tecla <= '9') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Entrando valor:");
    //numero = (numero * 10) + tecla -'0';
    numero = numero + tecla;
    lcd.setCursor(0,1);
    lcd.print(numero);
  } else if (tecla == '*') {
    if (numero.indexOf('.') == -1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Entrando valor:");
      //numero = (numero * 10) + tecla -'0';
      numero = numero + '.';
      lcd.setCursor(0,1);
      lcd.print(numero);
    }
  } else if (tecla == 'D') {
    bemVindo();
    numero = "";
  } else if (tecla == '#') {
    numero = numero.substring(0, numero.length()-1);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(numero);
  } else if (tecla == 'A') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Somando...");
    delay(1000);
    WiFiClient client;
    if (!client.connect(servidor, 80)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("FALHOU CONEXAO!");
      lcd.setCursor(0,1);
      lcd.print("ABORTANDO!");
      numero = "";
      bemVindo();
    }
    String url1 = "/update.php";
    url1 += "?id=1";
    url1 += "&numero=";
    url1 += numero;
    String url2;
    url2 += " HTTP/1.1\r\n";
    url2 += "Host: ";
    url2 += servidor;
    url2 += "\r\n";
    url2 += "Connection: close\r\n\r\n";
    String url3;
    url3 = url1 + url2;
    client.print(String("GET ") + url3);
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        client.stop();
        return;
      }
    }
    lcd.setCursor(0,1);
    lcd.print("OK!");
    delay(1000);
    numero = "";
    consultaSaldo();
  } else if (tecla == 'B') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Diminuindo...");
    delay(1000);
    WiFiClient client;
    if (!client.connect(servidor, 80)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("FALHOU CONEXAO!");
      lcd.setCursor(0,1);
      lcd.print("ABORTANDO!");
      numero = "";
      bemVindo();
    }
    String url1 = "/update.php";
    url1 += "?id=1";
    url1 += "&numero=";
    url1 += '-' + numero;
    String url2;
    url2 += " HTTP/1.1\r\n";
    url2 += "Host: ";
    url2 += servidor;
    url2 += "\r\n";
    url2 += "Connection: close\r\n\r\n";
    String url3;
    url3 = url1 + url2;
    client.print(String("GET ") + url3);
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        client.stop();
        return;
      }
    }
    lcd.setCursor(0,1);
    lcd.print("OK!");
    delay(1000);
    numero = "";
    consultaSaldo();
  } else if (tecla == 'C') {
    consultaSaldo();
  }
}
