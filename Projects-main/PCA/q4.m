X = load('mnist.mat');

dig_test = cast(X.digits_test,'double');
dig_train = cast(X.digits_train,'double');
labl_test = X.labels_test;
labl_train = X.labels_train;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%index 1 means digit '0'
dig_mean = zeros(28*28,10);   %28*28 x 10 matrix for means of 10 digits.
dig_cov = zeros(28*28,28*28,10);  %28*28 x 10 mtrix of Covariance of 10 digits.

dig_eig = zeros(1,10); %Principal eigen value of each digit.
dig_eigvec = zeros(28*28,10); %corresponding eigen-vectors.


for i=1:10   %looping on all digits 0-9
    
   dig_count = find(labl_train == i-1 );
   count = length(dig_count);
   
   dig_data = dig_train(:,:,dig_count);
   dig_data = reshape(dig_data,28*28,[]); %converting 28,28,N --> 28*28,N
   
   dig_mean(:,i) = sum(dig_data,2)/count;
   S = dig_data - dig_mean(:,i); %Standardised data matrix: mean = 0

   dig_cov(:,:,i) = (S*S')/count; %Covariance matrix of the raw data.
   
  [V, D] = eig(dig_cov(:,:,i));
  
   [d,ind] = sort(diag(D),'descend'); %sorted eigen values, their indices
   
   %principal mode of variation lambda_1,v_1.
   dig_eig(i) = d(1,1);
   dig_eigvec(:,i) = V(:,ind(1,1));
   
   %plotting all the eig-values for each digit.
   fig = figure;
   n = 1:784;
   plot(n,d,'-o','MarkerSize',5,'MarkerFaceColor','b');
   hold on;
   title(sprintf("Decreasing list of eigen values of digit: %d",i-1));
   
    xlabel('\bf eigenvalue number');
    ylabel('\bf eigen-values');
    ylim([0,inf]);

    %changing axes width & font sizes.
    ax = gca;
    ax.FontSize = 12;
    ax.LineWidth = 1.5;
    
    plot(0:500:1000,[2000 2000 2000],'Color','red','LineStyle','--','LineWidth',1);
    hold on;
    
    legend('eigenvals','y = 2000');
    saveas(fig,sprintf("eigenvals%d.png",i-1));
    
    useful = find(d >= d(1)/100 );
    display(sprintf("The digit %d has %d useful/large eigen values",i-1,length(useful)));
   
   %mean - lambda*v
   img_1 = reshape(dig_mean(:,i) - sqrt(dig_eig(i))*dig_eigvec(:,i),28,28);
   
   %mean
   img_mu = reshape(dig_mean(:,i),28,28);
   
   %mean + lambda*v
   img_2 = reshape(dig_mean(:,i) + sqrt(dig_eig(i))*dig_eigvec(:,i),28,28);
   
    
   %combined matrix for showing all 3 matrices side-by-side in same figure
   figure;
   
   %here imshow is overrriden ny imagesc, but i kept it to reduce the size
   %of imagesc() image
   imshow([img_1 img_mu img_2]);
   imagesc([img_1 img_mu img_2]); %imagesc shows better visuals than imshow.
   %imwrite([img_1 img_mu img_2],sprintf("Images%d.png",i-1));
   saveas(gcf,sprintf("Images%d",i-1),'png')
end


%saving the asked data.
save('means','dig_mean');
save('Covariances','dig_cov');
save('principle_eigvals','dig_eig');
save('principle_eigvecs','dig_eigvec');
