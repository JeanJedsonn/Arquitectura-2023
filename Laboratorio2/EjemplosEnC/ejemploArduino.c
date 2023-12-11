// Definimos el pin del LED
#define LED_PIN 13


/* Este programa hace que un LED conectado al pin 13 parpadee.
El código encenderá el LED durante un segundo, luego lo apagará
 durante un segundo, y repetirá este ciclo indefinidamente.*/

// Función de configuración que se ejecuta una vez al inicio
void setup() {
  // Configuramos el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);
}

// Función de bucle que se ejecuta continuamente
void loop() {
  digitalWrite(LED_PIN, HIGH); // Encendemos el LED
  delay(1000); // Esperamos un segundo (1000 milisegundos)
  digitalWrite(LED_PIN, LOW); // Apagamos el LED
  delay(1000); // Esperamos un segundo
}