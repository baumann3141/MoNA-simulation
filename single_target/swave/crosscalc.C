#include <complex>
#include <iostream>
typedef std::complex<double> compd;
class crosscalc
{
    public:
        // data members
        double ca;
        double pi;
        double v;
        double a;
        compd ii;
        double m;
        double h;
        double ei;
        double gamma;
        double Vun;
        double rs;
        double de;
        double dz;
        double db;
        double li;
        double mi;
        double lf;
        double mf;
        double constant;
        double ef;
    public:
        // functions
        compd hankel(double i, compd r);
        double harm(double l, double m, double x, double z);
        double calculate_cross(double phase);

};

compd
crosscalc::hankel(double i,compd r)
{
    compd II(0.,1.);
    compd hnk(0.,0.);
    if (i == 0)
    {
        hnk=(-1./(-II*r))*std::exp(-(-II*r));
    } else if (i == 1)
    {
        hnk=II*(1./(-II*r)+1./(std::pow(-II*r,2)))*std::exp(-(-II*r));
    } else if (i == 2)
    {
        hnk=(1./(-II*r)+3./(std::pow(-II*r,2))+3./(std::pow(-II*r,3)))*std::exp(-(-II*r));
    }
    return hnk;
}

double
crosscalc::harm(double l, double m, double x, double z)
{
    double harm = 0.;
    if (l == 0.)
    {
        harm=1./(std::sqrt(4.*pi));
    } else if (l == 1.)
    {
        if (m == 0) 
        {
            harm=std::sqrt(3./(4.*pi))*z/std::sqrt(std::pow(x,2)+std::pow(z,2));
        } else if (m == 1) 
        {
            harm=-std::sqrt(3./(8.*pi))*x/std::sqrt(std::pow(x,2)+std::pow(z,2));
        }
    } else if (l == 2.)
    {
        if (m == 0) 
        {
            harm=std::sqrt(5./(16.*pi))*((3.*(std::pow(z,2))/(std::pow(x,2)+std::pow(z,2)))-1.);
        } else if (m == 1) 
        {
            harm=-std::sqrt(15./(8.*pi))*x*z/(std::pow(x,2)+std::pow(z,2));
        } else if (m == 2) 
        {
            harm=std::sqrt(15./(32.*pi))*(std::pow(x,2))/(std::pow(x,2)+std::pow(z,2));
        }
    }
    return harm;
}

double
crosscalc::calculate_cross(double phase)
{
    ii = compd(0.,1.);

    double kf=sqrt(2.*m*ef)/h;
    double const2=constant*kf;

    double b=3.;
    double crossfin=0.;
    //integrale sur impact parameter
    //do 2 j=1,292      
    //for (int j=0;j<1;j++)
    for (int j=0;j<292;++j)
    {
        //cout << j << endl;
        b=b+db;
        //cout << "b " << b << endl;
        compd cro(0.,0.);
        double z=0.-dz;
        //integrale sur z
        //do 3 k=1,21000
        for (int k=0; k<21000;++k)
        {
            z=z+dz;
            //cout << "z " << z << endl;
            double q=(ef-ei)/(h*v);   
            //cout << "q " << q << endl;
            double ep=cos(q*z);
            //cout << "ep " << ep << endl;

            double r=std::sqrt(pow(b,2)+pow(z,2));
            //cout << "r " << r << endl;
            compd ri=ii*gamma*r;
            //cout << "ri " << ri << endl;
            compd rf=kf*r;
            //cout << "rf " << rf << endl;
            compd bki=hankel(li,ri);
            //cout << "bki " << bki << endl;
            compd bkf=ii*hankel(lf,rf);
            //cout << "bkf " << bkf << endl;
            double harmi=harm(li,mi,b,z);
            //cout << "harmi " << harmi << endl;
            double harmf=harm(lf,mf,b,z);
            //cout << "harmf " << harmf << endl;

            compd gui=ep*bki*bkf*harmi*harmf;
            //cout << "gui " << gui << endl;
            cro=cro+gui*dz;
            //cout << "cro " << cro << endl;
        }
        //fin integrale sur z

        //argument
        cro=pow(ii,li)*cro*2.;
        //cout << "cro " << cro << endl;
        compd realp=(cro+conj(cro));
        //cout << "realp " << realp << endl;
        compd imp=(cro-(conj(cro)))*(-1.*ii);
        //cout << "imp " << imp << endl;
        double arg=atan2(real(imp),real(realp));
        //cout << "arg " << arg << endl;
        //norme
        //cout << const2 << " " << abs(cro) << " " << pow(abs(cro),2) << endl;
        double norm=const2*pow(abs(cro),2);
        //cout << "norm " << norm << endl;
        // print*,b,ef,arg,norm      
        double partel=(pow(1.-cos(2.*(phase+arg)),2))+pow(sin(2.*(phase+arg)),2);
        //cout << "partel " << partel << endl;
        double cross=norm*partel;
        //cout << "cross " << cross << endl;
        crossfin=crossfin+cross*exp(-log(2.)*exp((rs-b)/a))*b*db;
        //cout << "crossfin " << crossfin << endl;
    }
    return crossfin;
}
