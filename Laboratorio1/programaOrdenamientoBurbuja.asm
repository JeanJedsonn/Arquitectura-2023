###############################################################
#			Estudiantes:			      #
#		Alejandro Cerpa 30.334.870                    #
#		Jeanmarco Alarcon 27.117.926                  #
###############################################################


# Programa que obtiene varios numeros Fibonacci
.data
vector:.word 9 0 6 8 2 8 4 3 5 1 # "Vector" de palabras de memoria (32 bits = 4 bytes) que contendra los numeros del arreglo
tamano: .word  10                # Tamaño del "arreglo" del tamaño del vector
mensaje: .asciiz "\nOrdenando numeros en el vector con ordenamiento burbuja..." # mensaje a mostrar en pantalla
.text

main:   la $a0, vector		# $a0 es el argumento de la dirección del vector para print
	la $a1, tamano		# $a1 es el argumento de la dirección del tamaño del vector para print
	lw $a1, 0($a1)		# $a1 es el argumento del tamaño del vector para print
	
	jal print		# Imprimir el vector
	
	la $a0, mensaje         # Lee la direccion del mensaje para una llamada a una rutina(servicio) del sistema o syscall
	li $v0, 4	        # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
	syscall		        # Esta sección imprime la cadena
	
	la $a1, vector		# $a1 es el argumento de la dirección del vector para print
	la $a0, tamano		# $a0 es el argumento de la dirección del tamaño del vector para print
	lw $a0, 0($a0)		# $a0 es el argumento del tamaño del vector para print
	
	jal burbuja		# Ordenar el vector
	
	la $a0, vector		# $a0 es el argumento de la dirección del vector para print
	la $a1, tamano		# $a1 es el argumento de la dirección del tamaño del vector para print
	lw $a1, 0($a1)		# $a1 es el argumento del tamaño del vector para print
	
	jal print		# Imprimir el vector

	# Está parte es equivalente a return 0 de la función "main" en c, en otras palabras finaliza el programa.
	li   $v0, 10          # Especifica que syscall usará un servicio de salida
	syscall               # Programa finalizado
		
###############################################################
# Procedimiento que imprime los numeros en el vector en una linea.
      .data
space:.asciiz  " "          # Esta etiqueta servira para dar los espacios entre numeros
head: .asciiz  "\nLos numeros en el vector son:\n"
      .text
print:add  $t0, $zero, $a0  # Carga en el registro temporal $t0 la direccion del "vector" para imprimir los numeros
      add  $t1, $zero, $a1  # Carga la cantidad de numeros a imprimir en $t1 (iterador)
      la   $a0, head        # Carga la direccion de la cadena a imprimir con syscall
      li   $v0, 4           # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
      syscall               # Imprime la cadena "head"
      
out:  lw   $a0, 0($t0)      # Carga el numero del vector que actualmente se debe imprimir
      li   $v0, 1           # Especifica que syscall usará un servicio de escritura de un entero por pantalla
      syscall               # Imprime el numero Fibonacci actual
      
      la   $a0, space       # Carga la direccion de la cadena a imprimir con syscall
      li   $v0, 4           # Especifica que syscall usará un servicio de escritura de cadenas de caracteres por pantalla
      syscall               # Coloca un espacio en pantalla
      
      addi $t0, $t0, 4      # Incrementa en 1 entero la dirección del "vector"
      addi $t1, $t1, -1     # desincrementa en 1 el iterador
      bgtz $t1, out         # Si no se han imprimido todos los numeros seguir imprimiendo
      
      jr   $ra              # Retornar al programa principal
# Final del procedimiento para imprimir los nuemros del vector
###############################################################

###############################################################
# Procedimiento Ordenamiento burbuja

      .text
burbuja:
	addi $sp, $sp, -12 	# Hace espacio en la pila del sistama para guardar 3 valores
	sw $s0, 8($sp)		# Guarda el valor del registro $s0
	sw $s1, 4($sp)		# Guarda el valor del registro $s1
	sw $s2, 0($sp)		# Guarda el valor del registro $s2
	sll $s2, $a0, 2		# Guarda en el registro $s2 el tamaño del vector * 4 para comprobar si se llego al final de el
	add $s0, $zero, $zero	# El registro $s0 funcionará como iterador i del vector
	addi $s2, $s2, -4	# Para evitar visitar una direccion extra en el vector se le resta un entero al tamaño en $s2
for1:
	beq $s0, $s2, salir2	# Si el iterador i llego al final del vector este ya está ordenao y se sale del ciclo principal
	add $s1, $zero, $zero	# Se inicializa el registro $s1 a 0 como iterador j
for2:
	beq $s1, $s2, salir1	# Si j es igual al tamaño del vector se sale del ciclo secundario
	add $t0, $a1, $s1	# la direccion principal del vector + el iterador j dan la direccion v[j] para $t0
	lw $t1, 0($t0)		# $t1 = v[j]
	lw $t2, 4($t0)		# $t2 = v[j+1]
	slt $t3, $t2, $t1	# $t3 = 1 si v[j+1] < j[j] sino $t3 = 0
	beq $t3, $zero, noCambio # Si $t3 = 0 evita intercambiar v[j] y v[j+1]
	sw $t2, 0($t0)		# Esta linea y la de abajo intercambian v[j] y v[j+1]
	sw $t1, 4($t0)		# Descripcion arriba 
noCambio:
	addi $s1, $s1, 4	# iterador j = j + 1
	j for2			# Continuar el bucle interior
salir1:
	addi $s0, $s0, 4	# iterador i = i + 1
	j for1			# Continuar el bucle principal
salir2:
	lw $s2, 0($sp)		# Restaurar el valor original de $s2 antes de la llamada al procedimiento
	lw $s1, 4($sp)		# Restaurar el valor original de $s1 antes de la llamada al procedimiento
	lw $s0, 8($sp)		# Restaurar el valor original de $s0 antes de la llamada al procedimiento
	addi $sp, $sp, 12	# Reacomoda la pila
	jr $ra			# Salir del procedimiento de ordenamiento burbuja
	
# Final del procedimiento Ordenamiento burbuja
###############################################################








