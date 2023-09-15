import torch
from torchvision import transforms , datasets
import torchvision
import torch.nn as nn
from torch.utils.data import DataLoader
import cv2 as cv
import PIL.Image as Image

Transforms = transforms.Compose([
    transforms.Resize(32),
    transforms.CenterCrop(28),
    transforms.Grayscale(),
   ])

image = Image.open(r"C:\Users\wifea\Pytorch_LeNet_MNIST\images\0\omg.png")
image = Transforms(image);
image.save("kjbj.png")



