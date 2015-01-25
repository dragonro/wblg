---
layout: post
title: �clairage d'escalier � LED avec d�tection de pr�sence
date: 2014-11-04 13:06:25
tags: electronics lighting
category: francais
comments: true
img_rel: "/~sven337/data/stairs"
---

<script type="text/javascript" src="//cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

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

Comme d'habitude pour la d�tection de pr�sence, j'utilise un PIR (**d�tecteur passif � infrarouge**) *�PasCher(TM)* [achet� sur eBay](http://www.ebay.com/itm/400330055400). Ce d�tecteur est tr�s sensible, ne co�te presque rien, et est tr�s facile � interfacer avec une �lectronique de contr�le quelle qu'elle soit.

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

Le ruban � LED se connecte directement au LEDNode avec des pins au format standard. Par contre, ce n'est pas souhaitable d'avoir l'�lectronique de puissance physiquement proche du ruban � LED, pour des raisons d'esth�tique&nbsp;: en effet le ruban est n�cessairement � un endroit o� il sera **visible** (m�me indirectement), alors qu'on souhaite que l'�lectronique de la solution soit invisible. J'ai donc fabriqu� une **rallonge** de 2 m�tres avec du c�ble que j'avais. Attention � utiliser des conducteurs suffisants pour transporter les courants importants qui vont circuler.
Le d�tecteur de pr�sence sera plac� de telle sorte que sa lentille de Fresnel puisse d�tecter correctement les gens. Ce d�tecteur a deux r�glages : un potentiom�tre pour la "port�e", dont la valeur peut faire varier la qualit� de la d�tection, et un potentiom�tre pour le d�lai d'allumage, qu'il convient de r�gler sur une valeur assez faible. On se moque en r�alit� de cette dur�e, puisqu'elle sera **g�r�e par le microcontr�leur** et non le capteur, mais plus elle est importante plus le capteur mettra du temps � re-d�tecter un mouvement apr�s avoir �t� enclench�. On reliera le d�tecteur sur un pin d'input du LEDNode.

## Programmation

Mon code est disponible sur [Github](https://github.com/sven337/jeenode/blob/master/stairs_light/stairs_light.ino). Quelques commentaires&nbsp;:

### Logique

Le principe g�n�ral est le suivant : lorsque l'Atmega d�tecte que le capteur envoie un **1** (correspondant � une pr�sence d�tect�e), et qu'il n'est pas d�j� en train d'ex�cuter une phase d'�clairage, le programme va d�marrer la s�quence d'�clairage, et prendre note de la "date" � laquelle la pr�sence a �t� d�tect�e.
Il allume �galement une LED de diagnostic dont la pr�sence simplifiera la phase de debug. Ma femme m'a fait retirer cette LED une fois l'installation termin�e.

La s�quence d'�clairage est compos�e d'une s�rie de "rampes" lin�aires qui seront ex�cut�es l'une apr�s l'autre, sachant qu'au bout de 15 secondes le programme va d�clencher la phase d'extinction et marquer la phase d'�clairage comme termin�e (permettant l'allumage � la prochaine d�tection de pr�sence).

### Syst�me de "rampes"

Je n'ai pas invent� ce concept. Une rampe est d�finie par une couleur cible (RGB), un temps pour l'atteindre (en millisecondes), et une autre rampe � ex�cuter ensuite. L'ex�cution de cette rampe correspond � un fondu lin�aire entre la couleur actuelle des LED et la couleur cible, sur la dur�e d�finie par la rampe.  Lorsque cette dur�e est atteinte, la rampe suivante est ex�cut�e automatiquement. Si on ne souhaite pas changer de rampe, il suffit de la faire boucler sur elle-m�me pour maintenir la couleur.

La toute premi�re rampe du programme est d�nomm�e *OFF* et est un peu sp�ciale : elle vise le noir complet en une milliseconde. C'est celle qu'on pourra utiliser pour une extinction d'urgence (par exemple en cas de souci thermique). Mon programme ne s'en sert que comme rampe d'initialisation. (Je n'ai d'ailleurs pas mis de s�curit� thermique, m�me si j'ai c�bl� une thermistance, vestige d'un montage pr�c�dent, car il n'y a aucun risque de surchauffe des transistors pour une dur�e d'allumage de quelques secondes par heure en moyenne.)

Je d�finis ensuite un certain nombre de rampes d'allumage (qui ont �t� modifi�es plusieurs fois pour obtenir l'effet d�sir�), et la m�me chose pour l'exctinction. Le choix des rampes a �t� fait en famille afin d'obtenir les couleurs souhait�es. On d�sire un allumage franc (afin d'�clairer suffisamment les marches), et une extinction lente.

Voici les rampes pour l'allumage :

```C
	// delay, R, G, B, next
	{    1, 0.02, 0.01, 0   , 2 }, //RISE_START
	{  200, 0.15, 0   , 0.08, RISE_END },
	{ 1000, 1   , 0.35, 0.08, RISE_END }, // RISE_END
```

Soit en bon fran�ais : 

1. atteindre la couleur $$R = 0.02, G = 0.01, B = 0.00$$ en 1 milliseconde.
1. atteindre la couleur $$R = 0.15, G = 0.00, B = 0.08$$ en 200 millisecondes.
1. atteindre la couleur $$R = 1.00, G = 0.35, B = 0.08$$ en 1000 millisecondes, et maintenir cette couleur


Et pour l'extinction :

```C
	{ 1000, 0.15, 0, 0.13, 9 }, // FADEOUT_START
	{ 1000, 0.15, 0, 0.08, FADEOUT_END },
	{ 4000, 0   , 0, 0   , FADEOUT_END }, // FADEOUT_END
```

1. atteindre la couleur $$R = 0.15, G = 0.00, B = 0.13$$ en 1 seconde.
1. atteindre la couleur $$R = 0.15, G = 0.00, B = 0.08$$ en 1 seconde.
1. s'�teindre compl�tement, progressivement, en 4 secondes


### Arithm�tique point fixe

Afin de calculer la bonne couleur � attribuer aux LED � un instant donn�, on doit calculer lors du d�marrage de la rampe le *delta* qui sera appliqu�. Les valeurs des LED sont un entier sur 8 bits, donc avec 256 valeurs discr�tes. � un pas de temps donn� (qui dans mon programme correspond empiriquement � 11ms), les LED varieront en g�n�ral d'une valeur inf�rieure � 1 : comment, d�s lors, calculer la valeur � attribuer � chaque LED ?
Sur un ordinateur on utiliserait des nombres � point flottant, sur un Arduino pour des raisons de performance mais surtout de simplicit� de programmation, l'arithm�tique � point fixe est un meilleur choix.
Le concept est le suivant : on va calculer toutes les valeurs avec des nombres entiers, mais en multipliant celles-ci par 1000. Ainsi, on pourra repr�senter les nombres d�cimaux jusqu'� 3 chiffres apr�s la virgule. Par exemple, si on souhaite additionner $$1 + 0.5 + 0.75$$, on va calculer $$1000 + 500 + 750 = 2250$$. **2250** est la repr�sentation de la valeur **2.25**, qu'on n'aurait pas obtenue en travaillant uniquement sur les parties enti�res.
Bien s�r, les ordinateurs travaillent en base 2, donc on ne va pas multiplier par une puissance de 10, mais par une puissance de 2. Cela ne change rien au concept de base.
On passe de la valeur � sa repr�sentation point fixe en d�calant d'un certain nombre de bits vers la gauche (multiplication par **2^N**), et de la repr�sentation point fixe � la valeur � envoyer aux LEDs en d�calant du m�me nombre de bits vers la droite (division euclidienne par **2^N**):

```C
	#define I2F(I) (I << 20)
	#define F2I(F) (F >> 20)
```

### Correction de luminosit�

Le signal envoy� par l'Arduino pour commander les LED est un PWM lin�aire. Or, une variation lin�aire du rapport cyclique ne se traduit pas par une variation lin�aire de la luminosit� per�ue par l'�tre humain, et c'est tr�s simple de s'en convaincre. En programmant une rampe de 0 � 255 sur plusieurs secondes, on voit tr�s facilement que pour les faibles valeurs, les variations sont tr�s perceptibles, alors que pour une luminosit� plus �lev�e il faut varier beaucoup plus l'intensit� pour percevoir une diff�rence � l'oeil.
De mani�re g�n�rale, la sensation est le logarithme de la stimulation : il faut corriger les valeurs de PWM de telle sorte que nos rampes lin�aires soient lin�aires *du point de vue de l'humain*. On va donc corriger les valeurs pour leur faire suivre une courbe de forme exponentielle, ce qui *lin�arisera* la rampe.
� noter que de nombreuses personnes sur Internet appliquent une correction gamma (bas�e sur une fonction puissance), ce qui me semble �tre incorrect mais que je n'ai pas creus�. De toute fa�on on ne recherche pas la perfection th�orique, mais simplement � avoir une forme *un peu meilleure* qu'un simple PWM lin�aire.

J'ai choisi la fonction exponentielle la plus simple qui associe l'intervalle $$[0;1]$$ � des valeurs dans $$[0;1]$$ :
$$
bri(in) = e^{\ln{2} * in} - 1
$$

$$in$$ repr�sente la valeur de PWM lin�aire, $$bri(in)$$ est la valeur corrig�e � utiliser pour piloter les LED.

Voici le graphe de la fonction, avec une fonction lin�aire pour comparer :

![Courbe de correction](bri_correction_1.jpg)

On constate que l'�cart est assez faible avec une �chelle lin�aire, et les r�sultats ne sont d'ailleurs pas tr�s convaincants. Il faudrait augmenter la courbure pour obtenir une meilleure correction.

[D'autres](http://jared.geek.nz/2013/feb/linear-led-pwm) [personnes](http://electronics.stackexchange.com/questions/1983/correcting-for-non-linear-brightness-in-leds-when-using-pwm) ont trait� le probl�me diff�remment.

## Int�gration

Ce projet a une dimension esth�tique importante. Par "chance", mon propri�taire a mal install� son escalier et n'a pas r�alis� les finitions qu'il aurait d� : il restait donc beaucoup de place sous l'escalier pour placer l'�lectronique, et j'ai utilis� un panneau de particules peint pour dissimuler le montage, avec un trou pour faire appara�tre le d�tecteur. 
J'ai fait quelques photographies.

![Vue d'en haut](lit_top.jpg)
![Vue d'en haut (2)](lit_top2.jpg)
![Vue d'en bas](lit_bottom.jpg)
![Installation du ruban � LED](ledstrip_view.jpg)
![Connecteur du ruban � LED](ledstrip_connector.jpg)
![Capteur de pr�sence](PIR.jpg)
![�lectronique](elec_top.jpg)
![Vue de c�t�](lit_side.jpg)
![Extinction progressive 1](fade_side_1.jpg)
![Extinction progressive 2](fade_side_2.jpg)

Voir �galement [une vid�o](/~sven337/data/stairs_light.avi) sur laquelle la puissance d'�clairage semble tr�s faible, en r�alit� il est assez puissant.

## R�sultat

Le r�sultat est excellent et approuv� par la famille et les amis. Le positionnement du capteur m'a un peu inqui�t� au d�but mais il est suffisamment sensible pour d�tecter la pr�sence � la mont�e comme � la descente. On s'habitue tellement vite � l'�clairage automatique qu'on souhaiterait l'avoir dans toute la maison !

<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>
