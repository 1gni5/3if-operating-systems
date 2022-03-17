

default: all

all: crible cat-stdio cat-readwrite cat-mmap prenoms

##########################################
# compilation des programmes

crible: crible.c
	gcc -std=gnu11 -g -Wall -Werror -o crible crible.c

cat-stdio: cat-stdio.c
	gcc -std=gnu11 -g -Wall -Werror -o cat-stdio cat-stdio.c

cat-readwrite: cat-readwrite.c
	gcc -std=gnu11 -g -Wall -Werror -o cat-readwrite cat-readwrite.c

cat-mmap: cat-mmap.c
	gcc -std=gnu11 -g -Wall -Werror -o cat-mmap cat-mmap.c

prenoms: prenoms.c
	gcc -std=gnu11 -g -Wall -Werror -o prenoms prenoms.c

##########################################
# nettoyage des fichiers générés

clean:
	rm -f crible cat-stdio cat-readwrite cat-mmap prenoms
