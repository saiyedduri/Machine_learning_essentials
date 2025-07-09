function U = median_filter(F,s)

% median filter
sz = size(F);
U = F;
for k=s+1:sz(1)-s
  for l=s+1:sz(2)-s
    % TODO: compute median
    U(k,l) = median(F(k+(-s:s),l+(-s:s)),'all');
  end
  h = waitbar((k-s)/(sz(1)-2*s));
end
close(h);

end