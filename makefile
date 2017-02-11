__HEADER__		?=	Ground Floor Makefile
DIR_SOURCE		?=	src
DIR_DOCUMENT	?=	doc
DIR_DOC_API		?=	$(DIR_DOCUMENT)/api
DIR_BUILD		?=	$(DIR_DOCUMENT)/_build
DIR_BUILD_XML	?=	$(DIR_BUILD)/doxyxml

DEVICE			?=	/dev/cu.SLAB_USBtoUART
BAUDRT			?=	115200


___LINE___		:=	$$( \
	printf "%0.s-" $$(seq 1 $$(printf "%s" "$(__HEADER__)" | wc -c) \
))

help:
	@echo $(__HEADER__)
	@echo $(___LINE___)
	@echo
	@echo "clean"		"\t\t"	"cleanup all files"
	@echo "cleanapi"	"\t"	"cleanup all documentation files"
	@echo "cleandoc"	"\t"	"cleanup all documentation files"
	@echo
	@echo "doc"			"\t\t"	"build all documentation with sphinx"
	@echo "docapi"		"\t\t"	"generate apidoc from xml with breathe-apidoc"
	@echo "docwait"		"\t"	"watch for changes and build docs then"
	@echo "docxml"		"\t\t"	"create xml with doxygen"
	@echo
	@echo "browse"		"\t\t"	"open webbroser"
	@echo
	@echo "serial"		"\t\t"	"open platformio device monitor on $(DEVICE)"
	@echo "serialcom"	"\t"	"same but with minicom"
	@echo "\t->"		"\t"	"switch: 'make serial DEVICE=/dev/cu.blabla'"
	@echo

CMD_APIDOC		:=	breathe-apidoc
CMD_DELTREE		:=	rm -rfv
CMD_DEVMON		:=	platformio device monitor
CMD_DOXYGEN		:=	doxygen
CMD_MINICOM		:=	minicom
CMD_PYTHON3		:=	python3
CMD_SPHINX		:=	sphinx-build
CMD_WATCHDOG	:=	watchmedo


cleanxml:
	@$(CMD_DELTREE) $(DIR_BUILD_XML)/*
cleandoc:
	@$(CMD_DELTREE) $(DIR_BUILD)/*
clean: cleanxml cleandoc

cleanapi:
	@(	read -p "are you sure?!? [y/N] > " sure; case $$sure in \
		[Yy]) $(CMD_DELTREE) $(DIR_DOC_API)/* ;; \
		*) echo "obviously not.." ;; \
	esac)


docxml:
	@(cd $(DIR_DOCUMENT) && $(CMD_DOXYGEN))
docapi: docxml
	@$(CMD_APIDOC) -o $(DIR_DOC_API) $(DIR_BUILD_XML)
doc: docxml
	@$(CMD_SPHINX) -M html "$(DIR_DOCUMENT)" "$(DIR_BUILD)"

docwait:
	@$(CMD_WATCHDOG) shell-command \
		--ignore-directories --recursive \
		--patterns="*.hpp;*.cpp;*.rst" --drop \
		--command='echo "$(___LINE___)"; $(MAKE) doc' .


browse: doc
	@$(CMD_PYTHON3) -m "webbrowser" \
		-t "file://$$(cd "$(DIR_BUILD)" || exit 7 && pwd)/html/index.html"

serial:
	@$(CMD_DEVMON) --port "$(DEVICE)" --baud $(BAUDRT)
serialcom:
	@$(CMD_MINICOM) -D "$(DEVICE)" -b $(BAUDRT)
