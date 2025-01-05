DESTDIR=~/.lv2

CC            := gcc
LD            := ld
CC_FLAGS_UTIL := -fvisibility=hidden -fPIC -pthread -O2
CC_FLAGS_LV2  := -fvisibility=hidden -fPIC -pthread -O2 `pkg-config --cflags lv2`
# LD_FLAGS_LV2  := -Bstatic -Bdynamic --as-needed -lm `pkg-config --libs lv2`
LD_FLAGS_LV2  := --as-needed -lm `pkg-config --libs lv2`
CP            := cp
RM            := rm -f
MD            := mkdir -p
CD            := cd
GREP          := grep
XARGS         := xargs
SED           := sed
MV            := mv

all: rvdbTremolo rvdbDelay rvdbReverseDelay

.PHONY: rvdbTremolo
rvdbTremolo: rvdbTremolo/rvdbTremolo.so

.PHONY: rvdbDelay
rvdbDelay: rvdbDelay/rvdbDelay.so

.PHONY: rvdbReverseDelay
rvdbDelay: rvdbReverseDelay/rvdbReverseDelay.so

%.so: %.c util/ringbuffer.o
	$(eval PLUGNAME=$(subst /,,$(dir $@)))
	@echo "--- Compiling $(PLUGNAME) ---"
	$(MD) $(DESTDIR)/$(PLUGNAME).lv2
	$(CC) -c $(CC_FLAGS_LV2) $(PLUGNAME)/$(PLUGNAME).c -o $(PLUGNAME)/$(PLUGNAME).o
	$(LD) -shared -o $(PLUGNAME)/$(PLUGNAME).so $(LD_FLAGS_LV2) $(PLUGNAME)/$(PLUGNAME).o util/ringbuffer.o
	@echo "--- Copying to $(DESTDIR)/$(PLUGNAME)/ ---"
	$(CP) $(PLUGNAME)/$(PLUGNAME).so $(DESTDIR)/$(PLUGNAME).lv2/
	$(CP) $(PLUGNAME)/$(PLUGNAME).ttl $(DESTDIR)/$(PLUGNAME).lv2/
	$(CP) $(PLUGNAME)/manifest.ttl $(DESTDIR)/$(PLUGNAME).lv2/

util/%.o: util/%.c
	@echo "--- Compiling $< ---"
	$(CC) -c $(CC_FLAGS_UTIL) $< -o $@

.PHONY: clean
clean:
	$(RM) util/ringbuffer.o
	$(RM) rvdbTremolo/rvdbTremolo.so
	$(RM) rvdbDelay/rvdbDelay.so
	$(RM) rvdbReverseDelay/rvdbReverseDelay.so

# Convenience functions
.PHONY: listlv2
listlv2:
	@lv2ls | $(GREP) kellywashere

.PHONY: runlv2-%
runlv2-%:
	$(eval PLUGNAME=$(subst runlv2-,,$@))
	$(eval PLUGFOUND=$(shell lv2ls | grep $(PLUGNAME)))
	@echo "Starting plugin $(PLUGFOUND)"
	jalv.gtk3 $(PLUGFOUND)

