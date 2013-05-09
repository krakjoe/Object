/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Joe Watkins <joe.watkins@live.co.uk>                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_OBJECT_H
#define PHP_OBJECT_H

extern zend_module_entry object_module_entry;
#define phpext_object_ptr &object_module_entry

#ifdef PHP_WIN32
#	define PHP_OBJECT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_OBJECT_API __attribute__ ((visibility("default")))
#else
#	define PHP_OBJECT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(object);
PHP_MSHUTDOWN_FUNCTION(object);
PHP_MINFO_FUNCTION(object);

PHP_RINIT_FUNCTION(object);
PHP_RSHUTDOWN_FUNCTION(object);

PHP_METHOD(Object, extend);
PHP_METHOD(Object, is);

ZEND_BEGIN_MODULE_GLOBALS(object)
    HashTable changed;
ZEND_END_MODULE_GLOBALS(object)

#ifdef ZTS
#define OBJECT_G(v) TSRMG(object_globals_id, zend_object_globals *, v)
#else
#define OBJECT_G(v) (object_globals.v)
#endif

#endif	/* PHP_OBJECT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
