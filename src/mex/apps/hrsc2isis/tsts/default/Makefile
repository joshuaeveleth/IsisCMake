APPNAME = hrsc2isis

include $(ISISROOT)/make/isismake.tsts

commands:
	$(APPNAME) FROM=$(INPUT)/h0279_0000_re2.img TO=$(OUTPUT)/h0279_0000_re2.cub > /dev/null;
	tabledump FROM=$(OUTPUT)/h0279_0000_re2.cub TO=$(OUTPUT)/h0279_0000_re2.txt NAME=LineScanTimes > /dev/null;
	catlab FROM=$(OUTPUT)/h0279_0000_re2.cub to=$(OUTPUT)/h0279_0000_re2.pvl > /dev/null;
	if [ `$(APPNAME) FROM=$(INPUT)/h1580_0008_sr3.img \
	  TO=$(OUTPUT)/junk.cub \
	   >& $(OUTPUT)/error.txt` ]; \
	then true; \
	fi;
