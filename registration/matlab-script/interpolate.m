% Function to interpolate an image onto a user-defined grid. Both linear 
% and spline interpolation are performed. The FAIR toolbox is used.
%
% \param data           matrix storing the data to interpolate
% \param omega          domain
% \param m              number of grid cells along each direction
% \out   data_linear    linear interpolation of data
% \out   data_spline    spline interpolation of data

function [data_linear, data_spline] = interpolate(data, omega, m)
    % Add FAIR toolbox to current path
    path(path, '../fair');
    cd ../fair
    FAIRstartup
    cd ../matlab-script
      
    % Construct coarse grid
    xc = getCellCenteredGrid(omega, m);

    % Linear interpolation
    inter('reset','inter','linearInter');
    data_linear = inter(data, omega, xc);
    data_linear = reshape(data_linear, m);

    % Spline interpolation
    inter('reset','inter','splineInter', 'regularizer','moments', 'theta',1e-2);
    wc = getSplineCoefficients(data, 'regularizer','moments', 'theta',1e-2);
    data_spline = inter(wc, omega, xc);
    data_spline = reshape(data_spline, m);
end