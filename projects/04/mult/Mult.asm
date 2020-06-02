// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// Initialize counter as first number
// Add second number till counter is zero
// Store value in RAM[2]

@R2  //Initialize final output
M=0

@R0   // Initialize counter as first number i.e n=x
D=M
@Halt  //If number is zero stop
D; JEQ 
@n   
M=D

(Loop)  // Add second number till counter is zero
@R1     
D=M
@R2     //Add x to output y times so that answer comes out to be x*y
M=D+M
@n      //Decrease counter and store value back in counter register
D=M     
MD=D-1  
@Loop
D; JNE 

@Halt   // loop forever
(Halt)
0;JMP