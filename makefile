__HEADER__		?=	Ground Floor Makefile
DIR_SOURCE		?=	src
DIR_DOCUMENT	?=	doc
DIR_DOCBUILD	?=	$(DIR_DOCUMENT)/_build

help:
	@echo $(__HEADER__)
	@echo
	@echo "clean"		"\t\t"	"cleanup all files"
	@echo "-> doc"		"\t\t"	"cleanup all documentation files"
	@echo "doc"			"\t\t"	"build all documentation with sphinx"
	@echo "-> xml"		"\t\t"	"create xml with doxygen"
	@echo "-> loop"		"\t\t"	"watch for changes and build docs then"

CMD_DELTREE		=	rm -rfv
CMD_DOXYGEN		=	doxygen
CMD_SPHINX		=	sphinx-build
CMD_WATCHDOG	=	watchmedo

clean: cleandoc
cleandoc:
	@$(CMD_DELTREE) $(DIR_DOCBUILD)/*

doc: docxml
	@$(CMD_SPHINX) -M html "$(DIR_DOCUMENT)" "$(DIR_DOCBUILD)"
docxml:
	@(cd $(DIR_DOCUMENT) && $(CMD_DOXYGEN))
docloop:
	$(CMD_WATCHDOG) shell-command \
		--ignore-directories --recursive \
		--patterns="*.hpp;*.cpp;*.rst" --wait --drop \
		--command='$(MAKE) doc' \
		.
