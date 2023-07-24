.data
	texto1:	.asciiz "Introduzca el numero que desea obtener:"
	texto2: .ascii "el termino resultante es:"
	
.text
Main:
	la $a0,texto1	#carga direccion de la cadena
	li $v0,4	#instruccion de imprimir cadena
	syscall
	
	li $v0,5	#instruccion de leer un entero
	syscall
	
	
	move $t1,$v0	#tope, el n deseado se copia a un lugar seguro
	li $v0,1	#valor por defecto de fib en 1 y 2
	
	beq $t1,1,Ffor		#no tiene sentido evaluar el caso base
	beq $t1,2,Ffor		#
	
	li $a0,1	#1er elemento de fib
	li $a1,1	#2do elemento de fib
	
	li $t0,2	#i=2,el caso anterior de n=2 no es lo mismo 
	For: beq $t0,$t1,Ffor		#si t0=n , se termino de evaluar
		add $v0,$a0,$a1		#elemento i+1 de fib
		move $a0,$a1		#ahora a0 es el termino que le sigue
		move $a1,$v0		#ahora a1 es el termino calculado
	addi $t0,$t0,1	#incrementa en 1
	j For		#regresa para comparar si el ciclo termino
	Ffor:
	
	move $t0,$v0	#guarda el resultado
	la $a0,texto2	#carga la cadena
	li $v0,4	#instruccion de imprimir cadena
	syscall
	
	move $a0,$t0	#carga el resultado
	li $v0,1	#instruccion de imprimir entero
	syscall
	
	
li $v0,10
syscall