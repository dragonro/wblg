---
layout: post
title: D�tection de pr�sence pour �clairage � LED
date: 2014-08-03 20:04:50
tags: electronics lighting
category: francais
comments: true
img_rel: ""
---

<script type="text/javascript" src="//cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

Cet article vise � expliquer th�oriquement comment rajouter une d�tection de pr�sence � un syst�me d'�clairage � LED. Je l'�cris pour un ami, sans avoir r�alis� tous les montages que je vais d�crire. N'h�sitez pas � me rapporter d'�ventuelles erreurs, ou les d�tails de votre r�alisation.

# Objectif

On suppose une pi�ce �clair�e par un ruban de LED 12V. Ce type d'installation devient � la mode : c'est plut�t joli, peu co�teux, et la qualit� de lumi�re est acceptable, surtout depuis que les lampes � incandescence sont interdites en Union Europ�enne car c'est cela qui va sauver la plan�te, oui-je-suis-parti-en-Guadeloupe-�-No�l-dernier-mais-j'ai-mis-des-lampes-�-�conomie-d-�nergie.

L'objectif de cet article est d'am�liorer le montage typique d'�clairage � ruban de LED, pour y rajouter une d�tection de pr�sence qui sera tr�s pratique dans un couloir, une pi�ce de stockage, ... et de mani�re g�n�rale toute pi�ce aveugle. R�f�rez-vous � mon [premier article sur la d�tection de pr�sence](/~sven337/english/2014/03/30/Automatic_lighting_in_bathroom.html), en anglais, pour plus de d�tails sur les choix techniques dans la d�tection de pr�sence. Je ne reviens pas dessus.

# Situation de d�part

Le montage de d�part est le suivant : un ruban de LED d'une seule couleur (� deux fils, un + et un -) aliment�e � environ 1 amp�re/m�tre sous 12V par une alimentation � d�coupage �PasCher(TM) reli�e au secteur. Cette alimentation est reli�e � une prise command�e par un interrupteur : lorsque l'utilisateur rentre dans la pi�ce, il ferme l'interrupteur, ce qui active l'alimentation, qui fait s'�clairer les LED.

# D�tection de pr�sence

On r�alisera la d�tection de pr�sence avec un d�tecteur � infrarouge passif �PasCher(TM) achet� sur eBay : <http://www.ebay.com/itm/400330055400>.
Ce d�tecteur doit �tre aliment� par 3.3...12V DC, et a une sortie binaire : 1 = pr�sence detect�e, 0 = pas de pr�sence d�tect�e.

# Commutation

## Dans l'alimentation

J'en parle en premier afin d'�vacuer le sujet rapidement. Il est courant que les contr�leurs PWM des alimentations disposent d'une patte "on/off" activable avec un niveau logique compatible avec celui du d�tecteur choisi. 
Toutefois, ce n'est pas �vident que le mod�le pr�cis de votre alimentation ait une telle patte, et m�me si le contr�leur en avait une il faudrait aller y souder un fil. Cela peut-�tre compliqu�.
Mais le vrai probl�me est la **s�curit�**. Le contr�leur est situ� au primaire de l'alimentation, c'est-�-dire pour une alimentation secteur du c�t� du 230V AC. Si on s'amuse � interfacer un capteur avec le primaire de l'alimentation, on perd l'isolation galvanique qui est une propri�t� fondamentale d'une alimentation secteur : th�oriquement le montage va fonctionner sans tuer son utilisateur, mais au moindre probl�me, on court le risque de voir du 230V AC se promener dans le capteur, et donc � la sortie, tuant � la fois l'�lectronique en sortie (de mani�re spectaculaire), et, pourquoi pas, l'utilisateur.
Pour r�aliser le montage correctement il faut rajouter un optocoupleur (c'est comme �a que fonctionnent les alimentations avec standby qu'on trouve par exemple dans les t�l�visions ou les ordinateurs), mais cela devient compliqu� � faire.

Oublions donc cette id�e qui avait pour elle l'avantage de l'�l�gance.

## En AC � l'entr�e

L'id�e la plus simple, c'est de remplacer (ou de compl�ter) l'interrupteur physique par un interrupteur �lectronique command� par le capteur. 
Il y a deux m�thodes pour commuter une tension "secteur" en toute s�curit� : 

1. un relais �lectrom�canique
1. un relais � semiconducteurs

Voir la [section correspondante](/~sven337/english/2014/03/30/Automatic_lighting_in_bathroom.html#turn-on-the-light) dans mon pr�c�dent article.

Le relais �lectrom�canique co�te cher, n�cessite une diode suppl�mentaire, et un courant assez important pour le maintenir en "on" : pas forc�ment une super id�e.
Le relais � semiconducteurs co�te cher, est assez gros, et n�cessite un courant *un peu moins* important [...].
Dans les deux cas le courant n�cessaire pour commuter le relais est trop important pour le mod�le de capteur que j'ai retenu, donc il faudra rajouter un transistor.

Cette solution est int�ressante mais n�cessite de faire attention lors du branchement et de la "mise en bo�te", car le montage va manipuler les tensions importantes. Il faut imp�rativement couper l'�lectricit� au compteur avant de toucher � l'installation, et s'assurer d'avoir correctement s�curis� les composants et les c�blages.

Le type de relais importe peu, et le choix de cette solution d�pend principalement de vos contraintes sp�cifiques pour l'int�gration. De mani�re g�n�rale je pense que la commutation AC demande de faire plus attention (s�curit�), donc l'int�gration est plus compliqu�e, donc ce n'est pas la solution que je retiendrais.

## En DC � la sortie

L'autre possibilit� est de faire la commutation au niveau de la **sortie** de l'alimentation, c'est-�-dire rajouter un transistor qui va "couper l'�lectricit�" aux LED sauf lorsque le capteur le rend passant.

Par rapport � la solution "AC", voici les avantages :

- pas de question de s�curit� lors du montage et en utilisation
- volume plus faible
- co�t plus faible
- pas de courant de "holding", donc pas de transistor suppl�mentaire � rajouter
- c�blage plus simple

Et les inconv�nients :

- le transistor va chauffer, donc il faut un radiateur
- difficile de s'y retrouver parmi les centaines de milliers de transistors sur le march�

### Ennuis thermiques

Cette solution a bien s�r ma faveur, toutefois il faut faire vraiment attention � l'aspect thermique. Faire passer 6A dans un transistor ne pose aucune esp�ce de probl�me � condition que celui-ci soit pourvu d'un radiateur. Celui-ci n'a pas besoin d'�tre �norme, mais sa pr�sence est indispensable sous peine de d�truire le transistor et de cr�er un danger d'incendie. Les vrais ing�nieurs en �lectronique dimensionnent leurs radiateurs par le calcul th�orique. Ce n'est pas difficile � faire, mais superflu pour un projet de ce genre. Il suffit d'acheter un radiateur con�u pour le format de transistor retenu (TO-220 par exemple), de le monter, et de mettre les doigts dessus pour voir s'il chauffe trop. Pour m�moire, vous commencez � vous br�ler les doigts aux alentours de 50�C, et un transistor peut fonctionner sans probl�me jusqu'� 90�C voire 105�C. 

Notons �galement que ce genre de syst�me d'�clairage est plut�t con�u pour les pi�ces de passage, et non pour les pi�ces de vie : la lumi�re reste rarement allum�e plus de quelques minutes ! Par cons�quent le transistor ne sera passant que pendant quelques minutes � chaque fois, et n'aura pas trop le temps de chauffer.

### Choix du transistor

Il existe pratiquement autant de mod�les de transistors que d'�tres humains sur Terre, mais le choix est assez simple.
D'abord, on va se restreindre � ceux qu'on peut [acheter facilement](/~sven337/francais/2014/06/02/Acheter-des-composants-electroniques-sur-Internet.html), ce qui r�duit �norm�ment la liste.
On cherche un transistor de puissance, c'est-�-dire un transistor qui fait passer de gros (> 1A) courants, et dans un package suffisamment volumineux pour ne pas avoir de probl�me de dissipation thermique.

Nos crit�res sont les suivants :

- courant nominal > 6A (en supposant 5 m�tres de ruban � 1.2A/m), le plus grand est le mieux
- tension de claquage >= 16V (en supposant qu'on alimente en 12V, en prenant une marge de s�curit�)
- gate en "logic level" c-�-d tension de seuil < 3.3V (sinon le capteur n'imposera pas une tension assez grande pour commuter le transistor)

En [trois secondes sur eBay](http://www.ebay.com/itm/5x-IRLZ44N-PBF-MOSFET-N-Channel-Logic-Level-41A-55V-0-022OHM-TO-220-IRLZ34NL-/121129875333?pt=LH_DefaultDomain_0&hash=item1c33e73785), je trouve des transistors qui ont l'air de faire parfaitement l'affaire (il est possible de les brancher en parall�le - correctement - pour diminuer le stress thermique ou faire passer des courants plus grands, � noter que les 47A nominaux de ce transistor ne sont atteignables qu'avec un *gros* radiateur, et si vous commutez 47A d'�clairage vous avez probablement mal con�u votre solution).

Pour les radiateurs, c'est un peu plus compliqu� car il y en a plein et j'ignore quelle taille prendre. J'aurais tendance � choisir un mod�le [de ce genre](http://www.ebay.com/itm/Lots-10-Heatsink-Heat-Sink-With-Screw-Sets-For-TO-220-/310317074587?pt=US_CPU_Fans_Heatsinks&hash=item484056c89b).

# R�alisation en commutation DC

On va "couper" la sortie de l'alimentation pour rajouter un transistor. Cela peut se faire sur le + ou sur le -, mais le choix n'est pas indiff�rent ! Avec un transistor de type N (le plus courant), il faut obligatoirement placer le transistor sur le - de telle sorte que sa patte "source" soit reli�e au - de l'alimentation (la gate sera reli�e � la sortie du capteur, et le drain sera reli� au retour du - des LED). Cet article est bien assez long donc je n'explique pas pourquoi c'est ainsi.
