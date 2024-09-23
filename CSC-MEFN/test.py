import torch
import datetime
import os
from glob import glob
import numpy as np
from skimage.io import imsave, imread
from networks import CSC_Fusion_MEF
from utils import FolderDataset, loss_fn, halo_fn, test
from torch.utils.data import DataLoader 

if __name__ == "__main__":
    act = 'sst'
    num_blocks = 4

    # testset loaders
    testset4          = FolderDataset('test',None)
    testloader4       = DataLoader(testset4,       batch_size=1)
    
    loader = {'HDRPS': testloader4}
    
    save_path = r'result'
    net = CSC_Fusion_MEF(act=act,num_blocks=num_blocks).cuda()
    net.load_state_dict(torch.load('./model/best_net.pth'))
    test(net, loader['HDRPS'],  save_path)