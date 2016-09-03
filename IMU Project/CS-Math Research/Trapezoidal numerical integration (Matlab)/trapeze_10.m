function []=trapeze_10()
clear all
close all
clc


f=@(x)exp(-x^2);   %Entrer ici la fonction de x

    a=input('Entrer la borne mini de l integrale');
    b=input('Entrer la borne maxi de l integrale');
    Eps=input('Entrer la précision voulue');
   
p=1;
Nmax=2000;
i=0;
I_N=0;
N_N=0;

for N=1:p:Nmax
    
    %I_n
    %%%%%%%%%%%%%%%%%%%%
    
    Dx=(b-a)/N; %Calcul de delta X

    C=Dx*(f(a)+f(b))/2;  % Calcul de la constante de l'intégrale

    S=0;
    for i=1:N-1
        S=f(i*Dx)+S;
    end

    I_n=C+Dx*S;
    
    %%%%%%%%%%%%%%%%%%%%%

    i=i+1;
    N_N(i)=N;
    I_N(i)=I_n;
    
    if abs(I_N(i)-I_N(i-1))<Eps
        N_opt=N
        I_N(i)
        break
    end

    
end

% plot(N_N,I_N);
end