; EXPECT: sat
(set-logic ALL)
(set-option :incremental false)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun x2 () Int)
(declare-fun x3 () Int)
(assert (<= (+ (+ (+ (* 10 x0) (* 25 x1)) (* 10 x2)) (* (- 28) x3)) 20))
(assert (<= (+ (+ (+ (* 24 x0) (* (- 9) x1)) (* (- 12) x2)) (* 15 x3)) 3))
(check-sat)
