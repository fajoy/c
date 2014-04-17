#ref http://www.gnu.org/software/make/manual/make.html#Quick-Reference
#ref https://www.gnu.org/software/make/manual/html_node/Special-Variables.html#Special-Variables
name1 := $(lastword $(MAKEFILE_LIST))
include inc.mk
name2 := $(lastword $(MAKEFILE_LIST))
abspath = $(abspath $(name1))
dir = $(dir $(abspath))
patsubst = $(patsubst %/,%,$(dir))
all:
	@echo CURDIR = $(CURDIR)
	@echo notdir  CURDIR = $(notdir  ,$(CURDIR))
	@echo MAKEFILE_LIST = $(MAKEFILE_LIST)
	@echo name1 = $(name1)
	@echo name2 = $(name2)

	@echo abspath = $(abspath)
	@echo dir = $(dir)
	@echo patsubst = $(patsubst)

