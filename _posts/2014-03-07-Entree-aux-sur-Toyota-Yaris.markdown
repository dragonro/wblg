---
layout: post
title:  "DIY - Rajouter une entr�e AUX sur Yaris 2007"
date:   2014-03-07 21:16:55
categories: francais
---

# Objectif

Rajouter une **entr�e auxiliaire** sur une **Yaris** de **2007** achet�e en
France. Le v�hicule utilise un autoradio **W58824** fabriqu� par Panasonic
sous la r�f�rence **CQ-TS0570LC** (exclusif � Toyota).

J'ai �crit un article en anglais � ce sujet, avant de r�aliser que les
personnes concern�es seraient probablement plut�t francophones.

Cet article en est un r�sum�.

# Mes essais n'ont pas fonctionn�

J'ai r�alis� trois tentatives diff�rentes qui n'ont pas fonctionn�. Les Yaris
am�ricaines disposent, sur le connecteur � l'arri�re de l'autoradio, d'une
entr�e auxiliaire. Celle-ci n'est bien souvent pas c�bl�e dans le v�hicule,
mais elle existe n�anmoins, et il suffit d'y connecter 3 fils pour pouvoir
s'en servir. C'est ce que r�alisent des objets tels que celui-ci:
![](DIY%20-%20Rajouter%20une%20entr%C3%A9e%20AUX%20sur%20Yaris%202007_files
/CQ-TS0570LC.html) (ou, si cela vous chante, la r�f�rence Toyota officielle
qui vous co�tera de l'ordre de 50 euros). Ensuite, un simple appui sur le
bouton AUX (ou DISP, ou CD, ... selon le mod�le exact qui n'est _pas_ W58824) et
l'entr�e fonctionne.

Nous avons moins de chance en France, car si le m�me connecteur est bien
pr�sent au dos de l'autoradio, il est utilis� � des fins diff�rentes. Sur ma
voiture, seules 3 des 20 pattes de ce connecteur sont utilis�es, et c'est pour
la commande au volant.

# Quelles sont les options restantes ?

Il y a **trois possibilit�s** pour rajouter une entr�e auxiliaire :

1.**�muler un changeur de CD**

2.**injecter l'entr�e auxiliaire sur la sortie du lecteur CD**

3.**injecter l'entr�e auxiliaire sur l'entr�e de l'amplificateur**

�muler un changer de CD est plus compliqu� qu'il n'y para�t. En effet, les
donn�es voyagent sous forme analogique compatible directement avec le c�ble
jack 3.5mm que nous souhaitons ajouter, mais l'autoradio est bas� sur **AVC-
LAN**, un protocole r�seau made in Toyota dont le principal but semble �tre de
compliquer les manipulations telles que celle que nous tentons ici.

Il faut donc de l'�lectronique sp�cifique pour parler � l'autoradio en AVC-
LAN, afin de lui faire croire qu'un changeur de CD est pr�sent, cela pour
qu''il accepte de lire les donn�es analogiques qui lui sont envoy�es sur la
prise changeur CD. Voir [http://www.softservice.com.pl/corolla/avc/avclan.php]
(http://www.softservice.com.pl/corolla/avc/avclan.php), en anglais, pour plus
d'informations.

Certains appareils se trouvent sur AliExpress ou eBay, qui permettent d'�muler
un changeur de CD. Ils sont relativement on�reux (compter plus de 50 euros)
car ils sont intrins�quement complexes. On verra � la fin de cet article que,
la mort dans l'�me, je suis contraint de recommander cette solution (quoique
ne l'ayant pas test�e moi m�me).

Les options **2** et **3** imposent de d�monter l'appareil.

# D�monter l'appareil

D�monter l'appareil est compliqu�, notamment parce que Panasonic a choisi
d'utiliser des clips en plastique dur, du genre de ceux qui se cassent si vous
les regardez de travers. J'esp�re que UFC-Que choisir se d�cidera � faire un
�norme proc�s � l'inventeur de ces clips.

## Carte �lectronique

La plupart des puces utilis�es semblent �tre � usage interne Panasonic, et je
n'ai pu trouver aucune _datasheet_ sur Internet !

Un manuel de r�paration d'un mod�le proche m'a cependant aid� un petit peu.

## Sortie du lecteur CD

La sortie du lecteur CD dispose de 14 pins, donc trois nous int�ressent -
**Left**, **AGND**, **Right**. Se brancher dessus est difficile car les pistes
sur le PCB sont minuscules, et celui-ci est copieusement verni ce qui
complique d'autant la soudure.

Ceci ne fonctionne pas : en effet j'ai observ� que lorsque je branchais mon
lecteur MP3 � travers la prise jack, pour injecter le signal sur ces trois
pins, le son se coupait compl�tement, comme si le signal du CD �tait "�cras�".
Par contre, ce n'est pas pour autant que j'ai entendu le son de mon lecteur
MP3...

## Entr�e de l'amplificateur

Si vous ne parvenez pas � identifier l'amplificateur sur la carte en 15
secondes, merci de tout remonter et vous faire un peu la main sur de
l'�lectronique avant de jouer � modifier un autoradio. Les erreurs co�tent
cher sur ce type d'appareil.

En se branchant sur l'entr�e de l'amplificateur (5 pins au milieu du chip,
**Front**L**eft/**F**ront**R**ight/**R**ear**L**eft/**Re**ar**R**ight et
**GND**), on peut tenter d'injecter du son, ind�pendamment du mode CD ou radio
de l'appareil.

Une fois de plus, si j'ai bien r�ussi avec mon c�ble � "tuer" le son qui
sortait, je n'ai pas r�ussi � entendre celui de mon lecteur.

J'ai r�alis� apr�s coup que l'appareil utilise un pr�-amplificateur - il est
donc probable que le niveau du signal n'�tait pas bon.

# Travail � venir

Les connexions sont dures � r�aliser de mani�re fiable et solide, et un
connecteur AUX ne vaut pas les neuf heures de travail que j'ai consacr� � ce
projet.

Un essai d'injection sur le pr�-amplificateur me semble �tre la meilleure
piste, mais la r�alisation d'un �mulateur de changeur CD (lien plus haut), ou
l'achat d'un tel appareil - qui fournit souvent par ailleurs des
fonctionnalit�s utiles comme un port USB - est probablement le choix le plus
simple.

Le co�t n'est bien s�r pas nul.

Si vous �tes int�ress� pour continuer les exp�rimentations, je serais tr�s
int�ress� de conna�tre vos r�sultats ! Contactez-moi et je les mettrai en
ligne.

Sven337

  
e5d0ceda@opayq.com

