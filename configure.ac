AC_INIT(gingr)

AC_ARG_WITH(harvest, [  --with-harvest=<path/to/harvest>     Protocol Buffer install dir (default: /usr/local/])
AC_ARG_WITH(protobuf, [  --with-protobuf=<path/to/protobuf>     Protocol Buffers install dir (default: /usr/local/)])

if test "$with_harvest" == ""
then
	with_harvest=/usr/local/
fi

if test "$with_protobuf" == ""
then
	with_protobuf=/usr/local/
fi

AC_LANG(C++)
CPPFLAGS="-I$with_harvest/include -I$with_protobuf/include"

AC_CHECK_HEADER(google/protobuf/stubs/common.h, [result=1], [result=0])

if test $result == 0
then
	AC_MSG_ERROR([Protocol Buffers not found.])
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

AC_SUBST(protobuf, $with_protobuf)
AC_SUBST(harvest, $with_harvest)
AC_OUTPUT(gingr.pro)
