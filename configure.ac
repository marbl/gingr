AC_INIT(gingr)

AC_ARG_WITH(harvest, [  --with-harvest=<path/to/harvest>     Protocol Buffer install dir (default: /usr/local/])

if test "$with_harvest" == ""
then
	with_harvest=/usr/local/
fi

AC_LANG(C++)
CPPFLAGS="-I $with_harvest/include"
AC_CHECK_HEADER(harvest.h)

AC_SUBST(harvest, $with_harvest)
AC_OUTPUT(gingr.pro)
