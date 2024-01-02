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
	char  msg[64];
	char  reponse[16];
	char *num_str;
	int   num;

	/* Chargement des donnees du challenge */
	if (game_get_challenge("STARTER_3"))
	{
		fprintf(stderr, "Impossible d'obtenir les donnees du challenge\n");
		return(-1);
	}
	memset(reponse, 0, 16);
	/* Recuperation des donnees d'entree du challenge */
	num_str = game_input("number");
	/* Conversion de la chaine de texte en nombre entier */
	num = atoi(num_str);


	(void)num; // ligne a supprimer ... permet juste d'eviter un warning de compilation

	/* ---- Ajouter le calcul ici ---- */




	/* Construit le resultat avec des guillemets pour plaire au serveur  */
	sprintf(msg, "\"%s\"", reponse);
	/* Envoi la reponse au serveur */
	game_send_response(msg);

	return(0);
}
/* EOF */
