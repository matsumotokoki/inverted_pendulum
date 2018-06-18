syms s a1 a2 b1 b2 c1 c2 f1 f0

M=vpa(0.7186);						%�@�̂̏d��(�U�q����)	����
m=vpa(0.0346);						%�U�q�̏d��				����
l=vpa(0.1444);						%�U�q�̒����̔���		����
r=vpa(0.0258);						%�^�C���̔��a			����
u=vpa(7.2)/vpa(400);				%�ő�o��				���艻�d���̓d�������(�f�t�H���g7.2V)
w=vpa(7.2)*vpa(pi)*vpa(125)/vpa(9);	%�ő��]��				���艻�d���̓d�������(�f�t�H���g7.2V)
G=vpa(80)/vpa(12);					%�M�A��

A=[0 1 0 0;0 b1*f1 a1 0;0 0 0 1;0 b2*f1 a2 0];
B=[0;b1*f0;0;b2*f0];
C=[0 0 c1 0;c2 0 0 0];
X=expand(det(s*eye(4,4)-A));				%�U�q�`�B�֐�����
Y=expand(sum(C*adjoint(s*eye(4,4)-A)*B));	%�U�q�`�B�֐����q

a1=-vpa(29.4)*m/(vpa(4)*M+m);
a2= vpa(29.4)*(M+m)/(vpa(4)*M+m)/l;
b1= vpa(4)/(vpa(4)*M+m);
b2=-vpa(3)/(vpa(4)*M+m)/l;
f0= G*u/r;
f1=-G*G*G*u/w/r/r;
c1=vpa(6.6)*vpa(pi)/vpa(5115);
c2=vpa(pi)*r/G/vpa(24);

A=subs(A);									%�l�̍X�V
B=subs(B);
C=subs(C);
X=subs(X);
Y=subs(Y);

h=tf(sym2poly(Y),sym2poly(X))				%���O�Ռv�Z
rlocus(h)
