%% Function for creating an obstacle of the form of a angled plate
function [ fu ] = obstacleplate( x00,y00,r00,s00,w00 )

    fu = @fuu;

    function z = fuu(conf,x,y,z)
        x0 = conf.lx*x00;
        y0 = conf.ly*y00;
        r0 = conf.ly*r00;
        s0 = conf.ly*s00;
        w0 =         w00*pi/180;
        
        
        
        xv = [ -r0*cos(w0)-s0*sin(w0) -r0*cos(w0)+s0*sin(w0) +r0*cos(w0)+s0*sin(w0) +r0*cos(w0)-s0*sin(w0) ];
        yv = [ +r0*sin(w0)-s0*cos(w0) +r0*sin(w0)+s0*cos(w0) -r0*sin(w0)+s0*cos(w0) -r0*sin(w0)-s0*cos(w0) ];
        
        %% Create obstacles
        for i=1:length(x)
            for j=1:length(y)
                if(inpolygon(x(i)-x0,y(j)-y0,xv,yv))
                    z(i,j) = 0;
                end
            end
        end
        
        
    end


end

