#######################
#
#   contrib libs
#

libs: \
	libcurl libdirectfb libdirectfbpp libppdirectfb libdvbpsi \
	libfreetype libjpeg libmad libid3tag libncurses libpng \
	libreadline libsdl libsigc libz libdvb libtool libFLAC

libs_optional: \
	libffi \
	libboost libgmp \
	libpcap libxml2 giflib \
	libexpat libcrypto sqlite

libs_host : \
	libgmp_host libmpfr_host libppl_host libcloog_host

libs_all: libs libs_optional libs_host

$(DEPDIR)/libboost: bootstrap @DEPENDS_libboost@
	@PREPARE_libboost@
	cd @DIR_libboost@ && \
		@INSTALL_libboost@
	@CLEANUP_libboost@
	touch $@

$(DEPDIR)/libcommoncplusplus: bootstrap libxml2 @DEPENDS_libcommoncplusplus@
	@PREPARE_libcommoncplusplus@
	cd @DIR_libcommoncplusplus@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		rm -f $(hostprefix)/bin/ccgnu2-config && \
		sed -e "s,^prefix=,prefix=$(targetprefix)," < src/ccgnu2-config > $(hostprefix)/bin/ccgnu2-config && \
		chmod 755 $(hostprefix)/bin/ccgnu2-config && \
		@INSTALL_libcommoncplusplus@
	@CLEANUP_libcommoncplusplus@
	touch $@

$(DEPDIR)/libcrypto: bootstrap libz @DEPENDS_libcrypto@
	@PREPARE_libcrypto@
	cd @DIR_libcrypto@ && \
		sed -e 's|__TUXBOX_CC__|$(target)-gcc|' -e 's|__TUXBOX_CFLAGS__|$(TARGET_CFLAGS)|' ./Configure > ./Configure.tuxbox && \
		$(BUILDENV) \
		sh ./Configure.tuxbox \
			-DOPENSSL_SMALL_FOOTPRINT \
			shared no-ec no-err no-engine no-hw zlib-dynamic \
			no-idea no-md2 no-mdc2 no-rc5 no-sha0 no-smime \
			no-rmd160 no-aes192 no-ripemd no-camellia no-ans1 no-krb5 \
			tuxbox --prefix=/ --openssldir=/ && \
		$(MAKE) depend MAKEDEPPROG=$(target)-gcc ZLIB_INCLUDE="-I$(targetprefix)/include" && \
		$(MAKE) -j1 all && \
		@INSTALL_libcrypto@
	sed -e "s,^prefix=,prefix=$(targetprefix)," < @DIR_libcrypto@/openssl.pc > $(targetprefix)/lib/pkgconfig/openssl.pc && \
	sed -e "s,^prefix=,prefix=$(targetprefix)," < @DIR_libcrypto@/libcrypto.pc > $(targetprefix)/lib/pkgconfig/libcrypto.pc && \
	sed -e "s,^prefix=,prefix=$(targetprefix)," < @DIR_libcrypto@/libssl.pc > $(targetprefix)/lib/pkgconfig/libssl.pc && \
	@CLEANUP_libcrypto@
	touch $@

if ENABLE_UCLIBC
CURL_UCLIBC=ac_cv_func_inet_ntoa_r=no
endif
if CURL_CURRENT
$(DEPDIR)/libcurl: bootstrap libz @DEPENDS_libcurl_current@
	@PREPARE_libcurl_current@
	cd @DIR_libcurl_current@ && \
		$(BUILDENV) \
		$(CURL_UCLIBC) \
		CPPFLAGS='-I$(targetprefix)/include' \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--disable-gopher \
			--disable-file \
			--disable-rtsp \
			--disable-imap \
			--disable-pop3 \
			--disable-smtp \
			--disable-ldap \
			--disable-dict \
			--disable-telnet \
			--without-ssl \
			--disable-manual \
			--disable-verbose \
			--with-random && \
		$(MAKE) all && \
		rm -f $(hostprefix)/bin/curl-config && \
		sed -e "s,^prefix=,prefix=$(targetprefix)," < curl-config > $(hostprefix)/bin/curl-config && \
		chmod 755 $(hostprefix)/bin/curl-config && \
		@INSTALL_libcurl_current@
	@CLEANUP_libcurl_current@
	touch $@
else
$(DEPDIR)/libcurl: bootstrap @DEPENDS_libcurl@
	@PREPARE_libcurl@
	cd @DIR_libcurl@ && \
		$(BUILDENV) \
		$(CURL_UCLIBC) \
		CPPFLAGS='-I$(targetprefix)/include' \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--disable-gopher \
			--disable-ldap \
			--disable-dict \
			--disable-telnet \
			--without-ssl \
			--with-random && \
		$(MAKE) all && \
		rm -f $(hostprefix)/bin/curl-config && \
		sed -e "s,^prefix=,prefix=$(targetprefix)," < curl-config > $(hostprefix)/bin/curl-config && \
		chmod 755 $(hostprefix)/bin/curl-config && \
		@INSTALL_libcurl@
	@CLEANUP_libcurl@
	touch $@
endif

$(DEPDIR)/libdirectfb: bootstrap libfreetype libjpeg libpng libz @DEPENDS_libdirectfb@
	@PREPARE_libdirectfb@
	cd @DIR_libdirectfb@ && \
		$(BUILDENV) \
		LDFLAGS=-L$(targetprefix)/lib \
		CPPFLAGS="-I$(buildprefix)/linux/arch/ppc" \
		CFLAGS="$(TARGET_CFLAGS) -I$(buildprefix)/linux/arch/ppc" \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--disable-debug \
			--with-inputdrivers=linuxinput \
			--disable-sdl \
			--disable-vnc \
			--disable-multi \
			--without-tools \
			--with-gfxdrivers=none && \
		$(MAKE) all && \
		@INSTALL_libdirectfb@
	@CLEANUP_libdirectfb@
	mv $(targetprefix)/lib/libdirectfb.la $(targetprefix)/lib/libdirectfb.la.old
	mv $(targetprefix)/lib/libfusion.la $(targetprefix)/lib/libfusion.la.old
	mv $(targetprefix)/bin/directfb-config $(targetprefix)/bin/directfb-config.old
	sed -e "s, /lib, $(targetprefix)/lib,g" < $(targetprefix)/lib/libdirectfb.la.old >$(targetprefix)/lib/libdirectfb.la
	sed -e "s, /lib, $(targetprefix)/lib,g" < $(targetprefix)/lib/libfusion.la.old >$(targetprefix)/lib/libfusion.la
	sed -e "s,^prefix=,prefix=$(targetprefix)," < $(targetprefix)/bin/directfb-config.old >$(targetprefix)/bin/directfb-config
	chmod 755 $(targetprefix)/bin/directfb-config
	rm -f $(targetprefix)/lib/libdirectfb.la.old
	rm -f $(targetprefix)/lib/libfusion.la.old
	rm -f $(targetprefix)/bin/directfb-config.old
	rm -f $(hostprefix)/bin/directfb-config
	ln -s $(targetprefix)/bin/directfb-config $(hostprefix)/bin/directfb-config
	touch $@

$(DEPDIR)/libdirectfbpp: bootstrap libdirectfb @DEPENDS_libdirectfbpp@
	@PREPARE_libdirectfbpp@
	cd @DIR_libdirectfbpp@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libdirectfbpp@
	@CLEANUP_libdirectfbpp@
	mv $(targetprefix)/lib/libdfb++.la $(targetprefix)/lib/libdfb++.la.old
	sed -e "s, /lib, $(targetprefix)/lib,g" < $(targetprefix)/lib/libdfb++.la.old >$(targetprefix)/lib/libdfb++.la
	rm -f $(targetprefix)/lib/libdfb++.la.old
	touch $@

$(DEPDIR)/libppdirectfb: bootstrap libdirectfb @DEPENDS_libppdirectfb@
	@PREPARE_libppdirectfb@
	cd @DIR_libppdirectfb@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libppdirectfb@
	@CLEANUP_libppdirectfb@
	mv $(targetprefix)/lib/lib++dfb.la $(targetprefix)/lib/lib++dfb.la.old
	sed -e "s, /lib, $(targetprefix)/lib,g" < $(targetprefix)/lib/lib++dfb.la.old >$(targetprefix)/lib/lib++dfb.la
	rm -f $(targetprefix)/lib/lib++dfb.la.old
	touch $@

$(DEPDIR)/libdvb: bootstrap @DEPENDS_libdvb@
	@PREPARE_libdvb@
	cd @DIR_libdvb@ && \
		$(BUILDENV) \
		CFLAGS="$(TARGET_CFLAGS) -I$(DVB_INCLUDESDIR) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE" \
		$(MAKE) libdvb.a libdvbci.a libdvbmpegtools.a && \
		@INSTALL_libdvb@
	@CLEANUP_libdvb@
	touch $@

$(DEPDIR)/libdvbpsi: bootstrap @DEPENDS_libdvbpsi@
	@PREPARE_libdvbpsi@
	cd @DIR_libdvbpsi@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libdvbpsi@
	@CLEANUP_libdvbpsi@
	touch $@

$(DEPDIR)/libexpat: bootstrap @DEPENDS_libexpat@
	@PREPARE_libexpat@
	cd @DIR_libexpat@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libexpat@
	@CLEANUP_libexpat@
	touch $@

$(DEPDIR)/libffi: bootstrap @DEPENDS_libffi@
	@PREPARE_libffi@
	cd @DIR_libffi@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libffi@
	@CLEANUP_libffi@
	touch $@

$(DEPDIR)/libfreetype: bootstrap libz @DEPENDS_libfreetype@
	@PREPARE_libfreetype@
	cd @DIR_libfreetype@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		rm -f $(hostprefix)/bin/freetype-config && \
		sed -e "s,^prefix=,prefix=$(targetprefix)," < builds/unix/freetype-config > $(hostprefix)/bin/freetype-config && \
		chmod 755 $(hostprefix)/bin/freetype-config && \
		@INSTALL_libfreetype@
	@CLEANUP_libfreetype@
	touch $@

$(DEPDIR)/libfribidi: bootstrap @DEPENDS_libfribidi@
	@PREPARE_libfribidi@
	cd @DIR_libfribidi@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--enable-memopt && \
		$(MAKE) all && \
		@INSTALL_libfribidi@
	@CLEANUP_libfribidi@
	touch $@

$(DEPDIR)/libgmp: bootstrap @DEPENDS_libgmp@
	@PREPARE_libgmp@
	cd @DIR_libgmp@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libgmp@
	@CLEANUP_libgmp@
	touch $@

$(DEPDIR)/libid3tag: bootstrap libz @DEPENDS_libid3tag@
	@PREPARE_libid3tag@
	cd @DIR_libid3tag@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--enable-shared=yes && \
		$(MAKE) all && \
		@INSTALL_libid3tag@
	@CLEANUP_libid3tag@
	touch $@

$(DEPDIR)/libjpeg: bootstrap @DEPENDS_libjpeg@
	@PREPARE_libjpeg@
	cd @DIR_libjpeg@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) libjpeg.so.6 && \
		@INSTALL_libjpeg@
	@CLEANUP_libjpeg@
	touch $@

if !BOXTYPE_SPARK
LIBMAD_OPT=--enable-speed --enable-fpm=$(CPU_ARCH)
endif

$(DEPDIR)/libmad: bootstrap libz @DEPENDS_libmad@
	@PREPARE_libmad@
	cd @DIR_libmad@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--enable-shared=yes \
			$(LIBMAD_OPT) \
			--enable-sso && \
		$(MAKE) all && \
		@INSTALL_libmad@
	@CLEANUP_libmad@
	touch $@

$(DEPDIR)/libncurses: bootstrap @DEPENDS_libncurses@
	@PREPARE_libncurses@
	cd @DIR_libncurses@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--with-terminfo-dirs=/share/terminfo \
			--disable-big-core \
			--without-debug \
			--without-progs \
			--without-ada \
			--with-shared \
			--without-profile \
			--disable-rpath \
			--without-cxx-binding \
			--with-fallbacks='linux vt100 xterm' && \
		$(MAKE) -j1 libs \
			HOSTCC=$(CC) \
			HOSTCCFLAGS="$(CFLAGS) -DHAVE_CONFIG_H -I../ncurses -DNDEBUG -D_GNU_SOURCE -I../include" \
			HOSTLDFLAGS="$(LDFLAGS)" && \
			@INSTALL_libncurses@
	@CLEANUP_libncurses@
	touch $@

$(DEPDIR)/libpcap: bootstrap @DEPENDS_libpcap@
	@PREPARE_libpcap@
	cd @DIR_libpcap@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--with-pcap=linux && \
		$(MAKE) all && \
		@INSTALL_libpcap@
	@CLEANUP_libpcap@
	touch $@

$(DEPDIR)/libpng: bootstrap libz @DEPENDS_libpng@
	@PREPARE_libpng@
	cd @DIR_libpng@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix=$(targetprefix) && \
		$(MAKE) all && \
		$(MAKE) install && $(target)-ar cru $(targetprefix)/lib/libpng12_pic.a *.o && \
		rm -f $(hostprefix)/bin/libpng-config && \
		ln -s $(targetprefix)/bin/libpng-config $(hostprefix)/bin/libpng-config
	@CLEANUP_libpng@
	touch $@

$(DEPDIR)/libreadline: bootstrap @DEPENDS_libreadline@
	@PREPARE_libreadline@
	cd @DIR_libreadline@ && \
		autoconf && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) all && \
		@INSTALL_libreadline@
	@CLEANUP_libreadline@
	touch $@

$(DEPDIR)/libsdl: bootstrap libdirectfb @DEPENDS_libsdl@
	@PREPARE_libsdl@
	cd @DIR_libsdl@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--disable-alsa \
			--disable-openbsdaudio \
			--disable-esd \
			--disable-arts \
			--disable-nas \
			--disable-diskaudio \
			--disable-nasm \
			--disable-nanox \
			--disable-video-x11 \
			--without-x \
			--enable-video-fbcon \
			--disable-video-photon \
			--disable-video-directfb \
			--disable-video-ps2gs \
			--disable-video-ggi \
			--disable-video-svga \
			--disable-video-vgl \
			--disable-video-aalib \
			--disable-video-dummy \
			--disable-video-opengl \
			--disable-stdio-redirect \
			--disable-directx && \
		$(MAKE) all && \
		rm -f $(hostprefix)/bin/sdl-config && \
		sed -e "s,^prefix=,prefix=$(targetprefix)," < sdl-config > $(hostprefix)/bin/sdl-config && \
		chmod 755 $(hostprefix)/bin/sdl-config && \
		@INSTALL_libsdl@
	@CLEANUP_libsdl@
	touch $@

$(DEPDIR)/libsigc: bootstrap @DEPENDS_libsigc@
	@PREPARE_libsigc@
	cd @DIR_libsigc@ && \
		sed -i  -e 's,^SUBDIRS = sigc++.*,SUBDIRS = sigc++,g' \
			-e 's,^DIST_SUBDIRS = $(SUBDIRS).*,DIST_SUBDIRS = \$$\(SUBDIRS\),g'  Makefile.in && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--disable-checks && \
		$(MAKE) all && \
		@INSTALL_libsigc@
	@CLEANUP_libsigc@
	touch $@

$(DEPDIR)/libvorbisidec: bootstrap @DEPENDS_libvorbisidec@
	@PREPARE_libvorbisidec@
	cd @DIR_libvorbisidec@ && \
		$(BUILDENV) \
		./autogen.sh \
			--build=$(build) \
			--host=$(target) \
			--prefix= && \
		$(MAKE) && \
		@INSTALL_libvorbisidec@
	@CLEANUP_libvorbisidec@
	touch $@

$(DEPDIR)/libxml2: bootstrap @DEPENDS_libxml2@
	@PREPARE_libxml2@
	cd @DIR_libxml2@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix= \
			--without-html-dir \
			--with-threads \
			--without-ftp \
			--without-http \
			--without-html \
			--without-catalog \
			--without-docbook \
			--with-xpath \
			--without-xptr \
			--without-xinclude \
			--without-iconv \
			--without-debug \
			--without-mem-debug \
			--without-python && \
		$(MAKE) all && \
		@INSTALL_libxml2@
	@CLEANUP_libxml2@
	touch $@

$(DEPDIR)/libz: bootstrap @DEPENDS_libz@
	@PREPARE_libz@
	cd @DIR_libz@ && \
		$(BUILDENV) \
		./configure \
			--prefix= \
			--shared && \
		$(MAKE) all && \
		@INSTALL_libz@
	@CLEANUP_libz@
	touch $@

$(DEPDIR)/libglib: bootstrap @LIBGETTEXT@ @DEPENDS_libglib@
	@PREPARE_libglib@
	echo "ac_cv_func_posix_getpwuid_r=yes" > @DIR_libglib@/config.cache
	echo "glib_cv_stack_grows=no" >> @DIR_libglib@/config.cache
	echo "glib_cv_uscore=no" >> @DIR_libglib@/config.cache
	cd @DIR_libglib@ && \
		$(BUILDENV) \
		./configure \
			--cache-file=config.cache \
			--build=$(build) \
			--host=$(target) \
			--prefix=$(targetprefix) &&\
		$(MAKE) all && \
		@INSTALL_libglib@
	@CLEANUP_libglib@
	touch $@

$(DEPDIR)/giflib: bootstrap @DEPENDS_giflib@
	@PREPARE_giflib@
	cd @DIR_giflib@ && \
		sed -i -e 's,^SUBDIRS = lib.*,SUBDIRS = lib,g' Makefile.in && \
		export ac_cv_prog_have_xmlto=no && \
		$(BUILDENV) \
		./configure \
			--host=$(target) \
			--build=$(build) \
			--prefix= \
		$(MAKE) && \
		@INSTALL_giflib@
	@CLEANUP_giflib@
	touch $@

$(DEPDIR)/libiconv: bootstrap @DEPENDS_libiconv@
	@PREPARE_libiconv@
	cd @DIR_libiconv@ && \
		$(BUILDENV) \
		./configure \
			--host=$(target) \
			--build=$(build) \
			--prefix= && \
		$(MAKE) && \
		@INSTALL_libiconv@
	@CLEANUP_libiconv@
	touch $@

$(DEPDIR)/libFLAC: bootstrap @DEPENDS_libFLAC@
	@PREPARE_libFLAC@
	cd @DIR_libFLAC@ && \
		$(BUILDENV) \
		./configure \
			--host=$(target) \
			--build=$(build) \
			--prefix= \
			--disable-ogg \
			--disable-altivec && \
		$(MAKE) -C src/libFLAC && \
		$(MAKE) install  -C include/FLAC DESTDIR=$(targetprefix) && \
		@INSTALL_libFLAC@
	@CLEANUP_libFLAC@
	touch $@

$(DEPDIR)/libgettext: bootstrap @DEPENDS_libgettext@
	@PREPARE_libgettext@
	cd @DIR_libgettext@/gettext-runtime && \
		$(BUILDENV) \
		./configure \
			--host=$(target) \
			--build=$(build) \
			--disable-java \
			--prefix= && \
		$(MAKE) -C intl && \
		@INSTALL_libgettext@
	@CLEANUP_libgettext@
	touch $@

# ripped from uclibc/.../libtool.mk
$(DEPDIR)/libtool: bootstrap @DEPENDS_libtool@
	@PREPARE_libtool@
	cd @DIR_libtool@ && \
		$(BUILDENV) \
		./configure \
			--target=$(target) \
			--host=$(target) \
			--build=$(build) \
			--prefix=/usr \
			--exec-prefix=/usr \
			--bindir=/usr/bin \
			--sbindir=/usr/sbin \
			--libexecdir=/usr/lib \
			--sysconfdir=/etc \
			--datadir=/usr/share \
			--localstatedir=/var \
			--mandir=/usr/man \
			--infodir=/usr/info && \
		$(MAKE) CC=$(target)-gcc && \
		$(INSTALL) -m755 libtool $(hostprefix)/bin
	@CLEANUP_libtool@
	touch $@

$(DEPDIR)/sqlite: bootstrap @DEPENDS_sqlite@
	@PREPARE_sqlite@
	cd @DIR_sqlite@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix=$(targetprefix) && \
		$(MAKE) all && \
		@INSTALL_sqlite@
	@CLEANUP_sqlite@
	touch $@

flash-sqlite: $(flashprefix)/root/bin/sqlite3

$(flashprefix)/root/bin/sqlite3: @DEPENDS_sqlite@ | $(flashprefix)/root
	@PREPARE_sqlite@
	cd @DIR_sqlite@ && \
		$(BUILDENV) \
		./configure \
			--build=$(build) \
			--host=$(target) \
			--prefix=$(flashprefix)/root && \
		$(MAKE) all && \
		$(MAKE) install PREFIX=$(flashprefix)/root
	@CLEANUP_sqlite@
	@FLASHROOTDIR_MODIFIED@

$(DEPDIR)/libfaad2: bootstrap @DEPENDS_libfaad2@
	@PREPARE_libfaad2@
	sed -i 's,-iquote $$(top_srcdir),-DFIXED_POINT -I$$(top_srcdir),g' faad2-2.7/libfaad/Makefile.am
	sed -i 's,-iquote $$(top_srcdir),-DFIXED_POINT -I$$(top_srcdir),g' faad2-2.7/libfaad/Makefile.in
	cd @DIR_libfaad2@ && \
		$(BUILDENV) \
		./configure \
			--host=$(target) \
			--build=$(build) \
			--without-xmms \
			--prefix= && \
		$(MAKE) && \
		@INSTALL_libfaad2@
	@CLEANUP_libfaad2@
	touch $@

$(DEPDIR)/libgmp_host: @DEPENDS_libgmp_host@
	@PREPARE_libgmp_host@
	cd @DIR_libgmp_host@ && \
		CC=$(CC) \
		CFLAGS="-pipe -fexceptions" \
		./configure \
			--prefix=$(hostprefix) \
			--disable-shared \
			--enable-static \
			--enable-cxx \
			--enable-fft \
			--enable-mpbsd && \
		$(MAKE) && \
		@INSTALL_libgmp_host@
	@CLEANUP_libgmp_host@
	touch $@

$(DEPDIR)/libmpfr_host: @DEPENDS_libmpfr_host@ libgmp_host
	@PREPARE_libmpfr_host@
	cd @DIR_libmpfr_host@ && \
		CC=$(CC) \
		CFLAGS="-pipe" \
		./configure \
			--prefix=$(hostprefix) \
			--disable-shared \
			--enable-static \
			--with-gmp=$(hostprefix) && \
		$(MAKE) && \
		@INSTALL_libmpfr_host@
	@CLEANUP_libmpfr_host@
	touch $@

$(DEPDIR)/libmpc_host: @DEPENDS_libmpc_host@ libmpfr_host
	@PREPARE_libmpc_host@
	cd @DIR_libmpc_host@ && \
		CC=$(CC) \
		CFLAGS="-pipe" \
		./configure \
			--prefix=$(hostprefix) \
			--disable-shared \
			--enable-static \
			--with-gmp=$(hostprefix) \
			--with-mpfr=$(hostprefix) && \
		$(MAKE) && \
		@INSTALL_libmpc_host@
	@CLEANUP_libmpc_host@
	touch $@

$(DEPDIR)/libppl_host: @DEPENDS_libppl_host@ libmpc_host
	@PREPARE_libppl_host@
	cd @DIR_libppl_host@ && \
		CC=$(CC) \
		CFLAGS="-pipe" \
		CXXFLAGS="-pipe" \
		CPPFLAGS="-I$(hostprefix)/include" \
		LDFLAGS="-Wl,-rpath,$(hostprefix)/lib" \
		./configure \
			--prefix=$(hostprefix) \
			--disable-debugging \
			--disable-assertions \
			--disable-ppl_lcdd \
			--disable-ppl_lpsol \
			--disable-shared \
			--enable-interfaces="c c++" \
			--enable-static \
			--with-gmp=$(hostprefix) && \
		$(MAKE) && \
		@INSTALL_libppl_host@
	@CLEANUP_libppl_host@
	touch $@

$(DEPDIR)/libcloog_host: @DEPENDS_libcloog_host@ libppl_host
	@PREPARE_libcloog_host@
	cd @DIR_libcloog_host@ && \
		cp -v configure{,.orig} && \
		sed -e "/LD_LIBRARY_PATH=/d" configure.orig > configure && \
		CC=$(CC) \
		CFLAGS="-pipe" \
		LIBS="-lm" \
		./configure \
			--prefix=$(hostprefix) \
			--with-bits=gmp \
			--with-host-libstdcxx=-lstdc++ \
			--disable-shared \
			--enable-static \
			--with-gmp-prefix=$(hostprefix) && \
		$(MAKE) && \
		@INSTALL_libcloog_host@
	@CLEANUP_libcloog_host@
	touch $@
