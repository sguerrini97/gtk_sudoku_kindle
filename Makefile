CROSS_COMPILE = arm-linux-gnueabi-
CFLAGS = -pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz
LDFLAGS = -Wl,-rpath,../lib -L/usr/arm-linux-gnueabi/lib -L../lib -lgtk-x11-2.0 -lgdk-x11-2.0 -lXrender -lXinerama -lXext -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lXdamage -lXfixes -latk-1.0 -lcairo -lpixman-1 -lpng16 -lxcb-shm -lxcb-render -lX11 -lxcb -lXau -lXdmcp -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lfontconfig -lfreetype -lz -lexpat -lgobject-2.0 -lffi -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 -lbz2
SRC = sudoku.c
OUT = sudoku
REMOTE_PATH = /mnt/us/extensions/sudoku/bin

clean:
	rm $(OUT)

pc: $(SRC)
	gcc $(SRC) -DBUILD_PC -o $(OUT) `pkg-config gtk+-2.0 --cflags --libs`

kindle: $(SRC)
	$(CROSS_COMPILE)gcc -DBUILD_KINDLE $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

deploy-wifi:
	scp $(OUT) root@192.168.254.2:$(REMOTE_PATH)

deploy-usb:
	scp $(OUT) root@192.168.15.244:$(REMOTE_PATH)
