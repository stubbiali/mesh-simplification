% Script to import 3D MRIs of human brains in NiFTI format.
% The script generates a .mat file with the following variables:
% - data    three-dimensional array of double storing gray-scale value for
%           each voxel; this array is compliant with the right-handed
%           coordiante system used by FAIR
% - omega   domain in the format [x_min, x_max, y_min, y_max, z_min, z_max]
% - m       number of voxels along each direction

clc
close all
clear variables
clear variables -global

% Specify path to NiFTI image (extension .nii)
infile = '../brain-data/MRI/ABIDE_50006_MRI_MP-RAGE_br_raw_20120830234259859_S165414_I329424.nii';

% Specify path to output file (extension .mat)
outfile = '../matlab-data/MRI/50006.mat';

% Import the image and get voxel size
[data, omega, m] = nii2mat(infile, outfile);
voxel_size = (omega(2:2:end) - omega(1:2:end))./m;

% Possibly trim the image to reach a certain resolution
% Comment the following three lines if not required
%data = data(1:end-1,1:end-1,41:end-41);
%m = size(data);
%omega = [0,m(1)*voxel_size(1), 0,m(2)*voxel_size(2), 0,m(3)*voxel_size(3)];

% Create Matlab dataset
save(outfile, 'data', 'omega', 'm');

% Re-save NiFTI image; this is done to set the origin in (0,0,0)
mat2nii(int16(data), voxel_size, infile);