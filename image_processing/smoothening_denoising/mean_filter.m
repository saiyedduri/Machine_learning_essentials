function U = mean_filter(F,s)

% TODO: mean filter
mask = ones(2*s+1)/(2*s+1)^2;
U = conv2(F,mask,'same');

end