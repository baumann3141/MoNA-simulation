      PROGRAM ReadDouble
      implicit none
      integer i

      integer ndata
      parameter (ndata =  15)
      double precision data(ndata)

      integer status

      integer num

      character*100 file
      data file /'tuple_nathan.dat.txt'/
      character*1000 line

c OPEN the file
      open(unit=11,iostat=status,status='old',err=100,file=file)

      write(*,'(''There was NO error opening the file '',a)')file
      
      num=0  ! number of lines read

c read one line
 200  continue

      do i=1,ndata        ! set data to zero
        data(i) = 0.d0
      enddo

      write(*,*)
      write(*,'(''*************************************************'')')
      write(*,'(a)')'Going to read one line...'
      read(unit=11,fmt='(a)',iostat=status,end=999,err=101)line

      write(*,'(a)')'There was no error and I read one line.  It was'
      write(*,*)
      write(*,'(a)') line
      write(*,*)

c parse line
      num=num+1    ! number of lines
      read(line,fmt=*)(data(i),i=1,ndata)

      write(*,1001)num, (data(i),i=1,ndata)
 1001 format('line ',i3.3,':',<ndata>(e15.6))

C read next line
      goto 200                  ! read next line


 100  continue                  ! error opening file
      write(*,*)'Error opening the file'
      write(*,*)'iostat = ',status
      stop

 101  continue                  ! error reading from file
      write(*,*)'Error reading from file'
      write(*,*)'Status is ', status
      write(*,*)'Data is',(data(i),i=1,ndata)
      stop

 999  continue                  ! found end-of-file
      write(*,'(''End of file'')')
      write(*,'(''Read '',i3.3,'' lines.'')'),num
      close(11)
      stop

      end
