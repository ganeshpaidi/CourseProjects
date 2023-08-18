rng(0);
for k=1:2   %Just a loop to read both Set-1, set-2
    
    S = load(sprintf("points2D_Set%d.mat",k));

    X = [(S.x)' ; (S.y)'];
    N = length(S.x);

    %empirical/sample mean.
    mu = sum(X,2)/N;
    SS = X - mu;  %standardised//mean = 0.

    %Sample covariance/empirical Cov.
    Cov = (SS*SS')/N;

    [V, D]= eig(Cov);

    for i=1:2       %orthonormalising the eig-vectors
       V(:,i) = V(:,i)/fro_norm(V(:,i)); 
    end

    [d,ind] = sort(diag(D),'descend');  %Picking the highest eigen value
    V = V(:,ind(1,1));   %Highest Eigen vector.

    %%________________Plotting the figure____________________

    fig = figure;
    set(gcf, 'Position',  [500, 300, 600, 500])

    scatter(X(1,:),X(2,:),25,'filled');
    hold on;
    grid on;
    
    title(sprintf("Scatter plot of PointSet-%d",k));

    %axis equal;
    if( k == 1)
        xlim([-0.2,1.2])
        ylim([2,5]);
    else
        xlim([-2,2]);
        ylim([-0.5,2]);
    end
    xlabel('\bf X');
    ylabel('\bf Y','Rotation',0);

    %changing axes width & font sizes.
    ax = gca;
    ax.FontSize = 12;
    ax.LineWidth = 1.5;


    %Principal mode of variation(eigen vectors).
    point1 = mu + sqrt(d(1,1))*V;
    point2 = mu - sqrt(d(1,1))*V;

    points = [mu' ;point1'];
    line(points(:,1)',points(:,2)','color','r','LineWidth',2);
    hold on;

    points = [mu' ;point2'];
    line(points(:,1)',points(:,2)','color','r','LineWidth',2);
    hold on;
    
    plot(mu(1,1),mu(2,1),'*','LineWidth',2,'Color','g');  %Marking mean
    hold on;

    x = linspace(-2,2,4);
    line(mu(1,1)+x*V(1,1),mu(2,1)+x*V(2,1),'Color','red','LineStyle','--','LineWidth',1)
    if(k == 1)
            legend('\bf(x,y)','Standard deviation','','Mean','Linear Relation Y vs X','Location','northwest');
    else
            legend('\bf(x,y)','Standard deviation','','Mean','Linear Relation Y vs X');
    end
    saveas(fig,sprintf("set%d.png",k));
    
    display("pointset:"+k+"  line passes through ("+mu(1,1)+","+mu(2,1)+")");
    display("slope vector =  ("+ V(1,1)+ "," + V(2,1) + ")");
    display("eigen values in set " + k + " are" + d(1,1)+ "," + d(2,1));
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Defining a function for the fro norm.
function n = fro_norm(X)
    n = 0;
    s = size(X);
    for i=1:s(1,1)
        for j=1:s(1,2)
            n = n + X(i,j)^2; 
        end
    end
    n = sqrt(n);
end


