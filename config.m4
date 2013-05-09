dnl $Id$
dnl config.m4 for extension object

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(object, for object support,
dnl Make sure that the comment is aligned:
dnl [  --with-object             Include object support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(object, whether to enable Object support,
[  --enable-object           Enable Object support])

if test "$PHP_OBJECT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-object -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/object.h"  # you most likely want to change this
  dnl if test -r $PHP_OBJECT/$SEARCH_FOR; then # path given as parameter
  dnl   OBJECT_DIR=$PHP_OBJECT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for object files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OBJECT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OBJECT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the object distribution])
  dnl fi

  dnl # --with-object -> add include path
  dnl PHP_ADD_INCLUDE($OBJECT_DIR/include)

  dnl # --with-object -> check for lib and symbol presence
  dnl LIBNAME=object # you may want to change this
  dnl LIBSYMBOL=object # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OBJECT_DIR/lib, OBJECT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OBJECTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong object lib version or lib not found])
  dnl ],[
  dnl   -L$OBJECT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OBJECT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(object, object.c, $ext_shared)
fi
