%	X:�U�q�`�B�֐�����		Y:�U�q�`�B�֐����q		P:��(���x�N�g��)
%	Z:�⏞��`�B�֐�����	W:�⏞��`�B�֐����q	h:�ꏄ�`�B�֐�(YW/XZ)��tf
%	�ɂ�X�̍ő原����2�{�w��
%	Z�̎����̂ق���1�傫��

function [Z,W,h]= solc2(X,Y,P)
	syms s

	M=1;							% �ɂ����Ƃ���s�̑������֓W�J
	for i=1:length(P)
		M=M*(s-P(i));
	end
	
	pX=vpa(sym2poly(X));			% ���ꂼ��poly(s�̌W���x�N�g��(�~��))�ɕϊ�(���x��vpa�ɐݒ�)
	pY=vpa(sym2poly(Y));
	pM=vpa(sym2poly(M));
	
	lx=length(pX);					% �W���̐��𐔂���
	ly=length(pY);
	
	vX=[pX].';						% ���ꂼ���poly���c�ɂ���
	vY=[pY].';
	A=[];							% A���s��ł��邱�Ƃ�錾(�ǂ����K�{)
	
	for i=1:lx-1					% �T�C�Y�����킹�ď㑤��0���l�߂�
		vX=[0;vX];
	end
	for i=1:2*lx-ly-1
		vY=[0;vY];
	end

	for i=lx-1:-1:0						% ��ɃV�t�g�������̂��l�߂Ă���(�V�t�g�������������Ă���)
		A=[A circshift(vX,-i)];
	end
	for i=lx-2:-1:0
		A=[A circshift(vY,-i)];
	end
	
	a=inv(A)*pM.';						% Ax=pM�̉�
	pZ=[a(1:lx)];						% ������
	pW=a(lx+1:length(a));
	Z=poly2sym(pZ,s);					% poly����s�̑������𐶐�
	W=poly2sym(pW,s);
	newpX=sym2poly(expand(X*Z));		% X*Z(�ꏄ�`�B�֐��̕���)�𐶐�
	newpY=sym2poly(expand(Y*W));		% Y*W(�ꏄ�`�B�֐��̕��q)�𐶐�
	h=tf(newpY,newpX);					% �ꏄ�`�B�֐���tf�𐶐�
	rlocus(h)							% �O�Ղ�`���ďI��
end
