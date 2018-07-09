clear variables
close all

c = ['b' 'r'] ;

for k = 1:2
    folder = 'Medium_60_AGVs' ;
    if (k == 1)
        domainDir = folder ;
    elseif (k ==2)
        domainDir = [folder '_baseline'] ;
    end

    verticesFile = [domainDir '/vertices_XY.csv'] ;
    edgesFile = [domainDir '/edges.csv'] ;
    capsFile = [domainDir '/capacities.csv'] ;
    distsFile = [domainDir '/distances.csv'] ;
    origsFile = [domainDir '/origins.csv'] ;

    results = [domainDir '/Results'] ;

    % Plot evaluation results and statistics
    epochs = 200 ;
    runs = 0:9 ;
    G = zeros(epochs,length(runs)) ;
    tMove = zeros(epochs,length(runs)) ;
    tEnter = zeros(epochs,length(runs)) ;
    tWait = zeros(epochs,length(runs)) ;
    
    for i = runs
        data = csvread(sprintf('%s/evaluation_%d.csv',results,i)) ;
        G(:,i+1) = data(:,2) ;
        tMove(:,i+1) = data(:,3) ;
        tEnter(:,i+1) = data(:,4) ;
        tWait(:,i+1) = data(:,5) ;
    end

    figure(1)
    hold on
    [meanG,sigG,meanGci,sigGi] = normfit(G') ;
%     paMeanGci = patch([1:epochs,epochs:-1:1],[meanGci(1,:),fliplr(meanGci(2,:))],c(k),...
%         'facealpha',0.2, 'linestyle','none') ;
    paMeanGci = patch([1:epochs,epochs:-1:1],[meanG+sigG,fliplr(meanG-sigG)],c(k),...
        'facealpha',0.2, 'linestyle','none') ;
    plMeanG = plot(1:epochs,meanG,c(k)) ;
    title('Learning Performance')
    xlabel('Learning epoch')
    ylabel('Total number of deliveries')
    set(1,'position',[350 700 560 420])

    figure(2)
    hold on
    [meanMove,sigMove,meanMoveci,sigMovei] = normfit(tMove') ;
%     paMeanMoveci = patch([1:epochs,epochs:-1:1],[meanMoveci(1,:),fliplr(meanMoveci(2,:))],c(k),...
%         'facealpha',0.2, 'linestyle','none') ;
    paMeanMoveci = patch([1:epochs,epochs:-1:1],[meanMove+sigMove,fliplr(meanMove-sigMove)],c(k),...
        'facealpha',0.2, 'linestyle','none') ;
    plMeanMove = plot(1:epochs,meanMove,c(k)) ;
    title('Total AGV Time Spent Moving')
    xlabel('Learning epoch')
    ylabel('AGV motion times')
    set(2,'position',[1000 700 560 420])

    figure(3)
    hold on
    [meanEnter,sigEnter,meanEnterci,sigEnterci] = normfit(tEnter') ;
%     paMeanEnterci = patch([1:epochs,epochs:-1:1],[meanEnterci(1,:),fliplr(meanEnterci(2,:))],c(k),...
%         'facealpha',0.2, 'linestyle','none') ;
    paMeanEnterci = patch([1:epochs,epochs:-1:1],[meanEnter+sigEnter,fliplr(meanEnter-sigEnter)],c(k),...
        'facealpha',0.2, 'linestyle','none') ;
    plMeanEnter = plot(1:epochs,meanEnter,c(k)) ;
    title('Total AGV Time Spent Waiting to Enter')
    xlabel('Learning epoch')
    ylabel('AGV wait to enter times')
    set(3,'position',[350 100 560 420])

    figure(4)
    hold on
    [meanWait,sigWait,meanWaitci,sigWaitci] = normfit(tWait') ;
%     paMeanWaitci = patch([1:epochs,epochs:-1:1],[meanWaitci(1,:),fliplr(meanWaitci(2,:))],c(k),...
%         'facealpha',0.2, 'linestyle','none') ;
    paMeanWaitci = patch([1:epochs,epochs:-1:1],[meanWait+sigWait,fliplr(meanWait-sigWait)],c(k),...
        'facealpha',0.2, 'linestyle','none') ;
    plMeanWait = plot(1:epochs,meanWait,c(k)) ;
    title('Total AGV Time Spent Waiting at Intersections')
    xlabel('Learning epoch')
    ylabel('AGV wait to cross times')
    set(4,'position',[1000 100 560 420])
end

