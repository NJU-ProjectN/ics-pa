-include nemu/Makefile.git
 
clean:
	-$(MAKE) -C nemu clean
	-$(MAKE) -C nexus-am clean
	-$(MAKE) -C nanos-lite clean
	-$(MAKE) -C navy-apps clean

submit: clean
	git gc
	cd .. && tar cj $(basename $(shell pwd)) > $(STU_ID).tar.bz2

.PHONY: clean submit
