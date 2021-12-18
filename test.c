#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 256
#define maxLenIdentifier 20

char cengLine[MAX_LEN]; //file line array
int length;             // line length
int count;              // number of '\"' in string
char temp_cengIdentifier[MAX_LEN];  // temp to keyword after bracket for control 
int rightp=0;  // count right bracket ')' 
int leftp=0;   // count left bracket '('
int rights=0;  // count right bracket '}'
int lefts=0;   // count left bracket '{'
int rightk=0; // count right bracket ']'
int leftk=0;    // count left bracket '['
int keyword_index=0;  // temp to keyword lastindex
bool flagFirstHalf=false;  // before := identifier control
bool flagSecondHalf=false;  // after := identifier control
char stringIdentifier[__INT16_MAX__]=",";  // temp to string variables 
char intIdentifier[__INT16_MAX__]=",";     // temp to integer variables
char lineIdentifier[__INT16_MAX__]=",";   // temp to variables in line 
char keyWord[__INT16_MAX__]="";          // temp to keyword in line 
char *delp;      // temp to variable after split array 
char *delp2;     //  temp to variable after split array 
int lastIndex=0;  // temp to lastindex every operation 
char * pch;       // temp to variable after split array 
char operator_temp;  // temp to operators in line 

char controlFunction(char cengLine[],FILE*lexfile);    // check the line call the function
char identifier(char cengLine[], int lastIndex,FILE*lexfile);  // find identifier function
char operators(char cengLine[],int lastIndex,FILE*lexfile);   // find operators function
char str_const(char line[],int lastIndex,FILE*lexfile);                   // find string function
char endofline(char cengLine[],int lastIndex,FILE*lexfile);  // find endofline function
char digit(char cengLine[],int lastIndex,FILE*lexfile);     // find digit function
char Comments(char line[],int lastIndex);                    // find comments function
char keywords(char cengLine[], int lastIndex,FILE*lexfile);  // find keywords function
char *get_substring(char *s, int i, int j);           
char brackets(char line[],int lastIndex,FILE*lexfile);      // find brackets function
char space_delete(char cengLine[],int lastIndex);            // space delete function
char after_bracket(char cengline[],int lastIndex,FILE*lexfile); // Control of situations that may come after the brackets and calling
char after_string(char cengline[],int lastIndex,FILE*lexfile);   // Control and search for situations that may come after the string
char after_keyword(char cengline[],int lastIndex,FILE*lexfile);  // Control and search for situations that may come after the keyword
char after_identifier(char cengline[],int lastIndex,FILE*lexfile); // Control and search for situations that may come after the identifier
char after_operator(char cengline[],int lastIndex,FILE*lexfile);  // Control and search for situations that may come after the operators
char after_digit(char cengline[],int lastIndex,FILE*lexfile);    //Control and search for situations that may come after the digit
char after_endofline(char cengLine[],int lastIndex,FILE*lexfile);  //Control and search for situations that may come after the endofline

int main(void){
	FILE* cengFile;
	cengFile = fopen("code.Ceng", "r"); //"code.Ceng"
	FILE* lexFile;
	lexFile = fopen("code.lex", "w");
	bool flagFirstHalf=false;
	bool flagSecondHalf=false;
	int lastIndex=0;
	while (fgets(cengLine, MAX_LEN - 1, cengFile)){
		controlFunction(cengLine,lexFile);
		keyWord[0]='\0';
		lineIdentifier[1]='\0';
		flagFirstHalf=false;
		flagSecondHalf=false;
		lastIndex=0; 
	}
	if(rights!=lefts){   
        printf("Curly bracket Error\n");
        exit(EXIT_SUCCESS);
	}
	fclose(lexFile);
	fclose(cengFile);
	return 0;
}
// the appropriate function is called by looking inside the line or at the beginning of the line
char controlFunction(char cengLine[],FILE*lexfile){    
	cengLine[strcspn(cengLine, "\n")] = 0;   
	lastIndex=0; 
    leftp=0,
    rightp=0;
    leftk=0;
    rightk=0;	
	while(lastIndex<strlen(cengLine)){
		cengLine[lastIndex]=tolower(cengLine[lastIndex]);
		lastIndex++;
	}
	lastIndex=0;
	flagFirstHalf=false;
	flagSecondHalf=false;
	lastIndex=space_delete(cengLine,lastIndex);
	if(strstr(cengLine,"if") || strstr(cengLine,"for") ||strstr(cengLine,"while") || strstr(cengLine,"float")|| strstr(cengLine,"switch")||strstr(cengLine,"printf")){  
		lastIndex=keywords(cengLine,lastIndex,lexfile);
		lastIndex=space_delete(cengLine, lastIndex);
		after_keyword(cengLine,lastIndex,lexfile);
	}
	else if(strstr(cengLine,"int") || strstr(cengLine,"char") || strstr(cengLine,"float")|| strstr(cengLine,"long")){  
		lastIndex=keywords(cengLine,lastIndex,lexfile);
		lastIndex=space_delete(cengLine, lastIndex);
		after_keyword(cengLine,lastIndex,lexfile);
	}
	else if(strstr(cengLine,"else")){
		lastIndex=keywords(cengLine,lastIndex,lexfile);
		after_keyword(cengLine,lastIndex,lexfile);
	}
	else if(isalpha(cengLine[lastIndex]))   {	
			lastIndex=identifier(cengLine,lastIndex,lexfile);
			after_identifier(cengLine,lastIndex,lexfile);
		
	}
	if(cengLine[lastIndex]=='}'){
		lastIndex=brackets(cengLine,lastIndex,lexfile);
	}
}
// Calling a function according to the situations after working on the endofline
char after_endofline(char cengLine[],int lastIndex,FILE*lexfile){
         if(rightp!=leftp && rightp!=0){
            printf("Bracket Error \n");
            exit(EXIT_SUCCESS);
            rightp=0;
            leftp=0;
        }
	lastIndex=Comments(cengLine,lastIndex);
}
// Space delete 
char space_delete(char cengLine[],int lastIndex){
	while(true){
		if(isspace(cengLine[lastIndex])){
			lastIndex++;
		}	
		else break;
	}
	return lastIndex;
}
// Calling a function according to the situations after working on the brackets
char after_bracket(char cengline[],int lastIndex,FILE*lexfile){
    lastIndex=space_delete(cengLine,lastIndex);
	if(cengline[lastIndex]=='\"'){
		lastIndex=str_const(cengline,lastIndex,lexfile);
        after_string(cengLine,lastIndex,lexfile);
	}
	else if(isdigit(cengline[lastIndex])){
		lastIndex=digit(cengline,lastIndex,lexfile);
		after_digit(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]=='(' ||cengline[lastIndex]=='[' || cengline[lastIndex]==')'||cengline[lastIndex]=='}') {
        lastIndex=brackets(cengline,lastIndex,lexfile);
		after_bracket(cengline,lastIndex,lexfile);
	}
    else if(cengline[lastIndex]==']'){
        lastIndex=operators(cengLine,lastIndex,lexfile);
		after_operator(cengLine,lastIndex,lexfile);
    }
	else if(cengline[lastIndex]=='{'){
		if(lastIndex==strlen(cengLine)){
			
		}
	}
	else if(cengLine[lastIndex]==';'){
		lastIndex=endofline(cengline,lastIndex,lexfile);	
        after_endofline(cengLine,lastIndex,lexfile);	
	}
	else if(cengLine[lastIndex]=='+'||cengLine[lastIndex]=='*'||cengLine[lastIndex]=='-'||cengLine[lastIndex]=='/'||(cengLine[lastIndex]==':'&&cengLine[lastIndex+1]=='=')){
          if(rightk!=leftk){   
            printf("Braket Error \n");
            exit(EXIT_SUCCESS);
            rightk=0;
            leftk=0;
        }
		lastIndex=operators(cengLine,lastIndex,lexfile);
		after_operator(cengLine,lastIndex,lexfile);
	}
	else if(lastIndex<strlen(cengLine)){
        
		lastIndex=identifier(cengline,lastIndex,lexfile);
		after_identifier(cengline,lastIndex,lexfile);
	}
}
// Calling a function according to the situations after working on the string
char after_string(char cengline[],int lastIndex,FILE*lexfile){   
    lastIndex=space_delete(cengLine,lastIndex);
	
    if(cengline[lastIndex]=='+' ||cengline[lastIndex]=='-' ||cengline[lastIndex]=='/' ||cengline[lastIndex]=='*' ||cengline[lastIndex]==':' ||cengline[lastIndex]=='='){
		lastIndex=operators(cengline,lastIndex,lexfile);
		after_operator(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]=='(' || cengline[lastIndex]=='{'||cengline[lastIndex]=='[' || cengline[lastIndex]==')'||cengline[lastIndex]=='}' || cengline[lastIndex]==']') {
		lastIndex=brackets(cengline,lastIndex,lexfile);
        after_bracket(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]==';'){
		lastIndex=endofline(cengline,lastIndex,lexfile);
        after_endofline(cengLine,lastIndex,lexfile);
	}
    else{	
	    printf("String Error \n");
        exit(EXIT_SUCCESS);
	}
}
// Calling a function according to the situations after working on the keyword
char after_keyword(char cengline[],int lastIndex,FILE*lexfile){   
    lastIndex=space_delete(cengLine,lastIndex);
	if (strstr(cengLine,"while")|| strstr(cengLine,"if")|| strstr(cengLine,"for")|| strstr(cengLine,"printf")|| strstr(cengLine,"else")){
		lastIndex=brackets(cengline,lastIndex,lexfile);
		after_bracket(cengline,lastIndex,lexfile);
	}
	else if (strstr(cengLine,"char")|| strstr(cengLine,"int")|| strstr(cengLine,"float")|| strstr(cengLine,"long")){		
        lastIndex=identifier(cengline,lastIndex,lexfile);
		
		after_identifier(cengline,lastIndex,lexfile);
	}
}
// Calling a function according to the situations after working on the identifier
char after_identifier(char cengline[],int lastIndex,FILE*lexfile){
    lastIndex=space_delete(cengLine,lastIndex);
	
	if(cengline[lastIndex]=='+' ||cengline[lastIndex]=='-' ||cengline[lastIndex]=='/' ||cengline[lastIndex]=='*' ||cengline[lastIndex]==':' ||cengline[lastIndex]=='='){
		lastIndex=operators(cengline,lastIndex,lexfile);
		after_operator(cengline,lastIndex,lexfile);
	}
	else if(cengLine[lastIndex]=='(' && cengLine[lastIndex+1]=='*'){
		printf("Endofline Error \n");
        exit(EXIT_SUCCESS);
	}
	else if(cengline[lastIndex]=='(' || cengline[lastIndex]=='{'||cengline[lastIndex]=='[' || cengline[lastIndex]==')'||cengline[lastIndex]=='}' || cengline[lastIndex]==']') {
		lastIndex=brackets(cengline,lastIndex,lexfile);
		after_bracket(cengline,lastIndex,lexfile);
	}
	else if(strlen(cengLine)==lastIndex && !strstr(cengLine,";"))
	{
		printf("Endofline Error \n");
		exit(EXIT_SUCCESS);
	}
	else if(cengline[lastIndex]==';'){
		lastIndex=endofline(cengline,lastIndex,lexfile);
		lastIndex=space_delete(cengLine,lastIndex);
		after_endofline(cengLine,lastIndex,lexfile);
	}
    else if(rightp==leftp && cengLine[lastIndex]=='(' && !strstr(cengLine,";")){
         printf("Endofline Erros\n");
         exit(EXIT_SUCCESS);
    }
}
// Calling a function according to the situations after working on the operator
char after_operator(char cengline[],int lastIndex,FILE*lexfile)
{ 
    lastIndex=space_delete(cengLine,lastIndex);  
    if(cengline[lastIndex]=='\"'){
       
		lastIndex=str_const(cengline,lastIndex,lexfile);
		after_string(cengline,lastIndex,lexfile);
	}
	else if(isdigit(cengline[lastIndex])){
		lastIndex=digit(cengline,lastIndex,lexfile);
		lastIndex=space_delete(cengLine,lastIndex);
		after_digit(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]=='(' || cengline[lastIndex]=='{'||cengline[lastIndex]=='[' || cengline[lastIndex]==')'||cengline[lastIndex]=='}' || cengline[lastIndex]==']') {
		lastIndex=brackets(cengline,lastIndex,lexfile);
		after_bracket(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]==';'){
		if(operator_temp=='+'){
			printf("Operator Error \n");
            exit(EXIT_SUCCESS);
		}
		lastIndex=endofline(cengline,lastIndex,lexfile);
        after_endofline(cengLine,lastIndex,lexfile);
	}
	else{
		lastIndex=identifier(cengline,lastIndex,lexfile);
        lastIndex=space_delete(cengLine,lastIndex);
		after_identifier(cengline,lastIndex,lexfile);
	}
}
// Calling a function according to the situations after working on the digit
char after_digit(char cengline[],int lastIndex,FILE*lexfile){
    lastIndex=space_delete(cengLine,lastIndex);	
    if(cengline[lastIndex]=='+' ||cengline[lastIndex]=='-' ||cengline[lastIndex]=='/' ||cengline[lastIndex]=='*' ||cengline[lastIndex]==':' ||cengline[lastIndex]=='='){
		lastIndex=operators(cengline,lastIndex,lexfile);
        lastIndex=space_delete(cengLine,lastIndex);
        after_operator(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]=='(' || cengline[lastIndex]=='{'||cengline[lastIndex]=='[' || cengline[lastIndex]==')'||cengline[lastIndex]=='}' || cengline[lastIndex]==']') {
		lastIndex=brackets(cengline,lastIndex,lexfile);
		after_bracket(cengLine,lastIndex,lexfile);
	}
	else if(cengline[lastIndex]==';'){
		lastIndex=endofline(cengline,lastIndex,lexfile);
        after_endofline(cengLine,lastIndex,lexfile);
	}
}
// Get a string substring
char *get_substring(char *s, int i, int j) {
	int len = j - i + 1;
	char *sub = (char *)malloc((len * sizeof(char)) + 1);
	sub[len] = '\0';
	strncpy(sub, s + i, len); 
	return sub;
}
// string search and check string accuracy
char str_const(char line[],int lastIndex,FILE*lexfile){
	count=0;
	length=strlen(line);
	int return_index=0; // temp to string lastindex
	int semicolon=0; // temp to semicolon index 
    char tempIntIdentifier[MAX_LEN]; // temp to integer identifier
	bool identifier=true; // string identifier control 
   strcpy(tempIntIdentifier,intIdentifier);
	delp=strtok(lineIdentifier,",");
	while(delp!=NULL){
		if(!strstr(line,"printf") && !strstr(stringIdentifier,delp)){
            	identifier=false;    
		}
		delp=strtok(NULL,",");
	}
    
    delp2=strtok(intIdentifier,",");
    char tempInt[MAX_LEN]="";
    while(delp2!=NULL){
        strcat(tempInt,"[");
        strcat(tempInt,delp2);
        strcat(tempInt,"]");
        if(strstr(line,tempInt)){
            identifier=true;
            break;
        }
       delp2=strtok(NULL,",");
	   tempInt[0]='\0';
    }
 	strcpy(intIdentifier,tempIntIdentifier);

	if(!strstr(line,"printf") && !strstr(stringIdentifier,lineIdentifier)){
		identifier=false;
	}
    if(identifier!=false){
		for(int i=lastIndex;i<length;i++){
			if(line[i]=='\"'){
				count++;
				if(count==2){
					//printf("String is correct\n");
					fprintf(lexfile,"String\n");
					return i+1;
				}
			}
			if(count==2 && line[i]=='+'){
				break;
			}
			if(line[i]==';'){
                semicolon=i;
				break;
			}
		}
	}
	else if(identifier==false){
		return_index=semicolon;
		printf("String is NOT correct ");
        exit(EXIT_SUCCESS);
	}
	return return_index;
}
// Comments search and check comment accuracy
char Comments(char line[],int lastIndex){  
	length=strlen(line);
	count=0;  
	bool flag_1=true; // control index semicolon and index comments first paranthes 
	bool flag=true; // control between semicolon and comments
	char str_cntrl[MAX_LEN]; // temp to between semicolon and comments
	int temp_nv=0; // semicolon  index
	int temp_cmm=0; // (* index
	int returnindex=0;
	if((strstr(line,"(*") || strstr(line,"*)"))&& strstr(line,";")){
		for(int j=lastIndex;j<strlen(line);j++){
			if((j)!=0 && line[j]==';'){
				temp_nv=j;
			}
			if((j+1)!=length && line[j]=='(' && line[j+1]=='*'){
				temp_cmm=j;
			}
		}
		if(temp_nv>temp_cmm){
			flag_1=false;
			printf("Comment NOT Correct");
            exit(EXIT_SUCCESS);
		}
		if(flag_1==true){
			char *sub = get_substring(line, temp_nv+1, temp_cmm-1);
			strcpy(str_cntrl,sub);
			for(int i=0;i<strlen(str_cntrl);i++){
				if(isspace(str_cntrl[i])==0){
					flag=false;
					break;
				}
			}
			if(flag==false){
				printf("Comment NOT Correct");
                exit(EXIT_SUCCESS);
			}
			if(flag==true){
				for(int y=lastIndex-1;y<strlen(line);y++){
					if((y+1)!=length && line[y]=='(' && line[y+1]=='*'){
						if(line[length-1]==')' && line[length-2]=='*'){
							returnindex=(length-1); // index ==> ) for *) 
							//printf("Comment Correct\n");
						}
						else if(line[length-1]!=')' || line[length-2]!='*'){
							printf("Comment NOt Correct");
                            exit(EXIT_SUCCESS);
						}
					}
				}
			}
		}
		return returnindex;
	}
}
// Brackets search and check bracket accuracy
char brackets(char line[],int lastIndex,FILE*lexfile){    
	int temp_index=0; // temp to after keyword first parenthes index
	int i=lastIndex;  //temp to lastindex
	int kose_index=0; // temp  index for control between square brackets
	char str[MAX_LEN]; // temp the line after the keyword in
	bool flag_error=true;  // control bracket after keyword
	keyword_index=lastIndex;
	char str_kose[MAX_LEN]; // temp to  between square brackets
	if(strstr(line,"if") || strstr(line,"while") || strstr(line,"for")|| strstr(line,"switch")||strstr(line,"printf")){
		char *sub_1 = get_substring(line,keyword_index ,strlen(line)-1);
		strcpy(str,sub_1);
		if(strlen(str)>0 && strstr(str,"(")){
			for(int j=lastIndex;j<strlen(line);j++){
				if(line[j]=='('){
					temp_index=j;
					break;
				}
			}
			char *sub_2 = get_substring(line,keyword_index ,temp_index-1);
			strcpy(str,sub_2);
			for(int i=0;i<strlen(str);i++){
				if(isspace(str[i])==0){
					printf("Error \n");
					flag_error=false;
                    exit(EXIT_SUCCESS);
					break;
				}
			}
		}
		else if(!strstr(line,"(")){
			printf("Syntax Error\n");
			flag_error=false;
            exit(EXIT_SUCCESS);
		}
	}
	if(line[i]=='('){
		//printf("LeftPar\n");
		fprintf(lexfile,"LefthPar\n");
		strcat(temp_cengIdentifier,"(");
		leftp++;
		i++;
	}
	if(line[i]==')'){ 
        //printf("RigthPar\n");
		fprintf(lexfile,"RigthPar\n");
		strcat(temp_cengIdentifier,")");
		rightp++;	
        i++;
	}    
	if(line[i]=='{' && rightp==leftp && !strstr(line,"else")){
		int len_temp=strlen(temp_cengIdentifier);
		if(temp_cengIdentifier[len_temp-1]==')'){
			//printf("LeftCurlyBracket\n");
			fprintf(lexfile,"LeftCurlyBracket\n");
			strcat(temp_cengIdentifier,"{");
			lefts++;
			i++;
		}
		else if(temp_cengIdentifier[len_temp-1]!=')'){
		    printf("Curly Bracket Error \n");
            exit(EXIT_SUCCESS);
        }		
	}
	else if(line[i]=='{' && (strstr(line,"else")||strstr(line,"while"))){
		lefts++;
		i++;
		//printf("LeftCurlyBracket\n");
		fprintf(lexfile,"LeftCurlyBracket\n");
	}
    else if(line[i]!='{' &&leftp==rightp&&leftp>0&& (strstr(line,"else")||strstr(line,"while"))){
		printf("Curly Bracket Error\n");
            exit(EXIT_SUCCESS);
	}
	if(line[i]=='}'&& rightp==leftp){
		//printf("RightCurlyBracket\n");
		fprintf(lexfile,"RightCurlyBracket\n");
		strcat(temp_cengIdentifier,"}");
		rights++;	
		if(rights>lefts){
			printf("Curly Bracket Error\n");
            exit(EXIT_SUCCESS);
			rights=0;
			lefts=0;
		}
		else 
			i++;
	}
	else if(line[i]=='}'){
		printf("Bracket Error \n");
        exit(EXIT_SUCCESS);
	}
	if(line[i]=='['){
		//printf("LeftSquareBracket\n");
		fprintf(lexfile,"LeftSquareBracket\n");
		leftk++;     
		for(int k=i;k<strlen(line);k++){
			if(line[k]==']'){ 
				kose_index=k;
				break;
			}
		}
	        i++;	
		}
		if(line[i]==']'){
			//printf("RightSquareBracket\n");
			fprintf(lexfile,"RightSquareBracket\n");
			rightk++;	
			if(rightk!=leftk){
				printf("Bracket Error \n");
                exit(EXIT_SUCCESS);
				rightk=0;
				leftk=0;
			}
		
				i++;
		}
	return i;
}
// Operators search and check operator accuracy
char operators(char cengLine[],int lastIndex,FILE*lexfile){
	operator_temp='\0';
	char operator[6] = { '+', '-', '*', '/', ':', '=' };
	int count=0; // integer control on the line array
	int temp=0; // for split 
	if(cengLine[lastIndex]=='+'){
		if(cengLine[lastIndex+1]=='+' ){
			pch = strtok (lineIdentifier,",");
			while (pch != NULL){
				char a[100] =","; 
				char b[10] =",";
				strcat(a,pch);
				strcat(a,b);
				if(strstr(intIdentifier,a)){
					count++;
				}
				pch = strtok (NULL, ",");              
				temp++; 
			}
			if(count==temp){
				//printf("Operator(++)\n");
				fprintf(lexfile,"Operator(++)\n");
				lastIndex=lastIndex+2;
                if(strstr(cengLine,":=")){
                    printf("Syntax Error \n");
                    exit(EXIT_SUCCESS);
                }
				return lastIndex;
			}
			else{
				printf("Syntax Error\n");
                lastIndex=lastIndex+2;
                exit(EXIT_SUCCESS);
				return lastIndex;
			}
		}
		else{
			for(int j=0; j<6; j++){
				if(operator[j]==cengLine[lastIndex+1])
					count++;
			}
			if(count==0){
				//printf("Operator(+)\n");
				fprintf(lexfile,"Operator(+)\n");
				lastIndex++;
				operator_temp='+';
				return lastIndex;
			}
			else{   
				printf("Syntax Error\n");
                lastIndex=lastIndex+1;
                exit(EXIT_SUCCESS);
				return lastIndex;
			}
		}      
	}

	if(cengLine[lastIndex]=='-'){
		if(cengLine[lastIndex+1]=='-' && cengLine[lastIndex+2]==';' ){
			//printf("Operator(--)\n");
			fprintf(lexfile,"Operator(--)\n");
			lastIndex=lastIndex+2;
            if(strstr(cengLine,":=")){
                printf("Syntax Error \n");
                exit(EXIT_SUCCESS);
            }
			return lastIndex;
		}
		else {
			for(int j=0; j<6; j++){
				if(operator[j]==cengLine[lastIndex+1])
					count++;
			}
			if(count==0){
				//printf("Operator(-)\n");
				fprintf(lexfile,"Operator(-)\n");
				lastIndex++;
				operator_temp='-';
				return lastIndex;
			}
			else{ 
				printf("Syntax Error\n");
                lastIndex=lastIndex+1;
                exit(EXIT_SUCCESS);
				return lastIndex;
			}
		}      
	}
	if(cengLine[lastIndex]=='*'){
		for(int j=0; j<6; j++){
			if(operator[j]==cengLine[lastIndex+1])
				count++;
		}
		if(count==0){
			//printf("Operator(*)\n");
			fprintf(lexfile,"Operator(*)\n");
			lastIndex++;
			operator_temp='*';
			return lastIndex;
		}
		else{
			printf("Syntax Error\n");
            lastIndex=lastIndex+1;
            exit(EXIT_SUCCESS);
			return lastIndex;
		}
	}
	if(cengLine[lastIndex]=='/'){
		for(int j=0; j<6; j++){
			if(operator[j]==cengLine[lastIndex+1])
				count++;
		}
		if(count==0){
			//printf("Operator(/)\n");
			fprintf(lexfile,"Operator(/)\n");
			lastIndex++;
			operator_temp='/';
			return lastIndex;
		}
		else{   
			printf("Syntax Error\n");
            lastIndex=lastIndex+1;
            exit(EXIT_SUCCESS);
			return lastIndex;
		}
	}
	if(cengLine[lastIndex]==':'){
		if(cengLine[lastIndex+1]=='='){
			//printf("Operator(:=)\n");
			fprintf(lexfile,"Operator(:=)\n");
			lastIndex++;
			return lastIndex+1;
		}
		else{
			printf("Syntax Error\n");
            lastIndex=lastIndex+1;
            exit(EXIT_SUCCESS);
			return lastIndex;
		}
	}
	return lastIndex;
}
// Endofline search and check endofline accuracy
char endofline(char cengLine[],int lastIndex,FILE*lexfile){
	int i=lastIndex; // temp to lastindex
	int count=0;  // integer control on the line array
	int temp=0;   // for split 
	if(cengLine[i]==';'){
		int count_string=0; 
		int d=0;
		int h=0;
		if(operator_temp=='+'){
			pch = strtok (lineIdentifier,",");
			while (pch != NULL){
				char a[100] =",";
				char b[10] =",";
				strcat(a,pch);
				strcat(a,b);
				if(strstr(intIdentifier,a)){
					count++;
				}
				if(strstr(stringIdentifier,a)){
					count_string++;
				}
				pch = strtok (NULL, ",");              
				temp++; 
				d=count;
				h= count_string;
			}
			if(count!=0){
				if(temp==count){
					//printf("EndOfLine\n");
					fprintf(lexfile,"EndOfLine\n");
					return i;
				}
				if(temp!=count){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
			}
			if(count==0){
				if(count_string!= 0 && temp==count_string){
					//printf("EndOfLine\n");
					fprintf(lexfile,"EndOfLine\n");
					return i;
				}
				if(count_string!= 0 && temp!=count_string){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string==0){
					printf("Undefined Variable\n");
                    exit(EXIT_SUCCESS);
				}
			}
		}
		if(operator_temp=='-'){
			pch = strtok (lineIdentifier,",");
			while (pch != NULL){
				char a[100] =",";
				char b[10] =",";
				strcat(a,pch);
				strcat(a,b);
				if(strstr(intIdentifier,a)){
					count++;
				}
				if(strstr(stringIdentifier,a)){
					count_string++;
				}
				pch = strtok (NULL, ",");              
				temp++;  
			}
			if(count!=0){
				if(temp==count){
					//printf("EndOfLine\n");
					fprintf(lexfile,"EndOfLine\n");
					return i;
				}
				if(temp!=count){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
			}
			if(count==0){
				if(count_string!= 0 && temp==count_string){
					printf("Invalid Transaction\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string!= 0 && temp!=count_string){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string==0){
					printf("Undefined Variable\n");
                    exit(EXIT_SUCCESS);
			    }
			}
		}
		if(operator_temp=='/'){
			pch = strtok (lineIdentifier,",");
			while (pch != NULL){
				char a[100] =",";
				char b[10] =",";
				strcat(a,pch);
				strcat(a,b);
				if(strstr(intIdentifier,a)){
					count++;
				}
				if(strstr(stringIdentifier,a)){
					count_string++;
				}
				pch = strtok (NULL, ",");              
				temp++;  
			}
			if(count!=0){
				if(temp==count){
					//printf("EndOfLine\n");
					fprintf(lexfile,"EndOfLine\n");
					return i;
				}
				if(temp!=count){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
			}
			if(count==0){
				if(count_string!= 0 && temp==count_string){
					printf("Invalid Transaction\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string!= 0 && temp!=count_string){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string==0){
					printf("Undefined Variable\n");
                    exit(EXIT_SUCCESS);
				}
			}
		}
		if(operator_temp=='*'){
			pch = strtok (lineIdentifier,",");
			while (pch != NULL){
				char a[100] =",";
				char b[10] =",";
				strcat(a,pch);
				strcat(a,b);
				if(strstr(intIdentifier,a)){
					count++;
				}
				if(strstr(stringIdentifier,a)){
					count_string++;
				}
				pch = strtok (NULL, ",");              
				temp++;  
			}
			if(count!=0){
				if(temp==count){
					//printf("EndOfLine\n");
					fprintf(lexfile,"EndOfLine\n");
					return i;
				}
				if(temp!=count){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
			}
			if(count==0){
				if(count_string!= 0 && temp==count_string){
					printf("Invalid Transaction\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string!= 0 && temp!=count_string){
					printf("Variable Mismatch\n");
                    exit(EXIT_SUCCESS);
				}
				if(count_string==0){
					printf("Undefined Variable\n");
                    exit(EXIT_SUCCESS);
				}
			}
		}
		else{
			//printf("EndOfLine\n");
			fprintf(lexfile,"EndOfLine\n");
			return i;
		}
	}
	return i;
}
// Digit search and check digit accuracy
char digit(char cengLine[],int lastIndex,FILE*lexfile){	
	if (isdigit(cengLine[lastIndex])){
		char digit[11]={0,0,0,0,0,0,0,0,0,0,0};
		int lexLen=0;
		if(cengLine[lastIndex-1]=='-' && cengLine[lastIndex-2]=='(' ){
			printf("Negative values are not supported.\n");
            exit(EXIT_SUCCESS);
		}
		else if(cengLine[lastIndex-1]=='-' && cengLine[lastIndex-2]=='=' ){
			printf("Negative values are not supported.\n");
            exit(EXIT_SUCCESS);
		}
		else{
			while(isdigit(cengLine[lastIndex])){
				digit[lexLen] =cengLine[lastIndex];
				lexLen++;
				lastIndex++;
			}
			lastIndex=space_delete(cengLine,lastIndex);
            if(cengLine[lastIndex]!='+' && cengLine[lastIndex]!='*' && cengLine[lastIndex]!='-' && cengLine[lastIndex]!=')' && cengLine[lastIndex]!=']' && cengLine[lastIndex]!=';' && cengLine[lastIndex]!='/'){
                 printf("Digit Error\n");
                 exit(EXIT_SUCCESS);
            }
			if (lexLen<=10){
				//printf("IntConst(%s)\n",digit);
				fprintf(lexfile,"IntConst(%s)\n",digit);
				return lastIndex;
			}
			else{
				printf("Error - digit too long\n");
                exit(EXIT_SUCCESS);
			}   
		}
	}
	return lastIndex;
}
// Identifier search and check identifier accuracy
char identifier(char cengLine[], int lastIndex,FILE*lexfile){
	int identifierLen;  // variable lenght
	char cengIdentifier[MAX_LEN];  // identifier array
	int cengLineLen; // line lenght 
	int strLen=0;    // variable length 	
	if(!flagFirstHalf&&!flagFirstHalf){
		while(cengLine[lastIndex]==' '){
			lastIndex++;
		}	
		while(lastIndex<strlen(cengLine)){
			if(!flagFirstHalf&&!flagSecondHalf&&(cengLine[lastIndex]==':'&&cengLine[lastIndex+1]=='=')||
					(cengLine[lastIndex]=='+'&&cengLine[lastIndex+1]=='+')||(cengLine[lastIndex]=='-'&&cengLine[lastIndex+1]=='-')
					||(cengLine[lastIndex]=='='&&cengLine[lastIndex+1]=='=')||(cengLine[lastIndex]=='!'&&cengLine[lastIndex+1]=='=')
					||(cengLine[lastIndex]=='[')||(cengLine[lastIndex]==' '||(cengLine[lastIndex]==';'))){
                
				flagFirstHalf=true;
				break;
			}
			else if(!flagFirstHalf&&!flagSecondHalf){
				cengIdentifier[strLen]=cengLine[lastIndex];
				strLen++;
			}     
			lastIndex++;
		}
		cengIdentifier[strLen]='\0';

		if(!flagSecondHalf){
			if(isalpha(cengIdentifier[0])){
				int z=0;
				while(z<strLen){
					if(isalpha(cengIdentifier[z])||cengIdentifier[z]=='_'||isdigit(cengIdentifier[z])){
						z++;
					}
					else{
						break;
					}
				}
				if(z<=20 && z==strLen){
					//printf("Identifier(%s)\n",cengIdentifier);
					fprintf(lexfile,"Identifier(%s)\n",cengIdentifier);
					strcat(lineIdentifier,cengIdentifier);
					strcat(lineIdentifier,",");
					if(strcmp(keyWord,"char")==0){
						strcat(stringIdentifier,cengIdentifier);
						strcat(stringIdentifier,",");
					}
					else if(strcmp(keyWord,"int")==0){
						strcat(intIdentifier,cengIdentifier);
						strcat(intIdentifier,",");
					}
				}
				else if(z<strLen){
					printf("Foreign Character Error\n");
                    exit(EXIT_SUCCESS);
				}
				else if(z>20){
					printf("More Than 20 Characters Error\n");
                    exit(EXIT_SUCCESS);
				}
			}
			else {
				printf("Syntax Error\n");
                exit(EXIT_SUCCESS);
			}
		}
		cengIdentifier[0]='\0';
	}
	else if(flagFirstHalf&&!flagSecondHalf){
		while(lastIndex<strlen(cengLine)){
			if(cengLine[lastIndex]==';'||cengLine[lastIndex]=='+'||cengLine[lastIndex]=='-'||cengLine[lastIndex]=='/'||cengLine[lastIndex]=='*'||
					cengLine[lastIndex]==')'||cengLine[lastIndex]=='['||cengLine[lastIndex]==']'||cengLine[lastIndex]==' '){
				
				break;
			}
			else if(!flagSecondHalf){
				cengIdentifier[strLen]=cengLine[lastIndex];
				strLen++;
			}  
			lastIndex++;
		}
		cengIdentifier[strLen]='\0';

		if(flagFirstHalf&&!flagSecondHalf){
			if(isalpha(cengIdentifier[0])){
				int z=0;
				while(z<strLen){
					if(isalpha(cengIdentifier[z])||cengIdentifier[z]=='_'||isdigit(cengIdentifier[z])){
						z++;
					}
					else{
						break;
					}
				}
				if(z<=20 && z==strLen){
					//printf("Identifier(%s)\n",cengIdentifier);
					fprintf(lexfile,"Identifier(%s)\n",cengIdentifier);
					strcat(lineIdentifier,cengIdentifier);
					strcat(lineIdentifier,",");
				}
				else if(z<strLen){
					printf("Foreign Character Error\n");
                    exit(EXIT_SUCCESS);
				}
				else if(z>20){
					printf("More Than 20 Characters Error\n");
                    exit(EXIT_SUCCESS);
				}
			}
			else {
				printf("Syntax Error\n");
                exit(EXIT_SUCCESS);
			}
		}
		cengIdentifier[0]='\0';
	}
	else if(flagFirstHalf&&flagSecondHalf){
		while(lastIndex<strlen(cengLine)){
			if(cengLine[lastIndex]==';'||cengLine[lastIndex]=='+'||cengLine[lastIndex]=='-'||cengLine[lastIndex]=='/'||cengLine[lastIndex]=='*'||
					cengLine[lastIndex]==')'||cengLine[lastIndex+1]==' '){
               
				cengIdentifier[strLen]=cengLine[lastIndex];
				strLen++;
			} 
			else{
				break;
			} 
			lastIndex++;
		}
		cengIdentifier[strLen]='\0';

		if(flagFirstHalf&&flagSecondHalf){
			if(isalpha(cengIdentifier[0])){
				int z=0;
				while(z<strLen){
					if(isalpha(cengIdentifier[z])||cengIdentifier[z]=='_'||isdigit(cengIdentifier[z])){
						z++;
					}
					else{
						break;
					}
				}
				if(z<=20 && z==strLen){
					//printf("Identifier(%s)\n",cengIdentifier);
					fprintf(lexfile,"Identifier(%s)\n",cengIdentifier);
					strcat(lineIdentifier,cengIdentifier);
					strcat(lineIdentifier,",");
				}
				else if(z<strLen){
					printf("Foreign Character Error\n");
                    exit(EXIT_SUCCESS);
				}
				else if(z>20){
					printf("More Than 20 Characters Error\n");
                    exit(EXIT_SUCCESS);
				}
			}
			else {
				printf("Syntax Error\n");
                exit(EXIT_SUCCESS);
			}
		}
	}
	return lastIndex;
}
// Keyword search and check keyword accuracy
char keywords(char cengLine[], int lastIndex,FILE* lexfile){
	char cengIdentifier[MAX_LEN];  // temp to keyword 
	int i=0; // temp to keyword length
	int lastIndexTemp; // temp to lastindex
	while(!isalpha(cengLine[lastIndex])&&cengLine[lastIndex]!='{'&&cengLine[lastIndex]!='}'){
		lastIndex++;
	}
	lastIndexTemp=lastIndex;
	while(cengLine[lastIndex]!='('&&cengLine[lastIndex]!='{'&&cengLine[lastIndex]!=';'&&cengLine[lastIndex]!=' '){
		cengIdentifier[i]=tolower(cengLine[lastIndex]);
		i++;
		lastIndex++;
	}
	cengIdentifier[i]='\0';

	if(!strcmp(cengIdentifier,"break")){
		//printf("Keyword(break)");
		fprintf(lexfile,"Keyword(break)\n");
	}
	else if(!strcmp(cengIdentifier,"case")){
		//printf("Keyword(case)");
		fprintf(lexfile,"Keyword(case)\n");
	}
	else if(!strcmp(cengIdentifier,"char")){
		//printf("Keyword(char)\n");
		fprintf(lexfile,"Keyword(char)\n");
		strcpy(keyWord,"char");
	}
	else if(!strcmp(cengIdentifier,"const")){
		//printf("Keyword(const)\n");
		fprintf(lexfile,"Keyword(const)\n");
	}
	else if(!strcmp(cengIdentifier,"continue")){
		//printf("Keyword(continue)\n");
		fprintf(lexfile,"Keyword(continue)\n");
	}
	else if(!strcmp(cengIdentifier,"do")){
		//printf("Keyword(do)\n");
		fprintf(lexfile,"Keyword(do)\n");
	}
	else if(!strcmp(cengIdentifier,"else")){
		//printf("Keyword(else)\n");
		fprintf(lexfile,"Keyword(else)\n");
	}
	else if(!strcmp(cengIdentifier,"enum")){
		//printf("Keyword(enum)\n");
		fprintf(lexfile,"Keyword(enum)\n");
	}
	else if(!strcmp(cengIdentifier,"float")){
		//printf("Keyword(float)\n");
		fprintf(lexfile,"Keyword(float)\n");
		strcpy(keyWord,"float");
	}
	else if(!strcmp(cengIdentifier,"for")){
		//printf("Keyword(for)\n");
		fprintf(lexfile,"Keyword(for)\n");
	}
	else if(!strcmp(cengIdentifier,"goto")){
		//printf("Keyword(goto)\n");
		fprintf(lexfile,"Keyword(goto)\n");
	}
	else if(!strcmp(cengIdentifier,"if")){
		//printf("Keyword(if)\n");
		fprintf(lexfile,"Keyword(if)\n");
	}
	else if(!strcmp(cengIdentifier,"int")){
		//printf("Keyword(int)\n");
		fprintf(lexfile,"Keyword(int)\n");
		strcpy(keyWord,"int");
	}
	else if(!strcmp(cengIdentifier,"long")){
		//printf("Keyword(long)\n");
		fprintf(lexfile,"Keyword(long)\n");
		strcpy(keyWord,"long");
	}
	else if(!strcmp(cengIdentifier,"record")){
		//printf("Keyword(record)\n");
		fprintf(lexfile,"Keyword(record)\n");
	}
	else if(!strcmp(cengIdentifier,"return")){
		//printf("Keyword(return)\n");
		fprintf(lexfile,"Keyword(return)\n");
	}
	else if(!strcmp(cengIdentifier,"static")){
		//printf("Keyword(static)\n");
		fprintf(lexfile,"Keyword(static)\n");
	}
	else if(!strcmp(cengIdentifier,"while")){
		//printf("Keyword(while)\n");
		fprintf(lexfile, "Keyword(while)\n");
	}
	else{
		return lastIndexTemp;
	}
	return lastIndex;
}