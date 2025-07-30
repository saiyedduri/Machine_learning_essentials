function [u,v] = defaultImages(img, angle, scale, shift)

xshift = shift(1);
yshift = shift(2);

% Convert image to square
[H, W] = size(img);
L = min(H, W);
img = img(1:L, 1:L);

% Make image odd-sized (so center is well-defined)
if mod(L, 2) == 0
    img = padarray(img, [1 1], 'post');
end

% Add margin to allow for rotation
padSize = ceil(size(img,1)*(sqrt(2)-1)/2)+1;
u = padarray(img, [padSize padSize], 0, 'both');

% Rotate
v = imrotate(u, angle * 180/pi, 'crop');

% Scale
szN = round((scale * size(v)) / 2) * 2 + 1;
v = imresize(v, szN);

% Resize u to match v (for subtraction or alignment)
sizeDiff = size(v) - size(u);
padU = max(0, ceil(sizeDiff / 2));
u = padarray(u, padU, 0, 'both');

% Trim to same size
commonSize = min(size(u), size(v));
u = u(1:commonSize(1), 1:commonSize(2));
v = v(1:commonSize(1), 1:commonSize(2));

% Translate
shiftX = round(xshift);
shiftY = round(yshift);
v = circshift(v, [shiftY, shiftX]);

end
