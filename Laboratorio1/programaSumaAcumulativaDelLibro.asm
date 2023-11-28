###############################################################
#			Estudiantes:			      #
#		Alejandro Cerpa 30.334.870                    #
#		Jeanmarco Alarcon 27.117.926                  #
###############################################################


# Datos estaticos para el programa:
	.data
mensaje: .asciiz "Digite el numero mayor a 0 para sacar su suma acumulativa: "
mensaje3: .asciiz "El resultado es: "
	.text
main:
	la $a0, mensaje       # Lee la direccion del mensaje para una llamada a una rutina(servicio) del sistema o syscall
	li $v0, 4	      # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
	syscall		      # Esta sección imprime la cadena
	li   $v0, 5           # Especifica que syscall usará un servicio de lectura de enteros
	syscall               # Lee un numero por pantalla y lo guarda en el registro de resultados $v0.
	blt  $v0, $zero, main # Si el numero ingresado es menor a 0 salta a "pr" para volver a leer un numero
	add  $s0, $v0, $zero  # Aqui se transfiere el numero ingresado al registro con el que se va a trabajar $s0
	
	add  $s1, $zero, $zero  # Aqui se transfiere el numero ingresado al registro con el que se va a trabajar $s1
	
	add  $a0, $s0, $zero  # Preparar el primer argumento para el rodecimiento de suma acumulativa recursiva
	add  $a1, $s1, $zero  # Preparar el segundo argumento para el rodecimiento de suma acumulativa recursiva
	
	jal sum		      # Llamada al prodecimiento de suma acumulativa recursiva
	
	add  $s0, $v0, $zero
	
	la $a0, mensaje3      # Lee la direccion del mensaje para una llamada a una rutina(servicio) del sistema o syscall
	li $v0, 4	      # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
	syscall		      # Esta sección imprime la cadena
	
	add $a0, $s0, $zero   # Da como argumento a imprimir el resultado de la suma
	li $v0, 1	      # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
	syscall		      # Esta sección imprime la cadena
	
	li   $v0, 10          # Especifica que syscall usará un servicio de salida
	syscall               # Programa finalizado

		
###############################################################
# Procedimiento de suma recursiva.
      .text
sum:    beq $a0, $zero, sum_exit # salto a sum_exit si n <= 0
	add $a1, $a1, $a0 # suma n a acc
	addi $a0, $a0, -1 # decrementa n
	j sum # salto a sum
sum_exit:
	add $v0, $a1, $zero # devuelve el valor acc
	jr $ra # retorno a la funcion main
# Final del procedimiento para imprimir los nuemro Fibonacci
###############################################################
