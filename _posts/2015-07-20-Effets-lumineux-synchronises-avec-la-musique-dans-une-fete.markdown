---
layout: post
title: Effets lumineux synchronis�s avec la musique dans une f�te
date: 2015-07-20 10:27:17
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/party_strobe"
---

� l'occasion d'une f�te, j'ai r�alis� un montage permettant d'avoir des effets lumineux sur le dancefloor.

# Objectifs

L'objectif est d'analyser en temps r�el la musique et de piloter des effets lumineux d'ambiance.

On se fixe comme objectif de r�aliser trois type d'effets d'ambiance :

- fondu entre plusieurs couleurs vives � rythme lent (pour les moments plus calmes tels que le repas)
- allumage synchronis� avec le rythme de la musique (pour faire danser)
- stroboscope � fr�quence variable (activ� manuellement et ponctuellement)

La s�lection de l'effet doit �tre r�alisable � la main par n'importe qui, y compris un invit� ivre.

# R�alisation : mat�riel

Cette r�alisation a, comme souvent, �t� faite dans l'urgence avec les moyens du bord. Pour la lumi�re, on choisira un bandeau de LED RGB, de pr�f�rence en 5050, et de longueur suffisante par rapport � la taille de la pi�ce � �clairer. Ce bandeau devra �tre pilot� par des transistors de puissance correctement dimensionn�s, par facilit� j'ai utilis� un des ["amplificateurs"](/~sven337/english/2015/01/19/Tearing-down-a-RGB-LED-strip-amplifier.html)[EN] que j'avais. Ces petits circuits sont con�us pour amplifier le signal sur de tr�s longs rubans � LED, mais en les diss�quant on peut r�cup�rer les transistors de puissance. Ceux-ci sont un peu faiblards mais c'est tout ce que j'avais sous la main.
L'alimentation sera un classique bloc 12V DC 6A, le courant �tant � adapter en fonction de la longueur du bandeau. Il faut garder � l'esprit que le courant moyen sera assez faible par rapport aux sp�cifications maximales th�oriques : en mode stroboscope, le rapport cyclique vaudra 50%, et il sera globalement plus faible dans les autres modes car on utilisera rarement R G et B en m�me temps et � pleine puissance (ou alors on choisirait un bandeau blanc, qui revient moins cher).

Pour l'�lectronique de contr�le, j'ai utilis� l'�ternel ["Arduino pro mini"](http://www.ebay.com/itm/Redesign-Pro-Mini-atmega328-3-3V-8M-Replace-ATmega128-Arduino-Compatible-Nano-/200914924969), en tout cas son clone chinois dont le prix est imbattable. Le mod�le que j'utilise est un 3.3V � 8MHz, et non un 5V � 16MHz (la tension plus faible est utile pour les montages sur batterie). En cons�quence la puissance de calcul disponible sera assez faible et cela pourrait poser probl�me pour l'analyse de la musique.

Pour intercepter le signal sonore, il faut un connecteur jack 3.5mm femelle, et le m�me connecteur m�le. J'ai r�cup�r� le connecteur femelle sur un vieux lecteur de CD-ROM dont j'ai sci� le PCB  ; et j'ai r�cup�r� le connecteur m�le sur un vieux c�ble jack 3.5mm. De cette fa�on, au lieu de brancher l'amplificateur directement � la sortie son de l'ordinateur, on intercale mon montage.

![Connecteur jack femelle](jack_female2.jpg)
![Connecteur jack femelle - envers](jack_female.jpg)
![Connecteur jack m�le](jack_male.jpg)


L'interface pour interagir avec le montage prend, dans mon cas, la forme d'un microswitch et d'un potentiom�tre, tous deux r�cup�r�s sur un autre lecteur de CD-ROM dont j'ai � nouveau sci� le PCB. Le potentiom�tre servait � la base de r�glage du volume sonore, dans ce montage nous nous en servirons pour faire varier la fr�quence du stroboscope, le microswitch  permettant de changer entre les diff�rents effets (OFF, FADE, MUSIC, STROBE).

![Interface](microswitch_potentiometer.jpg)

![Montage complet](complete_electronics.jpg)

# R�alisation : logiciel - th�orie

J'ai d�j� r�alis� le fondu entre couleurs dans un autre projet que je n'ai pas encore document� : il s'agissait de r�impl�menter les effets lumineux fournis par les tr�s c�l�bres contr�leurs-avec-t�l�commande chinois. Ceux-ci sont d'assez mauvaise qualit� mais la t�l�commande est utile.
![Contr�leur-avec-t�l�commande chinois](chinese_controller.jpg)
� l'occasion je documenterai ce projet.

Le stroboscope est conceptuellement simple � �crire, et faire varier sa p�riode dynamiquement en fonction de la valeur du potentiom�tre ne pose pas de probl�me particulier. La principale difficul� porte �videmment sur l'analyse en temps r�el de la musique et c'est une partie qui m'a occup� plus de dix heures.

Il y a en fait deux probl�mes :

1. analyser la musique pour calculer des valeurs num�riques corr�l�es aux "temps forts" de la musique  (en d�tectant des "choses" int�ressantes dans la musique)
1. utiliser ces valeurs pour d�terminer les valeurs R, G, B � utiliser (et donc la couleur � donner au bandeau)

Ces deux �l�ments sont distincts, et il faut faire du bon travail sur les deux : si on r�alise une mauvaise d�tection, la couleur du bandeau ne variera pas en fonction des moments forts de la musique, et inversement si on r�alise une d�tection parfaite des moments forts mais que les effets lumineux ne suivent pas, le r�sultat sera sans int�r�t.

## Analyse de la musique

L'approche la plus na�ve serait de d�tecter les grandes variations d'amplitude. Cependant l'intuition donne � penser que cela fonctionnera tr�s mal, en particulier sur de la musique pop dont la dynamique a �t� compress�e de telle sorte que l'amplitude maximale (l'enveloppe) ne varie pas vraiment.

La m�thode naturelle qui vient � l'esprit est la transform�e de Fourier, qui r�alise une analyse fr�quentielle d'un segment de la musique, permettant de diff�rencier les basses des aigus, et, peut-�tre, d'en tirer des conclusions utiles.
On ne va pas s'amuser � r�impl�menter une transform�e de Fourier sur l'Arduino. En fait c'est un algorithme proche qu'on va utiliser : la [FHT](https://en.wikipedia.org/wiki/Discrete_Hartley_transform#Fast_algorithms), impl�ment�e dans une biblioth�que qui fera toutes les math�matiques pour nous : [ArduinoFHT](http://wiki.openmusiclabs.com/wiki/ArduinoFHT).

Le fonctionnement est le suivant : on va capturer avec le convertisseur analogique-num�rique (ADC) un certain nombre d'�chantillons de la musique qui est en train d'�tre jou�e. La FHT va analyser ces �chantillons et nous donner l'amplitude dans chaque intervalle de fr�quence. Ensuite, il nous appartiendra de regarder si ces valeurs changent en effet en fonction des temps forts de la musique, et si c'est le cas nous pourrons piloter les lumi�res.

Il est int�ressant d'utiliser le [*channel analyzer*](http://wiki.openmusiclabs.com/wiki/ArduinoFHT?action=AttachFile&do=view&target=FHT_128_channel_analyser.zip) fourni avec ArduinoFHT, qui permettra de visualiser graphiquement les diff�rentes amplitudes et de **voir** directement si celles-ci varient en fonction des temps forts.
La r�ponse est oui, mais de mani�re moins �vidente que ce qu'on aurait pu penser. Tout d'abord, chaque intervalle de fr�quence a sa propre amplitude "de base" : quand aucun son ne passe, les valeurs d'amplitude ne sont pas � 0. Dans les basses, la valeur de base est de l'ordre de 192 (sur 256), ce qui signifie que l'amplitude de variation (de l'amplitude !) sera assez faible, donc potentiellement difficile � d�tecter.
On remarque �galement (en envoyant des signaux sinuso�daux de fr�quence donn�e avec `` speaker-test -t sine -f 400 ``) qu'il y a une corr�lation non-souhait�e entre les diff�rents intervalles de fr�quence : par exemple un signal � 5kHz va faire r�agir (dans notre analyse) l'intervalle contenant 5kHz, mais �galement les intervalles adjacents. Cela veut dire que la pr�cision de l'analyse est quand m�me limit�e et qu'il ne faut pas trop y faire confiance.

N�anmoins, en passant de la musique du genre de ce qu'on passe en soir�e (non, je ne donnerai pas de lien), il est facile de remarquer visuellement, avec le *channel analyzer*, que les valeurs �voluent r�ellement en fonction des temps forts, ou du moins en fonction des "beats" (coups de percussion, de fr�quence plut�t basse). Je suis d�sol� de n'avoir conserv� aucune capture d'�cran.

En gros, la FHT telle qu'impl�ment�e permet de d�tecter les beats, et pas grand chose de plus. Est-ce suffisant ? On verra plus loin que oui, mais j'ai d� exp�rimenter une dizaine d'heures avant de trouver le bon algorithme pour le pilotage.

## Pilotage des lumi�res

On sait d�sormais que notre analyse, si elle fonctionne, n'est pas d'une pr�cision extr�me et va donc n�cessiter une certaine quantit� de post-traitement. Il faut noter que je pr�sente ici mes r�sultats dans l'ordre "logique", mais en r�alit� j'ai br�l� les �tapes et tent� de piloter directement les lumi�res avec la sortie de la FHT (comme de nombreuses r�alisations sur Internet le proposent ! et sans surprise la plupart sont mauvaises). C'est lorsque que je me suis rendu compte que le r�sultat �tait nul que j'ai d� r�fl�chir un peu plus s�rieusement, utiliser le *channel analyzer* pour �tudier le comportement de la FHT, et en d�duire l'algorithme � utiliser.

L'id�e de base �tait d'attribuer une bande de fr�quence par canal de couleur : le bleu pour les basses, le vert pour les medium, et le rouge pour les aigus. Or, cela ne marche absolument pas, pour deux raisons :

1. la d�tection des medium et des aigus ne marche pas tr�s bien (notamment la discrimination entre les deux est assez inefficace)
1. les medium, les aigus, et les basses arrivent souvent en m�me temps, g�n�rant une superbe couleur R+G+B = blanc-tout-moche, ce qui n'exploite pas le potentiel d'un bandeau RGB

Apr�s de tr�s nombreuses exp�rimentations (qui ont incrust� la m�lodie de Axel F. dans mes r�ves pendant une nuit) et une discussion avec des amis, j'ai retenu l'id�e d'une approche [HSV](https://en.wikipedia.org/wiki/HSL_and_HSV) au lieu de RGB. Ici, on va utiliser les basses pour piloter la valeur (= la luminosit�) du bandeau, et les aigus (sans s'emb�ter � savoir si c'est vraiment aigu ou seulement medium) pour piloter la **vitesse de variation** de la teinte (hue). La saturation, elle, reste tout le temps au maximum : si on a achet� un bandeau lumineux ce n'est pas pour faire des couleurs fades.

Afin d'�liminer la composante constante des valeurs d'amplitude, le post-traitement consiste � calculer la moyenne glissante exponentielle (la plus simple qui soit � impl�menter) de l'amplitude. La diff�rence entre cette moyenne glissante et la valeur instantan�e est la grandeur utilis�e pour piloter la lumi�re.

Puisqu'on a observ� que la d�tection des basses �tait assez fiable, c'est celles-ci qu'on va utiliser exclusivement pour d�cider de l'intensit� lumineuse - et chaque coup de grosse caisse est accompagn� de son "flash" lumineux. Les aigus ont de toute fa�on moins d'importance dans les temps forts de la musique, donc ils vont avoir un effet pr�sent mais moins �vident sur la couleur du bandeau � travers le changement de teinte.

# Code

Le "sketch" Arduino pour ce projet se trouve [ici](https://github.com/sven337/jeenode/blob/master/party_strobe/party_strobe.ino).

<script>
    $(document).ready(function() {
        $("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>

