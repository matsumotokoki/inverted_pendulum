syms s a1 a2 b1 b2 c1 c2 f1 f0

M=vpa(0.7186);						%機体の重さ(振子除く)	入力
m=vpa(0.0346);						%振子の重さ				入力
l=vpa(0.1444);						%振子の長さの半分		入力
r=vpa(0.0258);						%タイヤの半径			入力
u=vpa(7.2)/vpa(400);				%最大出力				安定化電源の電圧を入力(デフォルト7.2V)
w=vpa(7.2)*vpa(pi)*vpa(125)/vpa(9);	%最大回転数				安定化電源の電圧を入力(デフォルト7.2V)
G=vpa(80)/vpa(12);					%ギア比

A=[0 1 0 0;0 b1*f1 a1 0;0 0 0 1;0 b2*f1 a2 0];
B=[0;b1*f0;0;b2*f0];
C=[0 0 c1 0;c2 0 0 0];
X=expand(det(s*eye(4,4)-A));				%振子伝達関数分母
Y=expand(sum(C*adjoint(s*eye(4,4)-A)*B));	%振子伝達関数分子

a1=-vpa(29.4)*m/(vpa(4)*M+m);
a2= vpa(29.4)*(M+m)/(vpa(4)*M+m)/l;
b1= vpa(4)/(vpa(4)*M+m);
b2=-vpa(3)/(vpa(4)*M+m)/l;
f0= G*u/r;
f1=-G*G*G*u/w/r/r;
c1=vpa(6.6)*vpa(pi)/vpa(5115);
c2=vpa(pi)*r/G/vpa(24);

A=subs(A);									%値の更新
B=subs(B);
C=subs(C);
X=subs(X);
Y=subs(Y);

h=tf(sym2poly(Y),sym2poly(X))				%根軌跡計算
rlocus(h)
