# SGF-MPI
Implémentation en MPI d'un système de gestion de fichiers distribué partagé simplifié. Avec primitives PUT fichier et GET fichier. Source unique de lectures/écritures mais plusieurs machines de stockage. Possible répartition de charge à la round-robin (tourniquet). 

## Système de fichiers non-distribué
**Dossier:** system_nondistribue

Système de gestion de fichier non distribué avec un systemede fichier complexe

**Explication Fonctionnelle**

![alt tag](http://img15.hostingpics.net/pics/860033explicationsFonctionnelle0.png)

## Système de fichiers très simplifié distribué
**Dossier:** system_simple_distribue

Système de gestion de fichier distribué avec un systeme de fichier très simple

**Contenu:**
* projet.c: Contient toute la parite distribué du programme = gestion des comportements Serveurs/Clients
* commandes.c: Contient le code des commandes à implémenter
* utils.c: Contient des fonctions diverses
* gestionFichier.c & gestionFichier.h: Contient toute les fonctions utiles à la gestion du fichier data.bd.
* data.bd: Liste des fichiers ainsi que leurs infos: nom de la machine, chemin du fichier, etc...
* run.sh: Petit script permettant de recompiler et executer le programme dans son ensemble.

**Organisation du Code**

![alt tag](http://img11.hostingpics.net/pics/634461explication.png)

**Explication Fonctionnelle**

![alt tag](http://img15.hostingpics.net/pics/184417explicationsFonctionnelle1.png)

## Système de fichiers complexe distribué
**Dossier:** ???

**Explication Fonctionnelle**

![alt tag](http://img15.hostingpics.net/pics/717274explicationsFonctionnelle2.png)

## Pour aller plus loin
* Ajouter de la redondance de données afin pour faire de la tolérance aux pannes
  * Faire une étude sur le nombre minimun de copies à réaliser 
* En prenant compte du nombre de demandes et leur sources, faire un placement intelligent en prenant en compte la topologie du réseau: vitesse, latence, débit, etc...
* Rendre le projet multi-serveur, c'est à dire que plusieurs serveur écrivent dans le fichier d'index. 
