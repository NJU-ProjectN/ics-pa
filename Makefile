include nemu/Makefile.git

default:
	@echo "Please run 'make' under any subprojects to compile."

clean:
	-$(MAKE) -s -C nemu clean
	-$(MAKE) -s -C abstract-machine clean-all
	-$(MAKE) -s -C nanos-lite clean
	-$(MAKE) -s -C navy-apps clean

submit: clean
	git gc
	STUID=$(STUID) STUNAME=$(STUNAME) bash -c "$$(curl -s http://jyywiki.cn/static/submit.sh)"

.PHONY: default clean submit
