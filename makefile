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
	@echo "build"
	@echo "clean"
	@echo "doc"
	@echo


###############################################################################

.PHONY: cleandoc
cleandoc:
	sphinx-build -M clean "$(DIR_DOCUMENT)" "$(DIR_BUILD_DOC)"

.PHONY: docxml docapi dochtml
docxml:
	[ ! -d "$(DIR_BUILD_DOC)" ] && mkdir "$(DIR_BUILD_DOC)" || true
	(cd "$(DIR_DOCUMENT)" && doxygen)
docapi: docxml
	breathe-apidoc -o "$(DIR_DOC_API)" "$(DIR_BUILD_XML)"
docex:
	./ex_doc.py "$(DIR_EXAMPLE)" "$(DIR_DOC_EX)"
dochtml: docex docapi
	sphinx-build -M html "$(DIR_DOCUMENT)" "$(DIR_BUILD_DOC)"


###############################################################################

P_EXAMPLE		:=	$(wildcard $(DIR_EXAMPLE)/*)
CLEAN_SAMPLE	:=	$(addprefix clean_,$(P_EXAMPLE))
BUILD_SAMPLE	:=	$(addprefix build_,$(P_EXAMPLE))

cleansample: $(CLEAN_SAMPLE)
$(CLEAN_SAMPLE): $(P_EXAMPLE)
	(cd "$(patsubst clean_%,%,$@)" && platformio run --target clean)

buildsample: $(BUILD_SAMPLE)
$(BUILD_SAMPLE): $(P_EXAMPLE)
	(cd "$(patsubst build_%,%,$@)" && platformio run)


###############################################################################

.PHONY: build
build: dochtml buildsample

.PHONY: clean
clean: cleandoc cleansample

.PHONY: doc
doc: dochtml

.PHONY: travis
travis: buildsample
