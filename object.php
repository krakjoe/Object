<?php
if (!extension_loaded("Object")) {
    dl("object.so") or die(
        "Cannot load Object extension"
    );
}

class My {
    public function myTest(){
        return __METHOD__;
    }
}

class Other {
    public function otherTest(){
        return __METHOD__;
    }
}

$test = new stdClass();

var_dump(Object::is($test, stdClass::class));       // bool(true)
var_dump(Object::is($test, My::class));             // bool(false)
var_dump(Object::extend($test, My::class));         // bool(true)
var_dump(Object::is($test, My::class));             // bool(true)
var_dump(Object::is($test, Other::class));          // bool(false)
var_dump(Object::extend($test, Other::class));      // bool(true)
var_dump(Object::is($test, Other::class));          // bool(true)

?>
