WINDRES=x86_64-w64-mingw32-windres
CC=x86_64-w64-mingw32-gcc
STRIP=x86_64-w64-mingw32-strip *.exe

all: reducecache.exe

reducecache.ro: reducecache.rc
	$(WINDRES) $< $@

reducecache.exe: reducecache.c reducecache.ro
	$(CC) -static -o $@ $^

strip:
	$(STRIP) reducecache.exe

clean:
	rm -rf *.ro *.o *.exe
