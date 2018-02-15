/** \file
 * apertures
 */

/** Class for apertures
 * 
 */
class StAperture : public StBLE {
public:
  StAperture(string name) : StBLE(name) {}
};

/** A RECTANGULAR (!!!) Aperture (not SQUARE!!!)
 *
 */
class StApertureSquare : public StAperture {
public:
  /** Constructor
   *  \param xl  Lower Value
   *  \param xu  Upper Value
   *  \param yl  Lower Value
   *  \param yu  Upper Value
   *  \param flag  Flag: 0 - act on all particles, 1 - act only on charged
   */
  StApertureSquare(double xl, double xu, double yl, double yu, int flag=0) : 
    StAperture("StApertureSquare"), fXl(xl), fXu(xu), fYl(yl), fYu(yu), 
    fActOnlyOnCharged(flag) {}
  
  /// action
  int act(StParticle* p ) {
    if (fActOnlyOnCharged && p->getZ() == 0) return 0;  // act only on charged particles
    if ( p->getx() < fXl || 
         p->getx() > fXu || 
         p->gety() < fYl || 
         p->gety() > fYu ) {
      p->setEkin(0);
    }
    return 0;
  }

private:
  double fXl, fXu, fYl, fYu;
  int fActOnlyOnCharged;
};


