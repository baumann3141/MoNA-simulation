#!/usr/bin/gawk -f

{
  $1="";
  $0=$0;
  
  if (NR==1) {
    for (i=1; i<=NF; i++) {
      printf("%8.1d ",i-1);
    }
    print "";
  }

  for (i=1; i<=NF; i++) printf("%8.1e ",$i);
  print "";
}
