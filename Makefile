all: build

build: editor.c
	gcc -Wall editor.c -o editor
clean:
	rm editor
# all: build
# 	./apel.out
# build: editor.c
# 	gcc -Wall editor.c -o apel.out