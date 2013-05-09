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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_object.h"

ZEND_DECLARE_MODULE_GLOBALS(object)

zend_class_entry *object_class_entry;

ZEND_BEGIN_ARG_INFO_EX(Object_extend, 0, 0, 2)
    ZEND_ARG_INFO(0, object)
    ZEND_ARG_INFO(0, with)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Object_is, 0, 0, 2)
    ZEND_ARG_INFO(0, object)
    ZEND_ARG_INFO(0, with)
ZEND_END_ARG_INFO()

static inline void object_globals_ctor(zend_object_globals *og TSRMLS_CC){}
static inline void object_globals_dtor(zend_object_globals *og TSRMLS_CC){}

static zend_bool zend_boolean_true = 1;
static zend_bool zend_boolean_false = 0;

/* {{{ object_functions[]
 *
 */
const zend_function_entry object_methods[] = {
	PHP_ME(Object, extend,	Object_extend, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_STATIC)
	PHP_ME(Object, is,	    Object_is,     ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_STATIC)
	PHP_FE_END
};
/* }}} */

/* {{{ object_module_entry
 */
zend_module_entry object_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"Object",
	NULL,
	PHP_MINIT(object),
	PHP_MSHUTDOWN(object),
	PHP_RINIT(object),
	PHP_RSHUTDOWN(object),
	PHP_MINFO(object),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OBJECT
ZEND_GET_MODULE(object)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(object)
{
    zend_class_entry oe;
    
    INIT_CLASS_ENTRY(oe, "Object", object_methods);
    
    object_class_entry = zend_register_internal_class(
        &oe TSRMLS_CC);

#ifdef ZTS
    ZEND_INIT_MODULE_GLOBALS(object, object_globals_ctor, object_globals_dtor);
#else
    object_globals_ctor(&object_globals);    
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(object)
{
#ifndef ZTS
    object_globals_dtor(&object_globals);
#endif

	return SUCCESS;
}
/* }}} */

static inline void object_changed_dtor(void *changes) {
    zend_hash_destroy((HashTable*) changes);
}

static inline HashTable* zend_object_inheritance(zend_object *zobject TSRMLS_DC) {
    HashTable inherit, *inheritance = NULL;
    
    if (zend_hash_index_find(&OBJECT_G(changed), (ulong) zobject, (void**) &inheritance) != SUCCESS) {
        zend_hash_init(
            &inherit, 8, NULL, NULL, 0);
        zend_hash_index_update(
            &OBJECT_G(changed), (ulong) zobject, (void**) &inherit, sizeof(HashTable), (void**) &inheritance);
    }
    
    return inheritance;
}

static inline zend_bool zend_object_implements(zend_object *zobject, zend_class_entry *check TSRMLS_DC) {
    return zend_hash_index_exists(
        zend_object_inheritance(zobject TSRMLS_CC), (ulong) check) || instanceof_function(zobject->ce, check TSRMLS_CC);
}

static inline void zend_object_extend(zend_object *zobject, zend_class_entry *entry TSRMLS_DC) {
    zend_class_entry zecopy = *zobject->ce, *pzecopy = NULL;
	    
    {
        zend_do_inheritance(
            &zecopy, entry TSRMLS_CC
        );
        
        zend_hash_index_update(
            zend_object_inheritance(zobject TSRMLS_CC), (ulong) entry,
            (void**) &zecopy, sizeof(zend_class_entry), (void**) &pzecopy
        );
        
        zobject->ce = pzecopy;
    }
}

PHP_RINIT_FUNCTION(object)
{
    zend_hash_init(
        &OBJECT_G(changed), 8, NULL, object_changed_dtor, 0);
}

PHP_RSHUTDOWN_FUNCTION(object)
{
    zend_hash_destroy(&OBJECT_G(changed));
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(object)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Object methods", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto bool Object::extend(mixed $object, mixed $with)
     Extend $object at runtime $with declaration */
PHP_METHOD(Object, extend)
{
	zval *object;
	zend_class_entry *with = NULL;
    
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oC", &object, &with) != SUCCESS) {
		return;
	}

	{
	    zend_object *zobject = (zend_object*) zend_object_store_get_object(object TSRMLS_CC);
	    
	    if (zobject && !zend_object_implements(zobject, with TSRMLS_CC)) {
	        zend_object_extend(
	            zobject, with TSRMLS_CC);
	        RETURN_BOOL(1);
	    }

	    RETURN_BOOL(0);
	}
}
/* }}} */

/* {{{ proto bool Object::is(mixed $object, mixed $with)
     Check if $object was extended $with at runtime */
PHP_METHOD(Object, is)
{
	zval *object;
	zend_class_entry *with = NULL;
    
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oC", &object, &with) != SUCCESS) {
		return;
	}

	{
	    RETURN_BOOL(
	        zend_object_implements(
	            (zend_object*) zend_object_store_get_object(object TSRMLS_CC), with TSRMLS_CC)
	    );
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
