CXX        ?= g++

CXXFLAGS   := -std=c++17 -Wall -Wextra -Wpedantic -c -g -MD -MP -fsanitize=address,undefined
CPPFLAGS   := -Iinclude -Itests -I3pp

LDFLAGS    := -fsanitize=address,undefined
LDLIBS     :=

CATCHFLAGS :=

QUIET      := @

target     := statvec_test
srcdir     := test
builddir   := build

ccext      := cc
oext       := o
dext       := d

obj        := $(patsubst $(srcdir)/%.$(ccext),$(builddir)/%.$(oext),$(wildcard $(srcdir)/*.$(ccext)))

.PHONY: all
all: $(target)

$(target): $(obj)
	$(info [LD]  $(notdir $@))
	$(QUIET)$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(builddir)/%.o: $(srcdir)/%.cc | $(builddir)
	$(info [CXX] $(notdir $@))
	$(QUIET)$(CXX) -o $@ $< $(CXXFLAGS) $(CPPFLAGS)

$(builddir):
	$(QUIET)mkdir -p $@

.PHONY: check
check: $(target)
	$(QUIET)./$^ $(CATCHFLAGS)

.PHONY: clean
clean:
	$(QUIET)rm -rf $(builddir) $(target)

-include $(obj:.$(oext)=.$(dext))
