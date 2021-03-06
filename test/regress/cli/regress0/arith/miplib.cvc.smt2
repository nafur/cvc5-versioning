; COMMAND-LINE: --miplib-trick
; EXPECT: sat
(set-logic ALL)
(set-option :incremental false)
(declare-fun tmp1 () Int)
(declare-fun tmp2 () Int)
(declare-fun tmp3 () Int)
(declare-fun x () Bool)
(declare-fun y () Bool)
(declare-fun z () Bool)
(assert (=> (and (not x) (and (not y) true)) (= tmp1 0)))
(assert (=> (and x (and (not y) true)) (= tmp1 4)))
(assert (=> (and (not x) (and y true)) (= tmp1 6)))
(assert (=> (and x (and y true)) (= tmp1 10)))
(assert (=> (and (not x) (and (not z) true)) (= tmp2 0)))
(assert (=> (and x (and (not z) true)) (= tmp2 2)))
(assert (=> (and (not x) (and z true)) (= tmp2 9)))
(assert (=> (and x (and z true)) (= tmp2 11)))
(assert (=> (and (not y) (and (not z) true)) (= tmp3 0)))
(assert (=> (and y (and (not z) true)) (= tmp3 5)))
(assert (=> (and (not y) (and z true)) (= tmp3 16)))
(assert (=> (and y (and z true)) (= tmp3 21)))
(check-sat)
