
load('testSet.mat');
count = 0;
num = 100;
for x = 1: num
    
value = func(x)
if(value == testLabels(x));
    count = count + 1;
end
end

percent = count / num * 100
