for a = 1:100
    sum =0;
    %a = 88;
    [d1 d2 d3] = part3(a);
    digit1 = reshape(d1, 784,1);
    number = testdigit(digit1);
    sum = sum * 10 + number;
    digit2 = reshape(d2, 784,1);
    number = testdigit(digit2);
    sum = sum * 10 + number;
    digit3 = reshape(d3, 784,1);
    number = testdigit(digit3);
    sum = sum * 10 + number
    
    pause(2);
    allPlots = findall(0, 'Type', 'figure', 'FileName', []);
    delete(allPlots);
end