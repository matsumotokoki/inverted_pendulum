%	X:振子伝達関数分母	Y:振子伝達関数分子	P:極(横ベクトル)
%	Z:補償器伝達関数分母	Z:補償器伝達関数分子	h:一巡伝達関数(YW/XZ)のtf

function [Z,W,h]= solc(X,Y,P)
	syms s

	M=1;							% 極を解とするsの多項式への展開
	for i=1:length(P)
		M=M*(s-P(i));
	end
	
	pX=vpa(sym2poly(X));					% それぞれpoly(sの係数ベクトル(降順))に変換(精度をvpaに設定)
	pY=vpa(sym2poly(Y));
	pM=vpa(sym2poly(M));
	
	lx=length(pX)-1;					% (lxはXの最大次数に一致)
	ly=length(pY)-1;					% (lyはYの最大次数に一致)
	
	vX=[pX].';						% それぞれのpolyを縦にする
	vY=[pY].';
	A=[];							% Aが行列であることを宣言(どうやら必須)
	
	for i=1:lx-1						% サイズを合わせて上側に0を詰める
		vX=[0;vX];
	end
	for i=1:2*lx-ly-1
		vY=[0;vY];
	end

	for i=lx-1:-1:0						% 上にシフトしたものを詰めていく(シフト幅を小さくしていく)
		A=[A circshift(vX,-i)];
	end
	for i=lx-1:-1:0
		A=[A circshift(vY,-i)];
	end
	
	a=inv(A)*pM.';						% Ax=pMの解
	pZ=a(1:4);						% 解を代入([Z W]=aはサイズ先約してもできなかった)
	pW=a(5:8);
	Z=poly2sym(pZ,s);					% polyからsの多次式を生成
	W=poly2sym(pW,s);
	newpX=sym2poly(expand(X*Z));				% X*Z(一巡伝達関数の分母)を生成
	newpY=sym2poly(expand(Y*W));				% Y*W(一巡伝達関数の分子)を生成
	h=tf(newpY,newpX);					% 一巡伝達関数のtfを生成
	rlocus(h)						% 軌跡を描いて終了
end
