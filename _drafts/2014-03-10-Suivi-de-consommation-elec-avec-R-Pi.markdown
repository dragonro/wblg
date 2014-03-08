---
title: Suivre sa consommation �lectrique avec un Raspberry Pi
date: 2014-03-08
category: francais
---

Cet article d�taille comment **suivre sa consommation �lectrique** avec un **Raspberry Pi** et un montage �lectronique simple et surtout tr�s bon march�. 
Il n�cessite de disposer d'un compteur �lectrique suffisamment moderne XXX d�tails et photos.

# Introduction

Le Raspberry Pi est un mini ordinateur capable de faire fonctionner Linux, et qui co�te peu cher. De nombreuses personnes sugg�rent de s'en servir comme serveur (web, e-mails, ...), mais je doute que ses performances soient suffisantes pour cela. (� vrai dire je n'ai jamais test� car j'utilise un eeePC 701 comme serveur depuis plusieurs ann�es, dont je suis tr�s satisfait.)

N�anmoins je poss�de un Raspberry Pi, ainsi qu'un placard �lectrique dans lequel je peux le poser, le brancher, et l'interfacer avec le compteur. En avant !

# La sortie t�l�info

La sortie **t�l�info** est pr�sente sur **tous les compteurs EDF** de moins de quelques ann�es. Elle r�pond � une sp�cification disponible en ligne : <http://norm.edf.fr/pdf/HN44S812emeeditionMars2007.pdf>. 
De nombreux projets se contentent de **capter l'impulsion lumineuse** du compteur (une impulsion = 1W.h en g�n�ral), mais la sortie t�l�info peut nous donner bien plus que cela :

- puissance instantan�e en watts
- indice du compteur
- situation heure pleine/heure creuse (qui ne me concerne toutefois pas car je me contente de l'option base, plus avantageuse dans ma situation)

La sortie t�l�info impl�mente un protocole qui n'est �lectriquement pas compatible avec les protocoles que parlent les ordinateurs en g�n�ral (RS-232, USB, parall�le, ...), ni d'ailleurs avec les protocoles du monde de l'informatique embarqu�e tels que I2C, SPI, ou un b�te GPIO. C'est un choix curieux de la part d'EDF, mais le protocole t�l�info est assez facile � convertir en un protocole connu tel que **RS-232**.
La sp�cification est publique et plut�t bien �crite, alors au travail !

La forme du signal est la suivante : un **0** correspond � une **sinuso�de** � **50KHz** variant entre **+0V** et **+12V**, un **1** correspond � un plateau � **+0V**. (j'�cris cela de m�moire, vous r�f�rer � la spec pour �tre s�rs).
**RS-232** utilise quant � lui **+12V** pour un 1, et **-12V** pour un 0. Cette tension n�gative est un mauvais choix technique qui complique beaucoup la vie de l'�lectronique moderne souhaitant impl�menter RS-232. En g�n�ral les circuits int�gr�s ont une UART qui utilise des niveaux de signaux diff�rents (TTL) : +Vdd = 3.3V pour un 1, et +0V pour un 0. C'est plus simple, et plus logique... mais les ports s�rie des ordinateurs "parlent" du vrai RS-232. Le circuit **MAX232** est un exemple de circuit int�gr� qui s'occupe de la conversion des niveaux entre RS-232 "r�el" et RS-232 TTL (celui � 3.3V).

# Travaux pr�c�dents

J'ignore ici les montages bas�s sur la **d�tection de l'impulsion lumineuse**. De nombreuses personnes ont r�alis� des montages (et parfois cr�� une activit� commerciale) permettant d'**interfacer la t�l�info avec RS-232**. On trouve un exemple abouti et bien expliqu�, qui a servi de base � ma r�alisation, � l'adresse suivante : <http://www.chaleurterre.com/forum/viewtopic.php?t=15153>.

Le probl�me de ces montages est que bien souvent ils visent � obtenir une compatibilit� avec RS-232 (par exemple <https://store.adtek.fr/home/12-interface-teleinfo-rs232.html>, mais aussi probablement, au vu du prix sup�rieur, <https://store.adtek.fr/home/10-teleinfo-usb-sans-souris-folle-v2.html>. Je ne connais pas cette soci�t� ni leurs produits, dont je n'ai pas de raison de penser qu'ils fonctionnent mal. Par contre, d�penser 30 euros ou plus pour ce genre de montage est clairement en dehors du budget que je m'�tais fix�.). Or, RS-232 ne nous int�resse pas, puisque le R-Pi impl�mente RS-232 avec des niveaux TTL, incompatible, mais aussi plus simple. Je ne suis bien s�r pas le premier � le remarquer, et on voit des montages qui consistent � brancher directement un optocoupleur bidirectionnel **SHF6206** � la sortie t�l�info et au Pi... sauf que ce mod�le est pratiquement introuvable, et que de mani�re g�n�rale les optos bidirectionnels co�tent cher.

J'ai donc r�alis� un montage diff�rent bas� sur des composants facilement disponibles (sur eBay, DealExtreme ou AliExpress, par exemple).

# Architecture

Ayant eu beaucoup de mal � trouver un optocoupleur qui ferait l'affaire pour un branchement direct (car il faut non seulement qu'il soit bidirectionnel mais aussi que ses caract�ristiques soient compatibles avec le timing du signal, chose pas toujours facile � garantir), j'ai opt� pour un montage redresseur et un filtrage (permettant d'obtenir **1** = **+12V** constant, **0** = **+0V**, qui attaque l'optocoupleur le moins cher que j'ai pu trouver, dont la sortie est reli�e au Pi de mani�re similaire au message du forum dont je donne un lien ci-dessus.

Bien s�r, cela repr�sente consid�rablement plus de composants, mais au final le montage m'est revenu moins cher que les alternatives, et je peux attester apr�s 8 mois de fonctionnement continu que la fiabilit� est au rendez-vous !
	
# R�alisation �lectronique


## Achat des fournitures
## Assemblage et branchement
# Int�gration logicielle
