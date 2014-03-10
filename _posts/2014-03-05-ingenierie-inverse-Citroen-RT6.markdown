---
layout: post
title: "Ing�nierie inverse de l'autoradio eMyWay (RT6) Citro�n"
date: 2014-03-05 23:00:00
tags: electronics auto
category: francais
comments: true
---

Je reprends ici un article que j'ai r�dig� il y a plus d'un an concernant l'autoradio eMyWay disponible sur les Citro�n C4. Cette version est plus compl�te et a l'int�r�t d'�tre en un seul morceau.

Notez que je ne poss�de plus ni de C4 ni de RT6, je ne suis donc plus actif sur ce projet.

M�thode: **�tude du CD d'upgrade firmware 2.20**

# 1. Travaux pr�c�dents

Le RT6, d�velopp� par Magneti Marelli, semble � premi�re vue �tre assez similaire au RT3, pour lequel un effort de documentation a d�j� �t� fait. On
est en pr�sence d'une plateforme **VxWorks 5.5.1** avec Tornado 2.2.1. La carte m�re est probablement similaire � une WindRiver d'architecture PowerPC
(exemples de produits : [http://www.windriver.com/products/bsp_web/bsp_architecture.html?architecture=PowerPC](http://www.windriver.com/products/bsp_web/bsp_architecture.html?architecture=PowerPC)) La carte est-elle la m�me que le RT3 ? Je pensais que le RT3 �tait en MIPS mais [http://fr.viadeo.com/fr/profile/cyrille.lohier](http://fr.viadeo.com/fr/profile/cyrille.lohier) laisse penser le contraire. Les binaires semblent �tre produits avec GCC: `(GNU) gcc-2.96 (2.96+ MW/LM) AltiVec VxWorks 5.5`.

Le RT4 a une architecture tr�s similaire et a �t� l'objet d'efforts d'ing�nierie inverse men�s par diff�rentes personnes dont plusieurs ont post� en fran�ais sur [Planete Citroen](http://www.planete-citroen.com). On notera qu'il existe une version RT6 des "mira scripts", en cours de d�veloppement ([http://mira308sw.altervista.org/en/index.htm](http://mira308sw.altervista.org/en/index.htm)). Mira conna�t tr�s bien l'appareil.

# 2. �tude du CD d'upgrade firmware 2.20

On trouve sur le CD:

  * des fichiers ".inf" 
  * des fichiers ".CMD" et ".ini" 
  * des fichiers ".out" 
  * des fichiers ".gz" 
  * ... 

## 2.1. Fichiers .inf

Ce sont des fichiers d�crivant un binaire, le binaire semble porter le m�me nom que le fichier mais sans le ".inf" (a.bin.inf d�crit le fichier a.bin).
Exemple :
```
    c1d5be54
    VER:295
    TYPE:DATA
    COMPRESSED:NO
    SIZE:1356848
    ENTRY:NO
```    

Pour obtenir la liste exhaustive des champs utilis�s par le firmware :

    
``` bash  
    find . -name '*.inf' | xargs tail -q -n +2 | cut -f1 -d: | sort | uniq
```    

On trouve :

    
    
    COMPRESSED
    ENTRY
    ID
    SIZE
    SUBVER
    TYPE
    USIZE
    VER
    

  * 1�re ligne : 2 valeurs hexa sur 16 bits chacune (4 caract�res). La premi�re valeur est un **CRC** (polyn�me sp�cifique, voir plus bas) du fichier INF lui m�me. On l'obtient en calculant le CRC sur ce fichier � partir de l'octet n�5 inclus (seek de 4). La deuxi�me valeur est un CRC du fichier binaire correspondant au INF, calcul� sur l'int�gralit� du binaire (compress� s'il est compress�). 
  * VER : version du fichier 
  * TYPE : soit "DATA" (donn�es ?) soit "RELOCABLE" (code ex�cutable) 
  * COMPRESSED : indique si le fichier binaire correspondant est compress� ou non. Le format de compression est DEFLATE tel qu'impl�ment� par zlib (outil zpipe), avec une petite subtilit� au niveau du format 
  * SIZE : taille du fichier tel que stock� (compress� ou non) 
  * USIZE : **u**ncompressed **size**, taille du fichier apr�s d�compression (pr�sent uniquement sur les COMPRESSED) 
  * ENTRY: YES ou NO... (aucun id�e de ce que c'est, � v�rifier) 
  * SUBVER : "sous version" ?? 
  * ID: "sous sous version" ?? 
<br />
 
## 2.2. Fichiers .CMD et .ini

On dirait que c'est une habitude chez MM d'utiliser des extensions pour des types de fichiers qui ne correspondent pas du tout. Les .CMD et .ini sont...
du **code C** ! En r�alit� ce code est interpr�t� par une biblioth�que d�nomm�e [EiC](http://www.linuxbox.com/tiki/node/149), qui permet d'�crire des scripts avec une syntaxe tr�s similaire au C.

## 2.3. Fichiers .out

Il s'agit d'ex�cutables.

```./Application/Boot/ssm_boot.out: ELF 32-bit MSB relocatable, PowerPC or cisco 4500, version 1 (SYSV), not stripped

Architecture **PowerPC**, BigEndian, 32 bit. D�sassemblable avec les binutils GNU compil�s pour PowerPC... ils ont m�me laiss� les symboles de debug (= les noms des fonctions) ! Merci MM !

## 2.4. Fichiers .gz

Ce sont des ex�cutables comme les .out, mais compress�s. Voir plus bas.

## 2.5. Bootrom

Il semble y avoir un fichier sp�cial, qui est un ex�cutable non compress� qui n'est pas ELF. C'est probablement le _BIOS_ de la carte. C'est le fichier `Application/Boot/RNEG2010_EUR_2_20/DG4/BOOTROM.DAT`. Ce fichier contient un certain nombre de cha�nes de caract�res int�ressantes. Sa version est `BSP_PPC-SECT83+BSP_PPC_6.86k1:project:SECT39+1`. C'est probablement du code binaire sans structure particuli�re, il faudra essayer de le d�sassembler comme si c'�tait du code machine brut.

## 2.6. BootRom.sym

Attention ce fichier n'est pas le m�me que dans la section pr�c�dente. C'est un ex�cutable PowerPC statique dont les symboles n'ont pas �t� supprim�s : on peut le d�sassembler pour l'�tudier. C'est lui qui semble fournir la majorit� des fonctions utilis�es dans le firmware, par exemple **CheckCRCFile**.

## 2.7. Hash des fichiers

Les fichiers .inf ont au d�but une valeur qui correspond � un hash **CRC**. J'ai essay� plusieurs possibilit�s - CRC32 avec divers polyn�mes connus, Adler32, sans succ�s. La formule de calcul du hash n'est donc pas �vidente - mais en lisant le code on se rend compte que c'est un hash sur 16 bits et non 32 bits. Si on regarde dans le code de BootRom.sym on se rend compte que *CheckCRCFile* appelle *CheckCRCInf* qui lui m�me fait deux appels :

  * l'un � `ReadINFCRC__FPCcR9Crc16Type` (Le nom bizarre correspond au "mangling" des fonctions C++ fait par GCC. Cela correspond � la signature : `ReadINFCRC(char const *, Crc16Type &)`) 
  * l'autre � `ComputeCRCFile`

J'ai manuellement recr�� le code de calcul du hash � partir du code machine. Il op�re par tranche de 8 bits, avec deux tables de lookup, et des �changes... ce n'est pas un Adler16. Il peut s'agir d'un algo "maison", dans ce cas le code ci-dessous associ� au contenu des tables permettra de recr�er les hash si on veut faire des modifs. J'ai �crit un programme en C pour [calculer les hashs](data/rt6/crc_rt6.c).

Exemple sur `/Application/BTL/File_Search.gz.inf` - le hash en d�but de fichier est **0c622f64**. **0c62** est le hash du **.inf**, **2f64** est le hash du **binaire**, comme on peut le voir :
 
~~~ 
    $ ./crc_rt6 /mnt/hd/Application/BTL/File_Search.gz.inf 4
    Computed CRC16 0x0c 0x62
    $ ./crc_rt6 /mnt/hd/Application/BTL/File_Search.gz 0
    Computed CRC16 0x2f 0x64
~~~


## 2.8. Compression des binaires

La plupart des ex�cutables sont des fichiers .gz compress�s. Un peu d'�tude du code nous am�ne � la fonction **inflate** qui est utilis�e pour d�compresser ces binaires. Malheureusement cette fonction ne semble pas avoir le comportement standard de la zlib. Un peu de Google nous m�ne � [cette page](http://read.pudn.com/downloads58/sourcecode/embed/205887/src/util/inflateLib.c__.htm). Tout en bas du fichier, la fonction `inflate` semble correspondre au code source de celle du RT6. Il reste � v�rifier en quoi cette fonction diff�re de ce que fait la zlib nativement et nous pourrons d�compresser (et je l'esp�re recompresser) les binaires du RT6. Avec d�compression + CRC, on pourra commencer � s'amuser � tout casser.

VxWorks ajoute en fait un octet de _bourrage_ au d�but de chaque fichier compress� (qui sert pour calculer un checksum, mais uniquement si la variable **inflateCksum** est d�finie - adresse `0xff4cf04` dans **BootRom.sym** qui est l'ex�cutable principal, et elle ne l'est pas pour l'instant). Cet octet fait que le format n'est pas reconnu par l'outil zpipe inclus dans zlib. Il faut donc faire une petite modification sur **zpipe** dans `main()`:

~~~ c
    /* do decompression if -d specified */
    else if (argc >= 2 && strcmp(argv[1], "-d") == 0) {
    if (argc == 3) {
    int sz = atoi(argv[2]);
    char buf[sz];
    fread(buf, sz, 1, stdin);
    }
    ret = inf(stdin, stdout);
    if (ret != Z_OK)
    zerr(ret);
    return ret;
    }
~~~
    

Cela permet de passer un _seek_ en argument. La valeur de _seek_ � utiliser est 1. On peut ensuite d�compresser n'importe quel binaire du RT6 de la mani�re suivante :

        
    zpipe -d 1 < file.gz > file.bin
    

J'ai fait cela sur tous les binaires compress�s du firmware. Pas de surprise, ce sont bien des binaires PowerPC, avec les symboles de debug.

## 2.9 Le processus de mise � jour

On a ``/F`` qui est la partition de boot. Probablement du flash, probablement soud� sur la carte m�re, peut-�tre une carte SD, � voir. (XXX mettre � jour selon les r�sultats des exp�rimentations qui ont eu lieu depuis) On a ``/SDC`` qui est la partition d'application. SDC comme **SD C**ard, je suppose.

### 2.9.1 Initialisation

Lors de l'insertion d'un CD d'upgrade du firmware, le syst�me r�agit � la pr�sence du fichier ``CD.inf`` et ex�cute ``FlasherROMStart("/path/to/cd")`` dans ``/UPG/Flasher/FLASHER.ROM`` (indication de Mira non v�rifi�e). Celui-ci d�tecte la version de l'appareil et si c'est bien un RNEG il ex�cute le fichier ``/UPG/Flasher/FLASHER.ROM.RNEG``. Cela prouve que le RT6 est globalement similaire aux RT3/RT4 qui l'ont pr�c�d�.
Dans ``FLASHER.ROM.RNEG`` on trouve la fonction ``FlasherRomRNEGStart(char *sourcedrive)`` qui appelle ``GetHardwareConfiguration("CFG_HW_FAMILY")`` pour v�rifier (� nouveau ?!) si c'est bien un RNEG, et �crit la r�ponse (= 1 sur RNEG) dans deux variables: 
- C_SETUP_HW::m_is_rneg_family
- C_SETUP_HW::m_is_sd_present. 

Ensuite, ``C_SETUP_HW::m_is_preampli_present`` et ``m_is_mtb_present`` sont renseign�s. Apparemment "mtb" signifie motherboard (carte m�re). J'esp�re bien, que la carte m�re est pr�sente... Il y a une �galement une variable ``mtb_tuner_type``... qui prend les valeurs 0 1 2 3).

Un affichage pour le _debug_ semble �tre r�alis� par ``Splash_PrintL1__FPCc``.

Le stockage flash est sur``/F``. La partition subit les appels suivants, dans l'ordre : 

~~~ 
KernelUnprotectFlash
UnMountPartition
MountAndCheckTffs
BootRomFormatTffs
KernelProtectBoot
~~~

**TFFS** semble donc �tre le format du FS. <http://en.wikipedia.org/wiki/Flash_file_system#TrueFFS> sous entend que c'est un FS utilis� par VxWorks, donc c'est une possibilit� tr�s nette que ce soit bien notre FS.

### 2.9.2 V�rification du CD
Une fois le FS format�, c'est la fonction ``LaunchSoftUpgrade`` qui prend le relais. Le RT6 utilise la biblioth�que **EiC** (<http://www.linuxbox.com/tiki/node/149>, merci � Mira) pour fournir des scripts dont la syntaxe est celle du langage C. EiC va d�finir un ensemble de fonctions C qui seront appelables depuis les scripts .CMD. Je disposais de la liste exhaustive de ces fonctions mais suite � une erreur de manipulation je ne les ai plus.
Ces fonctions sont export�es � travers EiC en appelant ``EiC_AddBuiltinFunc(const char *, void *(*func)(void))``. (Par exemple, ``EiC_AddBuiltinFunc("MaFunctionAMoi", &MaFunctionAMoi)``). On doit pouvoir en rajouter assez facilement de cette mani�re, mais Mira a utilis� une autre technique � mon avis plus compliqu�e.

Une fois **EiC** initialis�e, le programme cherche le fichier ``[source]/UPG/Command/CHECK_CD.CMD`` ([source] correspond � l'adresse du "device" contenant la mise � jour, je ne sais pas encore quels sont les points de montage). Ce script est tr�s explicite, �crit par un certain Philippe Chapelet <http://fr.linkedin.com/pub/philippe-chapelet/45/834/bb7>.

``EiC_ExeFile(argc, argv)``, **CHECK_CMD** est appel� avec ``argv[2]="BOOTROM"`` (TODO v�rifier s'il y a un appel avec "NORMAL" ou une autre valeur, il existe aussi "RECOVERY", voir comment c'est utilis�), les 2 autres arguments sont l'adresse source et le path du script lui m�me (ce qui est confirm� par le contenu du script).

### 2.9.3 Mise � jour

Le script de v�rification retourne 0 si tout est OK. Dans ce cas, la mise � jour va avoir lieu. Le programme cherche le script d'upgrade, ``[source]/UPG/Command/FLASHER.ROM.RNEG.CMD``. Ce script est appel� avec les m�mes arguments qu'au-dessus.
C'est lui qui fait l'essentiel du travail.

### 2.9.4 Apr�s la mise � jour

Une fonction v�rifie le flag ``g_Flasher_ROMERROR``. **0** = pas d'erreur, **3** = message d'**erreur** + SetDBBootFlagError(), 1 2 4 message d'erreur, en cas d'erreur "emergency reboot".

## 2.10 Questions restantes

� faire : d�tails du fonctionnement de l'upgrade firmware. Questions: est-ce qu'on peut ne mettre � jour qu'un seul fichier ? Est-ce qu'on peut faire des changements et revenir en arri�re sur une version officielle du firmware ?

Le syst�me reconna�t que le m�dia est une mise � jour du firmware � travers la pr�sence d'un fichier CD.inf, et il ex�cute le binaire `/upg/flasher/flasher.rom`.


# 3. Fonctionnalit�s Bluetooth

Bluetooth d�finit plusieurs **profils**, qui correspondent � un ensemble de fonctionnalit�s rendues par un appareil � un autre. Pour la musique, il existe **AD2P** qui est le profil permettant de transf�rer le son par radio, et **AVRCP** qui permet le contr�le de l'appareil lecteur par un autre (play, pause, next, previous, ...). Je souhaite savoir quelle version du profil Bluetooth AVRCP mon v�hicule supporte. Cette information n'est disponible nulle part mais l'�tude du module Bluetooth pourra r�pondre � la question. On trouve dans `Application/BCM/t2bf/bcm_t2bf.bin` (note: je nomme **.bin** les fichiers obtenus par d�compression du **.gz**) les cha�nes suivantes :


```
AVRCP version 1.0 supported
AVRCP version 1.3 supported
AVRCP unknown version
```

Donc le RT6 supporterait la version 1.3 d'AVRCP. Il y a une version 1.4 qui ajoute une fonction recherche et la possibilit� de g�rer plusieurs _players_ (par exemple deux t�l�phones simultan�ment pour streamer de la musique). <http://en.wikipedia.org/wiki/Bluetooth_profile#Audio.2FVideo_Remote_Control_Profile_.28AVRCP.29>

**sdptool** sous Linux liste les profils support�s par un appareil Bluetooth... mais **BT_CAR_SYSTEM** (le RT6) ne r�pond pas aux requ�tes. En tout cas cela m'a permis de d�tecter la version d'AVRCP sur mon Blackberry et mon YP-P2 - ces deux appareils sont en 1.0, donc la voiture ne pourra pas afficher les m�ta donn�es ni m'indiquer la liste des pistes. Conclusion il vaut mieux brancher ces appareils en **USB** (sauf que le Blackberry n'est pas accept� par le RT6 en USB, "m�dia illisible", probablement � cause de la table des partitions - en effet le BB expose une table des partitions avec une seule partition, alors que la plupart des cl�s n'ont pas de table des partitions, � v�rifier si c'est effectivement le souci, dans ce cas on serait en pr�sence d'une erreur/oubli de la part de MM). 

# 4. Architecture mat�rielle

## 4.1 Stockage
Le RT6 est �quip� d'une carte SD de 8Go qui sert de stockage syst�me au format TFFS, ainsi que d'une EEPROM qui stocke des param�tres de configuration persistants. La carte SD permet de r�aliser certaines manipulations sans risque (car on peut facilement la sauvegarder). L'EEPROM peut donner lieu � des retours en garantie qui se sont av�r�s co�teux. Il convient d'�tre particuli�rement prudent avec le _MiraScript_ **CONFIGFLAG**.
Il n'y a pas de disque dur, et pas de stockage volumineux qui permet de s'en servir en Jukebox : obligation d'utiliser le port USB si on veut avoir un stockage important.

## 4.2 D�montage
Voici des photographies prises par quelqu'un qui a d�mont� son RT6. Je les �tudie plus bas.

<a href="data/rt6/01.jpg" title="1"><img src="data/rt6/01-thumb.jpg" /></a>
<a href="data/rt6/02.jpg" title="2"><img src="data/rt6/02-thumb.jpg" /></a>
<a href="data/rt6/03.jpg" title="3"><img src="data/rt6/03-thumb.jpg" /></a>
<a href="data/rt6/04.jpg" title="4"><img src="data/rt6/04-thumb.jpg" /></a>
<a href="data/rt6/05.jpg" title="5"><img src="data/rt6/05-thumb.jpg" /></a>
<a href="data/rt6/06.jpg" title="6"><img src="data/rt6/06-thumb.jpg" /></a>
<a href="data/rt6/07.jpg" title="7"><img src="data/rt6/07-thumb.jpg" /></a>
<a href="data/rt6/09.jpg" title="9"><img src="data/rt6/09-thumb.jpg" /></a>
<a href="data/rt6/10.jpg" title="10 - puce GPS"><img src="data/rt6/10-thumb.jpg" /></a>
<a href="data/rt6/11.jpg" title="11"><img src="data/rt6/11-thumb.jpg" /></a>
<a href="data/rt6/12.jpg" title="12 - microcontr�leur"><img src="data/rt6/12-thumb.jpg" /></a>
<a href="data/rt6/13.jpg" title="13 - audio & radio"><img src="data/rt6/13-thumb.jpg" /></a>
<a href="data/rt6/14.jpg" title="14 - microcontr�leur"><img src="data/rt6/14-thumb.jpg" /></a>
<a href="data/rt6/15.jpg" title="15 - audio & radio"><img src="data/rt6/15-thumb.jpg" /></a>
<a href="data/rt6/16.jpg" title="16"><img src="data/rt6/16-thumb.jpg" /></a>
<a href="data/rt6/18.jpg" title="18 - Minus & Cortex"><img src="data/rt6/18-thumb.jpg" /></a>
<a href="data/rt6/19.jpg" title="19"><img src="data/rt6/19-thumb.jpg" /></a>

<!--[1](data/rt6/01.jpg)
[2](data/rt6/02.jpg)
[3](data/rt6/03.jpg)
[4](data/rt6/04.jpg)
[5](data/rt6/05.jpg)
[6](data/rt6/06.jpg)
[7](data/rt6/07.jpg)
8 - sans int�r�t
[9](data/rt6/09.jpg)
[10 - puce GPS](data/rt6/10.jpg)
[11](data/rt6/11.jpg)
[12 - microcontr�leur](data/rt6/12.jpg)
[13 - audio & radio](data/rt6/13.jpg)
[14 - microcontr�leur](data/rt6/14.jpg)
[15 - audio & radio](data/rt6/15.jpg)
[16](data/rt6/16.jpg)
17 - sans int�r�t
[18 - Minus & Cortex](data/rt6/18.jpg)
[19](data/rt6/19.jpg)
-->

### GPS

La puce **GPS** dans le RT6 est soit une **Atmel**, soit une **SIRF**, d'apr�s le code. La photo n�10 nous montre un chip **SIRF GSC2Xi**, ce qui semble correspondre au produit **SIRFStarII**. Bien s�r, cette entreprise supprime de son site les anciennes r�f�rences, et je n'ai pas pu trouver de datasheet. J'aurai l'occasion un jour de d�crire tout le bien que je pense que ces pratiques.

### Microcontr�leur

Sur les photos n�12 et 14 on trouve un microcontr�leur : <http://www.datasheetarchive.com/M30290FCTHP-datasheet.html>.
Si on savait � quoi est reli� le connecteur noir, on saurait � quoi il sert.

### Circuit audio

Photo n�13 et 15, la "carte son" **SAF7741** <http://www.nxp.com/documents/leaflet/75016755.pdf> associ�e aux deux tuners **TEF 7000** (en petit au dessus). Il faut que je trouve la datasheet du SAF7741, mais j'ai l'impression qu'il n'a que des sorties analogiques vers les HP (donc **pas de SPDIF**).

### Cortex

Photo n�18, un FPGA **Altera Cyclone 3** <http://www.altera.com/literature/hb/cyc3/cyclone3_handbook.pdf>, mod�le **EP3C25** package **F324** vitesse **A7**. Ce FPGA sert tr�s probablement de carte graphique pour piloter l'�cran. La technologie utilis�e semble �tre <http://www.altera.com/support/examples/nios2/exm-tes-demo.html>.
Un chip **flash 16Mo** Spansion <http://www.spansion.com/Support/Related%20Product%20Info/S29GL128N_overview.pdf> **GL128N90FFAR2** voila peut-�tre notre **/F**?
3 chips **Micron** marqu�s ``2DF42 D9GPD``, probablement de la **RAM** (en haut pour le FPGA, en bas pour le CPU ?), � v�rifier
Un **CPU Freescale MPC5200B** <http://cache.freescale.com/files/32bit/doc/data_sheet/MPC5200.pdf>, mod�le exact difficile � conna�tre, probablement **SPC5200CVR400** - PowerPC 32 bits 400MHz avec FPU, 16k cache, CAN, USB, Ethernet (??), ... 


# 5. Upgrade POI

D�tails du fonctionnement. Questions : pourquoi est-ce que �a boucle � l'infini chez certaines personnes sans plus d'infos ? Est-ce qu'on peut y faire quelque chose ? RE du format peut-�tre d�j� fait car il existe des POI non-officiels issues de SCDB pour le RT6 (vu sur gpsunderground). 
Le syst�me reconna�t que le m�dia est une mise � jour des points d'int�r�t � travers la pr�sence d'un fichier **POI_VER.POI**, et il ex�cute le script `upg/poi_upgrade.cmd`.

# 6. Upgrade Carto

D�tails du fonctionnement. Questions : Est-ce que le format est compr�hensible ? Est-ce qu'on peut envisager de mettre � jour les cartes � partir d'Openstreetmap ? Que signifie exactement "mise � jour pas compatible avec les v�hicules apr�s ao�t 2011" ?
Le syst�me reconna�t que le m�dia est une mise � jour des points d'int�r�t � travers la pr�sence d'un fichier **CD_VER.NAV**, et il ex�cute le script NAV_UPGRADE.CMD.

# 7. Modifier le code firmware

Il faut bien garder � l'esprit qu'il est assez facile de **comprendre comment les choses fonctionnent** (en tout cas pour quelqu'un du m�tier), car le firmware est livr� avec ses symboles de debug. Il suffit d'un d�sassembleur et de temps pour lire ce que le programme fait. Faire des modifications est un autre d�bat, malheureusement. � moins de se procurer un kit de d�veloppement VxWorks (payant), il faudra faire les modifications directement en assembleur PowerPC, et les injecter dans le firmware existant. J'avais commenc� � travailler avec **objdump** pour supprimer une fonction quelconque du firmware, et la remplacer par une �crite � la main, ce qui est un montage plus "propre" que le bidouillage de Mira pour les *generic function calls*. Toutefois la manipulation n'avait pas abouti.

# 9. Changer l'image d'accueil

Le BMP d'accueil est stock� sur le RT6 dans le chemin suivant : **/F/Application/Boot/BootScreen.bmp**. Il est, comme tous les fichiers du RT6, soumis � une v�rification du CRC selon la proc�dure d�crite plus haut. Ce fichier ne semble pas pr�sent sur le m�dia d'install du firmware 2.20. 
C'est la fonction **BootRomSplash** qui le charge (je crois). Elle est appel�e avec une valeur entre 0 et 5 qui d�crit le type de splash - "please insert upgrade CD", "error detected", etc.

Je n'ai jamais proc�d� � un changement d'image d'accueil, mais c'est une op�ration que Mira sait r�aliser.

# 10. Pour rigoler/avis sur l'appareil

Le code du firmware fait appara�tre que **Maserati** est �galement utilisateur du RT6. Moi, si j'ach�te une Maserati, j'attends nettement mieux que le RT6... :)
Je suis plut�t d��u de cet appareil qui est lent dans la plupart de ses op�rations (saisie d'une adresse GPS, d�marrage, lecture d'une cl� USB), qui contient certains bugs particuli�rement g�nants (d�connexions intempestives Bluetooth AD2P, refus de lire certaines cl�s USB), et dont le syst�me de navigation est assez mauvais tant dans ses algorithmes (il ne d�mord pas du chemin qu'il a choisi, par une esp�ce d'hyst�r�se, si vous vous en �cartez) que dans sa cartographie (qui n'est pas vraiment � jour m�me dans ses �ditions r�centes, le tout pour un prix prohibitif).
Il est vrai que travaillant dans un secteur plus dynamique et plus rapide � innover que l'automobile, j'ai des exigences tr�s importantes de la part de l'informatique embarqu�e que j'utilise au quotidien. Le RT6 fait ce pour quoi il a �t� con�u, mais le prix de l'option ne me semble pas �tre r�aliste en regard des d�fauts qu'il pr�sente.

# 11. <s>Besoin d'aide</s>

N'�tant plus actif sur ce projet je n'ai pas besoin d'aide - mais si vous avez des informations � me transmettre je les mettrai en ligne (contact en bas de la page).


<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.7/jquery.min.js"></script>
<link rel="stylesheet" href="fancybox/jquery.fancybox.css" type="text/css" media="screen" />
<script type="text/javascript" src="fancybox/jquery.fancybox.pack.js"></script>

<script>
    $(document).ready(function() {
		$("a[href$='.jpg'],a[href$='.jpeg'],a[href$='.png'],a[href$='.gif']").attr('rel', 'gallery').fancybox();
    });
</script>
