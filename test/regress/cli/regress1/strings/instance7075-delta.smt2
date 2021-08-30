(set-logic QF_S)
(set-info :status sat)
(declare-const X String)
(assert (not (str.in_re X (re.++ (re.range "0" "9") ((_ re.loop 1 2) (re.range "0" "9"))))))
(assert (str.in_re X (re.union (re.++ (re.range "0" "9") ((_ re.loop 1 6) (re.range "0" "9"))) (str.to_re ""))))
(check-sat)
