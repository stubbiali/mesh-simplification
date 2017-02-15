% Driver to nonrigidly allign a set of images to a user-defined template.
% Multi-level Image Registration (MLIR) provided by FAIR toolbox is used.
% The datasets storing the data should have been created through the
% function import_data.m.

clc
close all
clear variables
clear variables -global

% Specify reference dataset
fileR = '../matlab-data/MRI/template.mat';

% List template datasets and names for output NiFTI files
fileT = strings(0);
outfile = strings(0);
for id = 50029:50033
    if id ~= 50018 && id ~= 50021
        file = sprintf('../matlab-data/MRI/%i.mat', id);
        fileT = [fileT; file];
        
        file = sprintf('../result/MRI/%i_register.nii', id);
        outfile = [outfile; file];
    end
end

fileT = [fileT; '../matlab-data/MRI/50002.mat'];
outfile = [outfile; '../result/MRI/50002_register.nii'];

% Perform registrations
for i = 1:length(fileT)
    fprintf('------------------------------ REGISTRATION %i ---------------------------------- \n\n', i);
    register(char(fileT(i)), fileR, char(outfile(i)));
    fprintf('\n\n--------------------------------------------------------------------------------- \n\n');
end