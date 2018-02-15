/** \file
 * Implementation of particle initializers
 */

#include "st_part_init.hh"
#include <cmath>

/// constructor
/// \todo change order to match order in particle struct x,tx,y,ty,...
StPartInitRandomGauss::StPartInitRandomGauss(
  StRNG* rngx, 
  StRNG* rngtx, 
  StRNG* rngy, 
  StRNG* rngty, 
  StRNG* rnge,
  double xtx_slope,
  double yty_slope
                   ) :  StPartInitializer("StPartInitRandomGauss"), 
			rx(rngx), 
			rtx(rngtx), 
			ry(rngy), 
			rty(rngty), 
			re(rnge) {
                                   sl_xtx=xtx_slope;
				   sl_yty=yty_slope;
                        }


int StPartInitRandomGauss::act(StParticle* p) {
  p->setz(0);
  p->setpath(0);
  p->sett(0);
  p->setx   (rx  ?  rx->value() : 0 );
  p->sety   (ry  ?  ry->value() : 0 );
  p->settx  (rtx ? rtx->value() : 0 );
  p->setty  (rty ? rty->value() : 0 );
  p->setEkin(re  ?  re->value() : 0 );
  // THR 1 Sept 2017
  //p->setEkin(p->getEkin()*0.178537);

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
