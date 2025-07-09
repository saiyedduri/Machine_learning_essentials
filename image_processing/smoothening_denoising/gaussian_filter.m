function U = gaussian_filter(F,s)

if s==0
  U = F;
  return
end

% Determine sigma from the mask size
sigma=s/3;

% TODO: Compute the mask
% Gaussian values 
[X,Y] = meshgrid(-s:s,-s:s);
R = sqrt(X.^2 + Y.^2);      % for every pixel distance to central pixel
mask = 1/sqrt(2*pi*sigma^2) * exp(-(R).^2/(2*sigma^2));
% Ideas:
%   - Leave 1/sqrt(2*pi*sigma^2) since we normalize later on
%   - Alternatively use
% b = (2*pi*sigma^2)^(-1/4)*exp(-(-s:s).^2/(2*sigma^2));
% mask = b'*b;
%   - or compute U by conv2(conv2(A,b'),b) which is much faster for big 
%     filter sizes. Do not forget normalization.
%     This works since mask = conv(b',b) and convolution is 
%
% truncate
mask(R>3*sigma)=0;
% normalize
mask = mask ./ sum(mask(:));

% TODO: use the mask on F
% convolution (moving average)
U = conv2(F,mask,'same');

end