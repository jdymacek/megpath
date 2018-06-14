all:
	$(MAKE) -C shared
	$(MAKE) -C distributed
	$(MAKE) -C threaded
	$(MAKE) -C openMp
	$(MAKE) -C hybrid

clean:
	$(MAKE) -C shared clean
	$(MAKE) -C distributed clean
	$(MAKE) -C threaded clean
	$(MAKE) -C openMp clean
	$(MAKE) -C hybrid clean
