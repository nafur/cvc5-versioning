(set-logic HO_ALL)
(set-info :status sat)
(declare-const b (-> Int Int Int))
(declare-const c (-> Int Int))
(assert (and (= c (b 1)) (= c (b 0)) (> (b 1 0) 0) (> 0 (b 0 1)) (= 0 (c (b 0 0)))))
(check-sat)
