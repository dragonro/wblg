---
layout: post
title: Analyse d'un babyphone pourri
date: 2016-03-03 21:35:59
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/babyphone"
---

J'ai r�cup�r� � bon prix un "babyphone" d'occasion. Celui-ci est un Hama BC823 qui transmet non seulement le son, mais �galement la vid�o.

![R�cepteur](receiver.jpg)
![Transmetteur](transmitter_face.jpg)

Cet appareil est un d�sastre. Il dispose que 4 canaux de transfert sans fil, tous dans la bande des 2.4GHz. Le transfert est fait en analogique (sans surprise, car transf�rer de la vid�o num�rique en temps r�el n�cessite une puissance de calcul assez importante), avec une tr�s mauvaise qualit� d'image mais surtout du son : l'appareil est tr�s sensible aux perturbations et la port�e effective pour une transmission de qualit� est de l'ordre de 4 m�tres (dans un logement qui, il est vrai, a des murs �pais). Le premier canal �crase purement et simplement mon r�seau wifi. Les trois autres n'interf�rent pas avec mon wifi, mais peut-�tre avec celui des voisins.

Bref, l'appareil est tout � fait inutilisable. Dans cet article, je vais m'int�resser un peu � son architecture. Mon objectif � la base �tait de faire les am�liorations n�cessaires pour pouvoir m'en servir, mais j'ai rapidement conclu que la bonne solution �tait de refaire int�gralement l'�lectronique pour utiliser un syst�me de transmission num�rique du son, et de perdre la vid�o, qui me semble de toute fa�on relever du gadget. Je d�taillerai ce travail dans les prochains articles.  

# Transmetteur

![Transmetteur de profil](transmitter_side.jpg)
![�tiquette du produit](transmitter_label.jpg) 

Le syst�me s'av�re �tre "idiot" : pas de d�tection de niveau sonore. Le transmetteur transmet en permanence son et vid�o sur le canal s�lectionn�. La cam�ra est pourvue d'un capteur de luminosit�, et de plusieurs LEDs infrarouges pour la "vision de nuit" (sans quoi l'int�r�t de la fonction vid�o serait tr�s, tr�s r�duit).

On voit ici le microphone (�lectret) utilis� par l'appareil, et un ensemble de fils qui partent dans le module cam�ra (que j'ai d�mont� mais que je n'ai pas photographi�). L'amplification du microphone est r�alis�e dans le module cam�ra.

![Electret du transmetteur](transmitter_internal_electret.jpg)

La carte m�re du transmetteur ne contient pas grand chose.
![Carte m�re du transmetteur](transmitter_internal_front_board.jpg)	
![Carte m�re du transmetteur - verso](transmitter_back.jpg)	
![Carte m�re avec antenne (et condensateur � ignorer)](transmitter_internal_front_addedCAP.jpg)	

On distingue une LED D1 (allum�e lorsque le transmetteur est allum�), l'interrupteur SW2, le connecteur d'alimentation J1 (6V 400mA), le connecteur J3 pour le module cam�ra, le s�lecteur de canal (1 2 3 4) SW 1, et un module radio au centre derri�re un bouclier m�tallique, et dont sort une antenne.

Je n'ai pas trouv� de r�f�rence de ce module, ne l'ayant pas retir� de la carte afin de me garder la possibilit� de revendre le babyphone (ce que je ne ferai normalement pas �tant donn� mon projet de remplacement int�gral de l'�lectronique interne, mais � l'heure o� j'�cris ces lignes celui-ci n'est qu'� environ 30% d'avancement).

La documentation de [ce module radio](https://www.dpcav.com/data_sheets/AWM631TX.pdf) me donne � penser que le module pr�sent sur la carte, quoique diff�rent, est tr�s similaire, notamment avec le choix du canal 4 par d�faut. De toute fa�on ce module ne vaut rien donc �a ne m'int�resse pas vraiment de l'�tudier en d�tail.

� l'int�rieur du module camera, on trouve un amplificateur op�rationnel LM358 pour le microphone, un LM386 qui est probablement utilis� par le circuit vid�o (mais je n'ai pas v�rifi�), un petit chip marqu� C0 21F, et un chip de cam�ra marqu� [ViMicro](http://www.vimicro.com/) VA10. Ce produit n'appara�t pas sur le site de ViMicro, je pr�sume que la soci�t� ne conserve pas en ligne la documentation de ses anciens produits (ce qui est une pratique aussi courante que d�testable).


# R�cepteur

L� o� le transmetteur �tait tr�s simple, le r�cepteur nous montre une �tonnante complexit�.

![Ouverture du r�cepteur](receiver_disassemble.jpg)

## C�t� pile

![C�t� pile](receiver_back_board.jpg)

D'un c�t� de la carte, on observe :

- un module radio marqu� R(??)7RX(PCB) rev 1.2, 01RW67RX2L 1-210040-00. Mes recherches me donnent � penser qu'il pourrait s'agir d'un produit taiwanais nomm� [Richwave RW67RX](http://www.richwave.com.tw/product.php?CNo=9) (RT6712?). On trouve �galement mention de ce produit sur cette [curieuse page Wikip�dia](https://en.wikipedia.org/wiki/Spy_video_car).
- un amplificateur op�rationnel LM358, dont j'ignore l'utilisation
- un [APA4880](http://www.anpec.com.tw/ashx_prod_file.ashx?prod_id=122&file_path=20090109105809347.pdf&original_name=APA4880.pdf) qui semble �tre un amplificateur pour oreillettes (utilis� �galement pour le haut parleur ? non v�rifi�), un produit d'une soci�t� d�nomm�e Anpec qui m'est inconnue
- un r�gulateur lin�aire 560mA [APL5508](http://www.anpec.com.tw/ashx_prod_file.ashx?prod_id=412&file_path=20131021181317165.pdf&original_name=APL5508R/9R.pdf), �galement produit Anpec
- l'interrupteur s�lecteur de canal en haut de la photo
- un bouton qui active le r�tro�clairage de l'�cran
- un bouton qui active la fonction "vox", dont l'effet est... d'�teindre l'�cran si le b�b� ne fait pas de bruit. Tout en laissant le son. Vous avez donc le choix entre dormir avec un bruit blanc (plus diverses perturbations sur le canal), ou ne pas entendre votre b�b� car vous avez baiss� le volume pour pouvoir dormir. C'est un des �l�ments qui me fait dire que cet appareil est une d�sastre qui n'aurait jamais d� �tre vendu.
- un potentiom�tre de r�glage du volume
- un potentiom�tre de r�glage de "sensibilit�", dont je soup�onne fortement qu'il ne sert � rien. On peut imaginer qu'il d�termine � partir de quel seuil le r�cepteur va restituer le son, seulement celui-ci semble �tre restitu� en permanence, ind�pendamment de la valeur du potentiom�tre.

## C�t� face

![C�t� face](receiver_internal_back.jpg)
![Microcontr�leur](receiver_uc.jpg)

C'est de ce c�t� qu'on voit le plus de choses int�ressantes. On note que ce PCB est double face, et comprend un grand nombre de composants (y compris une quantit� impressionnante de condensateurs).

On observe :

- un microcontr�leur 8-bit bas de gamme [MDT10P22A3S](http://www.digchip.com/datasheets/download_datasheet.php?id=2799060&part-number=MDT10P22A3S)
- � nouveau deux r�gulateurs lin�aires 560mA APL5508 (!), tension 3.3V
- un d�codeur de vid�o analogique [ADV7180](http://www.analog.com/en/products/audio-video/video-decoders/adv7180.html) (dont la sortie est, j'imagine, reli�e � l'�cran, que je n'ai pas �tudi�)
- un chargeur de batterie NiMH [LS2364T](http://www.datasheetspdf.com/PDF/LS2364T/718641/1), qui semble �tre un [produit chinois](http://www.linkas.com.cn/e0.html)
- deux amplificateurs op�rationnels ST [LM324](http://www.st.com/web/catalog/sense_power/FM123/SC61/SS1378/PF63709) dont j'ignore l'utilisation

Ce qui est int�ressant sur ce circuit c'est la pr�sence d'un circuit de recharge NiMH que j'aimerais conserver, et pourquoi pas - si je trouve o� - conserver �galement le circuit d'amplification pour le haut-parleur ainsi que le potentiom�tre de r�glage du volume.
<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>

