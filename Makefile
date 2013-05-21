SRC_DIR=src
CFLAGS+=-Wall -g -DDEBUG -DNSS_IPV4_ONLY -DENABLE_LEGACY
CFLAGS+=-Isrc
CFILES=$(shell find $(SRC_DIR)  -maxdepth 1 -name "*.c")
COBJS=$(CFILES:%.c=%.o)
LIBS+=
TARGET=mdns

all:$(TARGET)

libmdns_client.so:$(COBJS)
	$(HOST)gcc -o $@ -fPIC -shared $^
	$(HOST)strip $@

-include $(addsuffix /*.d, $(SRC_DIR))

$(TARGET):test/mdns-test.o libmdns_client.so
	$(HOST)gcc -o $@ $^ $(LIBS)

$(COBJS) test/mdns-test.o:%.o:%.c
	$(HOST)gcc -c $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

clean:
	-rm -f $(addsuffix /*.d, $(SRC_DIR) test) $(addsuffix /*.o, $(SRC_DIR) test) $(TARGET) ./libmdns_client

.PHONY:all clean
