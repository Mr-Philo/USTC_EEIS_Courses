import torch
import torch.nn as nn

class LeNet(nn.Module):
    def __init__(self):
        super(LeNet, self).__init__()
        
        self.conv1 = nn.Sequential(
            # C1: (1×32×32)-->(6×28×28)
            # for MNIST, input size is (1×28×28), so add padding=2 here
            # Conv kernel: (5×5),no stride,no padding
            nn.Conv2d(in_channels=1, out_channels=6, kernel_size=5, padding=2),
            nn.ReLU(),
            # S2: (6×28×28)-->(6×14×14)
            nn.MaxPool2d(kernel_size=2),
        )
        
        self.conv2 = nn.Sequential(
            # C3: (6×14×14)-->(16×10×10)
            # Conv kernel: (5×5),no stride,no padding
            nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5),
            nn.ReLU(),
            # S4: (16×10×10)-->(16×5×5)
            nn.MaxPool2d(kernel_size=2),
        )
        
        self.fc = nn.Sequential(
            # flatten tensor from (batchsize×16×5×5) to batchsize×(16×5×5)
            nn.Flatten(),
            # C5: (16×5×5)-->120
            nn.Linear(in_features=16*5*5, out_features=120),
            nn.ReLU(),
            # F6: 120-->84
            nn.Linear(in_features=120, out_features=84),
            nn.ReLU(),
        )
        
        self.out = nn.Sequential(
            # OUTPUT: 84-->10
            nn.Linear(in_features=84, out_features=10),
            nn.Softmax(dim=1),
        )
        
    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        x = self.fc(x)
        return self.out(x)
        
if __name__ == '__main__':
    net = LeNet()
    # print(net)
    
    x = torch.randn(5, 1, 32, 32)   # batchzise × channel × height × width
    y = net(x)
    print(y)
