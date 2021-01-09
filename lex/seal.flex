 /*
  *  The scanner definition for seal.
  */

 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{

#include <seal-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdint.h>
#include <stdlib.h>
#include "fun_yytext.h"

/* The compiler assumes these identifiers. */
#define yylval seal_yylval
#define yylex  seal_yylex

/* Max size of string constants */
#define MAX_STR_CONST 256
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */
//extern  char*HEX_to_DEX(char*,int);
//extern char* str1_add_lineno(char*,int);
//extern char* str1_trim(char*,int);
/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the seal compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE seal_yylval;
bool flag_is_id_in_func=false;
bool flag_is_in_funcbody=false;
int delta_lines=0;
/*
 *  Add Your own definitions here
 */


%}
%option noyywrap
 /*
  * Define names for regular expressions here.
  */
string_num [_A-Za-z0-9]
identifiers  [a-z]{string_num}*
type_identifiers  "Int"|"Float"|"String"|"Bool"|"Void"
id_and_type {identifiers}[ ]*{type_identifiers} 
nonnatural_newline "\\""n"
 //x Float
HEX  "0x"|"0X"
NATURAL_NUM [1-9][0-9]*|0
INTDEX {NATURAL_NUM}
INTHEX {HEX}[0-9a-fA-F]+
FLOAT {NATURAL_NUM}"."{NATURAL_NUM}
STRING1_singleline  "\"".*"\""
STRING1_multiline   "\""(.*"\\""\n")+.*"\""
STRING2 "`"(.|"\n")*"`"
BOOL  ("true")|("false")
annotation_singleline "//".*|"/*".*"/*"
annotation_multiline "/*"(.|"\n")*"*/"

%x lock_str1
%x lock_str2
%x overlook
%s new_token
%s str_style1
%%
[ ]*
"\t"*
"\"" {char *e=(char *)"new line need \\";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);}
{annotation_singleline}
{annotation_multiline} {
  BEGIN(overlook);
  yyless(1);
} 
<lock_str1>.|"\n" {BEGIN(0);delta_lines=0;}
<lock_str2>.|"\n" {BEGIN(0);delta_lines=0;}
<overlook>"*/" {
  curr_lineno+=delta_lines;
  delta_lines=0;
  BEGIN(0);
}

<overlook>(.|"\n")*/"*/" {
  int i=0;
  
  while(i<yyleng){

  if (yytext[i]=='\n') delta_lines++;
       i++;
  }
}
"/""*" {char *e=(char *)"/* not match";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);}
"*""/" {char *e=(char *)"*/ not match";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);}
"+" {return(*yytext);}
"-" {return(*yytext);}
"*" {return(*yytext);}
"/" {return(*yytext);}
"%" {return(*yytext);}
"&" {return(*yytext);}
"|" {return(*yytext);}
"^" {return(*yytext);}
"~" {return(*yytext);}
">" {return(*yytext);}
">="  {return(GE);}
"==" {return(EQUAL);}
"<=" {return(LE);}
"!=" {return(NE);}
"<" {return(*yytext);}
"=" {return(*yytext);}
"&&" {return(*yytext);}
"||" {return(*yytext);}
"!" {return(*yytext);}


":" {return(*yytext);}
"\n" {curr_lineno++;}
";"   {return (*yytext);}
","   {return(*yytext);}
"}" {return(*yytext);}
"{"  {
  return(*yytext);
}
")"  {return(*yytext);}
"("  {
  return (*yytext);}



"for" {return(FOR);}
"while" {return(WHILE);}
"if" {return(IF);}
"else" {return(ELSE);}
"break" {return(BREAK);}
"continue" {return(CONTINUE);}
"struct" {return(STRUCT);}
"var" {return(VAR);}
{type_identifiers} {
  seal_yylval.symbol=idtable.add_string(yytext);
  return(TYPEID);
}
 "func" {return (FUNC);}
"return" {
  return(RETURN);
}



 /**/

{FLOAT} {
  yylval.symbol=floattable.add_string(yytext);
  return(CONST_FLOAT);
}
{INTHEX} {
  char* a=(char*)yytext;
  char* b ;
  b=HEX_to_DEX(a,yyleng);
  yylval.symbol=inttable.add_string(b);
  return(CONST_INT);
}
{INTDEX} {
  yylval.symbol=inttable.add_string(yytext);
  return(CONST_INT);
}


"true" {
  seal_yylval.boolean=1;
  return(CONST_BOOL);
}

"false" {
  seal_yylval.boolean=0;
  return(CONST_BOOL);
}

{STRING1_singleline} {
  if (yyleng>256){
    char *e=(char *)"string should less than 256";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);
  }


  str1_trim(yytext,yyleng);
  
   if(*yytext=='\0')  {
    char *e=(char *)"String contains null character '\\0'";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);}
  yylval.symbol=stringtable.add_string(yytext);
  return(CONST_STRING);
 }



{STRING1_multiline} {
   if (yyleng>256){
    char *e=(char *)"string should less than 256";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);
  }
  int i=0;
  while (i<yyleng){
    if((yytext[i]=='\n')&&yytext[i-1]!='\\')  {
      char *e=(char *)"new line need \\";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);
    }
    i++;
  }
  BEGIN(lock_str1);

  delta_lines= str1_add_lineno(yytext,yyleng);
  curr_lineno+=delta_lines;
  str1_trim(yytext,yyleng);
   if(*yytext=='\0')  {
    char *e=(char *)"String contains null character '\\0'";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);}
  yylval.symbol=stringtable.add_string(yytext);
  yyless(yyleng-1);
  return(CONST_STRING);
}
{STRING2} {
   if (yyleng>256){
    char *e=(char *)"string should less than 256";
    strcpy(seal_yylval.error_msg, e); 
    return(ERROR);
  }
  BEGIN(lock_str2);
  delta_lines= str2_add_lineno(yytext,yyleng);
  curr_lineno+=delta_lines;
  yytext=yytext+1;
  yytext[yyleng-2]='\0'; //delete  ``in head and tail
  yylval.symbol=stringtable.add_string(yytext);
  yyless(yyleng-1);
  return(CONST_STRING);
}
 /**/

 /*	
//  *Add Rules here. Error function has been given.
//  */
{identifiers} {
seal_yylval.symbol=idtable.add_string(yytext);
return(OBJECTID);
}
.	{
	strcpy(seal_yylval.error_msg, yytext); 
	return (ERROR); 
}

%%

