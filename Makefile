#
# makefile create 2021/8/14
#

CC := gcc

PROG := minihttp

CFLAGS := -I./include/

OUTPUT := output

vpath %.c src


obj = http.o http_init.o extend.o common.o pthread_pool.o

$(PROG): clean $(obj)
	mkdir -p $(OUTPUT)
	$(CC) -pthread -o $(OUTPUT)/$(PROG) $(obj) $(CFLAGS)

.PHONY : clean
clean:
	rm -f *.o
	rm -rf $(OUTPUT)