import torch
import torchvision
import torch.utils.data as Data
import os
from torchvision import transforms , datasets
import torch.nn as nn
from torch.utils.data import DataLoader


def dataloader(path, batchsize):
    
    DOWNLOAD = False
    if not(os.path.exists(path)) or not os.listdir(path):
    # not mnist dir or mnist is empyt dir
        DOWNLOAD = True
    else:
        print("MNIST dataset already exist in '{}', skip download".format(path))



    Transforms = transforms.Compose([
        transforms.Resize(32),
        transforms.CenterCrop(28),
        transforms.Grayscale(),
        transforms.ToTensor()])

    mypath = r'C:\Users\wifea\Pytorch_LeNet_MNIST\images'
    data_test = datasets.ImageFolder(mypath, transform=Transforms)
    data_loader = DataLoader(data_test, batch_size=batchsize, shuffle=True)
    
   
    train_data = torchvision.datasets.MNIST(
        root = path,
        train = True,
        transform = torchvision.transforms.ToTensor(),
        download = DOWNLOAD
    )
    
    test_data = torchvision.datasets.MNIST(
        root = path,
        train = False,
        transform = torchvision.transforms.ToTensor(),
        download = DOWNLOAD
    )
    
    train_loader = Data.DataLoader(
        dataset = train_data,
        batch_size = batchsize,
        shuffle = True
    )
    
    test_loader = Data.DataLoader(
        dataset = test_data,
        batch_size = batchsize
    )
    
    return train_loader, data_loader

if __name__ == '__main__':
    path = '../downloads/'
    train_loader, test_loader = dataloader(path, 128)
    print('done')
