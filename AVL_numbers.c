#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int co = 0;

struct node {
	int val;
	struct node *left;
	struct node *right;
	struct node *parent;
	int height;
};
typedef struct node *NODE;

struct avl {
	NODE root;
	int count;
	int height;
};
typedef struct avl *AVL;

typedef enum {
	TRUE = 0, FALSE = 1
}boolean;

AVL createEmptyAvl() {
	AVL avl = (AVL)malloc(sizeof(struct avl));
	avl->root = NULL;
	avl->count = 0;
	avl->height = 0;
	
	return avl;
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

int maxHeight(int a, int b) {
	if (a>b)
		return a;
	return b;
}

int getHeight(AVL avl) {
	return getHeight2(avl->root);
}

int getHeight2(NODE nd) {
	if (nd == NULL) {
		return -1;
	}
	//printf("ksjdhfk");
	int htLeft = 1 + getHeight2(nd->left);
	//printf("left %d\n", htLeft);
	int htRight = 1 + getHeight2(nd->right);
	//printf("right %d\n", htRight);
	
	if (htLeft > htRight) {
		return htLeft;
	}
	
	return htRight;
}

NODE rearrange_AVL(NODE par1, NODE par, NODE cur) {
	if (cur == par->right && par == par1->right) {
		// CASE 1 ==> RIGHT RIGHT
		printf("case right right\n");
		par1->right = par->left;
		
		//printf("point 1\n");
		if (par->left) 
			par->left->parent = par1;
		
		//printf("point 2\n");
		par->left = par1;
		par->parent = par1->parent;
		
		//printf("point 3\n");
		if (par->parent) {
			if (par->val > par->parent->val)
				par->parent->right = par;
			else
				par->parent->left = par;
		}
		
		par1->parent = par;
		//printf("point 4\n");
		par1->height = getHeight2(par1);
		//printf("point 5\n");
		par->height = getHeight2(par);
		//printf("point 6\n");
		cur->height = getHeight2(cur);
		
		printf("returning\n");
		return par;
	}
	else if (cur == par->left && par == par1->left) {
		// CASE 2 ==> LEFT LEFT
		printf("case left left\n");
		par1->left = par->right;
		if (par->right) {
			par->right->parent = par1;
		}
		par->right = par1;
		par->parent = par1->parent;
		
		if (par->parent) {
			if (par->val > par->parent->val)
				par->parent->right = par;
			else
				par->parent->left = par;
		}
		
		par1->parent = par;
		
		par1->height = getHeight2(par1);
		par->height = getHeight2(par);
		cur->height = getHeight2(cur);
		
		printf("returning\n");
		return par;
	}
	else if (cur == par->left && par == par1->right) {
		// CASE 3 ==> RIGHT LEFT
		printf("case right left\n");
		par1->right = cur->left;
		par->left = cur->right;
		
		//printf("point 1\n");
		if (cur->left) {
			cur->left->parent = par1;
		}
		if (cur->right) {
			cur->right->parent = par;
		}
		
		//printf("point 2\n");
		cur->right = par;
		cur->left = par1;
		
		//printf("point 3\n");
		cur->parent = par1->parent;
		if (cur->parent != NULL) {
			if (cur->val > cur->parent->val)
				cur->parent->right = cur;
			else
				cur->parent->left = cur;
		}
		
		//printf("point 4\n");
		par->parent = cur;
		par1->parent = cur;
		
		//printf("point 5\n");
		par1->height = getHeight2(par1);
		par->height = getHeight2(par);
		cur->height = getHeight2(cur);
		
		printf("returning\n");
		return cur;
	}
	else if (cur == par->right && par == par1->left) {
		// CASE 4 ==> LEFT RIGHT
		printf("case left right @ %d \n", par1->val);
		par1->left = cur->right;
		par->right = cur->left;
		
		if (cur->left) {
			cur->left->parent = par;
		}
		if (cur->right) {
			cur->right->parent = par1;
		}
		
		//printf("step1\n");
		cur->right = par1;
		cur->left = par;
		
		//printf("step2\n");
		cur->parent = par1->parent;
		
		if (cur->parent) {
			printf("cur parent is now %d", cur->parent->val);
			if (cur->val > cur->parent->val)
				cur->parent->right = cur;
			else
				cur->parent->left = cur;
		}
		
		par->parent = cur;
		par1->parent = cur;
		
		//printf("step3\n");
		par1->height = getHeight2(par1);
		par->height = getHeight2(par);
		cur->height = getHeight2(cur);
				
		//printf("returning\n");
		return cur;
	}
}

AVL insert_AVL(AVL avl, int value) {
	NODE temp = avl->root;
	NODE par = avl->root;
	
	NODE newNode = (NODE)malloc(sizeof(struct node));
	newNode->val = value;
	newNode->height = 0;
	newNode->left = newNode->right = newNode->parent = NULL;
	
	if (temp == NULL) {
		avl->root = newNode;
		printf("tree is null\n");
		return avl;
	}
	
	int dir = 0;
	
	if (value > temp->val) {
		par = temp;
		temp = temp->right;
		dir = 1;
		printf("initial right\n");
	}
	else if (value <= temp->val) {
		par = temp;
		temp = temp->left;
		dir = 0;
		printf("initial left\n");
	}
	
	while (temp != NULL) {
		if (value > temp->val) {
			par = temp;
			temp = temp->right;
			dir = 1;
			printf("go right\n");
		}
		else if (value <= temp->val) {
			par = temp;
			temp = temp->left;
			dir = 0;
			printf("go left\n");
		}
	}
	
	if (dir == 0) {
		par->left = newNode;
		newNode->parent = par;
		printf("inserted @ left\n");
	}
	else {
		par->right = newNode;
		newNode->parent = par;
		printf("inserted @ right\n");
	}
	
	int hgt = 1;
	NODE cur = newNode;
	NODE par1 = par->parent;
	
	if (par->height < hgt) {
		par->height = hgt;
	}
	hgt++;
			
	while (par1 != NULL) {
		//printf("in loop\n");
		int lh = (par1->left != NULL) ? par1->left->height+1 : 0;
		int rh = (par1->right != NULL) ? par1->right->height+1 : 0;
		//printf("hgts: %d, %d\n", lh, rh);
		par1->height = maxHeight(lh, rh);
		
		if (((lh - rh) < -1) || ((lh - rh) > 1)) {
			printf("rearranging\n");
			
			if (par1 == avl->root) {
				avl->root = rearrange_AVL(par1, par, cur);
			}
			else {
				par1 = rearrange_AVL(par1, par, cur);
			}
			printf("rearranged\n");
			break;
		}
		//printf("in loop 2\n");
		hgt++;
		cur = par;
		par = par1;
		par1 = par1->parent;
		//printf("in loop 3\n");
	}
	
	avl->count++;
	//printf("calculating height\n");
	avl->height = getHeight(avl);
	//printf("returning\n");
	return avl;
}

boolean find_AVL(AVL avl, int value) {
	NODE temp = avl->root;
	
	if (temp == NULL) {
		return FALSE;
	}
	
	while (temp != NULL) {
		if (value > temp->val) {
			temp = temp->right;
		}
		else if (value < temp->val) {
			temp = temp->left;
		}
		else {
			return TRUE;
		}
	}
	
	return FALSE;
}

AVL delete_AVL(AVL avl, int value) {
	NODE temp = avl->root;
	NODE par = avl->root;
	NODE nextN;
	
	if (temp == NULL) {
		return avl;
	}
	
	if (value > temp->val) {
		par = temp;
		temp = temp->right;
	}
	else if (value < temp->val) {
		par = temp;
		temp = temp->left;
	}
	else {
		par = NULL;
	}
	
	while (temp != NULL) {
		if (value > temp->val) {
			par = temp;
			temp = temp->right;
		}
		else if (value < temp->val) {
			par = temp;
			temp = temp->left;
		}
		else {
			break;
		}	
	}
	
	if (temp != NULL) {
		if (temp->right == NULL && temp->left == NULL) {
			// CASE 1: node is leaf
			printf("case 1\n");
			if (par == NULL) {
				avl->root = NULL;
				return avl;
			}
			else if (temp->val > par->val) {
				par->right = NULL;
			}
			else {
				par->left = NULL;
			}
			nextN = par;
			free(temp);	
		}
		else if (temp->right == NULL) {
			// CASE 2 L
			printf("case 2L\n");
			if (par == NULL) {
				avl->root = temp->left;
				//free(temp);
				//return avl;
			}
			else if (temp->val > par->val) {
				par->right = temp->left;
			}
			else {
				par->left = temp->left;
			}
			nextN = par;
			free(temp);	
		}
		else if (temp->left == NULL) {
			// CASE 2 R
			printf("case 2R\n");
			if (par == NULL) {
				avl->root = temp->right;
				//free(temp);
				//return avl;
			}
			else if (temp->val > par->val) {
				par->right = temp->right;
			}
			else {
				par->left = temp->right;
			}
			nextN = par;
			free(temp);	
		}
		else {
			// CASE 3: node has 2 children
			printf("case 3\n");
			NODE p = findSuccessor(temp);
			printf("suc got %d\n", p->val);
			NODE suc;
			if (p->left != NULL) {
				suc = p->left;
			}
			else {
				suc = p;
			}
			temp->val = suc->val;
			
			if (p == suc && p->right == NULL) {
				printf("temp right null be \n");
				temp->right = NULL;
			}
			else if (p == suc && p->right != NULL) {
				temp->right = p->right;
			}
			if (p != suc) 
				p->left = suc->right;
				
			nextN = temp;	
			free(suc);
			
		}
	}
	
	nextN->height = getHeight2(nextN);
	par = nextN->parent;
	par->height = getHeight2(par);
	NODE par1 = par->parent;
	
	//int hgt = par->height + 1;
	while (par1 != NULL) {
		par1->height = 1 + maxHeight(par1->left->height, par1->right->height);
		int tempHgt = 1 + maxHeight(par1->left->height, par1->right->height);
		
		if ((par1->left->height - par1->right->height) < -1 || (par1->left->height - par1->right->height) > 1) {
			if (par1->right->height > par1->left->height) {
				par = par1->right;
			}
			else {
				par = par1->left;
			}
			
			NODE cur;
			if (par->right->height > par->left->height) {
				cur = par->right;
			}
			else {
				cur = par->left;
			}
			par1 = rearrange_AVL(par1, par, cur);
		}
		if (par1->height == tempHgt) {
			printf("\n breaking out.\n");
			break;
		}
		else {
			printf("deleted but go up \n");
		}
		par1 = par1->parent;
	}
	return avl;
}

void print_AVL2(NODE nd) {
	if (nd == NULL) {
		return;
	}
	print_AVL2(nd->left);
	printf("count:: %d hgt:%d :: val:%d <----> \n", ++co, nd->height, nd->val);
	print_AVL2(nd->right);
}

void print_AVL(AVL avl) {
	printf("Tree:\n");
	co = 0;
	if (avl->root == NULL) {
		printf("empty avl\n");
	}
	print_AVL2(avl->root);
	printf("-----------------------------\n");
}

int main() {
	AVL avl = createEmptyAvl();
	int n, ch;
	char ans;
	FILE *fp;
	
	do {
		printf("Enter the code \n 1. Create Tree \n 2. Add Node \n 3. Delete Node \n 4. Find Node \n 5. Print avl \n");
		scanf("%d", &ch);
		
		switch(ch) {
			case 1:
				fp = fopen("randomNumbers.txt", "r");
				
				while(!feof(fp)) {
					fscanf(fp, "%d", &n);
					printf("inserting %d", n);
					avl = insert_AVL(avl, n);
					//print_AVL(avl);
					//getchar();
				}
				
				printf("done\n");
				fclose(fp);
				break;
				
			case 2:
				printf("Enter number: ");
				scanf("%d", &n);
				avl = insert_AVL(avl, n);
				break;
				
			case 3:
				printf("Enter number: ");
				scanf("%d", &n);
				avl = delete_AVL(avl, n);
				break;
							
			case 4:
				printf("Enter number: ");
				scanf("%d", &n);
				boolean found = find_AVL(avl, n);
				if (found == TRUE) {
					printf("found.\n");
				}
				else {
					printf("not found.\n");
				}
				break;
				
			case 5:
				print_AVL(avl);
				break;
			default:
				printf("Incorrect choice.\n");
				break;
		}
		getchar();
		printf("Continue ?");
		scanf("%c", &ans);
	}while(ans != 'n');
}
