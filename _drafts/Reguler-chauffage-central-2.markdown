---
layout: post
title: R�gulation de chauffage central - thermostat d'ambiance
date: 2015-11-06 22:59:05
tags: electronics
category: francais
comments: true
img_rel: "/~sven337/data/acleis"
---

Dans le pr�c�dent article nous avons vu un prototype permettant de contr�ler la chaudi�re. Dans cet article nous verrons le dispositif qui envoie les ordres � la chaudi�re en fonction de diff�rents param�tres.

Il s'agit ici uniquement de logiciel, qui fait intervenir :

- un thermom�tre int�rieur (DS18B20, nRF24L01+) pr�sent dans ma [lampe de bureau](/~sven337/english/2014/05/08/Transforming-halogen-lamp-into-LED-lamp.html)
- un deuxi�me thermom�tre int�rieur situ� dans une autre pi�ce
- un thermom�tre ext�rieur (DS18B20, nRF24L01+, NiMH 1.2V, module boost, panneau solaire, projet que je n'ai pas encore d�crit sur ce weblog car il a �t� victime du **syndrome du prototype** : on fait quelque chose, �a marche du premier coup, on le met en place sans jamais le mettre au propre parce qu'il marche parfaitement, et on l'oublie)
- une interface permettant de choisir le mode COLD, HOT et AWAY, le changement �tant r�alis� � travers une crontab
- un programme serveur qui, en fonction du mode en cours et des temp�ratures, calcule s'il faut activer le chauffage ou pas

# Donn�es d'entr�e

Le principe du contr�le climatique pour le chauffage est de se baser sur la temp�rature ext�rieure, afin d'anticiper les variations �ventuelles de temp�rature int�rieure. Je crois que c'est souvent mis en oeuvre dans les grands b�timents tertiaires, en revanche cela me semble moins justifi� pour une habitation. Dans le cas pr�sent je souhaite me baser sur la temp�rature int�rieure afin que les apports de chaleur, par exemple ceux de l'�lectrom�nager ou de la cuisine, soient pris en compte.
Voici l'approche que j'ai retenue :

- une "temp�rature de non chauffage", d'environ 15�C d'apr�s les diff�rentes pages web que j'ai lues (� ajuster � l'usage). Cette temp�rature est un seuil de temp�rature ext�rieure � partir duquel on arr�te de chauffer.
- un calcul de temp�rature int�rieure, sous forme de moyenne pond�r�e de diff�rentes pi�ces (sachant que le poids change selon nuit/jour)
- une d�cision binaire chauffer/pas chauffer selon la comparaison de la temp�rature de l'�tape pr�c�dente avec une valeur de consigne (diff�rente selon le mode "chaud" et d'un mode "froid")

# Interface

# Prise de d�cision

Je vais d�crire ici le fonctionnement 
temp�rature ext de non chauffage environ 15�C

dans un premier temps, fonctionnement en tout-ou-rien (avec temp�rature raisonnable configur�e sur la chaudi�re) bas� sur l'heure de la journ�e, temp�rature int�rieure (corrig�e), et temp�rature ext�rieure.

int compute_burner_power()
{
	// Set target interior temp based on date&time
	datetime_set_target_interior_temp();

	// Compute power based on exterior temperature
	int power = process_ext_temperature();

	// Alter it based on interior temperature
	power = process_int_temperature(power);
	return power;
}

int process_ext_temperature() 
{
	// Is exterior temperature reliable?
	if (!ext_temperature_is_old()) {
		if (ext_temperature() >= 16) {
			// Stop heating at 16� outside
			return 0;
		} else {
			return 100;
		}
	} else {
		warn("Exterior thermometer is out of order.");
		return 100;
	}
}

int datetime_set_target_interior_temp()
{
	int hot_target = 210;
	int cold_target = 180;
	int target = cold_target;
	if (hot_period(datetime)) {
		target = hot_target;
	}
}

int process_int_temperature(int power)
{
	interior_temperature = (2*office_temp() + rdc_temp()) / 3;

	if (interior_temperature < int_temperature_target - 5) {
		if (!power) {
			warn("Exterior is at no-heat temperature, but internal temp is below target!");
		}
		power = 100;
	} else if (interior_temperature > int_temperature_target + 5) {
		if (power == 100) {
			warn("Exterior requests heating, but internal temp is above target!");
		}
		power = 0;
	} else {
		// Temperature is within internal constraints - heat or not based on exterior decision
		;
	}

	return power;
}


