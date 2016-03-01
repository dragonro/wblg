---
layout: post
title: R�gulation de chauffage central avec une chaudi�re Acl�is - thermostat d'ambiance
date: 2016-02-29 22:59:05
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/acleis"
---

Dans [le pr�c�dent article](/~sven337/francais/2015/11/07/Reguler-chauffage-central-1.html) nous avons vu un prototype permettant de contr�ler la chaudi�re. Dans cet article nous verrons le dispositif qui envoie les ordres � la chaudi�re en fonction de diff�rents param�tres.

Les �l�ments utilis�s sont les suivants :

- un thermom�tre int�rieur (DS18B20, nRF24L01+) pr�sent dans ma [lampe de bureau](/~sven337/english/2014/05/08/Transforming-halogen-lamp-into-LED-lamp.html)
- un thermom�tre int�rieur situ� le s�jour, cach� dans le pot d'une plante (DS18B20, nRF24L01+, NiMH 1.2, module boost), non encore pr�sent� sur ce site
- un thermom�tre situ� dans la chambre (DS18B20, ESP8266, sur secteur), int�gr� dans le placard � c�t� de l'�lectronique d'�clairage � LED automatique, non encore present�s sur ce site
- un thermom�tre ext�rieur (DS18B20, nRF24L01+, NiMH 1.2V, module boost, panneau solaire, projet que je n'ai pas encore d�crit car il a �t� victime du **syndrome du prototype** : on fait quelque chose, �a marche du premier coup, on le met en place sans jamais le mettre au propre parce qu'il marche parfaitement, et on l'oublie... et deux ans apr�s il faut tout refaire car les d�fauts du prototype finissent par poser probl�me - dans le cas pr�sent, l'�lectronique s'est retrouv�e noy�e � plusieurs reprises apr�s de fortes pluies, et les contacts ont fini par se corroder)
- une interface permettant de choisir le mode COLD, HOT et AWAY, le changement �tant r�alis� � travers une [crontab](https://en.wikipedia.org/wiki/Cron)
- un programme serveur qui, en fonction du mode en cours et des temp�ratures, calcule s'il faut activer le chauffage ou pas et envoie la commande au dispositif reli� � la chaudi�re

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

## Exemple de fonctionnement

Voici un exemple du *log* de mon programme de contr�le. La temp�rature indiqu�e est exprim�e en dixi�mes de degr�s. (Donc 215 = 21.5�)

> 2016-02-29 16:38:06: State: HOT. Burner power is 0. Int temp 220
> 2016-02-29 16:53:14: State: HOT. Burner power is 0. Int temp 218
> 2016-02-29 16:53:51: State: FORCED. Burner power is 100. Int temp 218
> 2016-02-29 17:08:52: State: FORCED. Burner power is 100. Int temp 227

FORCED est la r�ponse � la commande forcedheating. J'ai d� avoir froid dans mon bureau � ce moment l�.

> 2016-02-29 17:14:05: State: HOT. Burner power is 0. Int temp 230
> 2016-02-29 17:29:13: State: HOT. Burner power is 0. Int temp 234
> 2016-02-29 17:44:20: State: HOT. Burner power is 0. Int temp 231
> 2016-02-29 17:59:28: State: HOT. Burner power is 0. Int temp 226
> 2016-02-29 18:14:36: State: HOT. Burner power is 0. Int temp 223
> 2016-02-29 18:29:44: State: HOT. Burner power is 0. Int temp 219
> 2016-02-29 18:44:52: State: HOT. Burner power is 0. Int temp 217
> 2016-02-29 19:00:00: State: HOT. Burner power is 0. Int temp 234
> 2016-02-29 19:15:09: State: HOT. Burner power is 0. Int temp 238
> 2016-02-29 19:30:16: State: HOT. Burner power is 0. Int temp 228
> 2016-02-29 19:45:24: State: HOT. Burner power is 0. Int temp 222
> 2016-02-29 20:00:31: State: HOT. Burner power is 0. Int temp 220
> 2016-02-29 20:15:39: State: HOT. Burner power is 0. Int temp 219
> 2016-02-29 20:31:05: State: COLD. Burner power is 0. Int temp 216
> 2016-02-29 20:46:13: State: COLD. Burner power is 0. Int temp 215
> 2016-02-29 21:01:20: State: COLD. Burner power is 0. Int temp 215
> 2016-02-29 21:16:28: State: COLD. Burner power is 0. Int temp 212
> 2016-02-29 21:31:36: State: COLD. Burner power is 0. Int temp 212
> 2016-02-29 21:46:44: State: COLD. Burner power is 0. Int temp 213
> 2016-02-29 22:01:51: State: COLD. Burner power is 0. Int temp 232
> 2016-02-29 22:16:59: State: COLD. Burner power is 0. Int temp 201
> 2016-02-29 22:32:07: State: COLD. Burner power is 0. Int temp 206
> 2016-02-29 22:47:14: State: COLD. Burner power is 0. Int temp 208
> 2016-02-29 23:02:22: State: COLD. Burner power is 0. Int temp 205
> 2016-02-29 23:17:29: State: COLD. Burner power is 0. Int temp 200
> 2016-02-29 23:32:37: State: COLD. Burner power is 0. Int temp 194
> 2016-02-29 23:47:45: State: COLD. Burner power is 0. Int temp 193
> 2016-03-01 00:02:53: State: COLD. Burner power is 0. Int temp 185
> 2016-03-01 00:09:39: State: COLD. Burner power is 100. Int temp 182

� ce moment dans la nuit, le br�leur va �tre activ� pour remonter la temp�rature au del� de la valeur de consigne.

> 2016-03-01 00:24:15: State: COLD. Burner power is 0. Int temp 189

... et 25 minutes plus tard, il sera arr�t�, alors que la temp�rature est remont�e de 0.7�C. Cela arrivera plusieurs fois dans la nuit.

> 2016-03-01 00:39:22: State: COLD. Burner power is 0. Int temp 192
> 2016-03-01 00:54:30: State: COLD. Burner power is 0. Int temp 191
> 2016-03-01 01:09:37: State: COLD. Burner power is 0. Int temp 187
> 2016-03-01 01:24:44: State: COLD. Burner power is 100. Int temp 183
> 2016-03-01 01:37:46: State: COLD. Burner power is 0. Int temp 184
> 2016-03-01 01:52:52: State: COLD. Burner power is 0. Int temp 189
> 2016-03-01 02:08:00: State: COLD. Burner power is 0. Int temp 191
> 2016-03-01 02:23:07: State: COLD. Burner power is 0. Int temp 188
> 2016-03-01 02:38:14: State: COLD. Burner power is 0. Int temp 184
> 2016-03-01 02:39:17: State: COLD. Burner power is 100. Int temp 181
> 2016-03-01 02:54:24: State: COLD. Burner power is 0. Int temp 187
> 2016-03-01 03:09:31: State: COLD. Burner power is 0. Int temp 190
> 2016-03-01 03:24:37: State: COLD. Burner power is 0. Int temp 187
> 2016-03-01 03:39:13: State: COLD. Burner power is 100. Int temp 183
> 2016-03-01 03:52:47: State: COLD. Burner power is 0. Int temp 184
> 2016-03-01 04:07:54: State: COLD. Burner power is 0. Int temp 193
> 2016-03-01 04:23:01: State: COLD. Burner power is 0. Int temp 189
> 2016-03-01 04:38:08: State: COLD. Burner power is 0. Int temp 185
> 2016-03-01 04:39:42: State: COLD. Burner power is 100. Int temp 181
> 2016-03-01 04:54:18: State: COLD. Burner power is 0. Int temp 187
> 2016-03-01 05:09:25: State: COLD. Burner power is 0. Int temp 189
> 2016-03-01 05:24:32: State: COLD. Burner power is 0. Int temp 186
> 2016-03-01 05:30:03: State: HOT. Burner power is 100. Int temp 186

� partir de 5h30 du matin on r�chauffe la maison � 21.5� afin de pr�parer le lever des occupants...

> 2016-03-01 05:45:10: State: HOT. Burner power is 100. Int temp 185
> 2016-03-01 06:00:17: State: HOT. Burner power is 100. Int temp 192
> 2016-03-01 06:15:24: State: HOT. Burner power is 100. Int temp 197
> 2016-03-01 06:30:32: State: HOT. Burner power is 100. Int temp 199
> 2016-03-01 06:45:39: State: HOT. Burner power is 100. Int temp 201

... au bout d'une heure, la temp�rature n'est pas compl�tement remont�e (on a fait environ 50% du r�chauffage n�cessaire). Cependant, � 7h00, la pond�ration des diff�rentes temp�ratures change, et la chambre se voit accorder moins d'importance. D'un coup, la temp�rature per�ue va donc augmenter :

> 2016-03-01 07:00:15: State: HOT. Burner power is 0. Int temp 222
> 2016-03-01 07:15:23: State: HOT. Burner power is 0. Int temp 221
> 2016-03-01 07:30:31: State: HOT. Burner power is 0. Int temp 217
> 2016-03-01 07:39:23: State: HOT. Burner power is 100. Int temp 212


# Interface utilisateur

## Bouton "j'ai froid"

La partie mat�rielle qui g�re la chaudi�re n'a pratiquement pas d'interface : elle est aliment�e directement par la chaudi�re, donc il n'y a pas de bouton on/off, et puisque les temp�ratures et �tats sont g�r�s logiciellement par un serveur sous Linux, il n'est pas possible de les modifier. J'ai tout de m�me rajout� un bouton, d�nomm� "j'ai froid". Lorsque vous appuyez sur ce bouton, le dispositif d�marrera le br�leur pendant 20 minutes, ce qui est en g�n�ral suffisant (sinon, il suffit d'appuyer � nouveau) pour fournir le confort manquant.

Ci-dessous des photographies du dispositif final (bien �videmment il est normalement cach� sous la chaudi�re, le bouton est accessible mais invisible.

![Montage final avec bouton "j'ai froid"](final.jpg)
![Dessous du bo�tier - les fils qui sortent servent pour la connexion � la chaudi�re](final_underside.jpg)


## Commande "j'ai froid"

Le script **heating_control.sh** est minimaliste et envoie simplement un paquet UDP au programme serveur, qui reconna�t les commandes **hot**, **cold**, et **away**. Une autre commande est **forceheating**, qui force le programme � activer le br�leur pendant 20 minutes. Cette commande intervient � un niveau diff�rent du bouton pr�c�demment d�crit, mais son effet est le m�me. Ma femme utilise le bouton car c'est le plus simple pour elle, mais quand j'ai froid et que je suis � mon bureau, j'utilise **forceheating**, cela m'�vite de me lever !

# Bilan

Ce montage est en fonctionnement depuis quelques mois et donne, � mon sens, de bons r�sultats. Il fait parfois trop froid ou trop chaud, mais c'est une cons�quence du chauffage central au gaz sans robinets thermostatiques. Cette technologie (ou, plus exactement, cette **absence** de technologie) ne permet pas un contr�le suffisamment fin pour avoir un confort optimal en toutes circonstances.

<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>

