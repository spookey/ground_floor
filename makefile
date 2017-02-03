__HEADER__		?=	Ground Floor Makefile
DIR_SOURCE		?=	src
DIR_DOCUMENT	?=	doc
DIR_DOCBUILD	?=	$(DIR_DOCUMENT)/_build
DEVICE			?= /dev/cu.SLAB_USBtoUART
BAUDRT			?= 115200

help:
	@echo $(__HEADER__)
	@echo
	@echo "clean"	"\t"	"cleanup all files"
	@echo "-> doc"	"\t"	"cleanup all documentation files"
	@echo "doc"		"\t"	"build all documentation with sphinx"
	@echo "-> xml"	"\t"	"create xml with doxygen"
	@echo "mser"	"\t"	"same but with minicom"
	@echo "pser"	"\t"	"open serial connection to $(DEVICE) using platformio"
	@echo "wait"	"\t"	"watch for changes and build docs then"

CMD_DELTREE		=	rm -rfv
CMD_DOXYGEN		=	doxygen
CMD_MINICOM		=	minicom
CMD_PLATFORMIO	=	platformio
CMD_SPHINX		=	sphinx-build
CMD_WATCHDOG	=	watchmedo

clean: cleandoc
cleandoc:
	@$(CMD_DELTREE) $(DIR_DOCBUILD)/*

doc: docxml
	@$(CMD_SPHINX) -M html "$(DIR_DOCUMENT)" "$(DIR_DOCBUILD)"
docxml:
	@(cd $(DIR_DOCUMENT) && $(CMD_DOXYGEN))

wait:
	$(CMD_WATCHDOG) shell-command \
		--ignore-directories --recursive \
		--patterns="*.hpp;*.cpp;*.rst" --wait --drop \
		--command='$(MAKE) doc' \
		.

pser:
	@$(CMD_PLATFORMIO) device monitor --port "$(DEVICE)" --baud $(BAUDRT)
mser:
	$(CMD_MINICOM) -D "$(DEVICE)" -b $(BAUDRT)
