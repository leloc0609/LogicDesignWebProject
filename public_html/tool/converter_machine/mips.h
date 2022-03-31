// Header that defines registers, functions and constants for a MIPS processor
//mips.h
#ifndef MIPS
#define MIPS

// struct that will store each label
struct JumpTable{
	int line;
	char label[16];
};

//Global Variables
extern char inputLine[129];
extern char outputLine[129];

extern char opcodeBinary[7];        // G Var for Binary
extern char rsBinary[6];
extern char rtBinary[6];
extern char rdBinary[6];
extern char shamtBinary[6];
extern char functBinary[7];
extern struct JumpTable JumpAdressTable[20]; //array of structs for storing labels
extern char immediateBinary[17];   // exclusive type I
extern char addressBinary[27]; 	// exclusive type J

extern char instructionAssembly[6];     // G Var for Assembly
extern char rsAssembly[6];
extern char rtAssembly[6];
extern char rdAssembly[6];
extern char shamtAssembly[3];      // 2^5 = 32 (max)
//extern char functAssembly[6];

extern char immediateAssembly[6];   // 2^16 = 65538 (max) - 5 caracteres
extern char addressAssembly[27]; 	// Address
extern char labelAssembly[20];    // label for bne and beq functions
extern int pcAssembly;         // Program Counter Assembly (numero da linha lida)





/*
// R Format Instructions - 12 types (6 bits Funct) (opcode  000000)
add = R100000 	// 0/20 *
addu = R100001 	// 0/21
and = R100100 	// 0/24
jr = R001000	// 0/08
nor = R100111 	// 0/27
or  = R100101 	// 0/25
slt = R101010 	// 0/2a
sltu = R101011	// 0/2b
sll  = R000000	// 0/00
srl  = R000010	// 0/02
sub = R100010 	// 0/22
subu = R100011  // 0/23

// I Format Instructions - 17 types (6 bits opcode)
addi = I001000 		// 8
addiu = I001001		// 9
andi = I001100		// c
beq = I000100 		// 4
bne = I000101		// 5
lbu = I00100100 	// 24 
lhu = I100101 		// 25
ll = I110000		// 30
lui = I001111		// f
lw = I100011 		// 23
ori = I001101 		// d
slti = I001010 		// a
sltiu = I001011 	// b  
sb = I101000		// 28
sc = I111000		// 38
sh = I101001		// 29
sw = I101011		// 2b

// J Format Instructions - 2 types (6 bits opcode) 
j = J000010 		// 2
jal = J000011 		// 3

*/

//declaration of functions
//Assembly to Binary address
void charTo16Bits (char *charInput, char *charOutput);
void charTo16BitsU (char *charInput, char *charOutput);//unsigned I types
void charTo5BitsU (char *charInput, char *charOutput);//R types : srl,sll
void charTo26BitsU (char *charInput, char *charOutput); 

// Seperate the binary code
void ripBinaryLabel();
void ripBinaryR();
void ripBinaryI();
void ripBinaryJ();

//Binary to Assembly address
void binary16ToChar(char *charInput, char *charOutput);
void binary5ToCharU(char *charInput, char *charOutput); 
void binary16ToCharU(char *charInput, char *charOutput); 
void binary26ToCharU(char *charInput, char *charOutput);


// # function Assembly -> Binary
void registerToAssembly(char *registerBinary, char *registerAssembly);
void instructionR ();
void instructionI ();
void instructionJ ();
void concatenateRBinary();
void concatenateIBinary();
void concatenateIBinaryParenthesis();
void concatenateILabel(char *outputLine);	
void concatenateJBinary(); 

// functions Binary -> Assembly
void addToAssembly(); // R
void adduToAssembly(); // R
void andToAssembly(); // R
void jrToAssembly(); // R
void norToAssembly(); // R
void orToAssembly(); // R
void sltToAssembly(); // R
void sltuToAssembly(); // R
void sllToAssembly(); // R
void srlToAssembly(); // R
void subToAssembly(); // R
void subuToAssembly(); // R
void addiToAssembly();  // I
void andiToAssembly();  // I
void beqToAssembly();  // I parenteses
void bneToAssembly();  // I parenteses
void lbuToAssembly();  // I parenteses
void lhuToAssembly();  // I parenteses
void llToAssembly();  // I parenteses
void luiToAssembly();    // I
void lwToAssembly();    // I
void oriToAssembly();    // I
void sltiToAssembly();    // I
void sltiuToAssembly();    // I paranteses
void sbToAssembly();  // I parenteses
void scToAssembly();  // I parenteses
void shToAssembly();  // I parenteses
void swToAssembly();  // I parenteses
void jToAssembly();  // J
void jalToAssembly();  // J

// header
unsigned long countLine();

//binary sum - disregards carry - does not check inputs
int xor(int a, int b)
{
	if ( a==b )
	{
		return 0;
	}
	else
	{
		return a + b;
	}
}


void not (char *string)
{
	int i=0;
	do
	{
		if (string[i] == '0') 
		{
			string[i] = '1';    				
		}
		else
		{
			string[i]='0';
    	}
		i++;
	} while (string[i] != '\0');
}

/* Function that cycles through the entire file putting labels in the JumpAdressTable array of records */
void fillJumpAdressTable(){
    
    char line[129];
    char label[16];
    int lineCounter = 1, i = 0, jumpTableCounter = 0;
    unsigned long numberOfLines = countLine();
    FILE *file;
    
    file = fopen("./code.txt", "r");
    
    if (file == NULL){
        printf( "<br>Error no file<br>" );
    }
    else{
		while ( fgets (line, 129, file)  ) 
		{
			// printf( "<br>Table line test: " );
			// puts (line);
	        char ch;	   
	        if(isLabel(line)){
	                i = 0;
	                do{
	                    ch = line[i];
	                    if(ch != ':'){
	                        label[i] = ch;
	                        i++;
	                    }
	                    else{
	                        label[i] = '\0';
	                    }
	                }while(ch != ':');
	                strcpy(JumpAdressTable[jumpTableCounter].label, label);
	                JumpAdressTable[jumpTableCounter].line = lineCounter;
	                jumpTableCounter++;
            }
            lineCounter++;
            i = 0;
		}
	}
    fclose(file);
}

int searchLabel(char label[]){

	int line, i = 0, found = 0;

	while(i < 20 && !found){
		if(!strcmp(label, JumpAdressTable[i].label)){
			line = JumpAdressTable[i].line;
			found = 1;
		}
		i++;
	}

	return line;
}
//Execute the address field of the assembly to binary
void padZero(char *string, int length)
{
	int i=0, originalLength;
	originalLength = strlen(string);  //size before filling
	char stringTemp[17] = {0};        // array temporaries
	strncpy (stringTemp, string, originalLength); 
	stringTemp[originalLength] = '\0';
	
	int padLength = length - strlen(string);  // difference that we will fill with '0'
	if (padLength < 0) padLength = 0;    // We avoid negative sizes
	
	for (i=0; i < padLength; i++)
	{
		string[i] = '0';		
	}
	strncpy ( (string + padLength) ,stringTemp, originalLength);   // concatenates back into the string (now with zeros) the original content
}
// returns 1 if the character is numeric, including the hyphen for negatives
// returns 0 if any other.
int isNumeric (char ch)
{
	int output;
	switch (ch){
		case '0': output = 1;
		case '1': output = 1;
		case '2': output = 1;
		case '3': output = 1;
		case '4': output = 1;
		case '5': output = 1;
		case '6': output = 1;
		case '7': output = 1;
		case '8': output = 1;
		case '9': output = 1;
		case '-': output = 1; 
		break;
	default:
		output = 0;
	}
	return output;
}

// similar a funcao isNumeric() 
int isBinary (char ch)
{
	int output;
	switch (ch){
		case '0': output = 1;
		case '1': output = 1;
		break;
	default:
		output = 0;
	}
	return output;
}

void concatenateR(char *outputLine)  //generates R type line
{
	strcpy (opcodeBinary,"000000");	 
	strcpy (shamtBinary,"00000"); 	
	
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, rsBinary);
	strcat (outputLine, rtBinary);
	strcat (outputLine, rdBinary);
	strcat (outputLine, shamtBinary);
	strcat (outputLine, functBinary);
}

void concatenateI(char *outputLine)	//generates I type line
{
	charTo16Bits (immediateAssembly, immediateBinary );
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, rtBinary);  //rs na verdade
	strcat (outputLine, rdBinary);  //rt na verdade
	strcat (outputLine, immediateBinary);
}

void concatenateILabel(char *outputLine)	//generates I type line
{
	int offsetLabel;
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, rtBinary);  //rs na verdade
	strcat (outputLine, rdBinary);  //rt na verdade
	//strcat (outputLine, labelAssembly);  // converte Label
	puts(labelAssembly);
	offsetLabel = searchLabel(labelAssembly);
	printf("<br>Label line: %d - PC: %d", offsetLabel, pcAssembly);
	offsetLabel = (offsetLabel - pcAssembly - 1);     // 
	sprintf(addressAssembly , "%d" , offsetLabel );	
	charTo16Bits(addressAssembly, addressBinary);
	strcat (outputLine, addressBinary);  // converte Label
}

void concatenateIU(char *outputLine)	//generates I type line
{
	charTo16BitsU (immediateAssembly, immediateBinary );
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, rtBinary); 
	strcat (outputLine, rdBinary); 
	strcat (outputLine, immediateBinary);
}

void addToBinary() // R
{
	strcpy (functBinary,"100000");	 
	concatenateR(outputLine);
}

void addiToBinary()  // I
{
	strcpy (opcodeBinary,"001000");
	concatenateI(outputLine);  
}

void addiuToBinary()  // IU
{
	strcpy (opcodeBinary,"001001");
	concatenateIU(outputLine); 
}

void adduToBinary() // R   -- revisar
{
	strcpy (functBinary,"100001");
	concatenateR(outputLine);  
}

void andToBinary() //  R
{
	strcpy (functBinary,"100100");
	concatenateR(outputLine);  
}

void andiToBinary() // I
{
	strcpy (opcodeBinary,"001100");
	concatenateI(outputLine);  
}
		
void beqToBinary() // I
{
	strcpy (opcodeBinary,"000100");
	concatenateILabel(outputLine);
}

void bneToBinary() // I
{
	strcpy (opcodeBinary,"000101");
	//ripar o label
	concatenateILabel(outputLine);
}
//With the J type,we decide to concatenate here because it is short
void jToBinary()  // J  
{
	int i = 2; //  recorta o label
	do {
		addressAssembly[i-2] = inputLine[i];
		i++;
	} while (inputLine[i] != '\n');
	
	int label;
	label = searchLabel(addressAssembly);
	itoa(label,addressAssembly,10);
	
	charTo26BitsU(addressAssembly, addressBinary);
	strcpy (opcodeBinary,"000010");
	
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, addressBinary);
}

void jalToBinary()  // J  
{
	strcpy (opcodeBinary,"000011");
	int i = 4; //  recorta o label
	do {
		addressAssembly[i-4] = inputLine[i];
		i++;
	} while (inputLine[i] != '\n');
	
	int label;
	label = searchLabel(addressAssembly);
	itoa(label,addressAssembly,10);
	
	charTo26BitsU(addressAssembly, addressBinary);
	
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, addressBinary);
}

void jrToBinary()   // R -  usar rs 
{					
	strcpy (functBinary,"001000");	
	strcpy (opcodeBinary,"000000");	 	
	strcpy (shamtBinary,"00000"); 	
	strcpy (rsBinary,"00000"); 
	strcpy (rdBinary,"00000"); 	
	
	strcpy (outputLine, opcodeBinary);
	strcat (outputLine, rtBinary);  
	strcat (outputLine, rsBinary);   
	strcat (outputLine, rdBinary);  
	strcat (outputLine, shamtBinary);
	strcat (outputLine, functBinary);
}

void lbuToBinary() // I
{
	strcpy (opcodeBinary,"100100");
	concatenateIU(outputLine);  
}

void lhuToBinary() // I  Unsigned
{
	strcpy (opcodeBinary,"100101");
	concatenateIU(outputLine);  	
}

void llToBinary() // I
{
	strcpy (opcodeBinary,"110000");
	concatenateI(outputLine);   	
}

void luiToBinary() // I  C2?  Load Upper Immed 
{
	strcpy (opcodeBinary,"001111");
	concatenateI(outputLine);  	
}

void lwToBinary() // I
{
	strcpy (opcodeBinary,"100011");
	concatenateI(outputLine);  	
}

void norToBinary()   // R
{
	strcpy (functBinary,"100111");	
	concatenateR(outputLine);
}	

void orToBinary()   // R
{
	strcpy (functBinary,"100101");	
	concatenateR(outputLine); 
}	

void oriToBinary() // I
{
	strcpy (opcodeBinary,"001101");
	printf("deu merda2");
	concatenateI(outputLine); 
}		

void sltToBinary() // R
{
	strcpy (functBinary,"101010");	
	concatenateR(outputLine); 
}

void sltiToBinary() // I
{
	strcpy (opcodeBinary,"001010");	
	concatenateI(outputLine); 
}

void sltiuToBinary() // I
{
	strcpy (opcodeBinary,"001011");	
	concatenateIU(outputLine);  
}

void sltuToBinary() // R   e U how???
{
	strcpy (functBinary,"101011");	
	concatenateR(outputLine);  
}

void sllToBinary() // R    
{
	strcpy (opcodeBinary,"000000");
	strcpy (functBinary,"000000");
	strcpy (rsBinary ,"00000");
	charTo5BitsU (immediateAssembly, shamtBinary ); //escreve no shamt
	
	strcpy (outputLine, opcodeBinary);  // concatena avulso por causa do shamt
	strcat (outputLine, rsBinary);
	strcat (outputLine, rtBinary);
	strcat (outputLine, rdBinary);
	strcat (outputLine, shamtBinary);
	strcat (outputLine, functBinary);
}

void srlToBinary() // R   
{
	strcpy (opcodeBinary,"000000");
	strcpy (functBinary,"000010");	
	strcpy (rsBinary ,"00000");
	charTo5BitsU (immediateAssembly, shamtBinary ); //escreve no shamt
	
	strcpy (outputLine, opcodeBinary);  // concatena avulso por causa do shamt
	strcat (outputLine, rsBinary);
	strcat (outputLine, rtBinary);
	strcat (outputLine, rdBinary);
	strcat (outputLine, shamtBinary);
	strcat (outputLine, functBinary);
}

void sbToBinary() // I
{
	strcpy (opcodeBinary,"101000");	
	concatenateI(outputLine);   // "empacota" a linha
}		

void scToBinary() // I
{
	strcpy (opcodeBinary,"111000");	
	concatenateI(outputLine);   // "empacota" a linha
}		

void shToBinary() // I
{
	strcpy (opcodeBinary,"101001");	
	concatenateI(outputLine);   // "empacota" a linha
}			

void swToBinary() // I
{
	strcpy (opcodeBinary,"101011");	
	concatenateI(outputLine);   // "empacota" a linha
}	

void subToBinary() // R
{
	strcpy (functBinary,"100010");	
	concatenateR(outputLine);   // "empacota" a linha
}

void subuToBinary() // R  --- atencao funcao R! mas eh U?
{
	strcpy (functBinary,"100011");	
	concatenateR(outputLine);   // "empacota" a linha
}

// ##################

void ripDataAssembly (char *myLine)    
										
{
	char rs[4], rt[4], rd[4];
	char immediate[6];  //only store until 65536 = 2^16 = 65536 unsigned - greater number than a 16bit immediate U can have. + 1 terminator
	char address[26]; 	// size open to review
	char label[20];
	char ch, chProx;
	int offset = 0, isNum = 0, tamanho = 0;

	//	this first part searches registrars - basically looking for "$"
	do
	{
		offset++;
		ch = myLine[offset]; 
	} while (ch != '$' && ch != '\0');
	
	if ( ch == '$' )
	{
		strncpy(rt,(myLine + offset),3);		
		rt[3] = '\0';
	}
	
	if ( ch != '\0' ) 
	{
		do
		{
			offset++;
			ch = myLine[offset];
		} while (ch != '$' && ch != '\0');
		
		if ( ch == '$' ) {
			strncpy(rd,(myLine + offset),3); 
			rd[3] = '\0';
		}
		
		if ( ch != '\0' ) //ainda nao acabou  ?
		{
			do
			{
				offset++;
				ch = myLine[offset];
			} while (ch != '$' && ch != '\0');
			
			if ( ch == '$' ){
				strncpy(rs,(myLine + offset),3); 			
				rs[3] = '\0';
			}
		}
	}
		
	// this part looks for the immediate (integer), if it exists
	// the technique consists of taking an element after the comma that doesn't have "$" so it is after a comma
	// we assume that we don't have spaces after the comma, and that we eventually have an immediate between parentheses.
	offset = 0; //important to reset the offset as we will rescan the line
	do
	{
		offset++;
		ch = myLine[offset];
		if ( ch == ',' ) {    
			offset++;
			chProx = myLine[offset];
			isNum = isNumeric(chProx);	
		}
	} while ( !isNum && ch != '\0' );  
	
	if ( ch != '\0' ) 
	{
		tamanho = offset;   
			do {			
				tamanho++;
				chProx = myLine[tamanho];
				isNum = isNumeric(chProx);
			} while (isNum);   
			tamanho = tamanho - offset;		
			strncpy(immediate,(myLine + offset),tamanho);
			immediate[tamanho] = '\0';
	}
	
	
	// rip Label (bne e beq)
	offset = 0;  //important to reset the offset as we will rescan the line
	do
	{
		offset++;
		ch = myLine[offset];
		if ( ch == ',' ) {    // 1a virgula
			do
			{
				offset++;
				ch = myLine[offset];
				if ( ch == ',' ) {    
					offset++;
					tamanho = offset;   
					
					do 				   //until the end of the line
					{						//verifica onde termina o label   
						tamanho++;
						ch = myLine[tamanho];
					} while ( ch != '\0' );  
					
					tamanho = tamanho - offset-1;		 // makes the difference to know the size of the immediate
					strncpy(label,(myLine + offset),tamanho);		//copY do immediate
					label[tamanho] = '\0';
				}
			} while ( ch != '\0' );
		}
		
	} while ( ch != '\0' ); // only exits this loop when \0 or finds a comma followed by a numeric
	
	if (!(strcmp(rs,"$ze")))   
		strcpy(rs,"$zero");
	if (!(strcmp(rt,"$ze")))
		strcpy(rt,"$zero");
	if (!(strcmp(rd,"$ze")))
		strcpy(rd,"$zero");
	
	
	strcpy ( rsAssembly, rs); 
	strcpy ( rtAssembly, rt);
	strcpy ( rdAssembly, rd);
	strcpy ( immediateAssembly, immediate);
	strcpy ( labelAssembly, label);
	
}

// function that returns the binaries of each Record - in case of error returns xxxxx
char *registerToBinary(char *registerAssembly)
{
	static char *registerBinary;
	registerBinary = (char *)malloc(6); // tamanho maximo do nome das funcoes, incluindo terminator
	
	if (!(strcmp(registerAssembly,"$zero"))){  		  //retorna 0 se iguais, portanto !0 = verdadeiro
		strcpy(registerBinary,"00000");
	} else if (!(strcmp(registerAssembly,"$at"))) {   	// 1 	Assemble temporary
		strcpy(registerBinary,"00001");		
	} else if (!(strcmp(registerAssembly,"$v0"))) {		// 2 	Function Results and Expression Evaluation
		strcpy(registerBinary,"00010");				
	} else if (!(strcmp(registerAssembly,"$v1"))) {
		strcpy(registerBinary,"00011");			
	} else if (!(strcmp(registerAssembly,"$a0"))) {		// 4	Arguments
		strcpy(registerBinary,"00100");		
	} else if (!(strcmp(registerAssembly,"$a1"))) {
		strcpy(registerBinary,"00101");		
	} else if (!(strcmp(registerAssembly,"$a2"))) {
		strcpy(registerBinary,"00110");		
	} else if (!(strcmp(registerAssembly,"$a3"))) {
		strcpy(registerBinary,"00111");		
	} else if (!(strcmp(registerAssembly,"$t0"))) {		// 8	Temporaries
		strcpy(registerBinary,"01000");		
	} else if (!(strcmp(registerAssembly,"$t1"))) {
		strcpy(registerBinary,"01001");
	} else if (!(strcmp(registerAssembly,"$t2"))) {
		strcpy(registerBinary,"01010");
	} else if (!(strcmp(registerAssembly,"$t3"))) {
		strcpy(registerBinary,"01011");
	} else if (!(strcmp(registerAssembly,"$t4"))) {
		strcpy(registerBinary,"01100");
	} else if (!(strcmp(registerAssembly,"$t5"))) {
		strcpy(registerBinary,"01101");
	} else if (!(strcmp(registerAssembly,"$t6"))) {
		strcpy(registerBinary,"01110");
	} else if (!(strcmp(registerAssembly,"$t7"))) {
		strcpy(registerBinary,"01111");												
	} else if (!(strcmp(registerAssembly,"$s0"))) {		// 16	Saved Temporaries
		strcpy(registerBinary,"10000");		
	} else if (!(strcmp(registerAssembly,"$s1"))) {
		strcpy(registerBinary,"10001");
	} else if (!(strcmp(registerAssembly,"$s2"))) {
		strcpy(registerBinary,"10010");
	} else if (!(strcmp(registerAssembly,"$s3"))) {
		strcpy(registerBinary,"10011");
	} else if (!(strcmp(registerAssembly,"$s4"))) {
		strcpy(registerBinary,"10100");
	} else if (!(strcmp(registerAssembly,"$s5"))) {
		strcpy(registerBinary,"10101");
	} else if (!(strcmp(registerAssembly,"$s6"))) {
		strcpy(registerBinary,"10110");
	} else if (!(strcmp(registerAssembly,"$s7"))) {
		strcpy(registerBinary,"10111");		
	} else if (!(strcmp(registerAssembly,"$t8"))) {		// 24	Temporaries
		strcpy(registerBinary,"11000");
	} else if (!(strcmp(registerAssembly,"$t9"))) {
		strcpy(registerBinary,"11001");			
	} else if (!(strcmp(registerAssembly,"$k0"))) {		// 26	Reserved for OS Kernel
		strcpy(registerBinary,"11010");
	} else if (!(strcmp(registerAssembly,"$k1"))) {
		strcpy(registerBinary,"11011");
	} else if (!(strcmp(registerAssembly,"$gp"))) {
		strcpy(registerBinary,"11100");
	} else if (!(strcmp(registerAssembly,"$sp"))) {
		strcpy(registerBinary,"11101");
	} else if (!(strcmp(registerAssembly,"$fp"))) {
		strcpy(registerBinary,"11110");
	} else if (!(strcmp(registerAssembly,"$ra"))) {
		strcpy(registerBinary,"11111");					
	}
	else {
		strcpy(registerBinary,"xxxxx");    //caso de Erro, nenhum registro compativel encontrado
	}
	registerAssembly=registerBinary;
	return (char *)registerBinary;   //(char *)registerBinary;
}

// receives two strings by reference, the first is the input ("numbers")
// the second returns the 5-bit UNSIGNED binary string
void charTo5BitsU (char *charInput, char *charOutput)   
{
	int num = 33; // workaround to get out of the second if
	if ( charInput[0] != '-' ) //procura numeros negativos, eles nem deveriam ser chamados
		num=atoi(charInput);   //num receberá o char passado para int
	
	if( num<32 ) // limite do range de 16bits
	{
        //itoa(int ,char* ,base int);
        itoa(num,charOutput,2);  // converts the integer 'num' to a base 2 binary and puts it in the string 'charBinary'
		padZero(charOutput, 5);
		charOutput[5] = '\0';
	}else
	{
	    strcpy(charOutput,"XXXXX");  //if cannot be found
		charOutput[5] = '\0';
	}
}
// takes two strings by reference, the first is the input ("numbers") 
// the second returns the 16-bit UNSIGNED binary string
void charTo16BitsU (char *charInput, char *charOutput)
{
	int num = 65537; // 2^16+1
	if ( charInput[0] != '-' ) //not negative
		num=atoi(charInput); 
	//if negative we treat it as 65537
	if( num<65536 ) // limite do range de 16bits
	{
        	itoa(num,charOutput,2); // change from char to binary  
		padZero(charOutput, 16);
	}else
	{
	    strcpy(charOutput,"XXXXXXXXXXXXXXXX");  // saida de erro
		charOutput[16] = '\0';
	}
}
// receives two strings by reference, the first is the input ("numbers")
// the second returns the 2-complement binary string in 16 bits
//Signed numbers
void charTo16Bits (char *charInput, char *charOutput)
{

	int num=atoi(charInput);   //str->int	
	
	if (-32768 <= num < 32768)
	{
		if (num>=0) // positive 
		{
	        	itoa(num,charOutput,2); // converts the integer 'num' to a base 2 binary and puts it in the string 'charBinary'
			padZero(charOutput, 16);  //complete with zeros
		}
		else // negative
		{
		    itoa ( (num*(-1)) -1 , charOutput,2 ); // if the number is negative, it is passed to positive (and we subtract 1)
		    padZero(charOutput, 16);  //complete with zeros
		    not (charOutput);   	//array complement
		}
	}else
	{
		strcpy(charOutput,"XXXXXXXXXXXXXXXX");  // error
	}
}
//J types address
void charTo26BitsU (char *charInput, char *charOutput)
{
	int num = 67108865; // 2^26+1
	if ( charInput[0] != '-' ) //look for negative numbers
		num=atoi(charInput);   //str->int
	
	if( num<67108864 )
	{
        	itoa(num,charOutput,2);  // converts the integer 'num' to binary and puts it in the string 'charBinary'
		padZero(charOutput, 26);
	}else
	{
	    strcpy(charOutput,"XXXXXXXXXXXXXXXXXXXXXXXXXX");  // Error
		charOutput[26] = '\0';
	}
}

//Binary to assembly in address field 
//receives two strings per reference, the first being a 16bit binary string
//second("the output") in a string representing integer
void binary16ToChar(char *charInput, char *charOutput)
{
	int i,aux=0;
	char tempInput[17];
	
	if(charInput[0]=='0') //the last bit, in 2's complement, is reserved for sign representation, so if it is '0' it means the number is positive.
	{
		strcpy(tempInput,charInput);
		aux = strtol(tempInput, &charOutput, 2); //function that converts binary, in one string, to a decimal in another string
		itoa ( aux, charOutput, 10 ); // change aux to char in decimal if there is the number else 0
	}
	else //treatment for negative numbers
	{
		strcpy(tempInput,charInput);
		not(tempInput);change all bits
		aux = strtol( tempInput, &charOutput, 2); 
		aux = (aux+1) * -1 ;if there is number +1 and after that -1 
		itoa ( aux, charOutput, 10 );
	}
	strcpy(immediateAssembly,charOutput);
}

void binary5ToCharU(char *charInput, char *charOutput)
{
	int aux=0;
	char tempInput[6];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // function that converts binary, in one string, to a decimal in another string
	itoa ( aux, charOutput, 10 ); // change aux to char in decimal if there is the number else 0
	strcpy(shamtAssembly,charOutput);
}

void binary16ToCharU(char *charInput, char *charOutput) // Unsigned I or R type
{
	int aux=0;
	char tempInput[17];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // function that converts binary, in one string, to a decimal in another string
	itoa ( aux, charOutput, 10 ); // change aux to char in decimal if there is the number else 0
	strcpy(immediateAssembly,charOutput);
}

void binary26ToCharU(char *charInput, char *charOutput) // J type
{
	int aux=0;
	char tempInput[27];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // function that converts binary, in one string, to a decimal in another string
	itoa ( aux, charOutput, 10 ); // change aux to char in decimal if there is the number else 0
	strcpy(addressAssembly,charOutput); // store in addressAssembly
}

//Seperate assembly code based on the name of functions
void filterInstruction(char *instruction)
{
	if (!(strcmp(instruction,"add"))){
		addToBinary();
	} else if (!(strcmp(instruction,"addi"))){
		addiToBinary();
	} else if (!(strcmp(instruction,"addiu"))){
		addiuToBinary();	
	} else if (!(strcmp(instruction,"and"))){
		andToBinary();
	} else if (!(strcmp(instruction,"andi"))){
		andiToBinary();
	} else if (!(strcmp(instruction,"beq"))){  // label?
		beqToBinary();
	} else if (!(strcmp(instruction,"bne"))){  // label?
		bneToBinary();
	} else if (!(strcmp(instruction,"j"))){    // J
		jToBinary();
	} else if (!(strcmp(instruction,"jal"))){   // J
		jalToBinary();
	} else if (!(strcmp(instruction,"jr"))){   // R
		jrToBinary();
	} else if (!(strcmp(instruction,"lbu"))){
		lbuToBinary();
	} else if (!(strcmp(instruction,"lhu"))){
		lhuToBinary();
	} else if (!(strcmp(instruction,"ll"))){
		llToBinary();
	} else if (!(strcmp(instruction,"lui"))){
		luiToBinary();
	} else if (!(strcmp(instruction,"lw"))){
		lwToBinary();
	} else if (!(strcmp(instruction,"nor"))){
		norToBinary();
	} else if (!(strcmp(instruction,"or"))){
		orToBinary();
	} else if (!(strcmp(instruction,"ori"))){
		printf("deu merda1");
		oriToBinary();
	} else if (!(strcmp(instruction,"slt"))){
		sltToBinary();
	} else if (!(strcmp(instruction,"slti"))){
		sltiToBinary();
	} else if (!(strcmp(instruction,"sltiu"))){
		sltiuToBinary();
	} else if (!(strcmp(instruction,"sltu"))){
		sltuToBinary();		
	} else if (!(strcmp(instruction,"sll"))){
		sllToBinary();
	} else if (!(strcmp(instruction,"srl"))){
		srlToBinary();
	} else if (!(strcmp(instruction,"sb"))){
		sbToBinary();
	} else if (!(strcmp(instruction,"sc"))){
		scToBinary();
	} else if (!(strcmp(instruction,"sh"))){
		shToBinary();
	} else if (!(strcmp(instruction,"sw"))){		
		swToBinary();
	} else if (!(strcmp(instruction,"sub"))){
		subToBinary();
	} else if (!(strcmp(instruction,"subu"))){
		subuToBinary();	
	}
	else {
		printf("Error knowing instruction!<br>");  //Error
	}		
}


// #######  Functions of binary -> Assembly

void getOpcodeBinary (char *inputLine)
{
	strncpy (opcodeBinary, inputLine, 6);
	opcodeBinary[6] = '\0';
}

void ripDataBinary(char *opcodeBinary)
{
	if (!(strcmp(opcodeBinary,"000000"))){  // R
		ripBinaryR();
	} else if ( (!(strcmp(opcodeBinary,"000010"))) || (!(strcmp(opcodeBinary,"000011"))) ){  // J
		ripBinaryJ();
	} else {  // I
		ripBinaryI();
	}
}
// Spliting the bianry into parts
void ripBinaryR()
{
	strncpy(rsBinary, inputLine+6, 5);  // split according to the R function
	strncpy(rtBinary, inputLine+11, 5);
	strncpy(rdBinary, inputLine+16, 5);
	strncpy(shamtBinary, inputLine+21, 5);
	strncpy(functBinary, inputLine+26, 6);
	
	registerToAssembly(rsBinary, rsAssembly);  
	registerToAssembly(rtBinary, rtAssembly);
	registerToAssembly(rdBinary, rdAssembly);
	instructionR ();  // escreve o funct na global
}

void ripBinaryJ()
{
	strncpy(addressBinary, inputLine+6, 26);   // split according to the J function
	instructionJ ();  // escreve o funct na global
}

void ripBinaryI()
{
	strncpy(rsBinary, inputLine+6, 5);   // split according to the I function
	strncpy(rtBinary, inputLine+11, 5);
	strncpy(immediateBinary, inputLine+16, 16);
	
	registerToAssembly(rsBinary, rsAssembly); 
	registerToAssembly(rtBinary, rtAssembly);
	instructionI ();
}

// function that converts a binary record to an assembly
void registerToAssembly(char *registerBinary, char *registerAssembly)
{
	if (!(strcmp(registerBinary,"00000"))){  		
		strcpy(registerAssembly,"$zero");
	} else if (!(strcmp(registerBinary,"00001"))) {   	// 1 	Assemble temporary
		strcpy(registerAssembly,"$at");		
	} else if (!(strcmp(registerBinary,"00010"))) {		// 2 	Function Results and Expression Evaluation
		strcpy(registerAssembly,"$v0");				
	} else if (!(strcmp(registerBinary,"00011"))) {
		strcpy(registerAssembly,"$v1");			
	} else if (!(strcmp(registerBinary,"00100"))) {		// 4	Arguments
		strcpy(registerAssembly,"$a0");		
	} else if (!(strcmp(registerBinary,"00101"))) {
		strcpy(registerAssembly,"$a1");		
	} else if (!(strcmp(registerBinary,"00110"))) {
		strcpy(registerAssembly,"$a2");		
	} else if (!(strcmp(registerBinary,"00111"))) {
		strcpy(registerAssembly,"$a3");		
	} else if (!(strcmp(registerBinary,"01000"))) {		// 8	Temporaries
		strcpy(registerAssembly,"$t0");		
	} else if (!(strcmp(registerBinary,"01001"))) {
		strcpy(registerAssembly,"$t1");
	} else if (!(strcmp(registerBinary,"01010"))) {
		strcpy(registerAssembly,"$t2");
	} else if (!(strcmp(registerBinary,"01011"))) {
		strcpy(registerAssembly,"$t3");
	} else if (!(strcmp(registerBinary,"01100"))) {
		strcpy(registerAssembly,"$t4");
	} else if (!(strcmp(registerBinary,"01101"))) {
		strcpy(registerAssembly,"$t5");
	} else if (!(strcmp(registerBinary,"01110"))) {
		strcpy(registerAssembly,"$t6");
	} else if (!(strcmp(registerBinary,"01111"))) {
		strcpy(registerAssembly,"$t7");												
	} else if (!(strcmp(registerBinary,"10000"))) {		// 16	Saved Temporaries
		strcpy(registerAssembly,"$s0");		
	} else if (!(strcmp(registerBinary,"10001"))) {
		strcpy(registerAssembly,"$s1");
	} else if (!(strcmp(registerBinary,"10010"))) {
		strcpy(registerAssembly,"$s2");
	} else if (!(strcmp(registerBinary,"10011"))) {
		strcpy(registerAssembly,"$s3");
	} else if (!(strcmp(registerBinary,"10100"))) {
		strcpy(registerAssembly,"$s4");
	} else if (!(strcmp(registerBinary,"10101"))) {
		strcpy(registerAssembly,"$s5");
	} else if (!(strcmp(registerBinary,"10110"))) {
		strcpy(registerAssembly,"$s6");
	} else if (!(strcmp(registerBinary,"10111"))) {
		strcpy(registerAssembly,"$s7");		
	} else if (!(strcmp(registerBinary,"11000"))) {		// 24	Temporaries
		strcpy(registerAssembly,"$t8");
	} else if (!(strcmp(registerBinary,"11001"))) {
		strcpy(registerAssembly,"$t9");			
	} else if (!(strcmp(registerBinary,"11010"))) {		// 26	Reserved for OS Kernel
		strcpy(registerAssembly,"$k0");
	} else if (!(strcmp(registerBinary,"11011"))) {
		strcpy(registerAssembly,"$k1");
	} else if (!(strcmp(registerBinary,"11100"))) {
		strcpy(registerAssembly,"$gp");
	} else if (!(strcmp(registerBinary,"11101"))) {
		strcpy(registerAssembly,"$sp");
	} else if (!(strcmp(registerBinary,"11110"))) {
		strcpy(registerAssembly,"$fp");
	} else if (!(strcmp(registerBinary,"11111"))) {
		strcpy(registerAssembly,"$ra");					
	} else {
		strcpy(registerAssembly ,"$xx");   //Wrong case
	}
}
// Binary-> assembly based on instructions
void instructionR ()
{
	if (!(strcmp(functBinary,"100000"))){  		  //returns 0 if equal, so !0 = true
		addToAssembly(); // R		
	} else if (!(strcmp(functBinary,"100001"))){  		 
		adduToAssembly(); // RU	
	} else if (!(strcmp(functBinary,"100100"))){
		andToAssembly();  //R
	} else if (!(strcmp(functBinary,"001000"))){
		jrToAssembly();  //R
	} else if (!(strcmp(functBinary,"100111"))){
		norToAssembly();  //R			
	} else if (!(strcmp(functBinary,"100101"))){
		orToAssembly();  //R
	} else if (!(strcmp(functBinary,"101010"))){
		sltToAssembly();  //R
	} else if (!(strcmp(functBinary,"101011"))){
		sltuToAssembly();  //R
	} else if (!(strcmp(functBinary,"000000"))){
		sllToAssembly();  //R
	} else if (!(strcmp(functBinary,"000010"))){
		srlToAssembly();  //R
	} else if (!(strcmp(functBinary,"100010"))){
		subToAssembly();  //R
	} else if (!(strcmp(functBinary,"100011"))){
		subuToAssembly();  //RU
	} else {
		strcpy(instructionAssembly ,"Rxxxx");    //caso de Erro, nenhum instruction compativel encontrado
	}	
}

void instructionJ ()
{
	if (!(strcmp(opcodeBinary,"000010"))){  		  //retorna 0 se iguais, portanto !0 = verdadeiro
		jToAssembly(); 
	} else if (!(strcmp(opcodeBinary,"000011"))){
		jalToAssembly();   		 
	} else {
		strcpy(instructionAssembly ,"Jxxxx");    //caso de Erro, nenhum instruction compativel encontrado
	}		
}

void instructionI ()
{
	if (!(strcmp(opcodeBinary,"001000"))){  		  
		addiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001100"))){
		andiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"000100"))){
		beqToAssembly();     // I
	} else if (!(strcmp(opcodeBinary,"000101"))){
		bneToAssembly(); 	 // I	
	} else if (!(strcmp(opcodeBinary,"100100"))){
		lbuToAssembly(); 	 // I parenteses	
	} else if (!(strcmp(opcodeBinary,"100101"))){
		lhuToAssembly(); 	 // I parenteses
	} else if (!(strcmp(opcodeBinary,"110000"))){
		llToAssembly(); // I parenteses - rever
	} else if (!(strcmp(opcodeBinary,"001111"))){
		luiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"100011"))){
		lwToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001101"))){
		oriToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001010"))){
		sltiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001011"))){
		sltiuToAssembly();    // IU
	} else if (!(strcmp(opcodeBinary,"101000"))){
		sbToAssembly();    // I parenteses
	} else if (!(strcmp(opcodeBinary,"111000"))){
		scToAssembly();    // I ??
	} else if (!(strcmp(opcodeBinary,"101001"))){
		shToAssembly();    // I parenteses
	} else if (!(strcmp(opcodeBinary,"101011"))){
		swToAssembly();    // I ??
	} else {
		strcpy(instructionAssembly ,"Ixxxx");    //Error
	}	
}
// Concatenation of Assembly after from Binary
void concatenateRBinary()  //generates R type line Binary
{
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rtAssembly);
	strcat (outputLine, ",");
	strcat (outputLine, rdAssembly);
	strcat (outputLine, ",");
	strcat (outputLine, rsAssembly);
}

void concatenateIBinary()	//generates I type line Binary
{
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rsAssembly); 
	strcat (outputLine, ",");
	strcat (outputLine, rtAssembly); 
	strcat (outputLine, ",");	
	strcat (outputLine, immediateAssembly);  
}

void concatenateIBinaryParenthesis()	//generates I type line Binary
{
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rsAssembly);
	strcat (outputLine, ",");
	strcat (outputLine, immediateAssembly); 
	strcat (outputLine, "(");
	strcat (outputLine, rtAssembly);
	strcat (outputLine, ")");	
}

void concatenateJBinary()	//generates J type line Binary
{
	binary26ToCharU (addressBinary, addressAssembly);
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, addressAssembly);
}

//  ###  Binary functions -> assembly
void addToAssembly() // R
{
	strcpy(instructionAssembly,"add");
	concatenateRBinary(outputLine); a
}

void adduToAssembly() // R
{
	strcpy(instructionAssembly,"addu");
	concatenateRBinary(outputLine); 
}

void andToAssembly() // R
{
	strcpy(instructionAssembly,"and");
	concatenateRBinary(outputLine);
}

void jrToAssembly() // R
{
	strcpy(instructionAssembly,"jr");
	
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rsAssembly);
}

void norToAssembly() // R
{
	strcpy(instructionAssembly,"nor");
	concatenateRBinary(outputLine);
}

void orToAssembly() // R
{
	strcpy(instructionAssembly,"or");
	concatenateRBinary(outputLine); 
}

void sltToAssembly() // R
{
	strcpy(instructionAssembly,"slt");
	concatenateRBinary(outputLine);   
}

void sltuToAssembly() // R
{
	strcpy(instructionAssembly,"sltu");
	concatenateRBinary(outputLine); 
}

void sllToAssembly() // R
{
	strcpy(instructionAssembly,"sll");
	
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rtAssembly);
	strcat (outputLine, ",");
	strcat (outputLine, rdAssembly);
	strcat (outputLine, ",");
	binary5ToCharU (shamtBinary, shamtAssembly);
	strcat (outputLine, shamtAssembly);
}

void srlToAssembly() // R
{
	strcpy(instructionAssembly,"srl");
	
	strcpy (outputLine, instructionAssembly);
	strcat (outputLine, " ");
	strcat (outputLine, rtAssembly);
	strcat (outputLine, ",");
	strcat (outputLine, rdAssembly);
	strcat (outputLine, ",");
	binary5ToCharU (shamtBinary, shamtAssembly);
	strcat (outputLine, shamtAssembly);
}

void subToAssembly() // R
{
	strcpy(instructionAssembly,"sub");
	concatenateRBinary(outputLine); 
}

void subuToAssembly() // R
{
	strcpy(instructionAssembly,"subu");
	concatenateRBinary(outputLine); 
}
// tipo I
void addiToAssembly()  // I
{
	strcpy(instructionAssembly,"addi");
	binary16ToChar(immediateBinary, immediateAssembly);  
	concatenateIBinary(outputLine);  
}

void andiToAssembly()  // I
{
	strcpy(instructionAssembly,"andi");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine); 
}

void beqToAssembly()  // I
{
	strcpy(instructionAssembly,"beq");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine); 
}

void bneToAssembly()  // I
{
	strcpy(instructionAssembly,"bne");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine); 
}

void lbuToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"lbu");
	binary16ToCharU(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine); 
}

void lhuToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"lhu");
	binary16ToCharU(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine); 
}

void llToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"ll");
	binary16ToCharU(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine);   
}

void luiToAssembly()    // I
{
	strcpy(instructionAssembly,"lui");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine);   
}

void lwToAssembly()    // I parenteses
{
	strcpy(instructionAssembly,"lw");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine); 
}

void oriToAssembly()    // I
{
	strcpy(instructionAssembly,"ori");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine);   
}

void sltiToAssembly()    // I
{
	strcpy(instructionAssembly,"slti");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine);  
}

void sltiuToAssembly()    // IU
{
	strcpy(instructionAssembly,"sltiu");
	binary16ToCharU(immediateBinary, immediateAssembly);
	concatenateIBinary(outputLine);  
}

void sbToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"sb");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine); 
}

void scToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"sc");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine);   
}

void shToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"sh");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine);   
}

void swToAssembly()  // I parenteses
{
	strcpy(instructionAssembly,"sw");
	binary16ToChar(immediateBinary, immediateAssembly);
	concatenateIBinaryParenthesis(outputLine); 
}

void jToAssembly()  // J
{
	strcpy(instructionAssembly,"j");
	concatenateJBinary(); // monta a linha
}

void jalToAssembly()  // J
{
	strcpy(instructionAssembly,"jal");
	concatenateJBinary(); // monta a linha
}

#endif