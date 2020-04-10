CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o regfile.o flags.o scan.o
XHDRS = simpledu.h regfile.h flags.h scan.h
EXEC = simpledu

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -lm

%.o: %.c %.h $(XHDRS)
	@$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	@-rm $(OBJS) $(EXEC)
