
###############################################################################
# MODULE     : makefile which can be included in administrative makefiles
# BY         : Joris van der Hoeven
# COPYRIGHT  : This software falls under the GNU general public license;
#              see the file 'LICENSE', which is provided with this package.
###############################################################################

prefix = /usr/local
exec_prefix = /usr/local
includedir = /usr/local/include
libdir = /usr/local/lib
bindir = /usr/local/bin
datadir = /usr/local/share
mandir = /usr/local/man
tmdir = TeXmacs
tmtgz = TeXmacs-1.0.6.8
tmrpm = TeXmacs-1.0.6.8-1
tmorig = /home/vdhoeven/texmacs/src
tmsrc = /home/vdhoeven/texmacs/src/TeXmacs
tmbin = /usr/local/libexec/TeXmacs
tmdata = /usr/local/share/TeXmacs
so = so
os = gnu-linux

MKDIR = mkdir -p
RM = rm -f
CP = cp -r -f
MV = mv -f
LN = ln -f
CHMOD = chmod -f
GZIP = gzip -f
STRIP = true
TOUCH = touch
TAR = tar
