#include <reg51.h> // Incluye el archivo de cabecera para el 8051

/*Este ejemplo configura el puerto P1 como salida y luego enciende y apaga un LED conectado al pin P1.0.*/

void delay(void); // Prototipo de función para la demora

void main() {
    P1 = 0x00; // Configura el puerto P1 como salida

    while(1) {
        P1 = 0x01; // Enciende el LED conectado al pin P1.0
        delay(); // Espera
        P1 = 0x00; // Apaga el LED conectado al pin P1.0
        delay(); // Espera
    }
}

void delay() {
    int i, j;
    for(i = 0; i < 1000; i++)
        for(j = 0; j < 1000; j++);
}