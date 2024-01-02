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
	char *stop_str;
	int   stop, somme;
	int   i;

	/* Chargement des donnees du challenge */
	if (game_get_challenge("STARTER_4"))
	{
		fprintf(stderr, "Impossible d'obtenir les donnees du challenge\n");
		return(-1);
	}
	/* Recuperation des donnees d'entree du challenge */
	stop_str = game_input("stop");
	stop = atoi(stop_str);

	/* Boucle qui itere de 1 à 'stop' */
	somme = 0;
	for (i = 1; i < stop; i++)
	{
		somme = somme + i;
	}

	printf("Valeur 'stop': %d --> somme: %d\n", stop, somme);

	/* Construit une chaine contenant le resultat attendu */
	sprintf(reponse, "%d", somme);
	/* Envoi la reponse au serveur */
	game_send_response(reponse);

	return(0);
}
/* EOF */
