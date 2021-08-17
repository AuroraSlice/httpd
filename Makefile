#
# makefile create 2021/8/14
#

CC := gcc -std=c99

PROG := minihttp

CFLAGS := -I./include/

OUTPUT := output

vpath %.c src


obj = http.o http_init.o extend.o

$(PROG): clean $(obj)
	mkdir -p $(OUTPUT)
	$(CC) -o $(OUTPUT)/$(PROG) $(obj) $(CFLAGS)

.PHONY : clean
clean:
	rm -f *.o
	rm -rf $(OUTPUT)