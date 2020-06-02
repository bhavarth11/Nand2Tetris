// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(Loop)
@SCREEN  //SCREEN = 16384
D=A
@addr 
M=D  //Screen's base address in addr variable

@KBD    //Check keyboard input
D=M
@Black
D; JNE //If any key is pressed go to code that turns screen black


(ClearScreen)
@addr  //If no key is pressed clear screen
A=M
M=0 //RAM[addr]=0000000000000000
@addr
M=M+1 //Move to next address  
D=M
@24576  //Check if last address(24576) is reached
D=A-D
@ClearScreen //If not reached repeat
D; JNE

@Loop  //Infinitely check if keyborad key is pressed
0; JMP

(Black)
(FillLoop)
@addr
A=M
M=-1 //RAM[addr]=1111111111111111
@addr
M=M+1  //Move to next address  
D=M
@24576  //Check if last address(24576) is reached
D=A-D
@FillLoop  //If not reached repeat
D; JNE


@Loop   //Infinitely check if keyborad key is pressed
0; JMP