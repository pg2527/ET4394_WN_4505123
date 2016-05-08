Prob_empty = makedist('Normal', 'mu', -67.0037, 'sigma', 2.62);
Prob_sig_present = makedist('Normal', 'mu', -49.37, 'sigma', 7.53);
threshold = -60;
pfa = 1 - cdf(Prob_empty, threshold)
pd = 1 - cdf(Prob_sig_present, threshold)
level = [-90:-30];
figure(1);
plot(level, Prob_empty.pdf(level));
hold on
plot(level,Prob_sig_present.pdf(level), 'm');
title('RTL-SDR Performance')
hold on
Y = 0:0.1:0.3;
X = threshold * ones(size(Y))
plot(X, Y, 'r--');
legend('No target present', 'Target present', 'Threshold')
xlabel ('level (dB)');
pfa_ROC = 1 - cdf(Prob_empty, level);
pd_ROC = 1 - cdf(Prob_sig_present, level);
figure(2);
plot(pfa_ROC, pd_ROC);
title('ROC curve');
ylabel ('Probability of Detection');
xlabel ('Probability of False Alarm');