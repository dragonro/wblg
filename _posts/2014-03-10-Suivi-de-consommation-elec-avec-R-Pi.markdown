---
layout: post
title: "Suivre sa consommation �lectrique avec un Raspberry Pi"
date: 2014-03-09 22:17:00
category: francais
---

Cet article d�taille comment **suivre sa consommation �lectrique** avec un **Raspberry Pi** et un montage �lectronique simple et surtout tr�s bon march�. 
Il n�cessite de disposer d'un compteur �lectrique suffisamment moderne.

# Introduction

Le Raspberry Pi est un mini ordinateur capable de faire fonctionner Linux, et qui co�te peu cher. De nombreuses personnes sugg�rent de s'en servir comme serveur (web, e-mails, ...), mais je doute que ses performances soient suffisantes pour cela. (� vrai dire je n'ai jamais test� car j'utilise un eeePC 701 comme serveur depuis plusieurs ann�es, dont je suis tr�s satisfait.)

N�anmoins je poss�de un Raspberry Pi, ainsi qu'un placard �lectrique dans lequel je peux le poser, le brancher, et l'interfacer avec le compteur. En avant !

# La sortie t�l�info

La sortie **t�l�info** est pr�sente sur **tous les compteurs EDF** de moins de quelques ann�es. Le mien ressemble � cela : <a href="data/teleinfo/compteur.jpg"  title="Compteur A14C5"><img src="data/teleinfo/compteur-thumb.jpg" /></a>. La vis que j'indique sur la photo n'est normalement pas scell�e, et permet d'ouvrir la trappe inf�rieure qui vous donnera acc�s aux bornes I1/I2 (� gauche) de la t�l�info. Il y a deux autres bornes qui elles v�hiculent (parfois) du 230V AC, donc ne mettez pas les doigts, je crois qu'elles servent pour brancher une lampe t�moin du _jour plein_ (option tarifaire d�su�te).

La t�l�info r�pond � une sp�cification disponible en ligne : <http://norm.edf.fr/pdf/HN44S812emeeditionMars2007.pdf>. De nombreux projets se contentent de **capter l'impulsion lumineuse** du compteur (une impulsion = 1W.h en g�n�ral), mais la sortie t�l�info peut nous donner bien plus que cela :

- puissance instantan�e en watts
- indice du compteur
- situation heure pleine/heure creuse (qui ne me concerne toutefois pas car je me contente de l'option base, plus avantageuse dans ma situation)

La sortie t�l�info impl�mente un protocole qui n'est �lectriquement pas compatible avec les protocoles que parlent les ordinateurs en g�n�ral (RS-232, USB, parall�le, ...), ni d'ailleurs avec les protocoles du monde de l'informatique embarqu�e tels que I2C, SPI, ou un b�te GPIO. C'est un choix curieux de la part d'EDF, mais le protocole t�l�info est assez facile � convertir en un protocole connu tel que **RS-232**.
La sp�cification est publique et plut�t bien �crite, alors au travail !

La forme du signal est la suivante : un **0** correspond � une **sinuso�de** � **50KHz** variant entre **-12V** et **+12V**, un **1** correspond � un plateau � **+0V**.
**RS-232** utilise quant � lui **+12V** pour un 1, et **-12V** pour un 0. Cette tension n�gative est un choix technique discutable qui complique la vie de l'�lectronique moderne souhaitant impl�menter RS-232. En g�n�ral les circuits int�gr�s ont une UART qui utilise des niveaux de signaux diff�rents (TTL) : +Vdd = 3.3V pour un 1, et +0V pour un 0. C'est plus simple, et plus logique... mais les ports s�rie des ordinateurs "parlent" du vrai RS-232. Le circuit **MAX232** est un exemple de circuit int�gr� qui s'occupe de la conversion des niveaux entre RS-232 "r�el" et RS-232 TTL (celui � 3.3V).

# Travaux pr�c�dents

J'ignore ici les montages bas�s sur la **d�tection de l'impulsion lumineuse**. De nombreuses personnes ont r�alis� des montages (et parfois cr�� une activit� commerciale) permettant d'**interfacer la t�l�info avec RS-232**.

Le probl�me de ces montages est que bien souvent ils visent � obtenir une compatibilit� avec RS-232 (par exemple <https://store.adtek.fr/home/12-interface-teleinfo-rs232.html> ou  <https://store.adtek.fr/home/10-teleinfo-usb-sans-souris-folle-v2.html>. Je ne connais pas cette soci�t� ni leurs produits, dont je n'ai pas de raison de penser qu'ils fonctionnent mal. Par contre, d�penser 30 euros ou plus pour ce genre de montage est clairement en dehors du budget que je m'�tais fix�.). Or, RS-232 ne nous int�resse pas, puisque le R-Pi impl�mente RS-232 avec des niveaux TTL, incompatible, mais aussi plus simple. Je ne suis bien s�r pas le premier � le remarquer, et on voit des montages qui consistent � brancher directement un optocoupleur bidirectionnel **SHF6206** � la sortie t�l�info et au Pi... sauf que ce mod�le est pratiquement introuvable, et que de mani�re g�n�rale les optos bidirectionnels co�tent cher. On trouve un exemple abouti et bien expliqu�, qui a servi de base � ma r�alisation, � l'adresse suivante : <http://www.chaleurterre.com/forum/viewtopic.php?t=15153>.

J'ai r�alis� un montage diff�rent dont l'objectif (atteint) �tait de n'utiliser que des composants facilement disponibles (sur eBay, DealExtreme ou AliExpress, par exemple), et peu chers.

# Architecture

Ayant eu beaucoup de mal � trouver un optocoupleur qui ferait l'affaire pour un branchement direct (car il faut non seulement qu'il soit bidirectionnel mais aussi que ses caract�ristiques soient compatibles avec le timing du signal, chose pas toujours facile � garantir), j'ai opt� pour un montage redresseur et un filtrage (permettant d'obtenir **0** = **+12V** constant, **1** = **+0V**, qui attaque l'optocoupleur le moins cher que j'ai pu trouver, dont la sortie est reli�e au Pi de mani�re similaire au message du forum dont je donne un lien ci-dessus.

Voici le sch�ma correspondant (cliquez pour l'avoir en grand):
<a href="data/teleinfo/schema.jpg" title="Sch�ma redressement teleinfo" ><img src="data/teleinfo/schema-thumb.jpg" style="border:1px solid black" width="80%" /></a>

J'ai d'abord tent� un redressement simple alternance, mais comme on verra dans le paragraphe suivant ce n'�tait pas une bonne id�e.

# R�alisation �lectronique

## Simulation

Avant toute chose et afin de dimensionner correctement les composants, j'ai choisi de simuler le circuit � l'aide de **ngspice** (logiciel libre disponible sous Linux). Ce type de simulateur ne donne pas toujours de bons r�sultats mais pour un circuit aussi simple il nous sera tr�s utile. Une alternative consisterait � calculer la valeur du condensateur et de la r�sistance de protection de l'optocoupleur � la main, mais cela n�cessiterait de savoir ce qu'on veut obtenir en termes math�matiques ! En r�alit�, on se contentera de regarder la forme du signal et de d�cider si oui ou non il se rapproche suffisament du signal carr� attendu par l'UART du Raspberry Pi. N'ayant pas d'oscilloscope, je fais tout cela en simulation.

Voici un premier circuit � simuler, avec redressement en simple alternance :
[title="Circuit redressement mono alternance"](data/teleinfo/filtrage_1diode.net)

Pour lancer la simulation, ``ngspice filtrage_1diode.net`` va charger le fichier, ensuite la commande ``tran 0.05us 3.2ms`` fait une simulation pendant 3.2 ms par pas de 0.05 us, et on peut visualiser la courbe de tension en un point donn� en utilisant la commande ``plot``.
``plot v(1)`` nous montre le signal appliqu� � l'entr�e :
<a  href="data/teleinfo/spice_input_signal.jpg"><img src="data/teleinfo/spice_input_signal-thumb.jpg" /></a>

Il s'agit de la s�quence 0->1->0->1. Bien s�r, c'est plut�t la sortie qui nous int�resse. Ce qu'on voudrait voir, c'est quand est-ce que **le Pi** voit un z�ro ou un un. Ce n'est pas quelque chose qu'on trouve directement, mais cette information d�pend du courant qui traverse la LED de l'optocoupleur, qui est lui-m�me proportionnel � la tension aux bornes du condensateur. On se contentera donc de regarder la tension aux bornes du condensateur, et de d�cider _au doigt mouill�_ si les transitions sont suffisamment franches ou pas.

Pour la tension aux bornes du condensateur, c'est ``plot v(3)``. On obtient la courbe suivante :
<a  href="data/teleinfo/spice_output_1D.jpg" title="Redressement mono-alternance"><img src="data/teleinfo/spice_output_1D-thumb.jpg" /></a>

### Qu'est-ce qu'on aimerait avoir ?

Un beau signal en cr�neau !

### Qu'est-ce qu'on a ?

Un moche signal en cr�neau :)
Plus s�rieusement, deux �l�ments attirent l'oeil : 
1. l'ondulation r�siduelle entre 4.5 et 5.7V, alors qu'on aimerait quelque chose de bien plat. Cela pourrait se traduire par 0 qui n'est pas compl�tement stable et qui risquerait d'�tre mal interpr�t�
1. le temps de chute lors du passage � 0V, qui est tellement important que le 0V n'est jamais atteint. Cela pourrait se traduire par un 1 qui est syst�matiquement lu comme un 0 car le niveau logique bas n'est jamais atteint

Ces points sont-ils vraiment un probl�me ? Dur � dire sans rentrer plus en d�tails dans les caract�ristiques de l'optocoupleur.

### Redressement double alternance 

On peut am�liorer l'ondulation r�siduelle en faisant un redressement double alternance, ce qui correspond au sch�ma que j'ai pr�sent� plus haut.

[Circuit redressement double alternance](data/teleinfo/filtrage_4diodes.net)

Voici le signal aux bornes du condensateur :
<a  href="data/teleinfo/spice_output_4D.jpg" title="Redressement bi-alternance"><img src="data/teleinfo/spice_output_4D-thumb.jpg" /></a>

Si le temps de chute n'a pas boug�, et reste inqui�tant, on note que l'ondulation r�siduelle est bien meilleure (car on charge le condensateur deux fois plus souvent, il se d�charge donc deux fois moins pendant l'oscillation !). On pourrait faire mieux, mais il faudrait alors augmenter la capacit� (ce qui compromettrait tr�s fortement le temps de chute, alors qu'on est d�j� limite), ou diminuer la r�sistance **R1** afin d'augmenter le courant de charge - mais cela nous sortirait de la sp�cification d'EDF, ce qui nous enverrait probablement directement en prison apr�s le d�part de feu � notre compteur ! 

Pour am�liorer le temps de chute, on peut jouer sur la valeur de **R2**, mais on assiste alors (je vous laisse jouer avec Spice) � un ph�nom�ne de _vases communicants_ : le passage � 0 est plus rapide si on diminue R2, mais l'ondulation r�siduelle � 1 devient tr�s importante.
La valeur de R2 est contrainte par la s�curit� de l'optocoupleur, qui nous impose dans la datasheet un courant maximal correspondant � une r�sistance d'au moins 270 Ohm. J'ai choisi 2.2 kOhm.

## Achat des fournitures

- diode 1N4148, vendue par 10 sur ebay � environ 1EUR (le montage en n�cessite une seule, mais autant en utiliser 4 et faire un redressement double alternance)
- optocoupleur PC817, vendu par 10 sur ebay � environ 1EUR (un seul est n�cessaire et nous n'en mettrons pas quatre)
- r�sistances 0.25W classiques (900 Ohm, 2.2 kOhm, 47 kOhm), � acheter dans un assortiment tel que <http://dx.com/p/1-4w-resistance-metal-film-resistors-400-piece-pack-121339>
- un condensateur de 22nF, j'ai pris un _ceramic disc_ sur eBay � environ 1EUR les 10 (vous commencez � conna�tre le refrain)
- (optionnel) une mini breadboard pour un premier montage (<http://dx.com/p/mini-prototype-printed-circuit-board-breadboard-white-140716>, <http://dx.com/p/breadboard-jumper-wires-for-electronic-diy-65-cable-pack-118826>, <http://dx.com/p/male-to-female-dupont-breadboard-jumper-wires-for-arduino-40-piece-pack-20cm-length-146935>)
- un petit PCB proto pour mettre le montage au propre <http://dx.com/p/pcb-prototype-blank-pcb-2-layers-double-side-3-x-7cm-protoboard-green-140924>
- une bo�te de biscuits en carton, des ciseaux et du scotch pour faire un bo�tier moche **ou** un bo�tier plastique qui vous co�tera plus cher que tous les composants r�unis

## Assemblage et branchement

J'ai r�alis� un premier prototype sur platine d'essai (_breadboard_). Le fonctionnement m'ayant donn� satisfaction j'ai d�cid� de r�aliser un assemblage plus propre sur un PCB proto, que j'ai ensuite plac� dans un �crin en carton-de-paquet-de-biscuits.

<a href="data/teleinfo/montage_final.jpg" title="Montage final en �crin carton"><img src="data/teleinfo/montage_final-thumb.jpg" /></a>

Note : les connexions sont faites avec des _jumper wires_ que j'ai soud�s. En effet ce type de PCB dispose de pastilles mais pas de pistes pr�-trac�es, et je me suis rendu compte que les connexions sont finalement assez difficiles � faire. Je pr�f�re travailler avec une [Veroboard](https://en.wikipedia.org/wiki/Veroboard).


# Int�gration logicielle

Si tout est bon mat�riellement, le Pi recevra sur son port s�rie un signal qu'il est capable de comprendre.
Il faut tout de m�me:

1. s'assurer qu'aucun programme (par exemple un **getty** pour la console s�rie) n'�coute sur le port s�rie
1. configurer le port en **1200 baud**, **7/E/1** (7 bits de donn�es, 1 bit de parit� paire, 1 bit de stop)
1. lancer un programme pour �couter sur le port s�rie
1. cr�er des tableaux, des graphes, pr�senter tout dans une application web, ...

Je couvrirai le dernier point dans un article s�par�, qui concernera l'�lectricit� mais aussi le gaz.

Mon Raspberry Pi fonctionne avec la distribution Arch Linux, et voici le script que j'utilise :
~~~ bash
systemctl stop serial-getty@ttyAMA0.service
./ti_cat | egrep '^(PAPP|BASE)' -a --line-buffered | ./cksum | ../data/report_to_hm_web.sh
~~~

La deuxi�me ligne n'est pas l'expression la plus simple. 

- Le programme ``ti_cat`` est une version tr�s all�g�e d'un programme de teleinfo r�alis� par quelqu'un d'autre, celui-ci se contente d'�crire tout ce qu'il re�oit du port s�rie, apr�s l'avoir configur� correctement. ``cat`` n'est malheureusement pas suffisant opur cela.
- Le grep filtre les seuls �l�ments qui m'int�ressent, qui sont la puissance apparente et l'indice du compteur. Peut-�tre certains autres vous int�resseront-ils en particulier pour les abonnements HP/HC. 
- ``cksum`` est un programme qui calcule les checksums tels que sp�cifi�s par EDF, et, comme son nom ne l'indique pas, accumule les valeurs pour calculer une moyenne de puissance sur une minute. En effet la plupart des programmes tels que ``teleinfofs`` fournissent, lorsque vous les interrogez, la valeur *instantan�e* de la puissance apparente, alors qu'il est plus correct de faire la moyenne depuis la derni�re requ�te, ce que fait cksum.

Au final, on obtient, chaque minute, l'indice du compteur, ainsi que la puissance apparente. Rigoureusement l'int�grale de la puissance apparente doit nous permettre de retrouver l'indice, mais cela simplifie les calculs de co�t par jour d'avoir l'indice sous la main (le calcul devient d�s lors une simple soustraction).

Ces programmes sont disponibles sur Github : <https://github.com/sven337/home-monitoring-client> dans le r�pertoire **edf**. Les autres programmes seront couverts dans les articles suivants, parfois en anglais. Si vous ne parlez pas anglais et qu'un article vous int�resse, je ferai la traduction sur demande. Mon objectif est de d�crire en fran�ais ce qui concerne uniquement notre brave patrie, et en anglais ce qui peut int�resser une plus large audience.

# Graphes

Je traiterai cela plus en d�tail dans un prochain article, mais voici quand m�me un _teaser_:

## Graphe RRD
<a href="data/teleinfo/teleinfo_rrd.png" title="Graphe de consommation RRD sur une semaine"><img src="data/teleinfo/teleinfo_rrd-thumb.png" /></a>

On voit ici plusieurs informations int�ressantes, mais c'est � grosse maille. J'ai beaucoup cuisin� vendredi soir, et on peut voir que le four et les plaques � induction �taient allum�s en m�me temps car j'ai consomm� une puissance importante. On voit �galement assez facilement que, contrairement � mon habitude, j'ai cuisin� le mardi midi en plus du soir (je ne me souviens pas de ce que j'ai mang� et cela n'est pas sur le graphe).
Le trou mercredi correspond � la d�sactivation temporaire du syst�me de reporting afin de prendre les photos qui sont pr�sentes sur cette page. (Ce n'est d'ailleurs pas une grande r�ussite).

Le total de puissance et le co�t correspondant sont calcul�s par RRD avec le script ``rrd_render_graphs.sh``.

## Graphe Javascript
<a href="data/teleinfo/teleinfo_jsgraph.jpg" title="Graphe de consommation sur la journ�e"><img src="data/teleinfo/teleinfo_jsgraph-thumb.jpg" /></a>

Ce graphe est interactif, en Javascript, et cr�� � partir des m�mes donn�es. Sur la capture que je montre ce sont les donn�es sur une journ�e. On visualise entre 1h et 2h du matin l'activation du compresseur du r�frig�rateur, puis � nouveau de 4h � 5h. Je me suis lev� vers 8h50 et j'ai allum� mon ordinateur. Un peu avant 12h on observe la courbe caract�ristique (parce que j'ai l'habitude de la voir) de la plaque � induction utilis�e pour faire chauffer de l'eau : en mode _booster_ pendant quelques minutes � 3.2kW, puis � 1.75kW pendant le temps de cuisson de ce qui �tait (je m'en souviens) des p�tes.
On a ensuite la bouilloire �lectrique, l'aspirateur, et le s�che-cheveux, le tout pour une consommation en r�gime permanent d'environ 300W (informatique, r�frig�rateur, VMC) en journ�e, et 600W le soir (�clairage � _�conomie de gaz naturel_, je veux dire, �clairage incandescent).


<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>

