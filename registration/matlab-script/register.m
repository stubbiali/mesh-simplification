% Function performing Multi-level Image Registration (MLIR) using FAIR.
% The following settings for FAIR building blocks are used:
% - interpolation   spline with bending regularization
% - distance        sum of squared distances (SSD)
% - regularization  matrix-free curvature
% - preregistration three-dimensional affine
% For saving time and memory, the user may consider the opportunity to use 
% reduced resolution images, obtained by linear or spline interpolation of
% the original data on a coarse grid halfing the number of voxels along
% each direction. Of course, this would come at a cost of a reduced images
% quality. To exploit this opportunity, the datasets should be preprocessed
% using the function interpolate.m.
% Note that both FAIR and NiFTI image toolboxes will be used.
%
% \param fileT      path to Matlab dataset storing template image, i.e. the
%                   image to align; the dataset should have been created 
%                   through the function import_data
% \param fileR      path to Matlab dataset storing reference image; the dataset
%                   should have been created through the function import_data
% \param outfile    desired location of the output transformed template in
%                   NiFTI format (extension .nii)
% \out              transformed template in NiFTI format

function register(fileT, fileR, outfile)
    % Add FAIR and NiFTI image toolbox to current path
    path(path, '../nifti-image-toolbox');
    path(path, '../fair');
    cd ../fair
    FAIRstartup
    cd ../matlab-script
    
    % Load data (data[_preregister], data[_preregister]_linear, 
    % data[_preregister]_spline), domain (omega) and discretization (m)
    % Note that a rigid preregistration is performed by FAIR function
    % MLIR.m, then using preregistered data here is not strictly necessary
    load(fileR);    dataR = data;   omegaR = omega;     
    load(fileT);    dataT = data;   omegaT = omega;     m = size(dataT);
    %omega = [omegaT; omegaR];
    
    % Initialize FAIR building blocks
    inter('reset','inter','splineInter', 'regularizer','moments', 'theta',1e-2);
    distance('reset','distance','SSD');
    regularizer('reset','regularizer','mfCurvature', 'alpha',1e1);
    trafo('reset','trafo','affine3Dsparse');
    viewImage('reset','viewImage','imgmontage', 'colormap','gray(256)', 'direction','-zyx');

    % Get multi-level structure
    MLdata = getMultilevel({dataT,dataR},omegaT,m, 'fig',0, 'minLevel',3);

    % Run MLIR
    yc = MLIR(MLdata, 'plots',0, 'plotMLiter',0);

    % Compute transformed template
    wc = getSplineCoefficients(data, 'regularizer','moments', 'theta',1e-2);
    Tc = inter(wc, omegaT, yc);  Tc = reshape(Tc, m);

    % Update template dataset
    %save(fileT, 'yc', 'fileR', '-append');
    
    % Create NiFTI file of transformed template
    voxel_size = (omegaT(2:2:end) - omegaT(1:2:end))./m;
    mat2nii(Tc, voxel_size, outfile);
end