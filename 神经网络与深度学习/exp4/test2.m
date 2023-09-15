% test2.m

clear all
% Setup MatConvNet.
addpath 'E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\matlab';       %  添加matconvnet工具箱路径
vl_compilenn ;
vl_setupnn ;
% 加载数据集.
load('E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\data\mnist-baseline-simplenn\imdb.mat');
% 加载预训练模型（就是我们刚刚训练的结果）
load('E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\data\mnist-baseline-simplenn\net-epoch-20.mat') ;
% 将最后一层改为 softmax （原始为softmaxloss，这是训练用）
net.layers{1, end}.type = 'softmax';
% 设置默认参数
% net = vl_simplenn_tidy(net) ;
% 获取指定的图像，0-9手写数字的图片文件中已经给出，也可自行在MNIST数据集中获取
im = imread('E:\matlab\matlab2020b\toolbox\matconvnet-1.0-beta25\data\mnist-baseline-simplenn\sample3.bmp');
% 灰度化
im = rgb2gray(im);
% 反转图像，训练图像为黑底白字，测试图像为白底黑字，所以需要反转一下
% im = 255-im;
im_ = single(im) ; % note: 255 range
% 输入默认大小为28*28
im_ = imresize(im_, [28 28]) ;
% 减去均值
im_ = im_ - images.data_mean;
% Run the CNN.
%返回一个res结构的输出网络层
res = vl_simplenn(net, im_);%vl_simplenn函数返回的是一个多维的数组，最后一个维度保存的是分别识别为0~9的概率值，最大概率值就是最后的结果。
%展示分类结果
scores = squeeze(gather(res(end).x)) ;%得到图像属于各个分类的概率
[bestScore, best] = max(scores); %得到最大概率值，及其索引值
figure(1) ; clf ; 
%imagesc(im) ;
imshow(im,[]);%显示图像
title(sprintf('the identified result: %d, score: %.3f',...
    best-1, bestScore)) ;%添加标题——第几,所属此的概率