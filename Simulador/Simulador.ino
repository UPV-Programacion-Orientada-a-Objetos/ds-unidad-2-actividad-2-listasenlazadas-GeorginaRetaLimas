void setup() {
  // Inicializar comunicación serial a 9600 baudios
  Serial.begin(9600);
  
  // Esperar a que se establezca la conexión
  delay(3000);
}

void loop() {
  // Mensaje oculto: HELLO WORLD
  // Tramas en formato correcto: L,X o M,N
  
  const char* tramas[] = {
    "L,H",
    "L,E",
    "L,L",
    "M,2",
    "L,N",
    "L,Q",
    "L, ",
    "M,-2",
    "L,W",
    "L,O",
    "L,R",
    "L,L",
    "L,D",
  };
  
  int numTramas = 13;
  
  // Enviar cada trama
  for (int i = 0; i < numTramas; i++) {
    Serial.println(tramas[i]);
    delay(1000);  // 1 segundo entre tramas
  }
  
  // Esperar antes de repetir el ciclo
  delay(5000);
}