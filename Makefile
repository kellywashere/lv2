DESTDIR=~/.lv2

CC=gcc
CC_FLAGS=-fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -fPIC -shared -pthread -O2 `pkg-config --cflags lv2` -lm `pkg-config --libs lv2`
CP=cp
RM=rm
MD=mkdir -p
CD=cd
GREP=grep
XARGS=xargs
SED=sed
MV=mv

all: rvdbTremolo rvdbDelay

.PHONY: rvdbTremolo
rvdbTremolo:
	$(MD) $(DESTDIR)/$@.lv2
	$(CC) $(CC_FLAGS) $@/$@.c -o $@/$@.so
	$(CP) $@/$@.so $(DESTDIR)/$@.lv2/
	$(CP) $@/$@.ttl $(DESTDIR)/$@.lv2/
	$(CP) $@/manifest.ttl $(DESTDIR)/$@.lv2/

.PHONY: rvdbDelay
rvdbDelay:
	$(MD) $(DESTDIR)/$@.lv2
	$(CC) $(CC_FLAGS) $@/$@.c -o $@/$@.so
	$(CP) $@/$@.so $(DESTDIR)/$@.lv2/
	$(CP) $@/$@.ttl $(DESTDIR)/$@.lv2/
	$(CP) $@/manifest.ttl $(DESTDIR)/$@.lv2/

.PHONY: clean
clean:
	$(RM) rvdbTremolo/rvdbTremolo.so
	$(RM) rvdbDelay/rvdbDelay.so


