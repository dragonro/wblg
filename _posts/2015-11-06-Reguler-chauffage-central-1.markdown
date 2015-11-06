---
layout: post
title: R�gulation de chauffage central avec une chaudi�re Acleis - prototype
date: 2015-11-06 23:38:31
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/acleis"
---

Le logement dans lequel je viens d'emm�nager dispose d'un **chauffage central** au gaz avec une chaudi�re E.L.M. Leblanc Acleis de 2008. **Aucun thermostat** d'ambiance n'est pr�sent, et il n'y a par ailleurs **pas de robinet thermostatique** sur les radiateurs : en d'autres termes il n'existe aucun des moyens raisonnablement modernes de contr�ler la temp�rature du logement, et le choix est celui d'avoir trop chaud et de br�ler trop de gaz, ou d'avoir trop froid.
En effet le seul r�glage possible est celui de la temp�rature de d�part de l'eau du chauffage, directement sur la chaudi�re.

Ce syst�me archa�que ne me convient absolument pas, et j'ai d�cid� d'installer un thermostat d'ambiance... mais pourquoi payer pour acheter un syst�me du commerce qui sera limit�, alors que je pourrais contr�ler la chaudi�re avec un ordinateur ?

# D�couvrir ce qu'il est possible de faire avec cette chaudi�re

La d�cision a donc �t� prise de fabriquer mon propre thermostat d'ambiance. Premi�re �tape : lire le manuel de la chaudi�re et la d�monter, pour savoir le type de montage qu'il faudra r�aliser.

## Manuels techniques

Le manuel d'installation de la chaudi�re �tait fort heureusement pr�sent dans le logement. L'image ci-dessous montre ce qu'il nous propose concernant l'installation d'un thermostat.

![Installation d'un thermostat : manuel](manuel_thermostat.jpg)

On voit qu'il existe plusieurs m�canismes. L'un correspond au type le plus courant, figure 25 sur le document ci-dessus : une **commutation en 230V AC**. Un deuxi�me m�canisme figure 24 nous propose une **commutation en 24V DC avec 3 fils**, sans plus d'information.

On trouve sur le Web des manuels techniques de la marque, pour des chaudi�res diff�rentes, mais qui donnent plus d'information. Celui-ci, par exemple :
![Installation d'un thermostat : autre manuel](manuel_installation.jpg)

On identifie rapidement le connecteur ST12 qui est l'alimentation de la chaudi�re, **ST8** qui correspond au branchement figure 25, et **ST13** qui correspond au branchement figure 24 (utilisant les bornes 1 2 4, laissant les 9 et 8 avec leur strap). Ce document nous montre �galement un branchement utilisant les bornes 9 et 8 sur **ST13**, le cas 2', ce branchement n'�tant pas document� dans l'autre manuel.

On a donc au minimum 3 m�thodes de branchement possibles. Ouvrons la chaudi�re pour rep�rer les connecteurs.

## Ouverture de la chaudi�re

![Panneau de connexion de la chaudi�re](panel_connectors.jpg)

Sur cette photo (dont la qualit� laisse � d�sirer), j'ai rep�r� en **A** le connecteur **ST12** (alimentation), en **B** le connecteur **ST8** (commutation 230V AC, ici avec un strap car pas de thermostat), en **C** les trois bornes de **ST13** utilis�es pour le branchement du cas 3', en **D** les deux bornes du cas 2', et en **E** le connecteur qui sert (je crois) au thermostat TA 211E auquel le manuel fait vaguement r�f�rence (et qui contr�le une vanne m�langeuse 3 voies, il me semble).

Plus en d�tail, voici le connecteur ST8, avec son strap :

![Connecteur ST8](panel_230V.jpg)

Et le ST13, avec un strap �galement sur les bornes 9 et 8 (ainsi que deux fils que j'utilisais pour mes tests) :

![Connecteur ST13](panel_24V.jpg)

## Comparaison des m�thodes de raccord

### commutation ST8 (230V AC, circulateur)

Je pense que la commutation sur ST8 contr�le la **pompe de circulation** du chauffage. La chaudi�re maintient en permanence l'eau � la temp�rature r�gl�e en fa�ade, et l'eau ne circule que lorsque le thermostat fait le demande.
Cette commutation se fait en **230V AC**, et cela me posait probl�me pour une raison �vidente de s�curit�. En effet cela n�cessite une isolation un peu s�rieuse du montage �lectronique, et complique la mise au point car il faut d�brancher avant toute manipulation.
De plus, c'est un syst�me de type **tout ou rien** : soit le thermostat est en demande (et la chaudi�re va chauffer _� pleine puissance_), soit le thermostat n'est pas en demande et l'eau ne circule pas dans le circuit de chauffage.

C'est ce qui est en place **chez la plupart des gens** et ce syst�me fonctionne plut�t pas mal, mais j'ai r�alis� qu'on pouvait faire mieux.

### commutation ST13 broches 9 et 8 (24V DC, br�leur on/off)

Les broches 9 et 8 sont pont�es sur ma chaudi�re. En enlevant le pont afin de faire un test, on constate que ces broches (en 24V DC, donc aucun danger � faire des manipulations sous tension) commandent le **br�leur** de la chaudi�re en **tout ou rien**. M�me d�faut que pour **ST8**, mais au moins on travaille en **basse tension**... sauf qu'il y a un �norme probl�me : l'eau chaude sanitaire n'a pas la priorit� sur la commande, c'est-�-dire que si on branchait un thermostat sur ces broches 9 et 8, il n'y aurait d'eau chaude sanitaire que lorsque le thermostat est en demande de chauffage ! 
Il n'est pas surprenant, d�s lors, que ce branchement ne soit pas document� dans le manuel de la chaudi�re : il est en pratique **parfaitement inutilisable**.

### commutation ST13 broches 1 2 4 (24V DC, contr�le de d�bit du br�leur)

J'ai gard� le meilleur pour la fin. En lisant la documentation commerciale du thermostat TR200 pr�vu pour se brancher sur ces broches, on voit que E.L.M Leblanc parle d'une "r�gulation continue". Or, les m�thodes avanc�es de r�gulation du chauffage ne fonctionnent pas en tout ou rien, mais en modulant de mani�re continue la temp�rature de l'eau du chauffage : eau plut�t pas tr�s chaude (45-50�) quand il n'y a pas besoin de beaucoup chauffer, et eau tr�s chaude (jusqu'� 90�) lorsque les locaux sont tr�s froids. Voir par exemple [cette page](http://www.energieplus-lesite.be/index.php?id=10959) pour plus d'explications.

Il y avait donc de fortes chances que ces 3 broches permettent un contr�le continu de la temp�rature de l'eau. J'ai rapidement identifi� que la broche **1 (+24V DC)** et **4 (GND)** servaient d'alimentation. J'ai ensuite pri� pour ne rien casser, et j'ai branch� la 2 sur la 1 puis sur la 4 (courant observ� : 4mA) successivement. La chaudi�re n'a pas explos�, et j'ai constat� que ces op�rations permettaient un fonctionnement **tout-ou-rien** du **br�leur** (mais cette fois-ci l'eau chaude sanitaire fonctionnait).

Cela n'avait rien d'�vident : il y aurait pu avoir un **bus de communication** �volu� avec "handshake" entre le microcontr�leur dans la chaudi�re et dans le thermostat. D'ailleurs je pense que les chaudi�res r�centes fonctionnent comme cela, en tout cas c'est ce que moi je ferais pour �viter la concurrence sur les thermostats d'ambiance (produits d�gageant sans nul doute de tr�s fortes marges).

Bref, par chance, j'ai identifi� comment faire fonctionner le chauffage en **tout-ou-rien** avec une commutation � **basse tension**, ce qui est d�j� un tr�s bon point.

En installant un potentiom�tre j'ai ensuite observ� que la tension sur la broche n�2 semblait piloter le **d�bit de gaz** du br�leur&nbsp;: � 6.5V ou moins (3kOhm), le chauffage est compl�tement coup�, aux alentours de 10V le d�bit de gaz est moyennement important, et � 21V et plus il est maximal.
J'ignore si cette tension commande la **temp�rature de consigne de l'eau** (ce que j'esp�re), ou bien sert seulement � moduler la **puissance du br�leur** (ce que je crains). Il est possible de r�aliser quelques exp�riences pour le v�rifier mais j'estime que ce n'est pas n�cessaire dans cette premi�re �tape.
Il serait peut-�tre plus simple de d�monter la carte �lectronique pour faire une vraie ing�nierie inverse, mais je ne suis pas propri�taire de cette chaudi�re et c'est difficile d'�tudier le programme d'un micro-contr�leur de toute fa�on.

Ce qui est important, c'est que nous savons que nous pouvons utiliser les broches 1 2 et 4 pour une commutation tout-ou-rien, et plus si affinit�s (r�gulation continue).

# Prototype

Maintenant il s'agit de r�aliser un prototype de thermostat, basique, afin de prouver que le syst�me fonctionne. L'objectif est d'avoir un montage reli� � la chaudi�re, qui la contr�le en fonction des instructions qu'il re�oit de mon serveur de domotique (qui lui prendra sa d�cision en fonction de l'heure, de la temp�rature int�rieure, de la temp�rature ext�rieure, ...). Ce serveur (une combinaison d'eeePC et de Raspberry Pi) fonctionne sous Linux en wifi, et dispose �galement d'une interface nRF24L01+. Ici on utilisera du wifi car le montage ne fonctionnera pas sur batterie, donc pas besoin de subir les contraintes du nRF24L01+ afin d'�conomiser de l'�nergie.

## Microcontr�leur

L'**ESP8266** est un produit chinois qui est arriv� r�cemment sur le march� des bidouilleurs. Ce microcontr�leur est tr�s bon march� (quelques euros), et dispose d'une interface **wifi** (y compris WPA). Il est, gr�ce au travail de la communaut� et depuis 2015, compatible avec l'�cosyst�me **Arduino**, ce qui simplifie la mise en oeuvre.

J'ai fait un travail d'ing�nierie inverse sur le firmware de l'ESP8266, que je n'ai pour l'instant jamais publi�, mais je ne m'en suis jusqu'� pr�sent jamais servi pour un montage. Il faut bien commencer un jour !

## Alimentation

L'ESP8266 fonctionne en **3.3V** (en r�alit�, et bien que la documentation indique explicitement le contraire, il semble fonctionner correctement jusqu'� pr�s de 5V). La chaudi�re, dans sa grande bont�, nous propose **24V**. C'est trop pour un r�gulateur lin�aire, mais j'avais un petit module *buck* sous la main qui pouvait faire l'affaire. Il s'agit de [celui-ci](http://www.dx.com/p/mini-3a-4-5-28v-input-0-8-20v-output-step-down-voltage-regulator-green-238815). Le r�glage est assez compliqu� du fait du potentiom�tre qui permet tr�s peu de pr�cision, donc j'ai fait le choix d'adjoindre � ce module un r�gulateur lin�aire MCP1702-33, afin d'assurer une sortie 3.3V stable pour l'ESP8266 (associer ainsi des r�gulateurs n'est pas, th�oriquement, une tr�s bonne id�e du fait du risque d'instabilit�, mais je ne l'ai pas observ�).

## Contr�le de la chaudi�re

Pour contr�ler la chaudi�re, on souhaite � terme imposer sur la broche n�2 une **tension variable**, entre 0 et 24V (en r�alit� entre 6 et 21 car c'est l'intervalle qui semble faire une diff�rence). Dans un premier temps, on se contentera d'y mettre soit 0V, soit 24V.

Par d�faut, la broche 2 est **flottante** dans la chaudi�re (pas de pont install� en usine) : cela veut dire qu'en d�connectant simplement cette patte, la chaudi�re va fonctionner normalement (avec une tension observ�e de l'ordre de 21V : il y a certainement un *pull-up* interne � la chaudi�re). 

La solution est donc d'utiliser un transistor qui connectera cette broche � la masse (� travers une LED et une r�sistance, � des fins de diagnostic et de s�curisation de la chaudi�re en limitant le courant appel�), ou qui la laissera flottante. Il n'est pas possible de relier directement la broche au microcontr�leur : celui-ci ne tol�rera certainement pas de voir 24V sur un de ses outputs.

� terme, pour faire une variation continue, un PWM semble �tre une bonne id�e, � condition de lisser correctement le signal (car on ignore le type de signal que la chaudi�re attend ou tol�re !). Pour cela un condensateur entre la broche 2 et la masse devrait suffir.

## Photographies

Voici le montage tel que r�alis� sur *breadboard*. Mes excuses pour la pi�tre qualit� des photographies.

![ESP8266 et transistor de sortie](breadboard_esp8266.jpg)
![Alimentation](breadboard_power.jpg)
![Autre vue](breadboard_out.jpg)

Le transistor utilis� pour la sortie est un FDS6690A �PasCher(TM) achet� sur eBay. 
Je ne fais pas le sch�ma �lectronique car il est tr�s simple, si un lecteur le demande je prendrai peut-�tre le temps de m'en occuper.

## Code

Le montage re�oit en UDP une valeur de 0 � 100 correspondant � la puissance � commander � la chaudi�re. 
Le code est en ligne [ici](https://github.com/sven337/jeenode/blob/master/heating/heating.ino). 
Les tests montrent que le syst�me fonctionne correctement, en revanche les mises � jour *over-the-air* (OTA) en wifi de l'ESP8266 ne fonctionnent qu'une fois sur deux (j'utilise un module ESP-201 qui utilise une puce flash de 512Ko, valeur un peu faible pour les OTA, j'expliquerai peut-�tre les d�tails un jour), le module red�marre parfois de lui m�me sans raison apparente, ... mais c'est du d�tail.

# La suite...

Dans un prochain article je parlerai du c�t� serveur qui commande ce montage � partir du jour, de l'heure, et des temp�ratures int�rieures et ext�rieures.

<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>
