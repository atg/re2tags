[namespace] N1
[class] N1::C1
[property] N1::C1::$p1
[class_property] N1::$p2
[constant] N1::C1::p3
[method] N1::C1::m1
[method] N1::C1::m2
[interface] N1::IF1
[method] N1::IF1::m1
[method] N1::IF1::m2
[trait] N1::T1
[method] N1::T1::m1
[method] N1::T1::m2
[function] N1::f1
%%%
<?php

namespace N1 {
    abstract class C1 {
        public $p1 = 123;
        static $p2 = 123;
        const $p3 = 123;
        public function m1() {
            
        }
        function m2() {
            
        }
    }
    interface IF1 {
        private function m1();
        function m2(a, b, c);
    }
    trait T1 {
        private function m1() {
            
        }
        function m2(a, b, c) {
            
        }
    }
    function f1() {
        
    }
}

?>