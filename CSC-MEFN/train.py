# -*- coding: utf-8 -*-
"""
Created on Tue Mar 31 19:18:05 2020

@author: Anonymous
"""
import torch
import datetime
import os

from networks import CSC_Fusion_MEF
from utils import FolderDataset, loss_fn, halo_fn, test
from torch.utils.data import DataLoader 
from tensorboardX import SummaryWriter

# Hyper-parameters
batch_size = 8
lr = 5e-4
num_epoch = 50
num_blocks = 8
loss_weight = 0. # the initial loss weight
loss_weight_max = 10.

# Network
net = CSC_Fusion_MEF(act='sst',num_blocks=num_blocks).cuda()
optimizer = torch.optim.Adam(net.parameters(), lr=lr)


# Loggers
timestamp = datetime.datetime.now().strftime("%m-%d-%H-%M")
save_path = os.path.join('MFF_logs',timestamp+'_bs%d_epoch%d_lr%f_lw%f'%(batch_size,num_epoch,lr,loss_weight_max))
writer = SummaryWriter(save_path)


# test
best_net = CSC_Fusion_MEF(act='sst',num_blocks=num_blocks).cuda()
best_net.load_state_dict(torch.load(os.path.join(save_path,'best_net.pth')))

testset1          = FolderDataset(r'MEF_data\test',None)
testloader1       = DataLoader(testset1,       batch_size=1)
loader = {'HDRPS': testloader1}

test(net, loader['HDRPS'], save_path, 'HDRPS')