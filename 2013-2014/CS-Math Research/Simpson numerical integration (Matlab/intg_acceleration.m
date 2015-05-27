% Test d'integration avec les moindres carrés
clear all
load('acc2.mat')

% Interpolation des acceleration
c=input('le nombres de points de l acceleration à prendre <310:'); 
%     je choisi la tranche de l'integration à intégrer du point 1 à c
for i=1:c
        ac(i)=at(i,1);
        ta(i)=at(i,2);
 end
t1=linspace(ta(1),ta(c),(2*c+1));
pa=spline(ta,ac,t1);
% interpolation par des splines cubique de l'accelration 
figure('Name','acceleration');
plot(t1,pa,'ob');
axis([-1 50 -15 55]); 

  %Interpolation des vitesses
    n=size(pa);
    cpt1=1;
    f=abs(n(2)/3);
        for i=1:f
            tv(1)=t1(cpt1);
            tv(2)=t1(cpt1+1);
            tv(3)=t1(cpt1+2);
            tvc(1)=0;
            tvc(i+1)=tv(2);
            pac(1)=pa(cpt1);
            pac(2)=pa(cpt1+1);
            pac(3)=pa(cpt1+2);
            cpt1=cpt1+2;
            dv(i)=trapz(tv,pac);
                %integration par les trapèzes par bloc de 3 points
            v(1)=0;
            v(i+1)=v(i)+dv(i);
                %récuperation de la vitesse
        end
      figure('Name','vitesse');
      plot(tvc,v,'ob');
      
            %Interpolation des positions (on fait pareil)
             n=size(v);
             cpt1=1;
             f=abs(n(2)/3);
                for i=1:f
                     tx(1)=tvc(cpt1);
                     tx(2)=tvc(cpt1+1);
                     tx(3)=tvc(cpt1+2);
                     tx(1)=0;
                     txc(i+1)=tx(2);
                     pxc(1)=v(cpt1);
                     pxc(2)=v(cpt1+1);
                     pxc(3)=v(cpt1+2);
                     cpt1=cpt1+2;
                     dx(i)=trapz(tx,pxc);
                     %integration par les trapèzes par bloc de 3 points
                     x(1)=0;
                     x(i+1)=x(i)+dx(i);
                     %récuperation des positions
      
               end
             figure('Name','position');
             plot(txc,x,'+b');