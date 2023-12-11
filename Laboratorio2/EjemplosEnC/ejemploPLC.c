#include <plclib.h> // Incluye la biblioteca para el PLC

/*Este código inicializa el PLC, luego entra en un bucle infinito donde lee la entrada 0 y,
si está activa, activa la salida 0. Si la entrada 0 no está activa, desactiva la salida 0.*/

void delay(void); // Prototipo de función para la demora

void main() {
    PLC_Init(); // Inicializa el PLC

    while(1) {
        if (PLC_ReadInput(0)) { // Si la entrada 0 está activa
            PLC_WriteOutput(0, 1); // Activa la salida 0
        } else {
            PLC_WriteOutput(0, 0); // Desactiva la salida 0
        }
    }
}