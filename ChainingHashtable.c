#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE 100

struct node {
	char str[10];
	struct node *next;
};
typedef struct node *NODE;

struct hashtable {
	NODE buckets[SIZE];
	int count;
};
typedef struct hashtable *HASHTABLE;

typedef enum {
	TRUE = 0, FALSE = 1
}boolean;

HASHTABLE createHashtable() {
	HASHTABLE hashtbl = (HASHTABLE)malloc(sizeof(struct hashtable));
	//hashtbl->buckets = (NODE)malloc(sizeof(struct node)*50);
	hashtbl->count = 0;
	
	int i;
	for (i=0; i<SIZE; i++) {
		hashtbl->buckets[i] = NULL;
	}
	
	return hashtbl;
}

HASHTABLE insert_list(HASHTABLE hashtbl, int hashedVal, char str[]) {
	NODE cur = hashtbl->buckets[hashedVal];
	
	NODE newNode = (NODE)malloc(sizeof(struct node));
	strcpy(newNode->str, str);
	newNode->next = NULL;
	
	if (cur == NULL) {
		printf("empty");
		hashtbl->buckets[hashedVal] = newNode;
		return hashtbl;
	}
	
	printf("not empty");
	while(cur->next != NULL) {
		cur = cur->next;
	}
	cur->next = newNode;
	
	return hashtbl;
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
	int hashedVal = hash(str);
	printf("%d\n", hashedVal);
	hashtbl = insert_list(hashtbl, hashedVal, str);
	
	return hashtbl;	
}

void print_hash(HASHTABLE hashtbl) {
	int i;
	printf("hashtable:\n");
	for(i=0;i<SIZE;i++) {
		NODE cur = hashtbl->buckets[i];
		printf("%d:\t", i);
		while (cur != NULL) {
			printf("%s ->>", cur->str);
			cur = cur->next;
		}
		printf("\n");
	}
	printf("--------------------\n");
}

HASHTABLE delete_hash(HASHTABLE hashtbl, char str[]) {
	int hashValue = hash(str);
	NODE cur = hashtbl->buckets[hashValue];
	NODE prev = hashtbl->buckets[hashValue];
	if (strcmp(cur->str, str) == 0) {
		hashtbl->buckets[hashValue] = hashtbl->buckets[hashValue]->next;	
		return hashtbl;
	}
	
	while (cur != NULL) {
		if (strcmp(cur->str, str) == 0) {
			prev->next = cur->next;
			return hashtbl;
		}
		cur = cur->next;
	}
	return hashtbl;
}

boolean find_hash(HASHTABLE hashtbl, char str[]) {
	int hashValue = hash(str);
	NODE cur = hashtbl->buckets[hashValue];
	while(cur) {
		if (strcmp(cur->str, str) == 0) {
			return TRUE;
		}
		cur = cur->next;
	}
	return FALSE;
}

int main() {
	FILE *fp = fopen("strings.txt", "r");
	HASHTABLE hashtbl = createHashtable();
	
	char str[10];
	while (!feof(fp)) {
		fscanf(fp, "%s", str);
		insert_hash(hashtbl, str);
	}
	print_hash(hashtbl);
	return 0;
}
