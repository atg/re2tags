[type] T1
[property] T1::P1
[property] T1::P2
[property] T1::P3
[method] T1::m1
[method] T1::m2
[method] T1::M3
[method] T1::M4
[function] f1
[interface] I1
[property] I1::M5
[property] I1::M6
[constant] C1
[variable] V1
%%%
type T1 struct {
	P1 int
	*P2
	P3
}

func (T1) m1()
func (t *T1) m2()
func (*T1) M3()
func (t T1) M4()

func f1()

type I1 interface {
	M5()
	M6(time.Time) (string, int)
}

const C1 = math.Pi

var V1 = math.Floor(C1)
