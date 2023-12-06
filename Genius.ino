#define VERDE 8
#define AZUL 9
#define VERMELHO 10
#define AMARELO 11

#define B_VERDE 4
#define B_AZUL 5
#define B_VERMELHO 6
#define B_AMARELO 7

#define BUZZER 12

#define NOTA_VERDE 294
#define NOTA_AZUL 392
#define NOTA_VERMELHO 440
#define NOTA_AMARELO 880

typedef struct elemento{
    int dado;
    struct elemento *proximo;
} Elemento;

typedef struct fila{
    Elemento *inicio;
    Elemento *fim;
    int tamanho;    
} Fila;

Fila *inicializaFila(){
    Fila *f;
    f = (Fila *)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho=0;
    return f;
}

void insereElemento(int d, Fila *f){
  Elemento *novo;
  novo = (Elemento*)malloc(sizeof(Elemento));
  novo->dado = d;
  novo->proximo = NULL;
  
  if(f->inicio==NULL){
    f->inicio = novo;
    f->fim = novo;
  }else{
    f->fim->proximo = novo;
    f->fim = novo;
  }
  f->tamanho++;
}

void retiraDado(Fila *f){
  while (f->inicio != NULL) {
    Elemento* temp = f->inicio;;
    f->inicio = f->inicio->proximo;
    free(temp);
  }
  f->tamanho=0;
}

void tocaNota(int led){
  switch(led){
    case VERDE:
      tone(BUZZER,NOTA_VERDE);
      delay(100);
      noTone(BUZZER);
      break;
    case AZUL:
      tone(BUZZER,NOTA_AZUL);
      delay(100);
      noTone(BUZZER);
      break;
    case VERMELHO:
      tone(BUZZER,NOTA_VERMELHO);
      delay(100);
      noTone(BUZZER);
      break;
    case AMARELO: 
      tone(BUZZER,NOTA_AMARELO);
      delay(100);
      noTone(BUZZER);
      break;     
  }
}

void imprimeFila(Fila *f){
    if (f != NULL)
    {
        Elemento *iterador = f->inicio;
        while (iterador != NULL)
        {
            tocaNota(iterador->dado);
            ligaLED(iterador->dado);      
            iterador = iterador->proximo;
        }
        printf("\n");
    }
}


Fila *f1 = inicializaFila();
Fila *f2 = inicializaFila();
bool acertou = true;
bool start = 0;
int led = 0;

void ligaLED(int led){
  digitalWrite(led, HIGH);    // Liga led.
  delay(500);
  digitalWrite(led, LOW);  
  delay(500);
}

void ligaTudo(){
  for(int i=8;i<12;i++){
    digitalWrite(i, HIGH);    // Liga led.
  }
  delay(200);
  for(int i=8;i<12;i++){
    digitalWrite(i, LOW);   
  }
  delay(100);
}

int lerBotao(){
  if (digitalRead(B_VERDE) == LOW) { // Botão Pressionado;
    tocaNota(VERDE);
    return VERDE;
  }
  if (digitalRead(B_AZUL) == LOW) { // Botão Pressionado;
    tocaNota(AZUL);
    return AZUL;
  }
  if (digitalRead(B_VERMELHO) == LOW) { // Botão Pressionado;
    tocaNota(VERMELHO);;
    return VERMELHO;
  }
  if (digitalRead(B_AMARELO) == LOW) { // Botão Pressionado;
    tocaNota(AMARELO);;
    return AMARELO;
  }
  return 0;
}

bool comparar(Fila *f1, Fila *f2) { 
  Elemento *iterador1;
  Elemento *iterador2;
  iterador1 = f1->inicio;
  iterador2 = f2->inicio;

  while (iterador1 != NULL && iterador2!=NULL) {
    Serial.print(iterador1->dado);
    Serial.print("  -  ");
    Serial.print(iterador2->dado);
    Serial.print("\n");
    if(iterador1->dado != iterador2->dado){
      return false;
    }
    iterador2 = iterador2->proximo;
    iterador1 = iterador1->proximo;
  }
  return true;
}


void turnoArduino(){
  insereElemento(random(8,12),f1);   
  imprimeFila(f1);
}

void turnoPlayer(){
  for(int i=0;i<f1->tamanho;i++){

    led=lerBotao();

    while(led==0){
      led = lerBotao();  
    }

    insereElemento(led,f2);
    ligaLED(led);
    if(!(comparar(f1,f2))){    
      tone(BUZZER,50);delay(50);noTone(BUZZER);delay(50);
      tone(BUZZER,50);delay(50);noTone(BUZZER);delay(50); 
      i=f1->tamanho;
      retiraDado(f2);      
      start=0;
      retiraDado(f1);
    }
  } 
  retiraDado(f2);
}

void setup() {
  
  Serial.begin(9600);

  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  pinMode(VERMELHO, OUTPUT);
  pinMode(AMARELO, OUTPUT);

  pinMode(B_VERDE, INPUT_PULLUP);
  pinMode(B_AZUL, INPUT_PULLUP);
  pinMode(B_VERMELHO, INPUT_PULLUP);
  pinMode(B_AMARELO, INPUT_PULLUP); 

  pinMode(BUZZER, OUTPUT);
}

void loop() {

  led = lerBotao();

  if(led!=0){
    start = 1;
    delay(500);
  }else{
    ligaTudo();
  }
  while(start){
    turnoArduino();
    turnoPlayer();
  }

}
