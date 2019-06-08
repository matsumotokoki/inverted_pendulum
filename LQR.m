m=0.0346;      %振子の重さ
M=0.7186;      %全体の重さ
L=0.1444;      %振子の長さ
rw=0.0258;     %タイヤ半径
u=0.018;       %最大トルク
Omax=314.1593; %最大角速度
G=6.6666;      %ギア比
Ig=(m*L*L)/3;  %慣性モーメント
d=0.09;        %重心までのの長さ
Id=Ig+m*d*d;   %慣性モーメント（回転軸）
g=9.81;        %重力
x0=[0.3 0 0 0]; %初期値
f0=u*G/rw; %u*G/rw
f1=-(u*G^3)/(Omax*rw^2);%-(u*G^3)/(Omax*rw^2)
p=Ig*(M+m)+M*m*d^2;

A=[0       1                0          0;
   0  (Ig+m*d^2)*f1/p  -(m^2*g*d^2)/p  0;
   0       0                0          1;
   0  -(m*d)*f1/p       m*g*d*(m+M)/p  0];

B=[     0;
  (Ig+m*d^2)*f0/p;
        0;
  -m*d*f0/p];

C=[1 0 0 0;
   0 0 1 0];

D=[0;
   0];

Q=[65  0   0    0;
   0   40  0    0;
   0   0   100  0;
   0   0   0    40]; %各ゲインに対しての重み行列

R=15;
[K,S,e]=lqr(A ,B, Q, R);

disp('feedback gain =');
disp(K);

X=['float F[4]={ ',num2str(K(1)),', ',num2str(K(2)),', ',num2str(K(3)),', ',num2str(K(4)),'};'];
disp(X);

fileID=fopen('log.txt','a');
fprintf(fileID,"Q=[%.2f %.2f %.2f %.2f], R=%.2f, K=[%.2f %.2f %.2f %.2f]\r\n",diag(Q),R,K);
fclose(fileID);
A=A-B*K;
state=ss(A,B,C,D);
initial(state,x0);
