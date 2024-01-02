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

/**
 * @brief Point d'entree du programme
 *
 */
int main(void)
{
	char  reponse[128];
	char *tableau;
	char *ptr, *ptmp;
	int   valeur, somme;

	/* Chargement des donnees du challenge */
	if (game_get_challenge("STARTER_5"))
	{
		fprintf(stderr, "Impossible d'obtenir les donnees du challenge\n");
		return(-1);
	}
	/* Recuperation des donnees d'entree du challenge */
	tableau = game_input("values");

	/* La chaine recu est un tableau JSON de la forme [nombre,nombre] */

	/* Le premier caractere est normalement '[' */
	if (tableau[0] == '[')
		/* On commence le traitement au caractere suivant */
		ptr = tableau + 1;
	else
	{
		fprintf(stderr, "Erreur de format du tableau d'entree\n");
		return(-1);
	}

	somme = 0;

	/* Boucle qui itere tant que l'on a pas trouve le dernier element */
	while(ptr != 0)
	{
		/* Recherche le prochain caractere ',' */
		ptmp = strchr(ptr, ',');
		/* Si caractere non trouve, alors c'est le dernier element */
		if (ptmp == 0)
			break;
		/* On remplace la virgule par un 0x00 pour couper la chaine */
		*ptmp = 0;
		/* Converti la valeur ecrite en texte vers un nombre entier */
		valeur = atoi(ptr);
		/* Puis replace la virgule (pas utile, juste plus propre :D ) */
		*ptmp = ',';

		/* Si la valeur est supérieure ou égale a 5, on l'ajoute au resultat */
		if (valeur >= 5)
			somme += valeur;

		/* La suite du traitement se fait sur le caractere qui suit la
		   virgule que l'on vient de traiter. */
		ptr = ptmp + 1;
	}
	/* Ici, le dernier element a ete trouve ... mais pas encore traite ! */

	/* Le tableau se termine normalement par un caractere ']' */
	ptmp = strchr(ptr, ']');
	if (ptmp)
	{
		/* On fait le meme traitement que pour la virgule ci-dessus */
		*ptmp = 0;
		valeur = atoi(ptr);
		*ptmp = ']';

		if (valeur >= 5)
			somme += valeur;
	}

	printf("La tableau a traiter : %s\n", tableau);
	printf("Resultat : %d\n", somme);

	/* Construit une chaine contenant le resultat attendu */
	sprintf(reponse, "%d", somme);
	/* Envoi la reponse au serveur */
	game_send_response(reponse);

	return(0);
}
/* EOF */
