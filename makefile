DIR_DOCUMENT	=	doc
DIR_EXAMPLE		=	example

DIR_DOC_API		=	$(DIR_DOCUMENT)/api
DIR_DOC_EX		=	$(DIR_DOCUMENT)/example
DIR_BUILD_DOC	=	$(DIR_DOCUMENT)/_build
DIR_BUILD_XML	=	$(DIR_BUILD_DOC)/doxyxml


###############################################################################

.PHONY: help
help:
	@echo "+--------------+"
	@echo "| ground_floor |"
	@echo "+--------------+"
	@echo
	@echo "build                build all example projects"
	@echo "doc                  build sphinx documentation"
	@echo "clean                cleanup all unneeded files"
	@echo


###############################################################################

.PHONY: cleandoc
cleandoc:
	sphinx-build -M clean "$(DIR_DOCUMENT)" "$(DIR_BUILD_DOC)"
	touch "$(DIR_BUILD_DOC)/.gitkeep"


.PHONY: docxml
docxml:
	[ ! -d "$(DIR_BUILD_DOC)" ] && mkdir "$(DIR_BUILD_DOC)" || true
	touch "$(DIR_BUILD_DOC)/.gitkeep"
	(cd "$(DIR_DOCUMENT)" && doxygen)

.PHONY: docapi
docapi: docxml
	breathe-apidoc -f -o "$(DIR_DOC_API)" "$(DIR_BUILD_XML)"

.PHONY: docex
docex:
	./gen_doc.py "$(DIR_EXAMPLE)" "$(DIR_DOC_EX)"

.PHONY: dochtml
dochtml: docex docapi
	sphinx-build -M html "$(DIR_DOCUMENT)" "$(DIR_BUILD_DOC)"


###############################################################################

ENVIRONMENTS	:=	\
					esp8266 \
					nano \

PROJECTS		:=	$(wildcard $(DIR_EXAMPLE)/*)
BUILDS			:=	$(addprefix build_,$(PROJECTS))
CLEANS			:=	$(addprefix clean_,$(PROJECTS))

define _pio
	platformio run --project-dir "$(1)" --environment $(2) $(3);
endef

cleansample: $(CLEANS)
$(CLEANS): $(PROJECTS)
	$(foreach env,$(ENVIRONMENTS),$(call _pio,$(patsubst clean_%,%,$@),$(env),--target clean))

buildsample: $(BUILDS)
$(BUILDS): $(PROJECTS)
	$(foreach env,$(ENVIRONMENTS),$(call _pio,$(patsubst build_%,%,$@),$(env)))


###############################################################################

.PHONY: build
build: buildsample

.PHONY: clean
clean: cleansample cleandoc

.PHONY: doc
doc: dochtml

.PHONY: travis
travis: buildsample
