void setup() {
  // Inicializar comunicación serial a 9600 baudios
  Serial.begin(9600);
  
  // Esperar a que se establezca la conexión
  delay(2000);
  
  Serial.println("Arduino PRT-7 - Modo Cifrado");
  Serial.println("Enviando mensaje cifrado...");
  delay(1000);
}

void loop() {
  // Mensaje oculto: HELLO WORLD
  // Con cifrado César usando rotaciones del rotor
  
  const char* tramas[] = {
    // Enviar "HEL" sin cifrado (rotor en posición 0)
    "L,H",      // H -> H
    "L,E",      // E -> E  
    "L,L",      // L -> L
    
    // Rotar +2 y enviar "LO " cifrado
    "M,2",      // Rotar rotor +2 posiciones
    "L,N",      // L cifrado con +2 -> enviar 'N' para obtener 'L'
    "L,Q",      // O cifrado con +2 -> enviar 'Q' para obtener 'O'
    "L, ",      // Espacio (no se cifra)
    
    // Volver a posición 0 y enviar "WOR"
    "M,-2",     // Rotar rotor -2 posiciones (volver a 0)
    "L,W",      // W -> W
    "L,O",      // O -> O
    "L,R",      // R -> R
    
    // Enviar "LD" sin rotación adicional
    "L,L",      // L -> L
    "L,D"       // D -> D
  };
  
  int numTramas = 14;
  
  // Enviar cada trama con un delay
  for (int i = 0; i < numTramas; i++) {
    Serial.println(tramas[i]);
    delay(1500);  // Esperar 1.5 segundos entre tramas
  }
  
  // Mensaje de finalización
  Serial.println("--- Transmision completa ---");
  
  // Esperar 10 segundos antes de repetir
  delay(10000);
}