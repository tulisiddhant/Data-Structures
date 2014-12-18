#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct book {
	int accNo;
	char title[10];
	char author[10];
	int price;
};
typedef struct book BOOK;

struct node;
struct node {
	BOOK b;
	struct node *left;
	struct node *right;
};
typedef struct node *NODE;

struct bst{
	NODE root;
	int count;
	int height;
};
typedef struct bst *BST;

typedef enum {
	TRUE = 0, FALSE = 1
}boolean;

BST createEmptyBST() {
	BST temp = (BST)malloc(sizeof(struct bst));
	temp->root = NULL;
	temp->count = 0;
	temp->height = 0;
	
	return temp;
}

BST insert_BST(BST bt, BOOK b) {
	NODE temp = bt->root;
	NODE par = bt->root;
	
	NODE newNode = (NODE)malloc(sizeof(struct node));
	newNode->b = b;
	newNode->left = NULL;
	newNode->right = NULL;
	
	if (temp == NULL) {
		bt->root = newNode;
		temp = newNode;
		bt->count = 1;
		return bt;
	}
	
	int dir = 0;
	if (b.accNo > (temp->b).accNo) {
		temp = temp->right;
		dir = 1;
	}
	else if (b.accNo <= (temp->b).accNo) {
		temp = temp->left;
		dir = 0;
	}
		
	while (temp != NULL) {
		if (b.accNo > (temp->b).accNo) {
			par = temp;
			temp = temp->right;
			dir = 1;
		}
		else if (b.accNo <= (temp->b).accNo) {
			par = temp;
			temp = temp->left;
			dir = 0;
		}
	}
	
	if (dir == 0) {
		par->left = newNode;
	}
	else {
		par->right = newNode;
	}
	
	bt->count++;
	bt->height = getHeight(bt->root);
	return bt;
}

boolean find_BST(BST bt, BOOK b) {
	NODE temp = bt->root;
	
	if (temp == NULL) {
		return FALSE;
	}
		
	while (temp != NULL) {
		if ((temp->b).accNo > b.accNo) {
			temp = temp->right;
		}
		else if ((temp->b).accNo < b.accNo) {
			temp = temp->left;
		}
		else {
			return TRUE;
		}
	}
	
	return FALSE;
}

NODE findSuccessor(NODE nd) {
	NODE par = nd->right;
	nd = nd->right;
	
	while (nd->left != NULL) {
		par = nd;
		nd = nd->left;
	}
	
	return par;
}

BST delete_BST(BST bt, BOOK b) {
	NODE temp = bt->root;
	NODE par = NULL;
	//NODE grandPa = NULL;
	
	if (temp == NULL) {
		printf("emptry bst");
		return bt;
	}
	
	if (b.accNo > (temp->b).accNo) {
		par = temp;
		temp = temp->right; 
	}
	else if (b.accNo < (temp->b).accNo) {
		par = temp;
		temp = temp->left;
	}
	else {
		par = NULL;
	}
		
	while (temp != NULL) {
		if (b.accNo > (temp->b).accNo) {
			par = temp;
			temp = temp->right;
		}
		else if (b.accNo < (temp->b).accNo) {
			par = temp;
			temp = temp->left;
		}
		else {
			break;
		}
	}
	
	if (temp != NULL) {
		if (temp->left == NULL && temp->right == NULL) {
			
			printf("case 1");
			if (par == NULL) {
				bt->root = NULL;
			}
			else if ((temp->b).accNo > (par->b).accNo) {
				par->right = NULL;
			}
			else if ((temp->b).accNo <= (par->b).accNo) {
				par->left = NULL;
			}
			
			free(temp);
		}
		else if (temp->left == NULL) {
			printf("case 2 R");
			
			if (par == NULL) {
				bt->root = bt->root->right;
			}
			else {
				if ((par->b).accNo >= (temp->b).accNo) {
					par->left = temp->right;
				}
				else {
					par->right = temp->right;
				}
			}
			free(temp);
		}
		else if (temp->right == NULL) {
			printf("case 2 L");
			
			if (par == NULL) {
				bt->root = bt->root->left;
			}
			else {
				if ((par->b).accNo >= (temp->b).accNo) {
					par->left = temp->left;
				}
				else {
					par->right = temp->left;
				}
			}
			free(temp);
		}
		else {
			printf("case 3");
			NODE p = findSuccessor(temp);
			//printf("suc got\n");
			NODE suc;
			
			if (p->left != NULL) {
				suc = p->left;
			}
			else {
				suc = p;
			}
			temp->b = suc->b;
			
			if (p == suc && p->right == NULL) {
				temp->right = NULL;
			}
			else if (p == suc && p->right != NULL) {
				temp->right = p->right;
			}
			
			if (p != suc) 
				p->left = suc->right;
			
			free(suc);
		}
	}
	else {
		printf("not found.");
	}
	
	//printf("deleted.Calc. height \n");
	bt->count--;
	bt->height = getHeight(bt->root);
	
	return bt;
}

BST newBST(BST old, NODE nd) {
	printf("acc No. %d", (nd->b).accNo);
	
	BST temp = (BST)malloc(sizeof(struct bst));
	temp->root = nd;
	temp->height = getHeight(old->root);
	temp->count = old->count - 1;
	
	return temp;
}

BOOK find_MaxPrice2(NODE nd) {			// returns maximum price by in-order traversal
	if (nd == NULL) {
		BOOK bk;
		bk.price = -1;
		return bk;
	}
	
	BOOK mp = find_MaxPrice2(nd->left);
	if ((nd->b).price > mp.price) {
		mp = (nd)->b;
	}
	
	BOOK mp2 = find_MaxPrice2(nd->right);
	if (mp2.price > mp.price) {
		mp = mp2;
	}
	
	return mp;
}

BOOK find_MaxPrice(BST bt) {			// returns maximum price by in-order traversal
	return find_MaxPrice2(bt->root);
	/*NODE root = bt->root;
	if (bt == NULL) {
		BOOK bk;
		bk.price = -1;
		return bk;
	}
	
	BOOK mp = find_MaxPrice(newBST(bt, root->left));
	if (((bt->root)->b).price > mp.price) {
		mp = (bt->root)->b;
	}
	
	BOOK mp2 = find_MaxPrice(newBST(bt, root->right));
	if (mp2.price > mp.price) {
		mp = mp2;
	}
	
	return mp;
	*/
}

BOOK find_MinPrice2(NODE nd) {			// returns minimum price by in-order traversal
	if (nd == NULL) {
		BOOK bk;
		bk.price = -1;
		return bk;
	}
	
	BOOK mp = find_MinPrice2(nd->left);
	if (mp.price != -1) {
		if ((nd->b).price < mp.price) {
			mp = nd->b;
		}
	}
	else {
		mp = nd->b;
	}
	
	BOOK mp2 = find_MinPrice2(nd->right);
	if (mp2.price < mp.price && mp2.price != -1) {
		mp = mp2;
	}
	
	return mp;
}

BOOK find_MinPrice(BST bt) {			// returns minimum price by in-order traversal
	return find_MinPrice2(bt->root);
/*	NODE root = bt->root;
	if (bt == NULL) {
		BOOK bk;
		bk.price = -1;
		return bk;
	}
	
	BOOK mp = find_MinPrice(newBST(bt, root->left));
	if (((bt->root)->b).price < mp.price) {
		mp = (bt->root)->b;
	}
	
	BOOK mp2 = find_MinPrice(newBST(bt, root->right));
	if (mp2.price < mp.price) {
		mp = mp2;
	}
	
	return mp;
	*/
}

int getHeight2(BST bt) {
	return getHeight(bt->root);
	/*printf("finding height \n");
	if (bt->root == NULL) {
		return 0;
	}
	int htLeft = 1 + getHeight(newBST(bt, (bt->root)->left));
	int htRight = 1 + getHeight(newBST(bt, (bt->root)->right));
	
	if (htLeft > htRight) {
		return htLeft;
	}
	else {
		return htRight;
	}*/
}

int getHeight(NODE nd) {
	if (nd == NULL) {
		return 0;
	}
	int htLeft = 1 + getHeight(nd->left);
	int htRight = 1 + getHeight(nd->right);
	
	if (htLeft > htRight) {
		return htLeft;
	}
	else {
		return htRight;
	}
}

int countNodes(BST bt) {
	int count = 0;
	if (bt->root == NULL) {
		return 0;
	}
	count = 1 + countNodes(newBST(bt, (bt->root)->left)) + countNodes(newBST(bt, (bt->root)->right));
	
	return count;
}

BOOK find_latestBook(BST bt) {
	NODE root = bt->root;
	
	while (root->left != NULL) {
		root = root->left;
	}
	
	return root->b;
}

BOOK find_oldestBook(BST bt) {
	NODE root = bt->root;
	
	while (root->right != NULL) {
		root = root->right;
	}
	
	return root->b;
}

BST insertPrice(BST bt, BOOK b) {
	NODE temp = bt->root;
	NODE par = bt->root;
	
	NODE newNode = (NODE)malloc(sizeof(struct node));
	newNode->b = b;
	newNode->left = NULL;
	newNode->right = NULL;
	
	if (temp == NULL) {
		printf("null\n");
		bt->root = newNode;
		return bt;
	}
	
	int dir = 0;
	if (b.price > (temp->b).price) {
		temp = temp->right;
		dir = 1;
	}
	else if (b.price <= (temp->b).price) {
		temp = temp->left;
		dir = 0;
	}
		
	while (temp != NULL) {
		if (b.price > (temp->b).price) {
			par = temp;
			temp = temp->right;
			dir = 1;
		}
		else if (b.price <= (temp->b).price) {
			par = temp;
			temp = temp->left;
			dir = 0;
		}
	}
	
	if (dir == 0) {
		par->left = newNode;
	}
	else {
		par->right = newNode;
	}
	
	bt->count++;
	bt->height = getHeight(bt->root);
	return bt;
}

BST reOrderBST2(BST bst, NODE nd) {
	if (nd == NULL) {
		return bst;
	}
	
	bst = reOrderBST2(bst, nd->left);
	bst = insertPrice(bst, nd->b);
	bst = reOrderBST2(bst, nd->right);
	
	return bst;
}

BST reOrderBST(BST bt) {
	BST bst = createEmptyBST();
	//NODE root = bt->root;
	
	bst = reOrderBST2(bst, bt->root);
	/*
	bt = reOrderBST(newBST(bt, (bt->root)->left));
	bt = insertPrice(bt, (bt->root)->b);
	bt = reOrderBST(newBST(bt, (bt->root)->right));
	*/
	
	return bst;
}

void printBST2(NODE nd) {
	if (nd == NULL) {
		return;
	}
	
	printBST2(nd->left);
	
	BOOK bk = nd->b;
	printf("Acc No: %d \t Title: %s \t Price: %d \n --> ", bk.accNo, bk.title, bk.price);
	
	printBST2(nd->right);
}

void printBST(BST bt) {
	printBST2(bt->root);
}

int main() {
	int ch;
	char ans;
	BST bst = createEmptyBST();
	BST bst_price;
		
	do {
		printf("Enter the code \n 1. Create Tree \n 2. Add Node \n 3. Delete Node \n 4. Find Node \n 5. Max Price \n 6. Min Price \n 7. Oldest Book \n 8. Latest Book \n 9. Reorder BST \n 10. Print BST \n 11. Print price ordered BST \n");
		scanf("%d", &ch);
		
		BOOK bk;
		FILE *fp;
		
		switch(ch) {
			case 1:
				
				fp = fopen("bstData.txt", "r");
				
				while (!feof(fp)) {
					//fseek(fp, -1, 1);
					BOOK bk;
					fscanf(fp, "%d,%d,%s,%s\n", &bk.accNo, &bk.price, bk.title, bk.author);
					//fgets(bk.title, 5, fp);
					//fgets(bk.title, 7, fp);
					strcpy(bk.title, "hello");
					strcpy(bk.author, "world");
					
					printf("\nadding: %d, %s, %d, %s", bk.accNo, bk.title, bk.price, bk.author);
					bst = insert_BST(bst, bk);
				}
				break;
				
			case 2:
				printf("Enter details\n accNo: ");
				scanf("%d", &bk.accNo);
				getchar();
				printf("Title: ");
				gets(bk.title);
				printf("Author: ");
				gets(bk.author);
				printf("Price: ");
				scanf("%d", &bk.price);
				
				bst = insert_BST(bst, bk);
				printf("count: %d, height: %d, accNo: %d", bst->count, bst->height);
				if (bst->root == NULL) {
					printf("still null\n");
				}
				else {
					printf("not null\n");
				}
				break;
			
			case 3:
				printf("accNo: ");
				scanf("%d", &bk.accNo);
				
				delete_BST(bst, bk);
				break;
			
			case 4:
				printf("accNo: ");
				scanf("%d", &bk.accNo);
				
				boolean val = find_BST(bst, bk);
				if (val == TRUE) {
					printf("\nFound.\n");
				}
				else {
					printf("\nNot found.\n");
				}
				break;
			
			case 5:
				printf("Max price book\n");
				
				bk = find_MaxPrice(bst);
				printf("Acc No:%d \n Title: %s \n Price: %d \n", bk.accNo, bk.title, bk.price);
				break;
			
			case 6:
				printf("Min price book\n");
				
				bk = find_MinPrice(bst);
				printf("Acc No:%d \n Title: %s \n Price: %d \n", bk.accNo, bk.title, bk.price);
				break;
				
			case 7:
				printf("Oldest book\n");
				
				bk = find_oldestBook(bst);
				printf("Acc No:%d \n Title: %s \n Price: %d \n", bk.accNo, bk.title, bk.price);
				
				break;
				
			case 8:
				printf("Newest book\n");
				
				bk = find_latestBook(bst);
				printf("Acc No:%d \n Title: %s \n Price: %d \n", bk.accNo, bk.title, bk.price);
				
				break;
				
			case 9:
				printf("Reordering BST");
				bst_price = reOrderBST(bst);
				printf("Reordered BST created. \n");
				break;
				
			case 10:
				printf("BST: \n --> ");
				printBST(bst);
				break;
				
			case 11:
				if (bst_price != NULL) {
					printf("BST price ordered: \n");
					printBST(bst_price);
				}
				break;
				
			default:
				printf("Invalid.\n");
				break;
		}
		printf("\n status:\n count: %d, height: %d, root node accNo: %d \n", bst->count, bst->height, (bst->root->b).accNo);
		getchar();
		printf("Continue ?");
		scanf("%c", &ans);
	}while(ans != 'n');
	
	return 0;
}
