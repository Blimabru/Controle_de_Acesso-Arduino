// Inclusão das Bibliotecas
#include <Keypad.h> // Permite a leitura do teclado 4x4.
#include <Servo.h> // Controla o servo motor.
#include <Adafruit_LiquidCrystal.h> // Controla o display LCD.

// Configurações do LCD
Adafruit_LiquidCrystal lcd(0);  // Cria um objeto para o LCD, o 0 sendo a porta de comunicação I2C do Tinkercad

// Configurações do teclado 4x4
const byte LINHAS = 4; // Define o número de linhas no teclado 4x4 (total de 4 linhas)
const byte COLUNAS = 4; // Define o número de colunas no teclado 4x4 (total de 4 colunas)

// Mapeamento das teclas do teclado 4x4
// Cada elemento do array 'teclas' corresponde ao valor de uma tecla no teclado
char teclas[LINHAS][COLUNAS] = {
  {'1','2','3','A'}, // Primeira linha do teclado
  {'4','5','6','B'}, // Segunda linha do teclado
  {'7','8','9','C'}, // Terceira linha do teclado
  {'*','0','#','D'}  // Quarta linha do teclado
};

// Pinos do Arduino conectados às linhas do teclado
// Cada valor no array 'pinosLinhas' corresponde ao pino do Arduino que controla uma linha do teclado
byte pinosLinhas[LINHAS] = {2, 3, 4, 5}; // Linhas do teclado conectadas aos pinos 2, 3, 4 e 5 do Arduino

// Pinos do Arduino conectados às colunas do teclado
// Cada valor no array 'pinosColunas' corresponde ao pino do Arduino que controla uma coluna do teclado
byte pinosColunas[COLUNAS] = {6, 7, 8, 9}; // Colunas do teclado conectadas aos pinos 6, 7, 8 e 9 do Arduino

// Cria um objeto 'Keypad' que usa o mapeamento das teclas e os pinos definidos
// A função 'makeKeymap(teclas)' converte o array 'teclas' em um formato que a biblioteca Keypad pode usar
Keypad keypad = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS); // Inicializa o teclado e permite a leitura das teclas pressionadas

// Configuração do Servo Motor
Servo meuServo; // Cria um objeto servo
const int servoPin = 10; // Define o pino do Arduino ao qual o servo motor está conectado

// Definição dos pinos conectados ao LED RGB
const int vermelhoPin = 11;
const int verdePin = 12;
const int azulPin = 13;

// Definição do código de acesso
String senha = "1234"; // Código de acesso correto
String codigoEntrada = ""; // Código inserido pelo usuário

void setup() {
  meuServo.attach(servoPin); // Conecta o servo ao pino definido
  meuServo.write(0); // Define a posição inicial do servo (tranca fechada)

  // Define os pinos do LED RGB como saída
  pinMode(vermelhoPin, OUTPUT);
  pinMode(verdePin, OUTPUT);
  pinMode(azulPin, OUTPUT);

  // Configura o LED RGB para a cor azul (modo de espera)
  ledRGB(0, 0, 255);

  lcd.begin(16, 2); // Inicializa o LCD com 16 colunas e 2 linhas
  lcd.print("Insira o Codigo:"); // Exibe mensagem no LCD
}

// Loop principal
void loop() {
  char tecla = keypad.getKey(); // Lê a tecla pressionada no teclado e salva na variável "tecla"

  if (tecla) { // Se a variável "tecla" contiver um valor (ou seja, uma tecla foi pressionada)
    lcd.setCursor(codigoEntrada.length(), 1); // Posiciona o cursor na linha 1 e coluna correspondente ao comprimento do código digitado
    lcd.print(tecla); // Exibe a tecla pressionada no LCD
    codigoEntrada += tecla; // Concatena a tecla pressionada ao código inserido

    if (codigoEntrada.length() == senha.length()) { // Verifica se o código inserido tem o mesmo comprimento que o código correto
      delay(500); // Pequena pausa para estabilização

      if (codigoEntrada == senha) { // Se o código inserido corresponder à senha correta
        lcd.clear(); // Limpa a tela do LCD
        lcd.setCursor(0, 0); // Define o cursor para a primeira posição da primeira linha
        lcd.print("   Bem Vindo!   "); // Exibe mensagem de boas-vindas
        lcd.setCursor(0, 1); // Define o cursor para a primeira posição da segunda linha
        lcd.print("Acesso Liberado "); // Exibe mensagem de acesso liberado
        meuServo.write(90); // Comanda o servo motor para abrir a tranca

        // Configura o LED RGB para a cor verde
        ledRGB(0, 255, 0);

        delay(5000); // Mantém o acesso liberado por 5 segundos
        meuServo.write(0); // Fecha a tranca
      } else { // Se o código inserido estiver incorreto
        lcd.clear(); // Limpa a tela do LCD
        lcd.setCursor(0, 0); // Define o cursor para a primeira posição da primeira linha
        lcd.print("     Acesso     "); // Exibe mensagem de acesso negado
        lcd.setCursor(0, 1); // Define o cursor para a primeira posição da segunda linha
        lcd.print("     Negado!    "); // Exibe mensagem de acesso negado

        // Configura o LED RGB para a cor vermelha
        ledRGB(255, 0, 0);

        delay(2000); // Exibe a mensagem de acesso negado por 2 segundos
      }

      lcd.clear(); // Limpa a tela do LCD
      lcd.setCursor(0, 0); // Define o cursor para a primeira posição da primeira linha
      lcd.print("Insira a senha: "); // Solicita a entrada do código novamente

      // Reseta o código inserido e volta para o modo de espera
      codigoEntrada = ""; // Limpa o código inserido
      lcd.setCursor(0, 1); // Define o cursor para a primeira posição da segunda linha
      lcd.print("                "); // Limpa a linha

      // Configura o LED RGB para a cor azul (modo de espera)
      ledRGB(0, 0, 255);
    }
  }
}

// Função para alterar a cor do LED RGB
void ledRGB(int vermelhoValue, int verdeValue, int azulValue) {
  // Ajusta a intensidade do LED enviando um valor PWM (Modulação de Largura de Pulso) para os pinos correspondentes
  analogWrite(vermelhoPin, vermelhoValue); 
  analogWrite(verdePin, verdeValue);
  analogWrite(azulPin, azulValue);
}