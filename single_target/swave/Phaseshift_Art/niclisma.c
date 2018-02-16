/* niclisma.f -- translated by f2c (version 20090411).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    doublereal s;
    integer lstype, ispin;
} spin_;

#define spin_1 spin_

struct {
    doublereal param[13], rmax, dr, sr[1000]	/* was [250][4] */, si[1000]	
	    /* was [250][4] */;
    integer nint, mwvs;
} optout_;

#define optout_1 optout_

struct mxl_1_ {
    integer lmx, lmn, lset, ldum;
};

#define mxl_1 (*(struct mxl_1_ *) &mxl_)

struct optin_1_ {
    doublereal parin[13], parmin[13], parmax[13], ap3, athird, cmu, gamma, 
	    kwn, ksq, drin, rmaxin, amp, amt, alpar;
};

#define optin_1 (*(struct optin_1_ *) &optin_)

union {
    struct {
	doublereal sigma[250], sinsgc[250], cossgc[250], fc[251], fcp[250], 
		gc[251], gcp[250];
    } _1;
    struct {
	doublereal sigma[250], sinsgc[250], cossgc[250], f[251], fp[250], g[
		251], gp[250];
    } _2;
} couout_;

#define couout_1 (couout_._1)
#define couout_2 (couout_._2)

struct {
    doublereal drsq, udrsq[1000], wdrsq[1000], hrdrsq[1000], hidrsq[1000], 
	    vcdrsq[1000], rone, rtwo;
    integer wstwo, ico;
} pots_;

#define pots_1 pots_

struct {
    doublereal h__[1000];
} micso_;

#define micso_1 micso_

/* Initialized data */

struct {
    integer e_1[3];
    integer fill_2[1];
    } mxl_ = { 3, 1, 1 };

struct {
    doublereal fill_1[13];
    doublereal e_2[26];
    doublereal fill_3[6];
    doublereal e_4[2];
    doublereal fill_5[3];
    } optin_ = { {0}, 0., -10., .001, -1e3, -1e3, -10., .001, -15., -15., 
	    -10., 0., -10., 1e-5, 1e3, 5., 3., 1e3, 1e3, 5., 3., 30., 30., 5.,
	     3., 5., 1e4, {0}, .031, 20. };


/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__5 = 5;
static integer c__3 = 3;
static doublereal c_b41 = .33333333333333331;
static logical c_true = TRUE_;
static doublereal c_b115 = 1e30;
static doublereal c_b195 = 1e-16;

/* 	ijm=ij-2 */
/* 	if(ijm)777,771,772 */
/* 777	par1=30. */
/*  771	par1=-120. */
/*  772   par1=0. */
/* 	continue */
/* Main program */ int MAIN__(void)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[24];
    olist o__1;

    /* Builtin functions */
    integer s_rsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsle(void), s_wsle(cilist *), e_wsle(void), i_indx(char *, char 
	    *, ftnlen, ftnlen);
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    integer f_open(olist *);
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static char filename[20];
    static integer i__, j;
    static doublereal a1, a2, a4, a5, ab, ai;
    static integer ij;
    static doublereal as;
    static integer in;
    static doublereal ri, rs, ur, ws, zp, zt, ecm, asp, rsp, usp, sdfa, einc, 
	    dein, sdfc, sdfb;
    static integer maxe;
    static logical error;
    extern /* Subroutine */ int optmod_(logical *, logical *);

    /* Fortran I/O blocks */
    static cilist io___1 = { 0, 5, 0, 0, 0 };
    static cilist io___3 = { 0, 6, 0, 0, 0 };
    static cilist io___5 = { 0, 10, 0, 0, 0 };
    static cilist io___29 = { 0, 6, 0, 0, 0 };
    static cilist io___30 = { 0, 6, 0, 0, 0 };
    static cilist io___32 = { 0, 6, 0, 0, 0 };
    static cilist io___33 = { 0, 6, 0, 0, 0 };
    static cilist io___35 = { 0, 6, 0, 0, 0 };
    static cilist io___36 = { 0, 6, 0, 0, 0 };
    static cilist io___37 = { 0, 8, 0, 0, 0 };
    static cilist io___40 = { 0, 8, 0, 0, 0 };


/*      DATA PNAME /2HUS,2HRS,2HAS,2HWS,2HWD,2HRI,2HAI,3HUSP,3HWSP,3HRSP, */
/*     $3HASP,2HRC,4HNORM/ */
    s_rsle(&io___1);
    do_lio(&c__9, &c__1, filename, (ftnlen)20);
    e_rsle();
    s_wsle(&io___3);
    do_lio(&c__9, &c__1, "fichier input", (ftnlen)13);
    e_wsle();
    in = i_indx(filename, " ", (ftnlen)20, (ftnlen)1);
    o__1.oerr = 0;
    o__1.ounit = 10;
    o__1.ofnmlen = in + 3;
/* Writing concatenation */
    i__1[0] = in - 1, a__1[0] = filename;
    i__1[1] = 4, a__1[1] = ".inp";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)24);
    o__1.ofnm = ch__1;
    o__1.orl = 0;
    o__1.osta = "OLD";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    o__1.oerr = 0;
    o__1.ounit = 8;
    o__1.ofnmlen = 9;
    o__1.ofnm = "smali.dat";
    o__1.orl = 0;
    o__1.osta = "NEW";
    o__1.oacc = "SEQUENTIAL";
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
/*      OPEN(UNIT=13,FILE='volumeform.dat',STATUS='NEW',ACCESS='SEQUENTIAL') */
/*      OPEN(UNIT=14,FILE='so_form.dat',STATUS='NEW',ACCESS='SEQUENTIAL') */
/*      OPEN(UNIT=9,FILE='SMAbe.Top',STATUS='NEW',ACCESS='SEQUENTIAL') */
/*      OPEN(UNIT=12,FILE='SMApot.DAT',STATUS='NEW',ACCESS='SEQUENTIAL') */
    spin_1.lstype = 1;
    spin_1.ispin = 2;
    spin_1.s = .5f;
    s_rsle(&io___5);
    do_lio(&c__5, &c__1, (char *)&optin_1.amt, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&zt, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&ur, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&rs, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&as, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&ws, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&ri, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&ai, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&usp, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&rsp, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&asp, (ftnlen)sizeof(doublereal));
    do_lio(&c__3, &c__1, (char *)&maxe, (ftnlen)sizeof(integer));
    do_lio(&c__5, &c__1, (char *)&dein, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&optin_1.alpar, (ftnlen)sizeof(doublereal));
    e_rsle();
/* 9 3    39.8333 1.273987 .75 .0 1.368 .3   7.07 1.273987 .75 300 .017  3.33 */
    optin_1.amp = 1.f;
    zp = 0.f;
    optin_1.athird = pow_dd(&optin_1.amt, &c_b41);
    optin_1.ap3 = pow_dd(&optin_1.amp, &c_b41);
    optin_1.cmu = optin_1.amp * .04783258 * optin_1.amt / (optin_1.amp + 
	    optin_1.amt);
    optin_1.parin[0] = ur;
    optin_1.parin[2] = as;
    optin_1.parin[3] = ws;
    optin_1.parin[6] = ai;
    optin_1.parin[7] = usp;
    optin_1.parin[8] = 0.f;
/* wsp */
    optin_1.parin[9] = rsp;
    optin_1.parin[10] = asp;
    optin_1.parin[11] = 1.f;
    optin_1.parin[12] = 1.f;
/*        AEF=EEF */
    i__2 = maxe;
    for (i__ = 1; i__ <= i__2; ++i__) {
	einc = (real) i__ * dein;
	ij = (integer) (einc - 20);
	optin_1.parin[0] = ur;
/* -.145*einc!-24./9.!24*(N-Z)/A Perey */
	if (ij >= 0) {
	    goto L30;
	} else {
	    goto L20;
	}
L20:
	optin_1.parin[1] = rs;
	optin_1.parin[5] = ri;
	goto L12;
L30:
	optin_1.parin[1] = rs;
/* -0.005*ij */
	optin_1.parin[5] = ri;
/* -0.0025*ij */
	optin_1.parin[3] = 0.f;
/* einc*0.375-7.5!WV */
	goto L12;
L12:
	optin_1.parin[4] = ws;
/* +.365*EINC !WS=13.5 */
	if (einc > 40.) {
	    optin_1.parin[3] = 0.f;
/* 7.5-.02*(einc-40) */
	    optin_1.parin[4] = ws;
/* 1.666!16.266-0.1*(einc-40) */
	}
	if (einc > 120.) {
	    optin_1.parin[3] = 0.;
/* 5.9 */
	    optin_1.parin[4] = ws;
/* 1.666!8.226-0.07*(einc-120) */
	}
	a1 = optin_1.parin[0];
	a2 = optin_1.parin[1];
	a5 = optin_1.parin[4];
	a4 = optin_1.parin[3];
/* 	print *,einc,a1,a2,a5,a4 */
	ecm = einc * optin_1.amt / (optin_1.amp + optin_1.amt);
	optin_1.gamma = zp * .157454 * zt * sqrt(optin_1.amp / einc);
	optin_1.ksq = optin_1.cmu * ecm;
	optin_1.kwn = sqrt(optin_1.ksq);
	optmod_(&c_true, &error);
/* 	if(einc.eq.78)then */
/*       write(8,*)'ef=78MeV' */
/* 	endif */
/* 	write(8,*)einc */
	if (i__ == 1) {
	    goto L12222;
	}
	if (optout_1.si[0] * sdfc < 0.) {
	    s_wsle(&io___29);
	    do_lio(&c__9, &c__1, "resonance s", (ftnlen)11);
	    e_wsle();
	    s_wsle(&io___30);
	    do_lio(&c__5, &c__1, (char *)&einc, (ftnlen)sizeof(doublereal));
	    e_wsle();
	}
	if (optout_1.si[1] * sdfa < 0.) {
	    s_wsle(&io___32);
	    do_lio(&c__9, &c__1, "resonance p1/2", (ftnlen)14);
	    e_wsle();
	    s_wsle(&io___33);
	    do_lio(&c__5, &c__1, (char *)&einc, (ftnlen)sizeof(doublereal));
	    e_wsle();
	}
	if (optout_1.si[251] * sdfb < 0.) {
	    s_wsle(&io___35);
	    do_lio(&c__9, &c__1, "resonance p3/2", (ftnlen)14);
	    e_wsle();
	    s_wsle(&io___36);
	    do_lio(&c__5, &c__1, (char *)&einc, (ftnlen)sizeof(doublereal));
	    e_wsle();
	}
L12222:
	s_wsle(&io___37);
	do_lio(&c__5, &c__1, (char *)&einc, (ftnlen)sizeof(doublereal));
	e_wsle();
	i__3 = mxl_1.lmx;
	for (ij = mxl_1.lmn; ij <= i__3; ++ij) {
	    ab = (ij - 1 + .5f) / optin_1.kwn;
	    for (j = 1; j <= 2; ++j) {
		s_wsle(&io___40);
		do_lio(&c__5, &c__1, (char *)&optout_1.sr[ij + j * 250 - 251],
			 (ftnlen)sizeof(doublereal));
		do_lio(&c__5, &c__1, (char *)&optout_1.si[ij + j * 250 - 251],
			 (ftnlen)sizeof(doublereal));
		e_wsle();
/* L889: */
	    }
/* L888: */
	}
	sdfc = optout_1.si[0];
	sdfa = optout_1.si[1];
	sdfb = optout_1.si[251];
/* L111: */
    }
/* L400: */
    s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */

/* Subroutine */ int coulom_(logical *error, doublereal *eta, doublereal *kwn,
	 doublereal *rmax, doublereal *rd, integer *maxl)
{
    /* Initialized data */

    static integer minl = 0;
    static doublereal accur = 1e-15;
    static doublereal step = 100.;

    /* Format strings */
    static char fmt_777[] = "(/1x,\002PROBLEM IN COULOMB WAVEFUNCTION CALCUL"
	    "ATION\002,/)";

    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3, d__4, d__5, d__6, d__7, d__8, d__9, d__10, 
	    d__11;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal), log(doublereal), sin(
	    doublereal), cos(doublereal);
    integer s_wsfe(cilist *), e_wsfe(void);

    /* Local variables */
    static doublereal d__, f, g, h__;
    static integer i__, j;
    static doublereal k;
    static integer l, m;
    static doublereal p, q, r__, t, w, y, h2;
    static integer i2;
    static doublereal k1, k2, k3, k4, m1, m2, m3, m4, r3, ai, bi, di, dk, ar, 
	    br, dp, fp, dr, dq, tf, gp, pl;
    static integer lp;
    static doublereal ft, rh, wi, ts, rh2, acc, del, elp, rho, tfp;
    static integer ktr;
    static doublereal pmx, eta2, h2ll, rho2, xll1, pace, beta, etah, etar;
    static integer lmax, ktrp;
    static doublereal turn;
    static integer lmin1;
    static doublereal alpha, etasq, rhomx;

    /* Fortran I/O blocks */
    static cilist io___111 = { 0, 6, 0, fmt_777, 0 };


/* *** COULOMB WAVEFUNCTIONS CALCULATED AT R=RHO BY THE */
/* *** CONTINUED-FRACTION METHOD OF STEED */
/* *** SEE BARNETT FENG STEED AND GOLDFARB COMPUTER PHYSICS COMMUN 1974 */

/* **********************DOUBLE PRECISION INSERTIONS*********************** */


/* ************************************************************************ */
/* correction 18 july 89,ISPIN=2 ON 4 SEP 89 */
    spin_1.ispin = 2;
    spin_1.lstype = 1;

/*     PHASE SHIFTS FROM HIGH L SERIES */

    etasq = *eta * *eta;
    if (*maxl - 50 >= 0) {
	goto L35;
    } else {
	goto L20;
    }
L20:
    elp = 50.;
    j = 50;
    goto L45;
L35:
    elp = (doublereal) (*maxl);
    j = *maxl;
L45:
    d__1 = *eta / elp;
    alpha = atan(d__1);
/* Computing 2nd power */
    d__2 = elp;
    d__1 = etasq + d__2 * d__2;
    beta = sqrt(d__1);
    d__1 = alpha * 3.;
    d__2 = alpha * 5.;
    d__3 = alpha * 7.;
    d__4 = alpha * 9.;
/* Computing 3rd power */
    d__5 = beta;
/* Computing 5th power */
    d__6 = beta, d__7 = d__6, d__6 *= d__6;
/* Computing 7th power */
    d__8 = beta, d__9 = d__8, d__8 *= d__8, d__9 *= d__8;
/* Computing 9th power */
    d__10 = beta, d__11 = d__10, d__10 *= d__10, d__10 *= d__10;
    y = alpha * (elp - .5) + *eta * (log(beta) - 1.) - sin(alpha) / (beta * 
	    12.) + sin(d__1) / (d__5 * (d__5 * d__5) * 360.) - sin(d__2) / (
	    d__7 * (d__6 * d__6) * 1260.) + sin(d__3) / (d__9 * (d__8 * d__8) 
	    * 1680.) - sin(d__4) / (d__11 * (d__10 * d__10) * 1188.);
    m = j - 1;
    if (j - *maxl <= 0) {
	goto L65;
    } else {
	goto L70;
    }
L65:
    couout_1.sigma[j - 1] = y;
    ts = couout_1.sigma[j - 1] * 2.;
    couout_1.sinsgc[j - 1] = sin(ts);
    couout_1.cossgc[j - 1] = cos(ts);
L70:
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	elp += -1.;
	--j;
	d__1 = *eta / elp;
	y -= atan(d__1);
	if (j - *maxl <= 0) {
	    goto L95;
	} else {
	    goto L100;
	}
L95:
	couout_1.sigma[j - 1] = y;
	ts = couout_1.sigma[j - 1] * 2.;
	couout_1.sinsgc[j - 1] = sin(ts);
	couout_1.cossgc[j - 1] = cos(ts);
L100:
	;
    }
    rhomx = *kwn * *rmax;
/* L9999: */
    rho = rhomx;
    *error = FALSE_;
    pace = step;
    acc = accur;
    if (pace < 100.f) {
	pace = 100.;
    }
    if (acc < 1e-15 || acc > 1e-6) {
	acc = 1e-6;
    }
    r__ = rho;
/*      print *,"rho",RHO */
    ktr = 1;
    lmax = *maxl;
    lmin1 = minl + 1;
    i__1 = minl * lmin1;
    xll1 = (doublereal) i__1;
    eta2 = *eta * *eta;
    d__1 = eta2 + xll1;
    turn = *eta + sqrt(d__1);
    if (r__ < turn && abs(*eta) >= 1e-6) {
	ktr = -1;
    }
    ktrp = ktr;
    goto L2;
L1:
    r__ = turn;
    tf = f;
    tfp = fp;
    lmax = minl;
    ktrp = 1;
L2:
    etar = *eta * r__;
    rho2 = r__ * r__;
    i__1 = lmax + 1;
    pl = (doublereal) i__1;
    pmx = pl + .5;

/* *** CONTINUED FRACTION FOR FP(MAXL)/F(MAXL) XL IS F XLPRIME IS FP ** */

    fp = *eta / pl + pl / r__;
    dk = etar * 2.;
    del = 0.;
    d__ = 0.;
    f = 1.;
    k = (pl * pl - pl + etar) * (pl * 2. - 1.);
    if (pl * pl + pl + etar != 0.f) {
	goto L3;
    }
    r__ += 1e-6;
    goto L2;
L3:
    h__ = (pl * pl + eta2) * (1. - pl * pl) * rho2;
    k = k + dk + pl * pl * 6.;
    d__ = 1. / (d__ * h__ + k);
    del *= d__ * k - 1.;
    if (pl < pmx) {
	del = -r__ * (pl * pl + eta2) * (pl + 1.) * d__ / pl;
    }
    pl += 1.;
    fp += del;
    if (d__ < 0.f) {
	f = -f;
    }
    if (pl > 2e4) {
	goto L11;
    }
    d__1 = del / fp;
    if (abs(d__1) >= acc) {
	goto L3;
    }
    fp = f * fp;
    if (lmax == minl) {
	goto L5;
    }
    couout_1.fc[lmax] = f;
    couout_1.fcp[lmax] = fp;

/* *** DOWNWARD RECURSION TO MINL FOR F AND FP. ARRAYS GC,GCP ARE STORAGE */

    l = lmax;
    i__1 = lmax;
    for (lp = lmin1; lp <= i__1; ++lp) {
	pl = (doublereal) l;
	couout_1.gc[l] = *eta / pl + pl / r__;
	d__1 = eta2 + pl * pl;
	couout_1.gcp[l] = sqrt(d__1) / pl;
	couout_1.fc[l - 1] = (couout_1.gc[l] * couout_1.fc[l] + couout_1.fcp[
		l]) / couout_1.gcp[l];
	couout_1.fcp[l - 1] = couout_1.gc[l] * couout_1.fc[l - 1] - 
		couout_1.gcp[l] * couout_1.fc[l];
/* L4: */
	--l;
    }
    f = couout_1.fc[lmin1 - 1];
    fp = couout_1.fcp[lmin1 - 1];
L5:
    if (ktrp == -1) {
	goto L1;
    }

/* *** REPEAT FOR R=TURN IF RHO LT TURN */

/* ****NOW OBTAIN P+I.Q FOR MINL FROM CONTINUED FRACTION (32) */
/* *** REAL ARITHMETIC TO FACILITATE CONVERSION TO REAL*8 */
    p = 0.;
    q = r__ - *eta;
    pl = 0.;
    ar = -(eta2 + xll1);
    ai = *eta;
    br = q * 2.;
    bi = 2.;
    wi = *eta * 2.;
    dr = br / (br * br + bi * bi);
    di = -bi / (br * br + bi * bi);
    dp = -(ar * di + ai * dr);
    dq = ar * dr - ai * di;
L6:
    p += dp;
    q += dq;
    pl += 2.;
    ar += pl;
    ai += wi;
    bi += 2.;
    d__ = ar * dr - ai * di + br;
    di = ai * dr + ar * di + bi;
    t = 1. / (d__ * d__ + di * di);
    dr = t * d__;
    di = -t * di;
    h__ = br * dr - bi * di - 1.;
    k = bi * dr + br * di;
    t = dp * h__ - dq * k;
    dq = dp * k + dq * h__;
    dp = t;
    if (pl > 4.6e4) {
	goto L11;
    }
    if (abs(dp) + abs(dq) >= (abs(p) + abs(q)) * acc) {
	goto L6;
    }
    p /= r__;
    q /= r__;

/* *** SOLVE FOR FP,G,GP AND NORMALISE F AT L=MINL */

    g = (fp - p * f) / q;
    gp = p * g - q * f;
    d__1 = fp * g - f * gp;
    w = 1. / sqrt(d__1);
    g = w * g;
    gp = w * gp;
    if (ktr == 1) {
	goto L8;
    }
    f = tf;
    ft = tfp;
    lmax = *maxl;

/* *** RUNGE-KUTTA INTERATION OF G(MINL) AND GP(MINL) INWARDS FROM TURN */
/* ***            SEE FOX AND MAYERS 1968 PG 202 */

    if (rho < turn * .2f) {
	pace = 999.;
    }
    r3 = .33333333333333331;
    h__ = (rho - turn) / (pace + 1.);
    h2 = h__ * .5;
    d__1 = pace + .001;
    i2 = (integer) d__1;
    etah = *eta * h__;
    h2ll = h2 * xll1;
    spin_1.s = (etah + h2ll / r__) / r__ - h2;
L7:
    rh2 = r__ + h2;
    t = (etah + h2ll / rh2) / rh2 - h2;
    k1 = h2 * gp;
    m1 = spin_1.s * g;
    k2 = h2 * (gp + m1);
    m2 = t * (g + k1);
    k3 = h__ * (gp + m2);
    m3 = t * (g + k2);
    m3 += m3;
    k4 = h2 * (gp + m3);
    rh = r__ + h__;
    spin_1.s = (etah + h2ll / rh) / rh - h2;
    m4 = spin_1.s * (g + k3);
    g += (k1 + k2 + k2 + k3 + k4) * r3;
    gp += (m1 + m2 + m2 + m3 + m4) * r3;
    r__ = rh;
    --i2;
    if (abs(gp) > 1e70) {
	goto L11;
    }
    if (i2 >= 0) {
	goto L7;
    }
    w = 1. / (fp * g - f * gp);

/* *** UPWARD RECURSION FROM GC(MINL) AND GCP(MINL),STORED VALUES ARE R,S */
/* *** RENORMALISE FC,FCP FOR EACH L-VALUE */

L8:
    couout_1.gc[lmin1 - 1] = g;
    couout_1.gcp[lmin1 - 1] = gp;
    if (lmax == minl) {
	goto L10;
    }
    i__1 = lmax;
    for (l = lmin1; l <= i__1; ++l) {
	t = couout_1.gc[l];
	couout_1.gc[l] = (couout_1.gc[l - 1] * couout_1.gc[l] - couout_1.gcp[
		l - 1]) / couout_1.gcp[l];
	couout_1.gcp[l] = couout_1.gc[l - 1] * couout_1.gcp[l] - couout_1.gc[
		l] * t;
	couout_1.fc[l] = w * couout_1.fc[l];
/* L9: */
	couout_1.fcp[l] = w * couout_1.fcp[l];
    }
    couout_1.fc[lmin1 - 1] *= w;
    couout_1.fcp[lmin1 - 1] *= w;
    i__1 = lmax;
    for (l = lmin1; l <= i__1; ++l) {
	couout_1.gcp[l - 1] = *kwn * couout_1.gcp[l - 1];
	couout_1.fcp[l - 1] = *kwn * couout_1.fcp[l - 1];
/* L12: */
    }
    couout_1.gcp[lmax] = *kwn * couout_1.gcp[lmax];
    couout_1.fcp[lmax] = *kwn * couout_1.fcp[lmax];
    return 0;
L10:
    couout_1.fc[lmin1 - 1] = w * f;
    couout_1.fcp[lmin1 - 1] = *kwn * (w * fp);
    couout_1.gcp[lmin1 - 1] = *kwn * couout_1.gcp[lmin1 - 1];
    return 0;
L11:
    w = 0.;
    g = 0.;
    gp = 0.;
    *error = TRUE_;
    s_wsfe(&io___111);
    e_wsfe();
    goto L8;
} /* coulom_ */

/* Subroutine */ int optmod_(logical *newint, logical *error)
{
    /* Initialized data */

    static integer constr[13] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
    static integer mint = 1000;

    /* Format strings */
    static char fmt_1004[] = "(\0020CALCULATION TERMINATED--VOLUME INTEGRAL "
	    "OF IMAGINARY POTENTIAL POSITIVE\002)";
    static char fmt_1003[] = "(\00217H0WARNING--RMAXC(=\002,f6.2,\00210H ).G"
	    "T.RMAX\002)";
    static char fmt_1001[] = "(\00247H0CALCULATION TERMINATED--NUMBER OF INT"
	    "ERVALS =\002,i3)";
    static char fmt_5250[] = "(\002 PARTIAL  WAVE SERIES TRUNCATED - INCREAS"
	    "E RMAX\002)";
    static char fmt_1002[] = "(\002 WARNING--SCATTERING MATRIX DOES NOT CONV"
	    "ERGE  \002)";

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2, d__3, d__4, d__5;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void);
    double log(doublereal);
    integer do_fio(integer *, char *, ftnlen);
    double exp(doublereal);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j, m, n;
    static doublereal a3;
    static integer m1, n1, m2, n3;
    static doublereal u2, aa, bb;
#define ai ((doublereal *)&optout_1 + 6)
    static doublereal bi, am, dk;
    static integer n20, ii;
    static doublereal fl;
#define as ((doublereal *)&optout_1 + 2)
    static doublereal r12, rc, br, wd, ri, ui[7];
    static integer ms;
    static doublereal xi, rs, ur[7], us;
    static integer mz;
    static doublereal ws, rx, xr, sz, bbi, bci, cib, cic, cia, ecm, bbr, bcr, 
	    crb, crc, adv, cra, uib, uic, sdi;
#define asp ((doublereal *)&optout_1 + 10)
    static doublereal cor, bso, rdv;
#define rcx ((doublereal *)&optout_1 + 11)
    static doublereal udv;
#define wdx ((doublereal *)&optout_1 + 4)
#define rix ((doublereal *)&optout_1 + 5)
    static doublereal rsp, rho, usp;
    static integer n3m6;
    static doublereal wsp, xpo, urb;
#define rsx ((doublereal *)&optout_1 + 1)
    static doublereal urc, urp;
#define usx ((doublereal *)&optout_1)
    static doublereal uip;
#define wsx ((doublereal *)&optout_1 + 3)
    static doublereal snr, sni, sdr, snn, eta1, bcci, bbcr, cicp;
    static integer mici;
    static doublereal deri, bbso, crcp;
    static integer micr, mics;
    static doublereal derr, rimi;
    static integer lmax;
    static doublereal temp, amsq;
#define norm ((doublereal *)&optout_1 + 12)
    static doublereal uexp[1000], wexp[1000];
#define rspx ((doublereal *)&optout_1 + 9)
#define uspx ((doublereal *)&optout_1 + 7)
#define wspx ((doublereal *)&optout_1 + 8)
    static integer mics1;
    static doublereal temp1, rmx0i, uexp1[1000], wexp1[1000], rmx0r, aisav, 
	    eigls, rmaxc, assav, rcsav, rlmax, risav, wdsav, rssav, ussav, 
	    wssav, rmwvs, vccona, vcconb, vcconc, advsav, elterm, aspsav;
    extern /* Subroutine */ int coulom_(logical *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *);
    static logical spnorb;
    static doublereal rdvsav, udvsav, rspsav, uspsav, wspsav, sxtydr;

    /* Fortran I/O blocks */
    static cilist io___137 = { 0, 6, 0, fmt_1004, 0 };
    static cilist io___156 = { 0, 6, 0, fmt_1003, 0 };
    static cilist io___159 = { 0, 6, 0, fmt_1001, 0 };
    static cilist io___173 = { 0, 14, 0, 0, 0 };
    static cilist io___187 = { 0, 13, 0, 0, 0 };
    static cilist io___237 = { 0, 15, 0, 0, 0 };
    static cilist io___248 = { 0, 6, 0, fmt_5250, 0 };
    static cilist io___249 = { 0, 6, 0, fmt_1002, 0 };



/*  OPTICAL MODEL CALCULATION */
/*         INPUT PARAMETERS ARE NEWINT,ERROR,/OPTIN/ */
/*         NEWINT .TRUE. FOR FIRST PASS ON SEARCH OR FOR NOSEARCH */
/*         ERROR IS RETURNED .TRUE. IN CASE OF TROUBLE */

/* $======================================= */
/* $======================================= */

/* ******************* HI-OPTIM ************ NOTES ********************* */

/*     TOGETHER WITH THE CHANGES LISTED IN SUBROUTINE 'MAIN' THE FOLLOWING */
/*     CARDS CONVERT THE PROGRAM TO DOUBLE PRECISION FOR IBM 360/195 USE. */

/* *************************************************************************** */


/*     STATEMENT FUNCTION RMAX */

    spin_1.ispin = 2;
    spin_1.lstype = 1;
/* correction 18 july 89,ISPIN=2 ON 4 SEP 89 */
    *error = FALSE_;
    eta1 = 1e-8;
    r12 = .083333333333333329;
    cor = .033333333333333333;

/*     CONSTRAINTS */

/* 	print *,par1 */
    n = 1;
/* L123: */
    for (i__ = n; i__ <= 13; ++i__) {
	if (mici != 0 && i__ > 4 && i__ <= 7) {
	    goto L10;
	}
	if (micr != 0 && i__ > 1 && i__ <= 3) {
	    goto L10;
	}
	if (mics != 0 && i__ > 9 && i__ <= 11) {
	    goto L10;
	}
	ii = constr[i__ - 1];
	optout_1.param[i__ - 1] = optin_1.parin[ii - 1];
	if (optout_1.param[i__ - 1] >= optin_1.parmin[i__ - 1] && 
		optout_1.param[i__ - 1] <= optin_1.parmax[i__ - 1]) {
	    goto L10;
	}
	*error = TRUE_;
/*        WRITE (6,1000) PARAM(I),PARMIN(I),PARMAX(I) */
/* 1000 FORMAT (35H0CALCULATION TERMINATED--PARAMETER A4,3H (=1PE9.2,18H) */
/*     $IS OUT OF LIMITS 3X,4H.GE.,E9.2,3X,4H.LE. E9.2 ) */
L10:
	;
    }
    if (mici == 1) {
	goto L12;
    }
    rimi = abs(*rix) * optin_1.athird;
    if (*rix < 0.) {
	rimi += abs(*rix) * optin_1.ap3;
    }
    if (*wsx * rimi / 3. + *wdx * 2. * *ai >= 0.) {
	goto L12;
    }
    s_wsfe(&io___137);
    e_wsfe();
    *error = TRUE_;
L12:
    if (*error) {
	return 0;
    }

/*     SPNORB */

    if (! (*newint)) {
	goto L15;
    }
    spnorb = FALSE_;
    mics1 = mics + 1;
    switch (mics1) {
	case 1:  goto L13;
	case 2:  goto L14;
    }
L13:
    if (abs(*rspx) <= .001 || *asp <= .001) {
	goto L16;
    }
    if (spin_1.ispin > 1) {
	spnorb = TRUE_;
    }
    optin_1.parmin[10] = .001;
    goto L15;
L14:
    if (*uspx == 0. && *wspx == 0.) {
	goto L16;
    }
    if (spin_1.ispin > 1) {
	spnorb = TRUE_;
    }
L15:
    if (spnorb) {
	goto L18;
    }
/* $=========== */
L16:
    if (pots_1.wstwo >= 5) {
	goto L18;
    }
/* $=========== */
    *uspx = 0.;
    *wspx = 0.;
    *rspx = 0.;
    *asp = 0.;

/*     INTERNAL  PARAMETERS */

L18:
    us = *usx * optin_1.cmu;
    if (micr == 1) {
	goto L518;
    }
    rs = abs(*rsx) * optin_1.athird;
    if (*rsx < 0.) {
	rs = abs(*rsx) * optin_1.ap3 + rs;
    }
L518:
    ws = *wsx * optin_1.cmu;
    if (mici == 1) {
	goto L618;
    }
    wd = *wdx * optin_1.cmu;
    ri = abs(*rix) * optin_1.athird;
    if (*rix < 0.) {
	ri += abs(*rix) * optin_1.ap3;
    }
L618:
    rc = abs(*rcx) * optin_1.athird;
    if (*rcx < 0.) {
	rc += abs(*rcx) * optin_1.ap3;
    }
    if (mics == 1) {
	goto L718;
    }
/* $$$$$$$$$$$$$$$$$$$$$$ */
    if (pots_1.wstwo >= 5) {
	rdv = abs(*rspx) * optin_1.athird;
	if (*rspx < 0.) {
	    rdv += abs(*rspx) * optin_1.ap3;
	}
	udv = *uspx * optin_1.cmu;
	adv = *asp;
	usp = 0.;
	wsp = 0.;
	rsp = 0.;
	goto L719;
    }
/* $$$$$$$$$$$$$$$$$$$$$$ */
    rsp = abs(*rspx) * optin_1.athird;
    if (*rspx < 0.) {
	rsp += abs(*rspx) * optin_1.ap3;
    }
    usp = *uspx * optin_1.cmu * 4.;
/*      print *,"usp",usp,uspx,cmu */
    wsp = *wspx * optin_1.cmu * 4.;
/*      print *,"wsp",wsp,wspx,cmu */
    goto L719;
L718:
    usp = *uspx * optin_1.cmu;
    wsp = *wspx * optin_1.cmu;
/*      print *,"718" */
L719:
    ecm = optin_1.ksq / optin_1.cmu;

/*     RMAX,DR,MWV,VCCON */

    if (micr == 1 || mici == 1 || mics == 1) {
	goto L1919;
    }
    if (! (*newint)) {
	goto L1818;
    }
    rmx0r = rs + *as * log(max(*usx,.001) / (.001 * ecm) * max(*as,.001) * (
	    optin_1.kwn + (optin_1.gamma + optin_1.kwn * rs) / c_b115));

/* ***************** HI-OPTIM ********** NOTES ************************ */

/*      AMAX1 EXPLICITLY EDITED DMAX1 IN THIS ROUTINE TO */
/*      AVOID ID CONFLICT DUE TO VARIABLE NO''S OF ARGUMENTS. */

/* ***************************************************************** */

/* Computing MAX */
    d__2 = *wsx, d__3 = *wdx * 4., d__2 = max(d__2,d__3), d__3 = *wsx + *wdx *
	     4.;
    d__1 = max(d__2,d__3);
    rmx0i = ri + *ai * log(max(d__1,.001) / (.001 * ecm) * max(*ai,.001) * (
	    optin_1.kwn + (optin_1.gamma + optin_1.kwn * ri) / c_b115));
/* Computing MAX */
    d__2 = *wsx, d__3 = *wdx * 4., d__2 = max(d__2,d__3), d__3 = *wsx + *wdx *
	     4.f;
    d__1 = max(d__2,d__3);
/* Computing MAX */
    d__4 = rs + *as * log(max(*usx,.001) / (.001 * ecm) * max(*as,.001) * (
	    optin_1.kwn + (optin_1.gamma + optin_1.kwn * rs) / rmx0r)), d__5 =
	     ri + *ai * log(max(d__1,.001) / (.001 * ecm) * max(*ai,.001) * (
	    optin_1.kwn + (optin_1.gamma + optin_1.kwn * ri) / rmx0i));
    rmaxc = max(d__4,d__5);
    if (! (*newint)) {
	goto L1818;
    }
    optout_1.rmax = optin_1.rmaxin;
/*      print *,"rmax",rmax */
    if (optin_1.rmaxin == 0.f) {
	optout_1.rmax = rmaxc * 1.3;
    }
L1818:
    if (optout_1.rmax > rmaxc) {
	goto L1919;
    }
    s_wsfe(&io___156);
    do_fio(&c__1, (char *)&rmaxc, (ftnlen)sizeof(doublereal));
    e_wsfe();
L1919:
    if (! (*newint)) {
	goto L40;
    }
    if (micr == 0 && mici == 0 && mics == 0) {
	goto L124;
    }
    optout_1.rmax = optin_1.rmaxin;
L124:
    optout_1.dr = optin_1.drin;
    if (optout_1.dr == 0.f) {
	optout_1.dr = .3 / optin_1.kwn;
    }
    sxtydr = optout_1.dr * 60.;
    pots_1.drsq = optout_1.dr * optout_1.dr;
    dk = pots_1.drsq * optin_1.ksq;
/*      print *,"rmax",rmax */
    d__1 = optout_1.rmax / optout_1.dr + .5;
/* Computing MAX */
    i__1 = 6, i__2 = (integer) d__1;
    optout_1.nint = max(i__1,i__2);
/*      print *,"nint",nint */
    if (optout_1.nint <= mint - 3) {
	goto L19;
    }
    s_wsfe(&io___159);
    do_fio(&c__1, (char *)&optout_1.nint, (ftnlen)sizeof(integer));
    e_wsfe();
    *error = TRUE_;
    return 0;
L19:
    n1 = optout_1.nint + 1;
    n3 = optout_1.nint + 3;
    a3 = (doublereal) n3;
    i__1 = n3;
    for (i__ = n1; i__ <= i__1; ++i__) {
	pots_1.hidrsq[i__ - 1] = 0.;
	pots_1.hrdrsq[i__ - 1] = 0.;
	pots_1.udrsq[i__ - 1] = 0.;
/* L20: */
	pots_1.wdrsq[i__ - 1] = 0.;
    }
    vccona = optin_1.gamma * optin_1.kwn * pots_1.drsq;
    vcconb = vccona * 2.;
    goto L50;
/*      print *,nint */

/*     COULOMB POTENTIAL */

L40:
    if (rc == rcsav) {
	goto L60;
    }
L50:
    rcsav = rc;
    vcconc = vccona / rc;
    rx = 0.;
    i__1 = n3;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rx += optout_1.dr;
	if (rx > rc) {
	    goto L52;
	}
	pots_1.vcdrsq[i__ - 1] = vcconc * (3. - rx * rx / (rc * rc));
	goto L55;
L52:
	pots_1.vcdrsq[i__ - 1] = vcconb / rx;
L55:
	;
    }
    optout_1.rmax = (doublereal) optout_1.nint * optout_1.dr;
/*      print *,"rmax",rmax */

/*     PHENOMENOLOGICAL SPIN-ORBIT POTENTIAL */

/*      print *,"begin phenom" */
L60:
    if (spnorb) {
	goto L62;
    }
    if (! (*newint)) {
	goto L100;
    }
/*      print *,"60" */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*      print *,"i",i,"nint",nint */
	pots_1.hrdrsq[i__ - 1] = 0.;
/* L61: */
	pots_1.hidrsq[i__ - 1] = 0.;
    }
/*      PRINT "61" */
    goto L100;
L62:
    if (mics == 0) {
	goto L63;
    }
    if (mics == 1) {
	goto L83;
    }
L63:
    if (! (*newint)) {
	goto L65;
    }
/*      print *,"63" */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	pots_1.hrdrsq[i__ - 1] = 0.;
/* L64: */
	pots_1.hidrsq[i__ - 1] = 0.;
    }
/*      print *,"64" */
    if (*newint) {
	goto L66;
    }
L65:
    if (rsp == rspsav && *asp == aspsav) {
	goto L75;
    }
L66:
    if (rsp == 0. || *asp == 0.) {
	goto L100;
    }
/*      PRINT *,"phenomil" */
    rx = 0.;
    d__1 = optout_1.dr / *asp;
    bso = exp(d__1);
    d__1 = -rsp / *asp;
    bbso = bso * exp(d__1);
/* ============ANTES ERA================= */
/*     IF(WSTWO.GT.2) GO TO 69 */
/* ====================================== */
    if (pots_1.wstwo == 3) {
	goto L69;
    }
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rx += optout_1.dr;
/*      print *,"rx",rx,"nint",nint */
	if (bbso > 1e20) {
	    bbso = 0.;
	}
	temp = bbso / (rx * *asp * (bbso + 1.) * (bbso + 1.));
	micso_1.h__[i__ - 1] = temp * pots_1.drsq;
	s_wsle(&io___173);
	do_lio(&c__5, &c__1, (char *)&micso_1.h__[i__ - 1], (ftnlen)sizeof(
		doublereal));
	e_wsle();
/* L68: */
	bbso *= bso;
    }
    goto L76;
L69:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rx += optout_1.dr;
/*      print *,"69" */
	if (bbso > 1e20) {
	    bbso = 0.;
	}
	temp = bbso + 1.;
	micso_1.h__[i__ - 1] = pots_1.drsq * 2. * bbso / (rx * *asp * temp * 
		temp * temp);
/* L70: */
	bbso *= bso;
    }
    goto L76;
L75:
    if (usp == uspsav) {
	goto L80;
    }
L76:
    uspsav = usp;
/*      print *,"76" */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L77: */
	pots_1.hrdrsq[i__ - 1] = micso_1.h__[i__ - 1] * usp;
    }
/*      print *,"77" */
L80:
    if (*newint) {
	goto L81;
    }
    if (wsp == wspsav && rsp == rspsav && *asp == aspsav) {
	goto L100;
    }
L81:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L82: */
	pots_1.hidrsq[i__ - 1] = micso_1.h__[i__ - 1] * wsp;
    }
/*   82   write(14,*)HIDRSQ(I) */
    wspsav = wsp;
    rspsav = rsp;
    aspsav = *asp;
    goto L100;

/*     MICROSCOPIC SPIN-ORBIT POTENTIAL */

L83:
/* L85: */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*      PRINT *,"micro" */
	temp = pots_1.drsq * micso_1.h__[i__ - 1];
	pots_1.hrdrsq[i__ - 1] = usp * temp;
/*      print *,"85" */
/* L86: */
	pots_1.hidrsq[i__ - 1] = wsp * temp;
    }
/*      PRINT *,"micro2" */
    uspsav = usp;
    wspsav = wsp;

/*     LMAX SET AND COULOMB WAVE FUNCTIONS CALCULATED */

L100:
    rho = optin_1.kwn * optout_1.rmax;
/*      PRINT *,"100" */
    aa = rho * 4. * (rho - optin_1.gamma * 2.);
    bb = aa + 1.;
    rlmax = sqrt(bb) * .5 - .5;
    lmax = (integer) rlmax + 15;
    if (mxl_1.lset == 1) {
	lmax = mxl_1.lmx;
    }
    if (lmax > 249) {
	lmax = 249;
    }
    if (! (*newint)) {
	goto L150;
    }
    coulom_(error, &optin_1.gamma, &optin_1.kwn, &optout_1.rmax, &optout_1.dr,
	     &lmax);
    if (*error) {
	return 0;
    }

/*     REAL NUCLEAR POTENTIALS */

L150:
    if (micr == 0) {
	goto L650;
    }
    if (! (*newint)) {
	goto L156;
    }
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	uexp[i__ - 1] = pots_1.drsq * pots_1.udrsq[i__ - 1];
/* L640: */
    }
    ussav = us;
    goto L156;
L650:
    if (*newint) {
	goto L151;
    }
    if (rs == rssav && *as == assav) {
	goto L155;
    }
L151:
    if (rs == 0. || *as == 0.) {
	goto L160;
    }
    d__1 = optout_1.dr / *as;
    br = exp(d__1);
    d__1 = -rs / *as;
    bbr = br * exp(d__1);
    if (pots_1.wstwo == 0) {
	goto L251;
    }
/* $=================NO ESTABA========= */
    if (pots_1.wstwo == 4) {
	goto L251;
    }
    if (pots_1.wstwo >= 5) {
	goto L251;
    }
/* $=================================== */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	uexp[i__ - 1] = 0.f;
/*     * +par1*bbr*bbr/(1.0D0+BBR)**4/as/as */
/* 	print *, 'par1',par1 */
/* 	print *,'aa' */
/* DRSQ/((1.0D0+BBR)*(1.0D0+BBR)) */
/* L252: */
	bbr *= br;
    }
    goto L253;
L251:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 4th power */
	d__1 = bbr + 1., d__1 *= d__1;
	uexp[i__ - 1] = pots_1.drsq * (us / (bbr + 1.) - optin_1.cmu * 16.f * 
		optin_1.alpar * bbr * bbr / (d__1 * d__1));
	s_wsle(&io___187);
	do_lio(&c__5, &c__1, (char *)&uexp[i__ - 1], (ftnlen)sizeof(
		doublereal));
	e_wsle();
/* 	print *,us,bbr,alpar */
/* L152: */
	bbr *= br;
    }
L253:
    rssav = rs;
    assav = *as;
    ussav = us;
    goto L156;
L155:
    if (us == ussav && pots_1.wstwo <= 4) {
	goto L500;
    }
/* $========ANTES ERA===== */
/* $155 IF (US.EQ.USSAV) GO TO 160 */
/* $============= */
L156:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L157: */
	pots_1.udrsq[i__ - 1] = uexp[i__ - 1];
    }
    ussav = us;
/* $$$$$$$$$$$$$ */
L160:
    if (*newint) {
	goto L400;
    }
    if (rdv == rdvsav && adv == advsav) {
	goto L401;
    }
L400:
    if (rdv == 0. && adv == 0.) {
	goto L500;
    }
    d__1 = optout_1.dr / adv;
    bcr = exp(d__1);
    d__1 = -rdv / adv;
    bbcr = bcr * exp(d__1);
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	uexp1[i__ - 1] = bbcr;
/* L402: */
	bbcr *= bcr;
    }
    rdvsav = rdv;
    advsav = adv;
L401:
    if (pots_1.wstwo == 7) {
	i__1 = optout_1.nint;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L405: */
	    pots_1.udrsq[i__ - 1] += pots_1.drsq * 40. * udv / uexp1[i__ - 1];
	}
	udvsav = udv;
/*      print *,"405" */
	goto L500;
    }
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = uexp1[i__ - 1] + 1.;
/* L403: */
	pots_1.udrsq[i__ - 1] += pots_1.drsq * 40. * udv * uexp1[i__ - 1] / (
		temp * temp);
    }
    udvsav = udv;
/*      print "403" */
/* $$$$$$$$$$$$$$$$$ */

/*     IMAGINARY NUCLEAR POTENTIALS */

L500:
    if (mici == 0) {
	goto L660;
    }
/* $$$$$$$$ANTES ERA$$$$$ */
/* $160 IF(MICI.EQ.0) GO TO 660 */
/* $$$$$$$$$$$$$$$$$$$$$$ */
    if (! (*newint)) {
	goto L3670;
    }
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wexp[i__ - 1] = pots_1.drsq * pots_1.wdrsq[i__ - 1];
/* L670: */
    }
    wssav = 1.;
L3670:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	pots_1.wdrsq[i__ - 1] = ws * wexp[i__ - 1];
/* L3671: */
    }
    wssav = ws;
    goto L200;
L660:
    if (*newint) {
	goto L161;
    }
    if (ri == risav && *ai == aisav) {
	goto L165;
    }
L161:
    if (ri == 0. || *ai == 0.) {
	goto L180;
    }
    d__1 = optout_1.dr / *ai;
    bi = exp(d__1);
    d__1 = -ri / *ai;
    bbi = bi * exp(d__1);
/* $====================================== */
    d__1 = optout_1.dr / .4f;
    bci = exp(d__1);
    d__1 = -(optin_1.ap3 + optin_1.athird) / .4f;
    bcci = bci * exp(d__1);
/* $====================================== */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* $====================================== */
	wexp1[i__ - 1] = bcci;
	bcci = bci * bcci;
/* $====================================== */
	wexp[i__ - 1] = bbi;
/* L162: */
	bbi *= bi;
    }
    risav = ri;
    aisav = *ai;
    goto L166;
L165:
    if (ws == wssav && wd == wdsav) {
	goto L180;
    }
L166:
    if (pots_1.wstwo >= 2 && pots_1.wstwo < 5) {
	goto L168;
    }
/* $$$$$$$$$$$$$$$ANTES ERA$$$$$$$$$$$$$$$$ */
/* 166 IF(WSTWO.GE.2) GO TO 168 */
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = wexp[i__ - 1] + 1.;
/* L167: */
	pots_1.wdrsq[i__ - 1] = pots_1.drsq * (ws / temp + wd * 4. * wexp[i__ 
		- 1] / (temp * temp));
    }
    goto L170;
L168:
/* $==================================== */
    if (pots_1.wstwo == 4 || pots_1.wstwo == 6) {
	goto L269;
    }
/* $==================================== */
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = wexp[i__ - 1] + 1.;
/* L169: */
	pots_1.wdrsq[i__ - 1] = pots_1.drsq * (ws / (temp * temp) + wd * 8. * 
		wexp[i__ - 1] / (temp * temp * temp));
    }
    goto L170;
/* $=================================================================== */
L269:
    i__1 = optout_1.nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = wexp[i__ - 1] + 1.;
	temp1 = wexp1[i__ - 1] + 1.;
/* L369: */
	pots_1.wdrsq[i__ - 1] = pots_1.drsq * (ws / (temp1 * temp1) + wd * 4. 
		* wexp[i__ - 1] / (temp * temp));
    }
/* $=================================================================== */
L170:
    wssav = ws;
    wdsav = wd;

/*     POTENTIAL CUT-OFFS */

L180:
    if (pots_1.ico < 1 || pots_1.ico > 3) {
	goto L200;
    }
    d__1 = pots_1.rone / optout_1.dr + .5;
    m1 = (integer) d__1;
/*      print *,"rone",rone,"rtwo",rtwo */
    d__1 = pots_1.rtwo / optout_1.dr + .5;
    m2 = (integer) d__1;
    switch (pots_1.ico) {
	case 1:  goto L5775;
	case 2:  goto L5777;
	case 3:  goto L5779;
    }
L5775:
    i__1 = m2;
    for (i__ = m1; i__ <= i__1; ++i__) {
/* L5776: */
	pots_1.udrsq[i__ - 1] = 0.;
    }
    goto L200;
L5777:
    i__1 = m2;
    for (i__ = m1; i__ <= i__1; ++i__) {
/* L5778: */
	pots_1.wdrsq[i__ - 1] = 0.;
    }
    goto L200;
L5779:
    i__1 = m2;
    for (i__ = m1; i__ <= i__1; ++i__) {
/* L5780: */
	pots_1.hrdrsq[i__ - 1] = 0.;
    }

/*     NUCLEAR INTEGRATIONS INCORPORATING STARTING POINT ADVANCE */

L200:
    n20 = optout_1.nint - 20;
    n3m6 = n3 - 6;
    if (mxl_1.lmn <= 0 || mxl_1.lmn > mxl_1.lmx) {
	mxl_1.lmn = 1;
    }
    i__1 = mxl_1.lmx;
    for (optout_1.mwvs = mxl_1.lmn; optout_1.mwvs <= i__1; ++optout_1.mwvs) {
	rmwvs = (doublereal) optout_1.mwvs;
	i__2 = optout_1.mwvs - 1;
	fl = (doublereal) i__2;
	elterm = fl * (fl + 1.);
	d__1 = (fl * fl + fl) / 12.;
	m1 = (integer) sqrt(d__1);
	xpo = 1. / (fl * 2. + 1.);
	m2 = (integer) (pow_dd(&c_b195, &xpo) * (fl * 2. / (optin_1.kwn * 
		2.718f * optout_1.dr)));
	m1 = max(m1,m2) + 1;
	mz = min(m1,n20);
	i__2 = spin_1.ispin;
	for (ms = 1; ms <= i__2; ++ms) {
	    j = 0;
	    spin_1.s = .5f;
	    i__3 = ms - spin_1.ispin;
	    sz = spin_1.s + (doublereal) i__3;
	    eigls = ((fl + sz) * (fl + sz + 1.) - elterm - spin_1.s * (
		    spin_1.s + 1.)) / 2.;
	    if (optout_1.mwvs == 1) {
		eigls = 0.;
	    }
/*      print *,"ispin",ispin,"lstype",lstype */
	    if (spin_1.ispin == 1 || spin_1.lstype == 1) {
		goto L210;
	    }
	    eigls /= spin_1.s;
L210:
	    uib = 0.;
	    urb = 0.;
	    uic = 0.;
	    urc = eta1;
	    crb = 0.;
	    if (optout_1.mwvs == 2) {
		crb = eta1 * 2.;
	    }
	    cib = 0.;
	    cic = 0.;
	    crc = 0.;
	    am = 0.;
/*      print *,"n3",n3 */
	    i__3 = n3;
	    for (m = 1; m <= i__3; ++m) {
/*      print *,m,"mz",mz,"n3m6",n3m6 */
		am += 1.;
		amsq = am * am;
		if (m == mz) {
		    goto L230;
		}
		if (m < mz) {
		    goto L248;
		}

/*     P1 PREDICTOR */

		urp = urc + urc - urb + crc;
		uip = uic + uic - uib + cic;

/*     STEP BACK ONE */

		cra = crb;
		cia = cib;
		crb = crc;
		cib = cic;
		urb = urc;
		uib = uic;
		urc = urp;
		uic = uip;

/*     CALCULATE SECOND DERIVATIVES */

L230:
		xr = -dk + pots_1.vcdrsq[m - 1] - pots_1.udrsq[m - 1] - eigls 
			* pots_1.hrdrsq[m - 1] + elterm / amsq;
/*      print *,"eigls",eigls */
		xi = -pots_1.wdrsq[m - 1] - eigls * pots_1.hidrsq[m - 1];
/*      print *,"imaginary comps",WDRSQ(M),EIGLS,HIDRSQ(M) */
/* L231: */
		crc = xr * urc - xi * uic;
		cic = xr * uic + xi * urc;
/* L240: */
		if (m == mz) {
		    goto L247;
		}

/*     C3 CORRECTOR */

		urc += r12 * (crc + cra - crb - crb);
		uic += r12 * (cic + cia - cib - cib);
		crcp = crc;
		cicp = cic;

/*     BAYLIS PEEL CORRECTOR */

		crc = xr * urc - xi * uic;
		cic = xr * uic + xi * urc;
		urc += cor * (crc - crcp);
/*      print *,urc */
		s_wsle(&io___237);
		do_lio(&c__5, &c__1, (char *)&urc, (ftnlen)sizeof(doublereal))
			;
		e_wsle();
		uic += cor * (cic - cicp);
/*      print *,M */
/*      URP=(UR(7)-UR(1)+9.0D0*(UR(2)-UR(6))+45.0D0*(UR(5)-UR(3)))/SXTYDR */
/*      print *,URP,"mwvs",mwvs */
/*      WRITE(13,*)urp */
L247:
		if (m < n3m6) {
		    goto L248;
		}
		++j;
		ur[j - 1] = urc;
		ui[j - 1] = uic;
/*      print *,"j",j,"urc",urc,"mz",mz */
L248:
		;
	    }
	    urp = (ur[6] - ur[0] + (ur[1] - ur[5]) * 9. + (ur[4] - ur[2]) * 
		    45.) / sxtydr;
	    uip = (ui[6] - ui[0] + (ui[1] - ui[5]) * 9. + (ui[4] - ui[2]) * 
		    45.) / sxtydr;
/*      print *,"urp",urp,"uip",uip */
/* Computing 2nd power */
	    d__1 = ur[3];
/* Computing 2nd power */
	    d__2 = ui[3];
	    u2 = d__1 * d__1 + d__2 * d__2;
	    derr = (urp * ur[3] + uip * ui[3]) / u2;
	    deri = (uip * ur[3] - urp * ui[3]) / u2;
/*      print *,"derr",derr,"deri",deri */
/*      print *,"derr",derr */

/*          SCATTERING MATRIX */

	    snr = couout_2.fp[optout_1.mwvs - 1] - couout_2.f[optout_1.mwvs - 
		    1] * derr + couout_2.g[optout_1.mwvs - 1] * deri;
	    sni = couout_2.gp[optout_1.mwvs - 1] - couout_2.f[optout_1.mwvs - 
		    1] * deri - couout_2.g[optout_1.mwvs - 1] * derr;
	    sdr = couout_2.f[optout_1.mwvs - 1] * derr + couout_2.g[
		    optout_1.mwvs - 1] * deri - couout_2.fp[optout_1.mwvs - 1]
		    ;
	    sdi = couout_2.gp[optout_1.mwvs - 1] + couout_2.f[optout_1.mwvs - 
		    1] * deri - couout_2.g[optout_1.mwvs - 1] * derr;
/* Computing 2nd power */
	    d__1 = sdr;
/* Computing 2nd power */
	    d__2 = sdi;
	    snn = d__1 * d__1 + d__2 * d__2;
	    optout_1.sr[optout_1.mwvs + ms * 250 - 251] = (snr * sdr + sni * 
		    sdi) / snn;
	    optout_1.si[optout_1.mwvs + ms * 250 - 251] = (sni * sdr - snr * 
		    sdi) / snn;
/*      print *,"sr",sr(MWVS,MS),"si",si(MWVS,MS) */
/*      print *,"fp",FP(MWVS),"f",F(MWVS),"gp",gP(MWVS),"g",g(MWVS) */
/* L249: */
	}
	if (mxl_1.lset == 1) {
	    goto L250;
	}
	i__2 = spin_1.ispin;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if ((d__1 = optout_1.sr[optout_1.mwvs + i__ * 250 - 251], abs(
		    d__1)) <= .99995 || (d__2 = optout_1.si[optout_1.mwvs + 
		    i__ * 250 - 251], abs(d__2)) >= 5e-5) {
		goto L5251;
	    }
/* L5247: */
	}
	goto L300;
L5251:
	if (optout_1.mwvs < lmax) {
	    goto L250;
	}
	s_wsfe(&io___248);
	e_wsfe();
	goto L300;
L250:
	;
    }
    optout_1.mwvs = mxl_1.lmx;
    if (mxl_1.lset == 1) {
	goto L300;
    }
    s_wsfe(&io___249);
    e_wsfe();

/* C */
L300:
    return 0;
} /* optmod_ */

#undef wspx
#undef uspx
#undef rspx
#undef norm
#undef wsx
#undef usx
#undef rsx
#undef rix
#undef wdx
#undef rcx
#undef asp
#undef as
#undef ai


