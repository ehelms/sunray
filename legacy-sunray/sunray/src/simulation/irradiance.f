!***********************************************************************
!***** 2 ***************************************************************
      subroutine SunDistanceScaleFactor
     &           (rdayangle,
     &            sundist_scale)
     
!     NOTE:  get "rdayangle" from subroutine JULIAN in the "SunTracker" module
     
!     Purpose: Extraterrestrial solar flux depends onEarth-Sun distance 
!              This subroutine needs to run only once for a given day


!     Input:   rdayangle [radians]
!     Return:  sundist_scale

!     Calls:     none
!     Called by: main
 
      IMPLICIT REAL*8 (A-H,O-Z)
      REAL*8 MEANDIST

      PI = DACOS(-1.D0)

!     MEANDIST IS IN METERS
      MEANDIST = 1.49597890D11

!     COMPUTE ECCENTRICITY CORRECTION FACTOR
      TERM1 = +1.000110D0
      TERM2 = +0.034221D0*DCOS(RDAYANGLE)
      TERM3 = +0.001280D0*DSIN(RDAYANGLE)
      TERM4 = +0.000719D0*DCOS(2.D0*RDAYANGLE)
      TERM5 = +0.000077D0*DSIN(2.D0*RDAYANGLE)
      ECF = TERM1+TERM2+TERM3+TERM4+TERM5

!     COMPUTE SUN-EARTH DISTANCE
      SUNDIST = DSQRT( (MEANDIST**2.D0)/ECF )

      sundist_scale = ((MEANDIST/SUNDIST)**2.D0)

! DIAGNOSTICS BEGIN
!      write(*,*)'sundist_scale = ',sundist_scale
! DIAGNOSTICS END
      RETURN
      END
!***********************************************************************
!***** 3 ***************************************************************
      subroutine pathlength
     &           (apsza,
     &            atmpth)
      
      implicit real*8 (a-h,o-z) 
      
!     Purpose: calculates the atmospheric path length given solar zenith angle

!     Input:  apsza
!     Output: atmpth  

!     Calls:      none
!     Called by:  main   

      real*8 pi,apsza,atmpth,radearth,hatm,rapsza
      
      pi = dacos(-1.d0)

!     Mean radius of earth [km]
      radearth = 6376.d0
!     Nominal thickness of the atmosphere [km]      
      hatm = 100.d0
      
      rapsza = pi*apsza/180.d0
       
      term1 = (radearth + hatm)**2
      term2 = (radearth)**2
      term3 = (radearth*dcos(rapsza))**2
      term4 =  radearth*dcos(rapsza)
       
      atmpth = (dsqrt(term1 -term2 +term3)) - term4

!     normalize
      atmpth = atmpth/hatm

! DIAGNOSTICS BEGIN
!       write(*,*)apsza,atmpth
!      do 290 i = 0,90
!       rapsza = pi*dfloat(i)/180.d0
!       term1 = (radearth + hatm)**2
!       term2 = (radearth)**2
!       term3 = (radearth*dcos(rapsza))**2
!       term4 =  radearth*dcos(rapsza)
!       
!       test = (dsqrt(term1 -term2 +term3)) - term4
!
!!      normalize
!       test = test/hatm
!
!       write(*,*)i,test
!290   continue
! DIAGNOSTICS END
                      
      return
      end
!***********************************************************************       
!***** 5 ***************************************************************      
      subroutine InterpolateRegressionParams
     &           (apsza,o3,aod,o3min,o3max,aodmin,aodmax,
     &            direct_parameter_db, diffuse_parameter_db,
     &            direct_coeff, diffuse_coeff)

 
!     Purpose: Interpolate the regression coefficients so that coefficients will be
!      tailored to the apsza, o3, and aod of the simulation scenario.  The interpolation
!      will be piecewise linear between known values of the coefficients. 
!      The result will be arrays direct_coeff(27,4), and diffuse_coeff(27,6) that will be the
!     regression coefficients for the direct irradiance equation and the diffuse sky radiance
!     equation that are specific to the simulation scenario.
!
!     Input:     apsza,o3, aod, direct_parameter_db, diffuse_parameter_db
!     Returns:   direct_coeff, diffuse_coeff
!
!     Calls:         None
!     Called by:  main
      
!     Note: diffuse_parameter_db(apszax = apsza index, bandx = band index,
!               o3x = ozone index, aodx=aod index, i=parameter subscript)    
!           direct_parameter_db( bandx = band index, o3x = ozone index, 
!               aodx=aod index,i=parameter subscript)    
 

      implicit real*8 (a-h,o-z)
      
      integer apszax,bandx,o3x,aodx,apszax_low,apszax_high
     
      real*8 direct_parameter_db(27,2,2,4)
     &      ,diffuse_parameter_db(12,27,2,2,6)
     
      real*8 apsza,o3,aod,direct_coeff(27,4),diffuse_coeff(27,6)
      real*8 p1(27,2,2,6), p2(27,2,6), q1(27,2,4)

! Begin by interpolating for diffuse coefficients
!  p1() and p2() are dummy arrays
             
!     special case apsza = 88 to 90      
      if((apsza.ge.88).and.(apsza.le.90))then
       apszadelta1 = 1.
       apszadelta2 = 0.
       apszax_low  = 11
       apszax_high = 12
       goto 100 
      endif
      
!     Tailor the 6 diffuse sky radiance coefficients to scenario-specific apsza      
      apszax_low  = int(apsza/8.d0) + 1
      apszax_high = 1 + apszax_low 

      apszadelta1 =         dmod(apsza,8.d0) / 8.d0
      apszadelta2 = (8.d0 - dmod(apsza,8.d0))/ 8.d0
!     Note: [ apszax_low <= apsza <= apszax_high ]

100   do 290 bandx = 1,27
       do 285 o3x = 1,2
        do 280 aodx = 1,2
         do 270 i = 1,6

          p1(bandx,o3x,aodx,i) =
     &((apszadelta2)*diffuse_parameter_db(apszax_low ,bandx,o3x,aodx,i))
     & +
     &((apszadelta1)*diffuse_parameter_db(apszax_high,bandx,o3x,aodx,i))

270      continue
280     continue
285    continue
290   continue

!     Further tailor diffuse coefficients to scenario-specific o3
      
      o3delta1 = (o3 - o3min)/(o3max - o3min)
      o3delta2 = (o3max - o3)/(o3max - o3min)
      
      do 390 bandx = 1,27
       do 380 aodx = 1,2
        do 370 I = 1,6
          p2(bandx,aodx,i)=((o3delta2)*p1(bandx,1,aodx,i))
     &                    +((o3delta1)*p1(bandx,2,aodx,i))            
                     
370     continue
380    continue
390   continue

!      Further tailor diffuse coefficients to scenario-specific aod
       aoddelta2 = (aod - aodmin)/(aodmax - aodmin)
       aoddelta1 = (aodmax - aod)/(aodmax - aodmin)
 
      do 490 bandx = 1,27
       do 470 i=1,6
        diffuse_coeff(bandx,i) = ((aoddelta2)*p2(bandx,1,i))
     &                          + ((aoddelta1)*p2(bandx,2,i))
       

470    continue
490   continue

! Now tailor the 4 direct irradiance coefficients to scenario-specific o3

      do 590 bandx = 1,27
       do 580 aodx = 1,2
        do 570 i=1,4
         q1(bandx,aodx,i)=
     &               ((o3delta2)*direct_parameter_db(bandx,1,aodx,i))
     &              +((o3delta1)*direct_parameter_db(bandx,2,aodx,i))
        
570     continue
580    continue
590   continue

! Further tailor the 4 direct irradiance coefficients to scenario-specific aod
      do 690 bandx = 1,27
       do 670 i= 1,4
        direct_coeff(bandx,i)=((aoddelta2)*q1(bandx,1,i)
     &                       + (aoddelta1)*q1(bandx,2,i))
670    continue
690   continue
      return
      end

!***********************************************************************
!***** 11 **************************************************************      
      subroutine PolyDirectIrradiance
     &           (direct_coeff,apsza,saz,slope,aspect
     &           ,atmpth,sundist_scale,band,ssa
     &            , direct_irradiance)      

!     Purpose:  Determines the direct irradiance at 27 wavebands on a poly.
!               Uses the direct irrad. regression equation with coefficients
!               that have been tailored (i.e. interpolated) to the specific
!               O3, and AOD of the scenario to be simulated

!     Input:   direct_coeff[27,4],apsza,saz,slope,aspect,atmpth
!              ,sundist_scale,band,ssa
!     Return:  direct_irradiance[27]

!     Calls:      poly_beam_incident_angle
!     Called by:  main

      implicit real*8 (a-h,o-z)

      real*8 direct_coeff(27,4), apsza, saz,atmpth, sundist_scale
     &       ,band(27),direct_irradiance(27),ssa(27)
     &       ,pi,hor_surface_direct_irrad, norm_surface_direct_irrad
     &       ,incidentang,beta,rapsza,rincidentang,bandwidth
      
      integer iband
      
      pi = dacos(-1.d0)

      
      call poly_beam_incident_angle
     &           (slope,aspect,apsza,saz
     &             ,incidentang)
     
    
!  init array
       do 90 iband = 1,27
        direct_irradiance(iband) = 0.d0
90     continue


        rincidentang = (pi/180.d0)*incidentang
        rapsza = (pi/180.d0)*apsza
        
               

      do 190 iband = 1,27 
      
        if(rincidentang.gt.(pi/2.d0)) then
           direct_irradiance(iband) = 0.d0   
!           write(*,*),"rincident = ",rincidentang  
           goto 190
        endif
 
       beta = (atmpth - 1.d0) * ssa(iband) 
       hor_surface_direct_irrad = 0.d0
       norm_surface_direct_irrad = 0.d0
       
       hor_surface_direct_irrad =   
     &     direct_coeff(iband,1) 
     &  + (direct_coeff(iband,2))*(dcos(pi*apsza/360.d0)) 
     &  + (direct_coeff(iband,3))*(exp(-beta))
     &  + (direct_coeff(iband,4))*(dcos(pi*apsza/360.d0))*(exp(-beta))
     
       if(hor_surface_direct_irrad.lt.0.d0)then
        hor_surface_direct_irrad = 0.d0
       endif
       

       
      if(iband.le.23)bandwidth = 5.d0
      if(iband.ge.24)bandwidth = 100.d0
      hor_surface_direct_irrad = bandwidth * hor_surface_direct_irrad

      
       
180   norm_surface_direct_irrad = hor_surface_direct_irrad /
     & (dcos(rapsza))
     
 
!     correct for irradiance for date-specific earth-sun distance
      norm_surface_direct_irrad=norm_surface_direct_irrad*sundist_scale
        
      direct_irradiance(iband)=norm_surface_direct_irrad
     &                         *(dcos(rincidentang))
     
!      if( direct_irradiance(iband) > 10000.d0 ) then
!        write(*,*)"Hor is ",hor_surface_direct_irrad
!        write(*,*)"Norm is ",norm_surface_direct_irrad
!        write(*,*)"Direct is ",direct_irradiance(iband)
!        write(*,*)"Incidentang is ",rincidentang
!        write(*,*)"rapsza is ",rapsza
!        write(*,*)"sundist_scale is ",sundist_scale
!         direct_irradiance(iband) = direct_irradiance(iband)/10000.d0
!      endif
      

     
190   continue      
      return          
      end
!***********************************************************************
!***** 11A *************************************************************      
      subroutine poly_beam_incident_angle
     &           (slope,aspect,apsza,saz
     &             ,incidentang)

!     Purpose: Determine the angle between sun beam and poly's surface normal.

!     Note:  This subroutine requires input data from both the "SUNTRACKER"
!            module and polygon-specific slope and relative aspect data.

!     Input:  slope[deg], aspect[deg]
!             apparent solar zenith angley [deg]
!             solar azimuth array [deg]

!     Return: incident angle [deg]

!     Calls:     none
!     Called by: PolyDirectIrradiance

      implicit real*8 (a-h,o-z)
     
      
      real*8 slope, aspect, apsza, saz,ab
     &       ,rslope, raspect, rapsza,rsaz,incidentang
      
      
      pi = dacos(-1.d0)

         pa  = (pi/180.d0)*apsza
         pb  =  slope
         apb = (aspect - (pi/180.d0)*saz)
        

!     The angle between the sun and a point in the sky(zen,raz) is AB:

         ab  = dacos(  ((dcos(pa)) * (dcos(pb))              )  
     &      +          ((dsin(pa)) * (dsin(pb)) * (dcos(apb)))  ) 
        
        
!         write(*,*),"Slope = ",slope
!         write(*,*),"Aspect = ",aspect
!         write(*,*),"Incident = ",ab  
            
        rincidentang = ab
         incidentang = (180.d0/pi)*rincidentang

499   return
      end
!*********************************************************************** 
!***** 10 **************************************************************      
      subroutine PolyDiffuseIrradiance
     &          (diffuse_coeff,apsza,saz
     &          ,body_geo_orient,grid_res
     &          ,sundist_scale
     &          ,ray_slope,ray_aspect
     &          ,slope,aspect
     &          ,diffuse_irradiance) 
    

!     Purpose: Numerically integrate the diffuse irradiance on a given polygon, given its
!              slope and aspect, and the solar zenith and azimuth angles. Solid angle directions
!              that are obsrutcted by other polys are masked out (contributingno irrad).

!     Input:   diffuse_coeff,apsza,saz,sundist_scale,band,
!             ms,soredz,body_coord_twist,body_geo_orient,obstruct,grid_res,poly_polar_az_array

!     Return:  diffuse_irradiance

!     Calls:      rotate_2_sun, determine_poly_diffuse_sky_radiance
!     Called by:  main

      implicit real*8 (a-h, o-z)
     
      real*8 diffuse_coeff(27,6), apsza, saz, sundist_scale, band(27)
      real*8 body_geo_orient,ray_slope,ray_aspect,rad_array(27)
      real*8 geo_space_slope,geo_space_aspect, diffuse_irradiance(27),pi
      real*8 slope,aspect,dtheta,dphi
 
      
      integer bandx,k
      integer apszax, zenx, azx
      integer grid_res(2)
      
      
      pi = dacos(-1.d0)
      
            
      do 150 k=1,27
         diffuse_irradiance(k) = 0.d0
150   continue
      

!     Convert beam direction from model space to geo space     
      geo_space_slope = ray_slope
      geo_space_aspect = (2.d0*pi - (ray_aspect 
     &           - ((pi/180.d0)*body_geo_orient))) 

      if ( geo_space_aspect.gt.(pi*2.d0) ) then
        geo_space_aspect = geo_space_aspect - (pi*2.d0)
      endif

        
!       write(*,*)"Slope is ",geo_space_slope    
!       write(*,*)"Aspect is ",geo_space_aspect  
         
!     Find the radiance(27) at 27 lamda for a given ray direction
       
       call poly_diffuse_sky_radiance
     &           (apsza,geo_space_aspect,geo_space_slope,diffuse_coeff
     &            ,grid_res,saz
     &            ,rad_array)
       
      
         pa  =  ray_slope
         pb  =  slope
         apb = (ray_aspect - aspect)

!     The angle between the sun and a point in the sky(zen,raz) is AB:

         ab  = dacos(  ((dcos(pa)) * (dcos(pb))              )  
     &      +          ((dsin(pa)) * (dsin(pb)) * (dcos(apb)))  )
     
        r_theta = ab
        
        
      !     Convert to radians
      dtheta = (pi/180.d0)*dfloat(grid_res(1))
      dphi =   (pi/180.d0)*dfloat(grid_res(2))
    
      do 290 bandx = 1,27
         
          
!     These must be in model space

         
       if((rad_array(bandx).lt.0.d0).or.
     &     (dabs(r_theta).gt.(pi/2.d0)).or.
     &     ((r_theta).lt.0.d0))then
!        write(*,*),"Incident angle bigger than 90 ",r_theta
        rad_array(bandx)=0.d0
       endif


145   diffuse_irradiance(bandx) =   
     &       (rad_array(bandx))       
     &   *   ( (dcos(r_theta))*(dsin(r_theta))*(dtheta*dphi) )  
     
!       if(bandx.eq.25)then
!         write(*,*),"Rad Array is ",rad_array(bandx)
!     &   ," theta is ",dcos(r_theta)
!     &  ," Diffuse is ",diffuse_irradiance(bandx)," Band is ",bandx
!        endif
     
 
      if(bandx.le.23)bandwidth = 5.d0
      if(bandx.ge.24)bandwidth = 100.d0
      diffuse_irradiance(bandx) = bandwidth * diffuse_irradiance(bandx)
      

!     correct for irradiance for date-specific earth-sun distance
        diffuse_irradiance(bandx) =
     & (diffuse_irradiance(bandx))*sundist_scale

290    continue

999   return
      end
       
!***********************************************************************
!***** 10B *************************************************************      
      subroutine poly_diffuse_sky_radiance
     &           (apsza,geo_space_aspect,geo_space_slope,diffuse_coeff,
     &            grid_res,saz,
     &            rad_array)
     
!     Note: This subroutine is called once for each polygon
!     Purpose: For a given apsza, 
!              determine the array of radiance values 
!                   (at all view directions:90 x 360) at 27 bands  

!     Input:  apsza,sun_polar_az_array,zen,raz,diffuse_coeff,grid_res
!     Return: rad_array

!     Calls:      none
!     Called by:  poly_diffuse_irradiance
          
      implicit real*8 (a-h,o-z)
      
      integer bandx, apszax
      integer grid_res(2)
      
      real*8 diffuse_coeff(27,6),pi,apsza,geo_space_aspect
      real*8 rad_array(27),geo_space_slope,saz,ab
                            
      pi = dacos(-1.d0)
      
!     initialize rad_array
       do 80 k = 1,27
        rad_array(k) = 0.
80     continue
     
         
         pa  = (pi/180.d0)*apsza
         pb  =  geo_space_slope
         apb = (geo_space_aspect - (pi/180.d0)*saz)

!     The angle between the sun and a point in the sky(zen,raz) is AB:

         ab  = dacos(  ((dcos(pa)) * (dcos(pb))              )  
     &      +          ((dsin(pa)) * (dsin(pb)) * (dcos(apb)))  )
  
            
      do 290 bandx = 1,27
       k = bandx         
             
         if(geo_space_slope.gt.(pi/2.d0))then
          rad_array(k)=0.d0
          goto 290
         endif    

!      The following is the radiance regression equation; aka the "SkyMap"     
        
         rad_array(k) = 
     &       diffuse_coeff(bandx,1)  
     &    + (diffuse_coeff(bandx,2)) * (( dcos(ab/2.d0   )  ) ** 2   )
     &    + (diffuse_coeff(bandx,3)) * (( dcos(ab/2.d0   )  ) ** 100 ) 
     &    + (diffuse_coeff(bandx,4)) * (( dsin(ab/2.d0   )  ) ** 2   ) 
     &    + (diffuse_coeff(bandx,5)) * (( dsin(pb        )  ) ** 18  ) 
     &    + (diffuse_coeff(bandx,6)) * (( dcos(ab/2.d0 ) ) 
     &                               *  ( dsin(ab/2.d0 ) ) )


       if(rad_array(k).lt.0.d0)then
          rad_array(k) = 0.d0
         endif 
        

290   continue
      return
      end

!***********************************************************************       
!***** 12 **************************************************************      
      subroutine poly_total_irradiance
     &                (direct_irradiance, diffuse_irradiance,
     &                  total_irradiance)

!     Purpose: Sum the direct + diffuse irradiance on each poly, at 27 bands

!     Input:   direct_irradiance(27), diffuse_irradiance(27)
!     Return: total_irraidance(27)

!     Calls:        none
!     Called by: main

      real*8 direct_irradiance(27), diffuse_irradiance(27),
     &        total_irradiance(27)

      integer bandx

      do 190  bandx = 1,27
       total_irradiance(bandx) = direct_irradiance(bandx) +
     &                          diffuse_irradiance(bandx)



190   continue

!      write(31,*)'band, direct_irradiance, diffuse_irradiance, total'

      return          
      end     
      
      