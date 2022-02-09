

# si on tape "make" sans préciser de cible, make va chercher à
# construire la *première* cible du Makefile.
default: all

all: rebours parexec

##########################################
# compilation des programmes

rebours: rebours.c
	gcc -g -Wall -Werror -o rebours   rebours.c 

fourche: fourche.c
	gcc -g -Wall -Werror -o fourche fourche.c

doublerebours: doublerebours.c
	gcc -g -Wall -Werror -o doublerebours   doublerebours.c 

parexec: parexec.c
	gcc -g -Wall -Werror -o parexec   parexec.c

##########################################
# nettoyage des fichiers générés

clean:
	rm -f *.o parexec rebours
