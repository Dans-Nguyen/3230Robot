clear, close all, clc

% Load color sensor data. Columns are red, green, blue, clear channels
red = readmatrix("red.csv");
yellow = readmatrix("yellow.csv");
blue = readmatrix("blue.csv");
ambient = readmatrix("ambient.csv");

% normalize color data
redNorm = red(:,4)./red(:,1:3)*100;
blueNorm = blue(:,4)./blue(:,1:3)*100;
yellowNorm = yellow(:,4)./yellow(:,1:3)*100;
ambNorm = ambient(:,4)./ ambient(:,1:3)*100;

% Compute Bins
rBin = [min(redNorm(:,1)) max(redNorm(:,1)) 
        min(redNorm(:,2)) max(redNorm(:,2)) 
        min(redNorm(:,3)) max(redNorm(:,3))]

yBin = [min(yellowNorm(:,1)) max(yellowNorm(:,1)) 
        min(yellowNorm(:,2)) max(yellowNorm(:,2)) 
        min(yellowNorm(:,3)) max(yellowNorm(:,3))]

bBin = [min(blueNorm(:,1)) max(blueNorm(:,1)) 
        min(blueNorm(:,2)) max(blueNorm(:,2)) 
        min(blueNorm(:,3)) max(blueNorm(:,3))]

ambMean = mean(ambient(:,end))