#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// element liste
typedef struct Node {
	int coef;
	int exp;
	struct Node* next;
} node;


// zaglavlje
typedef struct {
	node* head;
	node* tail;
	int numElements;
} listHeader;


//brisanje clana
listHeader* deleteNode(listHeader* headerT, int expT) {
	if (headerT == NULL) {
		printf("Ne mozete izbrisati cvor iz polinoma!\n\n");
		return headerT;
	}
	else if (headerT->numElements == 0) {
		printf("Ne mozete izbrisati cvor iz praznog polinoma!\n\n");
		return headerT;
	}
	node* current = headerT->head, * previous = NULL;
	while (current != headerT->tail && current->exp != expT) {
		previous = current;
		current = current->next;
	}
	if (current->exp != expT) printf("Dati clan ne postoji!\n\n");
	else if (headerT->numElements == 1) {
		free(current);
		headerT->head = NULL;
		headerT->tail = NULL;
		headerT->numElements--;
	}
	else if (current == headerT->tail) {
		previous->next = current->next;
		free(current);
		headerT->tail = previous;
		headerT->numElements--;
	}
	else if (current == headerT->head) {
		headerT->tail->next = current->next;
		headerT->head = current->next;
		free(current);
		headerT->numElements--;
	}
	else {
		previous->next = current->next;
		free(current);
		headerT->numElements--;
	}
	return headerT;
}


//dodavanje clana polinoma
listHeader* addNode(listHeader* tHeader, int tCoef, int tExp) {
	if (tCoef == 0) goto z;
	node* newNode = (node*)malloc(sizeof(node));
	newNode->coef = tCoef; newNode->exp = tExp;
	if (tHeader == NULL) printf("Najpre morate ucitati polinom!\n\n");
	else if (tHeader->numElements == 0) {
		tHeader->head = newNode;
		tHeader->tail = newNode;
		tHeader->numElements = 1;
		newNode->next = newNode;
	}
	else if (tExp > tHeader->head->exp) {
		tHeader->tail->next = newNode;
		newNode->next = tHeader->head;
		tHeader->head = newNode;
		tHeader->numElements++;
	}
	else if (tHeader->head->exp == tExp) {
		if (tHeader->head->coef + tCoef == 0) tHeader = deleteNode(tHeader, tExp);
		else tHeader->head->coef += tCoef;
		goto z;
	}
	else {
		node* current = tHeader->head, * previous = NULL;
		while (tExp <= current->exp && current != tHeader->tail) {
			if (current->exp == tExp) {
				if (current->coef + tCoef == 0) tHeader = deleteNode(tHeader, tExp);
				else current->coef += tCoef;
				goto z;
			}
			previous = current;
			current = current->next;
		}
		if (current == tHeader->tail && current->exp == tExp) {
			if (current->coef + tCoef == 0) tHeader = deleteNode(tHeader, tExp);
			else current->coef += tCoef;
			goto z;
		}
		else if (current == tHeader->tail && tExp < current->exp) {
			newNode->next = tHeader->head;
			tHeader->tail->next = newNode;
			tHeader->tail = newNode;
			tHeader->numElements++;
		}
		else {
			previous->next = newNode;
			newNode->next = current;
			tHeader->numElements++;
		}
	}
	z:
	return tHeader;
}


//racunanje vrednosti polinoma
void evaluatePoly(listHeader* tHeader, int tValue) {
	if (tHeader == NULL) printf("Niste ni ucitali polinom!\n\n");
	else if (tHeader->numElements == 0) printf("Ne mozete izracunati vrednost praznog polinoma!\n\n");
	else {
		node* current = tHeader->head;
		int sum = 0;
		while (current != tHeader->tail) {
			sum += current->coef * (int)pow(tValue, current->exp);
			current = current->next;
		}
		sum += current->coef * (int)pow(tValue, current->exp);
		printf("Vrednost polinoma je: %d\n\n\n", sum);
	}
}


//sabiranje dva polinoma
listHeader* addTwoPolys(listHeader* tHeader1, listHeader* tHeader2) {
	if (tHeader1 == NULL && tHeader2 == NULL) return NULL;
	else if (tHeader1 == NULL) return tHeader2;
	else if (tHeader2 == NULL) return tHeader1;
	else {
		listHeader* newPoly = (listHeader*)malloc(sizeof(listHeader));
		newPoly->head = NULL; newPoly->tail = NULL; newPoly->numElements = 0;
		if (tHeader1->numElements == 0 && tHeader2->numElements == 0) return newPoly;
		else if (tHeader1->numElements == 0) return tHeader2;
		else if (tHeader2->numElements == 0) return tHeader1;
		else {
			node* first = tHeader1->head, * second = tHeader2->head;
			do { 
				newPoly = addNode(newPoly, first->coef, first->exp);
				first = first->next;
			} while (first != tHeader1->head);
			do { 
				newPoly = addNode(newPoly, second->coef, second->exp);
				second = second->next;
			} while (second != tHeader2->head);
			return newPoly;
		}
	}
}


//mnozenje dva polinoma
listHeader* multiplyTwoPolys(listHeader* tHeader1, listHeader* tHeader2) {
	if (tHeader1 == NULL || tHeader2 == NULL) return NULL;
	listHeader* newPoly = (listHeader*)malloc(sizeof(listHeader));
	newPoly->head = NULL; newPoly->tail = NULL; newPoly->numElements = 0;
	if (tHeader1->numElements == 0 || tHeader2->numElements == 0) return newPoly;
	else {
		node* first = tHeader1->head, * second = tHeader2->head;
		do {
			do {
				newPoly = addNode(newPoly, first->coef * second->coef, first->exp + second->exp);
				second = second->next;
			} while (second != tHeader2->head);
			first = first->next;
		} while (first != tHeader1->head);
		return newPoly;
	}
}


//ispis polinoma
void polyPrint(listHeader* headerT) {
	if (headerT == NULL) printf("Niste ni ucitali polinom!\n\n\n");
	else if (headerT->numElements == 0) printf("Vas polinom je prazan!\n\n\n");
	else {
		printf("Vas polinom: ");
		node* current = headerT->head;
		do {
			printf("[%d * x^%d] ", current->coef, current->exp);
			current = current->next;
		} while (current != headerT->head);
		printf("\n\n\n");
	}
}


//ucitavanje polinoma
listHeader* loadPoly(listHeader* tHeader) {
	if (tHeader == NULL) {
		tHeader = (listHeader*)malloc(sizeof(listHeader));
		tHeader->head = NULL;
		tHeader->tail = NULL;
		tHeader->numElements = 0;
	}
	else printf("Vec ste ucitali izabrani polinom! Prvo morate da ga izbrisete!\n");
	return tHeader;
}


//brisanje polinoma
listHeader* deletePoly(listHeader* tHeader) {
	if (tHeader == NULL) printf("Niste ni ucitali polinom!");
	else if (tHeader->numElements == 0) return NULL;
	else {
		tHeader->tail->next = NULL;
		node* current = tHeader->head, * previous = NULL;
		while (current != NULL) {
			previous = current;
			current = current->next;
			free(previous);
		}
	}
	return NULL;
}


//glavni program
int main() {
	int n, ind, c, e, x;
	listHeader* poly1 = NULL, * poly2 = NULL, * poly3 = NULL, * poly4 = NULL, * poly5 = NULL;
	listHeader* temp = NULL, * temp1 = NULL, * temp2 = NULL;
	printf("Glavni meni - izaberite jednu od sledecih opcija:\n--------------------------------------------------\n");
	printf("1 - ucitavanje novog polinoma\n2 - dodavanje clana polinoma\n3 - brisanje clana polinoma\n");
	printf("4 - ispis izabranog polinoma\n5 - sabiranje dva proizvoljna polinoma\n6 - mnozenje dva proizvoljna polinoma\n");
	printf("7 - racunanje vrednosti izabranog polinoma\n8 - brisanje izabranog polinoma\n9 - kraj rada programa\n--------------------------------------------------\n\n");
	printf("Vas izbor: "); scanf("%d", &ind);
	while (ind != 9) {
		switch (ind) {


		// ucitavanje polinoma
		case 1: 
			y1:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: poly1 = loadPoly(poly1); printf("\n\n"); break;
			case 2: poly2 = loadPoly(poly2); printf("\n\n"); break;
			case 3: poly3 = loadPoly(poly3); printf("\n\n"); break;
			case 4: poly4 = loadPoly(poly4); printf("\n\n"); break;
			case 5: poly5 = loadPoly(poly5); printf("\n\n"); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y1; break;
			}
			break;


		// dodavanje clana polinoma
		case 2: 
			printf("Unesite koeficijent i eksponent (respektivno): "); scanf("%d%d", &c, &e);
			y2:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: poly1 = addNode(poly1, c, e); printf("\n\n"); break;
			case 2: poly2 = addNode(poly2, c, e); printf("\n\n"); break;
			case 3: poly3 = addNode(poly3, c, e); printf("\n\n"); break;
			case 4: poly4 = addNode(poly4, c, e); printf("\n\n"); break;
			case 5: poly5 = addNode(poly5, c, e); printf("\n\n"); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y2; break;
			}
			break;


		// brisanje clana polinoma
		case 3: 
			printf("Unesite eksponent koji zelite da izbrisete: "); scanf("%d", &e);
			y3:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: poly1 = deleteNode(poly1, e); printf("\n\n"); break;
			case 2: poly2 = deleteNode(poly2, e); printf("\n\n"); break;
			case 3: poly3 = deleteNode(poly3, e); printf("\n\n"); break;
			case 4: poly4 = deleteNode(poly4, e); printf("\n\n"); break;
			case 5: poly5 = deleteNode(poly5, e); printf("\n\n"); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y3; break;
			}
			break;


		// ispis polinoma
		case 4: 
			y4:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: polyPrint(poly1); break;
			case 2: polyPrint(poly2); break;
			case 3: polyPrint(poly3); break;
			case 4: polyPrint(poly4); break;
			case 5: polyPrint(poly5); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y4; break;
			}
			break;


		// sabiranje dva polinoma
		case 5:
			y51:
			printf("Unesite redni broj prvog polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: temp1 = poly1;  break;
			case 2: temp1 = poly2;  break;
			case 3: temp1 = poly3;  break;
			case 4: temp1 = poly4;  break;
			case 5: temp1 = poly5;  break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y51; break;
			}
			y52:
			printf("Unesite redni broj drugog polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: temp2 = poly1;  break;
			case 2: temp2 = poly2;  break;
			case 3: temp2 = poly3;  break;
			case 4: temp2 = poly4;  break;
			case 5: temp2 = poly5;  break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y52; break;
			}
			temp = addTwoPolys(temp1, temp2);
			if (temp == NULL) polyPrint(temp);
			else {
				if (poly1 == NULL || poly1->numElements == 0) {
					poly1 = temp;
					polyPrint(poly1);
				}
				else if (poly2 == NULL || poly2->numElements == 0) {
					poly2 = temp;
					polyPrint(poly2);
				}
				else if (poly3 == NULL || poly3->numElements == 0) {
					poly3 = temp;
					polyPrint(poly3);
				}
				else if (poly4 == NULL || poly4->numElements == 0) {
					poly4 = temp;
					polyPrint(poly4);
				}
				else if (poly5 == NULL || poly5->numElements == 0) {
					poly5 = temp;
					polyPrint(poly5);
				}
				else {
					printf("Svi polinomi su popunjeni! Vas rezultat cemo smestiti u 5. polinom!\n");
					poly5 = temp;
					polyPrint(poly5);
				}
			}
			break;


		// mnozenje dva polinoma
		case 6: 
		y61:
			printf("Unesite redni broj prvog polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: temp1 = poly1;  break;
			case 2: temp1 = poly2;  break;
			case 3: temp1 = poly3;  break;
			case 4: temp1 = poly4;  break;
			case 5: temp1 = poly5;  break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y61; break;
			}
		y62:
			printf("Unesite redni broj drugog polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: temp2 = poly1;  break;
			case 2: temp2 = poly2;  break;
			case 3: temp2 = poly3;  break;
			case 4: temp2 = poly4;  break;
			case 5: temp2 = poly5;  break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y62; break;
			}
			temp = multiplyTwoPolys(temp1, temp2);
			if (temp == NULL) polyPrint(temp);
			else {
				if (poly1 == NULL || poly1->numElements == 0) {
					poly1 = temp;
					polyPrint(poly1);
				}
				else if (poly2 == NULL || poly2->numElements == 0) {
					poly2 = temp;
					polyPrint(poly2);
				}
				else if (poly3 == NULL || poly3->numElements == 0) {
					poly3 = temp;
					polyPrint(poly3);
				}
				else if (poly4 == NULL || poly4->numElements == 0) {
					poly4 = temp;
					polyPrint(poly4);
				}
				else if (poly5 == NULL || poly5->numElements == 0) {
					poly5 = temp;
					polyPrint(poly5);
				}
				else {
					printf("Svi polinomi su popunjeni! Vas rezultat cemo smestiti u 5. polinom!\n");
					poly5 = temp;
					polyPrint(poly5);
				}
			}
			break;


		// racunanje vrednosti polinoma
		case 7: 
			printf("Unesite vrednost promenljive x: "); scanf("%d", &x);
			y7:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: evaluatePoly(poly1, x); break;
			case 2: evaluatePoly(poly2, x); break;
			case 3: evaluatePoly(poly3, x); break;
			case 4: evaluatePoly(poly4, x); break;
			case 5: evaluatePoly(poly5, x); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y7; break;
			}
			break;


		// brisanje polinoma
		case 8:
			y8:
			printf("Unesite redni broj polinoma (1-5): "); scanf("%d", &n);
			switch (n) {
			case 1: poly1 = deletePoly(poly1); printf("\n\n"); break;
			case 2: poly2 = deletePoly(poly2); printf("\n\n"); break;
			case 3: poly3 = deletePoly(poly3); printf("\n\n"); break;
			case 4: poly4 = deletePoly(poly4); printf("\n\n"); break;
			case 5: poly5 = deletePoly(poly5); printf("\n\n"); break;
			default: printf("Los redni broj! Ponovo izaberite!\n"); goto y8; break;
			}
			break;
		default: printf("Los izbor! Izaberite neku od ponudjenih stavki!\n\n"); break;
		}
		printf("Vas izbor: "); scanf("%d", &ind);
	}
	printf("Uspesno ste zavrsili program!\n\n--------------------------------------------------");
	return 0;
}