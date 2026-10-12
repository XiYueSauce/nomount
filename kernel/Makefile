obj-$(CONFIG_NOMOUNT) += nomount.o

nomount-y := src/nomount.o \
	KallRecon/core.o \
	KallRecon/slide.o \
	KallRecon/anchor.o

# KallRecon vendored flat from rev 8392512
# anchor.S carries the .quad sprint_symbol relocation KallRecon needs
ccflags-y += -std=gnu11 -O3
ccflags-y += -Wno-declaration-after-statement
ccflags-y += -I$(src)/KallRecon
