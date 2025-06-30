function ContrastEnhancedImg = enhanceContrast(img,method)

% Ensure that the color values are 0,...,255 double
img = double(img);  
if any(img<=1-eps & img>=eps,'all') % Maybe transform from [0,1] to [0,255]
  img = round(255*img);
end

if strcmp(method,'contrastStretching')

  kmax = max(img(:));
  kmin = min(img(:));
  % TODO: Contrast Stretching
  X=(img-kmin)*(255/(kmax-kmin));
  ContrastEnhancedImg=uint8(X); % Convert back to displayable imag

% TODO: hist Equalization

elseif strcmp(method, 'histEqualization')
    % Histogram Equalization
    hist_counts = histcounts(img(:), 0:256);  % histogram
    totalpixels = numel(img);  % Corrected from 'nume1()'
    cdf = cumsum(hist_counts) / totalpixels;  % normalized cumulative distribution
    cdf_scaled = round(255 * cdf);  % scale CDF to 0–255
    lut = cdf_scaled;  % lookup table

    % Apply LUT to image
    ContrastEnhancedImg = uint8(reshape(lut(img + 1), size(img)));

else
    error("Unknown enhancement method.");

end
end