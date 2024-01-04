ALL	= mysync

C11	= cc -std=c11 -Wall -Werror
header=$(wildcard ./*.h)
src=$(wildcard ./*.c)

all:	$(ALL)

mysync: $(header) $(src)
	$(C11) -o mysync $(src)

clean:
	rm -f $(ALL)


