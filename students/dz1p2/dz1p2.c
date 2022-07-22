#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int counter = 0;

// student
typedef struct Node {
	int id;
	char name[30];
	char surname[30];
	char study[100];
	int year;
	struct Node* next;
} node;

// redovi za cekanje
typedef struct {
	node* head;
	node* tail;
	int numStudents;
} header;

// dodavanje studenata
header* addStudent(header* tHeader, int tID, char tName[], char tSurname[], char tStudy[], int tYear) {
	node* newStudent = (node*)malloc(sizeof(node));
	newStudent->id = tID;
	strcpy(newStudent->name, tName);
	strcpy(newStudent->surname, tSurname);
	strcpy(newStudent->study, tStudy);
	newStudent->year = tYear;
	if (tHeader->numStudents == 0) {
		tHeader->head = newStudent;
		tHeader->tail = newStudent;
		newStudent->next = newStudent;
		tHeader->numStudents++;
	}
	else {
		node* current = tHeader->head;
		do {
			if (current->id == tID) {
				printf("Vec postoji student sa ovim brojem indeksa!\n\n");
				return tHeader;
			}
			current = current->next;
		} while (current != tHeader->head);
		newStudent->next = tHeader->head;
		tHeader->tail->next = newStudent;
		tHeader->tail = newStudent;
		tHeader->numStudents++;
	}
	return tHeader;
}

// izbacivanje studenta
header* removeStudent(header* tHeader, int tID) {
	if (tHeader->numStudents == 0) {
		printf("U listi nema nijednog studenta!");
		return tHeader;
	}
	else if (tHeader->numStudents == 1) {
		if (tHeader->head->id == tID) {
			free(tHeader->head);
			tHeader->head = NULL;
			tHeader->tail = NULL;
			tHeader->numStudents--;
			return tHeader;
		}
		else {
			printf("U listi ne postoji student sa unetim indeksom!");
			return tHeader;
		}
	}
	else {
		node* current = tHeader->head, * previous = tHeader->tail;
		do {
			if (current->id == tID) {
				previous->next = current->next;
				if (current == tHeader->head) tHeader->head = current->next;
				else if (current == tHeader->tail) tHeader->tail = previous;
				free(current);
				tHeader->numStudents--;
				return tHeader;
			}
			previous = current;
			current = current->next;
		} while (current != tHeader->head);
		printf("U listi ne postoji student sa unetim indeksom!");
		return tHeader;
	}
}

// isprazni red
header* emptyQueue(header* tHeader) {
	counter = 0;
	if (tHeader->numStudents == 0) return tHeader;
	while (tHeader->numStudents != 0) tHeader = removeStudent(tHeader, tHeader->head->id);
	return tHeader;
}

// ispis liste
void printQueue(header* tHeader) {
	if (tHeader->numStudents == 0) printf("Vasa lista je prazna!\n\n");
	else {
		node* current = tHeader->head;
		do {
			printf("%d - %s %s - %s, %d godina\n", current->id, current->name, current->surname, current->study, current->year);
			current = current->next;
		} while (current != tHeader->head);
		printf("\n");
	}
}

// ispis studenta
// tInd = 1 za ispis svih informacija, tInd = 0 za ispis zahtevanih informacija
void printStudent(header* tHeader, int tID, int tInd) {
	if (tHeader->numStudents == 0) {
		printf("Najpre morate uneti studenta u listu za upis!\n\n");
		return;
	}
	node* current = tHeader->head;
	do {
		if (current->id == tID) {
			if (tInd) printf("%d - %s %s - %s, %d godina\n\n", current->id, current->name, current->surname, current->study, current->year);
			else printf("%s %s %d godina\n", current->name, current->surname, current->year);
			return;
		}
		current = current->next;
	} while (current != tHeader->head);
	printf("Student sa datim indeksom ne postoji u listi!\n\n");
}

// ucitavanje reda
header* loadQueue(header* tHeader) {
	tHeader = (header*)malloc(sizeof(header));
	tHeader->head = NULL;
	tHeader->tail = NULL;
	tHeader->numStudents = 0;
	return tHeader;
}

// ispis menija
void printMenu() {
	printf("Glavni meni\n");
	printf("1 - dodaj studenta u listu za upis\n2 - izbaci studenta iz liste za upis\n3 - pogledaj podatke o studentu\n");
	printf("4 - prikazi broj studenata u listi za upis\n5 - pregledaj listu\n6 - zapocni simulaciju\n\n");
}

// provera praznog reda
int isQueueEmpty(header* tHeader) {
	if (tHeader->numStudents == 0) return 1;
	return 0;
}

// postavljanje studenata u red ispred stud. sluzbe
header* putStudentsInAdmissionQueue(header* tHeader1, header* tHeader2) {
	node* current = NULL; int gg, i;
	while (!isQueueEmpty(tHeader1)) {
		current = tHeader1->head; 
		gg = rand() % tHeader1->numStudents;
		for (i = 0; i < gg; i++) current = current->next;
		tHeader2 = addStudent(tHeader2, current->id, current->name, current->surname, current->study, current->year);
		tHeader1 = removeStudent(tHeader1, current->id);
		counter++;
	}
	return tHeader2;
}

// upis studenata
header* enrollStudents(header* tHeader, int tN, double tX) {
	double someNumber;
	while (!isQueueEmpty(tHeader)) {
		someNumber = rand() / (double)RAND_MAX;
		if (someNumber > tX) {
			//printf("%s %s %d godina\n", tHeader->head->name, tHeader->head->surname, tHeader->head->year);
			printStudent(tHeader, tHeader->head->id, 0);
			tHeader = removeStudent(tHeader, tHeader->head->id);
			counter++;
		}
		else {
			int tpID = tHeader->head->id, tpYear = tHeader->head->year;
			char tpName[30], tpSurname[30], tpStudy[100];
			strcpy(tpName, tHeader->head->name);
			strcpy(tpSurname, tHeader->head->surname);
			strcpy(tpStudy, tHeader->head->study);
			tHeader = removeStudent(tHeader, tpID);
			tHeader = addStudent(tHeader, tpID, tpName, tpSurname, tpStudy, tpYear);
			counter++;
		}
	}
	return tHeader;
}

// provera punog reda
int isQueueFull(header* tHeader, int tNum) {
	if (tHeader->numStudents == tNum) return 1;
	return 0;
}

// glavni program
int main() {
	int ind, id, year, n;
	double X;
	char name[30], surname[30], study[100];
	header* waitQueue = NULL; waitQueue = loadQueue(waitQueue);
	header* admQueue = NULL; admQueue = loadQueue(admQueue);
	// printMenu();
	while (1) {
		printMenu();
		printf("Vas izbor: "); scanf("%d", &ind);
		switch (ind) {
		case 1: 
			printf("Unesite indeks: "); scanf("%d", &id); getchar();
			printf("Unesite ime: "); scanf("%s", &name);
			printf("Unesite prezime: "); scanf("%s", &surname);
			printf("Unesite stud. program: "); scanf("%s", &study);
			printf("Unesite godinu studija: "); scanf("%d", &year);
			waitQueue = addStudent(waitQueue, id, name, surname, study, year);
			printf("\n");
			break;
		case 2: 
			printf("Unesite broj indeksa: "); scanf("%d", &id);
			waitQueue = removeStudent(waitQueue, id);
			printf("\n\n");
			break;
		case 3: 
			printf("Unesite broj indeksa: "); scanf("%d", &id);
			printStudent(waitQueue, id, 1);
			break;
		case 4: 
			printf("Broj studenata u listi: %d\n\n", waitQueue->numStudents);
			break;
		case 5: 
			printQueue(waitQueue); 
			break;
		case 6: 
			n = waitQueue->numStudents;
			if (n == 0) printf("Ne mozete izvrsiti upis ako niste uneli nijednog studenta!\n\n");
			else {
				printf("Trenutni broj studenata u listi je: %d\n", n);
				srand(time(0));
				admQueue = putStudentsInAdmissionQueue(waitQueue, admQueue);
				// printQueue(admQueue);
				prag:
				printf("Unesite prag upisa X (0 <= X <= 0.5): "); scanf("%lf", &X);
				if (X < 0 || X > 0.5) {
					printf("Niste dobro uneli prag! Unesite ponovo!\n");
					goto prag;
				}
				admQueue = enrollStudents(admQueue, n, X);
				printf("Broj koraka simulacije: %d\n\n", counter);
				printf("Zavrsili ste upis grupe studenata. Da li zelite ponovo?\n");
				printf("Unesite 1 ako zelite, u suprotnom unesite bilo sta drugo.\nVas unos: "); scanf("%d", &ind);
				if (ind != 1) goto x;
				else {
					admQueue = emptyQueue(admQueue);
					printf("\n");
				}
			}
			break;
		default: 
			printf("Uneli ste pogresan izbor! Pokusajte ponovo!\n\n");
			break;
		}
	}
	x:
	printf("\nUspesno ste zavrsili upis studenata!\n");
	printf("--------------------------------------------------------");
	return 0;
}