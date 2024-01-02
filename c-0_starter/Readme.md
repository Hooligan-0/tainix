Tainix Starter en C
===================

Ce dossier contient des resources pour réaliser les premiers challenges Tainix
(difficulté "starter") en utilisant le langage C :) Au moment où ce fichier est
écrit, il y a 6 challenges starter.

Pour chaque challenge, il y a un fichier avec un squelette de code et un second
avec le suffixe "_solution" qui contient une réponse possible pour ce challenge.
Je n'ai pas cherché à faire le code le plus compact ou le plus rapide mais c'est
du pur C simple a lire et à reprendre :)

Attention ! Les plus attentifs remarqueront qu'il y a des buffers qui ne sont
pas désaloués (il manque des free() quoi). Etant donné que ces programmes ont
besoin de peu de mémoire et s'executent très peu de temps, je les aient
volontairement omis pour gagner en lisibilité (l'OS fera le menage à la fin)

Compilation
-----------

Le dossier contient un script Makefile pour automatiser la compilation. Je ne
l'ai teste que sous GNU/Linux, quelques adaptations seront necessaire pour
l'utiliser sous Windows ou sur Mac (ou sur Hurd, ou BeOS, ...)

Il y a une dependance a la librarie `curl` pour communiquer avec le serveur
(voir ci-dessous).

Communication avec l'API
------------------------

Pour récupérer les données des challenges et envoyer les resultats il faut
communiquer avec l'API Tainix. Je n'avais pas acces à de doc particulière
donc je suis parti du code Python fourni par Tainix pour former les requetes.
Les requetes HTTP sont réalisées au moyen de la librairie curl (parce que ca
juste marche !) Il est possible (probable ?) qu'il reste des bugs ou surtout
des cas non traités, n'hesitez pas à me contacter si vous avez des remarques :)

Le code contenu dans les fichiers `_game_.c _game_.h` prend en charge la
communication avec le serveur et expose trois fonctions :
 - `game_get_challenge()` permet de charger les données du challenge au début
 - `game_input(key)` permet de récuperer une donnée en particulier
 - `game_send_response()` permet de soumettre une réponse au serveur

Les données échangées avec le serveur utilisent du JSON. Pour avoir quelque
chose de portable et léger, j'ai utilisé la librarie [jsmn](https://github.com/zserge/jsmn)
de Serge Zaitsev, un grand merci à lui :)
