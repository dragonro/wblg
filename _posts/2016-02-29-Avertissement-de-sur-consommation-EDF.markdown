---
layout: post
title: Avertissement de sur-consommation EDF
date: 2016-02-29 10:36:11
tags: electronique
category: francais
comments: true
img_rel: "/~sven337/data/teleinfo"
---

J'ai pr�sent�, il y a bien longtemps, mon montage de [suivi de consommation �lectrique](/~sven337/francais/2014/03/09/Suivi-de-consommation-elec-avec-R-Pi.html) utilisant la t�l�info EDF. Jusqu'� pr�sent je m'en servais assez peu : pour m'�pargner le d�placement jusqu'au compteur lorsque, quelques fois par an, mon fournisseur d'�nergie me demande une auto-rel�ve ; et pour tirer des courbes que je ne regarde au demeurant que tr�s peu souvent (celles de temp�rature sont parfois utiles, celles de consommation �lectrique, tr�s rarement).

![Exemple de courbe de consommation �lectrique - liss�e sur la journ�e](teleinfo_jsgraph2.jpg)

Une utilisation concr�te fait suite � l'acquisition par mon foyer d'un s�che-linge, sans augmentation de la puissance de mon abonnement d'�lectricit� (car c'est tr�s cher et peu justifi�). Pour �viter les coupures de courant intempestives qui arrivent lorsque le s�che-linge est utilis� au mauvais moment (c'est-�-dire en m�me temps que le four, la machine � laver, les plaques � induction, ...), j'ai modifi� ma petite application Flask afin d'y ajouter un avertissement d�s que la puissance efficace d�passe 5500W (mon abonnement est de 6000VA, mais j'ai remarqu� que le disjoncteur principal �tait assez laxiste et me permettait de monter jusqu'� 6500W avant coupure). J'ai fix� ce seuil empiriquement de telle sorte qu'il corresponde � une situation critique (il faut r�agir *vite* sinon *�a va couper*), mais rattrapable (en �teignant quelques appareils on �vite la coupure g�n�rale). 

Le script d'avertissement m'envoie un e-mail, un SMS, et met le son au maximum sur l'eeePC qui me sert de serveur afin de jouer un mp3 d'alarme. La combinaison de ces m�thodes de notification suffit en g�n�ral � attirer mon attention � temps.

``` bash
#!/bin/bash
amixer sset PCM 255
amixer sset Master 100
amixer sset Speaker 100
mpg123 alarm.mp3 &
echo "POWER WARNING $1 W"| ~/sms-send-notification.sh
echo "POWER WARNING $1 W" | mail -s 'POWER WARNING'  root
```

Cela me permet de continuer � lancer le s�che-linge au mauvais moment, sans payer le prix de mon �tourderie sous la forme d'une coupure g�n�rale d'�lectricit� qui est toujours aga�ante (et sensiblement dommageable au syst�me d'exploitation des multiples ordinateurs qui fonctionnent � la maison, et par exemple du Raspberry Pi que j'utilise comme pour certaines fonctionnalit�s domotiques... il faut que je mette en place un syst�me de batterie de secours pour cet appareil).

<script>
    $(document).ready(function() { 
        $("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script> 

