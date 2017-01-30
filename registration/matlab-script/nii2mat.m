% Import a 3D NiFTI image (extension .nii) in a Matlab array.
% The NiFTI Matlab toolbox is used.
%
% \param filename       path to input NiFTI image
% \param outfile [opt]	path to output Matlab dataset storing the matrix, the
%                       domain and the resolution
% \out   mat            output Matlab matrix; note that the data are arranged in
%                       the array according to the right-handed reference system 
%                       used in FAIR
% \out   omega          image domain deduced from image resolution and voxels size
% \out   m              number of voxels along each direction

function [mat, omega, m] = nii2mat(infile, varargin)
    % Add toolbox for handling NiFTI images to the current path
    path(path, '../nifti-image-toolbox');
    
    % Import data
    nii = load_nii(infile);
    mat = nii.img;  
    voxel_size = nii.hdr.dime.pixdim(2:4);
    m = size(mat);
        
    % Make data compliant with right-handed coordinate system employed by FAIR
    mat = flip(flip(permute(double(mat), [3,2,1]), 3), 1);
    voxel_size = flip(voxel_size, 2);
    m = flip(m, 2);
    
    % Recover the domain
    omega = [0,m(1)*voxel_size(1), 0,m(2)*voxel_size(2), 0,m(3)*voxel_size(3)];
    
    % Save data
    if ~isempty(varargin)
        data = mat;
        save(varargin{1}, 'data', 'omega', 'm');
    end
end
    
    