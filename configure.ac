AC_INIT([gingr], [1.3])
AC_CONFIG_SRCDIR([src/MainWindow.cpp])

AC_ARG_WITH(harvest, [  --with-harvest=<path/to/harvest>     Protocol Buffer install dir (default: /usr/local/])
AC_ARG_WITH(capnp, [  --with-capnp=<path/to/capnp>         Cap'n Proto install dir (default: /usr/local/)])
AC_ARG_WITH(protobuf, [  --with-protobuf=<path/to/protobuf>   Protocol Buffers install dir (default: /usr/local/)])

if test "$with_harvest" == ""
then
	with_harvest=/usr/local/
fi

if test "$with_protobuf" == ""
then
	with_protobuf=/usr/local/
fi

if test "$with_capnp" == ""
then
	with_capnp=/usr/local/
fi

AC_LANG(C++)
CPPFLAGS="-std=c++17 -I$with_harvest/include -I$with_protobuf/include -I$with_capnp/include"

AC_CHECK_HEADER(google/protobuf/stubs/common.h, [result=1], [result=0])

if test $result == 0
then
	AC_MSG_ERROR([Protocol Buffers not found.])
fi

AC_CHECK_HEADER(capnp/serialize.h, [result2=1], [result2=0])

if test $result2 == 0
then
	AC_MSG_ERROR([Cap'n Proto headers not found.])
fi

AC_CHECK_HEADER(harvest/HarvestIO.h, [result=1], [result=0])

if test $result == 0
then
	AC_MSG_ERROR([Harvest not found.])
fi

AC_CHECK_HEADER(zlib.h, [result=1], [result=0])

if test $result == 0
then
	AC_MSG_ERROR([Zlib not found.])
fi

# Locate the static Protocol Buffers library. On Debian/Ubuntu the system
# package installs it under a multiarch directory (lib/x86_64-linux-gnu)
# rather than directly under lib/, so check the common locations.
protobuf_lib=""
for protobuf_libdir in "$with_protobuf/lib" "$with_protobuf/lib/x86_64-linux-gnu" "$with_protobuf/lib64"
do
	if test -f "$protobuf_libdir/libprotobuf.a"
	then
		protobuf_lib="$protobuf_libdir/libprotobuf.a"
		break
	fi
done

if test "$protobuf_lib" == ""
then
	AC_MSG_ERROR([libprotobuf.a not found under $with_protobuf.])
fi

AC_SUBST(capnp, $with_capnp)
AC_SUBST(protobuf, $with_protobuf)
AC_SUBST(protobuf_lib, $protobuf_lib)
AC_SUBST(harvest, $with_harvest)
AC_CONFIG_FILES([gingr.pro])
AC_OUTPUT
