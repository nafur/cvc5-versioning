; EXIT: 1
; EXPECT: (error "Extended set operators are not supported in default mode, try --sets-ext.")
(set-logic ALL)
(set-option :incremental true)
(set-option :produce-models true)
(declare-fun x () (Set Bool))
(declare-fun y () (Set Bool))
(declare-fun z () (Set Bool))
(assert (= x (singleton true)))
(assert (= y (singleton false)))
(push 1)

(assert (= z (complement y)))

(check-sat)

(pop 1)

(get-model)