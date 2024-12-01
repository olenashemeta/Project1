CDIR = client
SDIR = server

all: build

build:
	$(MAKE) -C $(CDIR)
	$(MAKE) -C $(SDIR)

clean:
	$(MAKE) -C $(CDIR) clean
	$(MAKE) -C $(SDIR) clean
