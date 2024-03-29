extra_config_options = --enable-multi-arch
libc_rtlddir = /lib64
libc_extra_cflags = -mcpu=ultrasparc

# build 32-bit (sparc) alternative library
EGLIBC_PASSES += sparc
DEB_ARCH_REGULAR_PACKAGES += libc6-sparc libc6-dev-sparc
sparc_add-ons = nptl $(add-ons)
sparc_configure_target=sparc-linux-gnu
sparc_CC = $(CC) -m32
sparc_CXX = $(CXX) -m32
libc6-sparc_shlib_dep = libc6-sparc (>= $(shlib_dep_ver))
sparc_extra_config_options = $(extra_config_options) --disable-profile
sparc_rtlddir = /lib
sparc_slibdir = /lib32
sparc_libdir = /usr/lib32

define libc6-dev-sparc_extra_pkg_install

mkdir -p debian/libc6-dev-sparc/usr/include
ln -s sparc64-linux-gnu/bits debian/libc6-dev-sparc/usr/include/
ln -s sparc64-linux-gnu/gnu debian/libc6-dev-sparc/usr/include/
ln -s sparc64-linux-gnu/fpu_control.h debian/libc6-dev-sparc/usr/include/

mkdir -p debian/libc6-dev-sparc/usr/include/sparc64-linux-gnu/gnu
cp -a debian/tmp-sparc/usr/include/gnu/stubs-32.h \
        debian/libc6-dev-sparc/usr/include/sparc64-linux-gnu/gnu

mkdir -p debian/libc6-dev-sparc/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/sparc64-linux-gnu/sys` ; do \
        ln -s ../sparc64-linux-gnu/sys/$$i debian/libc6-dev-sparc/usr/include/sys/$$i ; \
done

endef

define libc6-sparc_extra_pkg_install
mkdir -p debian/$(curpass)/lib
ln -s /lib32/ld-linux.so.2 debian/$(curpass)/lib
endef
