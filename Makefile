DESTDIR=~/.lv2

CC=gcc
CC_FLAGS=-fvisibility=hidden -fPIC -pthread -O2
CC_FLAGS_LV2=-fvisibility=hidden -fPIC -pthread -O2 `pkg-config --cflags lv2`
LD_FLAGS_LV2=-Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -lm `pkg-config --libs lv2`
CP=cp
RM=rm -f
MD=mkdir -p
CD=cd
GREP=grep
XARGS=xargs
SED=sed
MV=mv

all: rvdbTremolo rvdbDelay rvdbReverseDelay

util/ringbuffer.o: util/ringbuffer.c
	$(CC) -c $(CC_FLAGS) util/ringbuffer.c -o $@

.PHONY: rvdbTremolo
rvdbTremolo:
	$(MD) $(DESTDIR)/$@.lv2
	$(CC) -c $(CC_FLAGS_LV2) $@/$@.c -o $@/$@.o
	$(CC) -shared -o $@/$@.so $(LD_FLAGS_LV2) $@/$@.o
	$(CP) $@/$@.so $(DESTDIR)/$@.lv2/
	$(CP) $@/$@.ttl $(DESTDIR)/$@.lv2/
	$(CP) $@/manifest.ttl $(DESTDIR)/$@.lv2/

.PHONY: rvdbDelay
rvdbDelay: util/ringbuffer.o
	$(MD) $(DESTDIR)/$@.lv2
	$(CC) -c $(CC_FLAGS_LV2) $@/$@.c -o $@/$@.o
	$(CC) -shared -o $@/$@.so $(LD_FLAGS_LV2) $@/$@.o util/ringbuffer.o
	$(CP) $@/$@.so $(DESTDIR)/$@.lv2/
	$(CP) $@/$@.ttl $(DESTDIR)/$@.lv2/
	$(CP) $@/manifest.ttl $(DESTDIR)/$@.lv2/

.PHONY: rvdbReverseDelay
rvdbReverseDelay: util/ringbuffer.o
	$(MD) $(DESTDIR)/$@.lv2
	$(CC) -c $(CC_FLAGS_LV2) $@/$@.c -o $@/$@.o
	$(CC) -shared -o $@/$@.so $(LD_FLAGS_LV2) $@/$@.o util/ringbuffer.o
	$(CP) $@/$@.so $(DESTDIR)/$@.lv2/
	$(CP) $@/$@.ttl $(DESTDIR)/$@.lv2/
	$(CP) $@/manifest.ttl $(DESTDIR)/$@.lv2/

.PHONY: clean
clean:
	$(RM) util/ringbuffer.o
	$(RM) rvdbTremolo/rvdbTremolo.so
	$(RM) rvdbDelay/rvdbDelay.so
	$(RM) rvdbReverseDelay/rvdbReverseDelay.so


