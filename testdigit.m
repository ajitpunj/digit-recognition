function [output] = testdigit(array)
load('testSet.mat');
load('NN.mat');
z1 = 1./(1 + exp((finalW1L1 * array + finalB1L1)*(-1)));
z2 = 1./ (1 + (exp((finalW1L2 * z1 + finalB1L2)*(-1))));
z3 = finalSoftmaxTheta * z2;
[col, row] = max(z3);
if (row == 10)
    output = 0;
else
output = row;
end
end