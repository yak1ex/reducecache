WINDRES=x86_64-w64-mingw32-windres
CC=x86_64-w64-mingw32-gcc
STRIP=x86_64-w64-mingw32-strip *.exe

VER=0_02
DIR=reducecache-$(VER)

all: reducecache.exe

reducecache.ro: reducecache.rc
	$(WINDRES) $< $@

reducecache.exe: reducecache.c reducecache.ro
	$(CC) -static -o $@ $^

strip:
	$(STRIP) reducecache.exe

clean:
	rm -rf *.ro *.o *.exe

dist: strip
	rm -rf $(DIR)
	mkdir -p $(DIR)
	cp -p reducecache.exe reducecache.txt README.md $(DIR)
	(cd $(DIR); zip ../$(DIR).zip *)
	rm -rf $(DIR)
