---
layout: post
title: �clairage automatique d'�tag�res
date: 2015-01-25 18:04:41
tags: electronics lighting
category: francais
comments: true
img_rel: "/~sven337/data/shelves"
---

Mon cellier avait besoin d'�tag�res. J'ai fait le choix, risqu�, d'un produit �PasCher(TM) chez Leroy Merlin. Il s'agit de [ce mod�le](http://www.leroymerlin.fr/v3/p/produits/etagere-avasco-racky-en-acier-epoxy-blanc-l75xh150xp30cm-e62980), � 10EUR. J'en ai achet� 3. J'ai ensuite am�lior� ces �tag�res en rajoutant un syst�me d'�clairage automatique, � d�tection de pr�sence, qui permet non seulement d'�clairer le contenu des �tag�res mais aussi l'ensemble de la pi�ce. Cet article explique ma r�alisation.


# Une �tag�re � 10 euros

![�tag�re Leroy Merlin](etagere-avasco-racky-en-acier-epoxy-blanc-l75xh150xp30cm.jpg)
Ne vous laissez pas impressionner par les commentaires sur le site, cette �tag�re fait ce qu'on lui demande. �videmment elle n'est pas tr�s solide, mais en la montant correctement (c'est-�-dire en serrant les boulons **progressivement** et **apr�s** les avoir tous plac�s, comme toujours pour monter un meuble en t�le fine) elle fait ce que j'en attends. Je ne pense pas qu'elle soit appropri�e pour des charges importantes. La sp�cification annonce 50Kg par tablette, c'est tr�s optimiste. En pratique je ne me risquerais pas au del� de 25Kg, ce qui est d�j� bien suffisant pour la plupart des usages.

Au-del� du co�t, les trois avantages majeurs de cette �tag�re sont :

- finition blanc brillant (qui refl�te donc tr�s bien la lumi�re)
- pr�sence d'un repli sous chaque tablette dont la largeur correspond exactement � celle d'un bandeau de LED 3528 !
- tablettes fabriqu�es avec une seule plaque de t�le pli�e, laissant des petits trous aux angles permettant de passer des fils

# Choix des composants

## Bandeau

On va acheter un bandeau de LED 3528, � une seule couleur, de type "warm white" (blanc chaud). Sous chacune des tablettes de chaque �tag�re (� l'exception de celle du bas, � moins que vous ne souhaitiez �clairer votre sol pour mettre la poussi�re en �vidence !), on placera une longueur de LED (environ 70 cm) sur le repli de t�le qui semble pr�vu pour.

### Type waterproof

Je conseille fortement de choisir un bandeau de type "waterproof" IP65. Ce type de bandeau est recouvert d'une protection en silicone. Il n'est absolument pas �tanche (l'appellation *waterproof* est donc trompeuse), mais au moins vous pourrez le nettoyer avec une �ponge humide. (Pour une utilisation immerg�e ce sont des bandeaux IP67 ou IP68 qu'il faut, qui sont int�gralement **enrob�s** dans une r�sine et non simplement **recouverts**.) Voir [ici](http://www.bestledstriplights.com/waterproof-vs-non-waterproof) pour les d�tails, je reproduis ci-dessous une de leurs images.
![Types de bandeaux "waterproof"](http://www.bestledstriplights.com/wp-content/uploads/2014/10/waterproof-types.jpg)

L'int�r�t des bandeaux IP65 est que, dans ma maigre exp�rience, ils sont de meilleure qualit� que les bandeaux IP60 (non waterproof). De plus, la surface silicon�e des IP65 les rend plus faciles � nettoyer, et �vite les court-circuits accidentels qui d�truiraient imm�diatement l'�lectronique de contr�le (ou pourraient causer un incendie). Par cons�quent m�me pour un usage au sec, je pr�f�re d�penser un peu plus et acheter un bandeau IP65.

### Dimension

Il existe deux principaux types de bandeaux, ceux � LED 5050 et ceux � LED 3528. Les 3528 �clairent moins, mais ils font 8mm de large, alors que les 5050 font 10mm. Le repli sur les tablettes fait 8mm, donc on choisira du 3528. J'ai constat� apr�s assemblage que la puissance d'�clairage �tait largement suffisante avec des LED 3528 de toute fa�on.

### Commande

J'ai command� [2 rouleaux de 5 m�tres](http://www.ebay.com/itm/400542008436) sur eBay. 

Prix : environ 24EUR.

## Alimentation

Ces bandeaux s'alimentent en **12V** et sont annonc�s pour consommer environ **0.4A** par m�tre. Pour chaque �tag�re on aura $$ 3 * 0.7 = 2.1m $$ de bandeau, c'est-�-dire une consommation de $$ 2.1 * 0.4 = 0.84A $$ par �tag�re. Pour mes trois �tag�res, cela fait une puissance totale inf�rieure � **3A**, mais j'ai branch� une alimentation **7A** car je me suis tromp� dans mes calculs lors de la commande :) 
De toute fa�on, en achetant du mat�riel chinois, il n'est jamais trop prudent de surdimensionner tr�s largement les composants. En effet la sp�cification est rarement r�aliste, et mon alimentation vendue pour *7A* ne les tient certainement pas, vu son poids et sa taille.

J'ai command� [cette alimentation](http://www.ebay.com/itm/261704378838) sur eBay, mais je ne vous conseille pas d'acheter la m�me. Pr�f�rez plut�t [celle-l�](http://www.aliexpress.com/item/12V-6A-72W-AC-DC-Power-Adapter-Supply-Charger-for-3528-5050-RGB-LED-Strip-Light/587961982.html), � adapter bien s�r en fonction de votre besoin en puissance. 

Prix : environ 15EUR.

## Capteur de pr�sence

Si vous avez lu [mes](/~sven337/english/2014/03/30/Automatic_lighting_in_bathroom.html#pyro-electric-sensor) [articles](/~sven337/francais/2014/08/03/Detection-de-presence-pour-eclairage-a-LED.html#dtection-de-prsence) [pr�c�dents](/~sven337/francais/2014/11/04/Eclairage-decoratif-LED-avec-detection-presence.html#choix-du-matriel) vous savez que j'affectionne tout particuli�rement [un mod�le pr�cis](http://www.ebay.com/itm/400330055400) de capteur, � la fois efficace et pas cher.
Celui-ci commandera l'allumage des LED � travers un transistor.

Prix : environ 2EUR.

## Transistor de puissance

Si on branchait directement les bandeaux sur l'alimentation, ceux-ci seraient �clair�s en permanence. On pr�f�rera n'allumer les LED que quand une personne entre dans la pi�ce de stockage. Le capteur de pr�sence donnera un signal lorsqu'une personne passe devant, mais il n'est pas assez puissant pour alimenter les LED directement. Il faut donc faire intervenir un transistor de puissance, et le dimensionner correctement.
On cherchera un transistor "logic level", c'est-�-dire dont la tension de seuil est d'environ **3.3V**. Ce transistor devra �tre capable de supporter 12V (c'est � peu pr�s toujours le cas), mais surtout de pouvoir faire circuler les courants consid�r�s. Dans le cas pr�sent on parle d'environ **5A** : la plupart des transistors qui ne sont pas dans des formats minuscules (type transistors SMD) supportent th�oriquement des courants de cet ordre de grandeur, mais attention � l'aspect thermique. Un transistor doit �tre refroidi. Pour �viter de trop s'emb�ter il est pr�f�rable de choisir un transistor dans un grand *package*, par exemple un TO-220. Du simple fait de sa taille, la dissipation thermique sera naturellement suffisante pour faire circuler quelques amp�res sans difficult�, et au pire on pourra toujours rajouter un petit radiateur.

J'ai donc fait le choix d'un transistor [STP16NF06](http://www.st.com/web/en/resource/technical/document/datasheet/CD00002501.pdf) achet� [ici](http://www.ebay.com/itm/310725334401). Ils sont largement surdimensionn�s, ce qui me garantissait la tranquilit� d'esprit.

Finalement, j'ai pr�f�r� r�utiliser un [FDD8447](http://www.farnell.com/datasheets/695790.pdf) (achet� [ici](http://www.aliexpress.com/item/FDD8447L-FDD8444-TO252-new-original-spot-Double-Crown-store/1372672839.html)) qui me restait d'un [pr�c�dent projet](/~sven337/francais/2014/06/15/Mon-ecran-plat-est-en-panne--Comment-le-reparer-.html) (la t�l�vision en photographie dans cet article n'a finalement pas �t� r�par�e). 
Le format D-PAK de ce transistor en fait un composant de surface, ce qui m'arrangeait bien pour la r�alisation (l'�cartement entre ses pattes �tant compatible avec la [veroboard](https://en.wikipedia.org/wiki/Veroboard) que j'utilise), et ses caract�ristiques �lectriques et thermiques �taient compatibles avec le projet.

Prix : environ 3EUR.

## Fils de connexion

Pour relier les �tages de LED entre eux, il va falloir mettre des fils. Ceux-ci doivent-�tre capables de transporter un courant suffisant sans surchauffer, il est important de ne pas les sous-dimensionner.
Les fils qui relient la tablette du bas (sous laquelle se situera l'alimentation) � celle du dessus vont transporter les courants correspondant � 3 tablettes, c'est-�-dire un peu moins de **1A** d'apr�s nos calculs. C'est une valeur plut�t faible.
Le probl�me est qu'on ne peut pas choisir des c�bles arbitrairement gros : il faut qu'ils passent dans les "trous" (cr��s par le pliage de la t�le lors de la fabrication) aux coins des tablettes. Ces trous font environ 2.5mm de diam�tre, donc il faudra des c�bles de moins de 1.25mm de diam�tre.
J'ai achet� du [c�ble blanc](http://www.ebay.com/itm/111278677076) (pour plus de discr�tion), avec conducteurs 28AWG, qui peut transporter le courant d�sir� sans probl�me sur des courtes distances.

Prix : environ 2EUR.

# R�alisation : �lectronique

Je n'ai malheureusement pas photographi� le petit circuit de connexion que j'ai r�alis�. Il s'agit simplement de relier entre eux une prise "jack" femelle compatible avec l'alimentation choisie, le transistor, le d�tecteur de pr�sence, et deux fils de sortie "puissance" pour le bandeau de LED. 
J'ai fix� mon montage sur une plaque de plastique afin d'assurer l'isolation, et j'ai scotch� (...) le circuit sous la tablette du bas d'une des �tag�res.
Rien de tr�s compliqu� si vous avez un peu d'exp�rience.

# R�alisation : installation sur l'�tag�re

## Pr�paration des bandeaux
L'installation sur l'�tag�re est conceptuellement tr�s simple mais prend pas mal de temps, essentiellement au niveau de la pr�paration des fils et des bandeaux. De plus, j'ai choisi de r�aliser les connexions par des "dominos". En effet j'avais achet� des connecteurs sp�ciaux (de [ce type](http://www.lightingxy.co.uk/led-accessories/led-strip-connectors/t-shape-2-pads-single-colour-solderless-connector-for-3528-5050-smd-led-strip-for-1-39.html)), mais ils ne font pas l'affaire pour plusieurs raisons, � commencer par leur incompatibilit� avec mes bandeaux de LED.
![Connecteur incompatible - fermeture �crase la LED](bad_connector.jpg)
![Domino avec adh�sif double face](domino.jpg)

Sur chaque bandeau de LED j'ai donc soud� une courte longueur de fil, noy� la soudure dans de la colle � chaud afin d'am�liorer la r�sistance m�canique et d'isoler (car un court-circuit serait dangereux), et rep�r� le '+' avec un peu d'adh�sif rouge (� l'extr�mit� du fil).

Le bandeau est ensuite coll� sur le repli sous la tablette, les LED sont dirig�es vers le haut (et la peinture blanche brillante prend tout son int�r�t, permettant un �clairage indirect de grande qualit�).

![Bandeau pour tablette sup�rieure](mounting_top_strip.jpg)

## Raccords

Le concept est le suivant : chaque bandeau sera connect� � un domino, qui verra arriver les fils de l'�tage sup�rieur, et les fils de l'�tage inf�rieur.
En photo :
![Installation en cours](mounting_middle.jpg)
![Raccords termin�s pour cet �tage](mounting_middle_done.jpg)

� l'�tage du haut, on ne met pas de domino, car il n'y a pas d'�tage au dessus : les fils de connexion sont plus longs et descendent directement � l'�tage en dessous.

![Fils �tage du haut](mounting_top.jpg)

## Installation finale

Les �tag�res sont connect�es entre elles avec des fils de plus gros diam�tre (car les longueurs de raccords sont importantes, surtout entre l'�tag�re qui est situ�e � droite dans ma pi�ce et les deux autres), avec encore une fois des dominos. Le capteur de pr�sence est plac� au niveau du sol, il d�tectera nos jambes (et cela fonctionne !).

On constate, et c'est visible sur les photographies suivantes prises sans autre �clairage que celui des bandeaux, que la puissance d'�clairage permet de se dispenser du plafonnier de la pi�ce.

![Montage final, c�t� gauche](final_left.jpg)
![Montage final, c�t� droit](final_right.jpg)
![�clairage indirect](mounted_top.jpg)

Apr�s montage, les fils sont maintenus invisibles (sauf depuis l'int�rieur de l'�tag�re) :
![Fils cach�s](mounted_middle.jpg)

Il reste un petit d�faut, indiqu� en rouge sur la photo suivante : selon l'angle de vue, on peut se retrouver avec une LED qui �met directement dans l'oeil de l'utilisateur, ce qui est d�sagr�able. J'ai r�gl� le probl�me en masquant les trous concern�s par un peu de papier blanc.
En vert, la position du capteur de pr�sence, tr�s discret et n�anmoins fonctionnel.

![�blouissement en rouge, capteur de pr�sence en vert](final_left2.jpg)


<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>
<script type="text/javascript" src="//cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

