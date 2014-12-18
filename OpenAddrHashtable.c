#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 200

typedef enum {
	TRUE = 0, FALSE = 1
}boolean;

struct node {
	char str[10];	
	boolean deleted;
	boolean empty;
};
typedef struct node *NODE;

struct hashtable {
	NODE buckets[SIZE];
};
typedef struct hashtable *HASHTABLE;

HASHTABLE createHashtable() {
	HASHTABLE temp = (HASHTABLE)malloc(sizeof(struct hashtable));
	
	int i;
	
	for (i=0; i<SIZE; i++) {
		NODE tempNode = (NODE)malloc(sizeof(struct node));
		tempNode->deleted = FALSE;
		tempNode->empty = TRUE;
	
		temp->buckets[i] = tempNode;
	}
	
	return temp;
}

int hash(char str[]) {
	int len = strlen(str);
	int i, sum = 0;
	
	for (i=0; i<len; i++) {
		sum += (int)str[i];
	}
	return sum % SIZE;
}

HASHTABLE insert_hash(HASHTABLE hashtbl, char str[]) {
	int hashVal = hash(str);
	int initHash = hashVal-1;
	boolean flag = FALSE;
	printf(" @ %d", hashVal);
	
	while (flag != TRUE || hashVal != initHash) {
		NODE curBucket = hashtbl->buckets[hashVal];
		if ((curBucket->empty == TRUE) || (curBucket->deleted == TRUE)) {
			//printf("inside if\n");
			flag = TRUE;
			strcpy(curBucket->str, str);
			curBucket->empty = FALSE;
			printf("\t inserted @ %d\n", hashVal);
			return hashtbl;
		}
		else {
			//printf("inside else\n");
			hashVal = (hashVal+1) % SIZE;
		}
	}
}

boolean find_hash(HASHTABLE hashtbl, char str[]) {
	int hashVal = hash(str);
	int initHash = hashVal-1;
	NODE curBucket = hashtbl->buckets[hashVal];
	int c = 1;
	boolean cycle = FALSE;
	
	while (curBucket->empty == FALSE) {
		if (cycle == TRUE && hashVal >= initHash) {
			break;
		}
		if (strcmp(curBucket->str, str) == 0) {
			return TRUE;
		}
		else {
			/* LINEAR PROBING
			hashVal = (hashVal+1) % SIZE;
			*/
			
			// QUADRATIC PROBING
			if (hashVal >= SIZE) {
				cycle = TRUE;
			}
			hashVal = (hashVal + c*c) % SIZE;
			c++;
			
			curBucket = hashtbl->buckets[hashVal];
		}
	}
	
	return FALSE;
}

HASHTABLE delete_hash(HASHTABLE hashtbl, char str[]) {
	int hashVal = hash(str);
	int initHash = hashVal-1;
	boolean cycle = FALSE;
	
	NODE curBucket = hashtbl->buckets[hashVal];
	while (curBucket->empty == FALSE) {
		if (cycle == TRUE && hashVal >= initHash) {
			break;
		}
		if (strcmp(curBucket->str, str) == 0) {
			char *ptr = "\0";
			strcpy(curBucket->str, ptr);
			curBucket->deleted = TRUE;
			return hashtbl;
		}
		else {
			// LINEAR PROBING
			if (hashVal >= SIZE) {
				cycle = TRUE;
			}
			hashVal = (hashVal + 1) % SIZE;
			
			curBucket = hashtbl->buckets[hashVal];
		}
	}
}

void print_hash(HASHTABLE hashtbl) {
	int i;
	printf("hashtable:-\n");
	for (i=0; i<SIZE; i++) {
		printf("%d: %s\n", i, hashtbl->buckets[i]->str);
	}
	printf("----------------\n");
}

int main() {
	FILE *fp;
	char str[10];
	char ans;
	HASHTABLE hashtbl = createHashtable();
	
	do {
		int ch;
		printf("Select an option. \n 1.Create hashtable \n 2. Insert word \n 3. Find word \n 4. Delete word \n 5. Print hashtable\n");
		scanf("%d", &ch);
		
		switch(ch) {
			case 1:
				fp = fopen("words.txt", "r");
				while (!feof(fp)) {
					fscanf(fp, "%s", str);
					printf("inserting: %s", str);
					hashtbl = insert_hash(hashtbl, str);
				}
				fclose(fp);
				break;
			case 2:
				printf("Enter word: ");
				scanf("%s", str);
				hashtbl = insert_hash(hashtbl, str);
				printf("inserted.\n");
				break;
			case 3:
				printf("Enter word: ");
				scanf("%s", str);
				boolean found = find_hash(hashtbl, str);
				if (found == TRUE) {
					printf("found.\n");
				}
				else {
					printf("not found.\n9");
				}
				break;
			case 4:
				printf("Enter word: ");
				scanf("%s", str);
				hashtbl = delete_hash(hashtbl, str);
				printf("deleted.\n");
				break;
			case 5:
				print_hash(hashtbl);
				break;
			default:
				printf("Invalid choice.\n");
				break;
		}
		getchar();
		printf("Continue? ");
		scanf("%c", &ans);
	}while(ans != 'n');
	return 0;
}
