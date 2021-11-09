//Thiago Dos Santos
//Thiago Bulhosa
//Marina Bulhosa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct Ocorrencia
{
    int arquivo; //posição do arquivo na lista de arquivos processados.
    int qtdOcorrencias; //qtd de vezes em que a palavra foi encontrada
    int *linhas; //linhas onde a palavra foi encontrada
    struct Ocorrencia *prox; //ponteiro para a próxima ocorrência da palavra em outro arquivo
}Ocorrencia;

typedef struct Palavra
{
    char letras[50]; //a palavra em questão
    int qtdOcorrencias; //quantidade de arquivos que possuem a palavra
    struct Ocorrencia *ocorrencias; //lista contendo informações da ocorrência de uma palavra em um arquivo
    struct Palavra *prox;//ponteiro para a próxima palavra encontrada em qualquer arquivo (em
    //ordem alfabética)
}Palavra;

typedef struct Arquivo
{
    char nomeArquivo[50]; //nome de um arquivo texto já processado
    struct Arquivo *prox; //ponteiro para o nome do próximo arquivo texto
}Arquivo;

typedef struct Indice
{
    int qtdArquivos; //quantidade de arquivos de texto lidos até então
    struct Arquivo *arquivos; //lista contendo os nomes dos arquivos texto já processados
    int qtdPalavras;
    struct Palavra *palavras;//lista contendo todas as palavras já encontrada
}Indice;

Indice *indice=NULL;

int verificaIndice(char *nomeArq)
{
  Indice *aux=indice;
  Arquivo *arq=aux->arquivos;
  if (aux->qtdArquivos==0)
    return 0;

  else
  {
    while(arq!=NULL)
    {
      if(strcmp(arq->nomeArquivo,nomeArq)==0)
        return 1;
      arq= arq->prox;
    }
  }
  return 0;
}

void insereArquivo(char *nomeArq)
{
  Arquivo *arq=indice->arquivos;
  Arquivo *novo=(Arquivo *)malloc(sizeof(Arquivo));
  strcpy(novo->nomeArquivo,nomeArq);
  novo->prox=NULL;

  if (arq==NULL)
  {
      indice->arquivos=novo;
  }

  else
  {
      while (arq->prox!=NULL)
      {
          arq=arq->prox;

      }
      arq->prox=novo;
  }
}

void printa()
{
    Arquivo *auxArq = indice->arquivos;
    Palavra *auxPalavra = indice->palavras;
    Ocorrencia *auxOcorrencia;

    while (auxArq!=NULL){
        printf ("Nome: %s\n", auxArq->nomeArquivo);
        auxArq=auxArq->prox;
    }

    while (auxPalavra!=NULL)
    {
        printf("Palavra:%s\t",auxPalavra->letras);
        printf("Arquivos contendo a palavra:%d  ",auxPalavra->qtdOcorrencias);
        auxOcorrencia = auxPalavra->ocorrencias;

        while(auxOcorrencia!=NULL)
        {
          printf ("NoOcorrencias:%d ",auxOcorrencia->qtdOcorrencias);
          printf("Pos Arq:%d  ",auxOcorrencia->arquivo);
          printf("Linhas:");
          for(int i=0; i<auxOcorrencia->qtdOcorrencias; i++)
          {
              printf("%d ",(auxOcorrencia->linhas[i]));
          }
          auxOcorrencia=auxOcorrencia->prox;
        }

        printf("\n");
        auxPalavra=auxPalavra->prox;
    }

    printf("Palavras totais:%d \n\n",indice->qtdPalavras);
}

int verificaPalavra(char *palavra)
{
    Palavra *auxiliar = indice->palavras;
    if (indice->qtdPalavras==0)
        return 0;
    else
    {
        while(auxiliar!=NULL)
        {
            if(strcmp(auxiliar->letras,palavra)==0){
                return 1;
            }
            auxiliar = auxiliar->prox;
        }
    }
    return 0;
}

void inserePalavraOrdenada(Palavra *NovaPalavra)
{

    Palavra *aux;

    if (indice->palavras==NULL)
    {
      NovaPalavra->prox=NULL;
      indice->palavras=NovaPalavra;
    }

    else if(strcmp(NovaPalavra->letras,indice->palavras->letras)<0)
    {
        NovaPalavra->prox = indice->palavras;
        indice->palavras = NovaPalavra;
    }

    else
    {
        aux = indice->palavras;
        while(aux->prox && (strcmp(NovaPalavra->letras,aux->prox->letras)>0))
            aux= aux->prox;
        NovaPalavra->prox = aux->prox;
        aux->prox = NovaPalavra;
    }
}

void addPalavra(char *palavra,int Nlinha,int posArq)
{
  Palavra *NovaPalavra;
  NovaPalavra=(Palavra *)malloc(sizeof(Palavra));
  strcpy(NovaPalavra->letras, palavra);
  NovaPalavra->qtdOcorrencias = 0;
  NovaPalavra->prox = NULL;
  inserePalavraOrdenada(NovaPalavra);
}

void insereOcorrencia(char *palavra,int Nlinha,int posArq)
{
    int flag=0;
    Palavra *auxPalavras=indice->palavras;
    Ocorrencia *aux2;

    while (auxPalavras!=NULL)
    {
      if (strcmp(auxPalavras->letras,palavra)==0)
      {

        aux2=auxPalavras->ocorrencias;
        if (aux2==NULL)
        {
          flag = 1;
          auxPalavras->qtdOcorrencias++;
          Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
          nova->qtdOcorrencias=1;
          nova->arquivo=posArq;
          nova->linhas=(int *)malloc(sizeof(int));
          nova->linhas[0]=Nlinha;
          nova->prox=NULL;
          auxPalavras->ocorrencias=nova;
        }

        else
        {
          while(aux2!=NULL)
          {
            if(aux2->arquivo==posArq)
            {
              flag = 1;
              aux2->qtdOcorrencias++;
              aux2->linhas=(int *)realloc(aux2->linhas,aux2->qtdOcorrencias*sizeof(int));
              aux2->linhas[aux2->qtdOcorrencias-1]=Nlinha;
            }
            aux2=aux2->prox;
          }


          if(flag==0)
          {
            auxPalavras->qtdOcorrencias++;
            Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
            nova->qtdOcorrencias=1;
            nova->arquivo=posArq;
            nova->linhas=(int *)malloc(sizeof(int));
            nova->linhas[0]=Nlinha;
            nova->prox=NULL;

            aux2=auxPalavras->ocorrencias;
            while(aux2->prox!=NULL){
              aux2=aux2->prox;
            }
            aux2->prox=nova;
          }



        }
      }
      auxPalavras=auxPalavras->prox;
    }
}

int procuraArq(char *nomeArq)
{
  Arquivo *aux=indice->arquivos;
  int cont=0;

  while (aux!=NULL)
  {
    cont++;
    if (strcmp(aux->nomeArquivo,nomeArq)==0)
      return cont;

    aux=aux->prox;
  }
  return -1;
}

void leArquivo (int posArq,FILE *arq)
{
    int compara=0,cont=1,procura=-1;
    char palavraArq[1000],*token;
    while (fgets(palavraArq, 1000,arq)!=NULL)
    {
        token=strtok(palavraArq," ,.!\n\0");
        while (token!=NULL)
        {
            for(int i = 0; i<strlen(token); i++)
            {
                token[i] = tolower(token[i]);
            }
            compara=verificaPalavra(token);
            if (compara==0)
            {
                addPalavra(token,cont,posArq);
                insereOcorrencia(token,cont,posArq);
                indice->qtdPalavras++;
            }
            else
              insereOcorrencia(token,cont,posArq);

            token=strtok(NULL," ,.!\n\0");
        }
        cont++;
    }
    fclose(arq);
}

void recuperaIndice()
{
    int tamanhoTitulo=0;
    int tamanhoPalavra=0;
    FILE *dat;
    dat=fopen("indice.dat","rb");
    if (dat==NULL)
        printf("Erro!Nao é possivel abrir o arquivo :(\n");
    else
    {
        fread(&indice->qtdArquivos,sizeof(int),1,dat); //qtd arquivos processados
        printf("Arquivos processados: %d\n",indice->qtdArquivos);


        for (int i=0; i<indice->qtdArquivos; i++){
            indice->arquivos=(Arquivo*) malloc(sizeof(Arquivo));
            fread(&tamanhoTitulo,sizeof(int),1,dat); //le a quantidade de letras do arquivo
            printf("Tamanho do título do arquivo: %d\n", tamanhoTitulo);
            fread(&indice->arquivos->nomeArquivo,tamanhoTitulo*sizeof(char),1,dat); //le o nome do arquivo
            printf("Nome do arquivo: %s\n", indice->arquivos->nomeArquivo);
            indice->arquivos=indice->arquivos->prox;
        }
    fread(&indice->qtdPalavras,sizeof(int),1,dat); //quantidade de palavras diferentes encontradas
    printf("Qtd palavras diferentes: %d\n", indice->qtdPalavras);

    for (int i = 0; i < indice->qtdPalavras; i++){
        indice->palavras=(Palavra*) malloc(sizeof(Palavra));
        fread (&tamanhoPalavra,sizeof(int),1,dat); //tamanho da palavra
        printf ("Tamanho palavra: %d\n", tamanhoPalavra);
        fread (&indice->palavras->letras, tamanhoPalavra*sizeof(char),1,dat);//pega a palavra em si
        printf ("Palavra: %s\n", indice->palavras->letras);
        fread (&indice->palavras->qtdOcorrencias, sizeof(int),1,dat); //qtd ocorrencias da palavra ao longo dos arquivos
        printf ("Quantidade de ocorrencias: %d\n", indice->palavras->qtdOcorrencias);


        for (int j=0; j<indice->palavras->qtdOcorrencias; j++){
            indice->palavras->ocorrencias=(Ocorrencia*) malloc(sizeof(Ocorrencia)); //malloc para cada arquivo que encontrou a palavra
            fread (&indice->palavras->ocorrencias->arquivo, sizeof(int),1,dat);//numero do arquivo
            printf ("Numero do arquivo na fila: %d\n", indice->palavras->ocorrencias->arquivo);
            fread (&indice->palavras->ocorrencias->qtdOcorrencias, sizeof(int),1,dat);//quantidade ocorrencias
            printf ("Quantidade ocorrencias da palavra: %d\n", indice->palavras->ocorrencias->qtdOcorrencias);
                for (int k=0; k<indice->palavras->ocorrencias->qtdOcorrencias; k++){
                    indice->palavras->ocorrencias->linhas = (int*) malloc(sizeof(int));
                    fread (&indice->palavras->ocorrencias->linhas[i], sizeof(int),1,dat); //Pega o total de linhas
                    printf ("Linhas: %d\n", indice->palavras->ocorrencias->linhas[i]);
                }
        }
        indice->palavras=indice->palavras->prox;
    }
        fclose(dat);
    }
}

void escreveIndice(){

    FILE *dat;
    dat=fopen("indice.dat","w");

    int tamanhoTitulo = 0, tamanhoPalavras = 0;

    fwrite(&indice->qtdArquivos,sizeof(int),1,dat); //letra b

    Arquivo *auxTitulo=indice->arquivos;
    Palavra *auxPalavra=indice->palavras;
    Ocorrencia *auxOcorrencia;

    while (auxTitulo!=NULL){//letra c
        tamanhoTitulo = strlen(auxTitulo->nomeArquivo)+1;
        fwrite(&tamanhoTitulo,sizeof(int),1,dat); //qtd letras titulo
        fwrite(&auxTitulo->nomeArquivo,tamanhoTitulo*sizeof(char),1,dat); //nome do arquivo em questao
        auxTitulo=auxTitulo->prox;
    }

    fwrite(&indice->qtdPalavras,sizeof(int),1,dat); //letra d

     while (auxPalavra!=NULL){ //letra e
        tamanhoPalavras = strlen(auxPalavra->letras)+1;
        fwrite(&tamanhoPalavras,sizeof(int),1,dat);//qtd letras da palavra
        fwrite(&auxPalavra->letras,tamanhoPalavras*sizeof(char),1,dat);//letras da palavra
        fwrite(&auxPalavra->qtdOcorrencias,sizeof(int),1,dat);//qtd ocorrencias
        auxOcorrencia = auxPalavra->ocorrencias;
        while (auxOcorrencia!=NULL){ //letra f
            fwrite(&auxOcorrencia->arquivo,sizeof(int),1,dat); //numero do arquivo da ocorrencia
            fwrite(&auxOcorrencia->qtdOcorrencias,sizeof(int),1,dat); //quantidade de vezes que a palavra ocorreu
            for(int i=0; i<auxOcorrencia->qtdOcorrencias; i++){  //numero das linhas das palavras
                fwrite(&auxOcorrencia->linhas[i],sizeof(int),1,dat);
            }
        auxOcorrencia=auxOcorrencia->prox;
        }
        auxPalavra=auxPalavra->prox;
     }
    fclose(dat);
}

void limpaIndice(){

    Palavra *auxPalavra;
    Arquivo *auxArquivo;
    Ocorrencia *auxOcorrencia;

    while(indice->arquivos !=NULL)
    {
        auxArquivo=indice->arquivos->prox;
        free(indice->arquivos);
        indice->arquivos=auxArquivo;
    }
    while(indice->palavras !=NULL)
    {
        auxPalavra=indice->palavras->prox;
        while (indice->palavras->ocorrencias){
            auxOcorrencia=indice->palavras->ocorrencias->prox;
            free (indice->palavras->ocorrencias->linhas);
            free(indice->palavras->ocorrencias);
            indice->palavras->ocorrencias=auxOcorrencia;
        }
        free(indice->palavras);
        indice->palavras=auxPalavra;
    }
}

int main()
{
    indice=(Indice *)malloc(sizeof(Indice));
    indice->qtdArquivos=0;
    indice->qtdPalavras=0;
    indice->arquivos=NULL;
    indice->palavras=NULL;
    setlocale(LC_ALL, "Portuguese");
    int op=0,procura=0;
    char nomeArq[100];
    while (op!=5)
    {
        printf("\n1-Processar arquivo\n");
        printf("2-Salvar índice atual\n");
        printf("3-Ler índice\n");
        printf("4-Exibir índice atual\n");
        printf("5-Encerrar programa\n");
        printf("Digite uma opcao:");
        scanf("%d",&op);
        switch (op)
        {
        case 1:
            printf("Digite o nome do arquivo:");
            scanf("%s",nomeArq);
            FILE *arq;
            arq=fopen(nomeArq,"r");
            if (arq==NULL)
            {
                printf("Digite um arquivo valido");
            }
            else
            {
              if (verificaIndice(nomeArq)==0)
              {
                insereArquivo(nomeArq);
                indice->qtdArquivos++;
                leArquivo(indice->qtdArquivos-1,arq);
              }

              else
                printf("O arquivo ja foi lido\n");
            }
            break;

        case 2:
            escreveIndice();
            break;

        case 3:
            limpaIndice();
            recuperaIndice();

            break;

        case 4:
            printa();
            break;

        case 5:
            limpaIndice();
            break;

        default:
            printf("Digite uma opcao valida\n");
            break;
        }
    }
    return 0;
}
