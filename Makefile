NULL = 

SRC = presentation.tex
PANDOC_SRC = presentation.md

OUT = $(SRC:%.tex=%.pdf)
PANDOC_OUT = $(PANDOC_SRC:%.md=%.pdf)

BEAMER_THEME = "Berlin"
#BEAMER_THEME = "lankton-keynote"

default: pandoc

BYPRODUCTS = \
	$(SRC:%.tex=%.aux) \
	$(SRC:%.tex=%.log) \
	$(SRC:%.tex=%.nav) \
	$(SRC:%.tex=%.out) \
	$(SRC:%.tex=%.snm) \
	$(SRC:%.tex=%.toc) \
    $(NULL)

pandoc:
	@echo "Compiling $(PANDOC_SRC) to $(PANDOC_OUT)... "
	@pandoc $(PANDOC_SRC) -t beamer -s -o $(PANDOC_OUT) --smart --normalize \
	-f markdown+footnotes+implicit_figures \
	-H h-lualatexja.tex \
	--latex-engine=lualatex \
	--listings \
	-V theme:$(BEAMER_THEME) \
	-V colortheme:dolphin \
	-V fonttheme:structurebold \
	-V classoption:aspectratio=169 \
	--slide-level 2 \
	--highlight-style kate
	@echo "Done!"

continuous: pandoc
	@echo "The PDF will be updated automatically when you save the $(PANDOC_SRC) document. Press Ctrl+C to abort."
	@while inotifywait -q $(PANDOC_SRC); do sleep 0.1; make --no-print-directory pandoc; done

revealjs:
	@pandoc -s -t revealjs -V theme:default -o output.html $(PANDOC_SRC)

%.pdf: %.tex
	pdflatex $<

clean:
	rm -Rf $(OUT) $(PANDOC_OUT) $(BYPRODUCTS)

.PHONY: all clean

