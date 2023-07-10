#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct rec
{
	bool valid; // indica se o registro foi eliminado ou não
	int codigoLivro;
	// rest
	char titulo[30];
	char nomeCompletoPrimeiroAutor[30];
	int anoPublicacao;
};
typedef struct rec recordNode;

struct bTreeNode
{
	bool valid; //indica se o nó da árvore B foi eliminado ou não
	int pos; //tells where in node array is this treeNode present.
	bool isLeaf; //leaf or not?, if its leaf it's 1, else 0.

	int noOfRecs;
	int keyRecArr[2 * t - 1];
	int posRecArr[2 * t - 1];
	int children[2 * t]; // links
};
typedef struct bTreeNode bTreeNode;

struct tree
{
	char fileName[20];
	char registryFileName[20];
	FILE* fp; //File pointer
	FILE* rfp; //Registry file pointer
	int root;
	int nextPos;
	int nextRecPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName, char* registryFileName, bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree, int recordKey, int recordPos);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void dispRec(recordNode* rec);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);
void removeFile(bTree* ptr_tree, int pos);
void writeRecFile(bTree* ptr_tree, recordNode* p, int pos);
void readRecFile(bTree* ptr_tree, recordNode* p, int pos);
void removeRecFile(bTree* ptr_tree, int pos);

int createRecord(bTree* ptr_tree, int codigoLivro, char titulo[], char nomeCompletoPrimeiroAutor[], int anoPublicacao);
recordNode* search(bTree* tree, int key);
int searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
int setFromSucc(bTree* tree, bTreeNode *node, int idx, bool removeRec);
int setFromPred(bTree* tree, bTreeNode *node, int , bool removeRec);
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx, bool removeRec);
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx, bool removeRec);
void removeNode(bTree* tree, bTreeNode* node, int k, bool removeRec);
int findKey(bTreeNode* node, int k);
void hardPrint(bTree* tree);
void hardRecPrint(bTree* tree);