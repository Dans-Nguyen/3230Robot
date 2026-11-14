function [K, tau, omega_ss] =  lab11TransferFunction(omega,tOut,d,titleStr)
% Estimates transfer function and plots necessary figures for lab 11.
% To the TA grading this, hi

    % Find steady state speed
    omega_ss = mean(omega(20:55));
    
    % estimate time constant using semi-log plot
    y = log(1-omega/omega_ss);
     
    % estimate tau
    p = polyfit(tOut(1:12), y(1:12), 1);
    slope = p(1);      % slope
    tau = -1/slope;
    
    % Estimate K constant
    K = omega_ss/d;
    
    % Transfer function
    num = [K];
    den = [tau, 1];
    sys = tf(num, den);
    [yOutSys, tOutSys] =  step(d*sys, 1);

    % PLOTS

    % Measured angular speed vs time
    figure;
    plot(tOut, omega, "ro");
    xlabel("Time [s]")
    ylabel("Velocity [rad/s]")

    % estimate omega after 1 time constant
    hold on;
    plot(tau, 0.632*omega_ss, "bx")

    
    
    textStr = "Estimated time constant: %.3f s";
    str = compose(textStr, tau);
    text(tau + 0.03, 0.632*omega_ss, str)

    title(titleStr)

    % Time constant approximation graph
    figure;
    plot(tOut(1:12), y(1:12)); % plot only cropped data
    xlabel("Time [s]")
    ylabel("ln(1-\Omega/\Omega_{ss})")
    
    title(titleStr)
    
    % Computed transfer function output against measured data
    figure;
    plot(tOutSys, yOutSys);
    xlabel("Time [s]")
    ylabel("Velocity [rad/s]")
    hold on;
    plot(tOut, omega, "ro");
    legend("Computed Data", "Measured Data")
    
    title(titleStr)
end