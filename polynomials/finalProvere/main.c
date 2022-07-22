#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
	int info;
	struct Node *next;
} node;

typedef struct {
	node *head;
	node *tail;
	int numElem;
} header;

header *addNodeEnd(header *tHeader, int x) {
	node *newNode = (node *)malloc(sizeof(node));
	newNode->info = x;
	if (!tHeader) {
		tHeader = (header *)malloc(sizeof(header));
		tHeader->head = newNode;
		tHeader->tail = newNode;
		tHeader->numElem = 1;
		newNode->next = newNode;
		return tHeader;
	}
	else {
		newNode->next = tHeader->head;
		tHeader->tail->next = newNode;
		tHeader->tail = newNode;
		tHeader->numElem++;
		return tHeader;
	}
}

header *addNodeStart(header *tHeader, int x) {
	node *newNode = (node *)malloc(sizeof(node));
	newNode->info = x;
	if (!tHeader) {
		tHeader = (header *)malloc(sizeof(header));
		tHeader->head = newNode;
		tHeader->tail = newNode;
		tHeader->numElem = 1;
		newNode->next = newNode;
		return tHeader;
	}
	else {
		tHeader->tail->next = newNode;
		newNode->next = tHeader->head;
		tHeader->head = newNode;
		tHeader->numElem++;
		return tHeader;
	}
}

header *deleteNodeEnd(header *tHeader) {
	if (!tHeader) {
		printf("Greska\n");
		return NULL;
	}
	else if (tHeader->numElem == 1) {
		free(tHeader->head);
		free(tHeader);
		return NULL;
	}
	else {
		node *current = tHeader->head;
		while (current->next != tHeader->tail) 
			current = current->next;
		current->next = tHeader->head;
		free(tHeader->tail);
		tHeader->tail = current;
		return tHeader;
	}
}

header *deleteNodeStart(header *tHeader) {
	if (!tHeader) {
		printf("Greska\n");
		return NULL;
	}
	else if (tHeader->numElem == 1) {
		free(tHeader->head);
		free(tHeader);
		return NULL;
	}
	else {
		tHeader->tail->next = tHeader->head->next;
		free(tHeader->head);
		tHeader->head = tHeader->tail->next;
		return tHeader;
	}
}

void printList(header *tHeader) {
	if (!tHeader)
		printf("Vasa lista je prazna\n");
	else {
		node *current = tHeader->head;
		do {
			printf("%d ", current->info);
			current = current->next;
		} while (current != tHeader->head);
		putchar('\n');
	}
}

int main() {
	int ind, x; 
	header *list = NULL; printList(list);
	while (1) {
		printf("\n1 - AddNodeEnd\n2 - AddNodeStart\n3 - DeleteNodeEnd\n4 - DeleteNodeStart\n");
		printf("Izbor: "), scanf("%d", &ind);
		switch (ind) {
			case 1:
				printf("Vrednost: "), scanf("%d", &x);
				list = addNodeEnd(list, x);
				printList(list);
				break;
			case 2:
				printf("Vrednost: "), scanf("%d", &x);
				list = addNodeStart(list, x);
				printList(list);
				break;
			case 3:
				list = deleteNodeEnd(list);
				printList(list);
				break;
			case 4:
				list = deleteNodeStart(list);
				printList(list);
				break;
			default:
				goto z;
				break;
		}
	}
	z: printf("Kraj programa\n");
	return 0;
}