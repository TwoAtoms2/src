# onceonly.m4 serial 3
dnl Copyright (C) 2002, 2003 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

dnl This file defines some "once only" variants of standard autoconf macros.
dnl   AC_CHECK_HEADERS_ONCE         like  AC_CHECK_HEADERS
dnl   AC_CHECK_FUNCS_ONCE           like  AC_CHECK_FUNCS
dnl   AC_CHECK_DECLS_ONCE           like  AC_CHECK_DECLS
dnl   AC_REQUIRE([AC_HEADER_STDC])  like  AC_HEADER_STDC
dnl The advantage is that the check for each of the headers/functions/decls
dnl will be put only once into the 'configure' file. It keeps the size of
dnl the 'configure' file down, and avoids redundant output when 'configure'
dnl is run.
dnl The drawback is that the checks cannot be conditionalized. If you write
dnl   if some_condition; then gl_CHECK_HEADERS(stdlib.h); fi
dnl inside an AC_DEFUNed function, the gl_CHECK_HEADERS macro call expands to
dnl empty, and the check will be inserted before the body of the AC_DEFUNed
dnl function.

dnl Autoconf version 2.57 or newer is recommended.
AC_PREREQ(2.54)

# AC_CHECK_HEADERS_ONCE(HEADER1 HEADER2 ...) is a once-only variant of
# AC_CHECK_HEADERS(HEADER1 HEADER2 ...).
AC_DEFUN([AC_CHECK_HEADERS_ONCE], [
  :
  AC_FOREACH([gl_HEADER_NAME], [$1], [
    AC_DEFUN([gl_CHECK_HEADER_]m4_quote(translit(defn([gl_HEADER_NAME]),
                                                 [-./], [___])), [
      AC_CHECK_HEADERS(gl_HEADER_NAME)
    ])
    AC_REQUIRE([gl_CHECK_HEADER_]m4_quote(translit(gl_HEADER_NAME,
                                                   [-./], [___])))
  ])
])

# AC_CHECK_FUNCS_ONCE(FUNC1 FUNC2 ...) is a once-only variant of
# AC_CHECK_FUNCS(FUNC1 FUNC2 ...).
AC_DEFUN([AC_CHECK_FUNCS_ONCE], [
  :
  AC_FOREACH([gl_FUNC_NAME], [$1], [
    AC_DEFUN([gl_CHECK_FUNC_]defn([gl_FUNC_NAME]), [
      AC_CHECK_FUNCS(defn([gl_FUNC_NAME]))
    ])
    AC_REQUIRE([gl_CHECK_FUNC_]defn([gl_FUNC_NAME]))
  ])
])

# AC_CHECK_DECLS_ONCE(DECL1 DECL2 ...) is a once-only variant of
# AC_CHECK_DECLS(DECL1, DECL2, ...).
AC_DEFUN([AC_CHECK_DECLS_ONCE], [
  :
  AC_FOREACH([gl_DECL_NAME], [$1], [
    AC_DEFUN([gl_CHECK_DECL_]defn([gl_DECL_NAME]), [
      AC_CHECK_DECLS(defn([gl_DECL_NAME]))
    ])
    AC_REQUIRE([gl_CHECK_DECL_]defn([gl_DECL_NAME]))
  ])
])
