DESTDIR=~/.lv2

CC            := gcc
LD            := ld
AR            := ar rcs
CC_FLAGS_UTIL := -fvisibility=hidden -fPIC -pthread -O2
CC_FLAGS_LV2  := -fvisibility=hidden -fPIC -pthread -O2 `pkg-config --cflags lv2`
# LD_FLAGS_LV2  := -Bstatic -Bdynamic --as-needed -lm `pkg-config --libs lv2`
LD_FLAGS_LV2  := -fvisibility=hidden -L./util -lm `pkg-config --libs lv2` -lutil
CP            := cp
RM            := rm -f
MD            := mkdir -p
CD            := cd
GREP          := grep
XARGS         := xargs
SED           := sed
MV            := mv

ALL           := rvdbTremolo rvdbDelay rvdbReverseDelay rvdbLPF1

all: $(ALL)

.PHONY: rvdbTremolo
rvdbTremolo: rvdbTremolo/rvdbTremolo.so

.PHONY: rvdbDelay
rvdbDelay: rvdbDelay/rvdbDelay.so

.PHONY: rvdbReverseDelay
rvdbDelay: rvdbReverseDelay/rvdbReverseDelay.so

.PHONY: rvdbLPF1
rvdbLPF1: rvdbLPF1/rvdbLPF1.so

%.so: %.c util/libutil.a
	$(eval PLUGNAME=$(subst /,,$(dir $@)))
	@echo "--- Compiling $(PLUGNAME) ---"
	$(MD) $(DESTDIR)/$(PLUGNAME).lv2
	$(CC) -c $(CC_FLAGS_LV2) $(PLUGNAME)/$(PLUGNAME).c -o $(PLUGNAME)/$(PLUGNAME).o
	$(LD) -shared $(PLUGNAME)/$(PLUGNAME).o $(LD_FLAGS_LV2) -o $(PLUGNAME)/$(PLUGNAME).so 
	@echo "--- Copying to $(DESTDIR)/$(PLUGNAME)/ ---"
	$(CP) $(PLUGNAME)/$(PLUGNAME).so $(DESTDIR)/$(PLUGNAME).lv2/
	$(CP) $(PLUGNAME)/$(PLUGNAME).ttl $(DESTDIR)/$(PLUGNAME).lv2/
	$(CP) $(PLUGNAME)/manifest.ttl $(DESTDIR)/$(PLUGNAME).lv2/

util/libutil.a: util/ringbuffer.o util/lpf1.o
	@echo "--- creating libutil.a ---"
	$(AR) $@ $^

util/%.o: util/%.c
	@echo "--- Compiling $< ---"
	$(CC) -c $(CC_FLAGS_UTIL) $< -o $@

.PHONY: clean
clean:
	$(RM) util/*.o util/libutil.a
	echo $(ALL) | xargs -n 1 echo | xargs -I@ rm -f @/@.so @/@.o

# Convenience functions
.PHONY: listlv2
listlv2:
	@lv2ls | $(GREP) kellywashere

.PHONY: runlv2-%
runlv2-%:
	$(eval PLUGNAME=$(subst runlv2-,,$@))
	$(eval PLUGFOUND=$(shell lv2ls | grep $(PLUGNAME)))
	@echo "Starting plugin $(PLUGFOUND)"
	jalv.gtk3 $(PLUGFOUND) &

