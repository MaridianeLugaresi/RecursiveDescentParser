#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// Lista de tokens reconhecidos e seus c�digos num�ricos

#define TKId 1
#define TKVoid 2
#define TKInt 3
#define TKFloat 4
#define TKVirgula 5
#define TKDoisPontos 6
#define TKAbrePar 7
#define TKFechaPar 8
#define TKAtrib 9
#define TKPontoeVirg 10
#define TKAbreChaves 11
#define TKFechaChaves 12
#define TKSoma 13
#define TKDuploMais 14
#define TKProd 15
#define TKAbreColch 16
#define TKFechaColch 17
#define TKString 18
#define TK_Fim_Arquivo 19

struct {char id[20]; int t;} TabSimb[20];
int tamTS=0;

int buscaTS(char id[])
{
    int i;
    for(i=0;i<tamTS;i++){
        if(strcmp(id,TabSimb[i].id)==0){
            printf("Variavel %s ja reconhecida com tipo %d\n", TabSimb[i].id, TabSimb[i].t);
            return 1;
        }
    }
    return 0;
}

void insereTS(char id[], int tipo)
{
    if(buscaTS(id)){
        return;
    }
    strcpy(TabSimb[tamTS].id,id);
    TabSimb[tamTS].t=tipo;
    tamTS++;
}

/***********************************************************************************/
/*                                                                                 */
/*  IN�CIO DO L�XICO - N�o entre a n�o ser que tenha interesse pessoal em l�xicos  */
/*                                                                                 */
/***********************************************************************************/

int linlex=0,collex=1;

// lista de strings correspondentes a cada token para colocar em mensagens de depura��o. Deve
// estar na mesma ordem da lista de defines

char tokens[][20]={"","TK_id",
					  "TK_void",
					  "TK_int",
					  "TK_float",
					  "TK_virgula",
					  "TK_dois_pontos",
					  "TK_Abre_Par",
					  "TK_Fecha_Par",
					  "TK_Atrib",
					  "TK_Ponto_e_virg",
					  "TK_Abre_Chaves",
					  "TK_Fecha_Chaves",
					  "TK_Soma",
					  "TK_Duplo_Mais",
					  "TK_Prod",
					  "TK_Abre_Colch",
					  "TK_Fecha_Colch",
					  "TK_String",
					  "TK_Fim_Arquivo"
					  };

FILE *arqin;
int token;
char lex[200];

struct pal_res{char palavra[20]; int tk;};
struct pal_res lista_pal[]={{"void",TKVoid},
                  {"int",TKInt},
                  {"float",TKFloat},
                  {"fimtabela",TKId}};

int palavra_reservada(char lex[])
{
int postab=0;
while (strcmp("fimtabela",lista_pal[postab].palavra)!=0)
   {
   if (strcmp(lex,lista_pal[postab].palavra)==0)
      return lista_pal[postab].tk;
   postab++;
   }
return TKId;
}

char le_char()
{
unsigned char c;

if (fread(&c,1,1,arqin)==0) return -1;
if (c=='\n') {linlex++;collex=1;}
else collex++;
return c;
};

int le_token()
{
static int pos=0;
int estado=0;
static char c='\0';
while(1)
{
switch (estado)
{
case 0:if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_')
		  {
          lex[0]=c;
          pos=1;
          c=le_char();
		  estado=1;
		  break;
		  }
       if (c==',') {c=le_char();return TKVirgula;}
       if (c==';') {c=le_char();return TKPontoeVirg;}
       if (c=='{') {c=le_char();return TKAbreChaves;}
       if (c=='}') {c=le_char();return TKFechaChaves;}
       if (c=='(') {c=le_char();return TKAbrePar;}
       if (c==')') {c=le_char();return TKFechaPar;}
       if (c==':') {c=le_char();return TKDoisPontos;}
       if (c=='[') {c=le_char();return TKAbreColch;}
       if (c==']') {c=le_char();return TKFechaColch;}
       if (c=='"')
          {
          lex[0]=c;
          c=le_char();
		  pos=1;
		  estado=3;
		  break;
		  }
       if (c>='0' && c<='9')
          {
          lex[0]=c;
          c=le_char();
		  pos=1;
		  estado=2;
		  break;
		  }
	   if (c==-1) return TK_Fim_Arquivo;
	   if (c=='\n'||c=='\r'||c=='\t'||c=='\0'||c==' ')
	      {
		  c=le_char();
		  break;
		  }
case 1:if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9')
          {
		  lex[pos++]=c;
          c=le_char();
		  break;
		  }
        lex[pos]='\0';
        return palavra_reservada(lex);
case 2:if (c>='0' && c<='9')
          {
          lex[pos++]=c;
          c=le_char();
		  break;
		  }
	   else
	      {
		  estado=0;
		  lex[pos]='\0';
		  return TKInt;
		  }
case 3:if (c>='a' && c<='z' || c=='_' || c>='0' && c<='9')
          {
          lex[pos++]=c;
          c=le_char();
		  break;
		  }
	   else
	      {
		  estado=0;
		  lex[pos]='\0';
		  return TKString;
		  }
}
}
}

/********************/
/*                  */
/*  FIM DO L�XICO   */
/*                  */
/********************/

#define MAX_COD 10000

// L -> id {poetabsimb(id,Ltipo);L1tipo=Ltipo} , L1
// L -> id {poetabsimb(id,Ltipo);}

int L(int LTipo)
{
int L1Tipo;
if (token==TKId)
   {
   insereTS(lex,LTipo);
   printf("Reconheci a vari�vel %s como %d\n",lex,LTipo);
   token=le_token();
   if (token==TKVirgula)
      {
      token=le_token();
      L1Tipo=LTipo;
      return L(L1Tipo);
	  }
   else
      if (token==TKPontoeVirg)
      {
      token=le_token();
      return 1;
	  }
	  else return 0;
return 1;
   }
return 0;
}

// T -> int {T.tipo=int}
// T -> float {T.tipo=float}

int T(int *TTipo)
{
if (token==TKInt || token==TKFloat)
   {
   *TTipo=token;
   printf("Reconheci tipo %s\n",tokens[token]);
   token=le_token();
   return 1;
   }return 0;
}

//D -> T {L.tipo=T.tipo} L

int D( )
{
	int LTipo,TTipo;
	if (!T(&TTipo)) return 0;
	LTipo=TTipo;
	if (!L(LTipo)) return 0;
	return 1;
}

int Ldec()
{
if (token!=TKInt && token!=TKFloat) return 1;
if (D())
   if (Ldec())
   		return 1;
return 0;
}


int main()
{
char c;
if ((arqin=fopen("C:\\Projetos\\RecursiveDescentParser\\declaracoes.c","rt"))==NULL)
   {printf("Erro na abertura do arquivo");exit(0);}
token=le_token();
if (Ldec()) printf("Reconheceu tudo\n");
else printf("Erro no reconhecimento\n");
fclose(arqin);
system("pause");
}