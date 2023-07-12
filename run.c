#include "headers.h"

int main(int argc, char const *argv[])
{

	printf("Please enter one of the options below:\n");
	printf("Commands:\n");
	printf("Insert: I <ID>;<Title>;<Author>;<Year>\n");
	printf("Remove: R <ID>\n");
	printf("Search: B <ID>\n");
	printf("P1\n");
	printf("P2\n");
	printf("P3\n");
	printf("Close: F\n");

	char option[2]; 
	bTree* tree = createTree("tree.dat","data.dat",false);
	while(strcmp(option, "F")){
		scanf("%s", option);

	if(!strcmp(option, "I"))
    {

	char str[100], nome[30],titulo[30];
    int key, ano;
    scanf("%[^\n]s",str);
   	char * token = strtok(str, ";");
   	int i = 0;
      while(i < 4) {
        if(i == 0) key = atoi(token);
        if(i == 1) strcpy(titulo, token);
        if(i == 2) strcpy(nome, token);
        if(i == 3) ano = atoi(token);
        token = strtok(NULL, ";");
        i++;
   		}	
      insert(tree,key,createRecord(tree,key,titulo,nome,ano));

    }

    if(!strcmp(option, "R"))
    {
	    int key;
	    scanf("%d",&key);
	    bool res = removeFromTree(tree,key);
    	if(res)
    	{
    		printf("Successfull Deletion.\n");
    	}
    	else
    	{
    		printf("Deletion not successful.\n");
    	}
    }


	if(!strcmp(option, "B"))
    {
	    int key;
	    scanf("%d",&key);
	    recordNode* res = search(tree,key); // entender melhor o search
		if(res != NULL) {
			printf("%i\t%s\t%s\t%i\n", res->codigoLivro, res->titulo, res->nomeCompletoPrimeiroAutor, res->anoPublicacao);

			free(res);
		} else
			printf("O livro com código %d não existe na biblioteca\n", key);
    	
    }

	if(!strcmp(option, "P1"))
    {
	  p1(tree, tree->root);
    }

	if(!strcmp(option, "P2"))
    {
		p2(tree);
	}

	if(!strcmp(option, "P3"))
    {
    	p3(tree);
	}

}
	free(tree);

}	

