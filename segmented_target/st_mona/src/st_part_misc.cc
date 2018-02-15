/** \file
 * Implementation of particle initializers
 */

#include "st_part_misc.hh"
#include <cmath>

/// constructor
// for making weird input beam energy profiles THR 5 September 2017
StPartInitRandomMisc::StPartInitRandomMisc(
  StRNG* rngx, 
  StRNG* rngtx, 
  StRNG* rngy, 
  StRNG* rngty, 
  StRNG* rnge1,
  StRNG* rnge2,
  double xtx_slope,
  double yty_slope
                   ) :  StPartInitializer("StPartInitRandomMisc"), 
			rx(rngx), 
			rtx(rngtx), 
			ry(rngy), 
			rty(rngty), 
			re1(rnge1),
			re2(rnge2){
                                   sl_xtx=xtx_slope;
				   sl_yty=yty_slope;
                        }


int StPartInitRandomMisc::act(StParticle* p) {
  p->setz(0);
  p->setpath(0);
  p->sett(0);
  p->setx   (rx  ?  rx->value() : 0 );
  p->sety   (ry  ?  ry->value() : 0 );
  p->settx  (rtx ? rtx->value() : 0 );
  p->setty  (rty ? rty->value() : 0 );
//  p->setEkin(re  ?  re->value() : 0 );
  // THR 1 Sept 2017
  //p->setEkin(p->getEkin()*0.178537);

  double r1 =0;
  while(1){
    r1 = re1->value(); double r2 = re2->value();
    double fx = liseFit(r1);
    if(r2 < fx) break;
  }

  p->setEkin(r1*p->getA());

  // rotate xtx and yty
  // note that if you do this, dx, dtx, dy, and dty are no longer widths of those distributions, but widths of the axes of the ellipse that is being rotated.
  double ang1 = atan(sl_xtx);
  double xi  = p->getx();
  double txi = p->gettx();
  double yi  = p->gety();
  double tyi = p->getty();
  p->setx  ( xi*cos(ang1) - txi*sin(ang1) );
  p->settx ( xi*sin(ang1) + txi*cos(ang1) );
  double ang2 = atan(sl_yty);
  p->sety  ( yi*cos(ang2) - tyi*sin(ang2) );
  p->setty ( yi*sin(ang2) + tyi*cos(ang2) );

  return 0;
}
