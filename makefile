__HEADER__		?=	Ground Floor Makefile
DIR_SOURCE		?=	src
DIR_DOCUMENT	?=	doc
DIR_EXAMPLE		?=	example
DIR_DOC_API		?=	$(DIR_DOCUMENT)/api
DIR_BUILD		?=	$(DIR_DOCUMENT)/_build
DIR_BUILD_XML	?=	$(DIR_BUILD)/doxyxml
RST_EXAMPLE		?=	readme.rst

DEVICE			?=	/dev/cu.SLAB_USBtoUART
BAUDRT			?=	115200


define _line
$$( \
	printf "%0.s$(1)" $$(seq 1 $$(printf "%s" "$(2)" | wc -c)) \
)
endef
define _heading
$$( \
	printf "%s\n%s\n%s\n" \
		"$(call _line,$(1),$(2))" \
		"$(2)" \
		"$(call _line,$(1),$(2))" \
)
endef

help:
	@echo "$(call _heading,-,$(__HEADER__))"
	@echo
	@echo "all"			"\t\t"	"shortcut for 'doc' 'docapi' 'buildsample'"
	@echo
	@echo "cleansample"	"\t"	"cleanup all example project files"
	@echo "cleandoc"	"\t"	"cleanup all documentation files"
	@echo "cleanxml"	"\t"	"cleanup all doxygen xml files"
	@echo "clean"		"\t\t"	"shortcut for 'cleansample'"
	@echo "cleanall"	"\t\t"	"shortcut 'cleandoc' 'cleanxml' 'cleansample'"
	@echo
	@echo "doc"			"\t\t"	"build all documentation with sphinx"
	@echo "docwait"		"\t"	"watch for changes and build docs then"
	@echo "docxml"		"\t\t"	"create xml with doxygen"
	@echo "docapi"		"\t\t"	"generate apidoc from xml with breathe-apidoc"
	@echo "docsample"	"\t"	"create documentation for example projects"
	@echo
	@echo "rmdocapi"	"\t"	"remove api documentation files"
	@echo "rmdocsample"	"\t"	"remove all example projects documentation"
	@echo
	@echo "buildsample"	"\t"	"build all example projects"
	@echo
	@echo "browse"		"\t\t"	"open webbroser"
	@echo
	@echo "serial"		"\t\t"	"open platformio device monitor on $(DEVICE)"
	@echo "serialcom"	"\t"	"same but with minicom"
	@echo "\t->"		"\t"	"switch: 'make serial DEVICE=/dev/cu.blabla'"
	@echo "\t->"		"\t"	"and/or: 'make serial BAUDRT=115200'"
	@echo

CMD_APIDOC		:=	breathe-apidoc
CMD_DELTREE		:=	rm -rfv
CMD_DOXYGEN		:=	doxygen
CMD_MINICOM		:=	minicom
CMD_MKDIR		:=	mkdir -pv
CMD_PLATFORMIO	:=	platformio
CMD_PYTHON3		:=	python3
CMD_SPHINX		:=	sphinx-build
CMD_WATCHDOG	:=	watchmedo


clean: cleansample
all: doc docapi buildsample
travis: buildsample

cleanxml:
	@$(CMD_DELTREE) $(DIR_BUILD_XML)/*
cleandoc:
	@$(CMD_DELTREE) $(DIR_BUILD)/*
cleanall: cleanxml cleandoc cleansample


rmdocapi: _are_you_sure
	@$(CMD_DELTREE) $(DIR_DOC_API)/*
rmdocsample: _are_you_sure
	@$(CMD_DELTREE) $(DIR_DOCUMENT)/$(DIR_EXAMPLE)/*

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
	@$(CMD_PLATFORMIO) device monitor --port "$(DEVICE)" --baud $(BAUDRT)
serialcom:
	@$(CMD_MINICOM) -D "$(DEVICE)" -b $(BAUDRT)


PROS_EXAMPLE	:=	$(wildcard $(DIR_EXAMPLE)/*)
PROS_EXAMPLE_B	:=	$(addprefix build_,$(PROS_EXAMPLE))
PROS_EXAMPLE_C	:=	$(addprefix clean_,$(PROS_EXAMPLE))
PROS_EXAMPLE_D	:=	$(addprefix docex_,$(PROS_EXAMPLE))
.PHONY: _sample


buildsample: docsample $(PROS_EXAMPLE_B)
$(PROS_EXAMPLE_B): _sample
	@(cd "$(patsubst build_%,%,$@)" && $(CMD_PLATFORMIO) run;)

cleansample: $(PROS_EXAMPLE_C)
$(PROS_EXAMPLE_C): _sample
	@(cd "$(patsubst clean_%,%,$@)" && $(CMD_PLATFORMIO) run -t clean;)


define _rst_document
$$( \
	srpath="$(DIR_EXAMPLE)/$(1)/src/"; \
	echo "$(call _heading,=,$(1))"; \
	echo; \
	if [ -f "$$srpath/$(RST_EXAMPLE)" ]; then \
		cat "$$srpath/$(RST_EXAMPLE)"; \
	fi; \
	for exfile in "$$srpath/"*; do \
		exname="$$(basename "$$exfile")"; \
		echo; \
		if [ "$$exname" != "$(RST_EXAMPLE)" ]; then \
			echo "$(call _heading,-,$$exname)"; \
			echo; \
			echo ".. literalinclude:: ../../$$exfile"; \
			echo "    :language: cpp"; \
			echo "    :linenos:"; \
		fi; \
		echo; \
	done; \
	echo; \
)
endef

docsample: $(PROS_EXAMPLE_D)
$(PROS_EXAMPLE_D): _sample
	@(\
		target="$(patsubst docex_%,$(DIR_DOCUMENT)/%.rst,$@)"; \
		tgtdir="$$(dirname "$$target")"; \
		[ ! -d "$$tgtdir" ] && $(CMD_MKDIR) "$$tgtdir"; \
		source="$(patsubst docex_$(DIR_EXAMPLE)/%,%,$@)"; \
		result="$(call _rst_document,$$source)"; \
		echo "$$result" > "$$target"; \
	)

# convenience - asking for confirmation
_are_you_sure:
	@( \
		read -p "Are you sure?!? [y/N] > " sure && \
		case "$$sure" in [yY]) true;; *) false;; esac \
	)
