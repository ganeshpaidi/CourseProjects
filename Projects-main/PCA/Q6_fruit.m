rng(7);

%matrix to store data of 16 fruits.
fruit_data = zeros(6400*3,16);

for i=1:16  %loop to read n store 16 fruits.
    im = imread(sprintf("image_%d.png",i));
    im = cast(im,'double');
    fruit_data(:,i) = reshape(im,[],1);
end

%mean and covariance.
mean = sum(fruit_data,2)/16;
mean_img = reshape(mean,80,80,3);
figure;
image(mean_img/255);   %image of mean.
title("Image of mean");
imwrite(mean_img/255,'fruit_mean.png');


S = fruit_data - mean; %standardised matrix: mean=0;
Cov = S*S'/16;   %covariance matrix;

%top-4 eigen vectors
[eig_vecs,~] = eigs(Cov,4);

D = eigs(Cov,10); %top -10 eigen values.



%image of the 4 eigen vectors
fig = figure;

for i=1:4
    subplot(2,2,i)
    vec = eig_vecs(:,i);
    vec = vec/255; 
    vec = reshape(vec,80,80,3);
    image(vec);
end
sgtitle('Images of the Top Eigen vectors');
saveas(fig,'Eigvectors_img.png');

%plotting  eigen values
fig = figure;
plot(1:10,D,'-o','MarkerSize',5,'MarkerFaceColor','b');
grid on;
title('\bf Top 10 eigen values of Fruit-Data');

ylabel('\bf Eigen values');
xlabel('\bf eig-value number');
xlim([1,inf]);

%changing axes width & font sizes.
ax = gca;
ax.FontSize = 12;
ax.LineWidth = 1.5;

saveas(fig,'Eigen_Values.png');

%Plotting the closest representations of 16 fruits.
closest = zeros(6400*3,16);

%making the eigen vectors as unit vectors
for i=1:4
    eig_vecs(:,i) = eig_vecs(:,i)/norm(eig_vecs(:,i),2);
end

for i=1:16
   orig = reshape(fruit_data(:,i),80,80,3)/255;
   close = mean;
   
   %finding coefficients of 4 eigen vectors = dot product of the
   %eigen-vectors with the image vecto.
   for j=1:4
      close = close + (fruit_data(:,i)'*eig_vecs(:,j))*eig_vecs(:,j);
   end
   
   close = close/255;
   close = reshape(close,80,80,3);
   figure;
   title("comparing images for image number:  "+i);
   axis equal;
   image([close orig]);  %closest and original images side-by-side.
   title("Comparison for fruit: " +num2str(i));
   name = sprintf("fruit_comparison_%d.png",i);
   imwrite([close orig],name);
   
end



%Generating 3 sample images

for r=1:3
    %generating the 4 coefficients assuming GAUSSIAN
   coeffs = zeros(4,1);
   for c = 1:4
      coeffs(c,1) = randn()*sqrt(D(c));
   end
   
   %generating image from coefficeints.
   sample = mean;
   sample = sample + eig_vecs*coeffs;
   
   sample_img = reshape(sample,80,80,3)/255;
   
   figure;
   title("generated sample fruit: "+num2str(r));
   image(sample_img);
   name = sprintf('generated_fruit_%d.png',r);
   imwrite(sample_img,name);
    
end
