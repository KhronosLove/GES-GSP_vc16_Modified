# fusion_module.py
import torch
import os
from glob import glob
import numpy as np
from skimage.io import imsave, imread
from networks import CSC_Fusion_MEF
from utils import FolderDataset, test
from torch.utils.data import DataLoader

def fuse_images(input_folder, output_path):
    act = 'sst'
    num_blocks = 4

    testset = FolderDataset(input_folder, None)
    testloader = DataLoader(testset, batch_size=1)

    net = CSC_Fusion_MEF(act=act, num_blocks=num_blocks).cuda()
    net.load_state_dict(torch.load('C:/Users/kyy/Desktop/GES-GSP_vc16_Modified/CSC-MEFN/model/best_net.pth'))
 
    test(net, testloader, output_path)
