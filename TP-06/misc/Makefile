all: max-threads demo-sem-open

%: %.c
	gcc -pthread -o $@ $^

clean:
	rm -f max-threads demo-sem-open
