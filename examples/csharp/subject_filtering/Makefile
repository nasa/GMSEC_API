default all: make_pub make_sub

make_pub:
	$(MAKE) -f Makefile.publish

make_sub:
	$(MAKE) -f Makefile.subscribe

clean:
	$(MAKE) -f Makefile.publish $@
	$(MAKE) -f Makefile.subscribe $@
