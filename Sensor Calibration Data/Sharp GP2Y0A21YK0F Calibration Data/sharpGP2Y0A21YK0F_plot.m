clear, close all, clc

% Distance data [cm]
data = readtable("sharpGP2Y0A21YK0F_data.xlsx");
D = data.Dist_cm_;
V = data.Volt_V_;

figure;
plot(log(D), log(V), Linestyle='none', Marker='o')
xlabel('ln(distance)')
ylabel('ln(V)')
title("Dans Nguyen u1441973")