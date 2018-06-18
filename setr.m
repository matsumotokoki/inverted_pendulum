syms s a1 a2 b1 b2 c1 c2 f1 f0

A=[0 1 0 0;0 b1*f1 a1 0;0 0 0 1;0 b2*f1 a2 0];
B=[0;b1*f0;0;b2*f0];
C=[0 0 c1 0;c2 0 0 0];
X=expand(det(s*eye(4,4)-A));				%Uq“`’BŠÖ”•ª•ê
Y=expand(sum(C*adjoint(s*eye(4,4)-A)*B));	%Uq“`’BŠÖ”•ªq

a1=-0.349687178;
a2= 52.71651927;
b1= 1.37504297;
b2=-7.141843681;
f0= 4.651162791;
f1=-25.50406435;
c1=0.004054;
c2=0.000507;

A=subs(A);									%’l‚ÌXV
B=subs(B);
C=subs(C);
X=subs(X);
Y=subs(Y);

h=tf(sym2poly(Y),sym2poly(X))				%ª‹OÕŒvZ
rlocus(h)
