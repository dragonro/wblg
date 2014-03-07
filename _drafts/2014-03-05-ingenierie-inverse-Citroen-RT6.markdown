---
layout: post
title: "Ing�nierie inverse de l'autoradio eMyWay (RT6) Citro�n"
date: 2014-03-05 23:00:00
#encoding: iso-8859-15
categories: francais electronics auto
---

Je reprends ici un article que j'ai r�dig� il y a plus d'un an concernant
l'autoradio eMyWay disponible sur les Citro�n C4.

M�thode: **�tude du CD d'upgrade firmware 2.20**

# 1. Travaux pr�c�dents

Le RT6, d�velopp� par Magneti Marelli, semble � premi�re vue �tre assez
similaire au RT3, pour lequel un effort de documentation a d�j� �t� fait. On
est en pr�sence d'une plateforme **VxWorks 5.5.1** avec Tornado 2.2.1. La
carte m�re est probablement similaire � une WindRiver d'architecture PowerPC
(exemples de produits : [http://www.windriver.com/products/bsp_web/bsp_architecture.html?architecture=PowerPC](http://www.windriver.com/products/bsp_web/bsp_architecture.html?architecture=PowerPC)) La carte est-elle la m�me que le RT3
? Je pensais que le RT3 �tait en MIPS mais [http://fr.viadeo.com/fr/profile/cyrille.lohier](http://fr.viadeo.com/fr/profile/cyrille.lohier) laisse penser le
contraire. Les binaires semblent �tre produits avec GCC: `(GNU) gcc-2.96
(2.96+ MW/LM) AltiVec VxWorks 5.5`.

Le RT4 a une architecture tr�s similaire et a �t� l'objet d'efforts
d'ing�nierie inverse men�s par diff�rentes personnes dont plusieurs ont post�
sur ce forum. On notera qu'il existe une version RT6 des "mira scripts", en
cours de d�veloppement ([http://mira308sw.altervista.org/en/index.htm](http://
mira308sw.altervista.org/en/index.htm)). Mira conna�t tr�s bien l'appareil.

# 2. Arborescence

On trouve sur le CD:

  * des fichiers ".inf" 
  * des fichiers ".CMD" et ".ini" 
  * des fichiers ".out" 
  * des fichiers ".gz" 
  * ... 

## 2.1. Fichiers .inf

Ce sont des fichiers d�crivant un binaire, le binaire semble porter le m�me
nom que le fichier mais sans le ".inf" (a.bin.inf d�crit le fichier a.bin).
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

    
```bash  
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

## 2.2. Fichiers .CMD et .ini

On dirait que c'est une habitude chez MM d'utiliser des extensions pour des
types de fichiers qui ne correspondent pas du tout. Les .CMD et .ini sont...
du **code C** ! En r�alit� ce code est interpr�t� par une biblioth�que
d�nomm�e [EiC](http://www.linuxbox.com/tiki/node/149), qui permet d'�crire des
scripts avec une syntaxe tr�s similaire au C.

## 2.3. Fichiers .out

Il s'agit d'ex�cutables.

```./Application/Boot/ssm_boot.out: ELF 32-bit MSB relocatable, PowerPC or
cisco 4500, version 1 (SYSV), not stripped

Architecture **PowerPC**, BigEndian, 32 bit. D�sassemblable avec les binutils
GNU compil�s pour PowerPC... ils ont m�me laiss� les symboles de debug (= les
noms des fonctions) ! Merci MM !

## 2.4. Fichiers .gz

Ce sont des ex�cutables comme les .out, mais compress�s. Voir plus bas.

## 2.5. Bootrom

Il semble y avoir un fichier sp�cial, qui est un ex�cutable non compress� qui
n'est pas ELF. C'est probablement le _BIOS_ de la carte. C'est le fichier
`Application/Boot/RNEG2010_EUR_2_20/DG4/BOOTROM.DAT`. Ce fichier contient un
certain nombre de cha�nes de caract�res int�ressantes. Sa version est
`BSP_PPC-SECT83+BSP_PPC_6.86k1:project:SECT39+1`. C'est probablement du code
binaire sans structure particuli�re, il faudra essayer de le d�sassembler
comme si c'�tait du code machine brut.

## 2.6. BootRom.sym

Attention ce fichier n'est pas le m�me que dans la section pr�c�dente. C'est
un ex�cutable PowerPC statique dont les symboles n'ont pas �t� supprim�s : on
peut le d�sassembler pour l'�tudier. C'est lui qui semble fournir la majorit�
des fonctions utilis�es dans le firmware, par exemple **CheckCRCFile**.

## 2.7. Hash des fichiers

Les fichiers .inf ont au d�but une valeur qui correspond � un hash **CRC**.
J'ai essay� plusieurs possibilit�s - CRC32 avec divers polyn�mes connus,
Adler32, sans succ�s. La formule de calcul du hash n'est donc pas �vidente -
mais en lisant le code on se rend compte que c'est un hash sur 16 bits et non
32 bits. Si on regarde dans le code de BootRom.sym on se rend compte que
*CheckCRCFile* appelle *CheckCRCInf* qui lui m�me fait deux appels :

  * l'un � `ReadINFCRC__FPCcR9Crc16Type` (Le nom bizarre correspond au "mangling" des fonctions C++ fait par GCC. Cela correspond � la signature : `ReadINFCRC(char const *, Crc16Type &)`) 
  * l'autre � `ComputeCRCFile`

J'ai manuellement recr�� le code de calcul du hash � partir du code machine.
Il op�re par tranche de 8 bits, avec deux tables de lookup, et des �changes...
ce n'est pas un Adler16. Il peut s'agir d'un algo "maison", dans ce cas le
code ci-dessous associ� au contenu des tables permettra de recr�er les hash si
on veut faire des modifs. J'ai �crit un programme en C pour [calculer les
hashs](crc_rt6.c).

Exemple sur `/Application/BTL/File_Search.gz.inf` - le hash en d�but de
fichier est **0c622f64**. **0c62** est le hash du **.inf**, **2f64** est le
hash du **binaire**, comme on peut le voir :

    
    
    $ ./crc_rt6 /mnt/hd/Application/BTL/File_Search.gz.inf 4
    Computed CRC16 0x0c 0x62
    $ ./crc_rt6 /mnt/hd/Application/BTL/File_Search.gz 0
    Computed CRC16 0x2f 0x64
    

## 2.8. Compression des binaires

La plupart des ex�cutables sont des fichiers .gz compress�s. Un peu d'�tude du
code nous am�ne � la fonction **inflate** qui est utilis�e pour d�compresser
ces binaires. Malheureusement cette fonction ne semble pas avoir le
comportement standard de la zlib. Un peu de Google nous m�ne � [http://read.pu
dn.com/downloads58/sourcecode/embed/205887/src/util/inflateLib.c__.htm](http:/
/read.pudn.com/downloads58/sourcecode/embed/205887/src/util/inflateLib.c__.htm
). Tout en bas du fichier, la fonction `inflate` semble correspondre au code
source de celle du RT6. Il reste � v�rifier en quoi cette fonction diff�re de
ce que fait la zlib nativement et nous pourrons d�compresser (et je l'esp�re
recompresser) les binaires du RT6. Avec d�compression + CRC, on pourra
commencer � s'amuser � tout casser.

VxWorks ajoute en fait un octet de _bourrage_ au d�but de chaque fichier
compress� (qui sert pour calculer un checksum, mais uniquement si la variable
**inflateCksum** est d�finie - adresse `0xff4cf04` dans **BootRom.sym** qui
est l'ex�cutable principal, et elle ne l'est pas pour l'instant). Cet octet
fait que le format n'est pas reconnu par l'outil zpipe inclus dans zlib. Il
faut donc faire une petite modification sur **zpipe** dans `main()`:

    
    
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
    

Cela permet de passer un _seek_ en argument. La valeur de _seek_ � utiliser
est 1. On peut ensuite d�compresser n'importe quel binaire du RT6 de la
mani�re suivante :

    
    
    zpipe -d 1 < file.gz > file.bin
    

J'ai fait cela sur tous les binaires compress�s du firmware. Pas de surprise,
ce sont bien des binaires PowerPC, avec les symboles de debug.

# 3. Question Bluetooth

Bluetooth d�finit plusieurs **profils**, qui correspondent � un ensemble de
fonctionnalit�s rendues par un appareil � un autre. Pour la musique, il existe
**AD2P** qui est le profil permettant de transf�rer le son par radio, et
**AVRCP** qui permet le contr�le de l'appareil lecteur par un autre (play,
pause, next, previous, ...). Je souhaite savoir quelle version du profil
Bluetooth AVRCP mon v�hicule supporte. Cette information n'est disponible
nulle part mais l'�tude du module Bluetooth pourra r�pondre � la question. On
trouve dans `Application/BCM/t2bf/bcm_t2bf.bin` (note: je nomme **.bin** les
fichiers obtenus par d�compression du **.gz**) les cha�nes suivantes :

    
    
    AVRCP version 1.0 supported
    AVRCP version 1.3 supported
    AVRCP unknown version
    

Donc le RT6 supporterait la version 1.3 d'AVRCP. Il y a une version 1.4 qui
ajoute une fonction recherche et la possibilit� de g�rer plusieurs _players_
(par exemple deux t�l�phones simultan�ment pour streamer de la musique). [http
://en.wikipedia.org/wiki/Bluetooth_profile+Audio.2FVideo_Remote_Control_Profil
e_.28AVRCP.29](http://en.wikipedia.org/wiki/Bluetooth_profile+Audio.2FVideo_Re
mote_Control_Profile_.28AVRCP.29)

**sdptool** sous Linux liste les profils support�s par un appareil Bluetooth... mais **BT_CAR_SYSTEM** (le RT6) ne r�pond pas aux requ�tes. En tout cas cela m'a permis de d�tecter la version d'AVRCP sur mon Blackberry et mon YP-P2 - ces deux appareils sont en 1.0, donc la voiture ne pourra pas afficher les m�ta donn�es ni m'indiquer la liste des pistes. Conclusion il vaut mieux brancher ces appareils en **USB** (sauf que le Blackberry n'est pas accept� par le RT6 en USB, "m�dia illisible", probablement � cause de la table des partitions - en effet le BB expose une table des partitions avec une seule partition, alors que la plupart des cl�s n'ont pas de table des partitions, � v�rifier si c'est effectivement le souci, dans ce cas on serait en pr�sence d'une erreur/oubli de la part de MM). 

# 4. Upgrade firmware

� faire : d�tails du fonctionnement de l'upgrade firmware. Questions: est-ce
qu'on peut ne mettre � jour qu'un seul fichier ? Est-ce qu'on peut faire des
changements et revenir en arri�re sur une version officielle du firmware ?

Le syst�me reconna�t que le m�dia est une mise � jour du firmware � travers la
pr�sence d'un fichier CD.inf, et il ex�cute le binaire
`/upg/flasher/flasher.rom`.

# 5. Upgrade POI

D�tails du fonctionnement. Questions : pourquoi est-ce que �a boucle �
l'infini chez certaines personnes sans plus d'infos ? Est-ce qu'on peut y
faire quelque chose ? RE du format peut-�tre d�j� fait car il existe des POI
non-officiels issues de SCDB pour le RT6 (vu sur gpsunderground).

Le syst�me reconna�t que le m�dia est une mise � jour des points d'int�r�t �
travers la pr�sence d'un fichier **POI_VER.POI**, et il ex�cute le script
`upg/poi_upgrade.cmd`.

# 6. Upgrade Carto

D�tails du fonctionnement. Questions : Est-ce que le format est compr�hensible
? Est-ce qu'on peut envisager de mettre � jour les cartes � partir
d'Openstreetmap ? Que signifie exactement "mise � jour pas compatible avec les
v�hicules apr�s ao�t 2011" ?

Le syst�me reconna�t que le m�dia est une mise � jour des points d'int�r�t �
travers la pr�sence d'un fichier **CD_VER.NAV**, et il ex�cute le script
NAV_UPGRADE.CMD.

# 7. Besoin d'aide

Si vous voulez aider cet effort, et que vous n'�tes pas informaticien, voici
quelques id�es de choses � faire :

  * photographies d�taill�es de l'ext�rieur et int�rieur du RT6, avec rep�rage de tous les composants (afin de conna�tre les caract�ristiques du mat�riel) 
  * plus sp�cifiquement, d�terminer quel m�dium de stockage est dans le RT6 (disque dur, si oui marque et mod�le, flash, si oui amovible ou pas, si oui marque et mod�le, ...) 
  * pr�t d'un RT6 pour test (non, je veux pas la voiture avec) 
  * prise de contact avec MM pour solliciter le code source ou la documentation (tr�s peu de chances d'aboutir et c'est � double tranchant, r�serv� � des gens tr�s diplomates) 
  * r�cup�rer d'autres versions du firmware/POI/carto (attention aux aspects l�gaux) pour me les envoyer 
  * tester pour moi certaines fonctionnalit�s ("cheat codes" que j'indiquerai, etc.), voire plus selon votre courage 
