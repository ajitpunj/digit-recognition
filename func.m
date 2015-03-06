function [row] = func(x)
load('testSet.mat');
load('NN.mat');

data = testData(:,x);
y = reshape(data, 28, 28);
imshow(y);
z1 = 1./(1 + exp((finalW1L1 * testData(:,x) + finalB1L1)*(-1)));
%z1 = 1./(1 + exp((finalW1L1 * data1(:,x) + finalB1L1)*(-1)));
z2 = 1./ (1 + (exp((finalW1L2 * z1 + finalB1L2)*(-1))));
z3 = finalSoftmaxTheta * z2;
[col, row] = max(z3);

%if( col == 10)
%    value = 0
%else 
%    value = col
%end
end