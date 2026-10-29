clear, close all, clc

% Load color sensor data. Columns are red, green, blue, clear channels
red = readmatrix("red.csv");
yellow = readmatrix("yellow.csv");
blue = readmatrix("blue.csv");
lbrown = readmatrix("lightbrown.csv");
dbrown = readmatrix("darkbrown.csv");
ambient = readmatrix("ambient.csv");

% normalize color data
redNorm = red(:,4)./red(:,1:3)*100;
blueNorm = blue(:,4)./blue(:,1:3)*100;
yellowNorm = yellow(:,4)./yellow(:,1:3)*100;
lbrownNorm = lbrown(:,4)./lbrown(:,1:3)*100;
dbrownNorm = dbrown(:,4)./dbrown(:,1:3)*100;
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

lbBin = [min(lbrownNorm(:,1)) max(lbrownNorm(:,1)) 
         min(lbrownNorm(:,2)) max(lbrownNorm(:,2)) 
         min(lbrownNorm(:,3)) max(lbrownNorm(:,3))]

dbBin = [min(dbrownNorm(:,1)) max(dbrownNorm(:,1)) 
         min(dbrownNorm(:,2)) max(dbrownNorm(:,2)) 
         min(dbrownNorm(:,3)) max(dbrownNorm(:,3))]

aBin = [min(ambNorm(:,1)) max(ambNorm(:,1)) 
        min(ambNorm(:,2)) max(ambNorm(:,2)) 
        min(ambNorm(:,3)) max(ambNorm(:,3))]