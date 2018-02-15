C for PAW
      SUBROUTINE rd(iii)
      implicit none
      integer iii
      integer i

      integer ndata
      parameter (ndata =  15)
      double precision  data(ndata)
      real d1

      integer status

      integer num

      character*100 file
      data file /'tuple_nathan.dat.txt'/
      character*255 line
      character*9 value

      integer ic1,ic2   ! index for searching

c OPEN the file
      open(unit=11,iostat=status,status='old',err=100,file=file,
     &   recl=50)

      write(*,'(''There was NO error opening the file '',a)')file
      
      num=0  ! number of lines read

c read one line
 200  continue

      do i=1,ndata        ! set data to zero
        data(i) = 0.0
      enddo

      do i=1,15
        read(unit=11,fmt='(e23.17)',iostat=status,err=101,end=999)
     &     data(i)
      enddo
      write(*,*)
      write(*,*)data

C read next data set
      goto 200                  ! read next line


 100  continue                  ! error opening file
      write(*,*)'Error opening the file'
      write(*,*)'iostat = ',status
      stop

 101  continue                  ! error reading from file
      write(*,*)'Error reading from file'
      write(*,*)'Status is ', status
      write(*,*)'Data is',data
      stop

 102  continue                  ! error reading from file
      write(*,*)'Error converting line to data'
      write(*,*)'Status is ', status
      write(*,*)'Line is',line
      write(*,*)'Data is',data
      stop

 999  continue                  ! found end-of-file
      write(*,'(''End of file'')')
c      write(*,'(''Read '',i3.3,'' lines.'')'),num
      close(11)
      stop
      
      return 
      end

