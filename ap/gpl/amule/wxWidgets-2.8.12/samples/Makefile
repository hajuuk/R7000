#
# Makefile : Builds wxWindows samples for Unix.
#

SAMPLES_SUBDIRS=console 

all:
	@for d in $(SAMPLES_SUBDIRS); do (cd $$d && $(MAKE)); done

clean:
	@for d in $(SAMPLES_SUBDIRS); do (cd $$d && $(MAKE) clean); done
