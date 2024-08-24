	r "n ",n
	f i=1:1:n w "p",i," " r p(i)
	f i=0:1:n w "q",i," " r q(i)
	f i=0:1:n f j=0:1:n s r(i,j)=0
	f i=0:1:n s c(i,i)=0,w(i,i)=q(i) f j=i+1:1:n s:j'>n w(i,j)=w(i,j-1)+p(j)+q(j)
	f j=1:1:n s c(j-1,j)=w(j-1,j),r(j-1,j)=j
	f d=2:1:n f j=d:1:n s i=j-d,y=r(i,j-1),x=c(i,y-1)+c(y,j) d xx s c(i,j)=w(i,j)+x,r(i,j)=y
	w !,"matrix",!
	f m=0:1:n-1 w ! f l=1:1:n w r(m,l)," "
	w !! s s=1,s(s)=0_","_n,c=1,nx=2,a(1)="b(0"
	w 1
y	i $p(s(c),",",1)-$p(s(c),",",2)=0 s c=c+1 g:c<nx y g z
	w 2," ",a(c)," "
	s s(nx)=$p(s(c),",",1)_","_(r(@s(c))-1),a(nx)=a(c)_",1"
	w 3
	s nx=nx+1,s(nx)=r(@s(c))_","_$p(s(c),",",2),a(nx)=a(c)_",2",nx=nx+1
	w 4
	s c=c+1 g y
z	f i=1:1:c-1 s a(i)=a(i)_")"
	f i=1:1:c-1 s @a(i)=r(@s(i))
	f i=1:1:c-1 w !,a(i),"->",@a(i)
	h
xx	f k=r(i,j-1):1:r(i+1,j) i c(i,k-1)+c(k,j)<x s x=c(i,k-1)+c(k,j),y=k
	q
