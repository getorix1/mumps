	s t=$zd1
	f i=1:1:10000000 s ^a(i)=i
	f i=1:1:10000000 s j=^a(i)
	write $zd1-t,!
