[d1 d2 d3] = part3('image3.jpg');
load('testSet.mat');
load('NN.mat');
digit1 = reshape(d1, 784,1);

z1=zeros(200,1);
for i=1:784
    if digit1(i,1)==1
        for j=1:200
            z1(j,1)=z1(j,1)+finalW1L1(j,i);
        end
    end
end
for i=1:200
    z1(i,1)=z1(i,1)+finalB1L1(i,1);
    if z1(i,1) <0
        z1(i,1)=0;
    else
        z1(i,1)=1;
    end
end

z2=zeros(200,1);
for i=1:200
    if z1(i,1)==1
        for j=1:200
            z2(j,1)=z2(j,1)+finalW1L2(j,i);
        end
    end
end
for i=1:200
    z2(i,1)=z2(i,1)+finalB1L2(i,1);
    if z2(i,1) <0
        z2(i,1)=0;
    else
        z2(i,1)=1;
    end
end
z3=zeros(10,1);
for i=1:200
    if z2(i,1)==1
        for j=1:10
            z3(j,1)=z3(j,1)+finalSoftmaxTheta(j,i);
        end
    end
end

%number = testdigit(digit1);
