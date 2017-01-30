% Driver to rigidly allign a set of images to a user-defined reference.
% Multi-level Parametric Image Registration (MLPIR) provided by FAIR 
% toolbox is used.
% The datasets storing the data should have been created through the
% function import_data.m.

clc
close all
clear variables
clear variables -global

% Specify reference dataset
fileR = '../matlab-data/MRI/50002.mat';

% List datasets of images to allign
fileT = strings(0);
for id = 50033:50033
    if id ~= 50018 && id ~= 50021
        file = sprintf('../matlab-data/MRI/%i.mat', id);
        fileT = [fileT; file];
    end
end

% Perform preregistrations
for i = 1:length(fileT)
    preregister(char(fileT(i)), fileR);
end