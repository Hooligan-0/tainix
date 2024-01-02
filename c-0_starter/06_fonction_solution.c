/*
 * MIT License
 *
 * Copyright (c) 2024 Gwenael Saint-Genest <gwen@cowlab.fr>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_game_.h"

char tainix_key[] = "--votre clef d'api --";

void traite(int x, char *sortie);

/**
 * @brief Point d'entree du programme
 *
 */
int main(void)
{
	char  msg[1024];
	char  reponse[1020];
	char *tableau;
	char *ptr, *ptmp;
	int   valeur;

	/* Chargement des donnees du challenge */
	if (game_get_challenge("STARTER_6"))
	{
		fprintf(stderr, "Impossible d'obtenir les donnees du challenge\n");
		return(-1);
	}
	memset(reponse, 0, 1020);
	/* Recuperation des donnees d'entree du challenge */
	tableau = game_input("values");

	/* Ignore le caractere '[' de debut de tableau */
	ptr = tableau + 1;

	/* Boucle qui traite chaque element du tableau */
	while(ptr != 0)
	{
		/* Extrait la prochaine valeur du tableau */
		ptmp = strchr(ptr, ',');
		if (ptmp == 0)
			break;
		*ptmp = 0;
		valeur = atoi(ptr);
		*ptmp = ',';

		/* Appel la fonction de traitement sur cette valeur */
		traite(valeur, reponse);

		ptr = ptmp + 1;
	}
	/* Traite le dernier element en fin de tableau */
	ptmp = strchr(ptr, ']');
	if (ptmp)
	{
		/* Extrait la derniere valeur du tableau */
		*ptmp = 0;
		valeur = atoi(ptr);
		*ptmp = ']';
		/* Appel la fonction de traitement sur cette valeur */
		traite(valeur, reponse);
	}

	printf("La tableau a traiter : %s\n", tableau);
	printf("Resultat : %s\n", reponse);

	/* Construit un message avec des guillemets pour plaire au serveur  */
	sprintf(msg, "\"%s\"", reponse);
	/* Envoi la reponse au serveur */
	game_send_response(msg);

	return(0);
}

void traite(int x, char *sortie)
{
	char str_v[16];

	/* Si la valeur n'est pas multiple de 3, alors rien a faire */
	if (x % 3)
		return;

	/* Converti la valeur en texte pour pouvoir l'inserer dans le resultat */
	sprintf(str_v, "%d", x);

	/* Si la chaine de sortie contenait deja (au moins) une valeur */
	if (strlen(sortie) > 0)
		/* Alors, il faut ajouter un '-' comme separateur */
		strcat(sortie, "-");
	/* Ajoute la valeur trouvee a la chaine de sortie */
	strcat(sortie, str_v);
}

/* EOF */
