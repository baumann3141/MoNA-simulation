
#!/bin/bash

# generate maps.icc file

mkMapEntry () {
    test -f $6 || { echo "The file $6 does not exist.  Skipping $1." 1>&2; return -1; }
    echo "CosyMap $1;"
    echo "$1.setA($2);"
    echo "$1.setQ($3);"
    echo "$1.setLen($4);"
    echo "$1.setBrho($5);"
    echo "$1.read("
    gawk '{printf "\"%s\\n\"\n",$0}' $6
    echo ");"
    echo
    echo
    return 0
}
export MAP_LOCATION="/user/monasoft/st_mona/maps/"
export MAP_LOCAL_Z="/projects/mona-sim/jonesm/st_mona/maps/"

echo "// -*- mode: c -*-"
echo "// forward map"
       #         // A  Z  LENGTH  BRHO     FILENAME
mkMapEntry m23O   23  8  1.5741  3.52319  ${MAP_LOCATION}floating/23o-5_82-jun1.map
echo "// partial inverse map"
mkMapEntry m23Oi  23  8  1.5741  3.52319  ${MAP_LOCATION}floating/23o-5_82-jun1.imap

echo "// forward map"
mkMapEntry m22O   22  8  1.5741  3.52319  ${MAP_LOCATION}floating/22o-5_82-jul20.map
echo "// partial inverse map"
mkMapEntry m22Oi  22  8  1.5741  3.52319  ${MAP_LOCATION}floating/22o-5_82-jul20.imap

echo "// forward map"
mkMapEntry m6He    6  2  1.5741  2.77742  ${MAP_LOCATION}floating/6he-jun5-4_85.map
echo "// partial inverse map"
mkMapEntry m6Hei   6  2  1.5741  2.77742  ${MAP_LOCATION}floating/6he-jun5-4_85.imap

echo "// forward map"
mkMapEntry m8Li    8  3  1.5741  2.5632  ${MAP_LOCATION}floating/8li-jun19-4_4446.map
echo "// partial inverse map"
mkMapEntry m8Lii   8  3  1.5741  2.5632  ${MAP_LOCATION}floating/8li-jun19-4_4446.imap

echo "// forward map"
mkMapEntry m10Bea   10   4  1.5741  3.6375  ${MAP_LOCATION}floating/coulex_be10.map
echo "// partial inverse map"		         
mkMapEntry m10Beai   10  4  1.5741  3.6375  ${MAP_LOCATION}floating/coulex_be10.imap

echo "// forward map"
mkMapEntry m10Beb   10   4  1.5741  3.5668 ${MAP_LOCATION}floating/coulex_coinc_Be10.map
echo "// partial inverse map"		        
mkMapEntry m10Bebi   10  4  1.5741  3.5668 ${MAP_LOCATION}floating/coulex_coinc_Be10.imap

echo "// forward map"
mkMapEntry m11Bec   11   4  1.5741  3.6375 ${MAP_LOCATION}floating/03048_Be11.map
echo "// partial inverse map"		        
mkMapEntry m11Beci   11  4  1.5741  3.6375 ${MAP_LOCATION}floating/03048_Be11.imap

echo "// forward map"
mkMapEntry m24O   24  8  1.5741  3.77548  ${MAP_LOCATION}floating/24o-cosy.map
echo "// partial inverse map"		       
mkMapEntry m24Oi  24  8  1.5741  3.77548  ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m26f   26  9  1.5741  3.77548  ${MAP_LOCATION}floating/26f-6_542.map
echo "// partial inverse map"
mkMapEntry m26fi  26  9  1.5741  3.77548  ${MAP_LOCATION}floating/26f-6_542_sch.imap

echo "// forward map"
mkMapEntry m23Oa   23  8  1.5741  3.77548  ${MAP_LOCATION}floating/23o-6_542.map
echo "// partial inverse map"		        
mkMapEntry m23Oai  23  8  1.5741  3.77548  ${MAP_LOCATION}floating/23o-6_542_sch.imap

echo "// forward map"
mkMapEntry m22Oa   22  8  1.5741  3.77548  ${MAP_LOCATION}floating/22o-6_542.map
echo "// partial inverse map"		        
mkMapEntry m22Oai  22  8  1.5741  3.77548  ${MAP_LOCATION}floating/22o-6_542_sch.imap

echo "// forward map"
mkMapEntry m20O   20  8  3.926  2.9041  ${MAP_LOCATION}floating/z8n12_coinc.map
echo "// partial inverse map"		     
mkMapEntry m20Oi  20  8  3.926  2.9041  ${MAP_LOCATION}floating/z8n12_coinc.imap

echo "// forward map"
mkMapEntry m13B   13  5  3.926  2.9041   ${MAP_LOCATION}floating/z5n8.map
echo "// partial inverse map"		      
mkMapEntry m13Bi  13  5  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m25Ne   25  10  3.926  2.9041   ${MAP_LOCATION}floating/z10n15.map
echo "// partial inverse map"		        
mkMapEntry m25Nei  25  10  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m16C   16  6  3.926  2.9041   ${MAP_LOCATION}floating/z6n10.map
echo "// partial inverse map"		      
mkMapEntry m16Ci  16  6  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m19O   19  8  3.926  2.9041   ${MAP_LOCATION}floating/19O_coinc.map
echo "// partial inverse map"		      
mkMapEntry m19Oi  19  8  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m18Of   18  8  3.926  2.9041   ${MAP_LOCATION}floating/18Ofrag.map
echo "// partial inverse map"		       
mkMapEntry m18Ofi  18  8  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m21Of   21  8  3.926  2.9041   ${MAP_LOCATION}floating/21Ofrag.map
echo "// partial inverse map"		       
mkMapEntry m21Ofi  21  8  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

echo "// forward map"
mkMapEntry m22Of   22  8  3.926  2.9041   ${MAP_LOCATION}floating/22Ofrag.map
echo "// partial inverse map"		       
mkMapEntry m22Ofi  22  8  3.926  2.9041   ${MAP_LOCATION}floating/24o-sch.imap

#echo "// forward map"
#mkMapEntry m9Li   9  3  3.926  2.9041   ${MAP_LOCATION}floating/z3n6_05124.map
#echo "// partial inverse map"		      
#mkMapEntry m9Lii  9  3  3.926  2.9041   ${MAP_LOCATION}floating/05124_z3n6.imap

echo "// forward map"
mkMapEntry m9Li    9  3  1.5741  2.77742  ${MAP_LOCATION}floating/6he-jun5-4_85.map
echo "// partial inverse map"		       
mkMapEntry m9Lii   9  3  1.5741  2.77742  ${MAP_LOCATION}floating/6he-jun5-4_85.imap

echo "// forward map"
mkMapEntry m11Li   11  3  1.5741  3.813  ${MAP_LOCATION}floating/11li-6_5991-jun19.map
echo "// partial inverse map"		      
mkMapEntry m11Lii  11  3  1.5741  3.813  ${MAP_LOCATION}floating/11li-6_5991-jun19.imap

echo "// forward map"
mkMapEntry m11Li_BeBeam   11  3  1.5741  3.813  ${MAP_LOCATION}floating/11li-6_5991-jun19.map
echo "// partial inverse map"		 	     	       
mkMapEntry m11Lii_BeBeam  11  3  1.5741  3.813  ${MAP_LOCATION}floating/11li-6_5991-jun19.imap

echo "// forward map"
mkMapEntry m14Be   14  4  1.5741  3.658  ${MAP_LOCATION}floating/11li-6_5991-jun19.map
echo "// partial inverse map"		 	     	       
mkMapEntry m14Bei  14  4  1.5741  3.658  ${MAP_LOCATION}floating/11li-6_5991-jun19.imap

echo "// forward map"
mkMapEntry mJenna    9  3  1.5741  3.81832  ${MAP_LOCATION}floating/6he-jun5-4_85.map
echo "// partial inverse map"		       
mkMapEntry mJennai   9  3  1.5741  3.81832  ${MAP_LOCATION}floating/6he-jun5-4_85.imap

echo "// forward map"
mkMapEntry m15B_06025   15  5  1.5791  3.3228  /projects/mona-sim/kohley/st_mona/maps/B15_9_365kG_3_3228Tm_-03deg_050pos.map
echo "// partial inverse map"
mkMapEntry m15B_06025i  15  5  1.5791  3.3228  /projects/mona-sim/kohley/st_mona/maps/B15_9_365kG_3_3228Tm_-03deg_050pos_5param.imap

echo "// forward map"
mkMapEntry m14Be_B17beam    14  4  1.5791  3.560  /projects/mona-sim/kohley/st_mona/maps/Be14_10_040kG_3_560Tm_-03deg_050pos.map
echo "// partial inverse map"		       
mkMapEntry m14Be_B17beami   14  4  1.5791  3.560  /projects/mona-sim/kohley/st_mona/maps/Be14_10_040kG_3_560Tm_-03deg_050pos.imap

echo "// forward map"
mkMapEntry m24o_hope    24  8  1.5791  3.5611  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp9_9963_Bp3.5611_order3.map
echo "// partial inverse map"		       
mkMapEntry m24o_hopei   24  8  1.5791  3.5611  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp9_9963_Bp3.5611_order3.imap

echo "// forward map"
mkMapEntry m26ne_calem    26  10  1.5741  3.5611  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp9_9963_Bp3.5611_order3.map
echo "// partial inverse map"		       
mkMapEntry m26ne_calemi   26  10  1.5741  3.5611  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp9_9963_Bp3.5611_order3.imap

echo "// forward map"
mkMapEntry m8he_jesse    8  2  1.5791  3.5151  /projects/mona-sim/kohley/st_mona/maps/z2a8_Hp9_95022_Bp3.5151_order3.map
echo "// partial inverse map"		       
mkMapEntry m8he_jessei   8  2  1.5791  3.5151  /projects/mona-sim/kohley/st_mona/maps/z2a8_Hp9_95022_Bp3.5151_order3.imap

echo "// forward map"
mkMapEntry m12be_jesse    12  4  1.5791  3.1866  /projects/mona-sim/kohley/st_mona/maps/z4a12_Hp9_0604_Bp3.1866_order3.map
echo "// partial inverse map"		       
mkMapEntry m12be_jessei   12  4  1.5791  3.1866  /projects/mona-sim/kohley/st_mona/maps/z4a12_Hp9_0604_Bp3.1866_order3.imap

echo "// forward map"
mkMapEntry m23o_09028    23  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a23_Hp10_0655_Bp3.5595_order3.map
echo "// partial inverse map"		       
mkMapEntry m23o_09028i   23  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a23_Hp10_0655_Bp3.5595_order3.imap

echo "// forward map"
mkMapEntry m22o_09028    22  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a22_Hp10_0655_Bp3.5595_order3.map
echo "// partial inverse map"		       
mkMapEntry m22o_09028i   22  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a22_Hp10_0655_Bp3.5595_order3.imap

echo "// forward map"
mkMapEntry m24o_09028    24  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp10_0655_Bp3.5595_order3.map
echo "// partial inverse map"		       
mkMapEntry m24o_09028i   24  8  1.5791  3.5595  /projects/mona-sim/kohley/st_mona/maps/z8a24_Hp10_0655_Bp3.5595_order3.imap

echo "// forward map"
mkMapEntry m24O_Jones    24  8  1.5595  3.8465  /projects/mona-sim/jonesm/st_mona/maps/e12004/z8a24_Hp10_7922_Bp3.8465_order3_angle-0.3_pos0.50_DL1.5595.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jonesi   24  8  1.5595  3.8465  /projects/mona-sim/jonesm/st_mona/maps/e12004/z8a24_Hp10_7922_Bp3.8465_order3_angle-0.3_pos0.50_DL1.5595.imap

#echo "// forward map"
#mkMapEntry m24O_Jones    22  8  1.5595  3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/z8a22_Hp9_9824_Bp3.524_order3_angle-0.3_pos0.50_DL1.5595.map
#echo "// partial inverse map"		       
#mkMapEntry m24O_Jonesi   22  8  1.5595  3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/z8a22_Hp9_9824_Bp3.524_order3_angle-0.3_pos0.50_DL1.5595.imap



dL=1.5595

#sweep runs
echo "// forward map"
mkMapEntry m24O_Jones315A    20  8  $dL  3.494493  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp9_89681_Bp3.494493_order3_angle-0.3_pos0.50_DL${dL}_315A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones315Ai   20  8  $dL  3.494493  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp9_89681_Bp3.494493_order3_angle-0.3_pos0.50_DL${dL}_315A.imap

echo "// forward map"
mkMapEntry m24O_Jones325A    20  8  $dL  3.57145  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_10657_Bp3.57145_order3_angle-0.3_pos0.50_DL${dL}_325A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones325Ai   20  8  $dL  3.57145  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_10657_Bp3.57145_order3_angle-0.3_pos0.50_DL${dL}_325A.imap

echo "// forward map"
mkMapEntry m24O_Jones335A    20  8  $dL  3.65511  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_31298_Bp3.65511_order3_angle-0.3_pos0.50_DL${dL}_335A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones335Ai   20  8  $dL  3.65511  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_31298_Bp3.65511_order3_angle-0.3_pos0.50_DL${dL}_335A.imap

echo "// forward map"
mkMapEntry m24O_Jones345A    20  8  1.6397  3.73606  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_51628_Bp3.73606_DL1.6397_345A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones345Ai   20  8  1.6397  3.73606  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_51628_Bp3.73606_DL1.6397_345A.imap

echo "// forward map"
mkMapEntry m24O_Jones355A    20  8  $dL  3.81527  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_71625_Bp3.81527_order3_angle-0.3_pos0.50_DL${dL}_355A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones355Ai   20  8  $dL  3.81527  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a20_Hp10_71625_Bp3.81527_order3_angle-0.3_pos0.50_DL${dL}_355A.imap

echo "// forward map"
mkMapEntry m22O_Jones320A    22  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a22_Hp9_9824_Bp3.524_DL1.6397_320A.map
echo "// partial inverse map"		       
mkMapEntry m22O_Jones320Ai   22  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a22_Hp9_9824_Bp3.524_DL1.6397_320A.imap

echo "// forward map"
mkMapEntry m23O_Jones320A    23  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a23_Hp9_9824_Bp3.524_DL1.6397_320A.map
echo "// partial inverse map"		       
mkMapEntry m23O_Jones320Ai   23  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a23_Hp9_9824_Bp3.524_DL1.6397_320A.imap

echo "// forward map"
mkMapEntry m24O_Jones320A    24  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a24_Hp9_9824_Bp3.524_DL1.6397_320A.map
echo "// partial inverse map"		       
mkMapEntry m24O_Jones320Ai   24  8  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z8a24_Hp9_9824_Bp3.524_DL1.6397_320A.imap

echo "// forward map"
mkMapEntry m21N_Jones320A    21  7  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z7a21_Hp9_9824_Bp3.524_DL1.6397_320A.map
echo "// partial inverse map"		       
mkMapEntry m21N_Jones320Ai   21  7  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z7a21_Hp9_9824_Bp3.524_DL1.6397_320A.imap

echo "// forward map"
mkMapEntry m22N_Jones320A    22  7  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z7a22_Hp9_9824_Bp3.524_DL1.6397_320A.map
echo "// partial inverse map"		       
mkMapEntry m22N_Jones320Ai   22  7  1.6397 3.524  /projects/mona-sim/jonesm/st_mona/maps/e12004/sweep_runs/z7a22_Hp9_9824_Bp3.524_DL1.6397_320A.imap

# ---------- e15118 maps ---------- #

echo "// forward map"
mkMapEntry m24O_redpath      24  8  1.64   3.425  /projects/mona-sim/redpath/st_mona/maps/z8a24_Hp9_7124_Bp3.425_order3_angle-0.3_pos0.50_DL1.64.map
echo "//partial inverse map"
mkMapEntry m24O_redpathi     24  8  1.64   3.425  /projects/mona-sim/redpath/st_mona/maps/z8a24_Hp9_7124_Bp3.425_order3_angle-0.3_pos0.50_DL1.64.imap

# FIX THIS !!!! 26 October 2017

echo "// forawrd map"
mkMapEntry m22O_redpath	     22  8  1.64   3.445  /projects/e15118/n2analysis/maps/z8a22_Hp9_7124_Bp3.445_order3_angle-0.3_pos0.50_DL1.64.map
echo "//partial inverse map"
mkMapEntry m22O_redpathi     22  8  1.64   3.445  /projects/e15118/n2analysis/maps/z8a22_Hp9_7124_Bp3.445_order3_angle-0.3_pos0.50_DL1.64.imap

echo "// forawrd map"
mkMapEntry m27F_redpath	     27  9  1.64   3.425  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_7124_Bp3.425_order3_angle-0.3_pos0.50_DL1.64.map
echo "//partial inverse map"
mkMapEntry m27F_redpathi     27  9  1.64   3.425  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_7124_Bp3.425_order3_angle-0.3_pos0.50_DL1.64.imap

echo "// forawrd map, step sweep 1"
mkMapEntry m27Fsw1_redpath   27  9  1.64   3.124  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp8_8835_Bp3.124_order3_angle-0.3_pos0.50_DL1.64.map
echo "//partial inverse map"
mkMapEntry m27Fsw1_redpathi  27  9  1.64   3.124  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp8_8835_Bp3.124_order3_angle-0.3_pos0.50_DL1.64.imap

echo "// forawrd map, step sweep 2"
mkMapEntry m27Fsw2_redpath   27  9  1.64   3.208  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_1212_Bp3.208_order3_angle-0.3_pos0.50_DL1.64.map

echo "// forawrd map, step sweep 3"
mkMapEntry m27Fsw3_redpath   27  9  1.64   3.3025  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_3828_Bp3.3025_order3_angle-0.3_pos0.50_DL1.64.map

echo "// forawrd map, step sweep 4"
mkMapEntry m27Fsw4_redpath   27  9  1.64   3.3745  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_5775_Bp3.3745_order3_angle-0.3_pos0.50_DL1.64.map

echo "// forawrd map, step sweep 5"
mkMapEntry m27Fsw5_redpath   27  9  1.64   3.4573  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp9_7980_Bp3.4573_order3_angle-0.3_pos0.50_DL1.64.map

echo "// forawrd map, step sweep 6"
mkMapEntry m27Fsw6_redpath   27  9  1.64   3.539  /projects/mona-sim/redpath/st_mona/maps/z9a27_Hp10_0116_Bp3.539_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map"
#mkMapEntry m28Ne_redpath     28 10  1.64   3.445  /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp9_7124_Bp3.445_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map, step sweep 2"
#mkMapEntry m28Nesw2_redpath  28 10  1.64   3.238  /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp9_1212_Bp3.238_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map, step sweep 3"
#mkMapEntry m28Nesw3_redpath  28 10  1.64   3.3198 /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp9_3828_Bp3.3198_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map, step sweep 4"
#mkMapEntry m28Nesw4_redpath  28 10  1.64   3.3987 /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp9_5775_Bp3.3987_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map, step sweep 5"
#mkMapEntry m28Nesw5_redpath  28 10  1.64   3.4754 /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp9_7980_Bp3.4754_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map, step sweep 6"
#mkMapEntry m28Nesw6_redpath  28 10  1.64   3.5502 /projects/mona-sim/redpath/st_mona/maps/z10a28_Hp10_0116_Bp3.5502_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map"
#mkMapEntry m29Ne_redpath     29 10  1.64   3.445  /projects/mona-sim/redpath/st_mona/maps/z10a29_Hp9_7124_Bp3.445_order3_angle-0.3_pos0.50_DL1.64.map

#echo "// forward map"
#mkMapEntry m30Na_redpath     30 11  1.64   3.445  /projects/mona-sim/redpath/st_mona/maps/z11a30_Hp9_7124_Bp3.445_order3_angle-0.3_pos0.50_DL1.64.map
