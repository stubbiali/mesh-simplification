%==============================================================================
% (c) Jan Modersitzki 2010/12/27, see FAIR.2 and FAIRcopyright.m.
% http://www.mic.uni-luebeck.de/people/jan-modersitzki.html
%
% function [t,Yt,LSiter] = Armijo(objFctn,Yc,dY,Jc,dJ,varargin)
%
% Armijo Line Search, t=2^-[0:10]
%
% if objFctn( Yc + t*dY ) <= Jc + t*LSreduction*(dJ*dY), 
%   success!
% else
%   t=0, no success
% endIf
%
% Input:
%   objFctn    function handle to the objective function
%   Yc         current vlue of Y
%   dY         search direction
%   Jc         current function value
%   dJ         current gradient 
%  varargin    optional parameters, see below
%
% Output:
%  t          steplength
%  Yt         new iterate
%  LSiter     number of steps performed
%  LS         flag for success
%
% see, e.g., 
%
%  @Book{NocWri1999,
%      author = {J. Nocedal and S. J. Wright},
%       title = {Numerical optimization},
%        year = {1999},
%   publisher = {Springer},
%     address = {New York},
%  }
%  
% see also GaussNewton and optim.
%==============================================================================

function [t,Yt,LSiter,LS] = Armijo(objFctn,Yc,dY,Jc,dJ,varargin)

if nargin == 0, % help and minimal example
    help(mfilename);   GaussNewton;  return;
end;

LSMaxIter   = 10;           % max number of trials
LSreduction = 1e-4;         % slope of line
t = 1;                      % initial step
for k=1:2:length(varargin), % overwrites default parameter
  eval([varargin{k},'=varargin{',int2str(k+1),'};']);
end;

L = [];
descent =   dJ * dY;
for LSiter =1:LSMaxIter,
  Yt = Yc + t*dY;       % compute test value Yt
  Jt = objFctn(Yt);      % evalute objective function
  LS = (Jt<Jc + t*LSreduction*descent); % compare
  if LS, break; end;    % success, return
  L = [L;t,Jt];
  t = t/2;          % reduce t
  
end;
if LS, return; end;      % we are fine
fprintf('Line Search failed - break\n');
t = 0; Yt = Yc;        % take no action

%{ 
  =======================================================================================
  FAIR: Flexible Algorithms for Image Registration, Version 2011
  Copyright (c): Jan Modersitzki
  Maria-Goeppert-Str. 1a, D-23562 Luebeck, Germany
  Email: jan.modersitzki@mic.uni-luebeck.de
  URL:   http://www.mic.uni-luebeck.de/people/jan-modersitzki.html
  =======================================================================================
  No part of this code may be reproduced, stored in a retrieval system,
  translated, transcribed, transmitted, or distributed in any form
  or by any means, means, manual, electric, electronic, electro-magnetic,
  mechanical, chemical, optical, photocopying, recording, or otherwise,
  without the prior explicit written permission of the authors or their
  designated proxies. In no event shall the above copyright notice be
  removed or altered in any way.

  This code is provided "as is", without any warranty of any kind, either
  expressed or implied, including but not limited to, any implied warranty
  of merchantibility or fitness for any purpose. In no event will any party
  who distributed the code be liable for damages or for any claim(s) by
  any other party, including but not limited to, any lost profits, lost
  monies, lost data or data rendered inaccurate, losses sustained by
  third parties, or any other special, incidental or consequential damages
  arrising out of the use or inability to use the program, even if the
  possibility of such damages has been advised against. The entire risk
  as to the quality, the performace, and the fitness of the program for any
  particular purpose lies with the party using the code.
  =======================================================================================
  Any use of this code constitutes acceptance of the terms of the above statements
  =======================================================================================
%}