------------------------------------------------------------------

@@@@@@ PUISSANCE 4 @@@@@@@
Collaboreturs: Adam Haissoubi-Vogier et Jérôme Longuebray
Git: https://github.com/tracerline/Puissance4 

------------------------------------------------------------------


Présentation de l'application : 

Puissance4 est une application console qui propose deux modes de jeu.

Le premier mode de jeu est un match contre une Intelligence Artificielle simple qui tentera de bloquer les coups qui mèneraient à une potentielle victoire. Elle analyse aussi la disposition des pions pour jouer ses coups. Des couleurs ont été mises en place pour faciliter le jeu en console.
Le jeu se fait bien et la détection/ gestion des erreurs et des cas est mise en place pour une cohérence dans le jeu (gestion des victoires, tours, erreurs d'entrées, ...)

Le second mode est le Multijoueur et est expliqué en détail ci-dessous.


BONUS: Multijoueur (LAN)

Ce module a pris beaucoup de temps pour être mis en place et être fonctionnel. En effet, les librairies de programmation réseau sont assez complexes à manipuler et les méthodes demandent des connaissances solides dans le comportement des sockets. Cela explique notamment l'absence d'une interface graphique fonctionnelle.

---> Comment marche le multijoueur ?

Le multijoueur fonctionne avec un système dit "Peer-to-Peer" (Paire à Paire) où chaque joueur est à la fois client et serveur de sockets (moyen de communication entre les machines d'un réseau). On a la possibilité de soit : Héberger une partie ou Rejoindre une partie. 

Héberger une partie revient à créer une partie et attendre une connexion d'une machine distante (LAN exclusivement). Dès que la machine est connectée, la partie commence (par défaut, c'est le joueur hôte qui commence à jouer). Le système de socket gère le tour par tour en bloquant les envois de données du joueur J1 quand J2 doit entrer une colonne (et inversement).

Rejoindre une partie correspond à se connecter à un hôte distant. En effet, il est nécessaire de renseigner l'@IP de la machine distante qui héberge pour pouvoir rejoindre le jeu et commencé la partie.

Des vérifications sont constamment faites sur la connexions: état de la partie, état du socket, état des valeurs entrées (colonnes) pour une expérience optimale. Dès qu'un des deux joueurs se déconnecte ou interromps le jeu, la connexion se ferme.

PS: Tous les tests ont été faits sur un Linux et MacBook. La connexion entre Linux-MacBook pour une partie en multijoueur (sachant que ces deux machines appartenaient au même LAN) s'est parfaitement faites et le jeu fonctionne bien.