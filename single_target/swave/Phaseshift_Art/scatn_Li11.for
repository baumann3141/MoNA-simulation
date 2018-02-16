c
c la section efficace a un facteur 1.4
c
c input example
c 9 9  4 3   5.8 18. -1.665 .68 1. 1.5 1. 3 300 0.017 .6 1 .5
c
C c SMA.DAT then it calculates the tranfer cross section  
C WITH CORRECTED PRINT OUT 17 NOV 89, last version 26 oct 1990.
C
	 IMPLICIT REAL*8 (a-h,o-z)
	dimension aam(201),ak(999), sigbre(999),sigabs(999),sigtot(999)
        COMMON/GUI/ED,QR,AARG1,AARG2,PL1,PLL,L1
!        CHARACTER*20 FILENAME
!        CHARACTER*20 FILENAME1
!        INTEGER IN
!        CHARACTER*20 FILENAME2
!        CHARACTER*20 FILENAME3
!        write(*,*)'fichier input cross'
!        read(*,500) FILENAME
!	IN=INDEX(FILENAME,' ')
!        write(*,*)' '
!        READ(*,500) FILENAME1
!        READ(*,500) FILENAME2
!        READ(*,500) FILENAME3
 500    FORMAT (A20)
!        OPEN (UNIT=8,NAME='bbb.inp',TYPE='OLD')
        open(unit=10,file='Q1Be9-Be12.dat')
        open(unit=75,file='scatlengthn_Be12.dat')
!        open(unit=11,name=FILENAME2//'.dat',type='new')
!        open(unit=12,name=FILENAME3//'.dat',type='new')
       OPEN (UNIT=19,file='Qphase_s.dat')
c      OPEN (UNIT=29,file='faclj2.top',status='NEW')
        data AP,AT,ZP,ZT,RS,EINC,E1,C1,L1,SJ1,sf,LMAX,maxe,delef,a
     $  /9,12,4,4,6.12,18.,-1.665,0.68,1,1.5,1.,3,998,.001,.6/
     	data lpar,spar/0,.5/
c
c 13 9  6 3  6.2 26. -4.95 1.88 1. .5 1. 3 300 .017 .6 1 .5
c
c Fundamental constants, energies in MeV time in 10**(-22)Sec
		sc5=asin(-.08715574)
	        EQ=1.44 !  Charge**2 in MeV*fm
        AM=1.0432 ! Average nucleon mass in MeV (Sec/fm)**2 
        ACCA=6.5821 ! h_bar in MeV Sec
        PI=3.14159
 	D=.1
	ntt=200
	
        totabs=0.
	totbre=0.
        do 200 id=41,ntt
	dd=(id-1)*d
       crossfact=10.*2.*pi*sf*exp(-0.693*exp((rs-dd)/a))
c	print *, DD,crossfact
c  Center of mass energy and Coulomb barrier
c
        ECM=AT*EINC/(AP+AT)
        ECB=ZP*ZT*EQ/dd
      
c  Coulomb length parameter
	AC=ZP*ZT*EQ/2./ECM
c   Relative momentum per nucleon at the strong absorption radius RS
c
        AKD=SQRT(2.*(ECM-ECB)*AM/(AP*AT)*(AP+AT))/ACCA
C  Relative energy per nucleon at RS
C
        ED=(ACCA*AKD)**2/2./AM
c
	avd=akd*acca/am
c        print *, avd
c  initial state momentum
c
        AGI=SQRT(-2.*AM*E1)/ACCA
c
c        WRITE(9,40) AP,AT,ZP,ZT,RS,EINC,L1,SJ1
   40   FORMAT(1H0,10X,'REACTION DATA',//,5X,'AP=',F5.1,'AT=',F5.1,'ZP='
     1  ,F4.1,'ZT=',F4.1,'RS=',F10.5,'EINC =',F10.5,'L1=',I2,'J1=',F4.2)
c        WRITE(9,50) ECM,ECB,AKD,ED,AGI
   50   FORMAT(1H0,'ECM=',F10.5,'ECB=',F10.5,'KD=',F10.5,//,2X,'ED=',
     1  F10.5,//,   2X,'GAMMA1=',F10.5,/)
        OPEN(UNIT=7,file='smali.dat',status='OLD',ACCESS='SEQUENTIAL')
        sigmabre=0.
	sigmaabs=0.
c	sigbre(1)=0.
c	sigabs(1)=0.
c	sigtot(1)=0.
	DO 100 I=1,maxe
	M=I
	EF=REAL(I)*delef
c	if (i.eq.1)then
c	ef=.1
c	endif
	QR=E1-EF     
c       k_f    
         AKF=SQRT(2.*AM*EF)/ACCA
	akf1=SQRT(2.*AM*Ef*9./10.)/ACCA
c
        ETA=SQRT((AGI**2-AKF**2)/2.+(((AGI**2+AKF**2)/AKD)**2+AKD**2)/4.
     1  )

c
c       X_f,X_i


       	EX=EXP(-2.*ETA*dd)
	eta_1=1./eta
	BFI=C1*C1*.25 *EX*eta_1/dd/AKD/AKD*AM/ACCA/ACCA/AKF
C NEW PRINT OUT
C PRINT OUT CORRECTED 20 MAY 89,17 NOV 89
        
                  R1=1.2*Ap**.333333333
        R2=1.2*AT**.333333333
c	SP=RS-R1-R2
	AK1= -((-agi**2-akf**2+akd**2)/2/akd)
	AK2=-((-agi**2-akf**2-akd**2)/2/akd)
	ak(i)=ef!(Einc-ef+e1)*sqrt(1.+2.*940.*(ap-1)/(Einc-ef+e1))
	facja=1!sqrt((Einc-ef+e1)**2+2.*940.*(ap-1)*(Einc-ef+e1))/
c     1  ((Einc-ef+e1)+940.*(ap-1))
c   print *,ef,facja
	rk1=R1*AK1
	RK2=R2*AK2
c	WRITE (9,*)'K1=',AK1,'K2=',AK2,'RK1=',RK1,'RK2=',RK2
c	F1=C1*C1*PL1*EXP(-2*ETA*R1) 
c	ETAR2=ETA*R2
c	EXX=EXP(-2.*ETA*SP)/ETA/RS
c	FS=.5*CK*CK1*EXX*PI/AKF/AKD**2
c        WRITE(9,*)'B=',EXX,'EF=',ef
	sum1=0.
	sum2=0.
        SUM=0.
	read(7,*)ef	

        DO 11 LL=1,lmax
!	print *,l
cLC=LL-20
	JJ=-1
        L=LL-1
        AARG2=2.*(ETA/AKF)**2+1.
	AARG1=2.*(ETA/AGI)**2-1.
c PRINT *,ETA, AGI,AARG1
	pl1=pl(l1,aarg1)
	pll=pl(l,aarg2)
	!DO 15 IY=1,NTT
	!X=(IY-1)*D
	!ARG2=2.*(ETA/AKF)**2+1.+2.*ETA/dd/AKF**2*X**2
	!ARG1=2.*(ETA/AGI)**2-1.+2.*ETA/dd/AGI**2*X**2
	!AAM(IY)=EXP(-X**2)*PL(L1,ARG1)*PL(L,ARG2)
c   15   CONTINUE
c	CALL SIMP(AAM,ABM,NTT,D)	
c	ABM=2.*ABM/SQRT(PI)
cccccccccccccccccccccccccccccccccc
	abm=pl1*pll
c	AA=AAM(1)
	DO 10 J=1,2
	IF(L.EQ.0)JJ=1
        
	SJ2=L+.5*JJ
c    Reads the real and imaginary parts of S_l from SMA.DAT
c 
	ck=0.
	READ(7,*)RES,AIMS
!	print *,RES,AIMS
	RRES=1.-RES*res-aims*aims
	if(l.eq.lpar.and.sj2.eq.spar)then
	ck=1.
!	print *,res, ck

	PROD=(1.-RES)*ck
	sig=2.*pi*prod/akf/akf
         PRODc=sqrt((1.-res)**2+aims**2)!valeur absolue 1-S
	sclength1=-(prodc)/2./akf1      !4 manieres de calculer la sclength
	sclength2=-(aims)/(1+res)/akf1
	ph1=((1-res)/aims)!/akf1
	sclength3=-((1-res)/aims)/akf1
	 phshift=(datan2(1-res,aims))
        sclength=-tan(phshift)/akf1
	sc1=acos(res)/2.
	sc2=asin(aims)/2.
	sc3=acos(1.)
	sc4=asin(-1.)
!	print *, ef,aims
	if(id.eq.41)then
        write(75,*)ef,sclength,sclength3,sclength2,sclength1
	endif
!	print *,sc1,sc2,sc3,sc4
	if(id.eq.41)then
	write (19,*)ef,phshift!,bfi, abm
	endif
	endif
c        print *,ef,l,ck,jj
        jj=1
!	IF(RRES.LT.0)CK=0.
c	IF(LC)112,112,11
 112    CONTINUE
 	
	
c	WRITE(9,14)ABM,AA
c   14   FORMAT(1H0,10X,'INT=',E10.5,5X,'POL=',E10.5)


C	WRITE(9,*)'   RE S=',RES,'  IM S=',AIMS
        if(l.gt.LMAX) ck=0.
	PRODc=((1.-res)**2+aims**2)*ck

	prod1=((1.-res)**2+aims**2)*abm*ck
	prod2= (1.-res*res-aims*aims)*abm*ck
c	F2=PROD*exp(-2*etar2)
c        print *,l,prod1,prod2
	CALL FACLJ(L,SJ1,SJ2,FACTORLTOJ)
	IF(L.EQ.0)FACTORLTOJ=.5
	FAC2L=(2*l+1)*factorltoj
        ljj=l-4
	jll=j-1
c	if(ljj)70,72,70
c 72    if(jll)70,71,73
c  71    write(19,*)ak1,fac2l
c        go to 70
c  73    write(29,*)ak1,fac2l
c  70    continue
	SOM=PROD*FAC2L
	som1=prod1*fac2l*avd*acca
	som2=prod2*fac2l*avd*acca
c        WRITE(9,70)som2, abm,fac2l,L,SJ2
   70   FORMAT(5X,'som2=',E9.4,2X,'ABM=',E9.4,2X,'FATT=',F7.3,2X,
     1   'Lf=',I3,2X,'SJf=',F5.2)
c	write(9,*)'FACLJ=',FACTORLTOJ
        SUM=SUM+SOM
	sum1=sum1+som1
	sum2=sum2+som2
   10   CONTINUE
c	WRITE(9,*) '    PARTIAL SUM=  ',sum1     
   11   CONTINUE
c	print *,ex,eta_1,bfi,sum
	dPdE=BFI*SUM
        dPdEse=bfi*sum1
	dPdEabs=bfi*sum2
c	WRITE(9,*) 'SUM1=',SUM1,'dP/dE=',dPdE,'BFI=',BFI
c	print *,dpde
c Calculation of the cross section
c store ds/dk1
 	crossfactt=10.*pi*sf*rs/eta

        sig1=facja*crossfact*dpdese*d*dd/acca/avd+sigbre(i)
        sigbre(i)=sig1
        sig2=facja*crossfact*dpdeabs*d*dd/acca/avd+sigabs(i)
        sigabs(i)=sig2
        sigtot(i)= sigbre(i)+sigabs(i)
c integral over energy be aware of phase factors
       	sigmabre=sigmabre+crossfactt*dpdese/acca/avd
     	sigmaabs=sigmaabs+crossfactt*dpdeabs/acca/avd
        sigmatot=sigmabre+sigmaabs
c	print *,sigbre(i),sigabs(i)
 100    CONTINUE
c	print *,dd,sig1,sig2
	sigmd1=sigmaabs/(10.*2.*pi)!*dd!/exp(-0.693*exp((rs-dd)/a))
	sigmd=sigmabre/(10.*2.*pi)!*dd!/exp(-0.693*exp((rs-dd)/a))
	totabs=totabs+sigmaabs*dd*d*delef
	totbre=totbre+sigmabre*dd*d*delef
c	write(6,334)dd, SIGMd
       	CLOSE(UNIT=7)

  200   continue
!        write(10,*) 'totabs', totabs,' totbre',totbre
        WRITE(10,333)(ak(i),1.4*sigtot(i), i=1,maxe)!        WRITE(11,333)(ak(i),sigbre(i),i=1,maxe)
!        WRITE(12,333)(ak(i),sigabs(i),i=1,maxe)
  334   FORMAT(2E15.6)
  333   FORMAT(2E15.6)
  335   FORMAT(4E10.3)
	        STOP
        END
C
C  LEGENDRE POLYNOMIALS
C
        FUNCTION PL(L,ARG)
	 IMPLICIT REAL*8 (A-H,O-Z)
        IF(L)1,2,1
   2    PL=1.      
        RETURN
   1    PL0=0.
        PL1=1.
        DO 3 I=1,L
        AI=FLOAT(I-1)
        PL=((2*AI+1.)*ARG*PL1-AI*PL0)/(AI+1.)
        PL0=PL1
        PL1=PL
   3    CONTINUE
        RETURN
        END



	FUNCTION DERPOLEG(L,x)
	IMPLICIT REAL*8 (A-H,O-Z)
	IF(L.EQ.0)THEN
	DERPOLEG=0.
	RETURN
	ELSE IF (L.EQ.1) THEN
	DERPOLEG=1.
	RETURN
	END IF
	DERPOLEG=REAL(L)/(x*x-1.)*(x*PL(L,x)-PL(L-1,x))	!  from recurrence formula (8.914-2.) in Gradshtein and Ryzhik, A/P, 1980.
	RETURN
	END


	SUBROUTINE SIMP(W,SSUM,NT,DELTA)
	 IMPLICIT REAL*8 (A-H,O-Z)
	DIMENSION W(201)
	SSOM=0.0
	NTT=(NT-1)/2
	DO 1 IX=1,NTT
	II=IX*2
	IJ=(IX*2)+1
	SSOM=SSOM+(2.*W(II)+W(IJ))
   1    CONTINUE
	SSUM=(2.*SSOM+W(1)-W(NT))*DELTA/3.
	RETURN
	END

	SUBROUTINE FACLJ(L,AJ1,AJ2,FACTORLTOJ)
	IMPLICIT REAL*8 (A-H,O-Z)
	COMMON/GUI/ED,QR,AARG1,AARG2,PL1,PLL,L1
	
 	IF(ED.EQ.-QR)THEN
	FE=0.
	GO TO 5
	END IF
	VL1=REAL(L1)
        VL2=REAL(L)
	Y1=SQRT(AARG1**2-1.)
	dP1=DERPOLEG(L1,AARG1)
	F1=Y1*DP1/PL1
	Y2=SQRT(AARG2**2-1.)
	dP2=DERPOLEG(L,AARG2)
	F2=Y2*DP2/PLL
	FE=F1*F2
	
	IF(ED.LT.-QR) FE=-FE
	IF(AJ1.EQ.VL1-0.5.AND.AJ2.EQ.VL2-0.5) Rh=FE/(VL1*VL2)
	IF(AJ1.EQ.VL1-0.5.AND.AJ2.EQ.VL2+0.5) Rh=-FE/(VL1*(VL2+1.))
	IF(AJ1.EQ.VL1+0.5.AND.AJ2.EQ.VL2-0.5) Rh=-FE/(VL2*(VL1+1.))
	IF(AJ1.EQ.VL1+0.5.AND.AJ2.EQ.VL2+0.5) Rh=FE/((VL1+1.)*(VL2+1.))
  5      CONTINUE
		factorLtoj=0.5*(2.*AJ2+1.)/REAL(2*L+1)*(1.+Rh)

	 	RETURN
	END

        FUNCTION DGAMMA(X)
      DIMENSION C(13)	
      DATA C
     >/ 0.00053 96989 58808, 0.00261 93072 82746, 0.02044 96308 23590,
     >  0.07309 48364 14370, 0.27964 36915 78538, 0.55338 76923 85769,
     >  0.99999 99999 99998,-0.00083 27247 08864, 0.00469 86580 79622,
     >  0.02252 38347 47260,-0.17044 79328 74746,-0.05681 03350 86194,
     >  1.13060 33572 86556/
      Z=X
      IF(X.GT.0.0) GO TO 1
      IF(X.EQ.INT(X)) GO TO 5
      Z=1.0-Z
 1    F=1.0/Z
      IF(Z.LE.0.0) GO TO 4
      F=1.0
 2    IF(Z.LT.2.0) GO TO 3
      Z=Z-1.0
      F=F*Z
      GO TO 2
 3    Z=Z-1.0
 4    DGAMMA=
     > F*((((((C(1)*Z+C(2))*Z+C(3))*Z+C(4))*Z+C(5))*Z+C(6))*Z+C(7))/
     >   ((((((C(8)*Z+C(9))*Z+C(10))*Z+C(11))*Z+C(12))*Z+C(13))*Z+1.0)
      IF(X.GT.0.0) RETURN
      DGAMMA=3.1415926535897930/(SIN(3.1415926535897930*X)*DGAMMA)
      RETURN
 5    DGAMMA=0.0
      PRINT 10,X
      RETURN
 10   FORMAT(1X,' DGAMMA: ARGUMENT IS NON-POSITIVE INTEGER= ',F14.5)
      END
 
