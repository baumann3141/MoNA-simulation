
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
export MAP_LOCAL_Z="/projects/mona-sim/kohley/st_mona/maps/"

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

#echo "// forward map"
#mkMapEntry m12Be_06025   12  4  1.5791  3.3228  /projects/06025/root_files/maps/Be12_9_365kG_3_3228Tm_-03deg_050pos.map     # DV 10/27/16 had to comment out to compile.
#echo "// partial inverse map"
#mkMapEntry m12Be_06025i  12  4  1.5791  3.3228  /projects/06025/root_files/maps/Be12_9_365kG_3_3228Tm_-03deg_050pos.imap

echo "// forward map"
mkMapEntry m23O_10023   23  8  1.5791  3.5607  /projects/mona-sim/votaw_stmona/st_mona/maps/z8a23_Hp10_0693_Bp3.5607_order3_angle-0.3_pos0.50.map
echo "// partial inverse map"
mkMapEntry m23O_10023i  23  8  1.5791  3.5607  /projects/mona-sim/votaw_stmona/st_mona/maps/z8a23_Hp10_0693_Bp3.5607_order3_angle-0.3_pos0.50.imap

#echo "// forward map"
#mkMapEntry m14Be_06025   14  4  1.5791  3.3228  /projects/06025/root_files/maps/Be14_9_365kG_3_3228Tm_-03deg_050pos.map     # DV 10/27/16 had to comment out to compile.
#echo "// partial inverse map"
#mkMapEntry m14Be_06025i  14  4  1.5791  3.3228  /projects/06025/root_files/maps/Be14_9_365kG_3_3228Tm_-03deg_050pos.imap

echo "// forward map"
mkMapEntry m7Li_e11028  7  3  1.55  2.9695  /projects/e11028/n2analysis/maps/z3a7_Hp8_43287_Bp2.9695_order3_angle-0.3_pos0.50_DL1.55.map
echo "// partial inverse map"
mkMapEntry m7Li_e11028i  7  3  1.55  2.9695  /projects/e11028/n2analysis/maps/z3a7_Hp8_43287_Bp2.9695_order3_angle-0.3_pos0.50_DL1.55.imap

echo "// forward map"
mkMapEntry m6He_e11028  6  2  1.5791  3.9121  /projects/e11028/n2analysis/maps/z2a6_Hp10_9544_Bp3.9121_order3_angle-0.3_pos0.50_DL1.5791.map
#mkMapEntry m6He_e11028  6  2  1.5791  3.9121  /projects/e11028/n2analysis/maps/z2a6_Hp10_9544_Bp3.9121_drift1.5791_DL_0.8.map
echo "// partial inverse map"
mkMapEntry m6He_e11028i  6  2  1.5791  3.9121 /projects/e11028/n2analysis/maps/z2a6_Hp10_9544_Bp3.9121_order3_angle-0.3_pos0.50_DL1.5791.imap
#mkMapEntry m6He_e11028i  6  2  1.5791  3.9121  /projects/e11028/n2analysis/maps/z2a6_Hp10_9544_Bp3.9121_drift1.5791_DL_0.8.imap

echo "// forward map"
mkMapEntry m11Be_e15091  11  4  1.5791  2.60437  /projects/e15091/analysis_master/maps/Z4_A11_Bp2.60437_11Be.map
echo "// partial inverse map"
mkMapEntry m11Be_e15091i  11  4  1.5791  2.60437 /projects/e15091/analysis_master/maps/Z4_A11_Bp2.60437_11Be.imap

echo "// forward map"
mkMapEntry m12B_e15091  12  5  1.5791  2.25474  /projects/e15091/analysis_master/maps/Z5_A12_Bp2.25474_12B.map
echo "// partial inverse map"
mkMapEntry m12B_e15091i  12  5  1.5791  2.25474 /projects/e15091/analysis_master/maps/Z5_A12_Bp2.25474_12B.imap

echo "// forward map"
mkMapEntry m8He_11Be_e15091  8  2  1.5791  3.80893  /projects/e15091/analysis_master/maps/Z2_A8_Bp3.80893_11Be.map
echo "// partial inverse map"
mkMapEntry m8He_11Be_e15091i  8  2  1.5791  3.80893 /projects/e15091/analysis_master/maps/Z2_A8_Bp3.80893_11Be.imap

echo "// forward map"
mkMapEntry m8He_12B_e15091  8  2  1.5791  3.806925  /projects/e15091/analysis_master/maps/Z2_A8_Bp3.806925_12B.map
echo "// partial inverse map"
mkMapEntry m8He_12B_e15091i  8  2  1.5791  3.806925 /projects/e15091/analysis_master/maps/Z2_A8_Bp3.806925_12B.imap
