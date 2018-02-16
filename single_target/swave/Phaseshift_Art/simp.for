      OPEN(UNIT=17,FILE='phase_s.dat',STATUS='OLD',ACCESS=
     $     'SEQUENTIAL')
      open(unit=12,FILE='phase_s2.dat')
      


      do 1 i=1,400
      read(17,*)ef,phase
      write(12,*)phase
      read(17,*)ef,phase
 1    continue

      stop
      end
