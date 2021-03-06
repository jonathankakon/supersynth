

fs = 48000;
fc = 18300;
rlen = 10;
ppiv = 256;     %points per sampling interval
beta = 9.0;
apof = 0.9;
apobeta = 0.7;

pts = ppiv * rlen + 1;  %impulse length in points

x1 = 0:1:(pts-1);
x2 = rlen * 2 * (x1 - (pts-1)/2 + 0.00001)/(pts-1);
x3 = pi*fc/fs*x2;

h = sin(x3)./x3;
w = kaiser(pts, beta);
g = w.*h';

% apodization and normalization
aw = 1 - apof * kaiser(pts, apobeta);
g = aw.*g;
g = g/max(g) ;

dlmwrite('bandlimited_step_impulse_48000.dat',g','precision',32);



%g(501) = 0;
%h = zeros(1001,1);
%h(1) = g(1);
for ii = 2:length(g)
    g(ii) = g(ii-1) + g(ii);
end

g = g/g(end) * 2;

f = g;
for ii = 1:1281
    f(ii+1280) = f(ii+1280) -2;

end


dlmwrite('bandlimited_saw_difference_48000.dat',f' ,'precision', 32);

for ii = 2:length(f)
    f(ii) = f(ii-1) + f(ii);
end

f = f/max(f);

dlmwrite('bandlimited_triangle_difference_48000.dat',f' ,'precision', 32);

% Plot
    
figure(1);
subplot(1,2,1);
plot(x2/2,f);
axis([-rlen/2 rlen/2 -0.2 1.0001]); xlabel('Time in Sampling Intervals'); title('Bandlimited Impulse'); subplot(1,2,2);

zpad = 20;
g2 = [g ; zeros((zpad-1)*pts,1)];
wspec = abs(fft(g2));
wspec = max(wspec/max(wspec), 0.00001);
fmax = 60000;
rng = round(rlen*zpad*fmax/fs);
xidx = 0:1:rng; semilogy(fmax/1000*xidx/rng,wspec(1:(rng+1))); xlabel('Frequency in kHz');
title('Amplitude Spectrum');
grid;

% markers at 20 kHz, fs-20 kHz and fs
hold;
plot([20 20], [0.00001 1], 'g'); plot([fs/1000-20 fs/1000-20], [0.00001 1], 'r'); plot([fs/1000 fs/1000], [0.00001 1], 'r');
hold off;