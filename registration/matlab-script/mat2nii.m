% Convert a 3D Matlab array in a NiFTI image. The NiFTI Matlab toolbox is used.
%
% \param mat        Matlab array; this array is supposed to be compliant
%                   with the right-handed coordinate system employed in FAIR
% \param voxel_size size of each voxel
% \param filename   name of output file
%
% Note that the Matlab array is supposed to be compliant with the right-handed
% coordinate system employed in FAIR.

function mat2nii(mat, voxel_size, filename)
    % Add NiFTI Matlab toolbox to current path
    path(path, '../nifti-image-toolbox');
    
    % Permute data to be compliant with the default left-handed coordinate 
    % system of NiFTI format
    mat = permute(flip(flip(mat, 1), 3), [3,2,1]);
    voxel_size = flip(voxel_size, 2);
    
    % Convert to NiFTI format and save
    nii = make_nii(mat, voxel_size);
    save_nii(nii, filename);
end