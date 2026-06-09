#define LED_VERMELHO 11  // LED vermelho no pino 11
#define LED_VERDE 10     // LED verde no pino 10

String comando = "";  // String para armazenar comando recebido

void setup() {
  // Configurar comunicação Serial (9600 baud)
  Serial.begin(9600);
  
  // Configurar pinos dos LEDs como saída
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  
  // Iniciar com LEDs desligados
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);
  
  // Mensagem de inicialização
  Serial.println("====================================");
  Serial.println("Sistema de Controle de LEDs");
  Serial.println("====================================");
  Serial.println("Comandos disponíveis:");
  Serial.println("  '9' -> Liga LED VERMELHO, desliga LED VERDE");
  Serial.println("  '8' -> Liga LED VERDE, desliga LED VERMELHO");
  Serial.println("====================================");
}

void loop() {
  // Verificar se há dados disponíveis na porta Serial
  if (Serial.available() > 0) {
    char caractere = Serial.read();
    
    // Se receber "9"
    if (caractere == '9') {
      ligarVermelho();
    }
    // Se receber "8"
    else if (caractere == '8') {
      ligarVerde();
    }
    // Caso contrário, ignorar
  }
  
  delay(50);  // Pequeno delay para estabilidade
}


// Ligar LED vermelho e desligar LED verde
void ligarVermelho() {
  digitalWrite(LED_VERMELHO, HIGH);   // Liga vermelho
  digitalWrite(LED_VERDE, LOW);       // Desliga verde
  
  // Feedback ao PC
  Serial.println("LED VERMELHO LIGADO - LED VERDE DESLIGADO");
}

// Ligar LED verde e desligar LED vermelho
void ligarVerde() {
  digitalWrite(LED_VERMELHO, LOW);    // Desliga vermelho
  digitalWrite(LED_VERDE, HIGH);      // Liga verde
  
  // Feedback ao PC
  Serial.println("LED VERDE LIGADO - LED VERMELHO DESLIGADO");
}
