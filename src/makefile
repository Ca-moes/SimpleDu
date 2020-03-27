CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o regfile.o
XHDRS = simpledu.h regfile.h
EXEC = simpledu.exe

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
