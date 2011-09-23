      program specrad

      implicit real*8 (a-h,o-z)

      character*60 coeffs_beam_file, coeffs_diffuse_file
     &               ,reflectance_file, swf_file
     &               ,poly_data_file, out_file


      integer grid_res(2),ordination(10000),ordination_rev(10000)
     &       ,obstruct(181,360),obstruct_g(181,360),epsilon(10,2)

      integer n_polys,ms, ibeamflag,irbeamflag,bandx,fresnel_flag
     &   ,elev_lo,elev_hi,alb_lo,alb_hi,o3_lo,o3_hi,vis_lo,vis_hi


   
      real*8 reflectance(27,2),bc(27,2,3,3,4),dc(27,2,3,3,3,13)
     &      ,b(27,4),d(27,13),drad(27),gr_poly(91,2)
     &      ,arr(10000,5),sortedz(10000,5)
     &      ,poly_polar_az_array(32400,2),sun_polar_az_array(32400,2)  
     &      ,nssbi(27),hssbi(91,27),hssdi(91,27),hssi(91,27)
     &      ,hbi(91),hdi(91),hsi(91),grad(27),g_irrad(27)
     &      ,rho(11),rholo(11),rhohi(10)
     &      ,phi(11,25),philo(11,25),phihi(11,25)     
     &      ,beam_irrad(27),sky_irrad(27),rbeam_irrad(27),rsky_irrad(27)

      real*8 rdayangle,apsza,saz,elev,alb,z
     &      ,o3,vis
     &      ,body_coord_twist,body_geo_twist
     &      ,o3min,o3max,vismin,vismax
     &      ,sundist_scale,height,bandwidth,precision ,elev_asl
     &      ,elev_a1,elev_a2,elev_g1,elev_g2
     &      ,o3_a1,  o3_a2,  o3_g1,  o3_g2
     &      ,vis_a1, vis_a2, vis_g1, vis_g2
     &      ,alb_a1, alb_a2, alb_g1, alb_g2



10    call read_input
     &   (rdayangle,apsza,saz,elev,alb,z
     &   ,o3,vis
     &   ,body_coord_twist,body_geo_twist
     &   ,grid_res
     &   ,o3min,o3max,vismin,vismax
     &   ,coeffs_beam_file, coeffs_diffuse_file
     &   ,reflectance_file, swf_file, fresnel_flag
     &   ,poly_data_file, out_file)
   
20     call reflectance_function
     &           (reflectance_file,
     &            reflectance)

30     call sun_distance_scale_factor
     &           (rdayangle,
     &            sundist_scale)
       
40     call  read_coeffs_beam
     &            (coeffs_beam_file
     &            ,bc)

50     call read_coeffs_diffuse
     &                 (coeffs_diffuse_file
     &                  ,dc)

60     call compute_interpol_scale_factors
     &    (elev,o3,vis,alb
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi)

70     call interpolate_beam_coeffs
     &             (bc,elev,o3,vis
     &             ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &             ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &             ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &             ,b)

80      call interpolate_diffuse_coeffs
     &    (dc,elev,alb,o3,vis
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  O3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi
     &    ,d)    

90    call read_all_polygon_data
     &               (poly_data_file,n_polys
     &                ,arr) 


100   call sort_polygons_by_z
     &           (n_polys,arr
     &            ,sortedz,ordination,ordination_rev,height)


110   call create_poly_polar_az_array 
     &                (grid_res
     &                  ,poly_polar_az_array,precision)

120   call compute_hsi_field
     & (height,apsza
     & ,saz,precision,sortedz,n_polys
     & ,body_coord_twist,body_geo_orient
     & ,grid_res,poly_polar_az_array
     & ,elev_asl
     & ,b,d
     & ,sundist_scale
     & ,epsilon,rho,rhohi,phi,phihi,atmpth,hssbi,hssdi,hssi)        
      

      do 190 ms = 1,n_polys

       if(mod(ms,5).eq.0)then
        write(*,*)'Processing sorted polygon #',ms
       endif

130    call polygon_obstruction_mask
     &   (apsza,saz,sortedz,n_polys,ms,precision
     &   ,body_coord_twist,body_geo_orient
     &   ,obstruct,ibeamflag,irbeamflag)
      

140    call rotate_2_sun
     &      (apsza,saz,ms,sortedz,body_coord_twist,body_geo_orient
     &      ,grid_res,poly_polar_az_array,sun_polar_az_array) 

150    call spectral_irradiance
     & (sundist_scale,apsza,saz,atmpth,reflectance,fresnel_flag
     & ,grid_res,precision,poly_polar_az_array,sun_polar_az_array
     & ,body_coord_twist,body_geo_orient,ibeamflag,irbeamflag,obstruct
     & ,sortedz,ms,epsilon,rho,rhohi,phi,phihi,hssi,hssdi,hssbi
     & ,b,d
     & ,beam_irrad,sky_irrad,rbeam_irrad,rsky_irrad,g_irrad)

160    call output
     &        (out_file,ordination,ms,beam_irrad,sky_irrad
     &        ,rbeam_irrad,rsky_irrad,g_irrad
     &        )

190   continue

999   stop
      end 
!*********************************************************************** 
!*** 01 ****************************************************************

      subroutine read_input
     &   (rdayangle,apsza,saz,elev,alb,z
     &   ,o3,vis
     &   ,body_coord_twist,body_geo_twist
     &   ,grid_res
     &   ,o3min,o3max,vismin,vismax
     &   ,coeffs_beam_file, coeffs_diffuse_file
     &   ,reflectance_file, swf_file, fresnel_flag
     &   ,poly_data_file, out_file)

!   Purpose: specify all needed input

!   Input:   -
!   Return:  rdayangle, etc {same as sub statment list}

!   Calls:     -
!   Called by: main

      implicit real*8 (a-h,o-z)


      character*60 coeffs_beam_file, coeffs_diffuse_file
     &               ,reflectance_file, swf_file
     &               ,poly_data_file, out_file

      
      integer grid_res(2),fresnel_flag

      real*8 rdayangle,apsza,saz,elev,alb,z
     &   ,o3,vis
     &   ,body_coord_twist,body_geo_twist
     &   ,o3min,o3max,vismin,vismax

      open(7,file='specrad_input.txt')
      read(7,*)
      read(7,*)rdayangle
      read(7,*)apsza
      read(7,*)saz
      read(7,*)elev
      read(7,*)alb
      read(7,*)fresnel_flag
      read(7,*)z
      read(7,*)
      read(7,*)
      read(7,*)o3
      read(7,*)vis
      read(7,*)
      read(7,*)
      read(7,*)body_coord_twist
      read(7,*)body_geo_orient
      read(7,*)grid_res(1)
      read(7,*)grid_res(2)
      read(7,*)o3min
      read(7,*)o3max
      read(7,*)vismin
      read(7,*)vismax
      read(7,*)
      read(7,*)
      read(7,*)coeffs_beam_file 
      read(7,*)coeffs_diffuse_file
      read(7,*)reflectance_file
      read(7,*)swf_file
      read(7,*)poly_data_file 
      read(7,*)out_file

      return
      end
!***********************************************************************
!***** 02 **************************************************************  
  
      subroutine reflectance_function
     &           (reflectance_file,
     &            reflectance)
!     Purpose: Read array of real*8 into 2 dimensional array, given filename

!     Input:  reflectance_filename
!     Return: reflectance & snell-lambert param

!     Calls:     none
!     Called by: main

      character*60 reflectance_file
      integer bandx   

      real*8 reflectance(27,2)
      real*8 band

!   initialize new arrays
      reflectance(1:27,1:2) = 0.d0
      
      open(7,file= reflectance_file)
      read(7,*)
      do 190 bandx = 1,27
       read(7,*)band,reflectance(bandx,1),reflectance(bandx,2)
190   continue
       
      close(7)
      return          
      end      
!***********************************************************************
!*** 03 **************************************************************** 
   
      subroutine sun_distance_scale_factor
     &           (rdayangle,
     &            sundist_scale)
     
!     NOTE:  get "rdayangle" from subroutine JULIAN in "SunTracker" module
     
!     Purpose: Extraterrestrial solar flux depends onEarth-Sun distance 
!              This subroutine needs to run only once for a given day


!     Input:   rdayangle [radians]
!     Return:  sundist_scale

!     Calls:     none
!     Called by: main
 
      implicit real*8 (a-h,o-z)
      real*8 rdayangle,sundist_scale
      REAL*8 pi,MEANDIST,term1,term2,term3,term4,term5,ecf,sundist

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

      RETURN
      END
      
!***********************************************************************
!*** 04 **************************************************************** 

      subroutine read_coeffs_beam
     &            (coeffs_beam_file
     &            ,bc)
!   Purpose: read beam coefficents from file

!   Input:   coeffs_beam_file
!   Return:  bc

!   Calls:     none
!   Called by: main

      implicit real*8 (a-h,o-z)
      character coeffs_beam_file*100

      character model*2

      integer  bandx,elevx,o3x,visx,dx
      integer ilambda,ielev,io3,ivis

      real*8   lambda, elev, o3, vis

      real*8  bc(27,2,3,3,4)

!   initialize new arrays   
      bc(1:27,1:2,1:3,1:3,1:4) = 0.d0
  
      open(7,file = coeffs_beam_file)
      read(7,*)
      do 1190 bandx = 1,27
       do 1180 elevx = 1,1
        
         do 1160 o3x = 1,3
          do 1150 visx = 1,3
                 
1110        read(7,*,ERR=1195)model,ilambda,ielev,io3,ivis
     &        ,(bc(bandx,elevx,o3x,visx,dx),dx=1,4)

1120        format(a2,2x,i3,2x,i2,2x,i3,2x,i2,3x,4(e13.7,3x))

1150      continue
1160     continue

1180   continue
1190  continue 
      goto 1199
1195  write(*,*)'Error at line 1110 in SUB read_coeffs_beam'
     &         ,bandx,elevx,albedox,o3x,visx
1199  close(7)

      return
      end
!***********************************************************************
!*** 05 **************************************************************** 

      subroutine read_coeffs_diffuse
     &                 (coeffs_diffuse_file
     &                  ,dc)
!   Purpose: read diffuse coefficents from file

!   Input:   coeffs_diffuse_file
!   Return:  dc

!   Calls:     none
!   Called by: main

      implicit real*8 (a-h,o-z)
      character coeffs_diffuse_file*100,model*2

      integer bandx,elevx,albedox,o3x,visx,dx
      integer ilambda,ielev,ialbedo,io3,ivis

      real*8   lambda, elev, albedo, o3, vis

      real*8  dc(27,2,3,3,3,13)

!   initialize new arrays
      dc(1:27,1:2,1:3,1:3,1:3,1:4) = 0.d0

      open(7,file = coeffs_diffuse_file)
      read(7,*)
      do 1190 bandx = 1,27
       do 1180 elevx = 1,1
        do 1170 albedox = 1,3
         do 1160 o3x = 1,3
          do 1150 visx = 1,3
                 
1110        read(7,*,ERR=1195)model,ilambda,ielev,albedo,io3,ivis
     &        ,(dc(bandx,elevx,albedox,o3x,visx,dx),dx=1,13)

1150      continue
1160     continue
1170    continue
1180   continue
1190  continue 
      goto 1199
1195  write(*,*)'Error at line 1110 in SUB read_coeffs_diffuse'
     &         ,bandx,elevx,albedox,o3x,visx
1199  close(7)
      return
      end
!***********************************************************************
!*** 06 **************************************************************** 

      subroutine compute_interpol_scale_factors
     &    (elev,o3,vis,alb
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi)

!   Purpose: compute the scale factors that will be used to interpolate
!            the beam and diffuse coefficients so that they 
!            correspond to the actual values of elev,o3,vis
!            rather than the setpoint (aka "reference") values of elev...

!   Input:   elev,o3,vis
         
!   Return:  elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
!            o3_a1,  o3_a2,  o3_g1,  03_g2,  O3_lo,  o3_hi
!            vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi

!   Calls:     setpoints
!   Called by: main

      implicit real*8 (a-h,o-z)
      integer elev_lo,elev_hi,alb_lo,alb_hi,o3_lo,o3_hi,vis_lo,vis_hi

      integer i

      real*8 elev,o3,vis,alb
      real*8 elev_a1,elev_a2,elev_g1,elev_g2
     &      ,o3_a1,  o3_a2,  o3_g1,  o3_g2
     &      ,vis_a1, vis_a2, vis_g1, vis_g2
     &      ,alb_a1, alb_a2, alb_g1, alb_g2


      real*8 elevation_set(2),albedo_set(3),ozone_set(3)
     &      ,visibility_set(3)   



      call setpoints
     &    (elevation_set,albedo_set,ozone_set,visibility_set)

!   Calculate the arithmetic ( _a1; _a2) and geometric (_g1; _g2) scale factors

100   do 190 i = 1,1
       if(      (elev.ge.elevation_set(i))
     &   .and.(elev.le.elevation_set(i+1)))then
        elev_lo = i
        elev_hi = i+1 
        elev_a1 = 1.d0-(( elev               - elevation_set(i))/
     &                (elevation_set(i+1) - elevation_set(i)))
        elev_a2 = 1.d0 - elev_a1        
        if((elev.eq.0.).or.(elevation_set(i) .eq.0.)
     &               .or.(elevation_set(i+1).eq.0.))then
         elev_g1 = elev_a1
         elev_g2 = elev_a2
         goto 200
        endif
        elev_g1 = 1.d0 - ((log(elev/elevation_set(i)))
     &                /(log(elevation_set(i+1)/elevation_set(i))))
        elev_g2 = 1.d0 - elev_g1
        goto 200
       endif
190   continue
      if(elev.lt.elevation_set(1))then
       elev = elevation_set(1)
       goto 100
      elseif(elev.gt.elevation_set(2))then
       elev = elevation_set(2)
       goto 100
      endif
      write(*,*)'error at elevation interpolation: interpolate_diffuse'

200   do 290 i = 1,2
        if((alb.ge.albedo_set(i)).and.(alb.le.albedo_set(i+1)))then
         alb_lo = i
         alb_hi = i+1
         alb_a1 = 1.d0-(( alb            -  albedo_set(i))/
     &                (albedo_set(i+1) - albedo_set(i)))
         alb_a2 = 1.d0 - alb_a1
        if((alb.eq.0.).or.(albedo_set(i) .eq.0.)
     &              .or.(albedo_set(i+1).eq.0.))then
         alb_g1 = alb_a1
         alb_g2 = alb_a2
         goto 300
        endif
         alb_g1 = 1.d0 - ((log(alb/albedo_set(i)))
     &                /(log(albedo_set(i+1)/albedo_set(i))))
         alb_g2 = 1.d0 - alb_g1        
         goto 300
        endif
290   continue
      if(alb.lt.albedo_set(1))then
       alb = albedo_set(1)
       goto 200
      elseif(alb.gt.albedo_set(3))then
       alb = albedo_set(3)
       goto 200
      endif
      write(*,*)'error at albedo interpolation: interpolate_diffuse'
300   do 390 i = 1,2
       if((o3.ge.ozone_set(i)).and.(o3.le.ozone_set(i+1)))then
        o3_lo = i
        o3_hi = i+1 
        o3_a1 =  1.d0-((  o3            - ozone_set(i))/
     &                (ozone_set(i+1) - ozone_set(i)))
        o3_a2 = 1.d0 - o3_a1
        if((o3.eq.0.).or.(ozone_set(i) .eq.0.)
     &             .or.(ozone_set(i+1).eq.0.))then
         o3_g1 = o3_a1
         o3_g2 = o3_a2
         goto 400
        endif
        o3_g1 = 1.d0 - ((log(o3/ozone_set(i)))
     &                /(log(ozone_set(i+1)/ozone_set(i))))
        o3_g2 = 1.d0 - o3_g1     
        goto 400
       endif
390   continue
      if(o3.lt.ozone_set(1))then
       o3 = ozone_set(1)
       goto 300
      elseif(o3.gt.ozone_set(3))then
       o3 = ozone_set(3)
       goto 300
      endif
      write(*,*)'error at ozone interpolation: interpolate_diffuse'
400   do 490 i = 1,2
       if(      (vis.ge.visibility_set(i))
     &    .and.(vis.le.visibility_set(i+1)))then
        vis_lo = i
        vis_hi = i+1 
        vis_a1 =  1.d0-(( vis                - visibility_set(i))/
     &                (visibility_set(i+1) - visibility_set(i)))
        vis_a2 = 1.d0 - vis_a1   
        if((vis.eq.0.).or.(visibility_set(i) .eq.0.)
     &              .or.(visibility_set(i+1).eq.0.))then
         vis_g1 = vis_a1
         vis_g2 = vis_a2
         goto 500
        endif
        vis_g1 = 1.d0 - ((log(vis/visibility_set(i)))
     &                /(log(visibility_set(i+1)/visibility_set(i))))
        vis_g2 = 1.d0 - vis_g1     
        goto 500
       endif
490   continue
      if(vis.lt.visibility_set(1))then
       vis = visibility_set(1)
       goto 400
      elseif(vis.gt.visibility_set(3))then
       vis = visibility_set(3)
       goto 400
      endif
      write(*,*)'error at visibility interpolation: interpolate_diffuse'

500   return
      end
!***********************************************************************
!*** 06A *************************************************************** 

      subroutine setpoints
     &     (elevation_set,albedo_set,ozone_set,visibility_set)

!   Purpose: assign the setpoint (aka "reference") values of elev...
!           

!   Input:   -
         
!   Return:  elevation_set,albedo_set,ozone_set,visibility_set


!   Calls:     -
!   Called by: compute_interpol_scale_factors

      implicit real*8 (a-h,o-z)
      real*8 elevation_set(2),albedo_set(3),ozone_set(3)
     &      ,visibility_set(3)

!   initialize new arrays
      elevation_set(1:2)  = 0.d0
      albedo_set(1:3)     = 0.d0
      ozone_set(1:3)      = 0.d0
      visibility_set(1:3) = 0.d0


      elevation_set(1)= 0.d0
      elevation_set(2)= 3.d0

      albedo_set(1)= 0.05d0
      albedo_set(2)= 0.25d0
      albedo_set(3)= 1.00d0

      ozone_set(1)= 220.d0
      ozone_set(2)= 280.d0
      ozone_set(3)= 340.d0

      visibility_set(1)=  5.d0
      visibility_set(2)= 15.d0
      visibility_set(3)= 50.d0

      return
      end
!***********************************************************************
!*** 07 **************************************************************** 

      subroutine interpolate_beam_coeffs
     &             (bc,elev,o3,vis
     &             ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &             ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &             ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &             ,b)

!   Purpose: interpolate the beam coefficients so that they 
!      correspond to the actual values of elev,o3,vis
!      rather than the setpoint (aka "reference") values of elev...

!   Input:   bc,elev,o3,vis
!            elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
!            o3_a1,  o3_a2,  o3_g1,  03_g2,  O3_lo,  o3_hi
!            vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
 
!   Return:  b

!   Calls:     -
!   Called by: main

      implicit real*8 (a-h,o-z)
      integer vis_lo,vis_hi,o3_lo,o3_hi,elev_lo,elev_hi

      integer i,j,bandx,elevx,o3x


      real*8 bc(27,2,3,3,4),b(27,4)

      real*8 elev_a1,elev_a2,elev_g1,elev_g2
     &      ,o3_a1,  o3_a2,  o3_g1,  o3_g2
     &      ,vis_a1, vis_a2, vis_g1, vis_g2
    
      real*8 beo(27,2,3,4),be(27,2,4)
      real*8 term1,term2

! Initialization of b(), be(), and beo() is necessary to avoid runtime errors
        b(1:27,1:4)         = 0.d0
       be(1:27,1:2,1:4)     = 0.d0
      beo(1:27,1:2,1:3,1:4) = 0.d0

500   do 1390 i = 1,4
        j = i-1
       if((j.eq.0).or.(j.eq.1).or.(j.eq.2))then

!    arithmetic interpolation for terms and factors: j=0,1,2

        do 580 bandx = 1,27
         do 570 elevx = 1,1
           do 550 o3x = 1,3          
            term1 =  (vis_a1*bc(bandx,elevx,o3x,vis_lo,i))
            term2 =  (vis_a2*bc(bandx,elevx,o3x,vis_hi,i))    
            beo(bandx,elevx,o3x,i) = term1 + term2
550        continue
570      continue
580     continue
     
        do 680 bandx = 1,27
         do 670 elevx = 1,1                      
          term1 = (o3_a1*beo(bandx,elevx,o3_lo,i))     
          term2 = (o3_a2*beo(bandx,elevx,o3_hi,i))
          be(bandx,elevx,i) = term1 + term2
670      continue
680     continue

        do 880 bandx = 1,27
         term1 =  (elev_a1*be(bandx,elev_lo,i))      
         term2 = (elev_a2*be(bandx,elev_hi,i)) 
         b(bandx,i) = term1 + term2
880     continue
        
       else
!    geometric interpolation for exponents: j=3

        do 980 bandx = 1,27
         do 970 elevx = 1,1
           do 950 o3x = 1,3  
            term1 = (vis_g1*bc(bandx,elevx,o3x,vis_lo,i))
            term2 = (vis_g2*bc(bandx,elevx,o3x,vis_hi,i))
            beo(bandx,elevx,o3x,i) = term1 + term2
950        continue
970      continue
980     continue

        do 1080 bandx = 1,27
         do 1070 elevx = 1,1   
          term1 = (o3_g1*beo(bandx,elevx,o3_lo,i))      
          term2 = (o3_g2*beo(bandx,elevx,o3_hi,i))   
          be(bandx,elevx,i) = term1 + term2
1070     continue
1080    continue

        do 1280 bandx = 1,27
         term1 = (elev_g1*be(bandx,elev_lo,i))
         term2 = (elev_g2*be(bandx,elev_hi,i))  
         b(bandx,i) = term1 + term2
1280    continue
               
       endif
1390  continue

9999  return    
      end
!***********************************************************************
!*** 08 **************************************************************** 

      subroutine interpolate_diffuse_coeffs
     &    (dc,elev,alb,o3,vis
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  O3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi
     &    ,d)    

!   Purpose: interpolate the diffuse coefficients so that they 
!     correspond to the actual values of elev,alb,o3,vis
!     rather than the setpoint (aka "reference") values of elev...

!   Input:   dc,elev,alb,o3,vis
!            elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
!            o3_a1,  o3_a2,  o3_g1,  03_g2,  O3_lo,  o3_hi
!            vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
!            alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi

!   Return:  d

!   Calls:     -
!   Called by: main

      implicit real*8 (a-h,o-z)
      integer vis_lo,vis_hi,o3_lo,o3_hi,alb_lo,alb_hi,elev_lo,elev_hi

      integer i,j,bandx,elevx,albedox,o3x

      real*8 dc(27,2,3,3,3,13),d(27,13)
      real*8 elev,alb,o3,vis
      real*8 elev_a1,elev_a2,elev_g1,elev_g2
     &      ,o3_a1,  o3_a2,  o3_g1,  o3_g2
     &      ,vis_a1, vis_a2, vis_g1, vis_g2
     &      ,alb_a1, alb_a2, alb_g1, alb_g2

      real*8 deao(27,2,3,3,13),dea(27,2,3,13),de(27,2,13)
      real*8 term1,term2

!   Initialize new arrays
         d(1:27,1:13)             = 0.d0
        de(1:27,1:2,1:13)         = 0.d0
       dea(1:27,1:2,1:3,1:13)     = 0.d0
      deao(1:27,1:2,1:3,1:3,1:13) = 0.d0
     
     
500   do 1390 i = 1,13
        j = i-1
       if((j.eq.0).or.(j.eq.1).or.(j.eq.4).or.(j.eq.7).or.(j.eq.10))then

!    arithmetic interpolation for terms and factors: j=0,1,4,7,10

        do 580 bandx = 1,27
         do 570 elevx = 1,1
          do 560 albedox = 1,3
           do 550 o3x = 1,3 
            term1 = (vis_a1*dc(bandx,elevx,albedox,o3x,vis_lo,i))
            term2 = (vis_a2*dc(bandx,elevx,albedox,o3x,vis_hi,i))         
            deao(bandx,elevx,albedox,o3x,i) = term1 + term2          
550        continue
560       continue
570      continue
580     continue

        do 680 bandx = 1,27
         do 670 elevx = 1,1
          do 660 albedox = 1,3                   
           term1 = (o3_a1*deao(bandx,elevx,albedox,o3_lo,i))    
           term2 = (o3_a2*deao(bandx,elevx,albedox,o3_hi,i))  
           dea(bandx,elevx,albedox,i) = term1 + term2        
660       continue
670      continue
680     continue

        do 780 bandx = 1,27
         do 770 elevx = 1,1                   
          term1 = (alb_a1*dea(bandx,elevx,alb_lo,i))   
          term2 = (alb_a2*dea(bandx,elevx,alb_hi,i))         
          de(bandx,elevx,i) = term1 + term2            
770      continue
780     continue

        do 880 bandx = 1,27
         term1 = (elev_a1*de(bandx,elev_lo,i))   
         term2 = (elev_a2*de(bandx,elev_hi,i))   
         d(bandx,i) = term1 + term2     
880     continue

       else
!    geometric interpolation for exponents: j=2,3,5,6,8,9,11,12

        do 980 bandx = 1,27
         do 970 elevx = 1,1
          do 960 albedox = 1,3
           do 950 o3x = 1,3          
            term1 = (vis_g1*dc(bandx,elevx,albedox,o3x,vis_lo,i))  
            term2 = (vis_g2*dc(bandx,elevx,albedox,o3x,vis_hi,i)) 
            deao(bandx,elevx,albedox,o3x,i) = term1 + term2       
950        continue
960       continue
970      continue
980     continue

        do 1080 bandx = 1,27
         do 1070 elevx = 1,1
          do 1060 albedox = 1,3  
           term1 = (o3_g1*deao(bandx,elevx,albedox,o3_lo,i))     
           term2 = (o3_g2*deao(bandx,elevx,albedox,o3_hi,i))                  
           dea(bandx,elevx,albedox,i) = term1 + term2       
1060      continue
1070     continue
1080    continue

        do 1180 bandx = 1,27
         do 1170 elevx = 1,1  
          term1 = (alb_g1*dea(bandx,elevx,alb_lo,i))   
          term2 = (alb_g2*dea(bandx,elevx,alb_hi,i))                            
          de(bandx,elevx,i) = term1 + term2  
1170     continue
1180    continue

        do 1280 bandx = 1,27
         term1 = (elev_g1*de(bandx,elev_lo,i))   
         term2 = (elev_g2*de(bandx,elev_hi,i))      
         d(bandx,i) = term1 + term2     
1280    continue
               
       endif

1390  continue

9999  return    
      end
!***********************************************************************
!*** 09 **************************************************************** 

      subroutine read_all_polygon_data
     &               (poly_data_file,n_polys
     &                ,arr) 

!   Purpose:  Reads polygon data into an array ( arr[10000,5] ).  The
!     assumed polygon info is the x,y,z location of the polygon, as well 
!     as the slope and aspect, in degrees.  The direction of zero aspect 
!     is assumed to be the direction that the human model is facing.  In 
!     the absence of reading in real polygon data from a file, this 
!     subroutine generates its own, using a random number function.

!     Input:     poly_data_file,n (the number of polygons in the human model)
!     Returns:   arr

!     Calls:     none
!     Called by: main      

      implicit real*8 (a-h,o-z) 
      character*60 poly_data_file     
      integer n,line_err
      real*8 pi,ranval,arr(10000,5),dummy(5)
      
!     Populate the array of polygons arr[n,5]
!     x = arr(n,1) ; y = arr(n,2) ; z = arr(n,3) 
!     slope = arr(n,4) ;  aspect = arr(n,5) 
!     Note: slope is in range [0,180], aspect is in range [0,360)
!     Note: slope and aspect are assumed to be in MODEL SPACE.  
     
!    initialize new arrays
      arr(1:10000,1:5) = 0.d0

      pi = dacos(-1.d0)

      open(7,file = poly_data_file)

!   skip header line
      read(7,*)
 
      do 190 i = 1,10000      
        read(7,*,err=205,end=200)
     &      arr(i,1),arr(i,2),arr(i,3),arr(i,4),arr(i,5)
    
190   continue
      goto 999


200   n_polys = i-1
      write(*,*)'number of polygons =',n_polys
      goto 999

205   line_err = ((i-1)*j) + j
      write(*,*)'ERROR in SUB: read_all_polygon_data ; data file line='
     &,line_err
      stop
      goto 999


        
999   return
      end            
     
!***********************************************************************
!*** 10 ****************************************************************               


      subroutine sort_polygons_by_z
     &           (n,arr,
     &            sortedz,ordination,ordination_rev,height)

!     Purpose:
!      sort arr() by z values (z=arr(n,3))  
!      create translation arrays that store the original->sorted and 
!      sorted->orginal ordination     

!     Input:   n,arr
!     Return:  sortedz,ordination,ordination_rev,height

!     Calls:     none
!     Called by: main  
     
      implicit real*8 (a-h,o-z)     
      
      integer ordination(10000),ordination_rev(10000)
      real*8 arr(10000,5),sortedz(10000,5),temp(5)
      real*8 height,base
     
!   initialize new arrays
      sortedz(1:10000,1:5) = 0.d0
      temp(1:5) = 0.d0


!    copy original array to working array to preserve original
      do 90 j = 1,n  
       ordination(j) = j  
       do 80 jj = 1,5
        sortedz(j,jj) = arr(j,jj)
!        write(*,*)j,jj,arr(j,jj)
80     continue
90    continue
     
      
!     sort by z ; iflag is test of completion of sorting ; j is a counter ofnumber of passes           
      iflag = 0
     
100   do 190 i = 1,n-1      
       if(sortedz(i,3).gt.sortedz(i+1,3))then
        do 170 k = 1,5
         temp(k)        = sortedz(i  ,k)
         sortedz(i,k)   = sortedz(i+1,k)
         sortedz(i+1,k) = temp(k)
170     continue         
        iflag = 1
        
        itemp = ordination(i)
        ordination(i)=ordination(i+1)
        ordination(i+1)=itemp
       endif
190   continue
      
!     if a change was made, review the list again, else done
      if(iflag.ne.0)then
!      reset iflag      
       iflag = 0
       goto 100
      endif
  
!     determine reverse ordination  
      icounter = 1
300   do 390 i = 1,n
       if(ordination(i).eq.icounter)then
        ordination_rev(icounter) = i
        icounter = icounter + 1
        if(icounter.gt.n)then
         goto 400
        else
         goto 300
       endif
       endif
390   continue 

400   write(*,*)'sorting complete'  
      height = sortedz(n,3)

! DIAGNOSTICS BEGIN : Save This Demo
!      write(11,*)'regurgitate raw data'
!      do 740 i = 1,n
!       write(11,785)(arr(i,j),j=1,5)
!740   continue
!      write(11,*)'sorted data'
!      do 750 i = 1,n
!       write(11,785)(sortedz(i,j),j=1,5)
!750   continue
!      write(11,*)'reverse ordination'
!      do 760 i = 1,n
!       write(11,785)(sortedz(ordination_rev(i),j),j=1,5)
!760   continue
!785   format(5f9.4)
! DIAGNOSTICS END
 
      return
      end
!***********************************************************************   
!*** 11 **************************************************************** 

      subroutine create_poly_polar_az_array 
     &                (grid_res,
     &                  poly_polar_az_array,precision)
      
!   Note: This subroutine runs ONCE.
!   Purpose:  It creates an array of (polar,azimuthal) combinations
!     that will specify all of the directions in a polygon's hemispherical
!     field of view.  In subsequent subroutine calls, each of these 
!     directions will then be assigned a new set of polar and azimuthal 
!     angle values that define that same direction, but in coordinate 
!     systems of the human model.  These directionswill further be assigned 
!     another set polar and azimuthal angle values that define that same 
!     direction, but in the coordinate system of the geographic earth 
!     (i.e. North, East, South, and West).  And finally, an assignment of 
!     direction relative to the sun azimuth.
!     
!     input:     grid_res
!     returns:   poly_polar_az_array  
!
!     calls:     angleprecision 
!     called by: main
   
      implicit real*8 (a-h,o-z) 
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
     &       ,grid_res(2)
 
      real*8 pi,poly_polar_az_array(32400,2),precision

!   initialize new arrays
      poly_polar_az_array(1:32400,1:2) = 0.d0

      pi = dacos(-1.d0)

      
!     Note: grid_res(1) is the smallest polar angle [deg] that each poly can resolve
!           grid_res(2) is the smallest azimuthal angle [deg] that each poly can resolve 
      if( (grid_res(1).eq.1).or.(grid_res(1).eq.2 )
     &                      .or.(grid_res(1).eq.3 )
     &                      .or.(grid_res(1).eq.5 )
     &                      .or.(grid_res(1).eq.6 )
     &                      .or.(grid_res(1).eq.9 )
     &                      .or.(grid_res(1).eq.10) 
     &                      .or.(grid_res(1).eq.15) 
     &                      .or.(grid_res(1).eq.18)
     &                      .or.(grid_res(1).eq.30)  )then
       goto 100
      else
       grid_res(1)=15
      endif

100   if( (grid_res(2).eq.1).or.(grid_res(2).eq.2 )
     &                      .or.(grid_res(2).eq.3 )
     &                      .or.(grid_res(2).eq.5 )
     &                      .or.(grid_res(2).eq.6 )
     &                      .or.(grid_res(2).eq.9 )
     &                      .or.(grid_res(2).eq.10)
     &                      .or.(grid_res(2).eq.12)
     &                      .or.(grid_res(2).eq.15)  
     &                      .or.(grid_res(2).eq.18)  
     &                      .or.(grid_res(2).eq.20)  
     &                      .or.(grid_res(2).eq.24)
     &                      .or.(grid_res(2).eq.30)  
     &                      .or.(grid_res(2).eq.36)
     &                      .or.(grid_res(2).eq.40)
     &                      .or.(grid_res(2).eq.45)
     &                      .or.(grid_res(2).eq.60)
     &                      .or.(grid_res(2).eq.72)
     &                      .or.(grid_res(2).eq.90)  )then     
       goto 110
      else
       grid_res(2)=45
      endif
      
110   max_directions = int(( 90./dfloat(grid_res(1)))
     &                    *(360./dfloat(grid_res(2))))


!     Initialize array 
      do 150 k = 1,32400
        poly_polar_az_array(k,1) = 0.
        poly_polar_az_array(k,2) = 0.
150   continue


!     Integration of polygon will be over local polar and azimuthal angles     

      kcounter = 0
                          
      r = 1.d0 

      do 190 i = 0,89,grid_res(1)
      
       theta = dfloat(i) + dfloat(grid_res(1))/2.d0
       
       r_theta = pi*theta/180.
         
       do 170 j = 0,359,grid_res(2)
       
        phi  = dfloat(j) + dfloat(grid_res(2))/2.d0
       
        r_phi = pi*phi/180.  
        
        kcounter = kcounter+1

        poly_polar_az_array(kcounter,1) = r_theta
        poly_polar_az_array(kcounter,2) = r_phi
           
170    continue
190   continue 
      
      call angleprecision 
     &           (grid_res,
     &            precision)

999   return
      end      
!***********************************************************************  
!*** 11A ***************************************************************   

      subroutine angleprecision 
     &           (grid_res,
     &            precision)

!   NOTE:     This subroutine is called once
!   Purpose:  Determine the minimum precision of angles given grid_res

!   Input:   grid_res
!   Output:  precison [degrees]

!   Calls:      none
!   Called by: main

      implicit real*8 (a-h,o-z)
      integer grid_res(2)
      real*8 pi,pa,pb,apb,precision
      pi=dacos(-1.d0)


      pa =  (pi/180.d0)*dfloat(grid_res(1))
      pb =  (pi/180.d0)*dfloat(grid_res(1))
      apb = (pi/180.d0)*dfloat(grid_res(2))

      precision = (dacos(  ((dcos(pa)) * (dcos(pb))              )  
     &         +       ((dsin(pa)) * (dsin(pb)) * (dcos(apb)))  ))

      precision = (180.d0/pi)*precision

      return
      end
!*********************************************************************** 
!*** 12 **************************************************************** 

       subroutine compute_hsi_field
     & (height,apsza
     & ,saz,precision,sortedz,n_polys
     & ,body_coord_twist,body_geo_orient
     & ,grid_res,poly_polar_az_array
     & ,elev_asl
     & ,b,d
     & ,sundist_scale
     & ,epsilon,rho,rhohi,phi,phihi,atmpth,hssbi,hssdi,hssi)

!   Purpose: Calculate the horizontal surface irradiance field (hsi) at
!      each of the 91 polys making up the ground.  Thus hsi is a function
!      of the 27 wavebands. 

!   Input:   height,apsza,saz,precision,sortedz,n_polys,
!            body_coord_twist,body_geo_orient,grid_res,poly_polar_az_array,
!            elev_asl,b,d,sundist_scale
!           
!   Return:  epsilon,rho,rhohi,phi,phihiatmpth,hssbi,hssdi

!   Calls:     create_ground_poly_coords
!              ground_poly_obstruction_mask
!              rotate_2_sun
!              atmosphere_path
!              compute_drad

!   Called by: main


      implicit real*8 (a-h,o-z)
      integer grid_res(2),obstruct_g(181,360),epsilon(10,2),bandx
      integer ibeamflag,n_polys

      real*8 gr_poly(91,2)
      real*8 nssbi(27),hssbi(91,27),hssdi(91,27),hssi(91,27)
      real*8 hbi(91),hdi(91),hsi(91)
     
      real*8 apsza,saz,sortedz(10000,5)
      real*8 poly_polar_az_array(32400,2)     
      real*8 sun_polar_az_array(32400,2)
    
      real*8 height,rho(11),rholo(11),rhohi(10)
     &      ,phi(11,25),philo(11,25),phihi(11,25)     

      real*8 b(27,4),bvar1,bvar2,dvar1,dvar2,dvar3,dvar4
      real*8 d(27,13),drad(27)
    
      real*8 pi,rsaz,sdi,ab,bandwidth,atmpth

!   initialize the new arrays
      hbi(1:91) = 0.d0
      hdi(1:91) = 0.d0
      hsi(1:91) = 0.d0
      nssbi(1:27) = 0.d0

      drad(1:27) = 0.d0

      hssbi(1:91,1:27) = 0.d0
      hssdi(1:91,1:27) = 0.d0
      hssi(1:91,1:27) = 0.d0
      
      pi = dacos(-1.d0)

      rsaz = (pi/180.d0)*saz
      rapsza = (pi/180.d0)*apsza

      call create_ground_poly_coords
     &            (height,apsza
     &            ,gr_poly,epsilon,rho,rhohi,phi,phihi)


      do 590 m = 1,91

      ifrac = int((100.d0*(real(m)/91.d0))+0.5d0)
      if(mod(ifrac,20).eq.0)then
       write(*,*)'Ground Irrad. Field Calculation % Complete:',ifrac
      endif


!    calculate only one beam irradiance per surface
       mflag = 0

!   initialize arrays that are specific for for each m of the ground array
      nssbi(1:27) = 0.d0

      drad(1:27) = 0.d0

       x = gr_poly(m,1) * (dcos(gr_poly(m,2)))
       y = gr_poly(m,1) * (dsin(gr_poly(m,2)))

       ibeamflag = 0
       call ground_poly_obstruction_mask
     &            (rapsza,precision,sortedz,n_polys,x,y
     &            ,ibeamflag,obstruct_g)


       call rotate_2_sun
     &      (apsza,saz,m,sortedz,body_coord_twist,body_geo_orient
     &      ,grid_res,poly_polar_az_array
     &      ,sun_polar_az_array) 

       call atmosphere_path
     &     (apsza,elev_asl
     &      ,atmpth)
   

! direct irradiance variables:
       bvar1 = (dcos(pi*apsza/180.d0)) 
       bvar2 = (dexp(-atmpth))

! diffuse irradiance variable:
       pa  = (pi/180.d0)*apsza   

       dtheta = (pi/180.d0)*dfloat(grid_res(1))
       dphi =   (pi/180.d0)*dfloat(grid_res(2))

       kcounter = 0      
       do 490 i = 0,89,grid_res(1)
        do 470 j = 0,359,grid_res(2)
         kcounter = kcounter+1              
        
         ptheta = poly_polar_az_array(kcounter,1)
         pphi   = poly_polar_az_array(kcounter,2)
         stheta =  sun_polar_az_array(kcounter,1) 
         sphi   =  sun_polar_az_array(kcounter,2) 

!     itheta and iphi are indexes used only to address the obstruct array
         itheta = 1 + i
         iphi   = 1 + j


         if(stheta.lt.(pi/2.d0))then
!      1) ray points to sky
              
          ab  = dacos(  ((dcos(pa)) * (dcos(stheta))              )  
     &        +       ((dsin(pa)) * (dsin(stheta)) * (dcos(sphi)))  )

       
!      1A) calculate beam irradiance (beam_irrad)
          if( (ibeamflag.eq.0).and.(mflag.eq.0) )then

           mflag = 1 
           do 190 bandx = 1,27 
            hssbi(m,bandx) = 
     &       b(bandx,1)
     &     + b(bandx,2)*bvar1
     &     + b(bandx,3)*(bvar2**b(bandx,4))
            if(hssbi(m,bandx).lt.0.d0)then
             hssbi(m,bandx) = 0.d0
            endif
190        continue

          endif

!      1B) calculate Rayleigh-scattered sky irradiance 
         
          dvar1 = dexp(-atmpth)
          dvar2 = dexp(-ab )
          dvar3 = dsin( stheta )
          dvar4 = 1.d0 + ((dcos(ab))**2)
          call compute_drad
     &            (d,dvar1,dvar2,dvar3,dvar4,grid_res
     &            ,drad)
          do 290 bandx = 1,27

!         accumulate diffuse sky irradiance (sky_irrad) 
!         from diffuse sky radiance at (rtheta,rphi)       
           hssdi(m,bandx) =  hssdi(m,bandx)
     &     +   (1.d0/1.d0)
     &     *   (  (  dfloat(1-obstruct_g(itheta,iphi))          )
     &     *      ( (drad(bandx))                               )
     &     *      ( (dcos(ptheta))*(dsin(ptheta))*(dtheta*dphi) )  )
        

290       continue
                
         endif

470     continue
490    continue

       do 550 bandx = 1,27

!       Units of hssbi [mW/m2/nm]


            hssbi(m,bandx) = ((hssbi(m,bandx)) * sundist_scale)
            nssbi(bandx) = hssbi(m,bandx)/(dcos(pa))

!      Units of sky_irrad are [mW/m2/nm]
!       Convert to [W/m2/nm] and correct for earth-sun distance

             hssdi(m,bandx) = ((hssdi(m,bandx)) *sundist_scale)

550    continue     

       do 570 bandx = 1,27
        hssi(m,bandx) = hssbi(m,bandx) + hssdi(m,bandx)
        if(bandx.le.23)then
         bandwidth = 5.d0
        else
         bandwidth = 100.d0
        endif
        hbi(m) = hbi(m) + (bandwidth * hssbi(m,bandx))
        hdi(m) = hdi(m) + (bandwidth * hssdi(m,bandx))
570    continue

        hsi(m) =  hbi(m) + hdi(m)
!      write(11,*)'m,hbi(m),hdi(m),hsi(m)',m,hbi(m),hdi(m),hsi(m)

590   continue


999   return
      end
!***********************************************************************
!*** 12A *************************************************************** 

      subroutine create_ground_poly_coords
     &            (height,apsza
     &            ,gr_poly,epsilon,rho,rhohi,phi,phihi)

!   Purpose:  This sub generates 90 gridded ground polys, as identified by 
!      their centroid (rho,phi). Poly shape is that of an annulus segment.
!      All azimuth angles in this subroutine are (presumed to be) already 
!      in the sun-coord sys. h = model height (model = human/animal/plant 
!      3D graphic).  There is a 91st "imaginary" ground poly of infinite 
!      extent outside of the outermost defined (10th) annulus. This 91st 
!      poly has a surface irradiance equal to the standard "horizontal" 
!      surface irradiance (hsi).  "deltarho" is the maximum radial extent 
!      of the ground grid.  
!      The range of deltarho: 2*h at sza=0, to 4*h at sza>= 75.96376
!   Note: tan(75.96376) = 4.   So Gridded zone = shadow when shadow = 4*h

!   Input:   height,apsza
!   Return:  gr_poly,rho,rhohi,phi,phihi

!   Calls:     -
!   Called by: compute_hsi_field

      implicit real*8 (a-h,o-z)
      integer epsilon(10,2)
      real*8 height,apsza,gr_poly(91,2),rho(11),rholo(11),rhohi(10)
     &      ,phi(11,25),philo(11,25),phihi(11,25)

!   initialize new arrays
      epsilon(1:10,1:2) = 0.d0
      gr_poly(1:91,1:2) = 0.d0
      rho(1:11) = 0.d0
      rholo(1:11) = 0.d0
      rhohi(1:10) = 0.d0
      phi(1:11,1:25) = 0.d0
      philo(1:11,1:25) = 0.d0
      phihi(1:11,1:25) = 0.d0

      deltarho = 0.1d0*
     &(min((4.d0*height) 
     &  , ((4.d0*height) +2.d0*height*((apsza-75.96376)/75.96376))))

!    The ground polys are annulus segments, with lower/upper radial 
!    limits: rholo/rhohi ;  And lower/upper azimuthal limits: philo/phihi

      do 190 m = 1,10
       rholo(m) = (m-1)*deltarho
       rho(m)   = (deltarho/2.d0) + (m-1)*deltarho
       rhohi(m) = (m)*deltarho
190   continue
      
      epsilon(1,1) =   0     
      epsilon(1,2) =  24
      epsilon(2,1) = 120     
      epsilon(2,2) =  16
      epsilon(3,1) = 180     
      epsilon(3,2) =  12
      epsilon(4,1) = 240     
      epsilon(4,2) =   8
      epsilon(5,1) = 270     
      epsilon(5,2) =   6
      epsilon(6,1) = 285     
      epsilon(6,2) =   5
      epsilon(7,1) = 300     
      epsilon(7,2) =   4
      epsilon(8,1) = 315     
      epsilon(8,2) =   3
      epsilon(9,1) = 330     
      epsilon(9,2) =   2
      epsilon(10,1)= 345     
      epsilon(10,2)=   1
   

      do 290 m = 1,10
       philo(m,1)  = -(epsilon(m,1)/2.d0)
       phi(m,1)    =   0.d0
       phihi(m,1)  =  (epsilon(m,1)/2.d0)

       do 280 n = 2,1+epsilon(m,2)       
        philo(m,n)  = (epsilon(m,1)/2.d0)         + (n-2)*15.d0
        phi(m,n)    = (epsilon(m,1)/2.d0) + 7.5d0 + (n-2)*15.d0
        phihi(m,n)  = (epsilon(m,1)/2.d0)         + (n-1)*15.d0
280    continue
290   continue

      rho(11) = 1000.d0
      phi(11,1) =  0.d0

      icount = 0
      do 390 m = 1,10
       do 370 n = 1,1+epsilon(m,2)
        icount = icount + 1
!        write(11,355)rholo(m),rho(m),rhohi(m)
!     &             ,philo(m,n),phi(m,n),phihi(m,n)
!355     format(6f12.4)
        gr_poly(icount,1) = rho(m)
        gr_poly(icount,2) = phi(m,n)
370    continue
390   continue
      gr_poly(91,1) = 0.d0
      gr_poly(91,2) = 0.d0

999   return
      end
!***********************************************************************
!*** 12B *************************************************************** 

      subroutine ground_poly_obstruction_mask
     &            (rapsza,precision,sortedz,n,x,y
     &            ,ibeamflag,obstruct_g)
              
!      Purpose: From the point of view of ground poly at (x,y), determine 
!        the angles (theta,raz) in its view of the sky that are obstructed 
!        by model polygons. All ground polys have "z" = 0.  The output is 
!        an array "obstruct_g" that gives the status of the poly's skyview 
!        for discreet (integer) angles.

!     input:    apsza,saz,precision,sortedz,n,x,y
!     returns:  ibeamflag,obstruct_g

!     calls:     cart_2_sphericalpolar
!     called by: compute_hsi_field     
      
!     Note: n is the number of polygons in the graphic model.

      implicit real*8 (a-h,o-z)   
      
      integer obstruct_g(181,360),ibeamflag
      real*8 sortedz(10000,5)

!   initialize new arrays
      obstruct_g(1:181,1:360) = 0

      pi = dacos(-1.d0)
      ibeamflag = 0

      
!     initialize obstruct: note: theta ~ i-1 ; phi ~ ii-1
!      an "obstruct" value of 0 => NO obstruction in that direction
   
       if((x.eq.0.d0).and.(y.eq.0.d0))then
!    This is the "imaginary" poly at large dist from the model         
        goto 999
       endif

       do 190 j = 1,n  

!     test the ground poly against all model polys for obstruction of skyview
        deltax = (sortedz(j,1) - x)
        deltay = (sortedz(j,2) - y)
        deltaz = (sortedz(j,3) - 0.d0)
    
        if(deltaz.eq.0.d0)goto 190

150    call cart_2_sphericalpolar(deltax,deltay,deltaz,r,theta,phi)
 
       raz = dmod(  
     &  ( -body_coord_twist + dabs((360.d0+phi)-(saz-body_geo_orient)) )   
     &           ,360.d0 )

           
185    if(raz.lt.0.d0)then
        raz = 360.d0 + raz
        goto 185
       endif
       
       rraz = (pi/180.d0)*raz
       rtheta = (pi/180.d0)*theta
       apsza = (180.d0/pi)*rapsza

!    Check if direct sun beam is obstrcuted
       rsun2poly = (dacos(  ((dcos(rapsza)) * (dcos(rtheta))        )  
     &    +   ((dsin(rapsza)) * (dsin(rtheta)) * (dcos(rraz)))  ))

       sun2poly = (180.d0/pi)*rsun2poly
        
!       write(11,188)apsza,theta,raz,precision,sun2poly
188     format(5f21.2)
       if(sun2poly.le.(precision/2.d0))then
        ibeamflag = 1
!        write(11,*)'we have beam obstrcution'
       endif
  
        izen = int(theta + 0.5d0)
        iraz = mod(int(phi + 0.5d0 ),360)
           
!      record the obstruction info             
        i  = izen + 1
        ii = iraz + 1
        
        obstruct_g(i,ii) = 1
190    continue         
 
999   return
      end
  
!***********************************************************************
!*** 12Bi & 13A & 15Bi ************************************************* 
      subroutine cart_2_sphericalpolar
     &           (x,y,z,
     &            r,theta,phi)

!     Purpose: convert cartesian coordinates to spherical polar coords

!     Input:   x,y,z
!     Return:  r,theta,phi

!     Calls:     none

!     Called by: ground_poly_obstruction_mask
!                polygon_obstruction_mask
!                match_ray_to_ground_poly 
          

      implicit real*8 (a-h,o-z)   
      real*8 pi,x,y,z,r,theta,phi
   
      pi = dacos(-1.d0)

      r = sqrt((x**2) + (y**2) + (z**2))
     
      if(r.ne.0)then
       r_theta = dacos(z/r)
     
       goto 60
      else
       r_theta = 0.d0
       r_phi = 0.d0
       goto 100
      endif



60    if(x.lt.0.)then
       r_phi = pi + datan(y/x)
       goto 100
      elseif((x.gt.0.).and.(y.ge.0))then
       r_phi =  datan(y/x)
       goto 100
      elseif((x.gt.0.).and.(y.lt.0))then
       r_phi = 2.*pi + datan(y/x)
       goto 100 
      endif
      
      if((x.eq.0).and.(y.gt.0.))then
       r_phi = pi/2.
       goto 100
      elseif((x.eq.0.).and.(y.lt.0.))then
       r_phi = + 3.*pi/2.
       goto 100
      elseif((x.eq.0.).and.(y.eq.0.))then
       r_phi = 0.d0

      endif
        
100   theta = (180.d0/pi) *r_theta
      phi   = (180.d0/pi) *r_phi
      
      if(dabs(theta-360.d0).le.0.05)then 
       theta = 0.d0
      endif
      if(dabs(phi - 360.d0).le.0.05)then 
       phi = 0.d0
      endif
      
      if((dabs(theta).le.0.05).or.(dabs(theta-180.0).le.0.05))then
       phi = 0.d0
      endif
      
999   return
      end
!*********************************************************************** 
!*** 12C *************************************************************** 

      subroutine atmosphere_path
     &             (apsza,z
     &             ,atmpth)

!   Purpose: calculate the pathlength of sunbeam through atm to observer

!   Input:   apsza,z
!   Return:  atmpth

!   Calls:     - 
!   Called by: compute_hsi_field

      implicit real*8 (a-h,o-z)    
      real*8 pi,apsza,rapsza,z,atmpth,radearth,base_atm,height,horizon
     &       ,hatm,term1,term2,term3,term4,test

      pi = dacos(-1.d0)

!   units are kilometers
      radearth = 6376.d0
      base_atm =  100.d0

      height =  radearth + z
      hatm     = base_atm -  z

      horizon = 180.d0 - ((180.d0/pi)*dasin(radearth/height))
      rapsza = (pi/180.d0)*apsza      
     
      term1 = (height + hatm)**2
      term2 = (radearth)**2
      term3 = (radearth*dcos(rapsza))**2
      term4 =  radearth*dcos(rapsza)

      test = term1 - term2 + term3
      if(test.lt.0.)then
       write(*,*)'negative square root in atmosphere_path'
      endif
      atmpth = (dsqrt(term1 -term2 +term3)) - term4

!     normalize
      atmpth = atmpth/base_atm  
    
    
      return
      end
!***********************************************************************
!*** 12D & 15B ********************************************************* 
      subroutine compute_drad
     &            (d,dvar1,dvar2,dvar3,dvar4,grid_res
     &            ,drad)

!     Note: This subroutine is called once for each polygon
!     Purpose: For a given apsza, 
!              determine the array of radiance values 
!                   (at all view directions:90 x 360) at 27 bands  

!     Input:  d.dvar,apsza,sun_polar_az_array,diffuse_coeff,grid_res
!     Return: drad

!     Calls:      none
!     Called by:  compute_hsi_field
!                 spectral_irradiance


      implicit real*8 (a-h,o-z)     
      integer grid_res(2)

      real*8 d(27,13)     
      real*8 dvar1,dvar2,dvar3,dvar4
      real*8 drad(27)
 
!   initialize new arrays
      drad(1:27) = 0.d0

      pi = dacos(-1.d0)
         
      do 590 n = 1,27

       if((n.ge.1).and.(n.le.8))then

        drad(n) = abs(
     & d(n,1) 
     &+d(n,2)*(dvar1**d(n,3))*(dvar2**d(n,4)) 
     &+d(n,5)*(dvar1**d(n,6))*(dvar4**d(n,7)) )

         elseif((n.ge.9).and.(n.le.23))then
   
       drad(n) = abs(
     & d(n,1) 
     &+d(n,2)*(dvar1**d(n,3))*((1.d0-dtanh(-dlog(dvar2)))**d(n,4))
     &+d(n,5)*(dvar1**d(n,6))*(dvar3**d(n,7)) 
     &+d(n,8)*(dvar1**d(n,9))*(dvar4**d(n,10)) )

         elseif((n.ge.24).and.(n.le.27))then
    
       drad(n) = abs(
     & d(n,1) 
     &+d(n,2) *(dvar1**d(n,3))*((1.d0-dtanh(-dlog(dvar2)))**d(n,4)) 
     &+d(n,5) *(dvar1**d(n,6))*(dvar3**d(n,7)) 
     &+d(n,8) *(dvar1**d(n,9))*(dvar2**d(n,10))
     &+d(n,11)*(dvar1**d(n,12))*(dvar4**d(n,13)) )

       endif

590   continue             
               
999   return
      end
!***********************************************************************   
!*** 13 ****************************************************************               
  

      subroutine polygon_obstruction_mask
     &   (apsza,saz,sortedz,n_polys,ms,precision
     &   ,body_coord_twist,body_geo_orient
     &   ,obstruct,ibeamflag,irbeamflag)
          
    
!   Purpose:  From the point of view of polygon m, determine the angles 
!     (theta,raz) in m's view of the sky that are obstructed by other 
!      polygons.  Only polygons that are at higher "z" than m can obstruct 
!      m's view of the sky. The output is an array "obstruct" that gives 
!      the status of m's sky view for discreet (integer) angles.

!   Input:    apsza,saz,sortedz,n,m,body_coord_twist,body_geo_orient

!   Returns:  obstruct,ibeamflag,irbeamflag

!   Calls:     cart_2_spherical_polar
!   Called by: main     
      
!     n is the actual number of polygons in the model
!     m is the particlar polygon being examined : note m = 1,n-1      


      implicit real*8 (a-h,o-z)
      integer obstruct(181,360),ms      
      real*8 apsza,saz,precision,body_coord_twist,body_geo_orient
      real*8 sortedz(10000,5)

!   initialize new arrays
      obstruct(1:181,1:360) = 0


      pi = dacos(-1.d0)   
      ibeamflag = 0
      irbeamflag = 0

!     initialize obstruct: note: theta ~ i-1 ; phi ~ ii-1
!      an "obstruct" value of 0 => NO obstruction in that direction

   
!     cycle through all polygon pairs: (1,1+1),(1,1+2),...,(m,m+1),...(n-1,n)
      
!     compare with a second polygon   
!     polys with j<m are at lower z ;  polys with j>m are at higher z   
 
      do 190 j = 1,n_polys
       if(j.eq.ms)then
        goto 190
       endif

!     test each successive poly against all other polys of higher z for obstruction of sky view
!     and lower z for obstruction of the ground

       deltax = (sortedz(j,1) - sortedz(ms,1))
       deltay = (sortedz(j,2) - sortedz(ms,2))
       deltaz = (sortedz(j,3) - sortedz(ms,3))

       if(deltaz.eq.0.d0)goto 190

150    call cart_2_sphericalpolar(deltax,deltay,deltaz,r,theta,phi)
 
       raz = dmod(  
     &  ( -body_coord_twist + dabs((360.d0+phi)-(saz-body_geo_orient)) )   
     &           ,360.d0 )
           
!      write(11,*)ms,raz
185    if(raz.lt.0.d0)then
        raz = 360.d0 + raz
        goto 185
       endif
       

       rraz = (pi/180.d0)*raz
       rtheta = (pi/180.d0)*theta
       rapsza = (pi/180.d0)*apsza

!    Check if direct sun beam is obstrcuted
       sun2poly = (dacos(  ((dcos(rapsza)) * (dcos(rtheta))        )  
     &    +   ((dsin(rapsza)) * (dsin(rtheta)) * (dcos(rraz)))  ))

       sun2poly = (180.d0/pi)*sun2poly

!    Check if reflected direct sun beam is obstructed
       rsun2poly =  (dacos(  ((dcos(pi - rapsza)) * (dcos(rstheta))  )  
     &  +  ((dsin(pi - rapsza)) * (dsin(rstheta)) * (dcos(rraz)))  ))

       rsun2poly = (180.d0/pi)*rsun2poly

       
       if(sun2poly.le.(precision/2.d0))then
        ibeamflag = 1
!        write(11,*)'we have beam obstrcution',ms,sun2poly,precision
       endif
       if(rsun2poly.le.(precision/2.d0))then
        irbeamflag = 1
!        write(11,*)'we have reflected beam obstrcution',ms,rsun2poly
!     &    ,precision
       endif


       izen = int(theta + 0.5d0)
       iraz = mod(int(raz + 0.5d0 ),360)
     
     
!      record the obstruction info             
       i  = izen + 1
       ii = iraz + 1
        
       obstruct(i,ii) = 1

!  The matrix obstruct is unique to each polygon, and is referenced 
!  to a horizontal surface and to a "sun-relative" azimuth system, 
!  where an azimuthal directionidentical to the sun is defined as 
!  zero, and is positive CW (looking from (0,0,1) to (0,0,0).       

        
190   continue
 
     
999   return
      end 
!***********************************************************************       
!*** 14 & 12C **********************************************************               
           

      subroutine rotate_2_sun
     &      (apsza,saz,ms,sortedz,body_coord_twist,body_geo_orient
     &      ,grid_res,poly_polar_az_array,sun_polar_az_array) 

!  Purpose: 1    convert local r',theta',phi' [90 polar x 360 azimuthal] 
!                to local [x?,y?,z']
!           2    reports the coordinates of the vector in reference 
!                (hor surf)system
!           3    convert from x,y,z to r,theta,phi 

!     Input:   apsza, saz,ms,sortedz, body_coord_twist, body_geo_orient,
!              obstruct, grid_res, poly_polar_az_array 

!     Return: sun_polar_az_array

!     Calls:         spherpol_2_cart_for_arrays, rotation_in_3D,
!                    cart_2_spherpol_for_arrays,body_reference_frame
!                    geographic_reference_frame, sun_reference_frame

!     Called by: compute_hsi_field
!                main
    
      
      implicit real*8 (a-h,o-z)
      integer bandx, apszax, zenx, azx, delta_polar,delta_azimuthal
      integer grid_res(2),delta_theta,delta_phi   
     
      real*8 apsza,saz,sortedz(10000,5),rad(27),band(27)
      real*8 poly_polar_az_array(32400,2),poly_cartesian_array(32400,3)
      real*8 hor_cartesian_array(32400,3),hor_polar_az_array(32400,2)
      real*8 body_polar_az_array(32400,2),geo_polar_az_array(32400,2)
      real*8 sun_polar_az_array(32400,2),refl_polar_az_array(32400,2)

      pi = dacos(-1.d0)
 
      slope = sortedz(ms,4)
      aspect = sortedz(ms,5)

      
      max_directions = int(( 90./dfloat(grid_res(1)))
     &                    *(360./dfloat(grid_res(2))))
   
    
        call spherpol_2_cart_for_arrays
     &  (poly_polar_az_array,grid_res,poly_cartesian_array)

       
        call rotation_in_3D
     &  (poly_cartesian_array,grid_res,slope,aspect,hor_cartesian_array)


        call cart_2_spherpol_for_arrays
     &  (hor_cartesian_array,grid_res,hor_polar_az_array)


        call body_reference_frame
     &  (hor_polar_az_array,grid_res,body_coord_twist
     &   ,body_polar_az_array)


        call geo_reference_frame 
     & (body_polar_az_array,grid_res,body_geo_orient,geo_polar_az_array)


        call sun_reference_frame
     &  (geo_polar_az_array,grid_res,saz,sun_polar_az_array)
     

999   return
      end 

!***********************************************************************
!*** 14A & 12Ci ******************************************************** 

      subroutine spherpol_2_cart_for_arrays
     &           (poly_polar_az_array,grid_res,
     &            poly_cartesian_array)

!     Purpose:

!     Input:   poly_polar_az_array,grid_res
!     Return:  poly_cartesian_array

!     Calls:     none
!     Called by: rotate_2_sun 
      
      implicit real*8 (a-h,o-z)      
   
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
     
      real*8 poly_polar_az_array(32400,2),poly_cartesian_array(32400,3)
      
!   initialize new arrays
      poly_cartesian_array(1:32400,1:3) = 0.d0

      pi = dacos(-1.d0)
      r = 1.d0 
      
110   kcounter = 0
      
      do 190 i = 0,89,grid_res(1)
       do 170 j = 0,359,grid_res(2)
      
        kcounter = kcounter+1
        
        r_theta = poly_polar_az_array(kcounter,1)
        r_phi   = poly_polar_az_array(kcounter,2) 
       
        
        poly_cartesian_array(kcounter,1) = r*dsin(r_theta)*dcos( r_phi)
        poly_cartesian_array(kcounter,2) = r*dsin(r_theta)*dsin( r_phi)
        poly_cartesian_array(kcounter,3) = r*dcos(r_theta)       
   
170    continue
190   continue           
      
      return
      end
     
!***********************************************************************
!*** 14B 12Cii *********************************************************                 
  

      subroutine rotation_in_3D 
     &           (poly_cartesian_array,grid_res,slope,aspect
     &           ,hor_cartesian_array)

!     Purpose:

!     Input:    poly_cartesian_array,grid_res,slope,aspect
!     Return:   hor_cartesian_array

!     Calls:     none
!     Called by: rotate_2_sun 

   
      implicit real*8 (a-h,o-z)
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
     
      real*8 poly_cartesian_array(32400,3),hor_cartesian_array(32400,3)
     &      ,slope,aspect,r_slope,r_aspect,pi
      
!   initialize new arrays
      hor_cartesian_array(1:32400,1:3) = 0.d0

      pi = dacos(-1.d0)
      
      r_slope  = pi*slope/180.d0
      r_aspect = pi*aspect/180.d0     
             
      kcounter = 0
      
      do 190 i = 0,89,grid_res(1)
         
       do 170 j = 0,359,grid_res(2)
      
        kcounter = kcounter+1
        
        x = poly_cartesian_array(kcounter,1) 
        y = poly_cartesian_array(kcounter,2) 
        z = poly_cartesian_array(kcounter,3)        
      

!  [H] = [G]**-1 = [Z]**-1  x  [P]**-1 
35    h11 =  dcos(r_slope) * dcos(r_aspect)
      h12 = -dsin(r_aspect) 
      h13 =  dsin(r_slope) * dcos(r_aspect)
        h21 =  dcos(r_slope) * dsin(r_aspect)
        h22 =  dcos(r_aspect) 
        h23 =  dsin(r_slope) * dsin(r_aspect) 
          h31 = -dsin(r_slope)
          h32 =  0.d0
          h33 =  dcos(r_slope)
     
      hor_cartesian_array(kcounter,1) = h11*x + h12*y + h13*z     
      hor_cartesian_array(kcounter,2) = h21*x + h22*y + h23*z   
      hor_cartesian_array(kcounter,3) = h31*x + h32*y + h33*z             
        
170    continue
190   continue       
     
      return
      end

!***********************************************************************
!*** 14C & 12Ciii ****************************************************** 
         
      subroutine cart_2_spherpol_for_arrays
     &           (hor_cartesian_array,grid_res,hor_polar_az_array)
!     Purpose:

!     Input:  hor_cartesian_array,grid_res
!     Return: hor_polar_az_array

!     Calls:      none
!     Called by: rotate_2_sun
      
      
      implicit real*8 (a-h,o-z)     
     
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
    
      real*8 hor_polar_az_array(32400,2),hor_cartesian_array(32400,3)
      
!   initialize new arrays
      hor_polar_az_array(1:32400,1:2) = 0.d0

      pi = dacos(-1.d0)

      kcounter = 0
      
      do 190 i = 0,89,grid_res(1)
       do 170 j = 0,359,grid_res(2)
      
        kcounter = kcounter+1
        
        x =  hor_cartesian_array(kcounter,1)
        y =  hor_cartesian_array(kcounter,2)
        z =  hor_cartesian_array(kcounter,3)

        r = sqrt(x**2 + y**2 + z**2)

        if(r.ne.0)then
         r_theta = dacos(z/r)
         goto 60
        else
         r_theta = 0.d0
         r_phi = 0.d0
         goto 100
        endif
        
60      if(x.lt.0.)then
         r_phi = pi + datan(y/x)
         goto 100
        elseif((x.gt.0.).and.(y.ge.0))then
         r_phi =  datan(y/x)
         goto 100
        elseif((x.gt.0.).and.(y.lt.0))then
         r_phi = 2.*pi + datan(y/x)
         goto 100 
        endif
      
        if((x.eq.0).and.(y.gt.0.))then
         r_phi = pi/2.
         goto 100
        elseif((x.eq.0.).and.(y.lt.0.))then
         r_phi = + 3.*pi/2.
         goto 100
        elseif((x.eq.0.).and.(y.eq.0.))then
         r_phi = 0.d0
        endif

!       clean up any angles that should be zero        
100     if(dabs(r_theta - (2.d0*pi)).le.0.000001)then 
         r_theta = 0.d0
        endif
        if(dabs(r_phi   - (2.d0*pi)).le.0.000001)then 
         r_phi = 0.d0
        endif
      
        if(   (dabs(r_theta   ).le.0.000001)
     &    .or.(dabs(r_theta-pi).le.0.000001))then
         r_phi = 0.d0
        endif
        
        hor_polar_az_array(kcounter,1) = r_theta  
        hor_polar_az_array(kcounter,2) = r_phi  
        
170    continue
190   continue          
      
      return
      end
      
!***********************************************************************
!*** 14D & 12Civ *******************************************************               

      subroutine body_reference_frame
     &           (hor_polar_az_array,grid_res,body_vector,
     &            body_polar_az_array)
!     Purpose: If the cartesian grid of the body is not alligned with the
!       "facing" direction

!     Input:  hor_polar_az_array,grid_res,body_vector
!     Return: body_polar_az_array

!     Calls:     none
!     Called by: rotate_2_sun  
            
      implicit real*8 (a-h,o-z)   
     
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
    
      real*8 hor_polar_az_array(32400,2),body_polar_az_array(32400,2)

!   initialize new arrays
      body_polar_az_array(1:32400,1:2) = 0.d0

      pi = dacos(-1.d0)
      r_body_vector = pi*body_vector/180.d0
      
       
      kcounter = 0
      
      do 190 i = 0,89,grid_res(1)
       do 170 j = 0,359,grid_res(2)
      
        kcounter = kcounter+1              
        
        r_theta = hor_polar_az_array(kcounter,1)
        r_phi   = hor_polar_az_array(kcounter,2) 
        
        body_polar_az_array(kcounter,1) = r_theta 
        temp =   r_phi - r_body_vector
  
        if(temp.ge.0.d0)then
         body_polar_az_array(kcounter,2) = temp
        elseif(temp.lt.0.d0)then
         body_polar_az_array(kcounter,2) = (2.d0*pi) + temp
        endif

    
170    continue
190   continue         
     
999   return
      end      
!***********************************************************************
!*** 14E & 12Cv ********************************************************           

      subroutine geo_reference_frame
     &       (body_polar_az_array,grid_res,body_geo_orient
     &        ,geo_polar_az_array)

!     Purpose: The body_geo_orient is the geographic direction that the 
!       body is facing

!     Input:   body_polar_az_array,grid_res,body_geo
!     Return:  geo_polar_az_array

!     Calls:        None
!     Called by: rotate_2_sun
           
      implicit real*8 (a-h,o-z)    
     
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
    
      real*8 body_polar_az_array(32400,2),geo_polar_az_array(32400,2)

!   initialize new arrays
      geo_polar_az_array(1:32400,1:2) = 0.d0

      pi = dacos(-1.d0)
      
      r_body_geo_orient = pi*body_geo_orient/180.d0
      
      kcounter = 0

      do 190 i = 0,89
       do 170 j = 0,359
      
        kcounter = kcounter+1              
        
        r_theta = body_polar_az_array(kcounter,1)
        r_phi   = body_polar_az_array(kcounter,2) 

        geo_polar_az_array(kcounter,1) = r_theta 


        temp = ( r_body_geo_orient - r_phi)

        if    (temp.ge.0.d0)      then
          geo_polar_az_array(kcounter,2) =              temp

        elseif(temp.lt.0.d0)      then 
          geo_polar_az_array(kcounter,2) = (+2.d0*pi) + temp

        
        endif

!       correct the "azimuth wrt body_coord_twist & CCW" to "azimuth wrtgeo_orient & CW"     

170    continue
190    continue   
     
999   return
      end        
!***********************************************************************
!*** 14F & 12Cvi ******************************************************* 
               
      subroutine sun_reference_frame
     &           (geo_polar_az_array,grid_res,saz,
     &            sun_polar_az_array)

!     Purpose:

!     Input:  geo_polar_az_array,grid_res,saz
!     Return: sun_polar_az_array

!     Calls:     none
!     Called by: rotate_2_sun 
          
      implicit real*8 (a-h,o-z)         
     
     
      integer delta_polar,delta_azimuthal,delta_theta,delta_phi
      integer grid_res(2)      
    
      real*8 geo_polar_az_array(32400,2),sun_polar_az_array(32400,2)

!   initialize new arrays
      sun_polar_az_array(1:32400,1:2) = 0.d0

      pi = dacos(-1.d0)
      r_saz = pi*saz/180.d0
             
      kcounter = 0
      
      do 190 i = 0,89,grid_res(1)
       do 170 j = 0,359,grid_res(2)
      
        kcounter = kcounter+1              
        
        r_theta = geo_polar_az_array(kcounter,1)
        r_phi   = geo_polar_az_array(kcounter,2) 
        
        sun_polar_az_array(kcounter,1) = r_theta 

        temp = (r_phi - r_saz)

        if    ((temp.ge.(-pi))     .and.(temp.lt.      pi)) then
          sun_polar_az_array(kcounter,2) =              temp

        elseif((temp.ge.(+pi))     .and.(temp.lt.(2.d0*pi)))then
          sun_polar_az_array(kcounter,2) = (-2.d0*pi) + temp

        elseif((temp.gt.(-2.d0*pi)).and.(temp.lt.    (-pi)))then
          sun_polar_az_array(kcounter,2) = (+2.d0*pi) + temp
         
        else
           write(*,*)'error in 10Avi'

        endif
              
      
170    continue
190    continue         
     
999   return
      end
       
!***********************************************************************   
!*** 15 **************************************************************** 

      subroutine spectral_irradiance
     & (sundist_scale,apsza,saz,atmpth,reflectance,fresnel_flag
     & ,grid_res,precision,poly_polar_az_array,sun_polar_az_array
     & ,body_coord_twist,body_geo_orient,ibeamflag,irbeamflag,obstruct
     & ,sortedz,ms,epsilon,rho,rhohi,phi,phihi,hssi,hssdi,hssbi
     & ,b,d
     & ,beam_irrad,sky_irrad,rbeam_irrad,rsky_irrad,g_irrad)


!     Purpose: Calculate the 5 components of spectral irradiance on a poly

!     Input:   sundist_scale,apsza,saz,elev_asl,reflectance
!              grid_res,precision,poly_polar_az,array,sun_polar_az_array
!              obstruct,pt,epsilon,rho,rhohi,phi,phihi,hsi
!              b,d
!     Return:  beam_irrad,sky_irrad,rbeam_irrad,rsky_irrad,g_irrad
!              tot_down_irrad,tot_up_irrad,tot_irrad
!              tot_beam_irrad,tot_rayleigh_irrad)

!     Calls:   beam_incident_angle
!              compute_drad
!              match_ray_to_ground_poly
                

!     Called by: main


      implicit real*8 (a-h,o-z)
    

      integer grid_res(2),obstruct(181,360),epsilon(10,2),bandx
      integer ibeamflag,fresnel_flag

      real*8 gr_poly(91,2)
      real*8 hssbi(91,27),hssdi(91,27),hssi(91,27)
 
     
      real*8 apsza,saz,sortedz(10000,5)
      real*8 poly_polar_az_array(32400,2)     
      real*8  sun_polar_az_array(32400,2)
    
      real*8 h,rho(11),rholo(11),rhohi(10)
     &      ,phi(11,25),philo(11,25),phihi(11,25)     

      real*8 b(27,4),bvar1,bvar2,dvar1,dvar2,dvar3,dvar4
      real*8 d(27,13),drad(27)
    
      real*8 pi,rsaz,sdi,ab,bandwidth,atmpth

      real*8 reflectance(27,2)
    

      real*8 beam_irrad(27),sky_irrad(27),rbeam_irrad(27),rsky_irrad(27)
     &    ,g_irrad(27)

      real*8 dtheta,dphi,grad(27),ptheta,pphi,incidentang


!   initialize new arrays
      drad(1:27) = 0.d0 
      grad(1:27) = 0.d0

      sky_irrad(1:27) = 0.d0
      beam_irrad(1:27) = 0.d0
      rsky_irrad(1:27) = 0.d0
      rbeam_irrad(1:27) = 0.d0
      g_irrad(1:27) = 0.d0


      pi = dacos(-1.d0)

     
!    calculate only one beam irradiance per poly
       mflag = 0
!    calculate only one reflected beam irradiance per poly
       mrflag = 0

   
! direct irradiance variables:
       bvar1 = (dcos(pi*apsza/180.d0)) 
       bvar2 = (dexp(-atmpth))
     
! diffuse irradiance variable:
       pa  = (pi/180.d0)*apsza   

       dtheta = (pi/180.d0)*dfloat(grid_res(1))
       dphi =   (pi/180.d0)*dfloat(grid_res(2))


       kcounter = 0      
       do 970 i = 0,89,grid_res(1)
        do 950 j = 0,359,grid_res(2)
         kcounter = kcounter+1              
        
         ptheta = poly_polar_az_array(kcounter,1)
         pphi   = poly_polar_az_array(kcounter,2)
         stheta =  sun_polar_az_array(kcounter,1) 
         sphi   =  sun_polar_az_array(kcounter,2) 
       
!     itheta and iphi are indexes used only to address the obstruct array
         itheta = 1 + i
         iphi   = 1 + j
     

         if(stheta.lt.(pi/2.d0))then
!      1) ray points to sky
         
          ab  = dacos(  ((dcos(pa)) * (dcos(stheta))              )  
     &        +       ((dsin(pa)) * (dsin(stheta)) * (dcos(sphi)))  )
          
!      1A) calculate beam irradiance (beam_irrad)
          if( (ibeamflag.eq.0).and.(mflag.eq.0) )then
              
           mflag = 1

           slope = sortedz(ms,4)
           aspect = body_geo_oreint + (sortedz(ms,5) - body_coord_twist) 

           call beam_incident_angle
     &        (slope,aspect,apsza,saz
     &        ,incidentang)
          
           if(incidentang.ge.90.d0)then
            do 180 bandx = 1,27
             beam_irrad(bandx) = 0.d0           
180         continue
            goto 200
           endif

           do 190 bandx = 1,27 
            beam_irrad(bandx) = dcos((pi/180.d0)*incidentang)
     &   *(  b(bandx,1)
     &     + b(bandx,2)*bvar1
     &     + b(bandx,3)*(bvar2**b(bandx,4)) )

            if(beam_irrad(bandx).lt.0.d0)then
             beam_irrad(bandx) = 0.d0
            endif

190        continue

          endif

!      1B) calculate Rayleigh-scattered sky irradiance 
         
200       dvar1 = dexp(-atmpth)
          dvar2 = dexp(-ab )
          dvar3 = dsin( stheta )
          dvar4 = 1.d0 + ((dcos(ab))**2)
         
          call compute_drad
     &            (d,dvar1,dvar2,dvar3,dvar4,grid_res
     &            ,drad)
          
          do 290 bandx = 1,27
        
!         accumulate diffuse sky irradiance (sky_irrad) 
!         from diffuse sky radiance at (rtheta,rphi)       
           sky_irrad(bandx) =  sky_irrad(bandx)
     &     +   (1.d0/1.d0)
     &     *   (  (  dfloat(1-obstruct(itheta,iphi))          )
     &     *      ( (drad(bandx))                               )
     &     *      ( (dcos(ptheta))*(dsin(ptheta))*(dtheta*dphi) )  )
           
290       continue
                

        elseif(stheta.ge.(pi/2.d0))then
!      2) ray points to ground

!      2A)calculate Snell-relfected beam irrad 

          if( (irbeamflag.eq.0).and.(mrflag.eq.0) )then
        
           mrflag = 1 

           slope = sortedz(ms,4)
           aspect = body_geo_oreint + (sortedz(ms,5) - body_coord_twist) 
           refl_apsza = 180.d0 - apsza
           call beam_incident_angle
     &        (slope,aspect,refl_apsza,saz
     &        ,incidentang)
          
           if(incidentang.ge.90.d0)then
            do 380 bandx = 1,27
             beam_irrad(bandx) = 0.d0           
380         continue
            goto 400 
           endif

           do 390 bandx = 1,27 
            rbeam_irrad(bandx) = dcos((pi/180.d0)*incidentang)
     &   * ( b(bandx,1)
     &     + b(bandx,2)*bvar1
     &     + b(bandx,3)*(bvar2**b(bandx,4)) )

         
            if(rbeam_irrad(bandx).lt.0.d0)then
             rbeam_irrad(bandx) = 0.d0
            endif

!       correct for SL parameter and %reflectance
            rbeam_irrad(bandx) = rbeam_irrad(bandx)
     &                        *reflectance(bandx,1)
     &                        *reflectance(bandx,2)

!       correct for loss due to transmission into water            
             if(fresnel_flag.eq.1)then
               if(pa.eq.0.d0)then
                fresnel_refl = 0.020d0
                goto 385
               endif
              ta = dasin( (1.d0/1.33d0)*dsin(pa) )
              Rn = ((dsin(pa - ta))/(dsin(pa + ta)))**2
              Rp = ((dtan(pa - ta))/(dtan(pa + ta)))**2
              fresnel_refl = (Rn + Rp)/2.d0
385           rbeam_irrad(bandx) = rbeam_irrad(bandx) * fresnel_refl 
             endif

390        continue
          endif        


!      2B) calculate Snell-reflected Rayleigh-scattered sky radiance array 

400       xtheta = pi-stheta

          ab  = dacos(  ((dcos(pa)) * (dcos(xtheta))              )  
     &        +       ((dsin(pa)) * (dsin(xtheta)) * (dcos(sphi)))  )
          dvar1 = dexp(-atmpth)
          dvar2 = dexp(-ab )
          dvar3 = dsin( stheta )
          dvar4 = 1.d0 + ((dcos(ab))**2)
          call compute_drad
     &            (d,dvar1,dvar2,dvar3,dvar4,grid_res
     &            ,drad)

!      accumulate Snell-reflected Rayleigh-scattered sky irradiance 
          do 490 bandx = 1,27
!       scale the reflected radiance by SL parameter and %reflectance
            drad(bandx) = drad(bandx)*reflectance(bandx,1)
     &                             *reflectance(bandx,2)

!       correct for loss due to transmission into water            
             if(fresnel_flag.eq.1)then
               if(xtheta.eq.0.d0)then
                fresnel_refl = 0.020d0
                goto 485
               endif
              ta = dasin( (1.d0/1.33d0)*dsin(xtheta) )
              Rn = ((dsin(xtheta - ta))/(dsin(xtheta + ta)))**2
              Rp = ((dtan(xtheta - ta))/(dtan(xtheta + ta)))**2
              fresnel_refl = (Rn + Rp)/2.d0
485           drad(bandx) = drad(bandx) * fresnel_refl 
             endif


           rsky_irrad(bandx) =  rsky_irrad(bandx)
     &     +   (1.d0/1.d0)
     &     *   (  (  dfloat(1-obstruct(itheta,iphi))          )
     &     *      ( (drad(bandx))                               )
     &     *      ( (dcos(ptheta))*(dsin(ptheta))*(dtheta*dphi) )  )
  
490       continue

!      2C) calc Lambert-scattered ground radiance array 

          call match_ray_to_ground_poly
     &       (sortedz,ms,stheta,sphi
     &       ,epsilon,rho,rhohi,phi,phihi
     &       ,ix)

         
!    scale the Lambertian-reflected radiance by (1-SL) and %reflectance
          do 590 bandx = 1,27
           grad(bandx) = (1.d0/pi)*(hssi(ix,bandx))
     &                            *        reflectance(bandx,1)
     &                            *(1.d0 - reflectance(bandx,2))
        
   
!     accumulate Lambert-reflected Horizontal Surface Irradiance (Lambert_irrad)
           g_irrad(bandx) = g_irrad(bandx)
     &     +   (1.d0/1.d0)
     &     *   (  (  dfloat(1-obstruct(itheta,iphi))            )
     &     *      ( (grad(bandx))                               )
     &     *      ( (dcos(ptheta))*(dsin(ptheta))*(dtheta*dphi) )  )
590       continue        

        endif

950    continue
970   continue
    

       do 1550 bandx = 1,27

!       Units of irradiances are [mW/m2/nm]
!        Must div by 1000 to get [W/m2/nm]
!       Also correct for date-specific earth-sun distance

         beam_irrad(bandx)= (( beam_irrad(bandx))*sundist_scale)/1000.d0
          sky_irrad(bandx)= ((  sky_irrad(bandx))*sundist_scale)/1000.d0
        rbeam_irrad(bandx)= ((rbeam_irrad(bandx))*sundist_scale)/1000.d0
         rsky_irrad(bandx)= (( rsky_irrad(bandx))*sundist_scale)/1000.d0
            g_irrad(bandx)= ((    g_irrad(bandx))*sundist_scale)/1000.d0

1550   continue           

      do 1590 bandx = 1,27
!    Convert from Spectral Irrad to Irrad 
       if(bandx.le.23)then
        bandwidth = 5.d0
       else
        bandwidth = 100.d0
       endif
      
         beam_irrad(bandx)= ( beam_irrad(bandx))*bandwidth
          sky_irrad(bandx)= (  sky_irrad(bandx))*bandwidth
        rbeam_irrad(bandx)= (rbeam_irrad(bandx))*bandwidth
         rsky_irrad(bandx)= ( rsky_irrad(bandx))*bandwidth
            g_irrad(bandx)= (    g_irrad(bandx))*bandwidth

1590   continue
9999  return
      end
!***********************************************************************
!*** 15 A **************************************************************          

      subroutine beam_incident_angle
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
!     Called by: poly_direct_irradiance

      implicit real*8 (a-h,o-z)
     
      
      real*8 slope, aspect, apsza, saz
     &       ,rslope, raspect, rapsza,rsaz,incidentang
      
      
      pi = dacos(-1.d0)

         pa  = (pi/180.d0)*apsza
         pb  = (pi/180.d0)*slope
         apb = (pi/180.d0)*(aspect - saz)

!     The angle between the sun and a point in the sky(zen,raz) is AB:

         ab  = dacos(  ((dcos(pa)) * (dcos(pb))              )  
     &      +          ((dsin(pa)) * (dsin(pb)) * (dcos(apb)))  )
      

        rincidentang = ab
         incidentang = (180.d0/pi)*rincidentang

499   return
      end

!***********************************************************************  
!*** 15 C **************************************************************

      subroutine match_ray_to_ground_poly
     &       (sortedz,ms,stheta,sphi
     &       ,epsilon,rho,rhohi,phi,phihi
     &       ,ix)


!   Purpose: Determine which ground poly is "seen" from a model poly 
!     at cartesian location (x,y,z) when looking in 
!     polar direction (dir(1),dir(2)). Returns the identifying ordinal 
!     number of the ground poly (ix). 


!   Note: isct(1) is the radial coordinate of the point of intersection 
!    with z=0.  of a ray with origin at a poly of the model going in 
!    direction: sun_polar_az_array(kcounter,1),sun_polar_az_array(kcounter,2)

!   Note: isct(2) is the corresponding azimuthal coordinate of said point

!   Input:   sortedz,ms,sun_polar_az_array,epsilon,rho,rhohi,phi,phihi

!   Return:  ix

!   Calls:     cart_2_sphericalpolar
!   Called by: spectral_irradiance 


      implicit real*8 (a-h,o-z)
      integer epsilon(10,2),ix
      real*8 sortedz(10000,5),stheta,sphi
     &      ,rho(11),rhohi(10),phi(11,25),phihi(11,25)
     &      ,isct(2),isctpoly(2)

!   initialize new arrays
      isct(1:2) = 0.d0
      isctpoly(1:2) = 0.d0
       
      pi = dacos(-1.d0)

      rtheta0 = stheta
      rho0 = sortedz(ms,3)*dtan(pi - rtheta0)
     
      rphi0 =   sphi

      x = sortedz(ms,1) + rho0*dcos(rphi0)
      y = sortedz(ms,2) + rho0*dsin(rphi0)
      z = 0.d0
      
      isct(1) = dsqrt(x**2 + y**2)

      do 190 m = 10,1,-1
     
       if(isct(1).gt.rhohi(m))then
        im = m+1

        goto 200
       endif     
190   continue
      im = 1

200   if(im.eq.11)then
       in = 1
       goto 300
      endif      

      call cart_2_sphericalpolar
     &           (x,y,z,
     &            radial,theta1,isct(2))

       
      do 290 n = 1,1+epsilon(im,2)
       if(isct(2).lt.phihi(im,n))then
        in = n
        goto 300
       endif
290   continue
      in = 1

300   isctpoly(1) = rho(im)
      isctpoly(2) = phi(im,in)

      icount = 0
      do 390 m = 1,10
       do 370 n = 1,1+epsilon(m,2)
        icount = icount + 1
        if((m.eq.im).and.(n.eq.in))then
         ix = icount
         goto 999
        endif      
370    continue
390   continue
      ix = 91
     
999   return
      end
!***********************************************************************
!*** 16 **************************************************************** 
      subroutine output
     &        (outfile,ordination,ms,beam_irrad,sky_irrad
     &        ,rbeam_irrad,rsky_irrad,g_irrad
     &        )

!     Purpose: Writes to file the 5 components of spectral irradiance 
!              on each poly

!     Input:   outfile,ordination,ms,beam_irrad,sky_irrad
!              rbeam_irrad,rsky_irrad,g_irrad
              
!     Return:  -

!     Calls:     -                

!     Called by: main

      implicit real*8 (a-h,o-z)
      character*60 outfile

      integer ordination(10000),ms,bandx

      real*8 beam_irrad(27), sky_irrad(27)
     &     ,rbeam_irrad(27),rsky_irrad(27),g_irrad(27)

      open(50,file = outfile,position ='append')
!      open(50,file =  outfile)

      write(50,*)
      write(50,105)ordination(ms)

      do 190 bandx = 1,27
       write(50,105)bandx,beam_irrad(bandx),sky_irrad(bandx)
     &         ,rbeam_irrad(bandx),rsky_irrad(bandx),g_irrad(bandx)
105    format(i8,5e12.3)       
190   continue

      return
      end
!***********************************************************************

! ! !
! Here begins modifications for Sunray

      subroutine determine_coefficients (
     &   beam_coeffs_source, diff_coeffs_source, 
     &   elev, o3, vis, alb,
     &   beam, diffuse
     &)

      real*8 beam_coeffs_source(27,2,3,3,4)
      real*8 diff_coeffs_source(27,2,3,3,3,13)
      real*8 elev, o3, vis, alb
      real*8 beam(27,4), diffuse(27,13)

      real*8 elev_a1,elev_a2,elev_g1,elev_g2
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2
     &    ,vis_a1, vis_a2, vis_g1, vis_g2
     &    ,alb_a1, alb_a2, alb_g1, alb_g2
      integer elev_lo,elev_hi, o3_lo, o3_hi 
     &    ,vis_lo, vis_hi, alb_lo, alb_hi

      beam(1:27,1:4) = 0.d0
      diffuse(1:27,1:13) = 0.d0

      call compute_interpol_scale_factors(elev,o3,vis,alb
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi)

      call interpolate_beam_coeffs(beam_coeffs_source
     &    ,elev,o3,vis
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  o3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,beam)

      call interpolate_diffuse_coeffs(diff_coeffs_source
     &    ,elev,alb,o3,vis
     &    ,elev_a1,elev_a2,elev_g1,elev_g2,elev_lo,elev_hi
     &    ,o3_a1,  o3_a2,  o3_g1,  o3_g2,  O3_lo,  o3_hi
     &    ,vis_a1, vis_a2, vis_g1, vis_g2, vis_lo, vis_hi
     &    ,alb_a1, alb_a2, alb_g1, alb_g2, alb_lo, alb_hi
     &    ,diffuse)    

      return
      end
