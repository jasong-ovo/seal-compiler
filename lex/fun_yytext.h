#include <iostream>  
#include <string>
using namespace std;

char* trim(char *x){
char val=*x;
while (val==' '){
  x++;
  val=*x;
}
return x;

}
int tanslate_alphabeta_num(char x){
  switch (x){
  case 'a' : return 10;break;
  case 'b' : return 11;break;
  case 'c' : return 12;break;
  case 'd' : return 13;break;
  case 'e' : return 14;break;
  case 'f' : return 15;break;
  case 'A' : return 10;break;
  case 'B' : return 11;break;
  case 'C' : return 12;break;
  case 'D' : return 13;break;
  case 'E' : return 14;break;
  case 'F' : return 15;break;
  case 0   : return 0;break ;
  case 1   : return 1;break ;
  case 2   : return 2;break ;
  case 3   : return 3;break ;
  case 4   : return 4;break ;
  case 5   : return 5;break ;
  case 6   : return 6;break ;
  case 7   : return 7;break ;
  case 8   : return 8;break ;
  case 9   : return 9;break ;
  }
return 0;
}

char* HEX_to_DEX(char *a ,int leng){
  a=a+2;
  int sum=0;
  for (int i=2;i<leng;i++){
    sum=tanslate_alphabeta_num(*a)+sum*16;
  }
  char *b=new char;
  sprintf(b,"%d",sum);
  return b;
}

int str1_add_lineno(char* a,int leng){
  int delta=0;
  for (int i=0;i<leng;i++){
    if (a[i+1]=='\0'){
      break;
    }
    if (a[i]=='\\'){
      if(a[i+1]=='\n'){
        delta++;
      }
    }
  }
  return delta;
}

int str2_add_lineno(char* a,int leng){
  int delta=0;
  for (int i=0;i<leng;i++){
    if (a[i+1]=='\0'){
      break;
    }
 
      if(a[i]=='\n'){
        delta++;
      }
}
 return delta;
}


void str1_trim(char*a,int leng){
  char *b=new char;
  
  int j=0;//pointer b
  bool flag=false;//be escaped?
  for (int i=0;i<leng;i++){
    if (a[i]=='\0'){
      break;
    }
    b[j]=a[i];
    j++;
    if (!flag) {
      if (a[i]=='\\') {
        j--;
        flag=true;
        }
    }
    else {if(a[i]=='0'&&flag==true){b[1]='\0';break;}
                if(a[i]=='\"'&&flag==true){b[j-1]='\"';}
                if(a[i]=='n'&&flag==true){b[j-1]='\n';}
                if(a[i]=='b'&&flag==true){b[j-1]='\t';}
                if(a[i]=='t'&&flag==true){b[j-1]='\b';}
                if(a[i]=='f'&&flag==true){b[j-1]='\f';}
                    flag=false;}
  }
  int i=0;
  for ( i=0;i<257;i++){
    a[i]=b[i+1]; //overlook \" in head
    if(b[i+1]=='\0') break; 
  }
a[i-1]='\0';//overlook \" in tail
}
