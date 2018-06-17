%	X:�U�q�`�B�֐�����		Y:�U�q�`�B�֐����q		P:��(���x�N�g��)
%	Z:�⏞��`�B�֐�����	Z:�⏞��`�B�֐����q	h:�ꏄ�`�B�֐�(YW/XZ)��tf

function [Z,W,h]= solc(X,Y,P)
	syms s

	M=1;								% �ɂ����Ƃ���s�̑������ւ̓W�J
	for i=1:length(P)
		M=M*(s-P(i));
	end
	
	pX=vpa(sym2poly(X));				% ���ꂼ��poly(s�̌W���x�N�g��(�~��))�ɕϊ�(���x��vpa�ɐݒ�)
	pY=vpa(sym2poly(Y));
	pM=vpa(sym2poly(M));
	
	lx=length(pX)-1;					% (lx��X�̍ő原���Ɉ�v)
	ly=length(pY)-1;					% (ly��Y�̍ő原���Ɉ�v)
	
	vX=[pX].';							% ���ꂼ���poly���c�ɂ���
	vY=[pY].';
	A=[];								% A���s��ł��邱�Ƃ�錾(�ǂ����K�{)
	
	for i=1:lx-1						% �T�C�Y�����킹�ď㑤��0���l�߂�
		vX=[0;vX];
	end
	for i=1:2*lx-ly-1
		vY=[0;vY];
	end

	for i=lx-1:-1:0						% ��ɃV�t�g�������̂��l�߂Ă���(�V�t�g�������������Ă���)
		A=[A circshift(vX,-i)];
	end
	for i=lx-1:-1:0
		A=[A circshift(vY,-i)];
	end
	
	a=inv(A)*pM.';						% Ax=pM�̉�
	pZ=[a(1) a(2) a(3) a(4)];			% ������([Z W]=a�̓T�C�Y��񂵂Ă��ł��Ȃ�����)
	pW=[a(5) a(6) a(7) a(8)];
	Z=poly2sym(pZ,s);					% poly����s�̑������𐶐�
	W=poly2sym(pW,s);
	newpX=sym2poly(expand(X*Z));		% X*Z(�ꏄ�`�B�֐��̕���)�𐶐�
	newpY=sym2poly(expand(Y*W));		% Y*W(�ꏄ�`�B�֐��̕��q)�𐶐�
	h=tf(newpY,newpX);					% �ꏄ�`�B�֐���tf�𐶐�
	rlocus(h)							% �O�Ղ�`���ďI��
end
