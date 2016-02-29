---
layout: post
title: R�gulation de chauffage central - thermostat d'ambiance
date: 2015-11-06 22:59:05
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/acleis"
---

Dans le pr�c�dent article nous avons vu un prototype permettant de contr�ler la chaudi�re. Dans cet article nous verrons le dispositif qui envoie les ordres � la chaudi�re en fonction de diff�rents param�tres.

Il s'agit ici uniquement de logiciel, qui fait intervenir :

- un thermom�tre int�rieur (DS18B20, nRF24L01+) pr�sent dans ma [lampe de bureau](/~sven337/english/2014/05/08/Transforming-halogen-lamp-into-LED-lamp.html)
- un thermom�tre int�rieur situ� le s�jour, cach� dans le pot d'une plante (DS18B20, nRF24L01+, NiMH 1.2, module boost)
- un thermom�tre situ� dans la chambre (DS18B20, ESP8266, sur secteur), int�gr� dans le placard � c�t� de l'�lectronique d'�clairage � LED automatique (non pr�sent�es sur ce weblog pour l'instant)
- un thermom�tre ext�rieur (DS18B20, nRF24L01+, NiMH 1.2V, module boost, panneau solaire, projet que je n'ai pas encore d�crit sur ce weblog car il a �t� victime du **syndrome du prototype** : on fait quelque chose, �a marche du premier coup, on le met en place sans jamais le mettre au propre parce qu'il marche parfaitement, et on l'oublie... et deux ans apr�s il faut tout refaire car les d�fauts du prototype finissent par poser probl�me - dans le cas pr�sent, l'�lectronique s'est retrouv�e noy�e � plusieurs reprises apr�s de fortes pluies, et les contacts ont fini par se corroder)
- une interface permettant de choisir le mode COLD, HOT et AWAY, le changement �tant r�alis� � travers une [crontab](https://en.wikipedia.org/wiki/Cron)
- un programme serveur qui, en fonction du mode en cours et des temp�ratures, calcule s'il faut activer le chauffage ou pas

# Donn�es d'entr�e

Le principe du contr�le climatique pour le chauffage est de se baser sur la temp�rature ext�rieure, afin d'anticiper les variations �ventuelles de temp�rature int�rieure. Je crois que c'est souvent mis en oeuvre dans les grands b�timents tertiaires, en revanche cela me semble moins justifi� pour une habitation. Dans le cas pr�sent je souhaite me baser sur la temp�rature int�rieure afin que les apports de chaleur, par exemple ceux de l'�lectrom�nager ou de la cuisine, soient pris en compte.
Voici l'approche que j'ai retenue :

- une "temp�rature de non chauffage", d'environ 15�C d'apr�s les diff�rentes pages web que j'ai lues (� ajuster � l'usage). Cette temp�rature est un seuil de temp�rature ext�rieure � partir duquel on arr�te de chauffer.
- un calcul de temp�rature int�rieure, sous forme de moyenne pond�r�e de diff�rentes pi�ces (sachant que le poids change selon nuit/jour, par exemple la nuit, on pond�rera fortement la chambre, alors que le jour c'est plut�t le s�jour qui prendra le pas)
- une d�cision binaire chauffer/pas chauffer selon la comparaison de la temp�rature de l'�tape pr�c�dente avec une valeur de consigne (diff�rente selon le mode "chaud" et d'un mode "froid")

C'est un gros d�faut du chauffage central que de ne pas avoir la possibilit� d'un contr�le fin pi�ce par pi�ce. J'esp�re � terme disposer d'un chauffage �lectrique que je pourrai piloter pi�ce par pi�ce en fonction de la temp�rature de cette pi�ce, ce qui m'�viterait le bricolage d�crit dans cet article.

# Contr�le de chauffage

## Temp�ratures
Un programme serveur est responsable de calculer la temp�rature � intervalles r�guliers. Pour cela il fait une requ�te HTTP adress�e � la mini-application Flask que j'utilise pour stocker mes temp�ratures, et calcule une moyenne en fonction d'une pond�ration qui d�pend de l'heure du jour.

Voici un exemple des temp�ratures relev�es :

``` python
{'bed': 18.75, 'living': 21.6875, 'office': 22.18, 'exterior': 6.25, 'pantry': 15.312}
{'bed': datetime.datetime(2016, 2, 29, 21, 9, 11, 853085), 'living': datetime.datetime(2016, 2, 29, 21, 16, 22, 277984), 'office': datetime.datetime(2016, 2, 29, 21, 7, 2, 392827), 'exterior': datetime.datetime(2016, 2, 29, 21, 6, 38, 335546), 'pantry': datetime.datetime(2016, 2, 29, 21, 15, 4, 386432)}
```

## Consigne

La temp�rature obtenue est compar�e � la temp�rature de consigne, qui d�pend de l'�tat du programme. Il y a trois �tats :

- HOT: temp�rature de consigne de 21.5�C
- COLD: temp�rature de consigne de 18.5�C
- AWAY: temp�rature de consigne de 12�C

En pratique, je n'utilise pas AWAY : quand je pars, je coupe tout simplement la chaudi�re. Ma femme ne manque pas de me faire remarquer, � chaque retour de vacances, qu'il fait froid, mais la maison est chaude en 2 heures, donc je ne vois pas bien l'int�r�t de laisser fonctionner une chaudi�re en mon absence, m�me si c'est pour qu'elle chauffe tr�s peu.

Le changement HOT/COLD est d�termin� par une crontab qui ressemble � cela :

``` cron
## Basic rule: "hot" period is 5h30-8h30, then 17h-20h30
30 5    * * *  heating_control.sh hot
00 17   * * *  heating_control.sh hot
30 8,20 * * *  heating_control.sh cold

## Wife is home on Mondays, Wednesday, Saturdays and Sundays: these days are full-hot 5h-20h
## Note minute = 01 to override the basic block above
01 5-19 * * mon,wed,sat,sun heating_control.sh hot
```


# Interface utilisateur

## Bouton "j'ai froid"

La partie mat�rielle qui g�re la chaudi�re n'a pratiquement pas d'interface : elle est aliment�e directement par la chaudi�re, donc il n'y a pas de bouton on/off, et puisque les temp�ratures et �tats sont g�r�s logiciellement par un serveur sous Linux, il n'est pas possible de les modifier.
J'ai tout de m�me rajout� un bouton, d�nomm� "j'ai froid". Lorsque vous appuyez sur ce bouton, le dispositif d�marrera le br�leur pendant 20 minutes, ce qui est en g�n�ral suffisant (sinon, il suffit d'appuyer � nouveau) pour fournir le confort manquant.

Voici le dispositif :


## Commande "j'ai froid"

Le script **heating_control.sh** est minimaliste et envoie simplement un paquet UDP au programme serveur, qui reconna�t les commandes **hot**, **cold**, et **away**. Une autre commande est **forceheating**, qui force le programme � activer le br�leur pendant 20 minutes. Cette commande intervient � un niveau diff�rent du bouton pr�c�demment d�crit, mais son effet est le m�me. Ma femme utilise le bouton car c'est le plus simple pour elle, mais quand j'ai froid et que je ne veux pas me lever de mon bureau, j'utilise **forceheating**, cela m'�vite de me lever !

# Bilan

Ce montage est en fonctionnement depuis quelques mois et donne, � mon sens, de bons r�sultats. Il fait parfois trop froid ou trop chaud, mais c'est une cons�quence du chauffage central au gaz sans robinets thermostatiques. Cette technologie (ou, plus exactement, cette **absence** de technologie) ne permet pas un contr�le suffisamment fin pour avoir un confort optimal.
