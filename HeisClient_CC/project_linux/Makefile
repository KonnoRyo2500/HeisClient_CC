# heis Linux版ビルド用Makefile
# Author: Ryo Konno

# 参考:  http://giraffydev.hatenablog.com/entry/2016/10/04/101004
# 改善できる部分が多そう．修正・改善希望．

COMPILER = g++
CXXFLAGS = -g -Wall -O2 -MMD -MP
INCLUDE = -I../src/AI -I../src/common -I../src/field -I../src/JSON -I../src/local_mode -I../src/online_mode -I../src/picojson-master/picojson-master -I../src/unit
TARGETS = HeisClient_CC.exe
TARGETDIR = bin
SRCROOT = ../src
OBJROOT = obj
$(eval SRCDIRS := $(shell find $(SRCROOT) -type d))
$(eval SOURCES = $(shell find $(SRCROOT) -name *.cpp))
OBJECTS = $(addprefix $(OBJROOT), $(subst $(SRCROOT), , $(SOURCES:.cpp=.o)))
DEPENDS = $(OBJECTS:.o=.d)

$(TARGETS): $(OBJECTS)
	@if [ ! -e $(TARGETDIR) ]; then mkdir -p $(TARGETDIR); fi
	$(COMPILER) -o $(TARGETDIR)/$@ $^

$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)
