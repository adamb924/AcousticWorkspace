[x,fs] = auload('/Users/Adam/Documents/Octave/sample.wav');
sig = x(1601:2112);
ifft(log(abs(fft(hamming(512) .* sig))))
cepstrum1 = abs(ifft(log(abs(fft(hamming(512) .* sig)))));
plot(cepstrum1(1:20))
cepstrum2 = abs(fft(log(abs(fft(hamming(512) .* sig)))));
plot(cepstrum2(1:20))
plot(cepstrum1,cepstrum2)
% evidently it doesn't make a difference