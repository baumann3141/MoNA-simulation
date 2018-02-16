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
  double yty_slope,
  //ANK: added on 6-1-2016 for multiple component beam distribution
  
  StRNG* rngx2, 
  StRNG* rngtx2, 
  StRNG* rngy2, 
  StRNG* rngty2, 
  double xtx_slope2,
  double yty_slope2,
  StRNG* rngx3, 
  StRNG* rngtx3, 
  StRNG* rngy3, 
  StRNG* rngty3,
  double xtx_slope3,
  double yty_slope3,
  double xy_slope3,
  StRNG* r2,
  double normscale1,
  double normscale2
  
  //ANK
                   ) :  StPartInitializer("StPartInitRandomGauss"), 
			rx(rngx), 
			rtx(rngtx), 
			ry(rngy), 
			rty(rngty), 
			re(rnge),
			//ANK
			
			rx2(rngx2), 
			rtx2(rngtx2), 
			ry2(rngy2), 
			rty2(rngty2),
			rx3(rngx3), 
			rtx3(rngtx3), 
			ry3(rngy3), 
			rty3(rngty3),
			random(r2)
			
			//ANK
{
  sl_xtx=xtx_slope;
  sl_yty=yty_slope;
  //ANK
  
  sl_xtx2=xtx_slope2;
  sl_yty2=yty_slope2;
  sl_xtx3=xtx_slope3;
  sl_yty3=yty_slope3;
  sl_xy3=xy_slope3;
  s1=normscale1;
  s2=normscale2;
  
  //ANK
}

/*
int StPartInitRandomGauss::act(StParticle* p) {
  p->setz(0);
  p->setpath(0);
  p->sett(0);
  p->setx   (rx  ?  rx->value() : 0 );
  p->sety   (ry  ?  ry->value() : 0 );
  p->settx  (rtx ? rtx->value() : 0 );
  p->setty  (rty ? rty->value() : 0 );
  p->setEkin(re  ?  re->value() : 0 );

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
*/

//ANK: below is from JKS

int StPartInitRandomGauss::act(StParticle* p) {
  p->setz(0);
  p->setpath(0);
  p->sett(0);
  double rand = random->value();
  double xpos,ypos,txpos,typos;
  double ang1 = 0;
  double ang2 = 0;
  double xc = 0;
  double txc = 0;
  double yc = 0;
  double tyc = 0;
  if (rand<s1)
  {
  	xpos = rx->value();
	ypos = ry->value();
	txpos = rtx->value();
	typos = rty->value();
  }
  else if (rand>s1 && rand<s2)
  {
  	xpos = rx2->value();
	ypos = ry2->value();
	txpos = rtx2->value();
	typos = rty2->value();
  }
  else
  {
  	xpos = rx3->value();
	xc = rx3->getCentroid();
	ypos = ry3->value() + sl_xy3*(xpos-xc); // allow for a slope of correlation between x and y.
	txpos = rtx3->value();
	typos = rty3->value();
  }
  p->setx   (xpos);
  p->sety   (ypos);
  p->settx  (txpos);
  p->setty  (typos);
  p->setEkin(re  ?  re->value() : 0 );

  // rotate xtx and yty
  // note that if you do this, dx, dtx, dy, and dty are no longer widths of those distributions, but widths of the axes of the ellipse that is being rotated.
  if (rand<s1)
  {
	  ang1 = atan(sl_xtx);
	  ang2 = atan(sl_yty);
	  xc = rx->getCentroid();
	  txc = rtx->getCentroid();
	  yc = ry->getCentroid();
	  tyc = rty->getCentroid();
  }
  else if (rand>s1 && rand<s2)
  {
	  ang1 = atan(sl_xtx2);
	  ang2 = atan(sl_yty2);
	  xc = rx2->getCentroid();
	  txc = rtx2->getCentroid();
	  yc = ry2->getCentroid();
	  tyc = rty2->getCentroid();
  }
  else
  {
	  ang1 = atan(sl_xtx3);
	  ang2 = atan(sl_yty3);
	  xc = rx3->getCentroid();
	  txc = rtx3->getCentroid();
	  yc = ry3->getCentroid();
	  tyc = rty3->getCentroid();
  }

  double xi  = p->getx();
  double txi = p->gettx();
  double yi  = p->gety();
  double tyi = p->getty();
  p->setx  ( (xi-xc)*cos(ang1) - (txi-txc)*sin(ang1) + xc );
  p->settx ( (xi-xc)*sin(ang1) + (txi-txc)*cos(ang1) + txc );
  p->sety  ( (yi-yc)*cos(ang2) - (tyi-tyc)*sin(ang2) + yc );
  p->setty ( (yi-yc)*sin(ang2) + (tyi-tyc)*cos(ang2) + tyc );

  return 0;
}


