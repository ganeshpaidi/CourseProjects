X = load('mnist.mat');

dig_test = cast(X.digits_test,'double');
dig_train = cast(X.digits_train,'double');
labl_test = X.labels_test;
labl_train = X.labels_train;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%index 1 means digit '0'
dig_mean = zeros(28*28,10);   %28*28 x 10 matrix for means X(:,5)--mean of 4
dig_cov = zeros(28*28,28*28,10);

for i=1:10
   dig_count = find(labl_train == i-1 );
   count = length(dig_count);
   
   dig_data = dig_train(:,:,dig_count);
   dig_data = reshape(dig_data,28*28,[]); %converting 28,28,N --> 28*28,N
   
   dig_mean(:,i) = sum(dig_data,2)/count;
   S = dig_data - dig_mean(:,i); %Standardised matrix.
   
   %finding covariance matrix.
   dig_cov(:,:,i) = (S*S')/count;
   
   %eigen vectors and values.
   [V, D] = eig(dig_cov(:,:,i));

   %picking the first of all digits for testing.
   X = dig_train(:,:,dig_count(1,1));
   X = reshape(X,28*28,1);
   
   
    %reducing and regenerating the image[ 84 dimensions.].
    C = reduce(X,dig_mean(:,i),V,D,84);
    X_new = regen(C,dig_mean(:,i),V,D,84);
    
    %net-image: side-by-side 2 images.
    img = [reshape(X_new,28,28) reshape(X,28,28)];
    
    figure;
    
    %the imshow image will be overrriden by imagesc image, but I still kept
    %it to reduce the size of the imagesc image.
    imshow(img);
    imagesc(img);
    %imwrite(img,sprintf("Images_%d.png",i-1));
    saveas(gcf,sprintf("Images_%d",i-1),'png')

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%function to reduce the dimension from 784-84 provided the Eigen vectors
%and values of the Covariance matrix.

%FUNCTION TAKES THE 784dim Vector X to be transformed/reducedin dimension, The mean 
%of the sample data mu, The eigen values E of the sample data and it's eigen vectors, and M: the
%number of dimensions to which we should reduce the data.

function C = reduce(X,mu,E,D,M)
    C = zeros(M,1);
    [d,ind] = sort(diag(D),'descend');
    eigvecs = zeros(28*28,M);    
    eigvals = zeros(1,M); %M primary eig values.
    
    %finding the M primary eigen UNIT vectors and eig-values.
    for i=1:M
        eigvals(1,i) = d(ind(i));
        eigvecs(:,i) = E(:,ind(i))/norm(E(:,ind(i)),2);
    end
    
    X = X - mu; %First moving into a cooridinate frame of 0 mean.Shifting the data my mean.
    
    %projecting X to C into the new lower  M-dim basis
    for i = 1:M
        C(i,1) = (X')*eigvecs(:,i); %dot product.
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%regenrating function from 84 to 784 dimension.

%_____Similiar arguments as above, but now we transform 'M' dimensional C into
%original 784 dimension vector X.
function X = regen(C,mu,E,D,M)
    X = zeros(28*28,1);
    [d,ind] = sort(diag(D),'descend');
    eigvecs = zeros(28*28,M);
    eigvals = zeros(1,M);
    
    %finding the top-M eigenvectors for the basis of C.
    for i=1:M
        eigvals(1,i) = d(ind(i));
        eigvecs(:,i) = E(:,ind(i));
    end
    
    %reconstructing X from C.
    for i = 1:M
        X = X + C(i,1)*eigvecs(:,i);
    end
    
    %adding the mean to the 0-mean data finally.
    X = X + mu;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

