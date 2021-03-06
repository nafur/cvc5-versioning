; symptom: assertion failure : conflict <=> equality engine inconsistent
;
; issue: the assertion is too strong. what is true is that there is an internal
; conflict <=> equality engine inconsistent. but in case of propagating (dis)equalities
; between shared terms, the some other theory might become inconsistent, and we should
; stop.
; 
(set-info :source |fuzzsmt|)
(set-info :smt-lib-version 2.6)
(set-info :category "random")
(set-info :status sat)
(set-logic QF_UFLIAFS)
(define-sort Element () Int)
(declare-fun f0 ( Int Int Int) Int)
(declare-fun f1 ( (Set Element) (Set Element)) (Set Element))
(declare-fun p0 ( Int Int Int) Bool)
(declare-fun p1 ( (Set Element)) Bool)
(declare-fun v0 () Int)
(declare-fun v1 () (Set Element))
(declare-fun v2 () (Set Element))
(declare-fun v3 () (Set Element))
(declare-fun v4 () (Set Element))
(assert (let ((e5 2))
(let ((e6 (+ v0 v0)))
(let ((e7 (* e6 e5)))
(let ((e8 (* e6 (- e5))))
(let ((e9 (ite (p0 e7 v0 e6) 1 0)))
(let ((e10 (f0 v0 e8 e8)))
(let ((e11 (ite (p1 v1) (set.singleton 1) (set.singleton 0))))
(let ((e12 (set.union v3 v3)))
(let ((e13 (set.inter v3 v1)))
(let ((e14 (ite (p1 v3) (set.singleton 1) (set.singleton 0))))
(let ((e15 (set.inter v2 e14)))
(let ((e16 (ite (p1 e11) (set.singleton 1) (set.singleton 0))))
(let ((e17 (ite (p1 v4) (set.singleton 1) (set.singleton 0))))
(let ((e18 (set.union e15 v2)))
(let ((e19 (ite (p1 e16) (set.singleton 1) (set.singleton 0))))
(let ((e20 (set.inter e18 v3)))
(let ((e21 (set.minus v4 e12)))
(let ((e22 (set.union v3 v2)))
(let ((e23 (set.minus e12 v4)))
(let ((e24 (set.minus v3 e16)))
(let ((e25 (set.inter e19 e20)))
(let ((e26 (ite (p1 e15) (set.singleton 1) (set.singleton 0))))
(let ((e27 (set.minus e17 e15)))
(let ((e28 (f1 e23 e12)))
(let ((e29 (set.member e10 e16)))
(let ((e30 (set.member e10 v1)))
(let ((e31 (set.member e7 e19)))
(let ((e32 (f1 e12 e12)))
(let ((e33 (f1 e16 e25)))
(let ((e34 (f1 v1 e27)))
(let ((e35 (f1 e19 e19)))
(let ((e36 (f1 e24 e32)))
(let ((e37 (f1 e28 e35)))
(let ((e38 (f1 e27 e20)))
(let ((e39 (f1 e23 e23)))
(let ((e40 (f1 e39 e27)))
(let ((e41 (f1 e17 e32)))
(let ((e42 (f1 e34 e33)))
(let ((e43 (f1 e34 e17)))
(let ((e44 (f1 e34 e25)))
(let ((e45 (f1 e26 e26)))
(let ((e46 (f1 e17 e21)))
(let ((e47 (f1 e40 e26)))
(let ((e48 (f1 e16 v2)))
(let ((e49 (f1 e46 e33)))
(let ((e50 (f1 e15 e15)))
(let ((e51 (f1 e18 e18)))
(let ((e52 (f1 e15 e18)))
(let ((e53 (f1 e11 e49)))
(let ((e54 (f1 e14 e42)))
(let ((e55 (f1 e48 e18)))
(let ((e56 (f1 e49 e52)))
(let ((e57 (f1 v4 e12)))
(let ((e58 (f1 e22 e45)))
(let ((e59 (f1 e13 e13)))
(let ((e60 (f1 v3 e36)))
(let ((e61 (distinct e8 e7)))
(let ((e62 (> v0 e8)))
(let ((e63 (< e10 e10)))
(let ((e64 (distinct v0 e10)))
(let ((e65 (<= e7 e8)))
(let ((e66 (distinct e9 v0)))
(let ((e67 (<= e6 e8)))
(let ((e68 (p0 e8 e7 e6)))
(let ((e69 (ite e63 e35 e13)))
(let ((e70 (ite e66 e25 e32)))
(let ((e71 (ite e62 e33 e19)))
(let ((e72 (ite e64 e46 v1)))
(let ((e73 (ite e65 e59 e51)))
(let ((e74 (ite e30 e14 e26)))
(let ((e75 (ite e68 e36 e39)))
(let ((e76 (ite e66 e35 e34)))
(let ((e77 (ite e64 e44 e54)))
(let ((e78 (ite e61 e70 e46)))
(let ((e79 (ite e31 e60 e26)))
(let ((e80 (ite e64 e55 e19)))
(let ((e81 (ite e63 e58 e33)))
(let ((e82 (ite e29 e32 e75)))
(let ((e83 (ite e67 e27 e73)))
(let ((e84 (ite e63 e51 e60)))
(let ((e85 (ite e64 e16 e73)))
(let ((e86 (ite e68 e47 e46)))
(let ((e87 (ite e67 v2 e11)))
(let ((e88 (ite e63 e59 e75)))
(let ((e89 (ite e30 e12 e83)))
(let ((e90 (ite e62 e45 e87)))
(let ((e91 (ite e29 e36 e89)))
(let ((e92 (ite e68 e24 e16)))
(let ((e93 (ite e61 e49 e76)))
(let ((e94 (ite e61 e12 e54)))
(let ((e95 (ite e67 e33 e16)))
(let ((e96 (ite e66 e26 e15)))
(let ((e97 (ite e65 e52 e13)))
(let ((e98 (ite e68 e38 e17)))
(let ((e99 (ite e65 e48 e46)))
(let ((e100 (ite e31 v3 e95)))
(let ((e101 (ite e31 e95 e18)))
(let ((e102 (ite e66 e37 e78)))
(let ((e103 (ite e31 e33 e17)))
(let ((e104 (ite e62 e55 e91)))
(let ((e105 (ite e65 e20 e90)))
(let ((e106 (ite e63 e22 e79)))
(let ((e107 (ite e64 e94 e97)))
(let ((e108 (ite e61 e53 e80)))
(let ((e109 (ite e63 e23 e52)))
(let ((e110 (ite e31 e100 e101)))
(let ((e111 (ite e68 e28 e98)))
(let ((e112 (ite e62 e50 e74)))
(let ((e113 (ite e30 e49 e96)))
(let ((e114 (ite e67 e14 e40)))
(let ((e115 (ite e61 e92 e91)))
(let ((e116 (ite e65 e18 e89)))
(let ((e117 (ite e63 e34 e51)))
(let ((e118 (ite e64 e56 e47)))
(let ((e119 (ite e68 e102 e18)))
(let ((e120 (ite e61 e43 e53)))
(let ((e121 (ite e31 e41 e98)))
(let ((e122 (ite e67 e114 e103)))
(let ((e123 (ite e65 v4 e92)))
(let ((e124 (ite e68 e33 e88)))
(let ((e125 (ite e68 e37 e101)))
(let ((e126 (ite e31 e36 e26)))
(let ((e127 (ite e65 e21 e95)))
(let ((e128 (ite e62 e42 v4)))
(let ((e129 (ite e66 e100 e19)))
(let ((e130 (ite e66 e78 e12)))
(let ((e131 (ite e62 e34 e103)))
(let ((e132 (ite e64 e59 e14)))
(let ((e133 (ite e31 e53 e89)))
(let ((e134 (ite e66 e57 e60)))
(let ((e135 (ite e31 e7 e10)))
(let ((e136 (ite e67 e10 e9)))
(let ((e137 (ite e65 e136 v0)))
(let ((e138 (ite e68 e6 e7)))
(let ((e139 (ite e66 e9 e9)))
(let ((e140 (ite e62 e8 e135)))
(let ((e141 (ite e30 e137 e6)))
(let ((e142 (ite e30 e140 e138)))
(let ((e143 (ite e29 e140 e6)))
(let ((e144 (ite e67 e141 v0)))
(let ((e145 (ite e62 e6 e137)))
(let ((e146 (ite e29 e137 e139)))
(let ((e147 (ite e30 e140 e144)))
(let ((e148 (ite e66 e142 e141)))
(let ((e149 (ite e63 e8 e144)))
(let ((e150 (ite e31 e135 e140)))
(let ((e151 (ite e64 e147 e141)))
(let ((e152 (ite e61 e147 e148)))
(let ((e153 (or e30 e63)))
(let ((e154 (or e67 e62)))
(let ((e155 (ite e154 e29 e154)))
(let ((e156 (and e66 e155)))
(let ((e157 (=> e31 e64)))
(let ((e158 (or e65 e153)))
(let ((e159 (not e158)))
(let ((e160 (xor e157 e68)))
(let ((e161 (xor e159 e61)))
(let ((e162 (= e156 e160)))
(let ((e163 (or e161 e161)))
(let ((e164 (not e162)))
(let ((e165 (=> e164 e163)))
e165
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

(check-sat)
