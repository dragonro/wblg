---
layout: post
title: �clairage d�coratif � LED avec d�tection de pr�sence
date: 2014-11-04 13:06:25
tags: electronics lighting
category: francais
comments: true
img_rel: "/~sven337/data/stairs"
---

Dans un [article pr�c�dent](/~sven337/francais/2014/08/03/Detection-de-presence-pour-eclairage-a-LED.html) j'abordais la d�tection de pr�sence pour un �clairage monochrome et tout ou rien.
Je pr�sente ici un montage plus complexe que j'ai r�alis� et install� dans mon escalier. Il s'agit d'un �clairage d'ambiance (et de s�curit�), en couleurs, reli� � un d�tecteur de pr�sence.

# Objectifs

On souhaite automatiser l'�clairage d'un escalier (ou couloir) tout en fournissant un effet lumineux plaisant. Le cahier des charges est le suivant :

- allumage automatique sur d�tection de pr�sence
- puissance d'�clairage suffisante pour ne pas avoir besoin d'utiliser une autre source lumineuse
- possibilit� de projeter plusieurs couleurs et de faire des fondus ("fade") entre elles
- bonne esth�tique du montage
- co�t r�duit

# Choix du mat�riel

Comme d'habitude pour la d�tection de pr�sence, j'utilise un PIR ( **d�tecteur passif � infrarouge**) *�PasCher(TM)* [achet� sur eBay](http://www.ebay.com/itm/400330055400). Ce d�tecteur est tr�s sensible, ne co�te presque rien, et est tr�s facile � interfacer avec une �lectronique de contr�le quelle qu'elle soit.

Afin d'obtenir un bel effet lumineux, uniforme sur la longueur � �clairer, le choix d'un **ruban � LEDs** RGB est naturel. J'ai choisi un ruban de LED SMD en 5050 (vendu pour 1,2A/m, m�me si en r�alit� la puissance est plus faible). Ces rubans sont contr�lables par des MOS � canal N : ils exposent un pin +, un R, un G, et un B. Dans ma r�alisation j'utilise 3 m�tres de ruban le long de mon escalier, et la puissance d'�clairage est suffisante pour bien voir les marches et ne plus avoir besoin de l'ampoule au plafond.

Pour r�aliser l'effet lumineux en lui m�me, il faudra des **transistors de puissance**, contr�l�s par une plateforme qui permet une **sortie "analogique"** PWM (pour la gradation lumineuse). J'ai choisi d'utiliser une plateforme compatible **Arduino**, dont la simplicit� de programmation et le bas co�t ne sont plus � d�montrer. Au lieu d'installer moi m�me les transistors, j'ai fait le choix d'un module tout int�gr� d�nomm� [**LEDNode**](http://www.digitalsmarties.net/products/led-node-v2), con�u par la m�me personne qui a cr�� le **[JeeNode](http://www.digitalsmarties.net/products/jeenode)** que j'utilise dans mes montages sur piles. Le prix du LEDNode est assez prohibitif, et si le montage �tait � refaire j'ach�terais moi-m�me les transistors de puissance. 

Il faudra pr�voir une **alimentation en 12VDC** capable de fournir la puissance demand�e par les LED. J'ai choisi une 6A, valeur ajustable � la baisse selon la longueur (et consommation r�elle) du ruban, en prenant en compte que la dur�e d'allumage sera de toute fa�on tr�s courte. N�anmoins les alimentations chinoises sont connues pour �tre �tiquett�es trompeusement, donc il est utile de prendre un peu de marge.

## Co�t

{:.CSSTableGenerator}
| �l�ment | Lien | Co�t |
| LED Node | <http://www.digitalsmarties.net/products/led-node-v2> | 27.50EUR |
| Ruban LED | [AliExpress](http://www.aliexpress.com/item/Best-13-02-5m-5m-300LED-IP65-waterproof-12V-SMD-5050-white-cold-white-warm/725423136.html) | 11.01EUR| 
| Alimentation | [AliExpress (un peu cher)](http://www.aliexpress.com/item/12V-6A-72W-AC-DC-Power-Adapter-Supply-Charger-for-3528-5050-RGB-LED-Strip-Light/587961982.html) | 11.70EUR | 
| D�tecteur de pr�sence | [eBay](http://www.ebay.com/itm/400330055400)| 1.51EUR | 
||||
| Total || **51.47EUR** |

Je pense qu'on pourrait diviser les co�ts par deux en utilisant un clone d'Arduino, des transistors de puissance peu chers, et une alimentation moins ch�re.

# R�alisation


## Connexions 

Le ruban � LED se connecte directement au LEDNode avec des pins au format standard. Par contre, ce n'est pas souhaitable d'avoir l'�lectronique de puissance physiquement proche du ruban � LED, pour des raisons d'esth�tique : en effet le ruban est n�cessairement � un endroit o� il sera **visible** (m�me indirectement), alors qu'on souhaite que l'�lectronique de la solution soit invisible. J'ai donc fabriqu� une **rallonge** de 2 m�tres avec du c�ble que j'avais. Attention � utiliser des conducteurs suffisants pour transporter les courants importants qui vont circuler.
Le d�tecteur de pr�sence sera plac� de telle sorte que sa lentille de Fresnel puisse d�tecter correctement les gens. Ce d�tecteur a deux r�glages : un potentiom�tre pour la "port�e", dont la valeur peut faire varier la qualit� de la d�tection, et un potentiom�tre pour le d�lai d'allumage, qu'il convient de r�gler sur une valeur assez faible. On se moque en r�alit� de cette dur�e, puisqu'elle sera **g�r�e par le microcontr�leur** et non le capteur, mais plus elle est importante plus le capteur mettra du temps � re-d�tecter un mouvement apr�s avoir �t� enclench�. On reliera le d�tecteur sur un pin d'input du LEDNode.

## Programmation

Mon code est disponible sur [Github](https://github.com/sven337/jeenode/blob/master/stairs_light/stairs_light.ino). Quelques commentaires:

### Logique

Le principe g�n�ral est le suivant : lorsque l'Atmega d�tecte que le capteur envoie un **1** (correspondant � une pr�sence d�tect�e), et qu'il n'est pas d�j� en train d'ex�cuter une phase d'�clairage, le programme va d�marrer la s�quence d'�clairage, et prendre note de la "date" � laquelle la pr�sence a �t� d�tect�e.
Il allume �galement une LED de diagnostic dont la pr�sence simplifiera la phase de debug. Ma femme m'a fait retirer cette LED une fois l'installation termin�e.

La s�quence d'�clairage est compos�e d'une s�rie de "rampes" lin�aires qui seront ex�cut�es l'une apr�s l'autre, sachant qu'au bout de 15 secondes le programme va d�clencher la phase d'extinction et marquer la phase d'�clairage comme termin�e (permettant l'allumage � la prochaine d�tection de pr�sence).

### Syst�me de "rampes"

Je n'ai pas invent� ce concept. Une rampe est d�finie par une couleur cible (RGB), un temps pour l'atteindre (en millisecondes), et une autre rampe � ex�cuter ensuite. L'ex�cution de cette rampe correspond � un fondu lin�aire entre la couleur actuelle des LED et la couleur cible, sur la dur�e d�finie par la rampe.  Lorsque cette dur�e est atteinte, la rampe suivante est ex�cut�e automatiquement. Si on ne souhaite pas changer de rampe, il suffit de la faire boucler sur elle-m�me pour maintenir la couleur.

La toute premi�re rampe du programme est d�nomm�e *OFF* et est un peu sp�ciale : elle vise le noir complet en une milliseconde. C'est cette qu'on pourra utiliser pour une extinction d'urgence (par exemple en cas de souci thermique). Mon programme ne s'en sert que comme rampe d'initialisation. (Je n'ai d'ailleurs pas mis de s�curit� thermique, m�me si j'ai c�bl� une thermistance, vestige d'un montage pr�c�dent, car il n'y a aucun risque de surchauffe des transistors pour une dur�e d'allumage de quelques secondes par heure en moyenne.

Je d�finis ensuite un certain nombre de rampes d'allumage (qui ont �t� modifi�es plusieurs fois pour obtenir l'effet d�sir�), et la m�me chose pour l'exctinction. Le choix des rampes a �t� fait en famille afin d'obtenir les couleurs souhait�es. On d�sire un allumage franc (afin d'�clairer suffisamment les marches), et une extinction lente.

### Arithm�tique point fixe

Afin de calculer la bonne couleur � attribuer aux LED � un instant donn�, on doit calculer lors du d�marrage de la rampe le *delta* qui sera appliqu�. Les valeurs des LED sont un entier sur 8 bits, donc avec 256 valeurs discr�tes. � un pas de temps donn� (qui dans mon programme correspond empiriquement � 11ms), les LED varieront en g�n�ral d'une valeur inf�rieure � 1 : comment, d�s lors, calculer la valeur � attribuer � chaque LED ?
Sur un ordinateur on utiliserait des nombres � point flottant, sur un Arduino pour des raisons de performance mais surtout de simplicit� de programmation, l'arithm�tique � point fixe est un meilleur choix.
Le concept est le suivant : on va calculer toutes les valeurs avec des nombres entiers, mais en multipliant celles-ci par 1000. Ainsi, on pourra repr�senter les nombres d�cimaux jusqu'� 3 chiffres apr�s la virgule. Par exemple, si on souhaite additionner 1 + 0.5 + 0.75, on va calculer 1000 + 0500 + 0750 = 2250. 2250 est la repr�sentation de la valeur 2.25, qu'on n'aurait pas obtenue en travaillant uniquement sur les parties enti�res.
Bien s�r, les ordinateurs travaillent en base 2, donc on ne va pas multiplier par une puissance de 10, mais par une puissance de 2. Cela ne change rien au concept de base.
On passe de la valeur � sa repr�sentation point fixe en d�calant d'un certain nombre de bits vers la gauche (multiplication par **2^N**), et de la repr�sentation point fixe � la valeur � envoyer aux LEDs en d�calant du m�me nombre de bits vers la droite (division euclidienne par **2^N**).

### Correction de luminosit�

Le signal envoy� par l'Arduino pour commander les LED est un PWM lin�aire. Or, une variation lin�aire du rapport cyclique ne se traduit pas par une variation lin�aire de la luminosit� per�ue par l'�tre humain, et c'est tr�s simple de s'en convaincre. En programmant une rampe de 0 � 255 sur plusieurs secondes, on voit tr�s facilement que pour les faibles valeurs, les variations sont tr�s perceptibles, alors que pour une luminosit� plus �lev�e il faut varier beaucoup plus l'intensit� pour percevoir une diff�rence � l'oeil.
C'est l'adage "la sensation est le logarithme de la stimulation" : il faut corriger les valeurs de PWM de telle sorte que nos rampes lin�aires soient lin�aires *du point de vue de l'humain*. En pratique il faudra donc les rendre exponentielles, et j'ai �crit une fonction qui corrige les valeurs afin de les rapprocher d'une �chelle per�ue comme lin�aire.
� noter que de nombreuses personnes sur Internet appliquent une correction gamma (bas�e sur une fonction puissance), ce qui me semble �tre incorrect mais que je n'ai pas creus�. De toute fa�on on ne recherche pas la correction th�orique, mais simplement � avoir une rampe *un peu meilleure* que ce qu'on obtient sans correction.

## Int�gration

Ce projet a une dimension esth�tique tr�s s�rieuse. Par chance, mon propri�taire a mal install� son escalier et n'a pas r�alis� les finitions qu'il aurait d� : il restait donc beaucoup de place sous l'escalier pour placer l'�lectronique, et j'ai utilis� un panneau de particules peint pour dissimuler le montage, avec un trou pour faire appara�tre le d�tecteur. 
J'ai fait quelques photographies.

## R�sultat

Le r�sultat est excellent et approuv� par la famille et les amis. Le positionnement du capteur m'a un peu inqui�t� au d�but mais il est suffisamment sensible pour d�tecter la pr�sence � la mont�e comme � la descente. On s'habitue tellement vite � l'�clairage automatique qu'on souhaiterait l'avoir dans toute la maison !
