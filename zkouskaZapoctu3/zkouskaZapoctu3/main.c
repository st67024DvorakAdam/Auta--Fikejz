#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct vozidlo {
	char znacka[20];
	char model[20];
	char spz[10];
	float spotreba;
	struct vozidlo* dalsi;
}tVozidlo;

void vypisPobocky(char* jmSoub) {
	FILE* input = fopen("data.csv", "r");
	FILE* output = fopen(jmSoub, "w");

	char line[200];
	while (fgets(line, 200, input) != NULL) {
		if (line[0] == 'P' && isupper(line[0])) {
			memmove(line, line + 2, strlen(line) - 1);
			fputs(line, output);
		}

	}
	fclose(input);
	fclose(output);
}

tVozidlo* nactiSeznam(char* jmSoub, char* jmPobocky) {
	tVozidlo* prvni = NULL; 
	tVozidlo* aktualni = NULL;  

	FILE* input = fopen(jmSoub, "r");
	char line[200];

	while (fgets(line, 200, input) != NULL) {
		if (strstr(line,jmPobocky) != NULL) {
			fgets(line, 200, input);
			while (line[0] != 'P') {
				tVozidlo* vozidlo = (tVozidlo*)malloc(sizeof(tVozidlo));
				char* token = strtok(line, ";");
				strcpy(vozidlo->znacka, token);

				token = strtok(NULL, ";");
				strcpy(vozidlo->model, token);

				token = strtok(NULL, ";");
				strcpy(vozidlo->spz, token);

				token = strtok(NULL, ";");
				vozidlo->spotreba = atof(token);
				vozidlo->dalsi = NULL;

				if (prvni == NULL) {
					prvni = vozidlo;
				}
				else {
					aktualni = prvni;
					while ((aktualni->dalsi) != NULL) {
						aktualni = aktualni->dalsi;
					}
					aktualni->dalsi = vozidlo;
				}

				fgets(line, 200, input);
			}
		}
	}
	fclose(input);
	return prvni;
}

void ulozNadPrumer(tVozidlo* seznamAut) {
	tVozidlo* prvni = seznamAut;
	tVozidlo* aktualni = prvni;
	double suma = 0;
	double prumernaSpotreba = 0;
	int counter = 0;

	FILE* output = fopen("nadprumernaSpotreba.txt", "w");

	while (aktualni != NULL) {
		suma += aktualni->spotreba;
		aktualni = aktualni->dalsi;
		counter++;
	}

	prumernaSpotreba = suma / counter;	

	aktualni = prvni;
	while (aktualni != NULL) {
		strcat(aktualni->model, "\n");
		if (aktualni->spotreba > prumernaSpotreba) {
			fputs(aktualni->model, output);
		}

		aktualni = aktualni->dalsi;
	}

	fclose(output);
}

void free_memory(tVozidlo* prvni) {
	tVozidlo* aktualni = prvni;

	while (aktualni != NULL) {
		tVozidlo* dalsi = aktualni->dalsi;
		free(aktualni);
		aktualni = dalsi;
	}
}

int main() {
	vypisPobocky("nazvyPobocek.txt");

	tVozidlo* prvni = nactiSeznam("data.csv", "pobocka 4");
	tVozidlo* aktualni = prvni;
	while (aktualni != NULL) {
		printf("%s\n", aktualni->model);
		aktualni = aktualni->dalsi;
	}

	ulozNadPrumer(prvni);

	free_memory(prvni);
	return 0;
}