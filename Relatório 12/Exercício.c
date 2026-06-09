
#define BOTAO 2           // Botão conectado ao pino 2 (INT0)
#define LED_CONFIRMACAO 13 // LED de confirmação 

volatile int contador = 0;  // volatile porque é alterada na interrupção
String comando = "";        // String para armazenar comando recebido

void setup() {
  // Configurar comunicação Serial (9600 baud)
  Serial.begin(9600);
  
  // Configurar pino do botão como entrada
  pinMode(BOTAO, INPUT_PULLUP);  // INPUT_PULLUP ativa o resistor interno
  
  // Configurar LED de confirmação (opcional)
  pinMode(LED_CONFIRMACAO, OUTPUT);
  digitalWrite(LED_CONFIRMACAO, LOW);
  
  attachInterrupt(digitalPinToInterrupt(BOTAO), contagemBotao, FALLING);
  
  // Mensagem de inicialização
  Serial.println("Sistema iniciado!");
  Serial.println("Pressione o botão ou envie 'zerar' para resetar o contador");
  Serial.print("Contagem: ");
  Serial.println(contador);
}

void loop() {
  // Verificar se há dados disponíveis na porta Serial
  if (Serial.available() > 0) {
    char caractere = Serial.read();
    
    // Adicionar caractere à string de comando
    if (caractere != '\n' && caractere != '\r') {
      comando += caractere;
    }
    
    // Se receber quebra de linha, processar o comando
    if (caractere == '\n') {
      processarComando();
      comando = "";  // Limpar a string
    }
  }
  
  delay(50);  // Pequeno delay para estabilidade
}


void contagemBotao() {
  // Fazer debounce simples com delay
  delay(20);
  
  // Verificar se o botão ainda está pressionado
  if (digitalRead(BOTAO) == LOW) {
    // Incrementar o contador
    contador++;
    
    // Piscar LED para confirmação (opcional)
    piscaLED();
    
    // Enviar contagem ao PC
    Serial.print("Botao pressionado! Contagem: ");
    Serial.println(contador);
  }
}

// Processar comando recebido do PC
void processarComando() {
  // Converter para minúsculas para facilitar comparação
  comando.toLowerCase();
  comando.trim();  // Remover espaços em branco
  
  if (comando == "zerar") {
    contador = 0;
    Serial.println("Contador zerado!");
    Serial.println("Contagem: 0");
  }
  else if (comando == "status") {
    Serial.print("Contagem atual: ");
    Serial.println(contador);
  }
  else if (comando != "") {
    Serial.println("Comando nao reconhecido!");
    Serial.println("Comandos disponíveis: 'zerar', 'status'");
  }
}

// Piscar LED de confirmação
void piscaLED() {
  digitalWrite(LED_CONFIRMACAO, HIGH);
  delay(100);
  digitalWrite(LED_CONFIRMACAO, LOW);
}
