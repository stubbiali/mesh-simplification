% Function performing Multi-level Parametric Image Registration (MLPIR) 
% using FAIR. This is particularly useful to rigidly allign MRIs before
% building the template by averaging among all MRIs.
% The following settings for FAIR building blocks are used:
% - interpolation   spline with bending regularization
% - distance        sum of squared distances (SSD)
% - transformation  three-dimensional affine
% For saving time and memory in the subsequent nonparametric image 
% registration, the user may consider the opportunity to use 
% reduced resolution images, obtained by linear or spline interpolation of
% the original data on a coarse grid halfing the number of voxels along
% each direction. Of course, this would come at a cost of a reduced images
% quality. To exploit this opportunity, the datasets should be preprocessed
% using the function interpolate.m.
%
% \param fileT  path to Matlab dataset storing template image, i.e. the 
%               image to align; the dataset should have been created 
%               through the function import_data
% \param fileR  path to Matlab dataset storing reference image; the dataset
%               should have been created through the function import_data
% \out          the template dataset is updated appending the transformed
%               template and its interpolations on the coarse grid

function preregister(fileT, fileR)
    % Add FAIR toolbox to current path
    path(path, '../fair');
    cd ../fair
    FAIRstartup
    cd ../matlab-script
    
    % Load data (data), domain (omega) and discretization (m)
    load(fileR);    dataR = data;    
    load(fileT);    dataT = data;

    % Initialize FAIR building blocks
    inter('reset','inter','splineInter', 'regularizer','moments', 'theta',1e-2);
    distance('reset','distance','SSD');
    trafo('reset','trafo','affine3Dsparse');
    viewImage('reset','viewImage','imgmontage', 'colormap','gray(256)', 'direction','-zyx');

    % Get multi-level structure
    MLdata = getMultilevel({dataT,dataR}, omega, m, 'fig',0, 'minLevel',3);

    % Run MLPIR and get transformation
    w = MLPIR(MLdata, 'plots',0);
    xc = getCellCenteredGrid(omega, m);
    yc = trafo(w, xc);

    % Compute T(yc)
    wc = getSplineCoefficients(data, 'regularizer','moments', 'theta',1e-2);
    data_preregister = inter(wc, omega, yc);  
    data_preregister = reshape(data_preregister, m);

    % Compute coarse grid
    xc = getCellCenteredGrid(omega, m/2);

    % Get linear interpolation
    inter('reset','inter','linearInter');
    data_preregister_linear = inter(data_preregister, omega, xc);
    data_preregister_linear = reshape(data_preregister_linear, m/2);

    % Spline interpolation
    inter('reset','inter','splineInter', 'regularizer','moments', 'theta',1e-2);
    wc = getSplineCoefficients(data_preregister, 'regularizer','moments', 'theta',1e-2);
    data_preregister_spline = inter(wc, omega, xc);
    data_preregister_spline = reshape(data_preregister_spline, m/2);

    % Save data
    save(fileT, 'data_preregister', 'data_preregister_linear', ...
        'data_preregister_spline', '-append');
end


