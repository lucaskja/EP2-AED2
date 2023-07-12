#include "headers.h"

// Respostas das perguntas feitas no ep:
// https://docs.google.com/document/d/13G6AX5XshXBNPPcrCFj1wfamBe47ywR9XHQNA26EyTM/edit#

/*****************************************************************************************/
/**   ACH2024 - Algoritmo e Estrutura de Dados II                                       **/
/**   EACH-USP - Primeiro Semestre de 2022                                              **/
/**   94 - Karina Valdivia Delgado                                                      **/
/**                                                                                     **/
/**   Segundo Exercicio-Programa                                                        **/
/**                                                                                     **/
/**   Lucas Kledeglau Jahchan Alves             13732182                                **/
/**   Guilherme Campos Silva Lemes Prestes      13720460                                **/
/**   Vinicius Kazuo Inagaki                    13747967                                **/
/**                                                                                     **/
/**                                                                                     **/
/*****************************************************************************************/

bTree* createTree(char* fileName, char* registryFileName, bool mode)
{
	bTree* tree = malloc(sizeof(bTree));
    
	if(!mode) //new file
    {
        //Criar arquivo
        strcpy(tree->fileName,fileName);
        tree->fp = fopen(fileName,"w");
        fclose(tree->fp);

        strcpy(tree->registryFileName,registryFileName);
        tree->rfp = fopen(registryFileName,"w");
        fclose(tree->rfp);

        tree->root = 0;
        tree->nextPos = 0;
        tree->nextRecPos = 0;
    } else {
        FILE *oldFile = fopen(fileName, "r");
        fread(tree, sizeof(bTree), 1, oldFile);
        fclose(oldFile);
    }

    tree->fp = fopen(fileName, "r+");
    tree->rfp = fopen(registryFileName, "r+");
    return tree;
}

bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree)
{
	node->isLeaf = isLeaf;
	node->noOfRecs=0;
	node->pos = tree->nextPos;
	(tree->nextPos)++;
	int i;
	for (int i = 0; i < 2*t; ++i)
	{
		node->children[i] = -1;
	}
	return node;
}

void writeFile(bTree* ptr_tree, bTreeNode* p, int pos) {
    if(pos == -1) {
        pos = ptr_tree->nextPos++;
    }

    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);
    fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);
    
}

void readFile(bTree* ptr_tree, bTreeNode* p, int pos) {
    p->valid = true;
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
    fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}

void removeFile(bTree* ptr_tree, int pos) {    
    bTreeNode* p = (recordNode*) malloc(sizeof(bTreeNode));
    readFile(ptr_tree, p, pos);
    p->valid = false;

    writeFile(ptr_tree, p, pos);
    free(p);
}

void writeRecFile(bTree* ptr_tree, recordNode* p, int pos) {
    if(pos == -1) {
        pos = ptr_tree->nextRecPos++;
    }

    fseek(ptr_tree->rfp, pos * sizeof(recordNode), 0);
    fwrite(p, sizeof(recordNode), 1, ptr_tree->rfp);    
}

void readRecFile(bTree* ptr_tree, recordNode* p, int pos) {    
    fseek(ptr_tree->rfp, pos * sizeof(recordNode), SEEK_SET);
    fread(p, sizeof(recordNode), 1, ptr_tree->rfp);
}

void removeRecFile(bTree* ptr_tree, int pos) {    
    recordNode* p = (recordNode*) malloc(sizeof(recordNode));
    readRecFile(ptr_tree, p, pos);
    p->valid = false;

    writeRecFile(ptr_tree, p, pos);

    free(p);
}

//Retorna a posicao do dado no arquivo
int createRecord(bTree* ptr_tree, int codigoLivro, char titulo[], char nomeCompletoPrimeiroAutor[], int anoPublicacao) {
    recordNode* record = (recordNode*) malloc(sizeof(recordNode));

    record->valid = true;
    record->codigoLivro = codigoLivro;
    strcpy(record->titulo, titulo);
    strcpy(record->nomeCompletoPrimeiroAutor, nomeCompletoPrimeiroAutor);
    record->anoPublicacao = anoPublicacao;

    writeRecFile(ptr_tree, record, -1);
    free(record);

    return ptr_tree->nextRecPos - 1;
}

void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z,y->isLeaf,tree);
	z->noOfRecs = t-1;

	int j;
	for(j=0;j<t-1;j++)
	{
		z->keyRecArr[j] = y->keyRecArr[j+t];
        z->posRecArr[j] = y->posRecArr[j+t];
	}

	if(!y->isLeaf)
	{
		for(j=0;j<t;j++)
		{
			z->children[j] = y->children[j+t];
            y->children[j+t] = -1;
		}
	}
	y->noOfRecs = t-1;

	for(j=(x->noOfRecs); j >= i+1;j--)
	{
		x->children[j+1] = x->children[j];
	}
	//linking z
	x->children[i+1] = z->pos;

	for(j=(x->noOfRecs) - 1; j >= i;j--)
	{
		x->keyRecArr[j+1] = x->keyRecArr[j];
        x->posRecArr[j+1] = x->posRecArr[j];
	}
	x->keyRecArr[i] = y->keyRecArr[t-1];
    x->posRecArr[i] = y->posRecArr[t-1];
	x->noOfRecs++;

    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);
	free(z);
}

void insertNonFull(bTree* tree,bTreeNode* x, int recordKey, int recordPos)
{	
	int i = (x->noOfRecs)-1;
	if(x->isLeaf == true)
	{
		while((i>=0) && (recordKey < x->keyRecArr[i]))
		{
			x->keyRecArr[i+1] = x->keyRecArr[i];
            x->posRecArr[i+1] = x->posRecArr[i];
			i--;
		}
		x->keyRecArr[i+1] = recordKey;
        x->posRecArr[i+1] = recordPos;
		(x->noOfRecs)++;
        x->valid = true;

        writeFile(tree, x, x->pos);
	}
	else
	{
		while((i>=0) && (recordKey < x->keyRecArr[i]))
		{
			i=i-1;
		}
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);
        
		if(childAtPosi->noOfRecs == (2*t-1))
		{
			splitChild(tree,x,i+1,childAtPosi);
			if( x->keyRecArr[i+1] < recordKey){
				i++;
			}
		}

        readFile(tree, childAtPosi, x->children[i+1]);
		insertNonFull(tree,childAtPosi, recordKey, recordPos);

		free(childAtPosi);
	}
}

void insert(bTree* tree, int recordKey, int recordPos)
{
	if(tree->nextPos == 0) //árvore vazia, primeiro elemento.
	{
		tree->root = tree->nextPos;

		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
        firstNode->valid = true;
		firstNode->keyRecArr[0] = recordKey;
        firstNode->posRecArr[0] = recordPos;
		(firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos);

		free(firstNode);
		return;
	}
	else
	{
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);

		if(rootCopy->noOfRecs == (2*t-1))
		{
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

			splitChild(tree,newRoot,0,rootCopy);

			int i=0;
			if(newRoot->keyRecArr[0] < recordKey){
				i++;
			}
			
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi, recordKey, recordPos);


			tree->root = newRoot->pos;

			free(newRoot);
            free(childAtPosi);
		}
		else
		{
			insertNonFull(tree,rootCopy, recordKey, recordPos);
		}
		free(rootCopy);
	}
}

void traverse(bTree* tree, int root) {
    
    if(root == -1) {    
        return;
    }

    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    readFile(tree, toPrint, root);
    if(toPrint->valid) dispNode(toPrint);

    for(int i = 0; i < 2*t; i++) {
        traverse(tree, toPrint->children[i]);
    }

    free(toPrint);
}

void dispNode(bTreeNode* node)
{
	printf("Position in node:%d\n",node->pos );
    printf("Number of Records:%d\n",node->noOfRecs );
	printf("Is leaf?:%d\n",node->isLeaf );
    
	printf("Keys:\n");
	for(int i = 0; i < node->noOfRecs; i++)
		printf("%d ", node->keyRecArr[i]);
    printf("\n");

    printf("Records positions:\n");
	for(int i = 0; i < node->noOfRecs; i++)
		printf("%d ", node->posRecArr[i]);

	printf("\n");
	printf("Links:\n");
	for (int i = 0; i < 2*t; ++i)
	{
		printf("%d ",node->children[i] );
	}
	printf("\n");
    printf("\n");
}

void dispRec(recordNode* record) {
    if(record->valid){
        printf("Codigo do livro:%d\n",record->codigoLivro);
        printf("Titulo:%s\n",record->titulo);
        printf("Primeiro autor:%s\n",record->nomeCompletoPrimeiroAutor);
        printf("Ano de publicacao:%d\n",record->anoPublicacao);
    } else{
        puts("Registro excluído:");
        printf("Codigo do livro:%d\n",record->codigoLivro);
        printf("Titulo:%s\n",record->titulo);
        printf("Primeiro autor:%s\n",record->nomeCompletoPrimeiroAutor);
        printf("Ano de publicacao:%d\n",record->anoPublicacao);
    }
    

	printf("\n");
    printf("\n");
}

//Returns the data position
int searchRecursive(bTree* tree, int key, bTreeNode* root) {
    int i = 0;
    
    while(i < root->noOfRecs && key > root->keyRecArr[i])
        i++;
    
    
    if(i < root->noOfRecs && key == root->keyRecArr[i])
        return root->posRecArr[i];
    
    
    else if(root->isLeaf) {
        return -1;
    }
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);

        int found = searchRecursive(tree, key, childAtPosi);
        free(childAtPosi);
        return found;
    }
}

recordNode* search(bTree* tree, int key) {
  bTreeNode* root = malloc(sizeof(bTreeNode));
  readFile(tree, root, tree->root);
  
  int posFound = searchRecursive(tree, key, root);
  free(root);

  if(posFound == -1)
    return NULL;

  recordNode* rec = malloc(sizeof(recordNode));
  readRecFile(tree, rec, posFound);
  return rec;
}

int findKey(bTreeNode* node, int k) {
    int idx=0;
    while (idx < node->noOfRecs && node->keyRecArr[idx] < k)
        ++idx;
    return idx;
}

// Uma função para remover a idx-ésima chave deste nó - que é um nó folha
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx, bool removeRec) {
    //Get rec position
    int recPos = node->posRecArr[idx];

    // Deslocar todas as teclas após o idx-ésimo pos uma posição para trás 
    for (int i=idx+1; i<node->noOfRecs; ++i){
	    node->keyRecArr[i-1] = node->keyRecArr[i];
        node->posRecArr[i-1] = node->posRecArr[i];
    }
    // Reduzir o número de chaves
    node->noOfRecs--;

    //Removing record from file
    if(removeRec)
      removeRecFile(tree, recPos);
}
 
// Uma função para remover a idx-ésima chave deste nó - que não é um nó folha
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx, bool removeRec) {
 
    int k = node->keyRecArr[idx];
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
 
    // Se o filho que precede k (C[idx]) tem pelo menos t chaves,
    // encontra o predecessor 'pred' de k na subárvore com raiz em
    // C[idx]. Substitui k por pred. Recursivamente apaga pred
    // em C[idx]
    
    if (child->noOfRecs >= t) {
        int keyToBeDeleted = setFromPred(tree, node, idx, removeRec);
        removeNode(tree, child, keyToBeDeleted, false);
    }
 
    // Se o filho C[idx] tiver menos que t chaves, examine C[idx+1].
    // Se C[idx+1] tem pelo menos t chaves, encontre o sucessor 'succ' de k em
    // na subárvore com raiz em C[idx+1]
    // Substitui k por succ
    // Recursivamente apaga succ em C[idx+1]
    else if  (sibling->noOfRecs >= t) {
        int keyToBeDeleted = setFromSucc(tree, node, idx, removeRec);
        removeNode(tree, sibling, keyToBeDeleted, false);
    }
 
    // Se tanto C[idx] como C[idx+1] tiverem menos de t chaves,fundir k e todas as de C[idx+1]
    // em C[idx]
    // Agora C[idx] contém 2t-1 chaves
    // Libertar C[idx+1] e recursivamente apagar k de C[idx]
    else {
        child = merge(tree, node, idx);
        removeNode(tree, child, k, removeRec);
	    return;
    }
    
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}


void removeNode(bTree* tree, bTreeNode* node, int k, bool removeRec) {

    int idx = findKey(node, k);
    // A chave a ser removida está presente neste nó
    if (idx < node->noOfRecs && node->keyRecArr[idx] == k) {
        // Se o nó é um nó folha - removeFromLeaf é chamada
        // Caso contrário, a função removeFromNonLeaf é chamada
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx, removeRec);
        } else {
            removeFromNonLeaf(tree, node, idx, removeRec);
        }
        
        node->valid = false;
	    writeFile(tree, node, node->pos);
    }
    else {
       
        // Se este nó for um nó folha, então a chave não está presente na árvore
        if (node->isLeaf) {
		    return;
       	}
 
        // A chave a ser removida está presente na sub-árvore enraizada com este nó
        // O flag indica se a chave está presente na sub-árvore enraizada
        // com o último filho deste nó
        bool flag = idx==node->noOfRecs;
 
        // Se o filho onde a chave é suposto existir tiver menos de t chaves,
        // preenchemos esse filho

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);

        if (childAtPosi->noOfRecs < t) {
            fill(tree, node, idx);
            readFile(tree, childAtPosi, node->children[idx]);
        }

        // Se o último filho foi fundido, ele deve ter sido fundido com o filho anterior
        // anterior e então nós recursamos no (idx-1)th filho. Caso contrário, nós recursamos no
        // (idx)th child que agora tem pelo menos t chaves
        if (flag && idx > node->noOfRecs) {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx-1]);
            removeNode(tree, sibling, k, removeRec);

            writeFile(tree, sibling, sibling->pos);
            free(sibling);
        }
        else {
            removeNode(tree, childAtPosi, k, removeRec);
        }

        childAtPosi->valid = false;
        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}


// Uma função para obter o antecessor das keys[idx]
int setFromPred(bTree* tree, bTreeNode *node, int idx, bool removeRec) {
    // Continue a deslocar-se para o nó mais à direita até chegar a uma folha
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);

    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }
        
    int keyToBeDeleted = curr->keyRecArr[curr->noOfRecs-1];

    if(removeRec)
      removeRecFile(tree, node->posRecArr[idx]);
    node->keyRecArr[idx] = curr->keyRecArr[curr->noOfRecs-1];
    node->posRecArr[idx] = curr->posRecArr[curr->noOfRecs-1];

    free(curr);
    return keyToBeDeleted;
}

int setFromSucc(bTree* tree, bTreeNode *node, int idx, bool removeRec) {
 
    // Continuar a mover o nó mais à esquerda a partir de C[idx+1] até chegar a uma folha
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx+1]);
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[0]);
    }
 
    int keyToBeDeleted = curr->keyRecArr[0];

    if(removeRec)
      removeRecFile(tree, node->posRecArr[idx]);
    node->keyRecArr[idx] = curr->keyRecArr[0];
    node->posRecArr[idx] = curr->posRecArr[0];
    
    free(curr);
    return keyToBeDeleted;
}
 
// Uma função para preencher a criança C[idx] que tem menos de t-1 chaves
void fill(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));

    readFile(tree, cPrev, node->children[idx-1]);
    readFile(tree, cSucc, node->children[idx+1]);

    // Se o filho anterior(C[idx-1]) tiver mais de t-1 chaves, pede emprestada uma chave desse filho
    if (idx!=0 && cPrev->noOfRecs>=t) {
        borrowFromPrev(tree, node, idx);
    }
 
    // Se o filho seguinte(C[idx+1]) tiver mais do que t-1 chaves, pede uma chave emprestada a esse filho
    else if (idx!=node->noOfRecs && cSucc->noOfRecs>=t) {
        borrowFromNext(tree, node, idx);
    }
 
    // Fundir C[idx] com o seu irmão
    // Se C[idx] for o último filho, funde-o com o seu irmão anterior
    // Caso contrário, funde-o com o seu irmão seguinte
    else {
        if (idx != node->noOfRecs)
            merge(tree, node, idx);
        else
            merge(tree, node, idx-1);
    }

    free(cPrev);
    free(cSucc);

    return;
}
 
// Uma função para pedir emprestada uma chave de C[idx-1] e inseri-la em C[idx]
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx-1]);
    
    // A última chave de C[idx-1] vai para o pai e key[idx-1]
    // do pai é inserida como a primeira chave em C[idx]. Assim, o irmão perde
    // irmão perde uma chave e o filho ganha uma chave
 
    // Movendo todas as chaves em C[idx] um passo à frente
    for (int i=child->noOfRecs-1; i>=0; --i) {
        child->keyRecArr[i+1] = child->keyRecArr[i];
        child->posRecArr[i+1] = child->posRecArr[i];
    }
 
    // Se C[idx] não for uma folha, mover todos os seus apontadores filhos um passo à frente
    if (!child->isLeaf) {
        for(int i=child->noOfRecs; i>=0; --i)
            child->children[i+1] = child->children[i];
    }
 
    // Definir a primeira chave do filho igual a keys[idx-1] do nó actual
    child->keyRecArr[0] = node->keyRecArr[idx-1];
    child->posRecArr[0] = node->posRecArr[idx-1];
 
    // Mover o último filho do irmão como o primeiro filho de C[idx]
    if (!node->isLeaf) {
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1;
    }
 
    // Deslocar a chave do irmão para o pai
    // Isto reduz o número de chaves no irmão
    node->keyRecArr[idx-1] = sibling->keyRecArr[sibling->noOfRecs-1];
    node->posRecArr[idx-1] = sibling->posRecArr[sibling->noOfRecs-1];
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);
    
    free(child);
    free(sibling);

    return;
}
 
// Uma função para pedir emprestada uma chave do C[idx+1] e colocá-la no C[idx]
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    // keys[idx] é inserida como a última chave em C[idx]
    child->keyRecArr[(child->noOfRecs)] = node->keyRecArr[idx];
    child->posRecArr[(child->noOfRecs)] = node->posRecArr[idx];
 
    // O primeiro filho do irmão é inserido como o último filho em C[idx]
    if (!(child->isLeaf))
        child->children[(child->noOfRecs)+1] = sibling->children[0];
 
    // A primeira chave do irmão é inserida em keys[idx]
    node->keyRecArr[idx] = sibling->keyRecArr[0];
    node->posRecArr[idx] = sibling->posRecArr[0];
 
    // Mover todas as chaves no irmão um passo atrás
    for (int i=1; i<sibling->noOfRecs; ++i) {
        sibling->keyRecArr[i-1] = sibling->keyRecArr[i];
        sibling->posRecArr[i-1] = sibling->posRecArr[i];
    }
 
    // Mover os ponteiros filhos um passo atrás
    if (!sibling->isLeaf) {
        for(int i=1; i<=sibling->noOfRecs; ++i)
            sibling->children[i-1] = sibling->children[i];

        sibling->children[sibling->noOfRecs] = -1; //RESPOSTA:
    }
 
    // Aumentar e diminuir o número de chaves de C[idx] e C[idx+1] respectivamente
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}
 
// Uma função para fundir C[idx] com C[idx+1]
// C[idx+1] é free da memória após a fusão
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    // Retirar uma chave do nó actual e inseri-la na (t-1)ª posição de C[idx]
    child->keyRecArr[t-1] = node->keyRecArr[idx];
    child->posRecArr[t-1] = node->posRecArr[idx];
 
    // Copiar as chaves de C[idx+1] para C[idx] no final
    for (int i=0; i<sibling->noOfRecs; ++i) {
        child->keyRecArr[i+t] = sibling->keyRecArr[i];
        child->posRecArr[i+t] = sibling->posRecArr[i];
    }
 
    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child->isLeaf) {
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }

    // Move todas as chaves depois de idx no nó atual uma posição anterior - 
	// para não deixar espaços vazios criados pelo fato de movermos as keys[idx] para C[idx]
    for (int i=idx+1; i<node->noOfRecs; ++i) {
        node->keyRecArr[i-1] = node->keyRecArr[i];
        node->posRecArr[i-1] = node->posRecArr[i];
    }
 
    // Mover os apontadores filhos depois de (idx+1) no nó actual um passo antes
    for (int i=idx+2; i<=node->noOfRecs; ++i) 
        node->children[i-1] = node->children[i];
    node->children[node->noOfRecs] = -1; //RESPOSTA:
    // Atualizar a contagem de chaves do nó filho e do nó atual
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
 
    if(node->noOfRecs == 0) {
        //Precisamos corrigir isso fazendo com que ela aponte para a raiz verdadeira
        removeFile(tree, tree->root);
        tree->root = node->children[0];
    } else 
      writeFile(tree, node, node->pos);

    // Free a memória ocupada pelo irmão
    writeFile(tree, child, child->pos);

    removeFile(tree, sibling->pos);
    free(sibling);

    return child;
}

bool removeFromTree(bTree* tree, int key) {
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    if(search(tree, key) != NULL)
        removeNode(tree, root, key, true);
    else
        return false;

    free(root);
    return true;
}

void hardPrint(bTree* tree) {
    bTreeNode* lido = (bTreeNode*) malloc(sizeof(bTreeNode));
    for(int i = 0; i < tree->nextPos; i++) {
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if(lido->isLeaf <= 1)
            dispNode(lido);
        else
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
    }

    free(lido);
}

void hardRecPrint(bTree* tree) {
    recordNode* lido = (recordNode*) malloc(sizeof(recordNode));

    for(int i = 0; i < tree->nextRecPos; i++) {
        readRecFile(tree, lido, i);
        dispRec(lido);
    }

    free(lido);
}

void doublePrint(bTree* tree) {
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
}