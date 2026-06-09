#define LED_VERDE 9      // LED verde no pino 9 (pino PWM)

String comando = "";     // String para armazenar valor recebido
int dutyCycle = 0;       // Armazena o duty cycle (0-100)
int valorPWM = 0;        // Armazena o valor PWM (0-255)

void setup() {
  // Configurar comunicação Serial (9600 baud)
  Serial.begin(9600);
  
  // Configurar pino do LED como saída
  pinMode(LED_VERDE, OUTPUT);
  
  // Iniciar LED desligado
  analogWrite(LED_VERDE, 0);
  
  // Mensagem de inicialização
  Serial.println("");
  Serial.println("  Controle de LED com PWM (Duty Cycle) ");
  Serial.println("");
  Serial.println("Envie um valor de 0 a 100 para controlar");
  Serial.println("  0   = LED desligado");
  Serial.println("  50  = Brilho médio");
  Serial.println("  100 = Brilho máximo");
  Serial.println("");
}

void loop() {
  // Verificar se há dados disponíveis na porta Serial
  if (Serial.available() > 0) {
    char caractere = Serial.read();
    
    // Adicionar caractere à string de comando se for dígito
    if (caractere >= '0' && caractere <= '9') {
      comando += caractere;
    }
    
    // Se receber quebra de linha, processar o comando
    if (caractere == '\n' || caractere == '\r') {
      if (comando.length() > 0) {
        processarComando();
      }
      comando = "";  // Limpar a string
    }
  }
  
  delay(50);  // Pequeno delay para estabilidade
}

// Processar comando recebido do PC
void processarComando() {
  // Converter string para inteiro
  dutyCycle = comando.toInt();
  
  // Validar se o valor está entre 0 e 100
  if (dutyCycle >= 0 && dutyCycle <= 100) {
    // Converter duty cycle (0-100) para PWM (0-255)
    valorPWM = map(dutyCycle, 0, 100, 0, 255);
    
    // Aplicar PWM ao LED
    analogWrite(LED_VERDE, valorPWM);
    
    // Exibir feedback
    exibirStatus();
  }
  else {
    // Erro: valor fora do intervalo
    Serial.println("❌ ERRO: Digite um valor entre 0 e 100!");
    Serial.print("   Você digitou: ");
    Serial.println(dutyCycle);
  }
}

// Exibir status atual do LED
void exibirStatus() {
  Serial.println("═══════════════════════════════════════");
  Serial.print("Duty Cycle: ");
  Serial.print(dutyCycle);
  Serial.println("%");
  
  Serial.print("Valor PWM: ");
  Serial.print(valorPWM);
  Serial.println("/255");
  
  // Barra visual do brilho
  Serial.print("Brilho: [");
  for (int i = 0; i < 20; i++) {
    if (i < (dutyCycle / 5)) {
      Serial.print("█");
    } else {
      Serial.print("░");
    }
  }
  Serial.println("]");
  Serial.println("═══════════════════════════════════════");
}
