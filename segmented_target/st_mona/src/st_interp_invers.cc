/** \file
 * Interpolation and inverse of function
 */

#include "st_interp_invers.hh"

void StInterp::add(double x, double y) {
  if (fXarray) ERR("Cannot add data after init of Interpolator");
  DEB("add");
  Data d;
  d.x=x;
  d.y=y;
  fData.insert(d);
  DSV(fData.size());
}


void StInterp::print() {
  DEB("print");
  DSV(fData.size());
  for (set<Data,ltData>::const_iterator it=fData.begin(); it!=fData.end(); it++) {
    printf("x, y:  %10g %10g\n",it->x,it->y);
  }
}


void StInterpLin::setup() {
  bool soFarNextIsLarger;
  bool soFarNextIsSmaller;
  fXarray = (double*) malloc(sizeof(double)*fData.size());
  fYarray = (double*) malloc(sizeof(double)*fData.size());
  if (!fXarray) ERR("!fXarray");
  if (!fYarray) ERR("!fYarray");
  size_t i=0;
    
  for (set<Data,ltData>::const_iterator it=fData.begin(); 
       it != fData.end(); it++) {
    fXarray[i]=it->x;
    fYarray[i]=it->y;
    i++;
  }
  // check if y is ordered also
  soFarNextIsLarger = true;
  soFarNextIsSmaller = true;
  for (size_t i=0; i < fData.size() - 1; i++) {
    if (soFarNextIsLarger  && (fYarray[i] >= fYarray[i+1])) soFarNextIsLarger=false;
    if (soFarNextIsSmaller && (fYarray[i] <= fYarray[i+1])) soFarNextIsSmaller=false;
    DSV(soFarNextIsLarger);
    DSV(soFarNextIsSmaller);
  }
  if (soFarNextIsLarger || soFarNextIsSmaller) fYisOrdered = true;
          
  fGSLInterp    = gsl_interp_alloc (gsl_interp_linear, fData.size());
  fGSLInterpInv = gsl_interp_alloc (gsl_interp_linear, fData.size());
  if (!fGSLInterp)    ERR("!fGSLInterp");
  if (!fGSLInterpInv) ERR("!fGSLInterp");
  gsl_interp_init (fGSLInterp,    fXarray, fYarray, fData.size());
  gsl_interp_init (fGSLInterpInv, fYarray, fXarray, fData.size());
}


double StInterpLin::operator()(double v){
  if (fGSLInterp && fXarray && fYarray) {
    double xMin, xMax;
    xMin = fXarray[0];
    xMax = fXarray[fData.size()-1];
    //zwk to much text output
    //if (v < xMin) WAR("Extrapolation: v, x_min  %.19f %.19f",v,xMin);
    //if (v > xMax) WAR("Extrapolation: v, x_max  %.19f %.19f",v,xMax);
    return gsl_interp_eval(fGSLInterp, fXarray, fYarray, v,0);
  } else ERR("something is wrong; pointer not initialized");
}


double StInterpLin::inv(double v) {
  if (!fYisOrdered) ERR("Y is not ordered; inverse cannot be calculated");
  if (fGSLInterpInv && fXarray && fYarray) {
    double yMin, yMax;
    yMin = fYarray[0];
    yMax = fYarray[fData.size()-1];
    if (yMin > yMax) {
      double t(yMin); 
      yMin = yMax;
      yMax = t;
    }
    //zwk to much text output
    //if (v < yMin || v > yMax) WAR("Extrapolation: v, y_min, y_max  %.19f %.19f %.19f",v,yMin,yMax);
    return  gsl_interp_eval(fGSLInterpInv, fYarray, fXarray, v,0);
  }
  else ERR("something is wrong; pointer not initialized");
}


void StInterpLin::reset() {
  if (fGSLInterp)    gsl_interp_free(fGSLInterp);    
  if (fGSLInterpInv) gsl_interp_free(fGSLInterpInv);
  if (fXarray)       free(fXarray);
  if (fYarray)       free(fYarray);
  fGSLInterp = 0;
  fGSLInterpInv = 0;
  fXarray = 0;
  fYarray = 0;
  fData.clear();
  fYisOrdered = false;
}



#ifdef TEST

int main(int argc, char* argv[]) {
  StInterpLin lut;
  lut.add(1,.2);
  lut.add(2,1);
  lut.add(3,4);
  lut.print();
  lut.setup();
  DSV(lut(1.5));
  DSV(lut(1.1));
  DSV(lut(2.3));
  DSV(lut.val(2.3));
  DSV(lut.inv(1.9));
  for (double i=0.0; i <= 5.0001; i+=.1) printf("%.5f %.5f %.5f\n",i,lut(i),lut.inv(i));

  DEB("Before reset");
  lut.print();
  lut.reset();
  DEB("After reset");
  lut.print();

  lut.add(1,.2);
  lut.add(2,1);
  lut.add(3,4);
  lut.add(2.5,5);  
  lut.print();
  lut.setup();
  DSV(lut(1.5));
  DSV(lut(1.1));
  DSV(lut(2.3));
  DSV(lut(2.5));
  DSV(lut(2.7));
  DSV(lut.val(2.3));
  DSV(lut.inv(1.9));   // should get error message
}

#endif

