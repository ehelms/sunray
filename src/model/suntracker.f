      program suntracker_


      
      integer*4 day,month,year,yrlength,hour,minute
      integer*4 tsr,tss,asr,ass,julday,lasr,lass,ij,nosr,noss

      real*8 alt(1440),apalt(1440),az(1440),incidentang(1440)
      real*8 nrmbflux(1440),horbflux(1440),diffuse(1440)
      real*8 lat,lon,pi,rdayangle,rdec,eqt,rlon,rlat
   

      pi = dacos(-1.d0)

! ERIC: these are the parameters that must be specified.
!       I have "hard coded" them here, but if we/you make this into
!       a callable module, then these same parameters would have to be
!       passed to the module.

      lat = -70.
      lon = -78.
      year = 2011
      month = 3
      day = 20
      do 800 hour = 0,23
       do 799 minute = 0,59
! ERIC: As you see, I've specified the values of hour and minute in
!       loops so as to show all the values in a 24-hour period
!       The loop structure can be removed when you want to make this
!       a module that specifies a single time (hour,minute) and returns
!       a single solar zenith angle and solar azimuth angle.     
      
      rlat = (pi/180.d0)*lat
      rlon = (pi/180.d0)*lon
            
       call julian (year, month, day, julday, yrlength, rdayangle)
            
       call declenation (rdayangle, rdec)

       call eqtime (rdayangle, eqt)

      
c      compute solar altitude and azimuth for 24 hours
c      arrays are a function of true solar time
       call solaralt(rlon,rlat,nosr,noss,rdec,tsr,tss
     &              ,alt,apalt,asr,ass)  

       
       call locstdtim (rlon,rlat,asr,eqt,lasr)
       call locstdtim (rlon,rlat,ass,eqt,lass)


       call solaraz(rlon,rlat,nosr,noss,rdec,alt,az)    
      
                             
       call output(rlon,rlat,nosr,noss,eqt,year,month,day
     &   ,hour,minute,lasr,lass,apalt,az)
     
799   continue
800   continue     
     
990   stop
      end
c***********************************************************************
c*******************************output 23feb94**************************
      subroutine output(rlon,rlat,nosr,noss,eqt,year,month,day
     &                 ,hour,minute,lasr,lass,apalt,az)

      
      integer*4 nosr,noss,month,lasr,lass,day,year,hour,minute,now
     &         ,nowtst,icummin,jmin,jcummin,jhour,miltime

      real*8 pi,lon,lat,apalt(1440),az(1440),temp,eqt,rlon,rlat

     
     
      pi = dacos(-1.d0)
      lat =  (180.d0/pi)*rlat
      lon = (180.d0/pi)*rlon


      icummin = (60*hour) + minute
      now = miltime(icummin)
      
       
      call trusoltim (rlon,rlat,now,eqt,nowtst)
     

c     convert current true solar time to cumulative minutes
c      so as to access the arrays
      temp =  (dfloat(nowtst))/100.d0
      jhour = int( temp )
      jmin = int( 100.d0*( temp - dfloat(jhour) ) )
      jcummin = (60*jhour) + jmin
      
       write(7,19)now
19     format(i4.4)  
!      write(7,20)now
!20    format(/,t25,' current time (assumed local standard time): ',i4.4)


!       write(*,*)'jcummin =',jcummin
        

!      if(nosr.eq.0) write(7,22)lasr
!      if(nosr.eq.1) write(7,24)
!      if(noss.eq.0) write(7,26)lass
!      if(noss.eq.1) write(7,28)
!22    format(/,t25,' sunrise: ',i4.4)
!24    format(/,t25,' no sunrise')
!26    format(t25,' sunset : ',i4.4)
!28    format(t25,' no sunset ')

      

      write(7,30)apalt(jcummin + 1)
!      write(7,32)
      write(7,34)az (jcummin + 1)
!      write(7,36)
30    format(/,t20,' current solar altitude = ',f8.0)
32    format(t20,' reference: local zenith = 90; horizon = 0')
34    format(/,t20,' current solar azimuth  = ',f8.0)
36    format(t20,' reference: range [+0=n,+90=e,+180=s,+270=w]')


999   return
      end

c***********************************************************************
c*******************************stdmer 20dec91**************************
c**************modified western hem calc 20may1999**********************

      subroutine stdmeridian (rlon,rlat,stdmer)

c     input {}
c     returns standard meridian (stdmer) [degrees]

c convention: western hemisphere standard meridians are negative
c           : eastern hemisphere standard meridians are positive

      
      integer stdmer
      real*8 pi,lon,rlon,rlat,x,y

     
      pi = dacos(-1.d0)
      lon = (180.d0/pi)*rlon

c clean up round off error that may alter dmod result
      lon = (dfloat(idint(0.5d0+((dabs(lon))*10000.d0))))/10000.d0
      lon = dsign(lon,rlon)
     
     
      if(lon.eq.0.d0)then
c      zulu (z) meridian
       stdmer = 0
       goto 199
      endif

      if( lon.lt.0.d0 ) then
c      western (w) hemisphere
cc       stdmer = - int( dabs(lon) - dmod(dabs(lon),15.0d0) )  
       x =  dabs(lon) 
       y =  dmod(dabs(lon),15.0d0) 
       stdmer = - int( x - y )
      endif

       

      if( lon.gt.0.d0) then
c      eastern (e) hemisphere
       stdmer = int(lon + (15.d0 - dmod(lon,15.0001d0) ))
      endif

199   return
      end
c***********************************************************************
c*******************************julian 09jan92**************************
      subroutine julian (year,month,day,julday,yrlength,rdayangle)

c     input year, month, day
c     returns julian day (julday), yearlength [days], rdayangle [rads]

      
      integer*4 daytab(13,2), year, day, yrlength,leap,m4,m100,m400
     &         ,julday,i,month
      real*8 pi,rdayangle
      pi = dacos(-1.d0)

      data daytab / 0,31,28,31,30,31,30,31,31,30,31,30,31,
     +              0,31,29,31,30,31,30,31,31,30,31,30,31 /

      leap = 1
      m4   = mod(year,4  )
      m100 = mod(year,100)
      m400 = mod(year,400)
      if(((m4.eq.0).and.(m100.ne.0)).or.(m400.eq.0))then
       leap = 2
      endif
      
      julday = day
      do 190 i=1,month
       julday = julday + daytab(i,leap)
190   continue

      yrlength = 365
      if(leap.eq.2) yrlength = 366

      rdayangle = (2.0*pi*(julday-1))/(dfloat(yrlength))

      return
      end
c***********************************************************************
c*******************************decsub 07jan92**************************
      subroutine declenation (rdayangle,rdec)
c     numerical algorithm from j.w.spencer, search 2 (5), 172 (1971)

c     input dayangle [radians], returns declenation of sun [radians]
      
      real*8 pi,term1,term2,term3,term4,term5,term6,term7,rdec,rdayangle
      
      pi = dacos(-1.d0)
      term1 = +0.006918d0
      term2 = -0.399912d0*dcos(rdayangle)
      term3 = +0.070257d0*dsin(rdayangle)
      term4 = -0.006758d0*dcos(2.0d0*rdayangle)
      term5 = +0.000907d0*dsin(2.0d0*rdayangle)
      term6 = -0.002697d0*dcos(3.0d0*rdayangle)
      term7 = +0.001480d0*dsin(3.0d0*rdayangle)
      rdec  = term1+term2+term3+term4+term5+term6+term7
      return
      end
c***********************************************************************
c*******************************eqtsub 07jan92**************************
      subroutine eqtime(rdayangle,eqt)
c     numerical algorithm from j.w.spencer, search 2 (5), 172 (1971)

c     input dayangle [radians], returns equation of time (minutes)
      
      real*8 rdayangle,eqt,eqtrad,term1,term2,term3,term4,term5
      
      term1 = +0.000075d0
      term2 = +0.001868d0*dcos(rdayangle)
      term3 = -0.032077d0*dsin(rdayangle)
      term4 = -0.014615d0*dcos(2.0d0*rdayangle)
      term5 = -0.040890d0*dsin(2.0d0*rdayangle)
      eqtrad = term1+term2+term3+term4+term5
      eqt = (229.18d0)*eqtrad
      return
      end
c***********************************************************************
c*******************************locstdtim 09jan92*********************** 
c*******change note > tst to hours must use double precision***21may1999
      subroutine locstdtim (rlon,rlat,tst,eqt,lst)

c     input true solar time [military], equation of time [minutes]
c     returns local standard time [military]

c     local longitude (lon) is negative in west hemisphere, pos in east
c     standard meridian (stdmer) is neg. in w. hemisphere, pos. in east

c     not robust for daylight savings time

      
      
      integer*4 tst,lst,hour,minute,stdmer
      real*8 rlon,rlat,eqt,lon,lat,pi


      lon = (180.d0/pi)*rlon
      pi = dacos(-1.d0)
c     express tst (military time) in hour, minute
c     the i4.4 format is appropriate for writing military time
      hour = int( (dfloat(tst))/100.d0)
      minute = tst - (100*hour)

      call stdmeridian(rlon,rlat,stdmer)

c     correct for local longitude and equation of time
      minute=minute-int((4.*(lon-stdmer))+
     +              (sign(real(0.5),real((lon-real(stdmer))))))-
     +              nint(eqt)
c     check if minute >= 60 or minute < 0
      if(minute.ge.120)then
       hour = hour + 2
       minute = minute - 120
       goto 899
      else if(minute.ge.60)then
       hour = hour + 1
       minute = minute - 60
       goto 899
      else if(minute.lt.-60)then
       hour = hour - 2
       minute = 120 + minute
       goto 899
      else if(minute.lt.0)then
       hour = hour - 1
       minute = 60 + minute
       goto 899
      end if

c     check if hour >= 24 or hour < 0
899   if(hour.eq.25)then
       hour = 1
       goto 999
      else if(hour.eq.24)then
       hour = 0
       goto 999
      else if(hour.eq.-2)then
       hour = 22
       goto 999
      else if(hour.eq.-1)then
       hour = 23
       goto 999
      end if

999   lst = (100*hour) + minute

      return
      end
c***********************************************************************
c*******************************trusoltim 09jan92*********************** 
c*******change note > lst to hours must use double precision***21may1999

      subroutine trusoltim (rlon,rlat,lst,eqt,tst)

c     input local standard time [military]
c      equation of time [minutes]
c     returns true solar time [military]

c     local longitude (lon) is negative in west hemisphere, pos in east
c     standard meridian (stdmer) is neg. in w. hemisphere, pos. in east

c     not robust for daylight savings time

      
      integer*4 tst,lst,hour,minute,stdmer
      real*8 pi,rlon,rlat,lon,eqt


     
      pi = dacos(-1.d0)
      lon = (180.d0/pi)*rlon

c     express lst (military time) in hour, minute
c     the i4.4 format is appropriate for writing military time
      hour = int( (dfloat(lst))/100.d0)
      minute = lst - (100*hour)
     
      call stdmeridian(rlon,rlat,stdmer)  

c     correct for local longitude and equation of time
      minute=minute+int((4.*(lon-real(stdmer)))
     & +(sign(real(0.5),real((lon-real(stdmer))))))
     & +              nint(eqt) 
     
     
      
     
c     check if minute >= 60 or minute < 0
      if(minute.ge.120)then
       hour = hour + 2
       minute = minute - 120
       goto 899
      else if(minute.ge.60)then
       hour = hour + 1
       minute = minute - 60
       goto 899
      else if(minute.lt.-60)then
       hour = hour - 2
       minute = 120 + minute
       goto 899
      else if(minute.lt.0)then
       hour = hour - 1
       minute = 60 + minute
       goto 899
      end if

c     check if hour >= 24 or hour < 0
899   if(hour.eq.25)then
       hour = 1
       goto 999
      else if(hour.eq.24)then
       hour = 0
       goto 999
      else if(hour.eq.-2)then
       hour = 22
       goto 999
      else if(hour.eq.-1)then
       hour = 23
       goto 999
      end if

999   tst = (100*hour) + minute

      return
      end
c***********************************************************************
c*******************************solaraz 18jun92*************************
      subroutine solaraz(rlon,rlat,nosr,noss,rdec,alt,az)
c     input  declenation (radians)
c      array of solar altitudes (degrees)
c      apparent sunrise and sunset times (military),
c     returns array of solar azimuths (degrees)

      
      integer*4 tim,nosr,noss
     
      real*8 pi,rlon,rlat,lon,lat,rdec,alt(1440),az(1440),ralt,psi,arg

      pi = dacos(-1.d0)
c     solar azimuth angle is constant at poles
      if(rlat.eq.(+pi/2.d0))then
       az(1:1440) = 180.d0
      endif
      if(rlat.eq.(-pi/2.d0))then
       az(1:1440) = 0.d0
      endif
      if(dabs(rlat).eq.(pi/2.d0))then
       goto 200
      endif
c     compute solar azimuth from apparent sunrise to apparent sunset
c     compute solar azimuth from 0000 to 2359 for program showsun only
c     solar azimuth is a function of true solar altitude (alt)

      do 190 tim = 0, 1439, 1
       ralt = (pi/180.d0)*alt(tim+1)

c      test: if solar altitude at zenith or nadir: azimuth undefined
       if(dabs(ralt).eq.(pi/2.d0)) then
c       test for solar zenith; assign azimuth = east
	if(ralt.eq.(+pi/2.d0))then
	 az(tim+1) = 90.d0
	endif
c       test for solar nadir;  assign azimuth = west
	if(ralt.eq.(-pi/2.d0))then
	 az(tim+1) = 270.d0
	endif
	goto 190
       end if

       arg =((dsin(ralt)*dsin(rlat))-dsin(rdec))/(dcos(ralt)*dcos(rlat))
       if ( (dabs(arg)).gt.1.d0) arg = dsign(1.d0,arg)
       if (tim.le.720) then
c       time of day is a.m.
	psi = (180.d0/pi)*dacos(arg)
       else
c       time of day is p.m.
	psi = -(180.d0/pi)*dacos(arg)
       end if
c      azimuth angle (psi) range [+180, -180)
c      azimuth angle (az) range [0, +360)
       az(tim+1) = 180.d0-psi
cc      write(1,10) alt(tim+1),az(tim+1)
10     format(' solar altitude (deg), azimuth (deg) = ',2f10.1)

190   continue
200   return
      end
c***********************************************************************
c*******************************solaralt 18jun92************************
      subroutine solaralt(rlon,rlat,nosr,noss,rdec,tsr,tss
     & ,alt,apalt,asr,ass)

c     input declenation (radians)
c     returns true sunrise and sunset times (military),
c      array of solar altitudes (degrees),
c      array of apparent solar altitudes (degrees),
c      and apparent sunrise, sunset times due to refraction (military)

      
      integer*4 tsr,tss,asr,ass,nosr,noss
      integer*4 tsrhour,tsrminute,tsshour,tssminute
      integer*4 tim, timtsrmin, timtssmin, timasrmin, timassmin
     &         ,minperday,miltsr,miltss,miltime
     
      real*8 pi,alt(1440),apalt(1440),rlat,rdec,daylen,argument
     &       ,tsunrise,tsunset,hrang,rzenith,rlon,rzprime

      
      pi = dacos(-1.d0)
c     daylen, tsunrise, tsunset are in decimal hours
c
      if ( rlat.eq.(pi/2.d0) ) then
       if (rdec.gt.0.) daylen = 24.d0
       if (rdec.eq.0.) daylen = 12.d0
       if (rdec.lt.0.) daylen = 0.d0
       goto 50
      end if
      if ( rlat.eq.(-pi/2.d0) ) then
       if (rdec.gt.0.) daylen = 0.d0
       if (rdec.eq.0.) daylen = 12.d0
       if (rdec.lt.0.) daylen = 24.d0
       goto 50
      end if
      argument = -(dtan(rlat))*(dtan(rdec))
      if ( (dabs(argument)).gt.1.d0 ) argument = dsign(1.d0,argument)
      daylen = (2.d0/15.d0)*(180.d0/pi)*dacos(argument)

50    tsunrise = 12.d0-(daylen/2.d0)
      tsunset  = 12.d0+(daylen/2.d0)
      minperday = nint(60.d0*daylen)

c     compute true sunrise and sunset in military time
      tsrhour = int(tsunrise)
      tsrminute = nint(60.d0*(tsunrise-float(tsrhour)))
      if(tsrminute.eq.60)then
       tsrhour = tsrhour + 1
       tsrminute = 0
      end if
      miltsr = 100*tsrhour + tsrminute
      tsshour = int(tsunset)
      tssminute = nint(60.d0*(tsunset-float(tsshour)))
      if(tssminute.eq.60)then
       tsshour = tsshour + 1
       tssminute = 0
      end if
      miltss = 100*tsshour + tssminute
      tsr = miltsr
      tss = miltss
cc      write(1,8)miltsr,miltss
8     format(' true sunrise, sunset (military) = ',i4.4,2x,i4.4)

c     compute time of true sunrise and sunset (cumulative minutes)
c     hour angle (hrang) range [+pi,-pi) over a 24-hour day
      timtsrmin = 720 - nint((dfloat(minperday))/2.d0)
      timtssmin = 720 + nint((dfloat(minperday))/2.d0)
      if (timtsrmin.lt.0) timtsrmin = 0
      if (timtssmin.gt.1439) timtssmin = 1439

      do 190 tim = 0, 1439, 1
       hrang = pi-((pi/180.d0)*(15.d0)*((dfloat(tim))/60.d0))
       argument = (dsin(rdec)*dsin(rlat))+
     +            (dcos(rdec)*dcos(rlat)*dcos(hrang))
       if ( (dabs(argument)).gt.1.d0 ) argument = dsign(1.d0,argument)
       alt(tim+1)  = (180.d0/pi)*dasin(argument)

c      compute apparent altitude (altitude corrected for refraction)
       rzenith = (pi/2.d0)-dasin(argument)
       rzenith = dmax1(0.d0,rzenith)
       call refract (rzenith,rzprime)
       apalt(tim+1) = (180.d0/pi)*((pi/2.d0)-rzprime)
cc      write(99,10) tim,alt(tim+1),apalt(tim+1)
10     format(' solar altitude, apparent altitude (deg)=',i4.4,1x2f10.1)
190   continue

c     determine apparent sunrise and sunset times due to  refraction

c     exclude cases of polar continuous day and polar continuous night
      nosr = 0
      noss = 0
      if(apalt(721).gt.0.d0) then
c      at solar noon the sun is above the horizon
       if(apalt(   1).gt.0.d0) nosr = 1
       if(apalt(1440).gt.0.d0) noss = 1
      else
c      at solar noon the sun is below the horizon
       nosr = 1
       noss = 1
      end if

      if(apalt(timtsrmin+1).gt.0.d0)then
       do 290 tim = timtsrmin, 0, -1
        if(apalt(tim+1).le.(-0.d0)) exit
290    continue
      end if
      timasrmin = max0(tim,0)

      if(apalt(timtssmin+1).gt.0.d0) then
       do 295 tim = timtssmin, 1439, 1
        if(apalt(tim+1).le.(-0.d0)) exit
295    continue
      end if
      timassmin = min0(tim,1439)

      asr = miltime(timasrmin)
      ass = miltime(timassmin)
      return
      end
c***********************************************************************
c*******************************refract 18jun92*************************
      subroutine refract(rzenith,rzprime)
c     numerical algorithm from j.j.streicher

c     input zenith angle (rzenith)[radians]
c     returns corrected zenith angle (rzprime)[radians]
c     routine accounts for atmospheric refraction in observed zenith

      
      real*8 pi,munot,rzenith,rzprime,zenith,e,factor,argument

      pi = dacos(-1.d0)

      zenith = rzenith*(180.d0/pi)

c     regression equation for zenith valid over range [0, 90.83]
c     apparent sunrise/sunset occur at zenith = 90.83
c     90.83 = 90.57 (refr of sun center) + 0.26 (semi-diam of sun)
c note: apaltitude computations reference sun center for alt >> horizon
c note: for apalt near the horizon, ref point migrates to top of sun
c note: to maintain apalt ref to center, must change code as follows -
c note: sub refract - parameterize rzprime wrt 90.57 degrees
c note: sub solaralt - compute asr, ass by comparing apalt with (-0.26),
c note:                not (0.0)

      if(zenith.gt.(90.83d0)) then
       rzprime = rzenith
       goto 190
      end if

      if(zenith.eq.(0.d0)) then
       rzprime = rzenith
       goto 190
      end if

      munot = 1.00029d0
      e = 21.2d0*dexp( (zenith/90.83d0)**8.8d0 )
      factor = 1.d0 + (0.000394965d0)*((zenith/90.83d0)**e)
      argument = (factor*dsin(rzenith)) / (munot)
      if ( (dabs(argument)).gt.1.d0 ) argument = dsign(1.d0,argument)
      rzprime = dasin(argument)
190   return
      end
c*********************************************************************** 
c*******************************cumtime 11feb92*************************
      integer*4 function cumtime(miltime)

c     input military time of day in i4.4 format [0000,2359]
c     returns cumulative time [0,1439] [minutes]

      
      integer*4 hour,minute,miltime

      hour = int( (dfloat(miltime))/100.d0 )
      minute = miltime-(100*hour)
      cumtime = minute + 60*hour

      return
      end
c***********************************************************************
c*******************************miltime 31jan92*************************
      function miltime(cumtime)

c     input cumulative time [0,1439] [minutes]
c     returns military time of day in i4.4 format [0000,2359]
c     note: this function does not increment date

     
      integer*4 hour,cumtime
      integer minute,miltime
     

      hour = int((dfloat(cumtime))/60.d0)
      minute = cumtime-(60*hour)

      if(minute.eq.60) then
       hour = hour + 1
       minute = 0
      end if
      if(hour.ge.24) hour = hour - 24

      miltime = 100*hour + minute

      return
      end
