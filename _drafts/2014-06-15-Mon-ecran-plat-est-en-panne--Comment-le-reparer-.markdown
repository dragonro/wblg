---
layout: post
title: Mon �cran plat est en panne ! Comment le r�parer ?
date: 2014-06-15 17:40:10
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/lcdfix"
---

Dans cet article je vais expliquer comment r�parer un �cran plat (t�l�vision, �cran d'ordinateur) en panne.
Je l'�cris car s'il existe de nombreuses ressources sur Internet (souvent en anglais), je n'ai pas trouv� d'article de synth�se accessible � des d�butants ou des personnes de niveau interm�diaire.

Dans un premier temps nous regarderons ensemble l'anatomie d'un �cran plat, afin d'identifier les grandes briques fonctionnelles. Dans un deuxi�me temps nous verrons, pour les briques couramment responsables de la panne, comment les diagnostiquer, identifier le composant d�fectueux, et proc�der au remplacement.

# Anatomie d'un �cran plat

Un �cran plat est constitu� des composants suivants :

* la dalle, �l�ment responsable de l'affichage
* les lampes, en g�n�ral des tubes fluorescents (comme les "n�ons" dans les bureaux), qui assurent le r�tro�clairage
* l'�lectronique de contr�le, qui assure l'envoi des donn�es � l'�cran, r�agit � la pression sur les boutons, ...
* l'�lectronique de puissance, c'est-�-dire l'alimentation

95% des pannes proviennent de l'alimentation. Ironiquement, c'est cette brique fonctionnelle qui est la moins "high tech", �tant bas�e sur des technologies connues depuis des ann�es, sans rien d'innovant dedans. L'essentiel de l'innovation est situ� dans la dalle (nouvelles matrices avec meilleur contraste, meilleur angle de vision, r�action plus rapide...), et l'�lectronique de contr�le.

## La dalle

Cet �l�ment est celui qui est r�ellement responsable de l'affichage. C'est une grande plaque, noire, rarement accessible facilement. Elle est prot�g�e devant l'�cran par un mat�riau transparent qui encaisse l'essentiel des coups et rayures. La dalle est en g�n�ral l'�l�ment qui flanche dans le cas d'une casse : lors d'une chute, par exemple d'un ordinateur portable, la dalle est susceptible de casser.

La dalle n'est jamais r�parable. Une fois cass�e, seul un remplacement complet de ladite permet de r�parer l'�cran. Un tel remplacement n'est en g�n�ral �conomiquement pas rentable : � neuf, il est impossible, et en occasion c'est rare de pouvoir trouver une dalle � un prix acceptable... mais dans le cas d'un ordinateur portable o� l'�cran n'est pas rempla�able, il n'y a gu�re le choix. N'achetez pas d'ordinateur portable.

## Les lampes

De nouvelles technologies apparaissent mais � l'heure o� j'�cris ces lignes la plus courante, en particulier sur les �crans de plus de 4 ans qui sont les plus susceptibles d'�tre concern�s par cet article, est le tube fluorescent. C'est la m�me technologie que vous avez dans les lampes � �conomie d'�nergie et dans les "n�ons" pr�sents dans les immeubles collectifs.

Un gaz est plac� dans une ampoule scell�e, et excit� par un **courant alternatif** de fr�quence assez �lev�e, d'intensit� assez faible, sous une tension d'environ **700V** (parfois plus). Lorsque ce gaz est excit� il �met de la lumi�re, qui n'est pas forc�ment blanche, mais intercept�e par un rev�tement de surface sur l'ampoule qui la transforme en lumi�re (plus ou moins blanche).

Les pannes arrivent parfois, toutefois je n'en ai jamais observ� personnellement. J'imagine qu'il existe deux modes de panne : soit la panne "circuit ouvert, pas de lumi�re", qui correspondra � une lampe grill�e (dans ce cas vous ne verrez pas une partie de l'image � moins de vous approcher tr�s, tr�s pr�s), soit la panne "court-circuit", qui vous garantit au minimum la mise en protection de l'alimentation (donc pas de lumi�re sur aucune des lampes), quand ce n'est pas le d�c�s "luminothermique" de ladite alimentation. 

## L'�lectronique de contr�le

Celle-ci est parfois r�partie sur plusieurs cartes �lectroniques au dos de l'�cran, selon la conception. Cette carte est assez facile � identifier : c'est la plus complexe, qui abrite des puces �lectroniques de taille assez importante, parfois �quip�es d'un radiateur. Le PCB (*printed circuit board*, c'est-�-dire le circuit imprim�) est en g�n�ral vert ou bleu, double face, multicouches et recouvert de composants miniaturis�s.
Cette �lectronique est responsable, de mani�re non exhaustive, de :

- envoyer les donn�es � la dalle 
- d�coder le signal radio (TV TNT = DVB-T, FM, ...) le cas �ch�ant
- d�coder le signal HDMI, VGA, ... le cas �ch�ant
- r�pondre aux appuis sur les boutons et la t�l�commande
- changer de cha�ne, augmenter le volume sonore, ...

Il est assez rare que toutes ces fonctionnalit�s soient int�gr�es sur une seule puce. L'amplificateur audio, le tuner radio, le contr�leur de dalle... sont en g�n�ral chacun sur leur propre puce, cela pour des raisons industrielles. Lorsque vous concevez une t�l�vision, celle-ci ne va probablement pas innover dans le domaine de l'amplification audio, et il est moins co�teux de prendre une puce d'amplification existante, que d'int�grer une logique d'amplification au sein du circuit que vous concevez pour cette nouvelle t�l�vision. 

On trouve bien s�r de la m�moire vive (RAM) et souvent de la m�moire de masse (flash) sur la ou les cartes de l'�lectronique de contr�le.

Les pannes sont rares au niveau de cette �lectronique, ce qui est paradoxal consid�rant que c'est la partie de l'�cran qui a co�t� le plus cher � concevoir (apr�s la dalle, mais ce n'est pas vraiment de l'�lectronique donc je l'ignore). Cela est tr�s heureux car les r�parations seraient pratiquement impossibles, d'une part car les composants susceptibles de tomber en panne (les puces) sont introuvables, d'autre part car m�me si vous pouviez les trouver vous ne pourriez pas les remplacer sans mat�riel professionnel et qualifications sp�cifiques (changer une puce en BGA, c'est un m�tier).

## L'alimentation

Nous allons, si vous ne l'aviez pas devin�, nous attarder longuement sur l'alimentation. En effet celle-ci est responsable de la majorit� des pannes d'�crans plats, ce que je trouve par ailleurs scandaleux car les pannes sont st�r�otyp�es, et �vitables � condition de concevoir correctement la thermique (ce qui semble n'�tre jamais fait, mais c'est le probl�me n�1 des alimentations �lectroniques grand public en 2014).

Je vais rentrer de plus en plus dans les d�tails, et je vous invite � sauter des paragraphes si les d�tails d�passent votre niveau en �lectronique. Vous n'aurez pas besoin de ces connaissances th�oriques pour d�panner votre �cran (plus bas).

L'alimentation est responsable de deux choses :

* alimenter l'�lectronique de contr�le (en g�n�ral **5V DC**)
* alimenter les lampes (en g�n�ral **700V AC**)

Ces deux responsabilit�s semblent tr�s diff�rentes, mais en r�alit� la deuxi�me est construite en rajoutant une brique par dessus la premi�re. L'alimentation est syst�matiquement structur�e sous la forme d'une alimentation � d�coupage g�n�rant un courant continu - d'une part le **5V DC** n�cessaire pour l'�lectronique de contr�le, d'autre part une tension plus �lev�e telle que **12V** ou **24V** DC. Cette tension plus �lev�e est ensuite fournie � un onduleur (en anglais *inverter*, vous verrez ce terme r�guli�rement utilis�, parfois � mauvais escient avec des auteurs qui assimilent l'int�gralit� de l'alimentation � un *inverter* alors que celui-ci n'en repr�sente que la moiti�).

Il arrive que l'onduleur soit d�port� sur une carte s�par�e, dans ce cas con�ue par d'autres ing�nieurs voire d'autres soci�t�s. (Ce qui vous donne d'ailleurs � r�fl�chir sur le niveau de comp�tence du concepteur de votre t�l�vision, qui se contente en fait d'assembler des composants qu'il n'a pas con�us ni fabriqu�s. Et on s'�tonne que �a tombe en panne.)

### Alimentation DC ###

Si vous �tes curieux je vous recommande chaudement la lecteur de l'article suivant, qui explique tr�s en d�tail comment fonctionne une alimentation d'ordinateur. La partie AC->DC de l'alimentation d'un �cran plat est tr�s similaire, voire identique (pr�sence d'un PFC actif) sur les "fortes" puissances.

[http://www.tomshardware.fr/articles/Fonctionnement-d-une-alimentation-1ere-partie,2-265.html](http://www.tomshardware.fr/articles/Fonctionnement-d-une-alimentation-1ere-partie,2-265.html)

On trouve parfois, sur cette partie de l'alimentation, un PFC actif. En g�n�ral un PFC passif est pr�sent (voire pas de PFC du tout, ce qui il me semble n'est autoris� en Union Europ�enne qu'en dessous d'une valeur de puissance tellement faible que cela ne s'applique probablement qu'aux �crans 17").
On a bien s�r un pont de redressement, des condensateurs d'entr�e, des transistors de d�coupage, un transformateur, des diodes de redressement, et des condensateurs de filtrage en sortie. La cause de panne la plus courante sur cette partie est la d�gradation (�ge et temp�ratures) des condensateurs �lectrolytiques de filtrage en sortie. Nous y reviendrons.

### Onduleur ###

Cet �tage est en g�n�ral attaqu� par une tension un peu plus �lev�e que les 3.3V ou 5V de l'�lectronique de contr�le. En effet les lampes consomment une puissance non n�gligeable (surtout sur des diagonales importantes), et puisqu'il faudra d�couper apr�s et �lever la tension, autant se dispenser de faire circuler de trop gros courants. Sur un �cran d'ordinateur de 17", l'alimentation DC attaquait l'onduleur avec **12V DC**, alors que sur une t�l�vision 37" dont je montrerai quelques photos plus loin, c'�tait du **24V DC** (avec tout de m�me **6A** de courant nominal).

Des condensateurs d'entr�e sont pr�sents, et la tension est d�coup�e et fournie � un transformateur par lampe (4 lampes, 4 transformateurs). Ce transformateur dispose d'un ratio important qui g�n�re une tension d'environ **1kV AC**. (Je n'ai pas mesur� tr�s pr�cis�ment). 
En g�n�ral sur l'onduleur, la panne vient des transistors de d�coupage, syst�matiquement sous-dimensionn�s, mais parfois �galement de leur driver (sachant que la mort du transistor entra�ne souvent la mort du driver).

# R�parer l'�cran plat

Je m'int�resse ici strictement aux probl�mes d'alimentations, qui sont majoritaires.

## S�CURIT�

Avant toute chose, il me faut aborder la question de la s�curit�. R�parer une alimentation �lectronique est une activit� dangereuse. Une fausse manipulation peut vous blesser, voire th�oriquement vous tuer. M�me une fois d�branch�e, une alimentation (c'est d'ailleurs valable pour les circuits de flash des appareils photos) peut rester dangereuse pendant plusieurs minutes. Le risque "r�aliste" est celui d'une br�lure grave � un doigt, et d'une douleur que je suis fier d'�tre incapable de d�crire car je ne l'ai jamais exp�riment�e. Le risque l�thal n'est n�anmoins pas exclu, car les condensateurs d'entr�e stockent une �nergie qui est th�oriquement suffisante pour vous tuer, si le courant venait � traverser votre coeur.

### Hors tension

Hors tension, c'est-�-dire d�branch�e de la prise (et non seulement avec l'interrupteur ouvert, car celui-ci commute normalement la phase, mais vous ne savez pas vraiment si la phase est � gauche ou � droite chez vous), la seule source de danger d'une alimentation provient de son stockage d'�nergie � haute tension, c'est-�-dire le ou les condensateurs �lectrolytiques qui sont situ�es � l'entr�e. Ceux-ci sont en effet charg�s � une tension d'environ **400V**, et disposent d'une quantit� d'�nergie susceptible de vous br�ler ou de d�polariser votre coeur. En g�n�ral, une r�sistance de d�charge de forte valeur est pr�sente (encore que je ne l'ai pas vue sur la derni�re alimentation que j'ai r�par�, mais peut-�tre �tait elle int�gr�e dans le *driver*), qui permet de vider progressivement le condensateur. 
Apr�s la mise hors tension je recommande d'attendre au moins une minute avant de toucher n'importe quel composant. Ensuite, une v�rification au voltm�tre aux bornes des condensateurs d'entr�e permettra de v�rifier que la tension � leur borne est inf�rieure � **30V**. � ce stade, l'alimentation est inoffensive et vous pouvez la toucher (avec les doigts, mais pas avec la langue !) sans crainte, du c�t� primaire (avant le transformateur) comme du c�t� du secondaire.
�vitez d'avoir les mains qui tremblent lors de votre mesure sur les condensateurs, et ne n�gligez pas le d�lai d'attente. Si vos mains tremblent, vous risquez de court-circuiter le condensateur avec les sondes du multim�tres, imposant une d�charge rapide qui fera des �tincelles et d�truira le condensateur. �a fait de la lumi�re, du bruit, �a fait peur, et �a peut faire mal. Pas id�al.

Le d�lai d'attente vise � vous �viter de mesurer des tensions importantes. Mesurer une tension de 300V ne pose pas de vrai probl�me et peut se faire en s�curit� avec la plupart des multim�tres, mais pour un multim�tres premier prix avec des sondes premier prix il est raisonnable de s'interroger sur la qualit� de l'isolation. En plus, faire la mesure avec vos deux mains met le coeur en plein dans le passage. 

### Sous tension

C'est malheureux, mais la plupart des diagnostics ne sont pas r�alisables hors tension, car il faut bien regarder l'alimentation fonctionner pour voir � quel endroit elle ne fonctionne pr�cis�ment pas.

#### Alimentation DC

Au secondaire de l'alimentation DC ne sont pr�sentes que des tensions faibles qui ne pr�sentent pas de danger particulier. Ne manipulez rien avec des mains mouill�es, faites attention � votre outillage, mais vous pouvez toucher n'importe quel point au secondaire sans trop de danger. Si par hasard l'alimentation fournit une tension sup�rieure � 24V je recommande d'�tre prudent. 
Au primaire de l'alimentation DC, **tous les points sont � un potentiel du secteur**. Ne touchez rien avec les doigts, m�me pas un radiateur ou la carte nue, et faites le moins possible de mesures au voltm�tre de ce c�t� l� (pour �viter les accidents).

#### Onduleur (alimentation DC->AC)

Pour l'onduleur, c'est l'inverse (� supposer qu'il soit sur une carte s�par�e) : le primaire ne pr�sente pas de danger particulier, mais le secondaire vous pr�sente plus de **500V** AC, avec un courant qui peut atteindre plusieurs centaines de milliamp�res. De plus vous risquez de vous retrouver en incapacit� physique (par t�tanisation des muscles) de rel�cher. **Le secondaire de l'onduleur ne doit pas �tre touch� sous tension !** (Hors tension, par contre, il n'est pas dangereux et ce sans d�lai d'attente car il n'y a pas de condensateur de stockage � haute tension, puisque le secondaire est enti�rement AC.)

Attention aux limites de votre multim�tre, un multim�tre � Pas Cher(TM) n'a en g�n�ral pas une isolation lui permettant de mesurer la tension pr�sent�e aux lampes. �a ne m'a pas emp�ch� de le faire quelques fois mais ce n'est pas une tr�s bonne id�e. 

## D�terminer quel �l�ment est responsable

Maintenant que vous tremblez de peur, nous allons commencer par quelques manipulations sans risque !

L'alimentation est en g�n�ral responsable du probl�me, mais nous allons tenter de le v�rifier. Les sympt�mes d'une panne d'alimentation sont les suivants :

### Le mode "veille" de la t�l�vision fonctionne (LED t�moin allum�e), mais lorsque vous d�marrez la t�l�vision, rien ne se passe

Panne probable sur l'alimentation DC ou l'onduleur.

### M�me chose, mais l'image appara�t un instant et s'�teint imm�diatement

Panne probable sur l'alimentation DC ou l'onduleur.
 
### M�me chose, mais l'image appara�t pour de bon, sauf qu'elle "vibre", est sous ou sur dimensionn�e, ...

Panne probable sur l'alimentation DC.

### le mode "veille" de la t�l�vision ne fonctionne m�me pas (pas de LED t�moin) -> cas rare

Panne probable sur l'alimentation DC.

### Son OK mais pas d'image

Si le son est OK mais l'image n'est pas pr�sente, v�rifiez si ce n'est pas le r�tro�clairage qui est en panne (cas courant). Pour cela, collez vous � l'�cran, vous devriez voir l'image tr�s sombre et sans contraste : panne de r�tro�clairage.
Si pas d'image du tout, panne de la dalle ou de l'�lectronique de contr�le, votre appareil n'est pas r�parable.

Pour une panne du r�tro�clairage, dans le cas pr�sent on soup�onnera d'abord les lampes, car dans le cas o� l'onduleur a un probl�me il est courant qu'il se mette en court-circuit (ce qui coupe l'alimentation DC et donc tout l'appareil). 


Les cas qui ne sont probablement *pas* des probl�mes d'alimentation sont les suivants :

- mauvaise r�ception TNT
- pixels morts ou coinc�s
- entr�e (HDMI, VGA, ...) qui ne fonctionne plus
- image OK mais pas de son

## Trouver la panne sur l'alimentation DC

La panne provient � 90% des condensateurs �lectrolytiques en sortie. Dans une alimentation, ces condensateurs sont pratiquement des pi�ces d'usure. Un condensateur �lectrolytique, c'est un cylindre en aluminium, scell�, qui contient un �lectrolyte liquide. Lorsque le condensateur chauffe (parce qu'il est soumis � une temp�rature �lev�e, courant car les diodes de redressement en sortie s'�chauffent, ou parce qu'un gros courant le traverse), l'�lectrolyte se dilate, et il arrive qu'il fuie. Vous voyez cela si le sommet du condensateur (l� o� une croix m�tallique est destin�e, pr�cis�ment pour permettre l'expansion sans explosion) est bomb�, ou s'il a fui.
C'est une panne tr�s courante et plut�t bien d�crite sur Internet, par exemple dans [cet article](http://www.tomshardware.fr/articles/Reparer-carte-mere,2-434-4.html).

Ne vous inqui�tez pas � propos des condensateurs en entr�e (les tr�s gros, en g�n�ral marqu�s 400V) : ceux l� chauffent peu et ne s'usent donc pratiquement pas. Seuls les condensateurs �lectrolytiques (cylindriques de gros volume) situ�s en sortie sont r�ellement susceptible d'�tre "us�s".

Parfois, les condensateurs de sortie ne sont pas visiblement bomb�s. Cela ne permet pas pour autant de les mettre hors de cause (et sur une alimentation d'�cran que j'ai r�par�e, rien ne permettait de voir que ces condensateurs devaient �tre chang�s).
Pour v�rifier qu'ils sont vraiment en cause, l'id�al serait de brancher un oscilloscope sur la sortie de l'alimentation et de regarder si la tension est stable. Si vous poss�dez un oscilloscope vous n'�tes probablement pas en train de lire mon article :)

Mon approche consiste � brancher le voltm�tre sur la sortie et � d�marrer l'�cran (*avec* une charge, c'est-�-dire avec la carte �lectronique branch�e sur l'alimentation), et � regarder si la tension de sortie varie. Si le condensateur est en mauvais �tat vous arriverez souvent � voir une baisse sur le voltm�tre (par exemple, 3.8V au lieu de 5V). C'�tait mon cas sur un �cran que j'ai r�par� o� tout fonctionnait, mais l'image �tait d�form�e, et clignotait, signe que l'�lectronique de contr�le "plantait" mais sans compl�tement se bloquait.
� noter qu'en g�n�ral la chaleur tend � r�duire les probl�mes des condensateurs d�fectueux : parfois, lorsqu'ils sont chauds (certains les attaquent au s�che-cheveux), le probl�me peut dispara�tre. C'est un signe de plus que les condensateurs de sortie sont � changer.

Ce changement n'est pas une op�ration tr�s difficile, les composants �tant faciles � trouver et � changer. Le co�t en revanche peut �tre cons�quent.

## Trouver la panne sur l'onduleur

Sur l'onduleur, la cause courante de la panne sera les transistors de d�coupage (qui sont parfois nombreux, �tant donn� qu'on trouve en g�n�ral plusieurs transformateurs chacun attaqu� par deux transistors). Ceux-ci sont souvent sous-dimensionn�s et mal refroidis, et se mettent en court-circuit. Ils ne jouent plus leur r�le de "robinet" pour le courant et deviennent constamment passants, ce qui d�truit souvent la puce *driver* et cr�e un court-circuit � l'entr�e de l'onduleur (auquel l'alimentation DC r�agit en coupant tout).

Pour tester les transistors, j'ai une technique tr�s approximative qui fonctionne n�anmoins plut�t bien. Identifiez (� l'aide de la *datasheet* obtenue sur *Google* � partir des marquages pr�sents sur le transistor) quel pin correspond � quelle fonction - en g�n�ral sur les MOSFET de surface l'ordre est gate, drain, source, et v�rifiez avec un ohmm�tre la r�sistance entre le drain et la source, avec le + sur le drain, et le - sur la source (imp�ratif). Vous devriez trouver une r�sistance infinie, ou du moins tr�s grande. Si vous trouvez 0, il y a de tr�s fortes chances que le transistor soit en court-circuit. Dans ce cas il est souhaitable de le d�ssouder (si possible sans le d�truite) et de le tester hors du circuit pour confirmer.

Pour tester le driver, je ne connais pas de m�thode. Si un des transistors est en court-circuit il est pr�f�rable de changer le driver qui aura possiblement �t� d�truit lorsque le transistor s'est mis en court-circuit, mais je me suis d�j� content� de ne changer que le transistor et cela fonctionnait tr�s bien. Pour moi, le remplacement du driver va surtout d�pendre de contraintes �conomiques et techniques : est-il possible de le trouver � bas co�t en quantit� raisonnable (car la bobine de 5000 ne vous int�resse probablement pas) ? Est-il possible de le changer avec votre fer � souder sans d�truire toute la carte ?

## Se procurer les composants

Voir [mon article sur les achats sur Internet](http://perso.aquilenet.fr/~sven337/francais/2014/06/02/Acheter-des-composants-electroniques-sur-Internet.html) pour comprendre o� je me fournis.

En g�n�ral et pour les pannes d�crites dans cet article, le meilleur choix est ebay.com (ne faites pas vos recherches en fran�ais, c'est inutile).

### Les condensateurs

Pour les condensateurs, rep�rez : 

- leur capacit� (en g�n�ral de l'ordre de 500 micro-farads)
- leur tension de service (� peu pr�s le double de la tension nominale de sortie)
- si vous avez un pied � coulisse, leur dimension

Le crit�re est de trouver un condensateur de m�me capacit�, de tension de service sup�rieure ou �gale, et de dimensions compatibles avec l'espace disponible sur la carte (souvent, c'est serr� !). En g�n�ral, vous aurez besoin d'un condensateur marqu� "low ESR" (*equivalent series resistance*, un crit�re secondaire mais important).

Certains sites vous recommandent de prendre une grande marque de condensateurs, par exemple Panasonic. Le probl�me est que leur co�t est parfois prohibitif pour r�parer du mat�riel usag�. J'ai tendance � penser qu'un achat de condensateur dans les bons crit�res, mais � Pas Cher(TM), est une bonne strat�gie. S'il meurt � nouveau dans l'ann�e qui vient vous saurez que vous pouvez acheter du haut de gamme !

### Les transistors de d�coupage

Le marquage sur les transistors vous indique toujours leur type, dont vous pourrez trouver la *datasheet* avec peu d'efforts. 
Le remplacement se fait de pr�f�rence � l'identique, mais cela pose rarement probl�me, en particulier pour les transistors de d�coupage de l'onduleur car les concepteurs utilisent syst�matiquement des transistors tr�s courants et peu on�reux (d'o� les pannes fr�quentes...).

Si vous ne pouvez pas remplacer � l'identique, bon courage pour trouver un transistor compatible. Cela n'est pas dur mais sort franchement du cadre de cet article introductif.

### Les circuits int�gr�s *driver*

Pour les *drivers*, le remplacement est **syst�matiquement** � l'identique. Parfois, la puce est introuvable, ou � un co�t d�lirant (plus de 20EUR, par exemple). Dans ce cas, vous �tes tout bonnement coinc�, et m�me si vous trouvez la puce, elle pourrait bien s'av�rer impossible � remplacer techniquement.

## Remplacer les composants d�fectueux

Le remplacement, c'est le moment o� vous aller toucher une carte �lectronique avec un bout de m�tal surchauff�, dans le but de la r�parer. Le risque de destruction en cas de fausse manipulation est bien r�el. (Concernant votre s�curit� personnelle, les lunettes de protection sont une bonne id�e, et essayez de ne pas attraper le fer chaud avec la main sous peine de voir votre femme vous verser le ketchup jusqu'� la fin de vos jours.)

### Mat�riel

Chez les snob, on va vous conseiller un fer � souder de grande marque (trois fois le prix de votre �cran neuf), probablement cinq � dix outils sp�cifiques, ruineux, et pas toujours utiles, etc.
Pour la r�paration � Pas Cher(TM), on se contentera de :

- fer � souder premier prix, sans r�glage de temp�rature, puissance 20W minimum (en dessous c'est un jouet pour les enfants)
- tresse � d�ssouder en cuivre
- soudure �tain-plomb la moins ch�re que vous trouvez dans un diam�tre raisonnable
- pompe � d�ssouder
- (optionnel) panne de fer � souder de forme plate (type tournevis plat), plus efficace qu'une panne pointue malgr� l'intuition
- (optionnel) pince brucelle invers�e
- (optionnel) troisi�me main (l'outil, pas la partie du corps)
- (optionnel) loupe

### D�ssouder les composants d�fectueux

Regardez une vid�o Youtube si vous ne savez pas faire. Pour les condensateurs traversants c'est le genre de chose que vous avez fait au coll�ge. Pour les composants de surface il y a des astuces mais je ne suis pas sp�cialement expert. Pour les puces en format type TSOP (voir photos dans cet article), il y a des astuces *compliqu�es*.

Faites tr�s attention � une chose : les petites pistes sur les PCB tendent � se d�coller voire � se couper quand elles ont trop chauff�. Si cela arrive, votre travail va devenir tr�s difficile, car vous vous �tes engag� dans la destruction de la carte, ce qui n'�tait pas notre objectif premier. Notez que vous pouvez toujours tenter de remplacer la carte, pour cela cherchez sur *Google* un des marquages appos�s sur le PCB. Quand vous verrez le prix, vous changerez probablement d'avis et reprendrez votre fer � souder en main !

### Installer les nouveaux composants

Installer les composants est plus facile que de les enlever : vous avez donc fait le plus dur.
Pour les composants traversants, (attention au sens des condensateurs �lectrolytiques qui sont polaris�s !) si vous n'avez pas r�ussi � retirer toute la soudure du trou, vous pouvez tenter d'ins�rer le composant tout en faisant fondre la soudure, ou carr�ment de percer dans le trou. Attention � percer au diam�tre le plus faible possible : il ne faut surtout pas �largir le trou dans le PCB, car le PCB est pourvu d'une bague en m�tal qui fait contact. Si vous d�truisez celle-ci vous aurez �norm�ment de difficult� � souder correctement le nouveau composant.

Pour les composants en surface, il faut de la patience, *Youtube*, et ne pas trop trembler. La tension de surface de l'�tain fondu, associ�e � la magie de la tresse � d�ssouder, rendent l'installation d'un chip assez facile par rapport � ce qu'on peut s'imaginer.

Le probl�me arrive lorsque l'�tape de d�ssoudage a �t� mal r�alis�, comme sur la catastrophe, dont je ne suis pas fier d'�tre responsable, que je vous pr�sente ci-dessous. Voyez comment plusieurs des pistes (entour�es sur l'image) sont cass�s, ce qui n�cessitera un raccord avec du fil. Notez �galement que certaines pistes semblent de plus �tre droites : c'est parce qu'elles sont d�coll�es ! Un enfer pour la r�paration.

