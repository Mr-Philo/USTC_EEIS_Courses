% test1.m

% 导入全体数据
load('E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\data\mnist-baseline-simplenn\imdb.mat');
% 挑选出测试集
test_index = find(images.set==3);
% 挑选出样本以及真实类别
test_data = images.data(:,:,:,test_index);
test_label = images.labels(test_index);
%导入模型文件
load('E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\data\mnist-baseline-simplenn\net-epoch-15.mat');
% 将最后一层改为 softmax （原始为softmaxloss，这是训练用）
net.layers{1, end}.type = 'softmax';
% net = vl_simplenn_tidy(net) ;
for i = 1:length(test_label)
    i
    im_ = test_data(:,:,:,i);
    im_ = im_ - images.data_mean;
    res = vl_simplenn(net, im_) ;
    scores = squeeze(gather(res(end).x)) ;
    [bestScore, best] = max(scores) ;
    pre(i) = best;
end
% 计算准确率
accurcy = length(find(pre==test_label))/length(test_label);
disp(['accurcy = ',num2str(accurcy*100),'%']);