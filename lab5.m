 for x = 1:100; 
     a = sprintf('image%d.jpg',x);
     img = imread('http://eec181.ece.ucdavis.edu:8081/photo.jpg');
     imwrite(img,a,'jpg'); 
     pause(2);
 end