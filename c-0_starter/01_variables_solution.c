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
#include "_game_.h"

char tainix_key[] = "--votre clef d'api --";

/**
 * @brief Point d'entree du programme
 *
 */
int main(void)
{
	char *cote_str;
	int   cote, peri, aire, somme;
	char  reponse[128];

	/* Chargement des donnees du challenge */
	if (game_get_challenge("STARTER_1"))
	{
		fprintf(stderr, "Impossible d'obtenir les donnees du challenge\n");
		return(-1);
	}

	/* Recuperation de la donnee "side" du challenge (taille du cote du carre) */
	cote_str = game_input("side");
	/* Conversion de la chaine de texte en nombre entier */
	cote = atoi(cote_str);

	/* Calcul du perimetre */
	peri = (cote * 4);
	/* Calcul de l'aire */
	aire = (cote * cote);
	/* Le resultat attendu est la somme des deux */
	somme = (peri + aire);

	/* Un petit message pour pouvoir verifier les valeurs ... et faire joli :) */
	printf("Le carre a un cote de %d unites", cote);
	printf("  - Le perimetre fait %d\n", peri);
	printf("  - L'aire fait %d\n", aire);
	printf("  - La somme des deux fait %d\n", somme);

	/* Met le resultat dans une chaine de texte pour envoi */
	sprintf(reponse, "%d", somme);
	/* Envoi la reponse au serveur */
	game_send_response(reponse);

	return(0);
}
/* EOF */
