//Definição do uso dos pinos do Arduino
#define pinDistEcho 2     //am
#define pinDistTrigger 4  //lj

//Declaração das funções programadas abaixo
void enviaPulso();
void medeDistancia();

//Declaração das variaveis para Sensor de Distancia
volatile unsigned long inicioPulso = 0;
volatile float distancia = 0;
volatile int modo = -1;

//Configuração do projeto no Arduino
void setup() {
  //INICIA COMUNICACAO SERIAL
  Serial.begin(9600);

  //Configura o estado das portas do Arduino
  pinMode(pinDistEcho, INPUT);
  pinMode(pinDistTrigger, OUTPUT);

  //Desliga a porta de controle do sensor de distancia
  digitalWrite(pinDistTrigger, LOW);

  //CONFIGURA A INTERRUPÇÃO PARA FUNCIONAMENTO DO SENSOR DE DISTANCIA
  attachInterrupt(digitalPinToInterrupt(pinDistEcho), medeDistancia, CHANGE);
}

void loop() {

  //Teste do Sensor de Distancia
  //ENVIA O COMANDO PARA O MÓDULO LER A DISTANCIA
  enviaPulso();

  //A RESPOSTA DA DISTANCIA VEM POR INTERRUPÇÃO, SÓ PRECISA ESPERAR ALGUNS MILISSEGUNDOS
  delay(25);  // TEMPO DE RESPOSTA APÓS A LEITURA
  Serial.print(" Dist:");
  Serial.print(distancia);
  Serial.println("cm");

  delay(20);
}

//PROGRAMAÇÃO DAS FUNÇÕES

//Envia um pulso para o sensor de distância
void enviaPulso() {
  // ENVIA O SINAL PARA O MÓDULO INICIAR O FUNCIONAMENTO
  digitalWrite(pinDistTrigger, HIGH);
  // AGUARDAR 10 uS PARA GARANTIR QUE O MÓDULO VAI INICIAR O ENVIO
  delayMicroseconds(10);
  // DESLIGA A PORTA PARA FICAR PRONTO PARA PROXIMA MEDIÇÃO
  digitalWrite(pinDistTrigger, LOW);
  // INDICA O MODO DE FUNCIONAMENTO (AGUARDAR PULSO)
  modo = 0;
}

//Calcula a distância detectada
//A interrupção será acionada quando o reflexo do pulso for recebido e então executa esta função.
void medeDistancia() {
  switch (modo) {
    case 0:
      {
        inicioPulso = micros();
        modo = 1;
        break;
      }
    case 1:
      {
        distancia = (float)(micros() - inicioPulso) / 58.3;  // distancia em CM
        inicioPulso = 0;
        modo = -1;
        break;
      }
  }
}
