---
layout: post
title: "Vidange de terrasse trop�zienne avec panneau solaire"
date: 2016-06-06 06:30:17
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/terrasse"
---


L'appartement que je loue dispose d'une terrasse trop�zienne : c'est une terrasse fabriqu�e en enlevant une partie du toit de la maison et en �tanch�ifiant le sol. Voir par exemple ici : http://www.deco.fr/jardin-jardinage/terrasse/actualite-493004-terrasses-tropeziennes.html
![Terrasse trop�zienne](tropezienne.jpg)

Ma terrasse utilise des dalles en bois exotique pos�es sur des plots en plastique, et fait environ 3m x 3m.

# �levage de moustiques

J'ai observ� un probl�me d�s l'emm�nagement dans cet appartement : apr�s une p�riode de pluie, la terrasse ne se vidange pas correctement, et il reste par endroits de l'eau stagnante pouvant monter jusqu'� 2 cm. Co�ncidence ou non, il y avait de nombreux moucherons et moustiques lorsque j'ai emm�nag�.
Il y a deux d�fauts.

- La terrasse dispose bien d'une �vacuation d'eau dans un coin, mais celle-ci est situ�e pas loin de 2 cm au dessus du reste de la terrasse, et ne me semble pas facile � modifier (car les plaques de bitume pour l'�tanch�it� sont relev�es sur les bords de la terrasse, et si je creuse un peu je risque d'envoyer l'eau dans l'appartement du dessous).
- La pente pour l'�vacuation est tr�s insuffisante, peut-�tre � cause d'une mauvaise r�alisation, ce qui fait que l'eau n'est pas dirig�e vers l'�vacuation mais s'accumule � d'autres endroits.

J'ai parl� du souci au propri�taire, qui ne me frappe pas comme �tant un grand bricoleur, et qui a fait le mort (probablement apr�s avoir vu le devis de l'artisan qui est venu et est rest� dix minutes). Il n'est pas improbable, au vu de ce que j'ai lu, que cet artisan lui ait par ailleurs dit que de l'eau  stagnante sur ce type de terrasse n'�tait pas un d�faut. Ce qui est s�rement vrai du point de vue de l'artisan cherchant � justifier du travail douteux, mais nettement moins du point de vue de l'habitant !

# Vidange 

Par cons�quent j'ai d�cid� de prendre les choses en main, et j'ai donc achet� une pompe et du tuyau, et j'ai mont� un prototype sur piles dont le co�t total est de moins de 15 euros. La pompe que j'ai utilis� est une pompe �PasCher, qui semble con�ue pour les aquariums, achet�e [ici](http://www.ebay.com/itm/182072189622).
![Pompe d'aquarium](pompe_ebay.jpg)
Elle fonctionne en (tr�s, tr�s) basse tension, a bien s�r une puissance et un d�bit ridicule, mais il se trouve que cela suffit. Le magasin de bricolage local vendait du tuyau au m�tre que j'ai utilis� pour relier la pompe (plac�e dans un des points bas de la terrasse o� l'eau s'accumule) � l'�vacuation.

![Prototype: pompe en place](pompe.jpg)
![Prototype: �vacuation](tuyau.jpg)
![Prototype: pompe cach�e](closed.jpg)

En fonctionnement normal, la vidange de la terrasse est r�alis�e par cette pompe en environ 2 heures. Dans un premier temps j'alimente la pompe avec 3 piles NiMH et un circuit boost +5V (qui sur la photo sont dans un sac cong�lation sur la terrasse, et � terme seront bien s�r cach�s sous les dalles).

## D�fauts

- La vidange n'est pas compl�te et laisse environ un millim�tre d'eau : la pompe d�samorce s'il y en a moins.
- J'ai not� quelques difficult�s � l'amor�age, m�me lorsqu'il y a beaucoup d'eau. Je pense que cette pompe est vraiment en limite de puissance pour pousser l'eau dans 2m de tuyau et 2cm de d�nivel�. En effet si je retire le tuyau l'amor�age est imm�diat, et en repla�ant le tuyau le pompage continue sans probl�me. Ces difficult�s � l'amor�age semblent se pr�senter une fois sur quatre, ce qui est assez g�nant. J'ai remarqu� qu'en appliquant des impulsions (arr�ter et red�marrer la pompe plusieurs fois), l'amor�age semblait plus facile � r�aliser. 

# La suite : syst�me automatique !

Ce premier prototype me donne satisfaction, et j'ai d�cid� de racheter une deuxi�me pompe pour faire une vidange plus exhaustive. J'ai pour projet d'automatiser le montage (avec d�tection du niveau d'eau et panneau solaire), j'aborderai la question dans un deuxi�me article.


<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>
