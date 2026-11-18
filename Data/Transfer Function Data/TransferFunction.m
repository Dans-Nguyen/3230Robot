clear, close all, clc

% 4.1 data
data4_1 = load("data4_2_grounded_no_ginny.mat"); % col: t, V, M
omega = data4_1.data(:,2);
tOut = data4_1.data(:,1);
d = 5; % Input step voltage
titleStr="Percy Transfer Function without Ginny";

[K, tau, omega_ss] = lab11TransferFunction(omega, tOut, d, titleStr)

% 4.2 data
data4_1 = load("data4_2_grounded_with_ginny.mat"); % col: t, V, M
omega = data4_1.data(:,2);
tOut = data4_1.data(:,1);
d = 5; % Input step voltage
titleStr="Percy Transfer Function without Ginny";

[K, tau, omega_ss] = lab11TransferFunction(omega, tOut, d, titleStr)