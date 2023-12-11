#include <reg51.h> // Incluye el archivo de cabecera para el 8051

/*Este ejemplo configura el Timer 0 en modo 1 (modo de temporizador de 16 bits) y luego inicia el temporizador.*/

void main() {
    TMOD = 0x01; // Configura el Timer 0 en modo 1 (16-bit Timer Mode)
    TH0 = 0x00; // Inicializa el valor alto del Timer 0
    TL0 = 0x00; // Inicializa el valor bajo del Timer 0
    TR0 = 1; // Inicia el Timer 0

    while(1) {
        // Aquí va tu código que se ejecutará mientras el temporizador está corriendo
    }
}