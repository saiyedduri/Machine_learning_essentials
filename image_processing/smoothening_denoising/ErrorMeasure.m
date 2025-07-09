function p = ErrorMeasure(u,f,method)

u = double(u);
f = double(f);
N = numel(u);

switch method
  case 'MAE'
    % TODO: Mean absolute Error
    p = 1/N * norm(u(:)-f(:),1);


  case 'MSE'
    % TODO: Mean square Error
    p = 1/N * norm(u(:)-f(:))^2;


  case 'PSNR'
    % TODO: PSNR
    p = 10 * log10( ((max(f(:)) - min(f(:)))^2) * N / norm(u-f,'fro')^2 );
    
end

end