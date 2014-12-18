#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef enum {
	TRUE = 0, FALSE = 1
}boolean;

struct node;
struct node {
	boolean isWord;
	int count;
	struct node *chars[26];
	struct node *parent;
};
typedef struct node *NODE;

struct trie {
	NODE root;
	int height;
	int count;
};
typedef struct trie *TRIE;

TRIE createNewTrie() {
	TRIE temp = (TRIE)malloc(sizeof(struct trie));
	
	NODE newNode = (NODE)malloc(sizeof(struct node));
	newNode->isWord = FALSE;
	newNode->count = 0;
	newNode->parent = NULL;
	
	int j;
	for (j=0; j<26; j++) {
		newNode->chars[j] = NULL;
	}
			
	temp->root = newNode;
	temp->height = 0;
	temp->count = 0;
	
	return temp;
}

TRIE insert_trie(TRIE trie, char word[]) {
	NODE temp = trie->root;
	NODE prev = trie->root;
	int i = 0;
	
	//word[i] = tolower(word[i]);
	//int ind1 = word[i] - (int)('a');
	//temp = temp->chars[ind1];
	
	for (i=0; i<strlen(word); i++) {
		//printf("word: %s %d", word, word[i]);
		
		word[i] = tolower(word[i]);
		int index = word[i] - (int)('a');
		
		printf("%d -> ", index);
		temp = temp->chars[index];
		
		if (temp == NULL) {
			int j;
			//printf("null\n");
			NODE newNode = (NODE)malloc(sizeof(struct node));
			for (j=0; j<26; j++) {
				newNode->chars[j] = NULL;
			}
			newNode->count = 0;
			newNode->parent = prev;
			
			//printf("point 1\n", index);
			if (i == strlen(word)-1) {
				newNode->isWord = TRUE;
				
				printf("word ends %c", word[i]);
				temp = newNode;
				prev->chars[index] = temp;
				prev->count++;
				break;
			}
			else {
				newNode->isWord = FALSE;
			}
			//printf("point 2\n", index);	
			temp = newNode;
			//printf("point 3\n", index);
			prev->chars[index] = temp;
			prev->count++;
		}
		else {
			if (i == strlen(word)-1) {
				temp->isWord = TRUE;
				break;
			}
		}
		prev = temp;
	}
	trie->count++;
	printf("\n");
	return trie;
}

boolean find_trie(TRIE trie, char word[]) {
	NODE temp = trie->root;
	int i;
	for (i=0; i<strlen(word); i++) {
		
		int index = word[i] - (int)('a');
		printf("%d ->  ", index);
		temp = temp->chars[index];
		
		if (temp == NULL) {
			printf("null be %d", index);
			return FALSE;
		}
		if (i == strlen(word)-1 && temp->isWord == FALSE) {
			return FALSE;
		}
	}
	
	return TRUE;
}

boolean delete_node(NODE nd, char word[], int index) {
	int ind = word[index] - (int)('a');
	boolean val;
	
	if (nd->chars[ind] == NULL) {
		return FALSE;
	}
	else {
		if (index < strlen(word)) {
			val = delete_node(nd->chars[index], word, index+1);
		}
		if (val == TRUE) {
			nd->chars[ind] = NULL;
			NODE temp = nd->chars[ind];
			free(temp);
		}
	}
	
	return val;
}

TRIE delete_trie(TRIE trie, char word[]) {
	NODE temp = trie->root;
	int i;
	boolean found = TRUE;
	
	for (i=0; i<strlen(word); i++) {
		
		int index = word[i] - (int)('a');
		temp = temp->chars[index];
		
		if (temp == NULL) {
			found = FALSE;
			break;
		}
		if (i == strlen(word)-1 && temp->isWord == FALSE) {
			found = FALSE;
			break;
		}
	}
	
	if (found == TRUE) {
		printf("Found.\n");
	}
	else {
		printf("Not found.\n");
		return trie;
	}
	
	i = strlen(word) - 1;
	if (temp->count >= 1 && temp->isWord == TRUE) {
		printf("leaf count > 1\n");
		temp->isWord = FALSE;
		return trie;
	}
	else if(temp->count == 0 && temp->isWord == TRUE) {
		printf("leaf count = 0\n");
		NODE t2 = temp;
		temp = temp->parent;
		int index = word[i] - (int)('a');
		temp->chars[index] = NULL;
		temp->count--;
		free(t2);
	}
	
	i--;
	while (i > 0 && temp != NULL) {
		printf("looping: %d\n", i);
		if (temp->count == 0) {
			if (temp->isWord == TRUE) {
				printf("its a word! exiting: %d\n", i);
				return trie;
			}
			else {
				printf("going up 1\n");
				NODE t2 = temp;
				temp = temp->parent;
				printf("going up 2\n");
				int index = word[i] - (int)('a');
				printf("going up 3 %d\n", index);
				if (temp == NULL) {
					printf("parent null be\n");
				}
				else {
					temp->chars[index] = NULL;
					temp->count--;
				}
				free(t2);
				printf("going up 4\n");
			}
		}
		else {
			printf("count > 0 exiting! \n: %d", i);
			return trie;
		}
		i--;
	}
	return trie;
}

int main() {
	printf("-------------- TRIES IMPLEMENTOR ---------------\n");
	char ans='y';
	
	TRIE trie = createNewTrie();
	
	do {
		int ch;
		printf("Select an option. \n 1.Create trie \n 2. Insert word \n 3. Find word \n 4. Delete word \n");
		scanf("%d", &ch);
		
		char words[10][10];
		FILE *fp;
		char word[10];
		int i;
		
		switch(ch) {
			case 1:
				fp = fopen("words_trie.txt", "r");
				
				while (!feof(fp)) {
					fscanf(fp, "%s", words[i]);
					trie = insert_trie(trie, words[i]);
				}
				fclose(fp);
				break;
				
			case 2:
				printf("Enter word: ");
				scanf("%s", word);
				
				trie = insert_trie(trie, word);
				break;
				
			case 3:
				printf("Enter word: ");
				scanf("%s", word);
				
				if (find_trie(trie, word) == TRUE) {
					printf("Word found.\n");
				}
				else {
					printf("Not found.\n");
				}
				break;
			case 4:
				printf("Enter word: ");
				scanf("%s", word);
				
				trie = delete_trie(trie, word);
				break;
			default:
				printf("Invalid option entered.\n");
		}
		
		getchar();
		printf("continue? ");
		scanf("%c", &ans);
	}while(ans != 'n');
}
