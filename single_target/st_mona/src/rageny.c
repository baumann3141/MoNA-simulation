/** \file
 * This file was generated from the fortran program rageny.F
 * from Betram Blank.  Concerning simple-track (st) it is just a hack
 * to have some quick energy loss calculation available for st.

 * At the end there is a test section, where tests with composites and pure
 * materials are performed.
 */

/* rageny.F -- translated by f2c (version 20050501).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "deb_err.h"
#include "f2c.h"

/* Common Block Declarations */

struct {
    doublereal ap, zp, atc[5], ztc[5], rtc[5];
    integer idedx, iopt;
} dxdec_;

#define dxdec_1 dxdec_

struct {
    doublereal etab1[1000], etab2[1000], etab3[1000], etab4[1000], etab5[1000]
	    , rgtab1[1000], rgtab2[1000], rgtab3[1000], rgtab4[1000], rgtab5[
	    1000];
} rge_;

#define rge_1 rge_

struct {
    doublereal zp, zt, a6, a7, a8, a9, conv, aco, bco;
} dedx_;

#define dedx_1 dedx_

/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static integer c__3 = 3;
static integer c__4 = 4;
static integer c__5 = 5;
static integer c__6 = 6;
static doublereal c_b104 = 120.;
static doublereal c_b109 = 0.;
static doublereal c_b110 = 1e-4;
static integer c__1000 = 1000;
static doublereal c_b268 = 10.;
static doublereal c_b280 = 4.;
static doublereal c_b281 = .98;
static doublereal c_b282 = 6.;
static doublereal c_b284 = 13.;
static doublereal c_b285 = .9;
static doublereal c_b286 = 50.;
static doublereal c_b287 = .88;
static doublereal c_b288 = 73.;
static doublereal c_b290 = 82.;
static doublereal c_b291 = .8;
static integer c__92 = 92;
static doublereal c_b341 = 2.;
static integer c__2 = 2;

/*<       PROGRAM RAGENY >*/
/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static char composite[20*21] = "SCI                 " "WATER            "
	    "   " "P10                 " "AIR                 " "BRASS       "
	    "        " "CONCRETE            " "ISOBUTANE           " "METHANE"
	    "             " "MYLAR               " "POLYETHYLENE        " 
	    "CSI                 " "NAI                 " "STEEL            "
	    "   " "TEFLON              " "KEVLAR              " "EPOXY       "
	    "        " "ISOFR               " "POLYPROPYLENE       " "GLASS  "
	    "             " "KAPTON              " "BBTE                ";
    static doublereal atable[105]	/* was [5][21] */ = { 12.,1.,0.,0.,0.,
	    1.,16.,0.,0.,0.,40.,12.,1.,0.,0.,14.,16.,40.,0.,0.,63.5,65.4,
	    207.2,0.,0.,1.,16.,26.98,28.1,40.1,1.,12.,0.,0.,0.,1.,12.,0.,0.,
	    0.,1.,12.,16.,0.,0.,1.,12.,0.,0.,0.,132.9,126.9,0.,0.,0.,22.99,
	    126.9,0.,0.,0.,55.85,52.,58.7,12.,0.,12.,19.,0.,0.,0.,1.,12.,14.,
	    16.,0.,1.,12.,16.,0.,0.,1.,12.,19.,0.,0.,1.,12.,0.,0.,0.,11.,16.,
	    23.,27.,28.,1.,12.,14.,16.,0.,40.,12.,1.,0.,0. };
    static doublereal ztable[105]	/* was [5][21] */ = { 6.,1.,0.,0.,0.,
	    1.,8.,0.,0.,0.,18.,6.,1.,0.,0.,7.,8.,18.,0.,0.,29.,30.,82.,0.,0.,
	    1.,8.,13.,14.,18.,1.,6.,0.,0.,0.,1.,6.,0.,0.,0.,1.,6.,8.,0.,0.,1.,
	    6.,0.,0.,0.,55.,53.,0.,0.,0.,11.,53.,0.,0.,0.,26.,24.,28.,6.,0.,
	    6.,9.,0.,0.,0.,1.,6.,7.,8.,0.,1.,6.,8.,0.,0.,1.,6.,9.,0.,0.,1.,6.,
	    0.,0.,0.,5.,8.,11.,13.,14.,1.,6.,7.,8.,0.,18.,6.,1.,0.,0. };
    static doublereal rtable[105]	/* was [5][21] */ = { 10.,11.,0.,0.,
	    0.,2.,1.,0.,0.,0.,90.,10.,40.,0.,0.,5.32,1.43,.864,0.,0.,.968,
	    .539,.0157,0.,0.,1.,5.6,.304,2.024,.371,10.,4.,0.,0.,0.,4.,1.,0.,
	    0.,0.,4.,5.,2.,0.,0.,2.,1.,0.,0.,0.,1.,1.,0.,0.,0.,1.,1.,0.,0.,0.,
	    1.,.2652,.1174,.0064,0.,2.,4.,0.,0.,0.,10.,1.666,.143,.125,0.,19.,
	    3.,.1875,0.,0.,20.,1.333,1.6842,0.,0.,2.,1.,0.,0.,0.,.7721,6.1948,
	    .12401,.02835,1.,10.,22.,4.,8.,0.,900.,.01,.01,0.,0. };

    /* Format strings */
    static char fmt_1013[] = "(\002 <I>: Density used: \002,f8.5,\002 g/cm"
	    "**3\002)";
    static char fmt_2[] = "(\002 (\002,i2,\002,\002,i3,\002)   -->   (\002,i"
	    "2,\002,\002,i3,\002)\002)";
    static char fmt_3[] = "(\002 (\002,i2,\002,\002,i3,\002)   -->   \002,a1"
	    "0)";
    static char fmt_4[] = "(\002 Incident energy:      \002,f8.3,\002 MeV/u "
	    "             beta1=\002,f5.3)";
    static char fmt_5[] = "(\002 Initial range:      \002,f10.2,\002 mg/cm^2"
	    " |\002,f12.5,\002 cm\002)";
    static char fmt_6[] = "(\002 Exit energy:          \002,f8.3,\002 MeV/u "
	    "             beta2=\002,f5.3)";
    static char fmt_7[] = "(\002 Residual range:     \002,f10.2,\002 mg/cm^2"
	    " |\002,f12.5,\002 cm\002)";
    static char fmt_8[] = "(\002 Energy loss:          \002,f8.3,\002 MeV/u "
	    "        (b2-b1)/b1=\002,f7.5)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublereal d__1, d__2, d__3, d__4;
    olist o__1;

    /* Builtin functions */
    integer f_open(olist *);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void), s_rnge(char *, integer, char *, integer), s_cmp(
	    char *, char *, ftnlen, ftnlen), i_dnnt(doublereal *), s_wsfe(
	    cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double sqrt(doublereal);

    /* Local variables */
    static char ctar_old__[20], cpro_old__[8];
    static integer i__, k;
    static doublereal l;
    static char c1[1];
    extern /* Subroutine */ int uppercase_(char *, ftnlen);
    static char ci[8];
    static integer ik;
    static doublereal at, td, rg, tf, ti, rl, zt;
    static integer irc;
    static doublereal rgf, rgi;
    static integer irg;
    static doublereal rho;
    extern /* Subroutine */ int pcha_(char *, char *, ftnlen, ftnlen);
    static integer i_hs__;
    extern doublereal dxde_();
    static char ctar[20];
    static integer itar;
    static char cpro[8];
    extern /* Subroutine */ int pilo_(char *, integer *, ftnlen), prtc_(char *
	    , integer *, ftnlen);
    static integer ipro;
    extern /* Subroutine */ int pyes_(char *, char *, ftnlen, ftnlen), prso_(
	    char *, doublereal *, integer *, ftnlen);
    static doublereal beta1, beta2, brho1, brho2;
    extern /* Subroutine */ int range_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, integer *), nolbl_(
	    char *, ftnlen);
    static integer icomp;
    static char p_yes__[1];
    static doublereal gamma1, gamma2;
    extern doublereal bbsim1_(D_fp, doublereal *, doublereal *, doublereal *);
    extern /* Subroutine */ int c_inch__(integer *, char *, ftnlen), c_chre__(
	    char *, doublereal *, ftnlen);
    static integer length;
    extern /* Subroutine */ int dedxhu_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), energy_(
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *);
    extern doublereal rhodata_(integer *);
    extern /* Subroutine */ int element_(doublereal *, doublereal *, char *, 
	    integer *, integer *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___10 = { 0, 6, 0, 0, 0 };
    static cilist io___11 = { 0, 6, 0, 0, 0 };
    static cilist io___12 = { 0, 6, 0, 0, 0 };
    static cilist io___14 = { 0, 6, 0, 0, 0 };
    static cilist io___19 = { 0, 6, 0, 0, 0 };
    static cilist io___21 = { 0, 6, 0, 0, 0 };
    static cilist io___22 = { 0, 6, 0, 0, 0 };
    static cilist io___30 = { 0, 6, 0, 0, 0 };
    static cilist io___32 = { 0, 6, 0, 0, 0 };
    static cilist io___34 = { 0, 6, 0, 0, 0 };
    static cilist io___37 = { 0, 6, 0, fmt_1013, 0 };
    static cilist io___38 = { 0, 6, 0, 0, 0 };
    static cilist io___40 = { 0, 6, 0, 0, 0 };
    static cilist io___41 = { 0, 6, 0, 0, 0 };
    static cilist io___42 = { 0, 6, 0, 0, 0 };
    static cilist io___43 = { 0, 6, 0, 0, 0 };
    static cilist io___44 = { 0, 6, 0, 0, 0 };
    static cilist io___45 = { 0, 6, 0, 0, 0 };
    static cilist io___56 = { 0, 6, 0, 0, 0 };
    static cilist io___57 = { 0, 6, 0, 0, 0 };
    static cilist io___58 = { 0, 6, 0, 0, 0 };
    static cilist io___59 = { 0, 6, 0, fmt_2, 0 };
    static cilist io___60 = { 0, 6, 0, fmt_3, 0 };
    static cilist io___61 = { 0, 6, 0, fmt_4, 0 };
    static cilist io___62 = { 0, 6, 0, fmt_5, 0 };
    static cilist io___63 = { 0, 6, 0, 0, 0 };
    static cilist io___64 = { 0, 6, 0, fmt_6, 0 };
    static cilist io___65 = { 0, 6, 0, fmt_7, 0 };
    static cilist io___66 = { 0, 6, 0, 0, 0 };
    static cilist io___67 = { 0, 6, 0, fmt_8, 0 };
    static cilist io___68 = { 0, 6, 0, fmt_8, 0 };
    static cilist io___69 = { 0, 6, 0, 0, 0 };
    static cilist io___70 = { 0, 6, 0, "('  Energy   beta      gamma        "
	    "   brho       ')", 0 };
    static cilist io___71 = { 0, 6, 0, "('----------------------------------"
	    "--------------')", 0 };
    static cilist io___72 = { 0, 6, 0, "(f8.3,'    ',F5.3,'  ' ,e10.3,'     "
	    "',f7.4,'  (before)')", 0 };
    static cilist io___73 = { 0, 6, 0, "(f8.3,'    ',F5.3,'  ' ,e10.3,'     "
	    "',f7.4,'  (after)')", 0 };
    static cilist io___74 = { 0, 6, 0, "(f8.3,'    ',F5.3,'   ',e10.4,'     "
	    "',f7.5,'  (after/before)')", 0 };
    static cilist io___75 = { 0, 6, 0, "(' ',f8.4,'  ',F7.4,'  ',e11.5,'  ',"
	    "f9.5,'  (% change)')", 0 };
    static cilist io___76 = { 0, 6, 0, 0, 0 };


/*     f77 rageny.f -Nl100 -L/scratch5/blank/fort/util -lutil */
/*       -L/usr/local/cern/new/lib -lkernlib -lmathlib -lpacklib  -o rage */

/*     ////////////////////////////// */
/*     CALCULATES RANGES AND ENERGIES */
/*     ////////////////////////////// */

/*     Option 1: high-energy fit (E > 100 MeV/u) */
/*     Option 2: low-energy  integration (E < 100 MeV/u) */


/*  the main reference: */
/* ==================== */
/* Hubert F, Bimbot R, Gauvin H. Range and stopping-power tables for 2.5-500 */
/* MeV/nucleon heavy ions in solids. */
/* Atomic Data & Nuclear Data Tables, vol.46, no.1, Sept. 1990, pp.1-213. USA */

/*<       IMPLICIT NONE >*/
/*<       INTEGER I_HS              ! for my stuff.  HS. >*/
/*<       DOUBLE PRECISION AT,ZT,TI,RGI,TF,RGF,TD,L, DXDE, BBSIM1, RG, RHO, RL >*/
/*<       DOUBLE PRECISION RHODATA, beta1, beta2, gamma1, gamma2, brho1, brho2 >*/
/*<       INTEGER IK, I, ICOMP, IRC, LENGTH, K, IRG, IPARA >*/
/*<       INTEGER IPRO, ITAR >*/
/*<       CHARACTER*1 P_YES, C1 >*/
/*<       CHARACTER*8 CPRO, CI, CPRO_OLD >*/
/*<       PARAMETER (IPARA = 21)    ! number of composites >*/
/*<       CHARACTER*20 CTAR, CTAR_OLD >*/
/*<       CHARACTER*2 DATAPATH >*/
/*<       PARAMETER (DATAPATH='./') >*/
/* *** composites */
/*<       CHARACTER*20 COMPOSITE(IPARA) >*/
/*<       DOUBLE PRECISION ATABLE(5,IPARA), ZTABLE(5,IPARA), RTABLE(5,IPARA) >*/
/*<       EXTERNAL DXDE >*/
/*<       EXTERNAL RANGE >*/

/*<       DOUBLE PRECISION AP, ZP, ATC(5), ZTC(5), RTC(5) >*/
/*<       INTEGER IDEDX, IOPT >*/
/*<       COMMON /DXDEC/ AP, ZP, ATC, ZTC, RTC, IDEDX, IOPT >*/

/*<        >*/
/*<        >*/
/*<        >*/

/*<        >*/
/*<        >*/
/*                                               (2*isobutane+8*freon) */
/*<        >*/
/*<        >*/
/*<       OPEN(UNIT=13, FILE=DATAPATH//'/radata.hubert') >*/
    o__1.oerr = 0;
    o__1.ounit = 13;
    o__1.ofnmlen = 16;
    o__1.ofnm = "../dat_files//radata.hubert";
    o__1.orl = 0;
    o__1.osta = 0;
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
/*<       OPEN(UNIT=14, FILE=DATAPATH//'/rgdata.hubert') >*/
    o__1.oerr = 0;
    o__1.ounit = 14;
    o__1.ofnmlen = 16;
    o__1.ofnm = "../dat_files//rgdata.hubert";
    o__1.orl = 0;
    o__1.osta = 0;
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);

/* *** DEFAULTS */
/*<       CPRO = 'C' >*/
    s_copy(cpro, "C", (ftnlen)8, (ftnlen)1);
/*<       CTAR = 'SCI' >*/
    s_copy(ctar, "SCI", (ftnlen)20, (ftnlen)3);
/*<       IOPT = 2 >*/
    dxdec_1.iopt = 2;
/*<       RL = 100. >*/
    rl = 100.f;
/*<       TI = 80. >*/
    ti = 80.f;
/*<       RGI = 0.0 >*/
    rgi = 0.f;
/* *** write possible composite materials to screen at beginning, HS */
/*<       WRITE(*,*)'composites: ' >*/
    s_wsle(&io___10);
    do_lio(&c__9, &c__1, "composites: ", (ftnlen)12);
    e_wsle();
/*<       WRITE(*,*)'=========== ' >*/
    s_wsle(&io___11);
    do_lio(&c__9, &c__1, "=========== ", (ftnlen)12);
    e_wsle();
/*<       WRITE(*,*)(COMPOSITE(I_HS),I_HS=1,IPARA) >*/
    s_wsle(&io___12);
    for (i_hs__ = 1; i_hs__ <= 21; ++i_hs__) {
	do_lio(&c__9, &c__1, composite + ((i__1 = i_hs__ - 1) < 21 && 0 <= 
		i__1 ? i__1 : s_rnge("composite", i__1, "rageny_", (ftnlen)
		145)) * 20, (ftnlen)20);
    }
    e_wsle();

/*<     1 CONTINUE >*/
L1:
/*<       WRITE(6,*) >*/
    s_wsle(&io___14);
    e_wsle();

/* *** PROJECTILE */
/*<  101  CALL PCHA('Enter Z(projectile) or symbol', CPRO ) >*/
L101:
    pcha_("Enter Z(projectile) or symbol", cpro, (ftnlen)29, (ftnlen)8);
/*<       call uppercase(cpro) >*/
    uppercase_(cpro, (ftnlen)8);
/*<       IF( CPRO .NE. CPRO_OLD ) IPRO = 0 >*/
    if (s_cmp(cpro, cpro_old__, (ftnlen)8, (ftnlen)8) != 0) {
	ipro = 0;
    }
/*<       CPRO_OLD = CPRO >*/
    s_copy(cpro_old__, cpro, (ftnlen)8, (ftnlen)8);

/*<       DO 1001 IK = 0, 9 >*/
    for (ik = 0; ik <= 9; ++ik) {
/*<         CALL C_INCH( IK , CI ) >*/
	c_inch__(&ik, ci, (ftnlen)8);
/*<         IF( CI .EQ. CPRO(1:1) ) THEN >*/
	if (s_cmp(ci, cpro, (ftnlen)8, (ftnlen)1) == 0) {
/*<           CALL C_CHRE( CPRO, ZP ) >*/
	    c_chre__(cpro, &dxdec_1.zp, (ftnlen)8);
/*<           IF( ZP .GT. 92. ) THEN >*/
	    if (dxdec_1.zp > 92.f) {
/*<             WRITE(6,*) '<E>: ZP > 92!' >*/
		s_wsle(&io___19);
		do_lio(&c__9, &c__1, "<E>: ZP > 92!", (ftnlen)13);
		e_wsle();
/*<             GOTO 101 >*/
		goto L101;
/*<           ENDIF >*/
	    }
/*<           IF( IPRO .EQ. 0 ) THEN >*/
	    if (ipro == 0) {
/*<             CALL ELEMENT( ZP, AP, CPRO, 3, IRC ) >*/
		element_(&dxdec_1.zp, &dxdec_1.ap, cpro, &c__3, &irc, (ftnlen)
			8);
/*<             IPRO = 1 >*/
		ipro = 1;
/*<           ENDIF >*/
	    }
/*<           IF( IRC .EQ. -1 ) GOTO 101 >*/
	    if (irc == -1) {
		goto L101;
	    }
/*<           GOTO 1101 >*/
	    goto L1101;
/*<         ENDIF >*/
	}
/*<  1001 CONTINUE >*/
/* L1001: */
    }

/*<       IF( IPRO .EQ. 0 ) THEN >*/
    if (ipro == 0) {
/*<         CALL ELEMENT( ZP, AP, CPRO, 4, IRC ) >*/
	element_(&dxdec_1.zp, &dxdec_1.ap, cpro, &c__4, &irc, (ftnlen)8);
/*<         IPRO = 1 >*/
	ipro = 1;
/*<       ENDIF >*/
    }
/*<       IF( IRC .EQ. -1 ) GOTO 101 >*/
    if (irc == -1) {
	goto L101;
    }

/*<  1101 CONTINUE >*/
L1101:
/*<       write(*,*)' ap',ap >*/
    s_wsle(&io___21);
    do_lio(&c__9, &c__1, " ap", (ftnlen)3);
    do_lio(&c__5, &c__1, (char *)&dxdec_1.ap, (ftnlen)sizeof(doublereal));
    e_wsle();
/*<       CALL PRSO('Enter A(projectile)', AP, 3 ) >*/
    prso_("Enter A(projectile)", &dxdec_1.ap, &c__3, (ftnlen)19);
/*<       IF( ZP .GT. AP ) THEN >*/
    if (dxdec_1.zp > dxdec_1.ap) {
/*<         WRITE(6,*) ' <E>: ZP > AP ! ' >*/
	s_wsle(&io___22);
	do_lio(&c__9, &c__1, " <E>: ZP > AP ! ", (ftnlen)16);
	e_wsle();
/*<         GOTO 101 >*/
	goto L101;
/*<       ENDIF >*/
    }

/*<       IF( ZP .LE. 0 ) GOTO 999 >*/
    if (dxdec_1.zp <= 0.) {
	goto L999;
    }

/* *** TARGET */
/*<       ICOMP = -1 >*/
    icomp = -1;
/*<  102  CONTINUE >*/
L102:
/*<       CALL PCHA('Enter Z(target) or symbol', CTAR ) >*/
    pcha_("Enter Z(target) or symbol", ctar, (ftnlen)25, (ftnlen)20);
/*<       IF( CTAR .NE. CTAR_OLD ) ITAR = 0 >*/
    if (s_cmp(ctar, ctar_old__, (ftnlen)20, (ftnlen)20) != 0) {
	itar = 0;
    }
/*<       CTAR_OLD = CTAR >*/
    s_copy(ctar_old__, ctar, (ftnlen)20, (ftnlen)20);

/*  ** DETERMINE LENGTH OF CTAR */
/*<       CALL NOLBL(CTAR) >*/
    nolbl_(ctar, (ftnlen)20);
/*<       DO I = 1, 20 >*/
    for (i__ = 1; i__ <= 20; ++i__) {
/*<         C1 = CTAR(21-I:21-I) >*/
	i__1 = 21 - i__ - 1;
	s_copy(c1, ctar + i__1, (ftnlen)1, 21 - i__ - i__1);
/*<         IF( C1 .NE. ' ' ) THEN >*/
	if (*(unsigned char *)c1 != ' ') {
/*<           LENGTH = 21 - I >*/
	    length = 21 - i__;
/*<           GOTO 1021 >*/
	    goto L1021;
/*<         ENDIF >*/
	}
/*<       ENDDO >*/
    }

/*<  1021 IF( LENGTH .LE. 2 ) THEN >*/
L1021:
    if (length <= 2) {
/*  **   NORMAL ELEMENT */
/*<         DO 1002 IK = 0, 9 >*/
	for (ik = 0; ik <= 9; ++ik) {
/*<           CALL C_INCH( IK , CI ) >*/
	    c_inch__(&ik, ci, (ftnlen)8);
/*<           IF( CI .EQ. CTAR(1:1) ) THEN >*/
	    if (s_cmp(ci, ctar, (ftnlen)8, (ftnlen)1) == 0) {
/*<             CALL C_CHRE( CTAR, ZT ) >*/
		c_chre__(ctar, &zt, (ftnlen)20);
/*<             IF( ZT .GT. 92. ) THEN >*/
		if (zt > 92.f) {
/*<               WRITE(6,*) '<E>: ZT > 92!' >*/
		    s_wsle(&io___30);
		    do_lio(&c__9, &c__1, "<E>: ZT > 92!", (ftnlen)13);
		    e_wsle();
/*<               GOTO 102 >*/
		    goto L102;
/*<             ENDIF >*/
		}
/*<             IF( ITAR .EQ. 0 ) THEN >*/
		if (itar == 0) {
/*<               CALL ELEMENT( ZT, AT, CTAR, 1, IRC ) >*/
		    element_(&zt, &at, ctar, &c__1, &irc, (ftnlen)20);
/*<               ITAR = 1 >*/
		    itar = 1;
/*<             ENDIF >*/
		}
/*<             IF( IRC .EQ. -1 ) GOTO 102 >*/
		if (irc == -1) {
		    goto L102;
		}
/*<             GOTO 1102 >*/
		goto L1102;
/*<           ENDIF >*/
	    }
/*<  1002   CONTINUE >*/
/* L1002: */
	}

/*<         IF( ITAR .EQ. 0 ) THEN >*/
	if (itar == 0) {
/*<           CALL ELEMENT( ZT, AT, CTAR, 4, IRC ) >*/
	    element_(&zt, &at, ctar, &c__4, &irc, (ftnlen)20);
/*<           ITAR = 1 >*/
	    itar = 1;
/*<         ENDIF >*/
	}
/*<         IF( IRC .EQ. -1 ) GOTO 102 >*/
	if (irc == -1) {
	    goto L102;
	}

/*<  1102   CALL PRSO('Enter A(target)', AT, 3 ) >*/
L1102:
	prso_("Enter A(target)", &at, &c__3, (ftnlen)15);
/*<         IF( ZT .GT. AT ) THEN >*/
	if (zt > at) {
/*<           WRITE(6,*) ' <E>: ZT > AT ! ' >*/
	    s_wsle(&io___32);
	    do_lio(&c__9, &c__1, " <E>: ZT > AT ! ", (ftnlen)16);
	    e_wsle();
/*<           GOTO 102 >*/
	    goto L102;
/*<         ENDIF >*/
	}

/*<       ELSE >*/
    } else {
/*  **   IF LENGTH > 2 ===> COMPOSITE */
/* ***   TARGET IS A COMPOSITE, CHECK WITH TABLE OF ALLOWED COMPOSITES */
/*<         ICOMP = 0 >*/
	icomp = 0;
/*  **   RESET */
/*<         DO I = 1, 5 >*/
	for (i__ = 1; i__ <= 5; ++i__) {
/*<           ATC(I) = 0. >*/
	    dxdec_1.atc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "atc", i__1, "rageny_", (ftnlen)243)] = 0.f;
/*<           ZTC(I) = 0. >*/
	    dxdec_1.ztc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "ztc", i__1, "rageny_", (ftnlen)244)] = 0.f;
/*<           RTC(I) = 0. >*/
	    dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "rtc", i__1, "rageny_", (ftnlen)245)] = 0.f;
/*<         ENDDO >*/
	}

/*<         CALL UPPERCASE(CTAR) >*/
	uppercase_(ctar, (ftnlen)20);
/*<         DO I = 1, IPARA >*/
	for (i__ = 1; i__ <= 21; ++i__) {
/*<           IF( CTAR .EQ. COMPOSITE(I) ) THEN >*/
	    if (s_cmp(ctar, composite + ((i__1 = i__ - 1) < 21 && 0 <= i__1 ? 
		    i__1 : s_rnge("composite", i__1, "rageny_", (ftnlen)250)) 
		    * 20, (ftnlen)20, (ftnlen)20) == 0) {
/*<             ICOMP = I >*/
		icomp = i__;
/*<             DO K = 1, 5 >*/
		for (k = 1; k <= 5; ++k) {
/*<               ATC(K) = ATABLE(K,I) >*/
		    dxdec_1.atc[(i__1 = k - 1) < 5 && 0 <= i__1 ? i__1 : 
			    s_rnge("atc", i__1, "rageny_", (ftnlen)253)] = 
			    atable[(i__2 = k + i__ * 5 - 6) < 105 && 0 <= 
			    i__2 ? i__2 : s_rnge("atable", i__2, "rageny_", (
			    ftnlen)253)];
/*<               ZTC(K) = ZTABLE(K,I) >*/
		    dxdec_1.ztc[(i__1 = k - 1) < 5 && 0 <= i__1 ? i__1 : 
			    s_rnge("ztc", i__1, "rageny_", (ftnlen)254)] = 
			    ztable[(i__2 = k + i__ * 5 - 6) < 105 && 0 <= 
			    i__2 ? i__2 : s_rnge("ztable", i__2, "rageny_", (
			    ftnlen)254)];
/*<               RTC(K) = RTABLE(K,I) >*/
		    dxdec_1.rtc[(i__1 = k - 1) < 5 && 0 <= i__1 ? i__1 : 
			    s_rnge("rtc", i__1, "rageny_", (ftnlen)255)] = 
			    rtable[(i__2 = k + i__ * 5 - 6) < 105 && 0 <= 
			    i__2 ? i__2 : s_rnge("rtable", i__2, "rageny_", (
			    ftnlen)255)];
/*<             ENDDO >*/
		}
/*<             GOTO 103 >*/
		goto L103;
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<        >*/
	s_wsle(&io___34);
	do_lio(&c__9, &c__1, "<E>: ", (ftnlen)5);
	do_lio(&c__9, &c__1, ctar, (ftnlen)20);
	do_lio(&c__9, &c__1, " is not forseen as a composite material", (
		ftnlen)39);
	e_wsle();
/*<         GOTO 102 >*/
	goto L102;
/*<       ENDIF >*/
    }

/*<  103   >*/
L103:
    prtc_("ENTER target thickness (in mg/cm^2 ---> positive values)", &c__6, (
	    ftnlen)56);
/*<        >*/
    prso_("                       (in cm      ---> negative values)", &rl, &
	    c__3, (ftnlen)56);

/*<       IF( ICOMP .LE. 0 ) THEN >*/
    if (icomp <= 0) {
/*       ELEMENT... */
/*<         RHO = RHODATA(NINT(ZT)) >*/
	i__1 = i_dnnt(&zt);
	rho = rhodata_(&i__1);
/*<       ELSE >*/
    } else {
/*<         RHO = RHODATA(92+ICOMP) >*/
	i__1 = icomp + 92;
	rho = rhodata_(&i__1);
/*<       ENDIF >*/
    }
/*<       IF( RL .LT. 0. ) THEN >*/
    if (rl < 0.f) {
/*<         L = - RL * RHO *  1000. >*/
	l = -rl * rho * 1e3f;
/*<         WRITE(6,1013) RHO >*/
	s_wsfe(&io___37);
	do_fio(&c__1, (char *)&rho, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<  1013   FORMAT(' <I>: Density used: ', F8.5, ' g/cm**3') >*/
/*<         WRITE(6,*) >*/
	s_wsle(&io___38);
	e_wsle();
/*<       ELSE >*/
    } else {
/*<         L = RL >*/
	l = rl;
/*<       ENDIF >*/
    }

/*<       IF( L .LE. 0.0 ) GOTO 999 >*/
    if (l <= 0.f) {
	goto L999;
    }

/*<  104  CALL PRTC('USE HIGH-ENERGY FIT  ==> (1)', 6) >*/
/* L104: */
    prtc_("USE HIGH-ENERGY FIT  ==> (1)", &c__6, (ftnlen)28);
/*<       CALL PILO('USE F. HUBERT TABLES ==> (2)', IOPT) >*/
    pilo_("USE F. HUBERT TABLES ==> (2)", &dxdec_1.iopt, (ftnlen)28);

/*<  105  CALL PRSO('ENTER energy (MeV/u) // 0 to ENTER range value ',TI,1) >*/
/* L105: */
    prso_("ENTER energy (MeV/u) // 0 to ENTER range value ", &ti, &c__1, (
	    ftnlen)47);
/*<       IRG = 0 >*/
    irg = 0;
/*<       IF( TI .LE. 1.0E-10 ) THEN >*/
    if (ti <= 1e-10f) {
/*<         TI = 0.0 >*/
	ti = 0.f;
/*<         CALL PRSO('ENTER range (mg/cm^2)',RGI,3) >*/
	prso_("ENTER range (mg/cm^2)", &rgi, &c__3, (ftnlen)21);
/*<         IF( RGI .LE. 1.0E-10 ) GOTO 999 >*/
	if (rgi <= 1e-10f) {
	    goto L999;
	}
/*<         IRG = 1 >*/
	irg = 1;
/*<       ENDIF >*/
    }

/*<       IF( IOPT .EQ. 2 .AND. TI .GT. 120. ) THEN >*/
    if (dxdec_1.iopt == 2 && ti > 120.f) {
/*<         WRITE(6,*) >*/
	s_wsle(&io___40);
	e_wsle();
/*<         WRITE(6,*) '<W>: Lower-energy option: E < 100 MeV/u' >*/
	s_wsle(&io___41);
	do_lio(&c__9, &c__1, "<W>: Lower-energy option: E < 100 MeV/u", (
		ftnlen)39);
	e_wsle();
/*<         WRITE(6,*) >*/
	s_wsle(&io___42);
	e_wsle();
/*<       ELSE IF( IOPT .EQ. 1 .AND. TI .LT. 80. ) THEN >*/
    } else if (dxdec_1.iopt == 1 && ti < 80.f) {
/*<         WRITE(6,*) >*/
	s_wsle(&io___43);
	e_wsle();
/*<         WRITE(6,*) '<W>: High-energy option: E > 100 MeV/u' >*/
	s_wsle(&io___44);
	do_lio(&c__9, &c__1, "<W>: High-energy option: E > 100 MeV/u", (
		ftnlen)38);
	e_wsle();
/*<         WRITE(6,*) >*/
	s_wsle(&io___45);
	e_wsle();
/*<       ENDIF >*/
    }

/* *** start range - energy calculations */

/*  ** check for composites */
/*<       IF( ICOMP .LE. 0 ) THEN >*/
    if (icomp <= 0) {
/*   *   NO COMPOSITES */

/*<         IF( IRG .EQ. 0 ) THEN >*/
	if (irg == 0) {
/*         RANGE */
/*<           CALL RANGE(AP, ZP, AT, ZT, TI, RGI, IOPT) >*/
	    range_(&dxdec_1.ap, &dxdec_1.zp, &at, &zt, &ti, &rgi, &
		    dxdec_1.iopt);
/*<           RGF = RGI - L >*/
	    rgf = rgi - l;
/*<           CALL ENERGY(AP, ZP, AT, ZT, RGF, TF, IOPT) >*/
	    energy_(&dxdec_1.ap, &dxdec_1.zp, &at, &zt, &rgf, &tf, &
		    dxdec_1.iopt);
/*<           TD = TI - TF >*/
	    td = ti - tf;

/*<         ELSE IF( IRG .EQ. 1 ) THEN >*/
	} else if (irg == 1) {
/*         ENERGY */
/*<           CALL ENERGY(AP, ZP, AT, ZT, RGI, TI, IOPT) >*/
	    energy_(&dxdec_1.ap, &dxdec_1.zp, &at, &zt, &rgi, &ti, &
		    dxdec_1.iopt);
/*<           RGF = RGI - L >*/
	    rgf = rgi - l;
/*<           CALL ENERGY(AP, ZP, AT, ZT, RGF, TF, IOPT) >*/
	    energy_(&dxdec_1.ap, &dxdec_1.zp, &at, &zt, &rgf, &tf, &
		    dxdec_1.iopt);
/*<           TD = TI - TF >*/
	    td = ti - tf;
/*<         ENDIF >*/
	}

/*<       ELSE IF( ICOMP .GT. 0 ) THEN >*/
    } else if (icomp > 0) {
/*   *   COMPOSITES */
/*<         IF( IOPT .EQ. 2 ) THEN >*/
	if (dxdec_1.iopt == 2) {
/*   *      GENERATE RANGE TABLES */
/*<        >*/
	    if (dxdec_1.rtc[0] > 0.f) {
		dedxhu_(&dxdec_1.zp, &dxdec_1.ap, dxdec_1.ztc, &c_b104, 
			rge_1.etab1, rge_1.rgtab1);
	    }
/*<        >*/
	    if (dxdec_1.rtc[1] > 0.f) {
		dedxhu_(&dxdec_1.zp, &dxdec_1.ap, &dxdec_1.ztc[1], &c_b104, 
			rge_1.etab2, rge_1.rgtab2);
	    }
/*<        >*/
	    if (dxdec_1.rtc[2] > 0.f) {
		dedxhu_(&dxdec_1.zp, &dxdec_1.ap, &dxdec_1.ztc[2], &c_b104, 
			rge_1.etab3, rge_1.rgtab3);
	    }
/*<        >*/
	    if (dxdec_1.rtc[3] > 0.f) {
		dedxhu_(&dxdec_1.zp, &dxdec_1.ap, &dxdec_1.ztc[3], &c_b104, 
			rge_1.etab4, rge_1.rgtab4);
	    }
/*<        >*/
	    if (dxdec_1.rtc[4] > 0.f) {
		dedxhu_(&dxdec_1.zp, &dxdec_1.ap, &dxdec_1.ztc[4], &c_b104, 
			rge_1.etab5, rge_1.rgtab5);
	    }
/*<         ENDIF >*/
	}

/*<         IF( IRG .EQ. 0 ) THEN >*/
	if (irg == 0) {
/*         RANGE */
/*<           IDEDX = -1 >*/
	    dxdec_1.idedx = -1;
/*<           RGI = BBSIM1( DXDE, 0.d0, TI, 0.0001d0) >*/
	    rgi = bbsim1_((D_fp)dxde_, &c_b109, &ti, &c_b110);
/*<           RGF = RGI - L >*/
	    rgf = rgi - l;
/*<           RG = RGF >*/
	    rg = rgf;
/*<           IF( RGF .LE. 0. ) RG = 0. >*/
	    if (rgf <= 0.f) {
		rg = 0.f;
	    }
/*<           IDEDX = 1 >*/
	    dxdec_1.idedx = 1;
/*<           TF =  BBSIM1( DXDE, 0.d0, RG, 0.0001d0 ) >*/
	    tf = bbsim1_((D_fp)dxde_, &c_b109, &rg, &c_b110);
/*<           TD = TI - TF >*/
	    td = ti - tf;

/*<         ELSE IF( IRG .EQ. 1 ) THEN >*/
	} else if (irg == 1) {
/*         ENERGY */
/*<           IDEDX = 1 >*/
	    dxdec_1.idedx = 1;
/*<           TI = BBSIM1( DXDE, 0.d0, RGI, 0.0001d0 ) >*/
	    ti = bbsim1_((D_fp)dxde_, &c_b109, &rgi, &c_b110);
/*<           RGF = RGI - L >*/
	    rgf = rgi - l;
/*<           RG = RGF >*/
	    rg = rgf;
/*<           IF( RGF .LE. 0. ) RG = 0. >*/
	    if (rgf <= 0.f) {
		rg = 0.f;
	    }
/*<           IDEDX = 1 >*/
	    dxdec_1.idedx = 1;
/*<           TF =  BBSIM1( DXDE, 0.d0, RG, 0.0001d0 ) >*/
	    tf = bbsim1_((D_fp)dxde_, &c_b109, &rg, &c_b110);
/*<           TD = TI - TF >*/
	    td = ti - tf;
/*<         ENDIF >*/
	}
/*<       ENDIF >*/
    }

/* *** OUTPUT */
/*<  100  CONTINUE >*/
/* L100: */
/*<       gamma1=1.d0+TI/931.5d0 >*/
    gamma1 = ti / 931.494028 + 1.;
/*<       gamma2=1.d0+TF/931.5d0 >*/
    gamma2 = tf / 931.494028 + 1.;
/*<       beta1=0.d0 >*/
    beta1 = 0.;
/*<       beta2=0.d0 >*/
    beta2 = 0.;
/*<       if (gamma1.gt.1.d0) beta1=sqrt(1.d0-1.d0/gamma1**2) >*/
    if (gamma1 > 1.) {
/* Computing 2nd power */
	d__1 = gamma1;
	beta1 = sqrt(1. - 1. / (d__1 * d__1));
    }
/*<       if (gamma2.gt.1.d0) beta2=sqrt(1.d0-1.d0/gamma2**2) >*/
    if (gamma2 > 1.) {
/* Computing 2nd power */
	d__1 = gamma2;
	beta2 = sqrt(1. - 1. / (d__1 * d__1));
    }
/*<       brho1=beta1*gamma1*931.5d0*AP/ZP/299.d0 >*/
    brho1 = beta1 * gamma1 * 931.494028 * dxdec_1.ap / dxdec_1.zp / 299.792458;
/*<       brho2=beta2*gamma2*931.5d0*AP/ZP/299.d0 >*/
    brho2 = beta2 * gamma2 * 931.494028 * dxdec_1.ap / dxdec_1.zp / 299.792458;
/*<       IF( IOPT .EQ. 1 ) WRITE(6,*) 'HIGH-ENERGY FIT:' >*/
    if (dxdec_1.iopt == 1) {
	s_wsle(&io___56);
	do_lio(&c__9, &c__1, "HIGH-ENERGY FIT:", (ftnlen)16);
	e_wsle();
    }
/*<       IF( IOPT .EQ. 2 ) WRITE(6,*) 'F. HUBERT TABLES:' >*/
    if (dxdec_1.iopt == 2) {
	s_wsle(&io___57);
	do_lio(&c__9, &c__1, "F. HUBERT TABLES:", (ftnlen)17);
	e_wsle();
    }
/*<       WRITE(6,*) '----------------' >*/
    s_wsle(&io___58);
    do_lio(&c__9, &c__1, "----------------", (ftnlen)16);
    e_wsle();
/*<       IF( ICOMP .LE. 0 ) WRITE(6,2) NINT(ZP),NINT(AP),NINT(ZT),NINT(AT) >*/
    if (icomp <= 0) {
	s_wsfe(&io___59);
	i__1 = i_dnnt(&dxdec_1.zp);
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	i__2 = i_dnnt(&dxdec_1.ap);
	do_fio(&c__1, (char *)&i__2, (ftnlen)sizeof(integer));
	i__3 = i_dnnt(&zt);
	do_fio(&c__1, (char *)&i__3, (ftnlen)sizeof(integer));
	i__4 = i_dnnt(&at);
	do_fio(&c__1, (char *)&i__4, (ftnlen)sizeof(integer));
	e_wsfe();
    }
/*<       IF( ICOMP .GT. 0 ) WRITE(6,3) NINT(ZP),NINT(AP),CTAR >*/
    if (icomp > 0) {
	s_wsfe(&io___60);
	i__1 = i_dnnt(&dxdec_1.zp);
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	i__2 = i_dnnt(&dxdec_1.ap);
	do_fio(&c__1, (char *)&i__2, (ftnlen)sizeof(integer));
	do_fio(&c__1, ctar, (ftnlen)20);
	e_wsfe();
    }
/*<     2 FORMAT(' (',I2,',',I3,')   -->   (',I2,',',I3,')') >*/
/*<     3 FORMAT(' (',I2,',',I3,')   -->   ',A10) >*/
/*<       WRITE(6,4) TI, beta1 >*/
    s_wsfe(&io___61);
    do_fio(&c__1, (char *)&ti, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&beta1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<     4 FORMAT(' Incident energy:      ',F8.3,' MeV/u              beta1=',F5.3) >*/
/*<       WRITE(6,5) RGI, RGI / RHO / 1000. >*/
    s_wsfe(&io___62);
    do_fio(&c__1, (char *)&rgi, (ftnlen)sizeof(doublereal));
    d__1 = rgi / rho / 1e3f;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<     5 FORMAT(' Initial range:      ',F10.2,' mg/cm^2 |', F12.5, ' cm') >*/
/*<       WRITE(6,*) ' ' >*/
    s_wsle(&io___63);
    do_lio(&c__9, &c__1, " ", (ftnlen)1);
    e_wsle();
/*<       WRITE(6,6) TF, beta2 >*/
    s_wsfe(&io___64);
    do_fio(&c__1, (char *)&tf, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&beta2, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<     6 FORMAT(' Exit energy:          ',F8.3,' MeV/u              beta2=',F5.3) >*/
/*<       WRITE(6,7) RGF, RGF / RHO / 1000. >*/
    s_wsfe(&io___65);
    do_fio(&c__1, (char *)&rgf, (ftnlen)sizeof(doublereal));
    d__1 = rgf / rho / 1e3f;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<     7 FORMAT(' Residual range:     ',F10.2,' mg/cm^2 |', F12.5, ' cm') >*/
/*<       WRITE(6,*) ' ' >*/
    s_wsle(&io___66);
    do_lio(&c__9, &c__1, " ", (ftnlen)1);
    e_wsle();
/*<       if (beta1.ne.0.d0) then >*/
    if (beta1 != 0.) {
/*<         WRITE(6,8) TD, (beta2-beta1)/beta1 >*/
	s_wsfe(&io___67);
	do_fio(&c__1, (char *)&td, (ftnlen)sizeof(doublereal));
	d__1 = (beta2 - beta1) / beta1;
	do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       else >*/
    } else {
/*<         WRITE(6,8) TD, 0.d0 >*/
	s_wsfe(&io___68);
	do_fio(&c__1, (char *)&td, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&c_b109, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       endif >*/
    }
/*<     8 FORMAT(' Energy loss:          ',F8.3,' MeV/u         (b2-b1)/b1=',F7.5) >*/

/*<       WRITE(6,*) >*/
    s_wsle(&io___69);
    e_wsle();
/*<       write(*,'(''  Energy   beta      gamma           brho       '')') >*/
    s_wsfe(&io___70);
    e_wsfe();
/*<       write(*,'(''------------------------------------------------'')') >*/
    s_wsfe(&io___71);
    e_wsfe();
/*<       write(*,'(f8.3,''    '',F5.3,''  '' ,e10.3,''     '',f7.4,''  (before)'')')ti,beta1,gamma1,brho1 >*/
    s_wsfe(&io___72);
    do_fio(&c__1, (char *)&ti, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&beta1, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&gamma1, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&brho1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(*,'(f8.3,''    '',F5.3,''  '' ,e10.3,''     '',f7.4,''  (after)'')')tf,beta2,gamma2,brho2 >*/
    s_wsfe(&io___73);
    do_fio(&c__1, (char *)&tf, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&beta2, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&gamma2, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&brho2, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<        >*/
    s_wsfe(&io___74);
    d__1 = tf / ti;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    d__2 = beta2 / beta1;
    do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
    d__3 = gamma2 / gamma1;
    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
    d__4 = brho2 / brho1;
    do_fio(&c__1, (char *)&d__4, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<        >*/
    s_wsfe(&io___75);
    d__1 = (tf * 100.f - ti) / ti;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    d__2 = (beta2 - beta1) * 100.f / beta1;
    do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
    d__3 = (gamma2 - gamma1) * 100.f / gamma1;
    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
    d__4 = (brho2 - brho1) * 100.f / brho1;
    do_fio(&c__1, (char *)&d__4, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(*,*) >*/
    s_wsle(&io___76);
    e_wsle();
/*<  999  P_YES = 'Y' >*/
L999:
    *(unsigned char *)p_yes__ = 'Y';
/*<       CALL PYES('another calculation (y)',P_YES) >*/
    pyes_("another calculation (y)", p_yes__, (ftnlen)23, (ftnlen)1);
/*<       TI = TF >*/
    ti = tf;
/*<       IF( P_YES .EQ. 'Y' ) GOTO 1 >*/
    if (*(unsigned char *)p_yes__ == 'Y') {
	goto L1;
    }
/*<       END >*/
    return 0;
} /* MAIN__ */

/*  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*<       DOUBLE PRECISION FUNCTION DXDE(E_OR_RG) >*/
doublereal dxde_(doublereal *e_or_rg__)
{
    /* Initialized data */

    static doublereal delta_e__ = .01;
    static doublereal delta_rg__ = 3.;

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal ret_val=0, d__1;

    /* Builtin functions */
    integer s_rnge(char *, integer, char *, integer);

    /* Local variables */
    static integer i__;
    static doublereal e1, e2, rg1, rg2, dedx[5];
    extern /* Subroutine */ int range_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, integer *);
    static doublereal ratio1, ratio2;
    extern doublereal rint1d_(doublereal *, doublereal *, doublereal *, 
	    integer *);
    extern /* Subroutine */ int energy_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    ;


/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION E_OR_RG, RG1, RG2, DEDX(5), RATIO1, RATIO2, E1, E2 >*/
/*<       DOUBLE PRECISION RINT1D, DELTA_E, DELTA_RG >*/
/*<       INTEGER I >*/

/*<       DOUBLE PRECISION AP, ZP, ATC(5), ZTC(5), RTC(5) >*/
/*<       INTEGER IDEDX, IOPT >*/
/*<       COMMON /DXDEC/ AP, ZP, ATC, ZTC, RTC, IDEDX, IOPT >*/

/*<        >*/
/*<        >*/
/*<        >*/

/*<       EXTERNAL range >*/
/*<       DATA DELTA_E/0.01/, DELTA_RG/3./ >*/
/*<       IF( IDEDX .EQ. -1 ) THEN >*/
    if (dxdec_1.idedx == -1) {
/* ***   CALCULATE DXDE */
/*<         DO I = 1, 5 >*/
	for (i__ = 1; i__ <= 5; ++i__) {
/*<           IF( RTC(I) .GT. 0. ) THEN >*/
	    if (dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "rtc", i__1, "dxde_", (ftnlen)452)] > 0.f) {
/*<             IF( IOPT .EQ. 1 ) THEN >*/
		if (dxdec_1.iopt == 1) {
/*<               CALL RANGE(AP, ZP, ATC(I), ZTC(I),E_OR_RG-DELTA_E,RG1,IOPT) >*/
		    d__1 = *e_or_rg__ - delta_e__;
		    range_(&dxdec_1.ap, &dxdec_1.zp, &dxdec_1.atc[(i__1 = i__ 
			    - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("atc", i__1,
			     "dxde_", (ftnlen)454)], &dxdec_1.ztc[(i__2 = i__ 
			    - 1) < 5 && 0 <= i__2 ? i__2 : s_rnge("ztc", i__2,
			     "dxde_", (ftnlen)454)], &d__1, &rg1, &
			    dxdec_1.iopt);
/*<               CALL RANGE(AP, ZP, ATC(I), ZTC(I),E_OR_RG+DELTA_E,RG2,IOPT) >*/
		    d__1 = *e_or_rg__ + delta_e__;
		    range_(&dxdec_1.ap, &dxdec_1.zp, &dxdec_1.atc[(i__1 = i__ 
			    - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("atc", i__1,
			     "dxde_", (ftnlen)455)], &dxdec_1.ztc[(i__2 = i__ 
			    - 1) < 5 && 0 <= i__2 ? i__2 : s_rnge("ztc", i__2,
			     "dxde_", (ftnlen)455)], &d__1, &rg2, &
			    dxdec_1.iopt);
                    DEB("rg1 %f",rg1);
                    DEB("rg2 %f",rg2);
                    DEB("en  %f",*e_or_rg__);
/*<             ELSE IF( IOPT .EQ. 2 ) THEN >*/
		} else if (dxdec_1.iopt == 2) {
/*<               IF( I .EQ. 1 ) THEN >*/
		    if (i__ == 1) {
/*<                 RG1 = RINT1D(E_OR_RG-DELTA_E, ETAB1, RGTAB1, 1000) >*/
			d__1 = *e_or_rg__ - delta_e__;
			rg1 = rint1d_(&d__1, rge_1.etab1, rge_1.rgtab1, &
				c__1000);
/*<                 RG2 = RINT1D(E_OR_RG+DELTA_E, ETAB1, RGTAB1, 1000) >*/
			d__1 = *e_or_rg__ + delta_e__;
			rg2 = rint1d_(&d__1, rge_1.etab1, rge_1.rgtab1, &
				c__1000);
/*<               ELSE IF( I .EQ. 2 ) THEN >*/
		    } else if (i__ == 2) {
/*<                 RG1 = RINT1D(E_OR_RG-DELTA_E, ETAB2, RGTAB2, 1000) >*/
			d__1 = *e_or_rg__ - delta_e__;
			rg1 = rint1d_(&d__1, rge_1.etab2, rge_1.rgtab2, &
				c__1000);
/*<                 RG2 = RINT1D(E_OR_RG+DELTA_E, ETAB2, RGTAB2, 1000) >*/
			d__1 = *e_or_rg__ + delta_e__;
			rg2 = rint1d_(&d__1, rge_1.etab2, rge_1.rgtab2, &
				c__1000);
/*<               ELSE IF( I .EQ. 3 ) THEN >*/
		    } else if (i__ == 3) {
/*<                 RG1 = RINT1D(E_OR_RG-DELTA_E, ETAB3, RGTAB3, 1000) >*/
			d__1 = *e_or_rg__ - delta_e__;
			rg1 = rint1d_(&d__1, rge_1.etab3, rge_1.rgtab3, &
				c__1000);
/*<                 RG2 = RINT1D(E_OR_RG+DELTA_E, ETAB3, RGTAB3, 1000) >*/
			d__1 = *e_or_rg__ + delta_e__;
			rg2 = rint1d_(&d__1, rge_1.etab3, rge_1.rgtab3, &
				c__1000);
/*<               ELSE IF( I .EQ. 4 ) THEN >*/
		    } else if (i__ == 4) {
/*<                 RG1 = RINT1D(E_OR_RG-DELTA_E, ETAB4, RGTAB4, 1000) >*/
			d__1 = *e_or_rg__ - delta_e__;
			rg1 = rint1d_(&d__1, rge_1.etab4, rge_1.rgtab4, &
				c__1000);
/*<                 RG2 = RINT1D(E_OR_RG+DELTA_E, ETAB4, RGTAB4, 1000) >*/
			d__1 = *e_or_rg__ + delta_e__;
			rg2 = rint1d_(&d__1, rge_1.etab4, rge_1.rgtab4, &
				c__1000);
/*<               ELSE IF( I .EQ. 5 ) THEN >*/
		    } else if (i__ == 5) {
/*<                 RG1 = RINT1D(E_OR_RG-DELTA_E, ETAB5, RGTAB5, 1000) >*/
			d__1 = *e_or_rg__ - delta_e__;
			rg1 = rint1d_(&d__1, rge_1.etab5, rge_1.rgtab5, &
				c__1000);
/*<                 RG2 = RINT1D(E_OR_RG+DELTA_E, ETAB5, RGTAB5, 1000) >*/
			d__1 = *e_or_rg__ + delta_e__;
			rg2 = rint1d_(&d__1, rge_1.etab5, rge_1.rgtab5, &
				c__1000);
/*<               ENDIF >*/
		    }
/*<             ENDIF >*/
		}
/*<             DEDX(I) =  2. * DELTA_E / (RG2 - RG1) >*/
		dedx[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("dedx",
			 i__1, "dxde_", (ftnlen)474)] = delta_e__ * 2.f / (
			rg2 - rg1);
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<         RATIO1 = 0. >*/
	ratio1 = 0.f;
/*<         RATIO2 = 0. >*/
	ratio2 = 0.f;
/*<         DO I = 1, 5 >*/
	for (i__ = 1; i__ <= 5; ++i__) {
/*<           IF( RTC(I) .GT. 0. ) THEN >*/
	    if (dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "rtc", i__1, "dxde_", (ftnlen)480)] > 0.f) {
/*<             RATIO1 = RATIO1 + RTC(I) * ATC(I) >*/
		ratio1 += dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? 
			i__1 : s_rnge("rtc", i__1, "dxde_", (ftnlen)481)] * 
			dxdec_1.atc[(i__2 = i__ - 1) < 5 && 0 <= i__2 ? i__2 :
			 s_rnge("atc", i__2, "dxde_", (ftnlen)481)];
/*<             RATIO2 = RATIO2 + RTC(I) * ATC(I) * DEDX(I) >*/
		ratio2 += dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? 
			i__1 : s_rnge("rtc", i__1, "dxde_", (ftnlen)482)] * 
			dxdec_1.atc[(i__2 = i__ - 1) < 5 && 0 <= i__2 ? i__2 :
			 s_rnge("atc", i__2, "dxde_", (ftnlen)482)] * dedx[(
			i__3 = i__ - 1) < 5 && 0 <= i__3 ? i__3 : s_rnge(
			"dedx", i__3, "dxde_", (ftnlen)482)];
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<         DXDE = RATIO1 / RATIO2 >*/
	ret_val = ratio1 / ratio2;
/*        WRITE(6,*) 'DXDE1 = ', DXDE, E_OR_RG-DELTA_E, */
/*     #                               E_OR_RG+DELTA_E, RG1, RG2 */
/*<       ELSE IF ( IDEDX .EQ. 1 ) THEN >*/
    } else if (dxdec_1.idedx == 1) {
/* ***   CALCULATE DEDX */
/*<         DO I = 1, 5 >*/
	for (i__ = 1; i__ <= 5; ++i__) {
/*<           IF( RTC(I) .GT. 0. ) THEN >*/
	    if (dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "rtc", i__1, "dxde_", (ftnlen)491)] > 0.f) {
/*<             IF( IOPT .EQ. 1 ) THEN >*/
		if (dxdec_1.iopt == 1) {
/*<        >*/
		    d__1 = *e_or_rg__ - delta_rg__ / 100.f;
		    energy_(&dxdec_1.ap, &dxdec_1.zp, &dxdec_1.atc[(i__1 = 
			    i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("atc", 
			    i__1, "dxde_", (ftnlen)493)], &dxdec_1.ztc[(i__2 =
			     i__ - 1) < 5 && 0 <= i__2 ? i__2 : s_rnge("ztc", 
			    i__2, "dxde_", (ftnlen)493)], &d__1, &e1, &
			    dxdec_1.iopt);
/*<        >*/
		    d__1 = *e_or_rg__ + delta_rg__ / 100.f;
		    energy_(&dxdec_1.ap, &dxdec_1.zp, &dxdec_1.atc[(i__1 = 
			    i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("atc", 
			    i__1, "dxde_", (ftnlen)495)], &dxdec_1.ztc[(i__2 =
			     i__ - 1) < 5 && 0 <= i__2 ? i__2 : s_rnge("ztc", 
			    i__2, "dxde_", (ftnlen)495)], &d__1, &e2, &
			    dxdec_1.iopt);
/*<             ELSE IF( IOPT .EQ. 2 ) THEN >*/
		} else if (dxdec_1.iopt == 2) {
/*<               IF( I .EQ. 1 ) THEN >*/
		    if (i__ == 1) {
/*<                 E1 = RINT1D(E_OR_RG-DELTA_RG, RGTAB1, ETAB1, 1000) >*/
			d__1 = *e_or_rg__ - delta_rg__;
			e1 = rint1d_(&d__1, rge_1.rgtab1, rge_1.etab1, &
				c__1000);
/*<                 E2 = RINT1D(E_OR_RG+DELTA_RG, RGTAB1, ETAB1, 1000) >*/
			d__1 = *e_or_rg__ + delta_rg__;
			e2 = rint1d_(&d__1, rge_1.rgtab1, rge_1.etab1, &
				c__1000);
/*<               ELSE IF( I .EQ. 2 ) THEN >*/
		    } else if (i__ == 2) {
/*<                 E1 = RINT1D(E_OR_RG-DELTA_RG, RGTAB2, ETAB2, 1000) >*/
			d__1 = *e_or_rg__ - delta_rg__;
			e1 = rint1d_(&d__1, rge_1.rgtab2, rge_1.etab2, &
				c__1000);
/*<                 E2 = RINT1D(E_OR_RG+DELTA_RG, RGTAB2, ETAB2, 1000) >*/
			d__1 = *e_or_rg__ + delta_rg__;
			e2 = rint1d_(&d__1, rge_1.rgtab2, rge_1.etab2, &
				c__1000);
/*<               ELSE IF( I .EQ. 3 ) THEN >*/
		    } else if (i__ == 3) {
/*<                 E1 = RINT1D(E_OR_RG-DELTA_RG, RGTAB3, ETAB3, 1000) >*/
			d__1 = *e_or_rg__ - delta_rg__;
			e1 = rint1d_(&d__1, rge_1.rgtab3, rge_1.etab3, &
				c__1000);
/*<                 E2 = RINT1D(E_OR_RG+DELTA_RG, RGTAB3, ETAB3, 1000) >*/
			d__1 = *e_or_rg__ + delta_rg__;
			e2 = rint1d_(&d__1, rge_1.rgtab3, rge_1.etab3, &
				c__1000);
/*<               ELSE IF( I .EQ. 4 ) THEN >*/
		    } else if (i__ == 4) {
/*<                 E1 = RINT1D(E_OR_RG-DELTA_RG, RGTAB4, ETAB4, 1000) >*/
			d__1 = *e_or_rg__ - delta_rg__;
			e1 = rint1d_(&d__1, rge_1.rgtab4, rge_1.etab4, &
				c__1000);
/*<                 E2 = RINT1D(E_OR_RG+DELTA_RG, RGTAB4, ETAB4, 1000) >*/
			d__1 = *e_or_rg__ + delta_rg__;
			e2 = rint1d_(&d__1, rge_1.rgtab4, rge_1.etab4, &
				c__1000);
/*<               ELSE IF( I .EQ. 5 ) THEN >*/
		    } else if (i__ == 5) {
/*<                 E1 = RINT1D(E_OR_RG-DELTA_RG, RGTAB5, ETAB5, 1000) >*/
			d__1 = *e_or_rg__ - delta_rg__;
			e1 = rint1d_(&d__1, rge_1.rgtab5, rge_1.etab5, &
				c__1000);
/*<                 E2 = RINT1D(E_OR_RG+DELTA_RG, RGTAB5, ETAB5, 1000) >*/
			d__1 = *e_or_rg__ + delta_rg__;
			e2 = rint1d_(&d__1, rge_1.rgtab5, rge_1.etab5, &
				c__1000);
/*<               ENDIF >*/
		    }
/*<             ENDIF >*/
		}
/*<             DEDX(I) = (E2 - E1) / DELTA_RG / 2. >*/
		dedx[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge("dedx",
			 i__1, "dxde_", (ftnlen)515)] = (e2 - e1) / 
			delta_rg__ / 2.f;
/*<             IF( IOPT .EQ. 1 ) DEDX(I) = DEDX(I) * 100. >*/
		if (dxdec_1.iopt == 1) {
		    dedx[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
			    "dedx", i__1, "dxde_", (ftnlen)516)] = dedx[(i__2 
			    = i__ - 1) < 5 && 0 <= i__2 ? i__2 : s_rnge("dedx"
			    , i__2, "dxde_", (ftnlen)516)] * 100.f;
		}
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<         RATIO1 = 0. >*/
	ratio1 = 0.f;
/*<         RATIO2 = 0. >*/
	ratio2 = 0.f;
/*<         DO I = 1, 5 >*/
	for (i__ = 1; i__ <= 5; ++i__) {
/*<           IF( RTC(I) .GT. 0. ) THEN >*/
	    if (dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? i__1 : s_rnge(
		    "rtc", i__1, "dxde_", (ftnlen)522)] > 0.f) {
/*<             RATIO1 = RATIO1 + RTC(I) * ATC(I) * DEDX(I) >*/
		ratio1 += dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? 
			i__1 : s_rnge("rtc", i__1, "dxde_", (ftnlen)523)] * 
			dxdec_1.atc[(i__2 = i__ - 1) < 5 && 0 <= i__2 ? i__2 :
			 s_rnge("atc", i__2, "dxde_", (ftnlen)523)] * dedx[(
			i__3 = i__ - 1) < 5 && 0 <= i__3 ? i__3 : s_rnge(
			"dedx", i__3, "dxde_", (ftnlen)523)];
/*<             RATIO2 = RATIO2 + RTC(I) * ATC(I) >*/
		ratio2 += dxdec_1.rtc[(i__1 = i__ - 1) < 5 && 0 <= i__1 ? 
			i__1 : s_rnge("rtc", i__1, "dxde_", (ftnlen)524)] * 
			dxdec_1.atc[(i__2 = i__ - 1) < 5 && 0 <= i__2 ? i__2 :
			 s_rnge("atc", i__2, "dxde_", (ftnlen)524)];
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<         DXDE = RATIO1 / RATIO2 >*/
	ret_val = ratio1 / ratio2;
/*       WRITE(6,*) 'DXDE2 = ', DXDE, DEDX(1), E_OR_RG-DELTA_RG, */
/*    #                         E_OR_RG+DELTA_RG, E1, E2 */
/*<       ENDIF >*/
    }
/*<       RETURN >*/
    return ret_val;
/*<       END >*/
} /* dxde_ */

/* ********************************************************************** */
/*<       SUBROUTINE RANGE(A, Z, AT, ZT, T, RG, IENER) >*/
/* Subroutine */ int range_(doublereal *a, doublereal *z__, doublereal *at, 
	doublereal *zt, doublereal *t, doublereal *rg, integer *iener)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), d_lg10(doublereal *);

    /* Local variables */
    static doublereal f[10], y1, y2, y3, y4, y5, etab[1000], fcor;
    extern /* Subroutine */ int fdata_(doublereal *, doublereal *, doublereal 
	    *, doublereal *);
    static doublereal coeff, rgtab[1000];
    extern /* Subroutine */ int fcorr_(doublereal *, doublereal *);
    extern doublereal rint1d_(doublereal *, doublereal *, doublereal *, 
	    integer *);
    static doublereal coeffi;
    extern /* Subroutine */ int dedxhu_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);


/*     ///////////////// */
/*     CALCULATES RANGES depending on option IOPT */
/*     ///////////////// */

/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION Y1,Y2,Y3,Y4,Y5,A,Z,AT,ZT,T,RG >*/
/*<       DOUBLE PRECISION COEFF,COEFFI,FCOR >*/
/*<       DOUBLE PRECISION F(10),RGTAB(1000),ETAB(1000),RINT1D >*/
/*<       INTEGER IENER >*/

/*<       IF( T .LE. 1.0E-6 ) THEN >*/
    if (*t <= 1e-6f) {
/*<         RG = 0. >*/
	*rg = 0.f;
/*<         GOTO 999 >*/
	goto L999;
/*<       ENDIF >*/
    }

/*<       IF( IENER .EQ. 2 ) GOTO 500 >*/
    if (*iener == 2) {
	goto L500;
    }

/*<       CALL FDATA( ZT, F, COEFFI, COEFF) >*/
    fdata_(zt, f, &coeffi, &coeff);

/*     ZUSAMMEN FASSUNG DER FITPARAMETER */
/*<       Y1 = 1.0+F(1)*Z+F(2)*Z**2+F(3)*Z**3+F(4)*Z**4 >*/
/* Computing 2nd power */
    d__1 = *z__;
/* Computing 3rd power */
    d__2 = *z__;
/* Computing 4th power */
    d__3 = *z__, d__3 *= d__3;
    y1 = f[0] * *z__ + 1.f + f[1] * (d__1 * d__1) + f[2] * (d__2 * (d__2 * 
	    d__2)) + f[3] * (d__3 * d__3);
/*<       Y2 = F(5)+F(6)*Z >*/
    y2 = f[4] + f[5] * *z__;
/*<       Y3 = F(7)+F(8)*Z >*/
    y3 = f[6] + f[7] * *z__;
/*<       Y4 = F(9)+F(10)*Z >*/
    y4 = f[8] + f[9] * *z__;
/*<       Y5 = Y3/(2.0*Y4) >*/
    y5 = y3 / (y4 * 2.f);

/*     MATERIEKOEFFIZIENT */
/*<       CALL FCORR(Z, FCOR) >*/
    fcorr_(z__, &fcor);
/*<       COEFF = (AT*COEFFI)/ZT**COEFF >*/
    coeff = *at * coeffi / pow_dd(zt, &coeff);

/*     REICHWEITE */
/*<        >*/
/* Computing 2nd power */
    d__1 = *z__;
/* Computing 2nd power */
    d__3 = d_lg10(t);
    d__2 = y1 * (y2 + y3 * d_lg10(t) + y4 * (d__3 * d__3));
    *rg = *a / (d__1 * d__1) * pow_dd(&c_b268, &d__2) * coeff * fcor;
/*<       GOTO 900 >*/
    goto L900;

/*<  500  CONTINUE >*/
L500:
/*<       CALL DEDXHU(Z, A, ZT, 120.d0, ETAB, RGTAB ) >*/
    dedxhu_(z__, a, zt, &c_b104, etab, rgtab);
/*<       RG = RINT1D( T, ETAB, RGTAB, 1000) >*/
    *rg = rint1d_(t, etab, rgtab, &c__1000);

/*<  900  CONTINUE >*/
L900:
/*<       IF( RG .LT. 0. ) RG = 0. >*/
    if (*rg < 0.f) {
	*rg = 0.f;
    }
/*<  999  RETURN >*/
L999:
    return 0;
/*<       END >*/
} /* range_ */

/*  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
/*<       SUBROUTINE ENERGY(A, Z, AT, ZT, RG, T, IENER) >*/
/* Subroutine */ int energy_(doublereal *a, doublereal *z__, doublereal *at, 
	doublereal *zt, doublereal *rg, doublereal *t, integer *iener)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), d_lg10(doublereal *), sqrt(
	    doublereal);

    /* Local variables */
    static doublereal f[10], y1, y2, y3, y4, y5, etab[1000], fcor;
    extern /* Subroutine */ int fdata_(doublereal *, doublereal *, doublereal 
	    *, doublereal *);
    static doublereal coeff, rgtab[1000];
    extern /* Subroutine */ int fcorr_(doublereal *, doublereal *);
    extern doublereal rint1d_(doublereal *, doublereal *, doublereal *, 
	    integer *);
    static doublereal coeffi;
    extern /* Subroutine */ int dedxhu_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);


/*     /////////////////////////////////////////// */
/*     CALCULATES ENERGIES FROM RANGES depending on option IOPT */
/*     /////////////////////////////////////////// */

/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION Y1,Y2,Y3,Y4,Y5,A,Z,AT,ZT,T,RG >*/
/*<       DOUBLE PRECISION COEFFI,COEFF,FCOR,RINT1D >*/
/*<       DOUBLE PRECISION F(10), ETAB(1000), RGTAB(1000) >*/
/*<       INTEGER IENER >*/

/*<       IF( RG .LE. 1.0E-9 ) THEN >*/
    if (*rg <= 1e-9f) {
/*<         T = 0. >*/
	*t = 0.f;
/*<         GOTO 999 >*/
	goto L999;
/*<       ENDIF >*/
    }

/*<       IF( IENER .EQ. 2 ) GOTO 500 >*/
    if (*iener == 2) {
	goto L500;
    }

/*<       CALL FDATA( ZT, F, COEFFI, COEFF) >*/
    fdata_(zt, f, &coeffi, &coeff);

/*     ZUSAMMEN FASSUNG DER FITPARAMETER */
/*<       Y1 = 1.0+F(1)*Z+F(2)*Z**2+F(3)*Z**3+F(4)*Z**4 >*/
/* Computing 2nd power */
    d__1 = *z__;
/* Computing 3rd power */
    d__2 = *z__;
/* Computing 4th power */
    d__3 = *z__, d__3 *= d__3;
    y1 = f[0] * *z__ + 1.f + f[1] * (d__1 * d__1) + f[2] * (d__2 * (d__2 * 
	    d__2)) + f[3] * (d__3 * d__3);
/*<       Y2 = F(5)+F(6)*Z >*/
    y2 = f[4] + f[5] * *z__;
/*<       Y3 = F(7)+F(8)*Z >*/
    y3 = f[6] + f[7] * *z__;
/*<       Y4 = F(9)+F(10)*Z >*/
    y4 = f[8] + f[9] * *z__;
/*<       Y5 = Y3/(2.0*Y4) >*/
    y5 = y3 / (y4 * 2.f);

/*     MATERIEKOEFFIZIENT */
/*<       COEFF = (AT*COEFFI)/ZT**COEFF >*/
    coeff = *at * coeffi / pow_dd(zt, &coeff);
/*     ENERGY */
/*<       CALL FCORR(Z, FCOR) >*/
    fcorr_(z__, &fcor);
/*<        >*/
/* Computing 2nd power */
    d__2 = y5;
/* Computing 2nd power */
    d__4 = *z__;
    d__3 = *rg / fcor / coeff * (d__4 * d__4) / *a;
    d__1 = -y5 - sqrt(d__2 * d__2 - y2 / y4 + d_lg10(&d__3) / (y1 * y4));
    *t = pow_dd(&c_b268, &d__1);
/*<       GOTO 900 >*/
    goto L900;

/*<  500  CONTINUE >*/
L500:
/*<       CALL DEDXHU(Z, A, ZT, 120.d0, ETAB, RGTAB ) >*/
    dedxhu_(z__, a, zt, &c_b104, etab, rgtab);
/*<       T = RINT1D( RG, RGTAB, ETAB, 1000) >*/
    *t = rint1d_(rg, rgtab, etab, &c__1000);

/*<  900  CONTINUE >*/
L900:
/*<       IF( T .LT. 0. ) T = 0. >*/
    if (*t < 0.f) {
	*t = 0.f;
    }

/*<  999  RETURN >*/
L999:
    return 0;
/*<       END >*/
} /* energy_ */

/* *********************************************************************** */

/*<       SUBROUTINE FDATA(Z,F,COEFFI,COEFF) >*/
/* Subroutine */ int fdata_(doublereal *z__, doublereal *f, doublereal *
	coeffi, doublereal *coeff)
{
    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);


/*   ____________________________________________________________________ */
/*  / */
/*  /  VARIABLES RELATIVES A LA MATIERE POUR LE CALCUL DES PERTES */
/*  /                                          D'ENERGIE DANS LA MATIERE. */
/*  / */
/*  /____________________________________________________________________ */

/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION Z,F,COEFFI, COEFF >*/
/*<       DIMENSION F(1:10) >*/

/*<       IF (Z.LE.5.0) THEN >*/
    if (*z__ <= 5.f) {
/*<         F(1) = -0.128482E-03 >*/
	f[0] = -1.28482e-4f;
/*<         F(2) = -0.173612E-05 >*/
	f[1] = -1.73612e-6f;
/*<         F(3) = 0.889892E-07 >*/
	f[2] = 8.89892e-8f;
/*<         F(4) = -0.705115E-09 >*/
	f[3] = -7.05115e-10f;
/*<         F(5) = -0.553492E+00 >*/
	f[4] = -.553492f;
/*<         F(6) = 0.912049E-02 >*/
	f[5] = .00912049f;
/*<         F(7) = 0.268184E+01 >*/
	f[6] = 2.68184f;
/*<         F(8) = -0.529303E-02 >*/
	f[7] = -.00529303f;
/*<         F(9) = -0.210108E+00 >*/
	f[8] = -.210108f;
/*<         F(10) = 0.774360E-03 >*/
	f[9] = 7.7436e-4f;
/*<         COEFFI = 4.0**0.98/9.012 >*/
	*coeffi = pow_dd(&c_b280, &c_b281) / 9.012f;
/*<         COEFF  = 0.98 >*/
	*coeff = .98f;
/*<       END IF >*/
    }
/*<       IF ((Z.GT.5.0).AND.(Z.LE.9.0)) THEN >*/
    if (*z__ > 5.f && *z__ <= 9.f) {
/*<         F(1) = 0.667801E-03 >*/
	f[0] = 6.67801e-4f;
/*<         F(2) = -0.392137E-05 >*/
	f[1] = -3.92137e-6f;
/*<         F(3) = 0.136917E-06 >*/
	f[2] = 1.36917e-7f;
/*<         F(4) = -0.972996E-09 >*/
	f[3] = -9.72996e-10f;
/*<         F(5) = -0.490202E+00 >*/
	f[4] = -.490202f;
/*<         F(6) = 0.751599E-02 >*/
	f[5] = .00751599f;
/*<         F(7) = 0.261390E+01 >*/
	f[6] = 2.6139f;
/*<         F(8) = -0.600822E-02 >*/
	f[7] = -.00600822f;
/*<         F(9) = -0.199549E+00 >*/
	f[8] = -.199549f;
/*<         F(10) = 0.731880E-03 >*/
	f[9] = 7.3188e-4f;
/*<         COEFFI = 6.0**0.98/12.011 >*/
	*coeffi = pow_dd(&c_b282, &c_b281) / 12.011f;
/*<         COEFF  = 0.98 >*/
	*coeff = .98f;
/*<       END IF >*/
    }
/*<       IF ((Z.GT.9.0).AND.(Z.LE.32.0)) THEN >*/
    if (*z__ > 9.f && *z__ <= 32.f) {
/*<         F(1) = -0.668659E-04 >*/
	f[0] = -6.68659e-5f;
/*<         F(2) = -0.185311E-05 >*/
	f[1] = -1.85311e-6f;
/*<         F(3) = 0.873192E-07 >*/
	f[2] = 8.73192e-8f;
/*<         F(4) = -0.690141E-09 >*/
	f[3] = -6.90141e-10f;
/*<         F(5) = -0.530758E+00 >*/
	f[4] = -.530758f;
/*<         F(6) = 0.898953E-02 >*/
	f[5] = .00898953f;
/*<         F(7) = 0.268916E+01 >*/
	f[6] = 2.68916f;
/*<         F(8) = -0.533772E-02 >*/
	f[7] = -.00533772f;
/*<         F(9) = -0.214131E+00 >*/
	f[8] = -.214131f;
/*<         F(10) = 0.773008E-03 >*/
	f[9] = 7.73008e-4f;
/*<         COEFFI = 13.0**0.90/26.982 >*/
	*coeffi = pow_dd(&c_b284, &c_b285) / 26.982f;
/*<         COEFF  = 0.90 >*/
	*coeff = .9f;
/*<       END IF >*/
    }
/*<       IF ((Z.GT.32.).AND.(Z.LE.64.0)) THEN >*/
    if (*z__ > 32.f && *z__ <= 64.f) {
/*<         F(1) =  1.23639E-03 >*/
	f[0] = .00123639f;
/*<         F(2) = -6.13893E-06 >*/
	f[1] = -6.13893e-6f;
/*<         F(3) = 1.84116E-07 >*/
	f[2] = 1.84116e-7f;
/*<         F(4) = -1.20551E-09 >*/
	f[3] = -1.20551e-9f;
/*<         F(5) = -0.263421E+00 >*/
	f[4] = -.263421f;
/*<         F(6) = 6.34349E-03 >*/
	f[5] = .00634349f;
/*<         F(7) = 2.61081E+00 >*/
	f[6] = 2.61081f;
/*<         F(8) = -6.38315E-03 >*/
	f[7] = -.00638315f;
/*<         F(9) = -0.204813E+00 >*/
	f[8] = -.204813f;
/*<         F(10) = 6.63267E-04 >*/
	f[9] = 6.63267e-4f;
/*<         COEFFI = 50.0**0.88/118.69 >*/
	*coeffi = pow_dd(&c_b286, &c_b287) / 118.69f;
/*<         COEFF  = 0.88 >*/
	*coeff = .88f;
/*<       END IF >*/
    }
/*<       IF ((Z.GT.64.0).AND.(Z.LE.76.0)) THEN >*/
    if (*z__ > 64.f && *z__ <= 76.f) {
/*<         F(1) = 0.199249E-04 >*/
	f[0] = 1.99249e-5f;
/*<         F(2) = -0.227944E-05 >*/
	f[1] = -2.27944e-6f;
/*<         F(3) = 0.105063E-06 >*/
	f[2] = 1.05063e-7f;
/*<         F(4) = -0.829122E-09 >*/
	f[3] = -8.29122e-10f;
/*<         F(5) = -0.325062E+00 >*/
	f[4] = -.325062f;
/*<         F(6) = 0.975017E-02 >*/
	f[5] = .00975017f;
/*<         F(7) = 0.268814E+01 >*/
	f[6] = 2.68814f;
/*<         F(8) = -0.607419E-02 >*/
	f[7] = -.00607419f;
/*<         F(9) = -0.218986E+00 >*/
	f[8] = -.218986f;
/*<         F(10) = 0.869283E-03 >*/
	f[9] = 8.69283e-4f;
/*<         COEFFI = 73.0**0.88/180.95 >*/
	*coeffi = pow_dd(&c_b288, &c_b287) / 180.95f;
/*<         COEFF  = 0.88 >*/
	*coeff = .88f;
/*<       END IF >*/
    }
/*<       IF (Z.GT.76.0) THEN >*/
    if (*z__ > 76.f) {
/*<         F(1) = -0.375861E-03 >*/
	f[0] = -3.75861e-4f;
/*<         F(2) = -0.373902E-05 >*/
	f[1] = -3.73902e-6f;
/*<         F(3) = 0.148861E-06 >*/
	f[2] = 1.48861e-7f;
/*<         F(4) = -0.112159E-08 >*/
	f[3] = -1.12159e-9f;
/*<         F(5) = -0.166220E+00 >*/
	f[4] = -.16622f;
/*<         F(6) = 0.126920E-01 >*/
	f[5] = .012692f;
/*<         F(7) = 0.259061E+01 >*/
	f[6] = 2.59061f;
/*<         F(8) = -0.725322E-02 >*/
	f[7] = -.00725322f;
/*<         F(9) = -0.202004E+00 >*/
	f[8] = -.202004f;
/*<         F(10) = 0.117942E-02 >*/
	f[9] = .00117942f;
/*<         COEFFI = 82.0**0.80/207.2 >*/
	*coeffi = pow_dd(&c_b290, &c_b291) / 207.2f;
/*<         COEFF  = 0.80 >*/
	*coeff = .8f;
/*<       END IF >*/
    }

/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* fdata_ */


/* ********************************************************************** */

/*     REFERENCE: F. Hubert et al., At. Data Nucl. Data Tables 46, 1 (199 */

/*<       SUBROUTINE DEDXHU(RZP, RAP, RZT, TI, ETAB, RF ) >*/
/* Subroutine */ int dedxhu_(doublereal *rzp, doublereal *rap, doublereal *
	rzt, doublereal *ti, doublereal *etab, doublereal *rf)
{
    /* Initialized data */

    static doublereal convdata[92] = { 597.4,150.5,86.78,66.82,55.7,50.14,
	    42.99,37.64,31.7,29.84,26.2,24.77,22.32,21.44,19.44,18.78,16.99,
	    15.07,15.4,15.02,13.4,12.57,11.82,11.58,10.96,10.78,10.22,10.26,
	    9.477,9.212,8.637,8.296,8.038,7.627,7.536,7.186,7.046,6.873,6.774,
	    6.602,6.482,6.277,6.083,5.958,5.852,5.66,5.583,5.358,5.245,5.074,
	    4.946,4.719,4.745,4.586,4.531,4.385,4.335,4.298,4.274,4.175,4.097,
	    4.005,3.963,3.83,3.789,3.706,3.651,3.6,3.565,3.48,3.442,3.374,
	    3.328,3.275,3.234,3.166,3.133,3.087,3.057,3.002,2.947,2.907,2.882,
	    2.868,2.868,2.713,2.7,2.665,2.653,2.596,2.607,2.53 };
    static doublereal adata[368]	/* was [4][92] */ = { 2.371,.5462,
	    -.07932,-.006853,2.809,.4847,-.08756,-.007281,3.095,.4434,-.09259,
	    -.007459,3.28,.4188,-.09564,-.007604,3.426,.4,-.09796,-.007715,
	    3.588,.3921,-.09935,-.007804,3.759,.4094,-.09646,-.007661,3.782,
	    .3734,-.1011,-.007874,3.816,.3504,-.1046,-.008074,3.863,.3342,
	    -.1072,-.008231,3.898,.3191,-.1086,-.008271,3.961,.314,-.1091,
	    -.008297,4.024,.3113,-.1093,-.008306,4.077,.3074,-.1089,-.008219,
	    4.124,.3023,-.1094,-.00824,4.164,.2964,-.1101,-.008267,4.21,.2936,
	    -.1103,-.00827,4.261,.2994,-.1085,-.008145,4.3,.2903,-.1103,
	    -.008259,4.344,.2897,-.1102,-.008245,4.327,.2707,-.1127,-.00837,
	    4.34,.2618,-.1138,-.00842,4.361,.2559,-.1145,-.008447,4.349,.24,
	    -.1166,-.00855,4.362,.2327,-.1174,-.008588,4.375,.2253,-.1185,
	    -.008648,4.362,.2069,-.1214,-.008815,4.346,.1857,-.1249,-.009021,
	    4.355,.18,-.1255,-.009045,4.389,.1806,-.1253,-.009028,4.407,.1759,
	    -.1258,-.009054,4.419,.1694,-.1267,-.009094,4.412,.1545,-.1289,
	    -.009202,4.419,.1448,-.1303,-.009269,4.436,.1443,-.1299,-.009229,
	    4.478,.1608,-.1262,-.008983,4.489,.1517,-.1278,-.009078,4.514,
	    .1551,-.1268,-.009005,4.533,.1568,-.1261,-.008945,4.548,.1572,
	    -.1256,-.008901,4.553,.1544,-.1255,-.008883,4.548,.1485,-.1259,
	    -.008889,4.489,.1128,-.1309,-.009107,4.402,.06656,-.1375,-.009421,
	    4.292,.01012,-.1459,-.009835,4.187,-.04539,-.1542,-.01025,4.577,
	    .13,-.1285,-.009067,4.583,.1253,-.1291,-.009084,4.58,.1174,-.1301,
	    -.009129,4.581,.111,-.1309,-.009161,4.582,.1046,-.1317,-.009193,
	    4.6,.1052,-.1315,-.009178,4.614,.1043,-.1315,-.009175,4.619,
	    .09769,-.1325,-.009231,4.671,.1136,-.1298,-.009078,4.706,.1206,
	    -.1287,-.009009,4.732,.1244,-.128,-.008968,4.722,.1156,-.1292,
	    -.00903,4.71,.106,-.1305,-.0091,4.698,.09647,-.1319,-.009169,
	    4.681,.08536,-.1335,-.009252,4.676,.07819,-.1345,-.009302,4.663,
	    .06867,-.1358,-.009373,4.676,.06861,-.1357,-.009363,4.649,.05362,
	    -.1379,-.00948,4.634,.04335,-.1394,-.009558,4.603,.02679,-.1418,
	    -.00969,4.584,.01494,-.1436,-.009783,4.576,.007043,-.1447,
	    -.009841,4.571,7.05e-4,-.1456,-.009886,4.566,-.005626,-.1464,
	    -.00993,4.561,-.01197,-.1473,-.009975,4.572,-.012,-.1472,-.009965,
	    4.569,-.01755,-.148,-.01,4.573,-.01992,-.1482,-.01001,4.57,
	    -.02547,-.149,-.01005,4.528,-.04613,-.1521,-.01022,4.494,-.0637,
	    -.1548,-.01037,4.564,-.027,-.1471,-.009852,4.546,-.04963,-.1523,
	    -.01022,4.594,-.03339,-.1496,-.01006,4.608,-.02886,-.1485,-.00999,
	    4.624,-.02639,-.1481,-.009971,4.636,-.02422,-.1477,-.009939,4.648,
	    -.02172,-.1471,-.009903,4.662,-.1192,-.1752,-.01196,4.69,-.009867,
	    -.1449,-.009771,4.715,-.002113,-.1435,-.009689,4.729,.001392,
	    -.1428,-.009644,4.729,-5.98e-4,-.143,-.009647,4.738,.001075,
	    -.1425,-.009618,4.751,.004244,-.1419,-.009576 };
    static doublereal acodata[92] = { 2.62277201e-4,2.6151631e-4,
	    2.58906279e-4,2.55371677e-4,2.51836842e-4,2.4922681e-4,
	    2.4812296e-4,2.47734599e-4,2.46927608e-4,2.44568335e-4,
	    2.39522924e-4,2.30656995e-4,2.2451453e-4,2.42256137e-4,
	    2.57341191e-4,2.63125403e-4,2.61961482e-4,2.56202882e-4,
	    2.48202356e-4,2.40313093e-4,2.34977852e-4,2.34999228e-4,
	    2.41120317e-4,2.45487085e-4,2.40917449e-4,2.3151451e-4,
	    2.23492694e-4,2.2022707e-4,2.21771479e-4,2.20454385e-4,
	    2.16872373e-4,2.14341839e-4,2.15205611e-4,2.17912384e-4,
	    2.20451737e-4,2.22871211e-4,2.25732758e-4,2.29598416e-4,
	    2.34238614e-4,2.36257809e-4,2.33500963e-4,2.31941594e-4,
	    2.36854408e-4,2.42590701e-4,2.40770867e-4,2.26940087e-4,
	    2.12342944e-4,2.08530197e-4,2.12578176e-4,2.17944544e-4,
	    2.19789523e-4,2.20083501e-4,2.21692957e-4,2.24259042e-4,
	    2.26616496e-4,2.27600089e-4,2.26199292e-4,2.22575574e-4,
	    2.19257228e-4,2.18489397e-4,2.19172711e-4,2.19371621e-4,
	    2.17628709e-4,2.14399028e-4,2.10608094e-4,2.07150923e-4,
	    2.04914875e-4,2.04248267e-4,2.03343239e-4,2.00759721e-4,
	    1.98685215e-4,1.99255563e-4,2.00772527e-4,2.00952039e-4,
	    1.99001021e-4,1.95628076e-4,1.96057706e-4,2.03936768e-4,
	    2.1208571e-4,2.13011823e-4,2.08796511e-4,2.03914606e-4,
	    2.01628398e-4,2.00800874e-4,2.00886192e-4,2.01789459e-4,
	    2.0341581e-4,2.05670323e-4,2.08458136e-4,2.11684339e-4,
	    2.15241234e-4,2.18969886e-4 };
    static doublereal bcodata[92] = { .935126662,.937326849,.942332983,
	    .948742092,.9551512,.960157335,.962725341,.963291347,.962659299,
	    .961633086,.961016774,.961614192,.960678637,.949014068,.940304458,
	    .937657118,.939596415,.944646776,.951332808,.958179057,.962559819,
	    .957249522,.939437032,.928570867,.941220164,.960178256,.965824127,
	    .962657094,.960942566,.967277169,.977537334,.984985471,.984694242,
	    .978978276,.971521497,.964244604,.958627403,.956149638,.957051337,
	    .956613481,.951577425,.949485362,.957303524,.968894005,.974842608,
	    .969201148,.959885418,.955879867,.956580281,.958985269,.960501969,
	    .960171998,.957655728,.953455031,.948282123,.942849219,.938500106,
	    .937929928,.939764142,.941950679,.94443047,.947642446,.951598167,
	    .954599559,.955186009,.95455718,.954577565,.956437945,.958634794,
	    .959314764,.957921565,.954823494,.952792108,.95419699,.957398534,
	    .959878385,.95961374,.955745041,.951571226,.950389028,.951328516,
	    .95247817,.953356564,.95778513,.965343416,.9741925,.982493639,
	    .988408089,.990096986,.985721588,.97440809,.959143162 };

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3, d__4, d__5;
    alist al__1;

    /* Builtin functions */
    integer i_dnnt(doublereal *), s_rnge(char *, integer, char *, integer), 
	    f_rew(alist *), s_rsle(cilist *), do_lio(integer *, integer *, 
	    char *, ftnlen), e_rsle(void);
    double log(doublereal), exp(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal ee, ap, ra1, ra2, ra3, ra4, ra5, ra6, rho, enew, ener1, 
	    ener2;
    extern doublereal bbsim_(U_fp, doublereal *, doublereal *, doublereal *);
    static doublereal apref;
    extern /* Subroutine */ doublereal fonct_(doublereal *);
    static doublereal estep, somme;
    static integer istep, nstep;
    static doublereal range1, rgdata[92];
    extern doublereal rhodata_(integer *);

    /* Fortran I/O blocks */
    static cilist io___120 = { 0, 14, 0, 0, 0 };
    static cilist io___122 = { 0, 14, 0, 0, 0 };
    static cilist io___123 = { 0, 14, 0, 0, 0 };
    static cilist io___126 = { 0, 13, 0, 0, 0 };



/*<       IMPLICIT NONE >*/
/*<       INTEGER NSTEP, I, ISTEP >*/
/*<       DOUBLE PRECISION RZP, RAP, RZT, TI, ENER1, ENER2, AP, RHO, APREF, ESTEP >*/
/*<       DOUBLE PRECISION RA1, RA2, RA3, RA4, RA5, RA6, EE, RANGE1, SOMME, ENEW >*/
/*<       DOUBLE PRECISION RF(1000),ETAB(1000),CONVDATA(92),ADATA(4,92),RHODATA >*/
/*<       DOUBLE PRECISION ACODATA(92), BCODATA(92), RGDATA(92), BBSIM >*/

/*<       DOUBLE PRECISION ZP, ZT, A6, A7, A8, A9, CONV, ACO, BCO >*/
/*<       COMMON/DEDX/ZP, ZT, A6, A7, A8, A9, CONV, ACO, BCO >*/

/*<       EXTERNAL FONCT, RHODATA >*/

/* *** CONVERSION FACTOR FROM (eV / 10**15 Atoms/cm**2) TO (MeV/mg/cm**2) */
/*     from F. Hubert et al., At. Data Nucl. Data Tables 46, 1 (1990), p. */
/*     for Z = 1 - 92 */
/*<        >*/

/*<        >*/
/* *** from Ziegler: "Helium Stopping Powers and Ranges in All Elements", */
/*     Pergamon, p. 66 & pp.69, coefficients A6 - A9 for high-energy stop */
/*     for Z = 1 - 92 */


/* *** CORRECTION COEFFICIENT FOR HIGH-ENERGY (>150MEV) ALPHA WITH RESPEC */
/*     TO JANNI'S PROTON DATA */
/*     for Z = 1 - 92 */
/*<        >*/

/* *** CORRECTION COEFFICIENT FOR HIGH-ENERGY (>150MEV) ALPHA WITH RESPEC */
/*     TO JANNI'S PROTON DATA */
/*     for Z = 1 - 92 */
/*<        >*/

/* ********************************************************************** */

/*<       ENER1 =   2.5 >*/
    ener1 = 2.5f;
/*<       ENER2 = TI >*/
    ener2 = *ti;
/*<       NSTEP = 999 >*/
    nstep = 999;
/*<       ZP = RZP >*/
    dedx_1.zp = *rzp;
/*<       ZT = RZT >*/
    dedx_1.zt = *rzt;
/*<       AP = RAP >*/
    ap = *rap;

/* *** READ FIT PARAMETERS FOR HIGH-ENERGY ALPHA RANGE */
/*     from Ziegler: "Helium Stopping Powers and Ranges in All Elements", */
/*     Pergamon, p. 66 & p.69, coefficients A6 - A9 for high-energy stopp */
/*     for Z = 1 - 92 */
/*<       A6 = ADATA(1,NINT(ZT)) >*/
    dedx_1.a6 = adata[(i__1 = (i_dnnt(&dedx_1.zt) << 2) - 4) < 368 && 0 <= 
	    i__1 ? i__1 : s_rnge("adata", i__1, "dedxhu_", (ftnlen)947)];
/*<       A7 = ADATA(2,NINT(ZT)) >*/
    dedx_1.a7 = adata[(i__1 = (i_dnnt(&dedx_1.zt) << 2) - 3) < 368 && 0 <= 
	    i__1 ? i__1 : s_rnge("adata", i__1, "dedxhu_", (ftnlen)948)];
/*<       A8 = ADATA(3,NINT(ZT)) >*/
    dedx_1.a8 = adata[(i__1 = (i_dnnt(&dedx_1.zt) << 2) - 2) < 368 && 0 <= 
	    i__1 ? i__1 : s_rnge("adata", i__1, "dedxhu_", (ftnlen)949)];
/*<       A9 = ADATA(4,NINT(ZT)) >*/
    dedx_1.a9 = adata[(i__1 = (i_dnnt(&dedx_1.zt) << 2) - 1) < 368 && 0 <= 
	    i__1 ? i__1 : s_rnge("adata", i__1, "dedxhu_", (ftnlen)950)];
/*     WRITE(6,*) 'A6-A9 = ', A6, A7, A8, A9 */

/* *** READ CONVERSION FACTOR FROM (eV / 10**15 Atoms/cm**2) TO (MeV/mg/c */
/*     from F. Hubert et al., At. Data Nucl. Data Tables 46, 1 (1990), p. */
/*     for Z = 1 - 92 */
/*<       CONV = CONVDATA(NINT(ZT)) >*/
    dedx_1.conv = convdata[(i__1 = i_dnnt(&dedx_1.zt) - 1) < 92 && 0 <= i__1 ?
	     i__1 : s_rnge("convdata", i__1, "dedxhu_", (ftnlen)956)];

/* *** READ TARGET DENSITY */
/*     for Z = 1 - 92 */
/*<       RHO = RHODATA(NINT(ZT)) >*/
    i__1 = i_dnnt(&dedx_1.zt);
    rho = rhodata_(&i__1);
/*     WRITE(6,*) 'RHO = ', RHO */

/* *** READ CORRECTION COEFFICIENT FOR HIGH-ENERGY (>150MEV) ALPHA WITH R */
/*     TO JANNI'S PROTON DATA */
/*     for Z = 1 - 92 */
/*<       ACO = ACODATA(INT(ZT)) >*/
    dedx_1.aco = acodata[(i__1 = (integer) dedx_1.zt - 1) < 92 && 0 <= i__1 ? 
	    i__1 : s_rnge("acodata", i__1, "dedxhu_", (ftnlen)966)];
/*<       BCO = BCODATA(INT(ZT)) >*/
    dedx_1.bco = bcodata[(i__1 = (integer) dedx_1.zt - 1) < 92 && 0 <= i__1 ? 
	    i__1 : s_rnge("bcodata", i__1, "dedxhu_", (ftnlen)967)];
/*     WRITE(6,*) 'ACO, BCO = ', ACO, BCO */

/* *** CALCULATE RANGE FOR 2.5 MEV/U */
/*<       IF( ZP .GE. 3 ) THEN >*/
    if (dedx_1.zp >= 3.) {
/* ***   READ 2.5 MEV/U RANGES AND MASS OF REFERENCE ISOTOPE FROM */
/*       L.C. Northcliffe and R.F. Schilling, Nucl. Data Tables A7, 233 ( */
/*<         REWIND(14) >*/
	al__1.aerr = 0;
	al__1.aunit = 14;
	f_rew(&al__1);
/*<         DO I = 1, INT(ZP-2.) >*/
	i__1 = (integer) (dedx_1.zp - 2.f);
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<           READ(14,*) APREF >*/
	    s_rsle(&io___120);
	    do_lio(&c__5, &c__1, (char *)&apref, (ftnlen)sizeof(doublereal));
	    e_rsle();
/*<           READ(14,*) >*/
	    s_rsle(&io___122);
	    e_rsle();
/*<           READ(14,*) RGDATA >*/
	    s_rsle(&io___123);
	    do_lio(&c__5, &c__92, (char *)&rgdata[0], (ftnlen)sizeof(
		    doublereal));
	    e_rsle();
/*<         ENDDO >*/
	}
/*       write(6,*) 'RGDATA = ', RGDATA */

/*<         RANGE1 = RGDATA(INT(ZT)) >*/
	range1 = rgdata[(i__1 = (integer) dedx_1.zt - 1) < 92 && 0 <= i__1 ? 
		i__1 : s_rnge("rgdata", i__1, "dedxhu_", (ftnlen)982)];
/*<         RANGE1 = RANGE1 *(AP/APREF) >*/
	range1 *= ap / apref;

/*<       ELSE >*/
    } else {
/* ***   READ LOW-ENERGY RANGE COEFFICIENTS FOR ALPHAS FROM FILE */
/*       from Ziegler: "Helium Stopping Powers and Ranges in All Elements */
/*       Pergamon, p. 66 & pp.70, coefficients A1 - A6 for low-energy sto */
/*       for Z = 1 - 92 */
/*<         REWIND(13) >*/
	al__1.aerr = 0;
	al__1.aunit = 13;
	f_rew(&al__1);
/*<         DO I = 1, INT(ZT) >*/
	i__1 = (integer) dedx_1.zt;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<           READ(13,*) RA1, RA2, RA3, RA4, RA5, RA6 >*/
	    s_rsle(&io___126);
	    do_lio(&c__5, &c__1, (char *)&ra1, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ra2, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ra3, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ra4, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ra5, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ra6, (ftnlen)sizeof(doublereal));
	    e_rsle();
/*<         ENDDO >*/
	}
/*       WRITE(6,*) 'RA1-RA6 = ',RA1, RA2, RA3, RA4, RA5, RA6 */
/*<         EE = LOG(2.5 * 4.0 *1000.) >*/
	ee = log(1e4f);
/*<         RANGE1 = EXP(RA1+RA2*EE+RA3*EE**2+RA4*EE**3+RA5*EE**4+RA6*EE**5) >*/
/* Computing 2nd power */
	d__1 = ee;
/* Computing 3rd power */
	d__2 = ee;
/* Computing 4th power */
	d__3 = ee, d__3 *= d__3;
/* Computing 5th power */
	d__4 = ee, d__5 = d__4, d__4 *= d__4;
	range1 = exp(ra1 + ra2 * ee + ra3 * (d__1 * d__1) + ra4 * (d__2 * (
		d__2 * d__2)) + ra5 * (d__3 * d__3) + ra6 * (d__5 * (d__4 * 
		d__4)));
/*<         RANGE1 = RANGE1*0.1*RHO >*/
	range1 = range1 * .1f * rho;
/*<         RANGE1 = RANGE1*( AP/4.) >*/
	range1 *= ap / 4.f;
/*<       ENDIF >*/
    }
/*     write(6,*) 'range1 = ', range1 */

/* *** INTEGRATE FROM 2.5 MEV/U TO 100 MeV/u */
/*<       ESTEP = (ENER2 - ENER1) / DFLOAT(NSTEP) >*/
    estep = (ener2 - ener1) / (doublereal) nstep;
/*<       RF(1) = 0. >*/
    rf[0] = 0.f;
/*<       ETAB(1) = 0. >*/
    etab[0] = 0.f;
/*<       DO 100 ISTEP = 1, NSTEP >*/
    i__1 = nstep;
    for (istep = 1; istep <= i__1; ++istep) {
/*<         ENEW = ENER1 + ESTEP * ISTEP >*/
	enew = ener1 + estep * istep;
/*<         ETAB(ISTEP+1) = ENEW >*/
	etab[(i__2 = istep) < 1000 && 0 <= i__2 ? i__2 : s_rnge("etab", i__2, 
		"dedxhu_", (ftnlen)1008)] = enew;
/*<         SOMME = BBSIM(FONCT, ENER1, ENEW, .0001d0) >*/
	somme = bbsim_((U_fp)fonct_, &ener1, &enew, &c_b110);
/*<         SOMME = SOMME * AP >*/
	somme *= ap;
/*<         RF(ISTEP+1) = SOMME + RANGE1 >*/
	rf[(i__2 = istep) < 1000 && 0 <= i__2 ? i__2 : s_rnge("rf", i__2, 
		"dedxhu_", (ftnlen)1011)] = somme + range1;
/*<  100  CONTINUE >*/
/* L100: */
    }
/*<  999  CONTINUE >*/
/* L999: */
/*<       END >*/
    return 0;
} /* dedxhu_ */

/* ______________ FUNCTION TO CALCULATE DE / DX _________________________ */

/*<       DOUBLE PRECISION FUNCTION FONCT(ENERGY) >*/
doublereal fonct_(doublereal *energy)
{
    /* Format strings */
    static char fmt_200[] = "(2x,\002PROBLEMS FOR THE FOLLOWING MEDIUM,ION,E"
	    "NERGY:\002)";
    static char fmt_300[] = "(3(f7.3))";

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double exp(doublereal), log(doublereal), pow_dd(doublereal *, doublereal *
	    );
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void), s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, 
	    char *, ftnlen);

    /* Local variables */
    static doublereal ee, vr, x111, y111, x222, x333, x444, cor, alph, gamma;

    /* Fortran I/O blocks */
    static cilist io___145 = { 0, 6, 0, 0, 0 };
    static cilist io___149 = { 0, 6, 0, fmt_200, 0 };
    static cilist io___150 = { 0, 6, 0, fmt_300, 0 };



/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION ENERGY, X111, X222, X333, X444, Y111, VR, GAMMA, EE, ALPH >*/
/*<       DOUBLE PRECISION COR >*/

/*<       DOUBLE PRECISION ZP, ZT, A6, A7, A8, A9, CONV, ACO, BCO >*/
/*<       COMMON/DEDX/ZP, ZT, A6, A7, A8, A9, CONV, ACO, BCO >*/

/* ----------------------------------------------------------------------- */
/*      ENERGY = 50. */
/*      ZP = 2. */
/*      ZT = 6. */
/*      A6 =  3.588000 */
/*      A7 =  0.392100 */
/*      A8 = -0.099350 */
/*      A9 = -0.007804 */
/*      CONV = 50.140 */
/*<       IF(ENERGY .LT. 2.4999) GO TO 999 >*/
    if (*energy < 2.4999f) {
	goto L999;
    }
/*<       IF(ENERGY .GT. 500.)   GO TO 999 >*/
    if (*energy > 500.f) {
	goto L999;
    }

/* *** calculates x111, x222, x333, x444, and gamma as in paper */
/*<       IF( ZT .EQ. 4. ) THEN >*/
    if (dedx_1.zt == 4.f) {
/*<         X222=-7. >*/
	x222 = -7.f;
/*<         X333=.2643 >*/
	x333 = .2643f;
/*<         X444=-0.4171 >*/
	x444 = -.4171f;
/*<         X111=2.045+2.00 * EXP(-0.04369 * ZP) >*/
	x111 = exp(dedx_1.zp * -.04369f) * 2.f + 2.045f;
/*<       ELSE IF(ZT .EQ. 6.) THEN >*/
    } else if (dedx_1.zt == 6.f) {
/*<         X222=-6.933 >*/
	x222 = -6.933f;
/*<         X333=0.2433 >*/
	x333 = .2433f;
/*<         X444=-0.3969 >*/
	x444 = -.3969f;
/*<         X111=2.584 + 1.910 * EXP(-0.03958 * ZP) >*/
	x111 = exp(dedx_1.zp * -.03958f) * 1.91f + 2.584f;
/*<       ELSE >*/
    } else {
/*<         X222= -(8.1436 + 0.098765 * LOG(ZT)) >*/
	x222 = -(log(dedx_1.zt) * .098765f + 8.1436f);
/*<         X333=0.31398 + 0.010725 * LOG(ZT) >*/
	x333 = log(dedx_1.zt) * .010725f + .31398f;
/*<         X444=-(0.52176 + 0.025214 * LOG(ZT)) >*/
	x444 = -(log(dedx_1.zt) * .025214f + .52176f);
/*<         Y111=1.1642 +0.23187 * EXP(-0.0043018 * ZT) >*/
	y111 = exp(dedx_1.zt * -.0043018f) * .23187f + 1.1642f;
/*<         X111=Y111 + 1.6583 * EXP (-0.051700 *ZP) >*/
	x111 = y111 + exp(dedx_1.zp * -.0517f) * 1.6583f;
/*<       ENDIF >*/
    }
/*     write(6,*) x111, x222, x333, x444 */

/*<  20   VR=X222 * (ENERGY**X333)*(ZP**X444) >*/
/* L20: */
    vr = x222 * pow_dd(energy, &x333) * pow_dd(&dedx_1.zp, &x444);
/*<       GAMMA=1.0-X111*EXP(VR) >*/
    gamma = 1.f - x111 * exp(vr);
/*<       IF(GAMMA.LT.0.) THEN >*/
    if (gamma < 0.f) {
/*<         WRITE(6,*) 'Help : GAMMA = ', GAMMA >*/
	s_wsle(&io___145);
	do_lio(&c__9, &c__1, "Help : GAMMA = ", (ftnlen)15);
	do_lio(&c__5, &c__1, (char *)&gamma, (ftnlen)sizeof(doublereal));
	e_wsle();
/*<         GO TO 999 >*/
	goto L999;
/*<       ENDIF >*/
    }
/*     write(6,*) 'vr, gamma = ', vr, gamma */

/*<  30   CONTINUE >*/
/* L30: */
/* *** Calculate alpha stopping power with Ziegler's high-energy fit para */
/*<       EE = LOG( 1.0/(ENERGY*4.0)) >*/
    ee = log(1.f / (*energy * 4.f));
/*<       ALPH = EXP(A6 + A7 * EE + A8 * (EE**2) + A9*(EE**3)) >*/
/* Computing 2nd power */
    d__1 = ee;
/* Computing 3rd power */
    d__2 = ee;
    alph = exp(dedx_1.a6 + dedx_1.a7 * ee + dedx_1.a8 * (d__1 * d__1) + 
	    dedx_1.a9 * (d__2 * (d__2 * d__2)));
/*     write(6,*) 'alp = ', alph, EE */
/*  ** convert stopping power from (eV / 10**15 Atoms/cm**2) to (MeV/mg/c */
/*<       ALPH = ALPH * CONV * 0.001 >*/
    alph = alph * dedx_1.conv * .001f;
/*  ** correct alpha stopping above 150 MeV/u according to Janni's proton */
/*<       COR = ACO * ENERGY + BCO >*/
    cor = dedx_1.aco * *energy + dedx_1.bco;
/*<       IF( ENERGY .LE. 150. ) COR =1. >*/
    if (*energy <= 150.f) {
	cor = 1.f;
    }
/*<       IF( COR .LT. 1.) COR=1. >*/
    if (cor < 1.f) {
	cor = 1.f;
    }
/*<       ALPH = ALPH * COR >*/
    alph *= cor;
/*   * scale stopping power from helium to projectile with eff. proj. cha */
/*<       FONCT = ((GAMMA * ZP)**2.)*ALPH/4. >*/
    d__1 = gamma * dedx_1.zp;
    ret_val = pow_dd(&d__1, &c_b341) * alph / 4.f;
/*     WRITE(6,*) 'FONCT = ', FONCT, GAMMA, ZP, ALPH, CONV, COR */
/*<       FONCT = 1. /FONCT >*/
    ret_val = 1.f / ret_val;
/*<       RETURN >*/
    return ret_val;

/*<  999  CONTINUE >*/
L999:
/*<       WRITE(6,200) >*/
    s_wsfe(&io___149);
    e_wsfe();
/*<  200  FORMAT(2X,'PROBLEMS FOR THE FOLLOWING MEDIUM,ION,ENERGY:') >*/
/*<  300  FORMAT(3(F7.3)) >*/
/* L300: */
/*<       WRITE(6,300)ZT,ZP,ENERGY >*/
    s_wsfe(&io___150);
    do_fio(&c__1, (char *)&dedx_1.zt, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&dedx_1.zp, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*energy), (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       FONCT = 0. >*/
    ret_val = 0.f;
/*<       RETURN >*/
    return ret_val;
/*<       END >*/
} /* fonct_ */

/* *********************************************************************** */

/*<       SUBROUTINE FCORR(Z,F) >*/
/* Subroutine */ int fcorr_(doublereal *z__, doublereal *f)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static doublereal r__;


/*     ------------------------------------------------------------------ */

/*     CALCULATES A CORRECTION TO THE RANGE FIT OF ECKHARD HANELT */
/*      based on measured dE/dx values from thesis Christoph Scheidenberg */
/*      Karl-Heinz Schmidt, 19. 12. 1995 */

/*     ------------------------------------------------------------------ */

/*<       IMPLICIT NONE >*/
/*<       DOUBLE PRECISION Z,R,F >*/

/*<       R = Z**2 / 1000. >*/
/* Computing 2nd power */
    d__1 = *z__;
    r__ = d__1 * d__1 / 1e3f;
/*     This is a preliminary version of the correction (19. 12. 1995) */
/*     F=1./(1.03653 - 0.0194135 * R + 0.00283533 * R**2 - 4.53044E-4 * R */
/*    #     + 4.27102E-5 * R**4) */
/*     This is a carefully determined correction (30. 12. 1995) */
/*<        >*/
/* Computing 2nd power */
    d__1 = r__;
/* Computing 3rd power */
    d__2 = r__;
/* Computing 4th power */
    d__3 = r__, d__3 *= d__3;
    *f = 1.f / (r__ * .00979114f + .965735686f + d__1 * d__1 * .00317099f - 
	    d__2 * (d__2 * d__2) * 6.71227e-4f + d__3 * d__3 * 2.28409e-5f);
/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* fcorr_ */


/* *********************************************************************** */
/* *********************************************************************** */
/* *********************************************************************** */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE C_CHIN( C_STR, I_INT  ) >*/
/* Subroutine */ int c_chin__(char *c_str__, integer *i_int__, ftnlen 
	c_str_len)
{
    /* System generated locals */
    icilist ici__1;

    /* Builtin functions */
    integer s_rsli(icilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsli(void);


/*     KONVERSION: CHARACTER VARIABLE  -> INTEGER - VARIABLE */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       INTEGER      I_INT >*/

/*<       READ( C_STR , * ) I_INT >*/
    ici__1.icierr = 0;
    ici__1.iciend = 0;
    ici__1.icirnum = 1;
    ici__1.icirlen = c_str_len;
    ici__1.iciunit = c_str__;
    ici__1.icifmt = 0;
    s_rsli(&ici__1);
    do_lio(&c__3, &c__1, (char *)&(*i_int__), (ftnlen)sizeof(integer));
    e_rsli();

/*<       END >*/
    return 0;
} /* c_chin__ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE C_CHRE( C_STR, R_REAL ) >*/
/* Subroutine */ int c_chre__(char *c_str__, doublereal *r_real__, ftnlen 
	c_str_len)
{
    /* System generated locals */
    icilist ici__1;

    /* Builtin functions */
    integer s_rsli(icilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsli(void);


/*     KONVERSION: CHARACTER VARIABLE  -> REAL - VARIABLE */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       DOUBLE PRECISION         R_REAL >*/

/*<       READ( C_STR , * ) R_REAL >*/
    ici__1.icierr = 0;
    ici__1.iciend = 0;
    ici__1.icirnum = 1;
    ici__1.icirlen = c_str_len;
    ici__1.iciunit = c_str__;
    ici__1.icifmt = 0;
    s_rsli(&ici__1);
    do_lio(&c__5, &c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
    e_rsli();

/*<       END >*/
    return 0;
} /* c_chre__ */


/* ______________________________________________________________________ */

/*<       SUBROUTINE C_INCH(  I_INT , C_STR ) >*/
/* Subroutine */ int c_inch__(integer *i_int__, char *c_str__, ftnlen 
	c_str_len)
{
    /* System generated locals */
    address a__1[2], a__2[3];
    integer i__1, i__2[2], i__3[3];
    char ch__1[4];
    icilist ici__1;

    /* Builtin functions */
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    ;
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer j;
    static char c_i__[1], c_form__[2], c_work__[1];

    /* Fortran I/O blocks */
    static icilist io___155 = { 0, c_i__, 0, "(I1)", 1, 1 };



/*     CONVERSION: INTEGER VARIABLE  -> CHARACTER - VARIABLE */

/*<       CHARACTER*(*)  C_STR >*/
/*<       CHARACTER*2    C_FORM >*/
/*<       CHARACTER*1    C_WORK, C_I >*/
/*<       INTEGER      I_INT, J >*/

/* ********************************************************************* */
/*<       WRITE(C_STR ,FMT='(I8)'  ) I_INT >*/
    ici__1.icierr = 0;
    ici__1.icirnum = 1;
    ici__1.icirlen = c_str_len;
    ici__1.iciunit = c_str__;
    ici__1.icifmt = "(I8)";
    s_wsfi(&ici__1);
    do_fio(&c__1, (char *)&(*i_int__), (ftnlen)sizeof(integer));
    e_wsfi();

/*     CUTS OFF UNNECESSARY SPACES */

/*<       DO 1 J = 1,8 >*/
    for (j = 1; j <= 8; ++j) {
/*<         C_WORK = C_STR(J:J) >*/
	*(unsigned char *)c_work__ = *(unsigned char *)&c_str__[j - 1];
/*<         IF( C_WORK .NE. ' ') THEN >*/
	if (*(unsigned char *)c_work__ != ' ') {
/*<           WRITE( C_I, FMT=('(I1)' ) ) 9 - J >*/
	    s_wsfi(&io___155);
	    i__1 = 9 - j;
	    do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	    e_wsfi();
/*<           C_FORM = 'I'//C_I >*/
/* Writing concatenation */
	    i__2[0] = 1, a__1[0] = "I";
	    i__2[1] = 1, a__1[1] = c_i__;
	    s_cat(c_form__, a__1, i__2, &c__2, (ftnlen)2);
/*<           WRITE( C_STR, FMT='('//C_FORM//')' )  I_INT >*/
	    ici__1.icierr = 0;
	    ici__1.icirnum = 1;
	    ici__1.icirlen = c_str_len;
	    ici__1.iciunit = c_str__;
/* Writing concatenation */
	    i__3[0] = 1, a__2[0] = "(";
	    i__3[1] = 2, a__2[1] = c_form__;
	    i__3[2] = 1, a__2[2] = ")";
	    ici__1.icifmt = (s_cat(ch__1, a__2, i__3, &c__3, (ftnlen)4), 
		    ch__1);
	    s_wsfi(&ici__1);
	    do_fio(&c__1, (char *)&(*i_int__), (ftnlen)sizeof(integer));
	    e_wsfi();
/*<         ENDIF >*/
	}
/*<     1 CONTINUE >*/
/* L1: */
    }
/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* c_inch__ */

/* ______________________________________________________________________ */

/*<       SUBROUTINE C_RECH(  R_REAL, C_STR ) >*/
/* Subroutine */ int c_rech__(doublereal *r_real__, char *c_str__, ftnlen 
	c_str_len)
{
    /* Builtin functions */
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
	    ;

    /* Local variables */
    extern /* Subroutine */ int nolbl_(char *, ftnlen);
    static char c_dummy__[8];

    /* Fortran I/O blocks */
    static icilist io___158 = { 0, c_dummy__, 0, "(F8.4)", 8, 1 };



/*     CONVERSION: REAL VARIABLE  -> CHARACTER - VARIABLE */

/*<       CHARACTER*8    C_DUMMY >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       DOUBLE PRECISION         R_REAL >*/

/* ********************************************************************* */
/*<       C_DUMMY = C_STR >*/
    s_copy(c_dummy__, c_str__, (ftnlen)8, c_str_len);
/*<       WRITE( C_DUMMY, FMT='(F8.4)' ) R_REAL >*/
    s_wsfi(&io___158);
    do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
    e_wsfi();
/*<       CALL NOLBL(C_DUMMY) >*/
    nolbl_(c_dummy__, (ftnlen)8);
/*<       C_STR = C_DUMMY >*/
    s_copy(c_str__, c_dummy__, c_str_len, (ftnlen)8);

/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* c_rech__ */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE PCHA( CSTR, CTR ) >*/
/* Subroutine */ int pcha_(char *cstr, char *ctr, ftnlen cstr_len, ftnlen 
	ctr_len)
{
    /* Format strings */
    static char fmt_1[] = "(\002 \002,a,\002: \002,a)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_rsfe(cilist *), e_rsfe(void), s_cmp(char *, char *, ftnlen, 
	    ftnlen);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static char chelp[20];

    /* Fortran I/O blocks */
    static cilist io___159 = { 0, 6, 0, fmt_1, 0 };
    static cilist io___160 = { 1, 5, 1, "(A)", 0 };



/*     PROMPTING - SUBROUTINE FOR CHARACTERS */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  CSTR, CTR >*/
/*<       CHARACTER*20   CHELP >*/

/*<       WRITE(6, 1) CSTR , CTR >*/
    s_wsfe(&io___159);
    do_fio(&c__1, cstr, cstr_len);
    do_fio(&c__1, ctr, ctr_len);
    e_wsfe();
/*<     1 FORMAT(' ',A,': ',A) >*/
/*<  10   READ( 5, FMT='(A)', ERR=11, END=11 ) CHELP >*/
/* L10: */
    i__1 = s_rsfe(&io___160);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = do_fio(&c__1, chelp, (ftnlen)20);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L11;
    }
/*     IF( CHELP .EQ. ' ' ) RETURN */

/*<       IF( CHELP .NE. ' ' ) CTR = CHELP >*/
    if (s_cmp(chelp, " ", (ftnlen)20, (ftnlen)1) != 0) {
	s_copy(ctr, chelp, ctr_len, (ftnlen)20);
    }

/*<  11   RETURN >*/
L11:
    return 0;
/*<       END >*/
} /* pcha_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE PILO( C_STR, I_INT  ) >*/
/* Subroutine */ int pilo_(char *c_str__, integer *i_int__, ftnlen c_str_len)
{
    /* Format strings */
    static char fmt_10[] = "(\002 \002,a,\002 \002,i5)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_rsfe(cilist *), e_rsfe(void), s_cmp(char *, char *, ftnlen, 
	    ftnlen), s_wsle(cilist *), do_lio(integer *, integer *, char *, 
	    ftnlen), e_wsle(void);

    /* Local variables */
    extern /* Subroutine */ int nolbl_(char *, ftnlen), c_chin__(char *, 
	    integer *, ftnlen);
    static char c_real__[80];

    /* Fortran I/O blocks */
    static cilist io___162 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___163 = { 1, 5, 1, "(A)", 0 };
    static cilist io___165 = { 0, 6, 0, 0, 0 };
    static cilist io___166 = { 0, 6, 0, 0, 0 };



/*     PROMPTING - SUBROUTINE FOR INTEGER */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       CHARACTER*(80) C_REAL >*/
/*<       INTEGER I_INT >*/

/*<  100  WRITE(6, 10) C_STR , I_INT >*/
L100:
    s_wsfe(&io___162);
    do_fio(&c__1, c_str__, c_str_len);
    do_fio(&c__1, (char *)&(*i_int__), (ftnlen)sizeof(integer));
    e_wsfe();
/*<  10   FORMAT(' ',A,' ',I5) >*/
/*<       READ( 5, FMT='(A)', ERR=11, END=11) C_REAL >*/
    i__1 = s_rsfe(&io___163);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = do_fio(&c__1, c_real__, (ftnlen)80);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L11;
    }
/*<       IF( C_REAL .EQ. ' ' ) RETURN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) == 0) {
	return 0;
    }

/*<       IF( C_REAL .NE. ' ' ) THEN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) != 0) {
/*<         IF( C_REAL(1:1) .EQ. ' ' ) CALL NOLBL(C_REAL) >*/
	if (*(unsigned char *)c_real__ == ' ') {
	    nolbl_(c_real__, (ftnlen)80);
	}
/*<       ENDIF >*/
    }

/*<        >*/
    if (*(unsigned char *)c_real__ != '+' && *(unsigned char *)c_real__ != 
	    '-' && *(unsigned char *)c_real__ != '1' && *(unsigned char *)
	    c_real__ != '2' && *(unsigned char *)c_real__ != '3' && *(
	    unsigned char *)c_real__ != '4' && *(unsigned char *)c_real__ != 
	    '5' && *(unsigned char *)c_real__ != '6' && *(unsigned char *)
	    c_real__ != '7' && *(unsigned char *)c_real__ != '8' && *(
	    unsigned char *)c_real__ != '9' && *(unsigned char *)c_real__ != 
	    '0') {
/*<         WRITE(6,*) '<E>: Integer number required!!!!!!' >*/
	s_wsle(&io___165);
	do_lio(&c__9, &c__1, "<E>: Integer number required!!!!!!", (ftnlen)34)
		;
	e_wsle();
/*<         WRITE(6,*) >*/
	s_wsle(&io___166);
	e_wsle();
/*<         GOTO 100 >*/
	goto L100;
/*<       ENDIF >*/
    }

/*<       IF( C_REAL .NE. ' ' ) THEN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) != 0) {
/*<         CALL C_CHIN( C_REAL, I_INT ) >*/
	c_chin__(c_real__, i_int__, (ftnlen)80);
/*<         RETURN >*/
	return 0;
/*<       ENDIF >*/
    }

/*<  11   CONTINUE >*/
L11:
/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* pilo_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE PRSO( C_STR, R_REAL, I) >*/
/* Subroutine */ int prso_(char *c_str__, doublereal *r_real__, integer *i__, 
	ftnlen c_str_len)
{
    /* Format strings */
    static char fmt_1[] = "(\002 \002,a,\002 \002,f9.0)";
    static char fmt_2[] = "(\002 \002,a,\002 \002,f9.1)";
    static char fmt_3[] = "(\002 \002,a,\002 \002,f9.2)";
    static char fmt_4[] = "(\002 \002,a,\002 \002,f9.3)";
    static char fmt_5[] = "(\002 \002,a,\002 \002,f9.4)";
    static char fmt_6[] = "(\002 \002,a,\002 \002,f9.5)";
    static char fmt_7[] = "(\002 \002,a,\002 \002,f9.6)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void), s_wsfe(cilist *), do_fio(integer *, char *, ftnlen),
	     e_wsfe(void), s_rsfe(cilist *), e_rsfe(void), s_cmp(char *, char 
	    *, ftnlen, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int nolbl_(char *, ftnlen), c_chre__(char *, 
	    doublereal *, ftnlen);
    static char c_real__[80];

    /* Fortran I/O blocks */
    static cilist io___167 = { 0, 6, 0, 0, 0 };
    static cilist io___168 = { 0, 6, 0, fmt_1, 0 };
    static cilist io___169 = { 0, 6, 0, fmt_2, 0 };
    static cilist io___170 = { 0, 6, 0, fmt_3, 0 };
    static cilist io___171 = { 0, 6, 0, fmt_4, 0 };
    static cilist io___172 = { 0, 6, 0, fmt_5, 0 };
    static cilist io___173 = { 0, 6, 0, fmt_6, 0 };
    static cilist io___174 = { 0, 6, 0, fmt_7, 0 };
    static cilist io___175 = { 1, 5, 1, "(A)", 0 };
    static cilist io___177 = { 0, 6, 0, 0, 0 };
    static cilist io___178 = { 0, 6, 0, 0, 0 };



/*     PROMPTING - SUBROUTINE */

/*<       IMPLICIT NONE >*/
/*<       INTEGER I >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       CHARACTER*(80) C_REAL >*/
/*<       DOUBLE PRECISION   R_REAL >*/

/*<  100  IF( I .LT. 0 ) THEN >*/
L100:
    if (*i__ < 0) {
/*<         WRITE(6,*) ' <E>: I < 0 IN SUBROUTINE PRSO' >*/
	s_wsle(&io___167);
	do_lio(&c__9, &c__1, " <E>: I < 0 IN SUBROUTINE PRSO", (ftnlen)30);
	e_wsle();
/*<         RETURN >*/
	return 0;
/*<       ENDIF >*/
    }
/*<       IF( I .GT. 6 ) I = 6 >*/
    if (*i__ > 6) {
	*i__ = 6;
    }

/*<       IF( I .EQ. 0 ) THEN >*/
    if (*i__ == 0) {
/*<         WRITE(6, 1) C_STR , R_REAL >*/
	s_wsfe(&io___168);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     1   FORMAT(' ',A,' ',F9.0) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 1 ) THEN >*/
    if (*i__ == 1) {
/*<         WRITE(6, 2) C_STR , R_REAL >*/
	s_wsfe(&io___169);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     2   FORMAT(' ',A,' ',F9.1) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 2 ) THEN >*/
    if (*i__ == 2) {
/*<         WRITE(6, 3) C_STR , R_REAL >*/
	s_wsfe(&io___170);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     3   FORMAT(' ',A,' ',F9.2) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 3 ) THEN >*/
    if (*i__ == 3) {
/*<         WRITE(6, 4) C_STR , R_REAL >*/
	s_wsfe(&io___171);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     4   FORMAT(' ',A,' ',F9.3) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 4 ) THEN >*/
    if (*i__ == 4) {
/*<         WRITE(6, 5) C_STR , R_REAL >*/
	s_wsfe(&io___172);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     5   FORMAT(' ',A,' ',F9.4) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 5 ) THEN >*/
    if (*i__ == 5) {
/*<         WRITE(6, 6) C_STR , R_REAL >*/
	s_wsfe(&io___173);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     6   FORMAT(' ',A,' ',F9.5) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<       IF( I .EQ. 6 ) THEN >*/
    if (*i__ == 6) {
/*<         WRITE(6, 7) C_STR , R_REAL >*/
	s_wsfe(&io___174);
	do_fio(&c__1, c_str__, c_str_len);
	do_fio(&c__1, (char *)&(*r_real__), (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<     7   FORMAT(' ',A,' ',F9.6) >*/
/*<         GOTO 10 >*/
	goto L10;
/*<       ENDIF >*/
    }
/*<  10   READ( 5, FMT='(A)', ERR=11, END=11 ) C_REAL >*/
L10:
    i__1 = s_rsfe(&io___175);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = do_fio(&c__1, c_real__, (ftnlen)80);
    if (i__1 != 0) {
	goto L11;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L11;
    }
/*<       IF( C_REAL .EQ. ' ' ) RETURN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) == 0) {
	return 0;
    }

/*<       IF( C_REAL .NE. ' ' ) THEN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) != 0) {
/*<         IF( C_REAL(1:1) .EQ. ' ' ) CALL NOLBL(C_REAL) >*/
	if (*(unsigned char *)c_real__ == ' ') {
	    nolbl_(c_real__, (ftnlen)80);
	}
/*<       ENDIF >*/
    }

/*<        >*/
    if (*(unsigned char *)c_real__ != '-' && *(unsigned char *)c_real__ != 
	    '+' && *(unsigned char *)c_real__ != '1' && *(unsigned char *)
	    c_real__ != '2' && *(unsigned char *)c_real__ != '3' && *(
	    unsigned char *)c_real__ != '4' && *(unsigned char *)c_real__ != 
	    '5' && *(unsigned char *)c_real__ != '6' && *(unsigned char *)
	    c_real__ != '7' && *(unsigned char *)c_real__ != '8' && *(
	    unsigned char *)c_real__ != '9' && *(unsigned char *)c_real__ != 
	    '0' && *(unsigned char *)c_real__ != '.') {
/*<         WRITE(6,*) '<E>: Real number required!!!!!!!' >*/
	s_wsle(&io___177);
	do_lio(&c__9, &c__1, "<E>: Real number required!!!!!!!", (ftnlen)32);
	e_wsle();
/*<         WRITE(6,*) >*/
	s_wsle(&io___178);
	e_wsle();
/*<         GOTO 100 >*/
	goto L100;
/*<       ENDIF >*/
    }
/*<       IF( C_REAL .NE. ' ' ) THEN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) != 0) {
/*<         CALL C_CHRE( C_REAL, R_REAL ) >*/
	c_chre__(c_real__, r_real__, (ftnlen)80);
/*<         RETURN >*/
	return 0;
/*<       ENDIF >*/
    }

/*<  11   CONTINUE >*/
L11:
/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* prso_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE PRTC( C_STR, I_INT  ) >*/
/* Subroutine */ int prtc_(char *c_str__, integer *i_int__, ftnlen c_str_len)
{
    /* Format strings */
    static char fmt_10[] = "(\002 \002,a,\002 \002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Fortran I/O blocks */
    static cilist io___179 = { 0, 0, 0, fmt_10, 0 };



/*     PROMPTING - SUBROUTINE FOR TEXT STRING */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  C_STR >*/
/*<       INTEGER I_INT >*/

/*<       WRITE(I_INT, 10) C_STR >*/
    io___179.ciunit = *i_int__;
    s_wsfe(&io___179);
    do_fio(&c__1, c_str__, c_str_len);
    e_wsfe();
/*<  10   FORMAT(' ',A,' ' ) >*/

/*<       RETURN >*/
    return 0;
/*<       END >*/
} /* prtc_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE PYES( C_STR, C_ANS  ) >*/
/* Subroutine */ int pyes_(char *c_str__, char *c_ans__, ftnlen c_str_len, 
	ftnlen c_ans_len)
{
    /* Format strings */
    static char fmt_10[] = "(\002 \002,a,\002?   --> \002,a,\002?\002)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_rsfe(cilist *), e_rsfe(void), s_cmp(char *, char *, ftnlen, 
	    ftnlen);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int nolbl_(char *, ftnlen);
    static char c_real__[80];

    /* Fortran I/O blocks */
    static cilist io___180 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___181 = { 1, 5, 1, "(A)", 0 };



/*     PROMPTING - SUBROUTINE FOR INTEGER */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*)  C_STR, C_ANS >*/
/*<       CHARACTER*(80) C_REAL >*/
/*     C_ANS WIRD ALS DEFAULT BENUTZT!!!!! */

/*<       WRITE(6, 10) C_STR, C_ANS >*/
    s_wsfe(&io___180);
    do_fio(&c__1, c_str__, c_str_len);
    do_fio(&c__1, c_ans__, c_ans_len);
    e_wsfe();
/*<  10   FORMAT(' ',A,'?   --> ',A,'?') >*/
/*<       READ( 5, FMT='(A)',ERR=100, END=100 ) C_REAL >*/
    i__1 = s_rsfe(&io___181);
    if (i__1 != 0) {
	goto L100;
    }
    i__1 = do_fio(&c__1, c_real__, (ftnlen)80);
    if (i__1 != 0) {
	goto L100;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L100;
    }

/*<       IF( C_REAL .EQ. ' ' ) C_REAL = C_ANS >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) == 0) {
	s_copy(c_real__, c_ans__, (ftnlen)80, c_ans_len);
    }

/*<       IF( C_REAL .NE. ' ' ) THEN >*/
    if (s_cmp(c_real__, " ", (ftnlen)80, (ftnlen)1) != 0) {
/*<         CALL NOLBL(C_REAL) >*/
	nolbl_(c_real__, (ftnlen)80);
/*<         C_ANS = C_REAL(1:1) >*/
	s_copy(c_ans__, c_real__, c_ans_len, (ftnlen)1);
/*<        >*/
	if (s_cmp(c_ans__, "J", c_ans_len, (ftnlen)1) == 0 || s_cmp(c_ans__, 
		"Y", c_ans_len, (ftnlen)1) == 0 || s_cmp(c_ans__, "O", 
		c_ans_len, (ftnlen)1) == 0 || s_cmp(c_ans__, "j", c_ans_len, (
		ftnlen)1) == 0 || s_cmp(c_ans__, "y", c_ans_len, (ftnlen)1) ==
		 0 || s_cmp(c_ans__, "o", c_ans_len, (ftnlen)1) == 0) {
/*<           C_ANS = 'Y' >*/
	    s_copy(c_ans__, "Y", c_ans_len, (ftnlen)1);
/*<           GOTO 100 >*/
	    goto L100;
/*<         ELSE >*/
	} else {
/*<           C_ANS = 'N' >*/
	    s_copy(c_ans__, "N", c_ans_len, (ftnlen)1);
/*<         ENDIF >*/
	}
/*<       ENDIF >*/
    }

/*<  100  RETURN >*/
L100:
    return 0;
/*<       END >*/
} /* pyes_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
/*<       DOUBLE PRECISION FUNCTION BBSIM(FCT, A, B, ACC ) >*/
doublereal bbsim_(U_fp fct, doublereal *a, doublereal *b, doublereal *acc)
{
    /* System generated locals */
  ERR("bbsim_ should not be used");
    integer i__1;
    doublereal ret_val, d__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);

    /* Local variables */
    static doublereal h__;
    static integer l;
    static doublereal s, s1, s2, s4, bs;
    static integer ks;
    static doublereal eps;
    static integer imax;
    extern /* Subroutine */ int prtc_(char *, integer *, ftnlen);
    static integer iopt;
    static doublereal slast;

    /* Fortran I/O blocks */
    static cilist io___185 = { 0, 6, 0, 0, 0 };
    static cilist io___196 = { 0, 6, 0, 0, 0 };



/*     INTEGRATES NUMERICALLY POSITIVE FUNCTION */
/*                          0 <= A <= X <= B */

/*<       IMPLICIT NONE >*/
/*<       INTEGER KS, L, IMAX, IOPT >*/
/*<       DOUBLE PRECISION    A, B, EPS, H, S1, S2, S4, SLAST, S, ACC, BS, FCT >*/

/*<       EPS = 0. >*/
    eps = 0.f;
/*<       IOPT = 0 >*/
    iopt = 0;
/*<       IF( B .LT. A ) THEN >*/
    if (*b < *a) {
/*<         WRITE(6,*) '<W> IN BBSIM: INVALID LIMITS ' >*/
	s_wsle(&io___185);
	do_lio(&c__9, &c__1, "<W> IN BBSIM: INVALID LIMITS ", (ftnlen)29);
	e_wsle();
/*<         IOPT = 1 >*/
	iopt = 1;
/*<         BS = A >*/
	bs = *a;
/*<         A = B >*/
	*a = *b;
/*<         B = BS >*/
	*b = bs;
/*<       ENDIF >*/
    }

/*<       IMAX = 0 >*/
    imax = 0;

/*<       H = 0.5E0 * (B - A) >*/
    h__ = (*b - *a) * .5f;
/*<       S1 = H/3.E0 * ( FCT(A) + FCT(B) ) >*/
    s1 = h__ / 3.f * ((*fct)(a) + (*fct)(b));
/*<       S2 = 0. >*/
    s2 = 0.f;
/*<       S4 = 4.E0*H/3.E0 * FCT(A+H) >*/
    d__1 = *a + h__;
    s4 = h__ * 4.f / 3.f * (*fct)(&d__1);
/*<       S = S1 + S2 + S4 >*/
    s = s1 + s2 + s4;
/*<       SLAST = 0. >*/
    slast = 0.f;
/* ***  READY FOR ITERATION */
/*<       L = 4 >*/
    l = 4;
/*<  10   CONTINUE >*/
L10:
/*<       H = 0.5E0 * H >*/
    h__ *= .5f;
/* ***       INTERVALLHALBIERUNG */
/*<       S1 = S1 * 0.5E0 >*/
    s1 *= .5f;
/*<       S2 = S2 * 0.5E0 + S4 * 0.25E0 >*/
    s2 = s2 * .5f + s4 * .25f;
/*<       S4 = 0. >*/
    s4 = 0.f;
/* ***       RESET FOR KS-LOOP */
/*<       DO 11 KS=1, L-1, 2 >*/
    i__1 = l - 1;
    for (ks = 1; ks <= i__1; ks += 2) {
/*<         S4 = S4 + 4.E0*H/3.E0 * FCT(A + FLOAT(KS)*H) >*/
	d__1 = *a + (real) ks * h__;
	s4 += h__ * 4.f / 3.f * (*fct)(&d__1);
/*<  11   CONTINUE >*/
/* L11: */
    }
/*<       SLAST = S >*/
    slast = s;
/* ***       UPDATING */
/*<       S = S1 + S2 + S4 >*/
    s = s1 + s2 + s4;
/*<       IF( S .LE. 1.E-50 .AND. SLAST .LE. 1.E-50 ) GOTO 999 >*/
    if (s <= 1e-50f && slast <= 1e-50f) {
	goto L999;
    }
/*<       IF( ABS(S+SLAST) .LE. 1.E-50 ) GOTO 19 >*/
    if ((d__1 = s + slast, abs(d__1)) <= 1e-50f) {
	goto L19;
    }
/*<       EPS = ABS((S-SLAST) / ( S + SLAST )) * 2. >*/
    eps = (d__1 = (s - slast) / (s + slast), abs(d__1)) * 2.f;
/*<       IF( EPS .LE. ACC ) GOTO 999 >*/
    if (eps <= *acc) {
	goto L999;
    }
/*<  19   CONTINUE >*/
L19:
/*<       IMAX = IMAX + 1 >*/
    ++imax;
/*<       IF( IMAX .GT.  20 ) GOTO 20 >*/
    if (imax > 20) {
	goto L20;
    }
/*<       L = L + L >*/
    l += l;
/*<       GOTO 10 >*/
    goto L10;
/*<  20   CONTINUE >*/
L20:

/*<       IF( EPS .LE. 1.E-10 ) GOTO 999 >*/
    if (eps <= 1e-10f) {
	goto L999;
    }
/*<       CALL PRTC('<W> FROM BBSIM: SPECIFIED ACCURACY NOT AVAILABLE',6) >*/
    prtc_("<W> FROM BBSIM: SPECIFIED ACCURACY NOT AVAILABLE", &c__6, (ftnlen)
	    48);
/*<       WRITE(6,*) 'ACC = ', ACC, ' EPS = ', EPS >*/
    s_wsle(&io___196);
    do_lio(&c__9, &c__1, "ACC = ", (ftnlen)6);
    do_lio(&c__5, &c__1, (char *)&(*acc), (ftnlen)sizeof(doublereal));
    do_lio(&c__9, &c__1, " EPS = ", (ftnlen)7);
    do_lio(&c__5, &c__1, (char *)&eps, (ftnlen)sizeof(doublereal));
    e_wsle();

/*<  999  CONTINUE >*/
L999:
/*<       IF( IOPT .EQ. 1 ) THEN >*/
    if (iopt == 1) {
/*<         B = A >*/
	*b = *a;
/*<         A = BS >*/
	*a = bs;
/*<       ENDIF >*/
    }

/*<       BBSIM = S >*/
    ret_val = s;
/*     WRITE(6,*) 'EPS = ', EPS */
/*     WRITE(6,*) 'SUM = ', S */
/*<       END >*/
    return ret_val;
} /* bbsim_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       DOUBLE PRECISION FUNCTION BBSIM1(FCT, A, B, ACC ) >*/
doublereal bbsim1_(D_fp fct, doublereal *a, doublereal *b, doublereal *acc)
{
    /* System generated locals */
  ERR("bbsim1_ should not be used");
    integer i__1;
    doublereal ret_val, d__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);

    /* Local variables */
    static doublereal h__;
    static integer l;
    static doublereal s, s1, s2, s4, bs;
    static integer ks;
    static doublereal eps;
    static integer imax;
    extern /* Subroutine */ int prtc_(char *, integer *, ftnlen);
    static integer iopt;
    static doublereal slast;

    /* Fortran I/O blocks */
    static cilist io___199 = { 0, 6, 0, 0, 0 };
    static cilist io___210 = { 0, 6, 0, 0, 0 };



/*     INTEGRATES NUMERICALLY POSITIVE FUNCTION */
/*                          0 <= A <= X <= B */

/*<       IMPLICIT NONE >*/
/*<       INTEGER KS, L, IMAX, IOPT >*/
/*<       DOUBLE PRECISION    A, B, EPS, H, S1, S2, S4, SLAST, S, ACC, BS, FCT >*/

/*<       EPS = 0. >*/
    eps = 0.f;
/*<       IOPT = 0 >*/
    iopt = 0;
/*<       IF( B .LT. A ) THEN >*/
    if (*b < *a) {
/*<         WRITE(6,*) '<W>: IN BBSIM1: INVALID LIMITS ' >*/
	s_wsle(&io___199);
	do_lio(&c__9, &c__1, "<W>: IN BBSIM1: INVALID LIMITS ", (ftnlen)31);
	e_wsle();
/*<         IOPT = 1 >*/
	iopt = 1;
/*<         BS = A >*/
	bs = *a;
/*<         A = B >*/
	*a = *b;
/*<         B = BS >*/
	*b = bs;
/*<       ENDIF >*/
    }

/*<       IMAX = 0 >*/
    imax = 0;

/*<       H = 0.5E0 * (B - A) >*/
    h__ = (*b - *a) * .5f;
/*<       S1 = H/3.E0 * ( FCT(A) + FCT(B) ) >*/
    s1 = h__ / 3.f * ((*fct)(a) + (*fct)(b));
/*<       S2 = 0. >*/
    s2 = 0.f;
/*<       S4 = 4.E0*H/3.E0 * FCT(A+H) >*/
    d__1 = *a + h__;
    s4 = h__ * 4.f / 3.f * (*fct)(&d__1);
/*<       S = S1 + S2 + S4 >*/
    s = s1 + s2 + s4;
/*<       SLAST = 0. >*/
    slast = 0.f;
/* ***  READY FOR ITERATION */
/*<       L = 4 >*/
    l = 4;
/*<  10   CONTINUE >*/
L10:
/*<       H = 0.5E0 * H >*/
    h__ *= .5f;
/* ***       INTERVALLHALBIERUNG */
/*<       S1 = S1 * 0.5E0 >*/
    s1 *= .5f;
/*<       S2 = S2 * 0.5E0 + S4 * 0.25E0 >*/
    s2 = s2 * .5f + s4 * .25f;
/*<       S4 = 0. >*/
    s4 = 0.f;
/* ***       RESET FOR KS-LOOP */
/*<       DO 11 KS=1, L-1, 2 >*/
    i__1 = l - 1;
    for (ks = 1; ks <= i__1; ks += 2) {
/*<         S4 = S4 + 4.E0*H/3.E0 * FCT(A + FLOAT(KS)*H) >*/
	d__1 = *a + (real) ks * h__;
	s4 += h__ * 4.f / 3.f * (*fct)(&d__1);
/*<  11   CONTINUE >*/
/* L11: */
    }
/*<       SLAST = S >*/
    slast = s;
/* ***       UPDATING */
/*<       S = S1 + S2 + S4 >*/
    s = s1 + s2 + s4;
/*<       IF( S .LE. 1.E-50 .AND. SLAST .LE. 1.E-50 ) GOTO 999 >*/
    if (s <= 1e-50f && slast <= 1e-50f) {
	goto L999;
    }
/*<       IF( ABS(S+SLAST) .LE. 1.E-50 ) GOTO 19 >*/
    if ((d__1 = s + slast, abs(d__1)) <= 1e-50f) {
	goto L19;
    }
/*<       EPS = ABS((S-SLAST) / ( S + SLAST )) * 2. >*/
    eps = (d__1 = (s - slast) / (s + slast), abs(d__1)) * 2.f;
/*<       IF( EPS .LE. ACC ) GOTO 999 >*/
    if (eps <= *acc) {
	goto L999;
    }
/*<  19   CONTINUE >*/
L19:
/*<       IMAX = IMAX + 1 >*/
    ++imax;
/*<       IF( IMAX .GT.  20 ) GOTO 20 >*/
    if (imax > 20) {
	goto L20;
    }
/*<       L = L + L >*/
    l += l;
/*<       GOTO 10 >*/
    goto L10;
/*<  20   CONTINUE >*/
L20:

/*<       IF( EPS .LE. 1.E-10 ) GOTO 999 >*/
    if (eps <= 1e-10f) {
	goto L999;
    }
/*<       CALL PRTC('<W> FROM BBSIM1: SPECIFIED ACCURACY NOT AVAILABLE',6) >*/
    prtc_("<W> FROM BBSIM1: SPECIFIED ACCURACY NOT AVAILABLE", &c__6, (ftnlen)
	    49);
/*<       WRITE(6,*) 'ACC = ', ACC, ' EPS = ', EPS >*/
    s_wsle(&io___210);
    do_lio(&c__9, &c__1, "ACC = ", (ftnlen)6);
    do_lio(&c__5, &c__1, (char *)&(*acc), (ftnlen)sizeof(doublereal));
    do_lio(&c__9, &c__1, " EPS = ", (ftnlen)7);
    do_lio(&c__5, &c__1, (char *)&eps, (ftnlen)sizeof(doublereal));
    e_wsle();

/*<  999  CONTINUE >*/
L999:
/*<       IF( IOPT .EQ. 1 ) THEN >*/
    if (iopt == 1) {
/*<         B = A >*/
	*b = *a;
/*<         A = BS >*/
	*a = bs;
/*<       ENDIF >*/
    }

/*<       BBSIM1 = S >*/
    ret_val = s;
/*     WRITE(6,*) 'EPS = ', EPS */
/*     WRITE(6,*) 'SUM = ', S */
/*<       END >*/
    return ret_val;
} /* bbsim1_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       SUBROUTINE NOLBL(CSTR) >*/
/* Subroutine */ int nolbl_(char *cstr, ftnlen cstr_len)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer i_len(char *, ftnlen);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static integer i__, ilen;
    static char cwork[1];


/*     UNTERDRUECKT BLANKS AM ANFANG EINES STRINGS */

/*<       IMPLICIT NONE >*/
/*<       CHARACTER*(*) CSTR >*/
/*<       CHARACTER*1   CWORK >*/
/*<       INTEGER     ILEN, I >*/

/*<       ILEN = LEN(CSTR) >*/
    ilen = i_len(cstr, cstr_len);
/*<       DO 1 I = 1,ILEN >*/
    i__1 = ilen;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         CWORK = CSTR(I:I) >*/
	*(unsigned char *)cwork = *(unsigned char *)&cstr[i__ - 1];
/*<         IF( CWORK .NE. ' ' ) THEN >*/
	if (*(unsigned char *)cwork != ' ') {
/*<           CSTR = CSTR(I:) >*/
	    s_copy(cstr, cstr + (i__ - 1), cstr_len, cstr_len - (i__ - 1));
/*<           GOTO 2 >*/
	    goto L2;
/*<         ENDIF >*/
	}
/*<     1 CONTINUE >*/
/* L1: */
    }
/*<     2 CONTINUE >*/
L2:

/*<       END >*/
    return 0;
} /* nolbl_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*<       DOUBLE PRECISION FUNCTION RINT1D( X, XTAB, YTAB, N ) >*/
doublereal rint1d_(doublereal *x, doublereal *xtab, doublereal *ytab, integer 
	*n)
{
    /* System generated locals */
    integer xtab_dim1, ytab_dim1, i__1, i__2, i__3, i__4, i__5;
    doublereal ret_val;

    /* Builtin functions */
    integer s_rnge(char *, integer, char *, integer);

    /* Local variables */
    static integer i__;
    static doublereal y, xx;
    static integer isave;


/* *** INTERPOLATION FUER LINEARE FUNKTIONEN */

/*<       IMPLICIT NONE >*/
/*<       INTEGER  N, I, ISAVE >*/
/*<       DOUBLE PRECISION     X, XTAB(N), YTAB(N), Y, XX >*/

/*<       DO 10 I = 1, N >*/
    /* Parameter adjustments */
    ytab_dim1 = *n;
    xtab_dim1 = *n;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         IF( X .EQ. XTAB(I) ) THEN >*/
	if (*x == xtab[(i__2 = i__ - 1) < 1 * xtab_dim1 && 0 <= i__2 ? i__2 : 
		s_rnge("xtab", i__2, "rint1d_", (ftnlen)1561)]) {
/*<           Y = YTAB(I) >*/
	    y = ytab[(i__2 = i__ - 1) < 1 * ytab_dim1 && 0 <= i__2 ? i__2 : 
		    s_rnge("ytab", i__2, "rint1d_", (ftnlen)1562)];
/*<           GOTO 9999 >*/
	    goto L9999;
/*<         ENDIF >*/
	}
/*<  10   CONTINUE >*/
/* L10: */
    }

/*<       IF( X .LT. XTAB(1) ) THEN >*/
    if (*x < xtab[(i__1 = 0) < 1 * xtab_dim1 ? i__1 : s_rnge("xtab", i__1, 
	    "rint1d_", (ftnlen)1567)]) {
/*<         ISAVE = 1 >*/
	isave = 1;
/*<         GOTO 100 >*/
	goto L100;
/*<       ENDIF >*/
    }

/*<       IF( X .GT. XTAB(N) ) THEN >*/
    if (*x > xtab[(i__1 = *n - 1) < 1 * xtab_dim1 && 0 <= i__1 ? i__1 : 
	    s_rnge("xtab", i__1, "rint1d_", (ftnlen)1572)]) {
/*<         ISAVE = N - 1 >*/
	isave = *n - 1;
/*<         GOTO 100 >*/
	goto L100;
/*<       ENDIF >*/
    }

/*<       DO 11 I = 1, N >*/
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         IF( X .LT. XTAB(I) ) THEN >*/
	if (*x < xtab[(i__2 = i__ - 1) < 1 * xtab_dim1 && 0 <= i__2 ? i__2 : 
		s_rnge("xtab", i__2, "rint1d_", (ftnlen)1578)]) {
/*<           ISAVE = I - 1 >*/
	    isave = i__ - 1;
/*<           GOTO 100 >*/
	    goto L100;
/*<         ENDIF >*/
	}
/*<  11   CONTINUE >*/
/* L11: */
    }

/*<  100  CONTINUE >*/
L100:

/*<       XX = X - XTAB(ISAVE) >*/
    xx = *x - xtab[(i__1 = isave - 1) < 1 * xtab_dim1 && 0 <= i__1 ? i__1 : 
	    s_rnge("xtab", i__1, "rint1d_", (ftnlen)1586)];
/*<        >*/
    y = ytab[(i__1 = isave - 1) < 1 * ytab_dim1 && 0 <= i__1 ? i__1 : s_rnge(
	    "ytab", i__1, "rint1d_", (ftnlen)1587)] + xx * (ytab[(i__2 = 
	    isave) < 1 * ytab_dim1 && 0 <= i__2 ? i__2 : s_rnge("ytab", i__2, 
	    "rint1d_", (ftnlen)1587)] - ytab[(i__3 = isave - 1) < 1 * 
	    ytab_dim1 && 0 <= i__3 ? i__3 : s_rnge("ytab", i__3, "rint1d_", (
	    ftnlen)1587)]) / (xtab[(i__4 = isave) < 1 * xtab_dim1 && 0 <= 
	    i__4 ? i__4 : s_rnge("xtab", i__4, "rint1d_", (ftnlen)1587)] - 
	    xtab[(i__5 = isave - 1) < 1 * xtab_dim1 && 0 <= i__5 ? i__5 : 
	    s_rnge("xtab", i__5, "rint1d_", (ftnlen)1587)]);
/*<  9999 RINT1D = Y >*/
L9999:
    ret_val = y;
/*<       RETURN >*/
    return ret_val;
/*<       END >*/
} /* rint1d_ */


/* ______________________________________________________________________ */

/*<       SUBROUTINE ELEMENT( Z, A, CZ, IOPT, IRC ) >*/
/* Subroutine */ int element_(doublereal *z__, doublereal *a, char *cz, 
	integer *iopt, integer *irc, ftnlen cz_len)
{
    /* Initialized data */
  cz_len=cz_len;
    static char czdata[2*97] = "H " "HE" "LI" "BE" "B " "C " "N " "O " "F " 
	    "NE" "NA" "MG" "AL" "SI" "P " "S " "CL" "AR" "K " "CA" "SC" "TI" 
	    "V " "CR" "MN" "FE" "CO" "NI" "CU" "ZN" "GA" "GE" "AS" "SE" "BR" 
	    "KR" "RB" "SR" "Y " "ZR" "NB" "MO" "TC" "RU" "RH" "PD" "AG" "CD" 
	    "IN" "SN" "SB" "TE" "J " "XE" "CS" "BA" "LA" "CE" "PR" "ND" "PM" 
	    "SM" "EU" "GD" "TB" "DY" "HO" "ER" "TM" "YB" "LU" "HF" "TA" "W " 
	    "RE" "OS" "IR" "PT" "AU" "HG" "TL" "PB" "BI" "PO" "AT" "RN" "FR" 
	    "RA" "AC" "TH" "PA" "U " "NP" "PU" "AM" "CM" "MY";
    static doublereal mass[97] = { 1.,4.,6.94,9.01,10.81,12.01,14.01,16.,19.,
	    20.18,23.,24.31,26.98,28.09,30.97,32.06,35.45,39.95,39.1,40.08,
	    44.96,47.88,50.94,52.,54.94,55.84,58.93,58.69,63.55,65.4,69.72,
	    72.59,74.92,78.96,79.9,83.8,85.47,87.62,88.91,91.22,92.91,95.93,
	    98.,101.07,102.91,106.42,107.87,112.41,114.82,118.71,121.76,127.6,
	    126.9,131.29,132.91,137.33,138.91,140.12,140.91,144.24,147.,
	    150.36,151.97,157.25,158.92,162.5,164.93,167.26,168.93,173.03,
	    174.97,178.49,180.95,183.85,186.21,190.2,192.22,195.08,196.97,
	    200.6,204.38,207.2,208.98,203.,209.,211.,212.,213.,222.,232.,231.,
	    238.,237.,244.,243.,247.,13.72 };

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_rnge(char *, integer, char *, integer), s_cmp(char *, char *, 
	    ftnlen, ftnlen), i_dnnt(doublereal *), s_wsle(cilist *), do_lio(
	    integer *, integer *, char *, ftnlen), e_wsle(void);

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int uppercase_(char *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___221 = { 0, 6, 0, 0, 0 };



/*     RETURNS ELEMENT SYMBOL FROM NUCLEAR CHARGE */

/*     IOPT 1: CHARGE NUMBER TO ELEMENT SYMBOL CONVERSION */
/*     IOPT 2: ELEMENT SYMBOL TO CHARGE NUMBER CONVERSION */
/*     IOPT 3: CHARGE NUMBER TO ELEMENT SYMBOL CONVERSION */
/*     IOPT 4: ELEMENT SYMBOL TO CHARGE NUMBER CONVERSION */
/*     IOPT 3 ET 4: INTEGER CUTTING FOR BEAMS */
/*     IOPT 5: RETURNS Z AND CZ FOR A GIVEN MASS NUMBER A */
/*     IOPT 6: Z TO SYMBOL CONVERSION */

/*<       DOUBLE PRECISION Z, A, MASS(97) >*/
/*<       INTEGER IOPT, IRC, I >*/
/*<       CHARACTER*2 CZ, CZDATA(97) >*/

/* ********************************************************************** */
/*<        >*/

/*<        >*/

/* ********************************************************************** */

/*<       CALL UPPERCASE(CZ) >*/
    uppercase_(cz, (ftnlen)2);

/*<       IF( IOPT .EQ. 1 .OR. IOPT .EQ. 3 ) THEN >*/
    if (*iopt == 1 || *iopt == 3) {
/*<         IF( Z .EQ. 6.6 ) THEN >*/
	if (*z__ == 6.6f) {
/*<           CZ = CZDATA(97) >*/
	    s_copy(cz, czdata + 192, (ftnlen)2, (ftnlen)2);
/*<           GOTO 990 >*/
	    goto L990;
/*<         ENDIF >*/
	}
/*<         DO I = 1, 96 >*/
	for (i__ = 1; i__ <= 96; ++i__) {
/*<           IF( INT(Z) .EQ. I ) THEN >*/
	    if ((integer) (*z__) == i__) {
/*<             CZ = CZDATA(I) >*/
		s_copy(cz, czdata + (((i__1 = i__ - 1) < 97 && 0 <= i__1 ? 
			i__1 : s_rnge("czdata", i__1, "element_", (ftnlen)
			1652)) << 1), (ftnlen)2, (ftnlen)2);
/*<             IRC = 0 >*/
		*irc = 0;
/*<             GOTO 990 >*/
		goto L990;
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}

/*<       ELSE IF( IOPT .EQ. 2 .OR. IOPT .EQ. 4 ) THEN >*/
    } else if (*iopt == 2 || *iopt == 4) {
/*<         DO I = 1, 97 >*/
	for (i__ = 1; i__ <= 97; ++i__) {
/*<           IF( CZ .EQ. CZDATA(I) ) THEN >*/
	    if (s_cmp(cz, czdata + (((i__1 = i__ - 1) < 97 && 0 <= i__1 ? 
		    i__1 : s_rnge("czdata", i__1, "element_", (ftnlen)1660)) 
		    << 1), (ftnlen)2, (ftnlen)2) == 0) {
/*<             Z = FLOAT(I) >*/
		*z__ = (real) i__;
/*<             IF( CZ .EQ. 'MY' ) Z = 6.6 >*/
		if (s_cmp(cz, "MY", (ftnlen)2, (ftnlen)2) == 0) {
		    *z__ = 6.6f;
		}
/*<             IRC = 0 >*/
		*irc = 0;
/*<             GOTO 990 >*/
		goto L990;
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<       ELSE IF( IOPT .EQ. 5 ) THEN >*/
    } else if (*iopt == 5) {
/*<         DO I = 1, 97 >*/
	for (i__ = 1; i__ <= 97; ++i__) {
/*<           IF( A .LE. MASS(I) ) THEN >*/
	    if (*a <= mass[(i__1 = i__ - 1) < 97 && 0 <= i__1 ? i__1 : s_rnge(
		    "mass", i__1, "element_", (ftnlen)1669)]) {
/*<             Z = FLOAT(I) >*/
		*z__ = (real) i__;
/*<             CZ = CZDATA(I) >*/
		s_copy(cz, czdata + (((i__1 = i__ - 1) < 97 && 0 <= i__1 ? 
			i__1 : s_rnge("czdata", i__1, "element_", (ftnlen)
			1671)) << 1), (ftnlen)2, (ftnlen)2);
/*<             GOTO 999 >*/
		goto L999;
/*<           ENDIF >*/
	    }
/*<         ENDDO >*/
	}
/*<       ELSE IF( IOPT .EQ. 6 ) THEN >*/
    } else if (*iopt == 6) {
/*<         CZ = CZDATA(NINT(Z)) >*/
	s_copy(cz, czdata + (((i__1 = i_dnnt(z__) - 1) < 97 && 0 <= i__1 ? 
		i__1 : s_rnge("czdata", i__1, "element_", (ftnlen)1676)) << 1)
		, (ftnlen)2, (ftnlen)2);
/*<         GOTO 999 >*/
	goto L999;
/*<       ENDIF >*/
    }

/*<       WRITE(6,*) ' <E>: Element not forseen ' >*/
    s_wsle(&io___221);
    do_lio(&c__9, &c__1, " <E>: Element not forseen ", (ftnlen)26);
    e_wsle();
/*<       IRC = -1 >*/
    *irc = -1;
/*<  990  CONTINUE >*/
L990:
/*<       A = MASS( NINT(Z) ) >*/
    *a = mass[(i__1 = i_dnnt(z__) - 1) < 97 && 0 <= i__1 ? i__1 : s_rnge(
	    "mass", i__1, "element_", (ftnlen)1683)];
/*<       IF( Z .EQ. 6.6 ) A = MASS(97) >*/
    if (*z__ == 6.6f) {
	*a = mass[96];
    }
/*<       IF( IOPT .EQ. 3 .OR. IOPT .EQ. 4 ) A = NINT(A) >*/
    if (*iopt == 3 || *iopt == 4) {
	*a = (doublereal) i_dnnt(a);
    }

/*<  999  RETURN >*/
L999:
    return 0;
/*<       END >*/
} /* element_ */


/* ______________________________________________________________________ */

/*<       SUBROUTINE UPPERCASE(CARG) >*/
/* Subroutine */ int uppercase_(char *carg, ftnlen carg_len)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer i_len(char *, ftnlen);

    /* Local variables */
    static integer i__, ilen;


/*     transform lowercase characters to uppercase characters */

/*<       CHARACTER*(*) CARG >*/
/*<       INTEGER ILEN, I >*/

/* ********************************************************************* */
/*<       ILEN = LEN(CARG) >*/
    ilen = i_len(carg, carg_len);
/*<       DO I = 1, ILEN >*/
    i__1 = ilen;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<         IF( CARG(I:I) .EQ. 'a' ) CARG(I:I) = 'A' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'a') {
	    *(unsigned char *)&carg[i__ - 1] = 'A';
	}
/*<         IF( CARG(I:I) .EQ. 'b' ) CARG(I:I) = 'B' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'b') {
	    *(unsigned char *)&carg[i__ - 1] = 'B';
	}
/*<         IF( CARG(I:I) .EQ. 'c' ) CARG(I:I) = 'C' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'c') {
	    *(unsigned char *)&carg[i__ - 1] = 'C';
	}
/*<         IF( CARG(I:I) .EQ. 'd' ) CARG(I:I) = 'D' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'd') {
	    *(unsigned char *)&carg[i__ - 1] = 'D';
	}
/*<         IF( CARG(I:I) .EQ. 'e' ) CARG(I:I) = 'E' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'e') {
	    *(unsigned char *)&carg[i__ - 1] = 'E';
	}
/*<         IF( CARG(I:I) .EQ. 'f' ) CARG(I:I) = 'F' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'f') {
	    *(unsigned char *)&carg[i__ - 1] = 'F';
	}
/*<         IF( CARG(I:I) .EQ. 'g' ) CARG(I:I) = 'G' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'g') {
	    *(unsigned char *)&carg[i__ - 1] = 'G';
	}
/*<         IF( CARG(I:I) .EQ. 'h' ) CARG(I:I) = 'H' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'h') {
	    *(unsigned char *)&carg[i__ - 1] = 'H';
	}
/*<         IF( CARG(I:I) .EQ. 'i' ) CARG(I:I) = 'I' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'i') {
	    *(unsigned char *)&carg[i__ - 1] = 'I';
	}
/*<         IF( CARG(I:I) .EQ. 'j' ) CARG(I:I) = 'J' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'j') {
	    *(unsigned char *)&carg[i__ - 1] = 'J';
	}
/*<         IF( CARG(I:I) .EQ. 'k' ) CARG(I:I) = 'K' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'k') {
	    *(unsigned char *)&carg[i__ - 1] = 'K';
	}
/*<         IF( CARG(I:I) .EQ. 'l' ) CARG(I:I) = 'L' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'l') {
	    *(unsigned char *)&carg[i__ - 1] = 'L';
	}
/*<         IF( CARG(I:I) .EQ. 'm' ) CARG(I:I) = 'M' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'm') {
	    *(unsigned char *)&carg[i__ - 1] = 'M';
	}
/*<         IF( CARG(I:I) .EQ. 'n' ) CARG(I:I) = 'N' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'n') {
	    *(unsigned char *)&carg[i__ - 1] = 'N';
	}
/*<         IF( CARG(I:I) .EQ. 'o' ) CARG(I:I) = 'O' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'o') {
	    *(unsigned char *)&carg[i__ - 1] = 'O';
	}
/*<         IF( CARG(I:I) .EQ. 'p' ) CARG(I:I) = 'P' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'p') {
	    *(unsigned char *)&carg[i__ - 1] = 'P';
	}
/*<         IF( CARG(I:I) .EQ. 'q' ) CARG(I:I) = 'Q' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'q') {
	    *(unsigned char *)&carg[i__ - 1] = 'Q';
	}
/*<         IF( CARG(I:I) .EQ. 'r' ) CARG(I:I) = 'R' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'r') {
	    *(unsigned char *)&carg[i__ - 1] = 'R';
	}
/*<         IF( CARG(I:I) .EQ. 's' ) CARG(I:I) = 'S' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 's') {
	    *(unsigned char *)&carg[i__ - 1] = 'S';
	}
/*<         IF( CARG(I:I) .EQ. 't' ) CARG(I:I) = 'T' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 't') {
	    *(unsigned char *)&carg[i__ - 1] = 'T';
	}
/*<         IF( CARG(I:I) .EQ. 'u' ) CARG(I:I) = 'U' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'u') {
	    *(unsigned char *)&carg[i__ - 1] = 'U';
	}
/*<         IF( CARG(I:I) .EQ. 'v' ) CARG(I:I) = 'V' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'v') {
	    *(unsigned char *)&carg[i__ - 1] = 'V';
	}
/*<         IF( CARG(I:I) .EQ. 'w' ) CARG(I:I) = 'W' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'w') {
	    *(unsigned char *)&carg[i__ - 1] = 'W';
	}
/*<         IF( CARG(I:I) .EQ. 'x' ) CARG(I:I) = 'X' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'x') {
	    *(unsigned char *)&carg[i__ - 1] = 'X';
	}
/*<         IF( CARG(I:I) .EQ. 'y' ) CARG(I:I) = 'Y' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'y') {
	    *(unsigned char *)&carg[i__ - 1] = 'Y';
	}
/*<         IF( CARG(I:I) .EQ. 'z' ) CARG(I:I) = 'Z' >*/
	if (*(unsigned char *)&carg[i__ - 1] == 'z') {
	    *(unsigned char *)&carg[i__ - 1] = 'Z';
	}
/*<       ENDDO >*/
    }
/*<       END >*/
    return 0;
} /* uppercase_ */


/* ********************************************************************** */

/*<       DOUBLE PRECISION FUNCTION RHODATA(I) >*/
doublereal rhodata_(integer *i__)
{
    /* Initialized data */

    static doublereal rhotable[113] = { 8.998001e-5,1.787e-4,.5298,1.803,
	    2.351,2.267,.00125,.001428,.001696,9.009e-4,.9702,1.737,2.699,
	    2.322,1.822,2.069,.003165,.001783,.8633,1.341,2.998,4.52,6.102,
	    7.193,7.435,7.867,8.797,8.897,8.951,7.107,5.909,5.338,5.72,4.786,
	    3.401,.003741,1.529,2.6,4.491,6.471,8.604,10.21,11.5,12.18,12.4,
	    11.96,10.47,8.582,7.315,7.283,6.618,6.225,4.939,.005860999,1.899,
	    3.522,6.175,6.673,6.775,7.003,7.22,7.557,5.259,7.903,8.279,8.554,
	    8.821,9.092,9.335,6.979,9.831,13.13,16.60001,19.28999,21.03999,
	    22.57001,22.50999,21.44,19.31,13.56,11.88,11.32,9.813,9.253,0.,
	    .009909,0.,5.023,10.07,11.66,15.4,19.05,1.032,1.,.001528,.00129,
	    8.489,2.32,.00265,7.1573e-4,1.3875,9.2e-4,4.51,3.67,7.93,2.2,1.44,
	    1.1,.003338,.935,2.5,1.2,.00178 };

    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Builtin functions */
    integer s_rnge(char *, integer, char *, integer), s_wsle(cilist *), 
	    do_lio(integer *, integer *, char *, ftnlen), e_wsle(void);

    /* Fortran I/O blocks */
    static cilist io___225 = { 0, 6, 0, 0, 0 };



/*     RETURNS TARGET DENSITIES IN G/CM**2 */
/*     I = 1 TO 92 ARE ELEMENTS, ABOVE COMPOSITES */

/*<       IMPLICIT NONE >*/
/*<       INTEGER I, IMAX >*/
/*<       PARAMETER( IMAX = 113 ) >*/
/*<       DOUBLE PRECISION RHOTABLE(IMAX) >*/
/* *** TARGET DENSITIES IN G/CM**3 */
/*<        >*/

/* *** COMPOSITES */


/*<       IF( I .GT. 0 .AND. I .LE. IMAX ) THEN >*/
    if (*i__ > 0 && *i__ <= 113) {
/*<         RHODATA = RHOTABLE(I) >*/
	ret_val = rhotable[(i__1 = *i__ - 1) < 113 && 0 <= i__1 ? i__1 : 
		s_rnge("rhotable", i__1, "rhodata_", (ftnlen)1776)];
/*<       ELSE >*/
    } else {
/*<         WRITE(6,*) '<E>: No density for this material!' >*/
	s_wsle(&io___225);
	do_lio(&c__9, &c__1, "<E>: No density for this material!", (ftnlen)34)
		;
	e_wsle();
/*<         RHODATA = 1. >*/
	ret_val = 1.f;
/*<       ENDIF >*/
    }
/*<       RETURN >*/
    return ret_val;
/*<       END >*/
} /* rhodata_ */

/* Main program alias */ int rageny_ () { MAIN__ (); return 0; }

/* ################  everything above is from f2c  ##################### */



#include "rageny.h"
#include <gsl/gsl_integration.h>

// use gsl to do integration 
double f (double x, void * params) {
  return dxde_(&x);
}
 

/// do calculation, depending on what is given
/// \return energy loss
double rageny_calc(double ap, double zp, rageny_target_t* t, rageny_data_t* d) {
  int i;
  double eloss;

#define LIM 10000
#define EPSABS 1.e-7
#define EPSREL 1.e-7

  static gsl_integration_workspace* ws = 0;
  double res;
  double abserr;

  gsl_function F;

  memset(&dxdec_1,0,sizeof(dxdec_1));
  dxdec_1.iopt  = 1;  /// use only option for now \todo use other options too
  dxdec_1.ap    = ap;
  dxdec_1.zp    = zp;
  if (t->n > MATMAX) { 
    /// error \todo error 
  }

  for (i = 0; i < t->n; i++ ) {
    dxdec_1.atc[i]= t->a[i];
    dxdec_1.ztc[i]= t->z[i];
    dxdec_1.rtc[i]= t->r[i];
  }

  // gsl work space
  if (!ws) {
    INFO("Allocating integration work space");
    ws = gsl_integration_workspace_alloc ((size_t) LIM);
  }
  // gsl function 
  F.function = &f;
  F.params = 0;
 
  if (0) {
  } else if (d->f == rageny_RANGE ) {
    DEB("RANGE");
    // subtract target thickness
    d->rf = d->ri -  t->t;
    if ( d->rf < 0 ) d->rf = 0;

    // calculate initial energy
    dxdec_1.idedx = 1;
    //    d->ti  = bbsim1_( (D_fp)dxde_, &null, &d->ri, &acc);

    gsl_integration_qag (&F, 0, d->ri, EPSABS, EPSREL, LIM, 
                         GSL_INTEG_GAUSS61, ws, &res, &abserr);
    d->ti = res;

    DEB("res %g",res);
    DEB("abserr %g",abserr);
    DEB("abserr/res %g",abserr/res);

  } else if (d->f == rageny_ENERGY ) {
    DEB("ENERGY");
    // initial range
    dxdec_1.idedx = -1;
    //    d->ri  = bbsim1_( (D_fp)dxde_, &null, &d->ti, &acc);

    gsl_integration_qag (&F, 0, d->ti, EPSABS, EPSREL, LIM, 
                         GSL_INTEG_GAUSS61, ws, &res, &abserr);
    d->ri = res;

    DEB("res %g",res);
    DEB("abserr %g",abserr);
    DEB("abserr/res %g",abserr/res);

    // subtract target thickness
    d->rf = d->ri - t->t;
    if ( d->rf < 0 ) d->rf = 0;

  } else {
    // error!!!
    ERR("Should not happen");
    return 0;
  }

  // calculate final energy
  dxdec_1.idedx = 1;
  //  d->tf  = bbsim1_( (D_fp)dxde_, &null, &d->rf, &acc);
  gsl_integration_qag (&F, 0, d->rf, EPSABS, EPSREL, LIM, 
                       GSL_INTEG_GAUSS61, ws, &res, &abserr);
  d->tf = res;
  DEB("res %g",res);
  DEB("abserr %g",abserr);
  DEB("abserr/res %g",abserr/res);

  d->f = rageny_CALC_DONE;

  eloss = d->ti - d->tf; 
  DEB("eloss %f",eloss);
  return eloss;
}



#ifdef TEST
/* test of some routines */
/* compile and run with 

     cc -DTEST -W -Wall -O2 -g2 rageny.c -lg2c && ./a.out

 */

#include <stdio.h>
#include <string.h>

int main () {
  double null=0;
  double small=.00001;  // accuracy for integration

  double ap=30,zp=12;
  int iopt=1;

  /* pure material 12C */
  double at=12,zt=6,t=50 /* 50 MeV/u */,rg;
  printf("Pure material 12C (60 mg/cm^2) incident by 30Mg @ 50MeV/u\n");
  range_(&ap,&zp,&at,&zt,&t,&rg,&iopt);  // calc range
  printf("range: %f  energy: %f \n",rg,t);
  rg-=60;  // 60 mg/cm^2 target
  energy_(&ap,&zp,&at,&zt,&rg,&t,&iopt); // (re)calc energy
  printf("range: %f  energy: %f \n",rg,t);
  /* agrees with rageny output, but \todo hubert table (iopt=2)do not work */
  
  
  /* 12C again */
  printf("Same thing, just different calculation\n");
  t=50;
  memset(&dxdec_1,0,sizeof(dxdec_1));
  dxdec_1.idedx = -1;
  dxdec_1.iopt  = iopt;
  dxdec_1.ap    = ap;
  dxdec_1.zp    = zp;
  dxdec_1.atc[0]= 12;
  dxdec_1.ztc[0]= 6;
  dxdec_1.rtc[0]= 1;
  
  rg = bbsim1_( (D_fp)dxde_, &null, &t, &small);
  printf("range: %f  energy: %f \n",rg,t);
  rg-=60;
  dxdec_1.idedx = 1;
  t  = bbsim1_( (D_fp)dxde_, &null, &rg, &small);
  printf("range: %f  energy: %f \n",rg,t);
  
  printf("Same beam on 60 mg/cm^2 plastic scintilator\n");
  t=50;
  memset(&dxdec_1,0,sizeof(dxdec_1));
  dxdec_1.idedx = -1;
  dxdec_1.iopt  = iopt;
  dxdec_1.ap    = ap;
  dxdec_1.zp    = zp;
  dxdec_1.atc[0]= 12;
  dxdec_1.atc[1]= 1;
  dxdec_1.ztc[0]= 6;
  dxdec_1.ztc[1]= 1;
  dxdec_1.rtc[0]=10;
  dxdec_1.rtc[1]=11;
  
  rg = bbsim1_( (D_fp)dxde_, &null, &t, &small);
  printf("range: %f  energy: %f \n",rg,t);
  rg-=60;
  dxdec_1.idedx = 1;
  t  = bbsim1_( (D_fp)dxde_, &null, &rg, &small);
  printf("range: %f  energy: %f \n",rg,t);

  {
    double de;
    rageny_data_t d;
    rageny_target_t t;

    // initial data
    d.ti = 50;
    d.f  = rageny_ENERGY;

    // target (12C again)
    t.n = 1;
    t.a[0] = 12;
    t.z[0] = 6;
    t.r[0] = 1;
    t.t =  60;

    de = rageny_calc(30,12,&t,&d);

    printf("Same beam on same 12C, but now with new function call.\n");
    printf("range: %f  energy: %f \n",d.ri,d.ti);
    printf("range: %f  energy: %f  de %f \n",d.rf,d.tf,de);

    // initial data
    d.ti = 50;
    d.f  = rageny_ENERGY;

    // target (12C again)
    t.n = 2;
    t.a[0] = 12;
    t.z[0] = 6;
    t.r[0] = 10;
    t.a[1] = 1;
    t.z[1] = 1;
    t.r[1] = 11;
    t.t =  60;

    de = rageny_calc(30,12,&t,&d);

    printf("Same beam on same Scint, but now with new function call.\n");
    printf("range: %f  energy: %f \n",d.ri,d.ti);
    printf("range: %f  energy: %f  de %f \n",d.rf,d.tf,de);
  }

  return 0;
}
#endif
