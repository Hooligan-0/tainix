Tainix Starter en Python
========================

Ce dossier contient des scripts python permettant de répondre aux challenges
Tainix de difficulté "starter". Au moment où ce fichier est écrit, il y a 6
challenges de ce niveau.

**Attention** Ces scripts contiennent des solutions. Si vous souhaitez
essayer de résoudre les challenges par vous-même attendez un peu avant de
les ouvrir :)

Pour le premier, j'ai fais une version purement procédurale (-proc) et une
version un peu plus objet (-obj).

Pour éviter d'inscrire la clef d'API dans le code, j'ai modifié la classe Game
pour que celle-ci aille la chercher dans une variable d'environnement TAINIX_KEY
Sous linux, il suffit de taper
```
export TAINIX_KEY="votre-clef-api"
```
