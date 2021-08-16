include nemu/scripts/git.mk

default:
	@echo "Please run 'make' under any subprojects to compile."

submit:
	git gc
	STUID=$(STUID) STUNAME=$(STUNAME) bash -c "$$(curl -s http://jyywiki.cn/static/submit.sh)"

.PHONY: default submit
