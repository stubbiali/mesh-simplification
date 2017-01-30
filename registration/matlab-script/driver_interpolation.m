% The high-resolution brains scans make the Multi-level Image Registration
% (MLIR) really time- and memory-consuming. Then, we perform a linear or 
% spline interpolation, thus halfing the number of voxels along each 
% direction. The FAIR interpolation toolbox is used.
%
% \sa interpolate.m

clc
close all
clear variables
clear variables -global

% List datasets
dataset = strings(0);
for id = 50002:50033
    if id ~= 50018 && id ~= 50021
        % Load the data
        file = sprintf('../matlab-data/MRI/%i.mat', id);
        dataset = [dataset; file];
    end
end

% Go through all datasets
for i = 1:length(dataset)
    % Load the data
    load(char(dataset(i)));

    % Get interpolated data
    [data_linear, data_spline] = interpolate(data, omega, m/2);

    % Save the data
    save(char(dataset(i)), 'data_linear', 'data_spline', '-append');
end